"""Check a real application run against independent original-game captures.

Run in Docker. Actual inputs/reports stay ignored; no original values live here.
"""

import argparse
import json
from pathlib import Path


def check(proof_path, fixture_path, native_path):
    proof = json.loads(Path(proof_path).read_text())
    fixture = json.loads(Path(fixture_path).read_text())
    native = json.loads(Path(native_path).read_text())
    final = proof["final_state"]
    actual = final["rule_state"]
    activity = fixture["expected"]["0"]["rule_state"]
    original = next(route for route in native["routes"] if route["command"] == 0)
    comparisons = {
        "first_result_boundary": [final["game_mode"], actual["day_event_index"]] == [0x31, 1],
        "native_parameter_words": actual["params"] == original["parameter_words_after"],
        "native_queue": actual["day_events"] == activity["day_events"],
        "native_all_girls": actual["girls"] == activity["girls"],
        "native_date": all(actual[key] == activity[key] for key in ("year", "month", "day", "weekday", "day_count", "calendar")),
        "native_rng_endpoint": proof["rng"]["state"] == fixture["expected"]["0"]["rng_state"],
        "native_actor_exit": actual["actor_state"] == {},
        "real_actor_draws": any(any(item.get("kind") == "native_actor_draw" for item in row["presentation"]) for row in proof["trace"]),
        "no_continuation": proof["trace"][-1]["mode"] == 0x31,
        "all_step_hashes": all(len(row["state_hash"]) == 64 and len(row["event_hash"]) == 64 for row in proof["trace"]),
    }
    print(json.dumps({"scope": "actual app command0 state/effect checkpoints; service latency and complete screenshot parity excluded", "comparisons": comparisons}, indent=2))
    if not all(comparisons.values()):
        raise SystemExit(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("proof")
    parser.add_argument("fixture")
    parser.add_argument("native")
    args = parser.parse_args()
    check(args.proof, args.fixture, args.native)
