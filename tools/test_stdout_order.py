#!/usr/bin/env python3
"""Keeps the result line of `unittest discover` last when the output goes through a pipe.

Several tests call tool entry points that print to stdout. Piped, stdout is block-buffered and flushed at
exit, after unittest wrote `OK` to stderr, so `... discover -s tools -p 'test_*.py' 2>&1 | tail -3 | grep '^OK'`
(the merge check) saw chatter instead of the result, depending on how many bytes the tests printed.
Importing this module while discovery loads the tests switches stdout to line buffering, which keeps the
order of the lines. It defines no test.
"""
import sys

try:
    sys.stdout.reconfigure(line_buffering=True)
except (AttributeError, ValueError):        # a replaced stdout (capturing runner): nothing to do
    pass
