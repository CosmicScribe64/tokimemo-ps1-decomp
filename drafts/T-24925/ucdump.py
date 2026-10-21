#!/usr/bin/env python3
"""Compile the guarded ETC/8013BEF0.c TU with the ucode-dumping uopt shim (T-24925).

Usage (docker):
    UCD_TAG=plain UCD_OUT=drafts/T-24925/ucdump/plain python3 drafts/T-24925/ucdump.py
    UCD_EDIT='switch (*(u8 *)&D_800E6280.unk_110D)' UCD_TAG=byteview ...   # replaces
        the plain selector text `switch (D_800E6280.unk_110D)` in a scratch copy.

UCD_EDIT is a plain-text replacement for the func_8013CCE4 switch selector
(default: none, compile the source as it is).
"""
import os
import sys
from pathlib import Path

sys.path.insert(0, "tools")
import cc  # noqa: E402
import funcloc  # noqa: E402
import funcdiff_verify  # noqa: E402

NAME = "func_8013CCE4"
PLAIN = "switch (D_800E6280.unk_110D)"


def main():
    out = Path(os.environ.get("UCD_OUT", "drafts/T-24925/ucdump/run"))
    tag = os.environ.get("UCD_TAG", "u")
    out.mkdir(parents=True, exist_ok=True)
    c = funcloc.locate(NAME, "ETC")
    source = c.src
    find = os.environ.get("UCD_FIND", PLAIN)
    edit = os.environ.get("UCD_REPLACE", os.environ.get("UCD_EDIT"))
    if edit:
        text = Path(source).read_text()
        start = text.index("s32 func_8013CCE4(void)")
        end = text.index("INCLUDE_ASM", start)
        body = text[start:end]
        if find not in body:
            sys.exit("ucdump.py: %r not found in func_8013CCE4" % find)
        scratch = out / ("scratch_%s.c" % tag)
        scratch.write_text(text[:start] + body.replace(find, edit, 1) + text[end:])
        source = str(scratch)
    pre = funcdiff_verify.preprocess_draft(source, True)
    unit_c = out / ("guarded_%s.c" % tag)
    unit_c.write_text(pre)
    obj = out / ("guarded_%s.o" % tag)

    shim = str(Path(__file__).resolve().parent /
               (os.environ.get("UCD_UOPT_SHIM", "uopt_shim.py")))
    ugen_shim = str(Path(__file__).resolve().parent / "ugen_shim.py")
    orig_env = cc.ido_frame_env

    def environment(ver, tmp, extra_shims=None):
        return orig_env(ver, tmp, {**(extra_shims or {}),
                                   "uopt": shim, "ugen": ugen_shim})

    cc.ido_frame_env = environment
    os.environ["UCODE_DUMP_DIR"] = str(out.resolve())
    os.environ["UCODE_DUMP_TAG"] = tag
    cc.main(["cc.py", str(unit_c), str(obj), "ido", "5.3"])
    print("\n".join(sorted(str(p) for p in out.glob("%s_*.uc" % tag))))


if __name__ == "__main__":
    main()
