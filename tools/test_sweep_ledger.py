"""Unit tests for tools/sweep_ledger.py (T-25015).

Run: python3 tools/test_sweep_ledger.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sweep_ledger import record, exhausted  # noqa: E402


def main():
    data = {}
    record(data, "func_80136C20", "shape_search", 236, 15)
    assert exhausted(data, "func_80136C20", "shape_search", 200)
    assert exhausted(data, "func_80136C20", "shape_search", 236)
    assert not exhausted(data, "func_80136C20", "shape_search", 500)
    assert not exhausted(data, "func_80136C20", "permuter", 100)
    assert not exhausted(data, "func_8006A044", "shape_search", 100)
    print("sweep_ledger tests pass")


if __name__ == "__main__":
    main()
