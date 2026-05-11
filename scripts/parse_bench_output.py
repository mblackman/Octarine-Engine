#!/usr/bin/env python3
import sys
import re
import json
import statistics
from collections import defaultdict

# Regex to match lines like:
# [2026-05-11 16:11:25.038] [info] TIMER: Registry::Update (total): 5.060000ms
# [2026-05-11 16:11:25.044] [info] ACCUM: Sweep Bipartite: 4.119000ms
pattern = re.compile(r'\] \[info\] (TIMER|ACCUM):\s+(.*?):\s+([0-9.]+)(ms|ns|s)')

metrics = defaultdict(list)

for line in sys.stdin:
    match = pattern.search(line)
    if match:
        metric_type = match.group(1) # TIMER or ACCUM
        name = match.group(2).strip()
        value = float(match.group(3))
        unit = match.group(4)
        
        # Normalize to ms for simplicity, though the engine mostly outputs ms
        if unit == 's':
            value *= 1000
        elif unit == 'ns':
            value /= 1000000
            
        metrics[name].append(value)

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
