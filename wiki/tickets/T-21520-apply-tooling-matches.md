---
id: T-21520
title: Apply the tooling-round matches
status: Done   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: tr-apply
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-20810-viewsweep]]", "[[tickets/T-20820-keep-proof]]", "[[tickets/T-20830-autotweak-recipes]]", "[[keep-lines]]", "[[game-state]]"]
---

## Goal

The tooling round (T-20810, T-20820, T-20830) found eight verified matches and two necessity proofs on the tree before the shared-type round (T-20900..T-20940) merged. Apply them to `src/` on the current tree, re-deriving each against the current types.

Targets:

- autotweak recipes (T-20830, `drafts/T-20830/README.md`): main `func_80072D70`, DATE `func_8013AF1C`, ETC `func_80143F24`, GYOZI `func_8013E638`, NAME_ENT `func_8013BE18`. Not TT `func_8013D2E0` (TT belongs to the contributor diman8) and not DATE `func_80155004` (already plain C).
- viewsweep (T-20810): NAME_ENT `func_80134B94`, `drafts/T-20810/patches/NAME_ENT_func_80134B94.patch`.
- keep proofs (T-20820, `drafts/T-20820/README.md`, `results.txt`): TACO `func_8014616C` and `func_80149538` (PROVEN). Add the `keep` lines the proof supports to `config/migrate_globals.txt`, and the C of the proof.

## Acceptance criteria

- [x] Each function: C in place of its `INCLUDE_ASM` or guarded draft, `funcdiff.py --resolve UNIT:func` says MATCH, unit sha1 OK, one commit `T-21520: match UNIT:func (via T-208x0)` (8 of 8).
- [x] A function that does not match on the current tree within about 15 minutes stays as it was; the reason is recorded under Notes (none failed).
- [x] Tricks (gstate pointer, chain stores, grouped lines, implicit-int view, kept symbol) carry a `FAKE` comment with ticket and reason (CODING_STANDARDS sections 7 and 9).
- [x] No edit to `src/ovl/TT/*` or the main `dialogue_render`.
- [x] Clean `rm -rf build && python3 configure.py && ninja` gives 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` pass.
- [x] Code review against `CODING_STANDARDS.md`, findings resolved, then Done (no findings).

## Notes

Rules: Docker only (`tools/docker.sh`), never push, other worktrees untouched.

Result: 8 applied, 0 failed. Every function MATCHes with literal bytes (`funcdiff.py --resolve`); each is one commit.

| function | bytes | via | form | FAKE mark |
| --- | --- | --- | --- | --- |
| main `func_80072D70` | 1,064 | T-20830 | `GameState *g` pointer; `D_800E7D10.unk_000` (retired scalar view of the work area) | pointer |
| DATE `func_8013AF1C` | 600 | T-20830 | `GameState *g` pointer | pointer |
| ETC `func_80143F24` | 188 | T-20830 | `GameState *g` pointer, three-byte chain store | pointer and chain |
| GYOZI `func_8013E638` | 236 | T-20830 | init, empty test and `do` on one line | grouped line (joins the existing FAKE) |
| NAME_ENT `func_8013BE18` | 348 | T-20830 | `GameState *g` pointer | pointer |
| NAME_ENT `func_80134B94` | 804 | T-20810 | `s32 func_800AE090();` implicit-int view, `s32` result with no `return` | override define and function |
| TACO `func_8014616C` | 1,512 | T-20820 | final `D_800E7384 += 1;`, keep line `proof=func_8014616C:165:bb8762f0` | tail |
| TACO `func_80149538` | 692 | T-20820 | final `D_800E7384 += 1;`, keep line `proof=func_80149538:33:d7dd7f6c` | tail |

Total 5,444 bytes. Skipped on purpose: TT `func_8013D2E0` (contributor diman8), DATE `func_80155004` (already plain C).

Findings while applying:

- All five recipe bodies and the viewsweep patch applied on the current tree; only `D_800E7D10 & 0xFF` needed the new `Work800E7D10` field (`.unk_000`). The proof numbers and sha1s of both TACO functions are unchanged on the current tree.
- NAME_ENT `func_80134B94`: the viewsweep patch alone left an unmarked `s32` result with no `return`. Tested as `void`: one `move v0,zero` row lost (ninja sha1 fails), so the `s32` stays with its own `FAKE` comment.
- The TACO tails use `D_800E7384 += 1;` (the proof tool printed `D_800E7384 = D_800E7384 + 1;`; both link to the same bytes, the shorter form was checked). `extern s32 D_800E7384;` is back in `include/main_api.h` (the `keep` rule needs the declaration). Keep-line marking follows the recommendation of [[keep-lines]] (a `FAKE` comment at the use). The rule text in `CODING_STANDARDS.md` and the [[game-state]] paragraph are still the lead's decision; if the lead rejects the rule, the two lines go and the two functions return to their guarded drafts.
- `migrate_globals.py --verify-proofs` was not run on the whole config; `keepproof.py check UNIT:func --symbol D_800E7384` on the committed files says PROVEN for both.

## Comments

- 2026-10-17: Code review against `CODING_STANDARDS.md`, In Review to Done. No findings. Checked: all eight functions are plain C (no `asm`, `volatile`, `register`, `//` or `#pragma` in the added lines) and `funcdiff.py --resolve` says MATCH with the byte counts of the table (1,064 / 600 / 188 / 236 / 348 / 804 / 1,512 / 692 = 5,444); every guarded `NON_MATCHING` block and its `INCLUDE_ASM` fallback is gone with its stale comment; every trick has a `FAKE` comment with reason and ticket (gstate pointer in four functions, the chain store in `func_80143F24`, the grouped line in `func_8013E638`, the `func_800AE090` implicit-int override define and the `s32` result of `func_80134B94`, the `D_800E7384` tail in both TACO functions); the added declarations sit at block top (C89); `include/main_api.h` gains one declaration (`extern s32 D_800E7384;`) with the reason in a comment, and `tools/check_headers.py` says headers OK; the two `keep` lines in `config/migrate_globals.txt` carry `proof=func:rows:sha1` and the proof comment, and `migrate_globals.py --check` (globals OK), `migrate_globals.py --verify-proofs` (both PROVEN: sufficiency yes, necessity yes, 165 and 33 rows) and `keepproof.py check ... --symbol D_800E7384` agree; `sync_protos.py --check-branch` OK (0 new disagreements). No edit under `src/ovl/TT/` and none to the main `dialogue_render`; no game data, drafts or binaries in the diff (only `src/`, `include/`, `config/`, `wiki/`); eleven commits each start with the ticket id and carry the trailer. Open point, not a finding of this ticket: the `keep`-rule sentence for `CODING_STANDARDS.md` is the lead's decision ([[keep-lines]]); until then the two lines stand on the existing section 8 allowance (a use that matches only through the old symbol is listed with its reason in `config/migrate_globals.txt`).
