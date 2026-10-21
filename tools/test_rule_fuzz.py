"""Unit tests for tools/rule_fuzz.py helpers (T-25015).

Run: python3 tools/test_rule_fuzz.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rule_fuzz import knob_env, parse_beds  # noqa: E402


def main():
    env = knob_env({}, [(404, 0), (12, 1)])
    assert env["TOKIMEMO_OPTN"] == "404:0,12:1", env
    assert knob_env({}, [])["TOKIMEMO_OPTN"] == ""
    beds = parse_beds(["h,b,r,func_80011111"])
    assert beds[0]["fn"] == "func_80011111"
    print("rule_fuzz tests pass")


if __name__ == "__main__":
    main()
