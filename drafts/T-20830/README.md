# T-20830 drafts

Source-only results of the first use of the recipe edit kinds. The command was `tools/autotweak.py --all --only-kinds gstate,group,chain,inline_multi --time 30 --score-jobs 2`. It ran over the 101 guarded drafts of the w2-good tree and over 16 wave-3 archive drafts (`drafts/T-17106`, `T-17110`, `T-17113`, `T-17115`, `T-17116`, `T-17203`, `T-17204`, `T-17207`, `T-19009`, `T-19022`, `T-19027`, `T-19030`).

The runs used scratch copies of the worktree. The archive drafts were put under `#ifdef NON_MATCHING` in their `src/` file. Eight of them did not compile at first and got the externs and types their `declarations.h` or notes name, before the guard. This ticket changed no `src/` and no `include/` file, so the matches below are for the lead to apply.

- `<UNIT>/<func>/body.c` is a function text that compiled to the original bytes. autotweak's own check passed (ninja object, `funcdiff --resolve`, unit sha1). The six new ones were then applied together in a scratch copy. The full build gave 27/27 sha1 OK, and `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` passed (0 new disagreements, 15 known). None carries a new `FAKE` comment (GYOZI `func_8013E638` keeps the one its draft has for the empty counter test); the lead decides how to comment them.
- `<UNIT>/<func>/best.c` is the best text that did not match, for the drafts whose score fell. It replaces the draft's definition and nothing else.
- `DATE/func_80155004/body.c` is the plain-C function already in `src/ovl/DATE/80154260.c`, found again from the archived draft of T-17203 (a chain assignment plus the initial assignments on the `do` line). It checks the edit kinds and is not a new match.

Scores are the stripped single-function score, then the exact full-unit score where autotweak printed it. The edits are the labels of `tools/recipes.py`.

| draft | size | stripped score | exact | edits | set |
| --- | --- | --- | --- | --- | --- |
| main:func_80072D70 | 1064 B | 355 -> 0 | match (exact) | gstate all last | 101 guarded |
| DATE:func_8013AF1C | 600 B | 495 -> 0 | match (exact) | gstate all | 101 guarded |
| ETC:func_80143F24 | 188 B | 283 -> 0 | match (exact) | chain join L6-8 210, gstate all | 101 guarded |
| GYOZI:func_8013E638 | 236 B | 90 -> 0 | match (exact) | group head L7 (2) | 101 guarded |
| NAME_ENT:func_8013BE18 | 348 B | 121 -> 0 | match (exact) | gstate all | 101 guarded |
| TT:func_8013D2E0 | 224 B | 105 -> 0 | match (stripped) | inline_multi v L7 use1 | 101 guarded |
| TT:func_8013E498 | 520 B | 1005 -> 580 | exact 198 -> 62 | chain join L16-17 | 101 guarded |
| DATE:func_8013B488 | 808 B | 699 -> 325 | exact 75 -> 54 | gstate all last | 101 guarded |
| TACO:func_80134A18 | 884 B | 685 -> 344 | exact 36 -> 13 | chain split L10 rtl | 101 guarded |
| TT:func_801374C0 | 404 B | 274 -> 18 | exact 60 -> 4 | chain join L24-26 210, group head L15 (3) | 101 guarded |
| main:func_80077CA8 | 392 B | 1247 -> 1107 | exact 28 -> 20 | gstate all | 101 guarded |
| main:func_80062210 | 468 B | 234 -> 130 | exact 24 -> 8 | group chunks4 L9-16 | 101 guarded |
| main:func_800764AC | 408 B | 203 -> 99 | exact 54 -> 26 | gstate all | 101 guarded |
| TACO:func_801484E4 | 392 B | 285 -> 195 | exact 12 -> 9 | gstate all last | 101 guarded |
| TT:func_80136AD8 | 396 B | 175 -> 85 | exact 58 -> 30 | chain join L25-26 | 101 guarded |
| TT:func_801392A0 | 1060 B | 105 -> 45 | exact 22 -> 18 | chain join L95-97 210 | 101 guarded |
| RENSYU:func_80133628 | 900 B | 730 -> 675 | exact 42 -> 20 | gstate first3 | 101 guarded |
| OPTION:func_80139E3C | 200 B | 470 -> 430 | exact 60 -> 46 | inline_multi g L20 use2 | 101 guarded |
| TT:func_80142570 | 384 B | 170 -> 130 | exact 56 -> 44 | chain join L14-15 10 | 101 guarded |
| BUNKA_SD:func_8013987C | 148 B | 270 -> 240 | exact 14 -> 10 | group head L7 (2) | 101 guarded |
| GEKO:func_80134228 | 244 B | 140 -> 120 | exact 16 -> 8 | gstate all | 101 guarded |
| GEKO:func_80135AC8 | 252 B | 140 -> 120 | exact 16 -> 8 | gstate all | 101 guarded |
| OPTION:func_80139F8C | 132 B | 136 -> 116 | exact 11 -> 5 | gstate all | 101 guarded |
| TT:func_80146000 | 548 B | 40 -> 20 | exact 14 -> 8 | inline_multi b L27 use1 | 101 guarded |
| NAME_ENT:func_801460D8 | 452 B | 220 -> 205 | exact 40 -> 34 | inline_multi temp_a1 L16 use1 | 101 guarded |
| SHUGAKU:func_801352B0 | 384 B | 294 -> 284 | exact 37 -> 33 | gstate all | 101 guarded |
| NAME_ENT:func_8013B994 | 380 B | | skipped | setup: invalid base: instruction shape differs from target ( | 101 guarded |
| TAIIKU:func_8014357C | 664 B | | skipped | setup: invalid base: instruction shape differs from target ( | 101 guarded |
| DATE:func_80155004 | 368 B | 385 -> 0 | match (exact) | chain join L23-25 210, group head L22 (3) | archive |
| TEL:func_80136724 | 7044 B | 10685 -> 7638 | exact 1401 -> 984 | gstate all last | archive |
| DATE:func_801449D4 | 1180 B | 714 -> 414 | exact 138 -> 48 | gstate all | archive |
| TACO:func_8013B460 | 1328 B | 305 -> 228 | exact 82 -> 52 | gstate all | archive |
| RPG_BAT:func_8014BFE4 | 892 B | 3495 -> 3435 | exact 128 -> 126 | group head L40 (2) | archive |
| DATE:func_80152E34 | 396 B | 663 -> 618 | exact 148 -> 144 | gstate all last | archive |
| TACO:func_80144094 | 924 B | 1759 -> 1739 | exact 252 -> 248 | group head L23 (2) | archive |
