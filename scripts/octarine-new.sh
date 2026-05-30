#!/usr/bin/env bash
# Scaffold a new Octarine project: project.ini + config.ini + game.lua + asset dirs.
# Usage:
#   octarine-new.sh --name "My Game" --package-id com.studio.mygame --dir /path/to/out
#
# After scaffolding, build a packaged release with:
#   cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=<dir>
#   cmake --build --preset ship-release --target package
#
# Or run from a debug build:
#   ./build/editor-debug/bin/debug/OctarineEngine <dir>
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: octarine-new.sh --name <display-name> --package-id <reverse-dns> --dir <output-dir>

Required:
  --name        Human-readable game title (e.g. "My Game")
  --package-id  Reverse-DNS bundle identifier (e.g. com.studio.mygame)
  --dir         Output directory (must not already exist or must be empty)

Optional:
  --version-name   semver string (default: 0.1.0)
  --version-code   integer build code (default: 1)
  -h, --help       show this message
EOF
}

name=""
package_id=""
dir=""
version_name="0.1.0"
version_code="1"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --name) name="$2"; shift 2 ;;
    --package-id) package_id="$2"; shift 2 ;;
    --dir) dir="$2"; shift 2 ;;
    --version-name) version_name="$2"; shift 2 ;;
    --version-code) version_code="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "unknown arg: $1" >&2; usage >&2; exit 2 ;;
  esac
done

[[ -z "$name" ]]       && { echo "--name required" >&2; exit 2; }
[[ -z "$package_id" ]] && { echo "--package-id required" >&2; exit 2; }
[[ -z "$dir" ]]        && { echo "--dir required" >&2; exit 2; }

if [[ ! "$package_id" =~ ^[a-z][a-z0-9_]*(\.[a-z][a-z0-9_]*)+$ ]]; then
  echo "--package-id must be reverse-DNS (e.g. com.studio.mygame): got '$package_id'" >&2
  exit 2
fi

if [[ -e "$dir" ]]; then
  if [[ -d "$dir" ]] && [[ -z "$(ls -A "$dir")" ]]; then
    :
  else
    echo "--dir already exists and is not empty: $dir" >&2
    exit 1
  fi
fi

mkdir -p "$dir/assets" "$dir/THIRD_PARTY_LICENSES.d"

cat > "$dir/project.ini" <<EOF
# Game identity for packaging. Read by desktop CPack, Android Gradle, iOS Info.plist (later).
# Single source of truth across every platform. Flat key=value, no sections.

name         = ${name}
package_id   = ${package_id}
version_name = ${version_name}
version_code = ${version_code}
EOF

cat > "$dir/config.ini" <<EOF
# Engine runtime config. Loaded at startup before game.lua.
Title=${name}
LogLevel=info
StartupScript=game.lua
DefaultScalingMode=nearest
DefaultWindowWidth=1280
DefaultWindowHeight=720
EOF

# shellcheck disable=SC2016
cat > "$dir/game.lua" <<EOF
-- ${name} — entry point loaded via config.ini StartupScript.
log("Hello from ${name}!")
EOF

cat > "$dir/THIRD_PARTY_LICENSES.d/README.txt" <<'EOF'
Drop additional third-party attribution files (one .txt per dependency) into this
directory. Packaged builds append them to the engine's shipped THIRD_PARTY_LICENSES.txt.
EOF

cat > "$dir/.gitignore" <<'EOF'
# Engine-generated runtime/editor state
imgui.ini
editor_prefs.ini
preferences.ini
*_prefs.ini

# Asset bake outputs + caches
asset_manifest.lua
*.meta.cache

# Local build dirs
build/
.cache/
EOF

cat > "$dir/README.md" <<EOF
# ${name}

Built with the Octarine engine.

## Run (dev)

\`\`\`sh
OctarineEngine $(pwd)/$(basename "$dir")
\`\`\`

## Package a release

\`\`\`sh
cmake --preset ship-release -DOCTARINE_PACKAGE_PROJECT=\$(pwd)
cmake --build --preset ship-release --target package
\`\`\`

Output bundles land under \`build/ship-release/\`.
EOF

echo "Scaffolded '${name}' at ${dir}"
