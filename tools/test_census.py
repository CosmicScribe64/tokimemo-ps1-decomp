"""Unit tests for tools/census.py classification (T-25015).

Run: python3 tools/test_census.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from census import classify_hunk, canon  # noqa: E402


def main():
    assert classify_hunk(["lui t6,0x8014"], ["nop"]) == "delay-slot"
    assert classify_hunk(["nop"], ["lui t6,0x8014"]) == "delay-slot"
    assert classify_hunk(["addu v0,t5,zero"], ["addu v0,t9,zero"]) == "register-only"
    assert classify_hunk(["addiu sp,sp,-64"], ["addiu sp,sp,-48"]) == "frame"
    assert classify_hunk(["lui v0,D_800E6280"], ["lui v0,D_800E9999"]) == "addressing"
    assert classify_hunk(["sll v0,v0,2"], ["srl v0,v0,2"]) == "instruction"
    assert canon("lw t0,4(sp)") == "lw R,4(R)"
    print("census tests pass")


if __name__ == "__main__":
    main()
