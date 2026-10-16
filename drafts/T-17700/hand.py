#!/usr/bin/env python3
"""T-17700: score the starting points the redraft does not make, full translation unit like redraft.py.

- guarded functions: the NON_MATCHING body in src (hand-written from a matched sibling or the census draft);
- functions whose redraft does not compile: the census body (drafts/T-14500 or drafts/T-16510).
The better of that and the redraft becomes drafts/T-17700/GEKO/<func>/body.c (`variant` "hand-guarded" or
"census"); result.json keeps the redraft's scores. Run in Docker from the repo root after redraft.py.
"""
import json
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import draft_fixes  # noqa: E402

OUT = Path("drafts/T-17700/GEKO")


def guarded_body(src, func):
    text = Path(src).read_text()
    m = re.search(r"#ifdef NON_MATCHING\n((?:(?!#ifdef NON_MATCHING|#endif).)*?\b%s\(.*?\n\}\n)#else\n" % func, text, re.S)
    return m.group(1) if m else None


def main():
    inv = {r["func"]: r for r in json.loads(Path("drafts/T-17700/inventory.json").read_text())}
    for res in sorted(OUT.glob("*/result.json")):
        rec = json.loads(res.read_text())
        func, row = rec["func"], inv.get(rec["func"])
        if row is None:
            continue
        cand, variant, decls = None, None, res.parent
        if row["guarded"]:
            cand, variant = guarded_body(row["src"], func), "hand-guarded"
        elif rec.get("score") is None and rec.get("census_path"):
            p = Path(rec["census_path"]) / "body.c"
            if p.exists():
                cand, variant, decls = p.read_text(), "census", p.parent
        if not cand:
            continue
        folder = res.parent
        if variant == "hand-guarded":
            # the source file with this function's NON_MATCHING block replaced by its body
            text = Path(row["src"]).read_text()
            m = re.search(r"#ifdef NON_MATCHING\n(?:(?!#ifdef NON_MATCHING|#endif).)*?\b%s\(.*?\n\}\n#else\n[^\n]*\b%s\b[^\n]*\n#endif\n"
                          % (func, func),
                          text, re.S)
            context = text[:m.start()] + cand + text[m.end():] if m else None
        else:
            context = draft_fixes.fresh_context(dict(row), cand, str(decls))
        if context is None:
            print(func, variant, "no context")
            continue
        work = Path("build/t17700-hand") / func
        work.mkdir(parents=True, exist_ok=True)
        score = draft_fixes.make_scorer(dict(row))(context, work)
        print(func, variant, score, "redraft", rec.get("score"), flush=True)
        key = "hand_guarded_score" if variant == "hand-guarded" else "census_rescore"
        if score >= 10 ** 6 or (rec.get("score") is not None and score >= rec["score"]):
            rec[key] = None if score >= 10 ** 6 else score
        else:
            (folder / "body.c").write_text(cand)
            (folder / "context.c").write_text(context)
            rec.update(variant=variant, score=score, status="candidate-zero" if score == 0 else "scored")
            rec[key] = score
            cs = rec.get("census_score")
            rec["vs_census"] = None if cs is None else ("better" if score < cs else "same" if score == cs else "worse")
        res.write_text(json.dumps(rec, sort_keys=True) + "\n")


if __name__ == "__main__":
    main()
