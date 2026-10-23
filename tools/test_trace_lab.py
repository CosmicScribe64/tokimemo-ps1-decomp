"""Unit tests for tools/trace_lab.py diff mode (T-25015).

The run mode is verified by integration (it reproduces the T-25013
as1 weights for GEKO func_80136C20); this test covers the diff driver.
Run: python3 tools/test_trace_lab.py
"""
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import trace_lab  # noqa: E402


class Args:
    pass


def main():
    with tempfile.TemporaryDirectory() as td:
        a, b = os.path.join(td, "a"), os.path.join(td, "b")
        os.makedirs(a)
        os.makedirs(b)
        for d, w in ((a, "1"), (b, "1")):
            with open(os.path.join(d, "summary.txt"), "w") as f:
                f.write("weight: %s\n" % w)
        args = Args()
        args.a, args.b = a, b
        assert trace_lab.diff(args) == 0
        with open(os.path.join(b, "summary.txt"), "w") as f:
            f.write("weight: 7\n")
        assert trace_lab.diff(args) == 1
    print("trace_lab tests pass")


if __name__ == "__main__":
    main()
