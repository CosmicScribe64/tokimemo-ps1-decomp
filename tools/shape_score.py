"""Relocation-aware funcdiff scorer (packet metric).

Normalizes funcdiff rows so a reference object assembled from a splat
.s and a compiled candidate compare on instruction structure: the raw
hex word is dropped, and when an instruction row is followed by an
R_MIPS relocation row, the folded immediate in the instruction text is
replaced by the relocation's symbol, mirroring tools/score.py in the
project's scratch history.
"""
import difflib
import re
import sys

import funcdiff  # noqa: E402

_HEX = re.compile(r"^[0-9a-f]{8}\s+")
_RELOC = re.compile(r"^R_MIPS_\S+\s+(\S+)$")
_LUI = re.compile(r"^(lui\s+\S+?),\S+$")
_MEM = re.compile(r"^(.*\s+\S+),-?\d+\((\S+)\)$")
_ADDIU = re.compile(r"^(addiu\s+\S+,\S+),-?\d+$")


def norm_rows(obj, fn):
    raw = funcdiff.functions(obj, [fn])[fn]
    out = []
    i = 0
    while i < len(raw):
        line = raw[i].split("#")[0].strip()
        i += 1
        if not line:
            continue
        if line.startswith("R_MIPS"):
            continue  # consumed with the previous text row when present
        text = _HEX.sub("", line)
        sym = None
        if i < len(raw):
            m = _RELOC.match(raw[i].split("#")[0].strip())
            if m:
                sym = m.group(1)
        if sym:
            t = _LUI.sub(r"\1," + sym, text)
            if t == text:
                t = _MEM.sub(lambda mm: mm.group(1) + "," + sym + "(" + mm.group(2) + ")", text)
            if t == text:
                t = _ADDIU.sub(r"\1," + sym, text)
            text = t
        text = re.sub(r"<[^>]*>", "", text)
        out.append(re.sub(r"\s+", " ", text).strip())
    return out


def diff_rows(ref_obj, cand_obj, fn):
    a = norm_rows(ref_obj, fn)
    b = norm_rows(cand_obj, fn)
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal")
