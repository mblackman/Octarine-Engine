#!/usr/bin/env python3
"""Parse TIMER/ACCUM lines from an OctarineEngine profiling run and emit
percentile-based JSON metrics suitable for github-action-benchmark.

Usage:
    scripts/bench.sh release-profile 8 | python3 scripts/parse_bench_output.py [--warmup SECONDS]

Options:
    --warmup SECONDS   Discard all TIMER/ACCUM lines from the first SECONDS of
                       the run (default: 2.0). This excludes the ramp-up transient
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
timestamp_pattern = re.compile(r'\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})\]')
metric_pattern = re.compile(r'\] \[info\] (TIMER|ACCUM):\s+(.*?):\s+([0-9.]+)(ms|ns|s)')


def parse_timestamp(ts_str):
    """Parse a spdlog timestamp string into a datetime object."""
    return datetime.strptime(ts_str, '%Y-%m-%d %H:%M:%S.%f')


def main():
    parser = argparse.ArgumentParser(description='Parse OctarineEngine profiling output.')
    parser.add_argument('--warmup', type=float, default=2.0,
                        help='Seconds of initial output to discard (default: 2.0)')
    args = parser.parse_args()

    metrics = defaultdict(list)
    first_timestamp = None
    discarded = 0

    for line in sys.stdin:
        match = metric_pattern.search(line)
        if not match:
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

        name = match.group(2).strip()
        value = float(match.group(3))
        unit = match.group(4)

        # Normalize to ms for simplicity, though the engine mostly outputs ms
        if unit == 's':
            value *= 1000
        elif unit == 'ns':
            value /= 1000000

        metrics[name].append(value)

    if discarded > 0:
        print(f"Discarded {discarded} warmup samples ({args.warmup}s)", file=sys.stderr)

    output = []

    for name, values in metrics.items():
        if not values:
            continue

        # Sort values to calculate percentiles
        values.sort()

        n = len(values)

        # Calculate P95, P99, and Max
        p95_idx = int(n * 0.95)
        p99_idx = int(n * 0.99)

        # Ensure indices are within bounds
        p95_idx = min(max(p95_idx, 0), n - 1)
        p99_idx = min(max(p99_idx, 0), n - 1)

        p95_val = values[p95_idx]
        p99_val = values[p99_idx]
        max_val = values[-1]

        extra_info = f"Samples: {n}"

        output.append({
            "name": f"{name} [p95]",
            "unit": "ms",
            "value": round(p95_val, 4),
            "range": "0",
            "extra": extra_info
        })

        output.append({
            "name": f"{name} [p99]",
            "unit": "ms",
            "value": round(p99_val, 4),
            "range": "0",
            "extra": extra_info
        })

        output.append({
            "name": f"{name} [max]",
            "unit": "ms",
            "value": round(max_val, 4),
            "range": "0",
            "extra": extra_info
        })

    # Print the JSON array to stdout so it can be captured by the GitHub action
    print(json.dumps(output, indent=2))


if __name__ == '__main__':
    main()
