#!/usr/bin/env python3
"""Generate the machine-readable systems.json and events.json catalogs.

Unlike components.json / modules.json -- which the engine emits at runtime from
the self-registering LuaComponentRegistry / LuaModuleRegistry (see
src/Lua/LuaApiManifest.cpp) -- systems and events carry NO runtime metadata:
the ECS erases the queried-component template pack once a SystemWrapper is built
(src/ECS/Registry.h), and the EventBus is purely template-dispatched with no
central type registry (src/EventBus/EventBus.h). There is nothing to introspect.

So these two catalogs are produced by static inspection of the source tree:

  * systems.json -- one entry per src/Systems/*.h, enriched with the registration
    tier ("serial" / "parallel" / "bulk") and queried components parsed from the
    Register*System<...>() calls in Game::Setup, plus the events each system
    emits / subscribes to and whether it exposes a Lua binding surface.
  * events.json  -- one entry per event struct in src/Events/*.h (name + payload
    fields), with every EmitEvent<> and SubscribeEvent<> call site in the tree.

Output is deterministic (sorted, fixed formatting) so it can be drift-gated in CI
exactly like the Lua artifacts: regenerate, then `git diff --quiet`. Run with no
arguments to (re)write both files at the repo root; pass --check to fail (exit 1)
if either file would change without writing.
"""

import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SRC = REPO / "src"

# Register{,Parallel,Bulk}System<Components...>(SystemName()) -- the optional
# <...> holds the queried components (no nested templates in practice), and the
# constructor call names the system. \s* spans the multi-line DopplerSystem case.
REG_RE = re.compile(r"Register(Parallel|Bulk)?System\s*(?:<([^>]*)>)?\s*\(\s*([A-Za-z_]\w*)\s*\(")
EMIT_RE = re.compile(r"EmitEvent\s*<\s*([A-Za-z_]\w*)\s*>")
SUB_RE = re.compile(r"SubscribeEvent\s*<\s*([A-Za-z_]\w*)\s*,\s*([A-Za-z_]\w*)\s*>")
EVENT_DECL_RE = re.compile(r"\b(struct|class)\s+([A-Za-z_]\w*)\s*:\s*(?:public\s+)?Event\b")
FIELD_RE = re.compile(r"^\s+([A-Za-z_][\w:]*(?:\s*<[^>]*>)?)\s+([A-Za-z_]\w*)\s*;\s*$")

TIER = {"": "serial", "Parallel": "parallel", "Bulk": "bulk"}


def rel(path: Path) -> str:
    return path.relative_to(REPO).as_posix()


def all_src_files():
    for ext in ("*.h", "*.hpp", "*.cpp"):
        yield from SRC.rglob(ext)


def parse_registrations():
    """system name -> {tier, order, queried_components} from Game::Setup."""
    text = (SRC / "Game" / "Game.cpp").read_text(encoding="utf-8")
    out = {}
    for order, m in enumerate(REG_RE.finditer(text)):
        kind, comps, name = m.group(1) or "", m.group(2), m.group(3)
        out[name] = {
            "tier": TIER[kind],
            "order": order,
            "queried_components": [c.strip() for c in comps.split(",")] if comps and comps.strip() else [],
        }
    return out


def parse_event_sites():
    """Collect every emit and subscribe call site across the tree."""
    emits = {}  # event -> list[{file,line}]
    subs = {}   # event -> list[{file,line,owner}]
    for path in all_src_files():
        for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            for m in EMIT_RE.finditer(line):
                emits.setdefault(m.group(1), []).append({"file": rel(path), "line": lineno})
            for m in SUB_RE.finditer(line):
                subs.setdefault(m.group(2), []).append({"file": rel(path), "line": lineno, "owner": m.group(1)})
    return emits, subs


def parse_events(emits, subs):
    events = []
    for path in sorted((SRC / "Events").glob("*.h")):
        lines = path.read_text(encoding="utf-8").splitlines()
        decl = next((m for line in lines if (m := EVENT_DECL_RE.search(line))), None)
        if not decl:
            continue
        name = decl.group(2)
        fields, in_body = [], False
        for line in lines:
            if EVENT_DECL_RE.search(line):
                in_body = True
                continue
            if in_body and "(" in line:  # reached the constructor -- fields come first
                break
            if in_body and (fm := FIELD_RE.match(line)):
                fields.append({"type": fm.group(1).strip(), "name": fm.group(2)})
        events.append({
            "name": name,
            "kind": decl.group(1),
            "source": rel(path),
            "fields": fields,
            "emit_sites": sorted(emits.get(name, []), key=lambda s: (s["file"], s["line"])),
            "subscribe_sites": sorted(subs.get(name, []), key=lambda s: (s["file"], s["line"])),
        })
    return sorted(events, key=lambda e: e["name"])


def has_lua_surface(name: str) -> bool:
    return any((SRC / "Lua").rglob(f"{name}LuaBinding.*"))


def parse_systems(regs, emits, subs):
    # event -> systems that emit/subscribe it, attributed by the source file's stem.
    emit_by_system, sub_by_system = {}, {}
    for event, sites in emits.items():
        for s in sites:
            stem = Path(s["file"]).stem
            if stem.endswith("System"):
                emit_by_system.setdefault(stem, set()).add(event)
    for event, sites in subs.items():
        for s in sites:
            if s["owner"].endswith("System"):
                sub_by_system.setdefault(s["owner"], set()).add(event)

    systems = []
    for path in sorted((SRC / "Systems").glob("*.h")):
        name = path.stem
        reg = regs.get(name)
        systems.append({
            "name": name,
            "source": rel(path),
            "tier": reg["tier"] if reg else None,
            "setup_order": reg["order"] if reg else None,
            "queried_components": reg["queried_components"] if reg else [],
            "emits": sorted(emit_by_system.get(name, [])),
            "subscribes": sorted(sub_by_system.get(name, [])),
            "lua_surface": has_lua_surface(name),
        })
    return systems


def render(obj) -> str:
    return json.dumps(obj, indent=2) + "\n"


def main() -> int:
    check = "--check" in sys.argv[1:]
    regs = parse_registrations()
    emits, subs = parse_event_sites()

    payloads = {
        REPO / "systems.json": {
            "_generated_by": "scripts/gen_api_catalogs.py",
            "_note": "Static catalog of src/Systems. Regenerate with scripts/gen_api_catalogs.py.",
            "systems": parse_systems(regs, emits, subs),
        },
        REPO / "events.json": {
            "_generated_by": "scripts/gen_api_catalogs.py",
            "_note": "Static catalog of src/Events. Regenerate with scripts/gen_api_catalogs.py.",
            "events": parse_events(emits, subs),
        },
    }

    drifted = []
    for path, payload in payloads.items():
        new = render(payload)
        old = path.read_text(encoding="utf-8") if path.exists() else None
        if old != new:
            drifted.append(rel(path))
            if not check:
                path.write_text(new, encoding="utf-8")

    if check and drifted:
        print("API catalogs are stale: " + ", ".join(drifted), file=sys.stderr)
        print("Run scripts/gen_api_catalogs.py and commit the result.", file=sys.stderr)
        return 1
    if not check:
        for path in payloads:
            print(f"wrote {rel(path)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
