# T-16510 m2c defect rewrites of the T-14500 drafts

Rewritten bodies of the selected T-14500 drafts, made by `tools/draft_fixes.py` (ticket T-16510). The
archive in `../T-14500/` is untouched. These are drafts for people who match functions, not matches. A
score of 0 here is one full-translation-unit comparison, with no linked-byte proof and no unit SHA1.

## Contents

- `survey.json`: the detector run over the 1,066 selected bodies of `../T-14500/best-quality-catalog.jsonl`
  (hit counts, why `goto` labels were left alone, and every rewritable or reported hit).
- `results.jsonl`: one line per body that a rewrite changes (50 bodies), best archived score first
  (`rank`). Each line has the archived score and the score of the unmodified body recompiled in the
  current tree (`baseline`). It also has the best score over the rewrites (`score`), the rewrites that gave
  it (`chosen`), every subset that was compiled (`tried`), the hashes of the old and new body, and the hash
  of the tool.
- `<UNIT>/<function>/`: only for rewrites that beat the baseline (34 bodies): `body.c`, `context.c` (the full
  translation unit that was scored) and `result.json` (the same record as in `results.jsonl`).
- `follow-up/GEKO/func_801327AC/`: the one case where a rewrite plus one hand edit reaches score 0.

`baseline` can differ from the archived score. The archived unit of `SHOUGATU:func_8013737C` no longer
compiled (another function of the file was matched after the census), so its unit was rebuilt from the
current source (`context` in its record).

## Result (50 bodies; the first 30 by archived score are ranks 1 to 30)

| | bodies | score 0 | improved | same | worse (original kept) |
|---|---:|---:|---:|---:|---:|
| ranks 1 to 30 | 30 | 3 | 18 | 2 | 7 |
| ranks 31 to 50 | 20 | 0 | 13 | 1 | 6 |
| `sjis-escape` only | 13 | 2 | 8 | 3 | 0 |
| `postinc-compare` (with `goto` for one body) | 11 | 1 | 10 | 0 | 0 |
| `goto-shared-store` only | 26 | 0 | 13 | 0 | 13 |

The three score-0 results (`BUNKAKEN:func_80145844`, `BUNKASAI:func_801431D4`, `SHOUGATU:func_80137144`) are
functions that agents had already matched by hand in T-15300, T-15400 and T-15600; the tool finds the same
fixes. The largest gains are `main:func_80079070` (450 to 35), `GEKO:func_801327AC` (223 to 2),
`BUNKA_SD:func_80139290` (509 to 133), `SHUGAKU:func_8013282C` (183 to 116) and `BUNKASAI:func_8013B25C`
(183 to 122).

`GEKO:func_801327AC` (still `INCLUDE_ASM` in `src/ovl/GEKO/801323E0.c`): the `goto` rewrite leaves one
differing row, a `sb` where the original has `sh`. The initializer in the same file already writes
`*(s16 *)&D_80144C40 = 0;` (T-9510); the same line in this function gives score 0
(`follow-up/GEKO/func_801327AC/`). Promote it only after `funcdiff.py --resolve` and the GEKO SHA1 pass.

## Reproduce

In Docker, from the repository root, at the commit that holds `tools/draft_fixes.py`:

    tools/docker.sh python3 tools/draft_fixes.py survey --archive drafts/T-14500 --json drafts/T-16510/survey.json
    tools/docker.sh python3 tools/draft_fixes.py rescore --archive drafts/T-14500 --top 0 --out drafts/T-16510 --jobs 2

Scores are the resolved full-unit comparison of `tools/exhaustive_drafts.py`; they depend on the headers and
sources of the tree that runs the command.
