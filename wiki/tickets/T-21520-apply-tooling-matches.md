---
id: T-21520
title: Apply the tooling-round matches
status: In Progress   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
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

- [ ] Each function: C in place of its `INCLUDE_ASM` or guarded draft, `funcdiff.py --resolve UNIT:func` says MATCH, unit sha1 OK, one commit `T-21520: match UNIT:func (via T-208x0)`.
- [ ] A function that does not match on the current tree within about 15 minutes stays as it was; the reason is recorded under Notes.
- [ ] Tricks (gstate pointer, chain stores, grouped lines, implicit-int view, kept symbol) carry a `FAKE` comment with ticket and reason (CODING_STANDARDS sections 7 and 9).
- [ ] No edit to `src/ovl/TT/*` or the main `dialogue_render`.
- [ ] Clean `rm -rf build && python3 configure.py && ninja` gives 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` pass.
- [ ] Code review against `CODING_STANDARDS.md`, findings resolved, then Done.

## Notes

Rules: Docker only (`tools/docker.sh`), never push, other worktrees untouched.

## Comments
