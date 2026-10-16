#!/usr/bin/env python3
"""T-18000: score the hand drafts (`SHUGAKU/<func>/hand.c`) through the full-unit path of redraft.py and keep
the better of hand draft and redraft in result.json (`hand_score`; `variant: hand` when it wins).

Run inside the project Docker image from the repo root:  python3 drafts/T-18000/hand.py
"""
import hashlib
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import draft_fixes  # noqa: E402

OUT = Path("drafts/T-18000")


def main():
    rows = {r["func"]: r for r in json.loads((OUT / "inventory.json").read_text())}
    for hand in sorted(OUT.glob("SHUGAKU/*/hand.c")):
        folder = hand.parent
        row = rows[folder.name]
        body = hand.read_text()
        # the hand draft uses only header declarations (the redraft's declarations.h may clash with them)
        context = draft_fixes.fresh_context(dict(row), body, OUT / "_work")
        score = draft_fixes.make_scorer(dict(row))(context, OUT / "_work" / folder.name) if context else None
        res = folder / "result.json"
        rec = json.loads(res.read_text()) if res.exists() else dict(scope=row["scope"], func=row["func"])
        rec["hand_score"] = score
        if score is not None and score < 10 ** 6 and (rec.get("score") is None or score < rec["score"]):
            rec.update(score=score, variant="hand", status="candidate-zero" if score == 0 else "scored",
                       draft_hash=hashlib.sha256(body.encode()).hexdigest())
            (folder / "body.c").write_text(body)
            (folder / "context.c").write_text(context)
            c = rec.get("census_score")
            rec["vs_census"] = None if c is None else ("better" if score < c else "same" if score == c else "worse")
        res.write_text(json.dumps(rec, sort_keys=True) + "\n")
        print(folder.name, score, rec.get("score"), rec.get("census_score"), flush=True)


if __name__ == "__main__":
    main()
