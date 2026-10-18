#!/usr/bin/env python3
"""Reproduce the T-24009 ENDING pointer-lifetime candidate in Docker.

Run from the repository root: python3 drafts/T-24009/probe.py
Only build/t24009/repro is written; production source remains unchanged.
"""
from pathlib import Path
import re
import sys

sys.path.insert(0, "tools")
import funcdiff
import order_search


def main():
    name = "func_8013A2E8"
    context = funcdiff.locate(name, "ENDING")
    source = Path(context.src).read_text()
    start = source.index("void " + name + "(void)")
    end = source.index("\n#else", start)
    body = source[start:end]
    call = re.search(r'func_800AE0F0\(D_800CA280, ("[^"\n]*")\);', body)
    if call is None:
        raise SystemExit("first string call not found")
    literal = call.group(1)
    candidate = body.replace("s32 pad;", "char *pad;", 1)
    candidate = candidate.replace(
        "unused word, the frame has 16 bytes above msg and the declared locals fill 13",
        "existing pad slot reused for an experimental string pointer lifetime")
    anchor = "    if (D_800E6280.unk_0F6.b[1] & 1) {"
    candidate = candidate.replace(anchor,
        "    pad = (char *)" + literal + ";\n"
        "    /* FAKE (T-24009): empty test changes pointer scheduling. */\n"
        "    if (pad) {}\n" + anchor, 1)
    candidate = candidate.replace(call.group(0),
        "func_800AE0F0(D_800CA280, (char *)pad);", 1)
    candidate = source[:start] + candidate + source[end:]
    score = order_search.make_text_scorer(context, name, resolve=True)
    for label, text in [("baseline", source), ("pointer-lifetime", candidate)]:
        work = Path("build/t24009/repro") / label
        work.mkdir(parents=True, exist_ok=True)
        (work / "full.c").write_text(text)
        print(label, score(text, work, label), flush=True)


if __name__ == "__main__":
    main()
