#!/usr/bin/env python3
"""Scratch diagnostic (T-25301, not a pipeline change): cvt_pass with zcopy off for every procedure."""
import sys
sys.path.insert(0, "/work/tools")
import cvt_pass as cp
cp._assigns_switch_temp_from_unsigned_global = lambda insns, s, e: True
cp.main(sys.argv)
