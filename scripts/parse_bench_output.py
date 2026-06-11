#!/usr/bin/env python3
"""Parse TIMER/ACCUM/COUNTER lines from an OctarineEngine profiling run and emit
percentile-based JSON metrics suitable for github-action-benchmark.

Usage:
    scripts/bench.sh player-profile 8 | python3 scripts/parse_bench_output.py [--warmup SECONDS]

Options:
    --warmup SECONDS   Discard all TIMER/ACCUM/COUNTER lines from the first SECONDS
                       of the run (default: 2.0). This excludes the ramp-up transient
                       where entities are still spawning and the scene hasn't
                       reached steady state.
"""
import sys
import re
import json
import argparse
from collections import defaultdict
from datetime import datetime

# Regex to match lines like:
# [2026-05-11 16:11:25.038] [info] TIMER: Registry::Update (total): 5.060000ms
# [2026-05-11 16:11:25.044] [info] ACCUM: Sweep Bipartite: 4.119000ms
# [2026-05-11 16:11:25.044] [info] COUNTER: Entities: User: 3383
timestamp_pattern = re.compile(r'\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})\]')
metric_pattern = re.compile(r'\] \[info\] (TIMER|ACCUM):\s+(.*?):\s+([0-9.]+)(ms|ns|s)')
counter_pattern = re.compile(r'\] \[info\] COUNTER:\s+(.*?):\s+(\d+)\s*$')


def parse_timestamp(ts_str):
    """Parse a spdlog timestamp string into a datetime object."""
    return datetime.strptime(ts_str, '%Y-%m-%d %H:%M:%S.%f')


def percentile(sorted_values, fraction):
    """Value at the given fraction (0..1) of an already-sorted list."""
    idx = int(len(sorted_values) * fraction)
    idx = min(max(idx, 0), len(sorted_values) - 1)
    return sorted_values[idx]


def main():
    parser = argparse.ArgumentParser(description='Parse OctarineEngine profiling output.')
    parser.add_argument('--warmup', type=float, default=2.0,
                        help='Seconds of initial output to discard (default: 2.0)')
    args = parser.parse_args()

    # Timer series keyed by (kind, name) so ACCUM twins of a TIMER scope can be
    # dropped after parsing instead of silently merging into one distribution.
    timers = defaultdict(list)
    counters = defaultdict(list)
    first_timestamp = None
    discarded = 0

    for line in sys.stdin:
        match = metric_pattern.search(line)
        counter_match = None if match else counter_pattern.search(line)
        if not match and not counter_match:
            continue

        # Extract timestamp for warmup filtering
        ts_match = timestamp_pattern.search(line)
        if ts_match:
            ts = parse_timestamp(ts_match.group(1))
            if first_timestamp is None:
                first_timestamp = ts

            elapsed = (ts - first_timestamp).total_seconds()
            if elapsed < args.warmup:
                discarded += 1
                continue

        if counter_match:
            counters[counter_match.group(1).strip()].append(int(counter_match.group(2)))
            continue

        kind = match.group(1)
        name = match.group(2).strip()
        value = float(match.group(3))
        unit = match.group(4)

        # Normalize to ms for simplicity, though the engine mostly outputs ms
        if unit == 's':
            value *= 1000
        elif unit == 'ns':
            value /= 1000000

        # Game::Render (total) and EndScene are dominated by the SDL dummy-driver's
        # SDL_RenderClear CPU memset under SDL_VIDEODRIVER=dummy and don't reflect
        # real GPU cost. Drop them so the CI dashboard tracks actionable CPU phases only.
        if name in ('Game::Render (total)', 'EndScene'):
            continue

        timers[(kind, name)].append(value)

    if discarded > 0:
        print(f"Discarded {discarded} warmup samples ({args.warmup}s)", file=sys.stderr)

    # Registry::Update wraps every system in both a TIMER scope and an ACCUM scope, and the
    # accumulator map gets dumped to the log each frame. Where both kinds exist for one name,
    # keep the direct TIMER measurement; ACCUM-only names (e.g. collision sub-phases that only
    # run on worker threads) still flow through.
    timer_names = {name for (kind, name) in timers if kind == 'TIMER'}
    metrics = {
        name: values
        for (kind, name), values in timers.items()
        if kind == 'TIMER' or name not in timer_names
    }

    output = []

    for name, values in metrics.items():
        if not values:
            continue

        values.sort()
        extra_info = f"Samples: {len(values)}"

        for label, fraction in (('p50', 0.50), ('p95', 0.95), ('p99', 0.99)):
            output.append({
                "name": f"{name} [{label}]",
                "unit": "ms",
                "value": round(percentile(values, fraction), 4),
                "range": "0",
                "extra": extra_info
            })

        output.append({
            "name": f"{name} [max]",
            "unit": "ms",
            "value": round(values[-1], 4),
            "range": "0",
            "extra": extra_info
        })

    # Per-frame workload counters (entity counts, collision pairs, render-queue size, ...).
    # These let the dashboard distinguish "the work got slower" from "there was more work".
    # Always-zero series are skipped: scenes that never exercise a counter would otherwise
    # add flatline noise to the dashboard.
    for name, values in counters.items():
        if not values or max(values) == 0:
            continue

        values.sort()
        extra_info = f"Samples: {len(values)}"

        output.append({
            "name": f"{name} [p50]",
            "unit": "count",
            "value": percentile(values, 0.50),
            "range": "0",
            "extra": extra_info
        })

        output.append({
            "name": f"{name} [max]",
            "unit": "count",
            "value": values[-1],
            "range": "0",
            "extra": extra_info
        })

    # Print the JSON array to stdout so it can be captured by the GitHub action
    print(json.dumps(output, indent=2))


if __name__ == '__main__':
    main()
