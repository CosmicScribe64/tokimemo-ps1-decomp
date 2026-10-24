---
id: T-25608
title: Finish gate - main func_8006B0C8
status: Done
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
- 2026-10-23 (orchestrator): main func_8006B0C8 matched (1,268 bytes) in three fresh-start lenses, each with a change to tools/ugen_pass.py. Two candidate rules (A: a fjp/tjp between the indirect store and the locking branch cancels the F lock; B: compares against 1 do not lock). The user approved an IDO 4.1 download to decide: 4.1's ugen gives B for the three swap copies, as the original and both rules do, but on 30 synthetic cases it agrees with neither rule everywhere (A misses 8, B misses 8, the old rule 7) and refutes B's premise (compares against 1 keep F in 4.1). Merged rule A without the ujp reset (the version 4.1 agrees with more), with its known limits in the pass docstring, and opened T-26000 to adopt 4.1's simpler rule once strCallback and func_8005938C are re-steered. Orchestrator review against CODING_STANDARDS.md 7 and 7a: uniform rule, docstring and wiki/toolchain.md updated, unit tests added (tools/test_ugen_pass.py), corpus unchanged except this function, exceptions written down; C side has two marked FAKEs (unused frame words, the swap label). Merge checks: clean build 27/27, tool unit tests, check_headers, migrate_globals --check, sync_protos --check-branch, funcdiff --resolve --version default MATCH, no INCLUDE_ASM.
