#!/usr/bin/env python3
"""T-29002: compare ordered projected JSONL traces and report first divergence.

Each line is an object with iteration (integer), phase, source, and state.
The complete record is compared, including input/events if supplied. These
records are projections prepared by capture adapters, never inferred RAM state.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys


def reject_float(value: str):
    raise ValueError(f"trace values must use exact integers, found {value}")


def load_trace(path: Path) -> list[dict]:
    rows = []
    previous = -1
    with path.open() as stream:
        for line_number, line in enumerate(stream, 1):
            row = json.loads(line, parse_float=reject_float, parse_constant=reject_float)
            if not isinstance(row, dict) or not {"iteration", "phase", "source", "state"} <= row.keys():
                raise ValueError(f"{path}:{line_number}: missing trace fields")
            iteration = row["iteration"]
            if type(iteration) is not int or iteration < previous:
                raise ValueError(f"{path}:{line_number}: iteration must be monotonic integer")
            if not isinstance(row["phase"], str) or not isinstance(row["source"], str) or not isinstance(row["state"], dict):
                raise ValueError(f"{path}:{line_number}: invalid phase/source/state")
            previous = iteration
            rows.append(row)
    if not rows:
        raise ValueError(f"{path}: empty traces cannot establish parity")
    return rows


def first_difference(expected, actual, path: str = "$"):
    if type(expected) is not type(actual):
        return path, expected, actual
    if isinstance(expected, dict):
        for key in sorted(set(expected) | set(actual)):
            if key not in expected or key not in actual:
                return f"{path}.{key}", expected.get(key, "<missing>"), actual.get(key, "<missing>")
            difference = first_difference(expected[key], actual[key], f"{path}.{key}")
            if difference:
                return difference
    elif isinstance(expected, list):
        for index, (left, right) in enumerate(zip(expected, actual)):
            difference = first_difference(left, right, f"{path}[{index}]")
            if difference:
                return difference
        if len(expected) != len(actual):
            return f"{path}.length", len(expected), len(actual)
    elif expected != actual:
        return path, expected, actual
    return None


def compare(expected: list[dict], actual: list[dict]):
    for index, (left, right) in enumerate(zip(expected, actual)):
        difference = first_difference(left, right)
        if difference:
            path, before, after = difference
            return {"record": index, "iteration": left["iteration"], "phase": left["phase"],
                    "source": left["source"], "field": path, "expected": before, "actual": after}
    if len(expected) != len(actual):
        return {"record": min(len(expected), len(actual)), "field": "$.trace.length",
                "expected": len(expected), "actual": len(actual)}
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("expected", type=Path)
    parser.add_argument("actual", type=Path)
    args = parser.parse_args()
    expected = load_trace(args.expected)
    actual = load_trace(args.actual)
    mismatch = compare(expected, actual)
    if mismatch:
        print(json.dumps({"equal": False, "first_divergence": mismatch}, sort_keys=True))
        return 1
    print(json.dumps({"equal": True, "records": len(expected)}))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (ValueError, OSError) as error:
        sys.exit(str(error))
