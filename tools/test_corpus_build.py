"""Unit tests for tools/corpus_build.py (T-25015).

Run: python3 tools/test_corpus_build.py
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from corpus_build import split_functions  # noqa: E402

SRC = """#include "common.h"

extern s32 D_12345678;

void func_80011111(void) {
    D_12345678 = 1;
}

void func_80022222(void)
{
    INCLUDE_ASM("asm/main/nonmatchings/x", func_80022222);
}

s32 func_80033333(s32 a) {
    return a + 1;
}
"""


def main():
    fns = split_functions(SRC)
    names = [n for n, _, _ in fns]
    assert names == ["func_80011111", "func_80033333"], names
    header = fns[0][1]
    assert '#include "common.h"' in header and "extern s32" in header
    assert "D_12345678 = 1;" in fns[0][2]
    assert "return a + 1;" in fns[1][2]
    print("corpus_build tests pass")


if __name__ == "__main__":
    main()
