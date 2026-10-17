# Redraft tool validation (T-20800)

Output of `tools/redraft.py` on ten unmatched functions in three units (GEKO, DATE2, main), made at `w2-good` plus the tool. Each folder holds the best draft the tool found: `body.c`, `declarations.h`, `context.c` (the whole translation unit that was scored) and `result.json`. A score is the number of diff rows of the function in the full unit, the measure of `drafts/T-14500`; 0 would be a candidate, not a verified match. None of the ten reached 0. `catalog.jsonl` has one line per function.

```
tools/docker.sh python3 tools/redraft.py --ticket T-20800 --base <git head> \
  GEKO:func_80133024 GEKO:func_80133598 GEKO:func_80144198 GEKO:func_80134228 \
  DATE2:func_801388C0 DATE2:func_80136860 DATE2:func_801330D4 \
  main:func_80044E8C main:func_8005CA94 main:func_80054140
```

Default variants, `--jobs 2`. The columns are the score of each variant; "fail" means no compile, "-" means the variant did not apply (`guarded` only runs for a guarded function).

| Function | Bytes | Census | Best archived (ticket) | plain | typed | typed-late | archived | guarded | Result | Variant |
|---|---|---|---|---|---|---|---|---|---|---|
| `DATE2:func_801330D4` | 388 | 121 | 113 (T-18800) | 131 | 113 | 113 | 113 | - | **113** | typed |
| `DATE2:func_80136860` | 560 | 186 | 164 (T-18800) | 186 | 192 | 188 | 164 | - | **164** | archived |
| `DATE2:func_801388C0` | 380 | 81 | 57 (T-18800) | 88 | 57 | 57 | 57 | - | **57** | typed |
| `GEKO:func_80133024` | 280 | 94 | 46 (T-17700) | 94 | 47 | 46 | 46 | - | **46** | typed-late |
| `GEKO:func_80133598` | 452 | 112 | 111 (T-17700) | 112 | 111 | 111 | 111 | - | **111** | typed |
| `GEKO:func_80134228` | 244 | 25 | 16 (T-17700) | 25 | 39 | 35 | 16 | 16 | **8** | archived + gs-local |
| `GEKO:func_80144198` | 216 | 97 | 69 (T-17700) | 99 | 69 | 69 | 69 | - | **62** | typed + gs-local |
| `main:func_80044E8C` | 264 | 41 | 41 (T-17000) | 41 | 41 | 41 | 41 | - | **41** | typed |
| `main:func_80054140` | 324 | 120 | 106 (T-17000) | 120 | 106 | 106 | 106 | - | **48** | typed + gs-local |
| `main:func_8005CA94` | 428 | 122 | 61 (T-17000) | 122 | 61 | 126 | 61 | - | **61** | typed |

Reading the table:

- Against the census every function is the same or better (9 of 10 better, `main:func_80044E8C` the same).
- Against the best archived draft, three are better (`GEKO:func_80144198` 69 to 62, `GEKO:func_80134228` 16 to 8, `main:func_80054140` 106 to 48; all through the `GameState *g` form that the tool tries on the winner), six are the same and one is an archived body the new drafts do not reach (`DATE2:func_80136860`, 164 against 188 at best).
- The archived bodies still compile against today's headers and score what they scored when they were made, so the fresh typed drafts mostly reproduce them. The rewrites are the same ones.
- The fold order changes the result for four of the ten. `typed-late` is better than `typed` in three (by 4 rows on `GEKO:func_80134228`, 4 on `DATE2:func_80136860`, 1 on `GEKO:func_80133024`) and much worse on `main:func_8005CA94` (126 against 61). That is why both run by default; `typed` wins six ties by being first in the list.
- `draft_fixes` found nothing to rewrite in these ten.

The run was repeated from an empty work directory on the final code and gave the same scores for all ten.

A second run covered the smallest unmatched function of each of the 25 units (TT left out) with `typed`, `typed-late`, `plain` and `guarded`, no draft_fixes: all 25 functions scored and no worker failed. Of the 88 variant runs, 87 scored; `typed-late` did not compile on `TAIIKU:func_8013B78C` (m2c wrote `D_801499A0.unk_06.unk0` for a halfword). The hand-written `NON_MATCHING` body in `src/` scored at least as well as any m2c draft for all 13 guarded functions in that set and strictly better for 10, which is why `guarded` is a default variant. Those results are not kept.

The run folders (`m2c.c`, diagnostics, the compiled-object cache) stay under `build/redraft/T-20800`, which is not committed.
