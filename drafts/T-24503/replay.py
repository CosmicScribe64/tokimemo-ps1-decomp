#!/usr/bin/env python3
"""Score the unmatched GEKO and OPTION source checkpoints, in Docker from the repo root.

This compiles complete translation units into ignored build/T-24503-replay;
production source files are never changed. Costs are aligned insert/delete edits,
not counts of differing instruction words and not literal linked-byte proofs.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, "tools")
import funcloc
import order_search


def main():
    for unit, name in (("OPTION", "func_8013A7EC"), ("OPTION", "func_801326BC"),
                       ("GEKO", "func_80135DC0"), ("GEKO", "func_80142C9C")):
        source = funcloc.locate(name, unit)
        text = Path(source.src).read_text()
        body = (Path(__file__).parent / unit / name / "body.c").read_text()
        pattern = r'INCLUDE_ASM\("[^\"]+",\s*' + name + r'\);'
        text, replaced = re.subn(pattern, lambda match: body, text)
        if replaced != 1:
            raise SystemExit(name + ": expected one asm fallback; checkpoint needs rebasing")
        score = order_search.make_text_scorer(source, name, resolve=True, non_matching=False)
        cost = score(text, Path("build/T-24503-replay") / name, name)
        print("%s:%s aligned cost %d (unmatched checkpoint)" % (unit, name, cost), flush=True)


if __name__ == "__main__":
    main()
