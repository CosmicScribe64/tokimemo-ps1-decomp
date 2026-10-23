#!/usr/bin/env python3
"""Scratch ugen shim (T-25122 diagnostic): save uopt's output ucode, then run the project's ugen pass."""
import os, shutil, sys
argv = sys.argv[1:]
real = argv[1]; args = argv[2:]
VALUED = ("-o", "-G", "-t", "-temp", "-Olimit", "-d", "-u", "-l", "-k")
i = 0
while i < len(args):
    if args[i] in ("-G", "-o", "-t", "-temp", "-f", "-l", "-Olimit"): i += 2
    elif args[i].startswith("-"): i += 1
    else: shutil.copy(args[i], "/scratch/cap.O"); break
os.execvp("python3", ["python3", "/work/tools/ugen_pass.py", "--ugen", real] + args)
