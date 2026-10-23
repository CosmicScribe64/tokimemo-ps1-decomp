#!/usr/bin/env python3
"""Scratch (T-25301): compile like tools/cc.py but with the zcopy-off cvt_pass variant as the uopt shim."""
import os, sys, json, shutil, tempfile
sys.path.insert(0, "/work/tools")
import cc
src, out = sys.argv[1], sys.argv[2]
orig = cc.ido_frame_env
cc.ido_frame_env = lambda v, tmp, extra=None: orig(v, tmp, {"uopt": "/work/build/t25301/cvt_zc.py"})
cc.main(["cc.py", src, out, "ido", "5.3"])
