#!/usr/bin/env python3
"""Reproduce the T-24014 TACO PRE operand-order tradeoff in Docker.

Run from the repository root: python3 drafts/T-24014/probe.py
Only ignored build/t24014/repro output is written.
"""
from pathlib import Path
import sys

sys.path.insert(0, "tools")
import funcdiff
import order_search


def main():
    name = "func_8014A480"
    context = funcdiff.locate(name, "TACO")
    source = Path(context.src).read_text()
    start = source.index("s32 " + name + "(")
    end = source.index("\n#else", start)
    body = source[start:end]
    arrow = "(sel + D_8015EDB4)->"
    dot = "D_8015EDB4[sel]."
    anchor = "    if (" + arrow + "unk84[1] <"
    seed = body.replace(anchor,
        "    /* FAKE (T-24014): discarded read probes PRE expression selection. */\n"
        "    if (" + dot + "unk84[1]) {}\n" + anchor, 1)
    variants = [("baseline", body), ("all-dot", body.replace(arrow, dot)),
                ("empty-dot-read", seed)]
    score = order_search.make_text_scorer(context, name, resolve=True)
    for label, candidate in variants:
        text = source[:start] + candidate + source[end:]
        work = Path("build/t24014/repro") / label
        work.mkdir(parents=True, exist_ok=True)
        (work / "full.c").write_text(text)
        print(label, score(text, work, label), flush=True)


if __name__ == "__main__":
    main()
