---
id: T-25608
title: Finish gate - main func_8006B0C8
status: Ready
assignee: finish gate
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25600-finish-gate]], [[playbook]]]
---

## Goal

Match main:func_8006B0C8 (318 words, `src/main/800674B0.c`) through the finish gate ([[tickets/T-25600-finish-gate]]): lenses, synthesis, two reviews, up to three rounds. Earlier work: every `drafts/*/main/func_8006B0C8/` folder and its NOTES.md.

## Acceptance criteria

- [ ] Literal default-C match, 27/27, both reviews approve; or after the last round a verdict naming the blocker.

## Notes

If an earlier wave matches it before its turn, this ticket closes as not needed.

## Comments

- 2026-10-23 (fresh r1, as1 lens, branch f-b0c8-r1-as1-line-order-plus-): MATCH, 1,268 B default C, 27/27 clean. `while (i < n)` with `i++` after the swap fixes the as1 order (90 to 82 rows). The 82 rows were the block-copy state: forcing `BBBB` matches, and the pass gave `BFFF`. `tools/ugen_pass.py` now locks F only when no `fjp`/`tjp` lies between the indirect store and the signed jump; the census changes no other procedure. Needs review of the pass change (CODING_STANDARDS 7a) before Done. Notes: `drafts/T-25608/main/func_8006B0C8/fresh-r1-as1-line-order-plus-permuter/NOTES.md`.
