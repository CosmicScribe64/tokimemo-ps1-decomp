#!/usr/bin/env python3
"""T-18500: score hand drafts through the full-unit path of redraft.py (copied from drafts/T-18300/hand.py).

    python3 drafts/T-18500/hand.py                    # every BUNKAKEN/<func>/hand.c: keep the better of hand
                                                      # draft and redraft in result.json (`hand_score`)
    python3 drafts/T-18500/hand.py --try FUNC FILE    # print the score of one body, write nothing

Run inside the project Docker image from the repo root.
"""
import hashlib
import json
import shutil
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import draft_fixes  # noqa: E402

OUT = Path("drafts/T-18500")
WORK = OUT / "_handwork"


def score_body(row, body, name):
    # the hand draft uses only header declarations (the redraft's declarations.h may clash with them)
    context = draft_fixes.fresh_context(dict(row), body, WORK)
    if not context:
        return None, None
    return draft_fixes.make_scorer(dict(row))(context, WORK / name), context


def main():
    rows = {r["func"]: r for r in json.loads((OUT / "inventory.json").read_text())}
    if len(sys.argv) == 4 and sys.argv[1] == "--try":
        score, _ = score_body(rows[sys.argv[2]], Path(sys.argv[3]).read_text(), sys.argv[2])
        print(sys.argv[2], score, flush=True)
        shutil.rmtree(WORK, ignore_errors=True)
        return
    for hand in sorted(OUT.glob("BUNKAKEN/*/hand.c")):
        folder = hand.parent
        row = rows[folder.name]
        body = hand.read_text()
        score, context = score_body(row, body, folder.name)
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
    shutil.rmtree(WORK, ignore_errors=True)  # compiled scratch objects stay out of git


if __name__ == "__main__":
    main()
