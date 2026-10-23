#!/usr/bin/env python3
"""Scratch as1 shim (T-25122 diagnostic): save ugen's binasm, then run the project's frame pass."""
import os, shutil, sys
argv = sys.argv[1:]
real = argv[1]; args = argv[2:]
i, valued = 0, ("-G", "-o", "-t", "-temp")
while i < len(args):
    if args[i] in valued: i += 2
    elif args[i].startswith("-"): i += 1
    else: shutil.copy(args[i], "/scratch/cap.G"); i += 1
os.execvp("python3", ["python3", "/work/tools/frame_pass.py", "--as1", real] + args)
