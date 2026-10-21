"""Unit tests for tools/precedent.py scanning (T-25015).

Run: python3 tools/test_precedent.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from precedent import scan_function  # noqa: E402

BODY = """
    if (flag) {}
    x = y * 1 == z;
    goto out;
    switch (x) { case 1: break; }
    b = *(u8 *) &w;
    s32 t = D_800E6280.unk_03F;
out:
    return;
"""
HEADER = "extern volatile s32 D_800E6280;\n"


def main():
    hits = scan_function(BODY, HEADER)
    for k in ("empty-if", "mul1", "goto", "switch", "byte-view", "field-copy", "volatile-global"):
        assert k in hits, (k, hits)
    assert scan_function("    return;", "") == {}
    print("precedent tests pass")


if __name__ == "__main__":
    main()
