---
id: T-26000
title: Adopt IDO 4.1's block-copy state rule in tools/ugen_pass.py
status: In Review
assignee: w17-ugen41 (Opus)
created: 2026-10-23
updated: 2026-10-24
links: [[[tickets/T-25608-gate-main-func_8006B0C8]], [[ido-52-evaluation]], [[toolchain]]]
---

## Goal

Replace the block-copy state rule in `tools/ugen_pass.py` with the one IDO 4.1's ugen follows on all 30 test cases (T-25608 check, [[ido-52-evaluation]] "T-25608: 4.1 ugen on the rule candidates"): a procedure starts in `B`, an indirect store sets `F`, a conditional branch comparing two registers sets `B`, nothing else changes the state. The T-25608 rule that matched main func_8006B0C8 is a narrower fit that disagrees with 4.1 in 8 synthetic shapes (none in matched code).

## Acceptance criteria

- [x] main `strCallback` (T-25303) and `func_8005938C` (T-25147), whose FAKE dead loops were written for the old detector, re-matched under the 4.1 rule (or shown to need another FAKE).
- [x] The 4.1 rule implemented uniformly with unit tests (the 30 cases as synthetic tests), all 27 sha1 unchanged.
- [x] CODING_STANDARDS.md 7a documentation updated ([[toolchain]], the pass docstring).

## Notes

Prototype: `drafts/T-25608/ido41/ugen_pass_41rule.py` (on the w16-ido41 branch, merged as notes).

## Comments

### 2026-10-24, w17-ugen41 (Opus): implemented, In Review

- `tools/ugen_pass.py`: the 4.1 rule (`copy_states`: B at entry, F at `Uistr`/`Uistv`, B at a `Ufjp`/`Utjp` whose compare has a non-constant right operand, `Ulnot` and `Urstr` skipped, a branch with no condition record raises PassError). The T-25608 lock (`_is_signed_blt_pos_imm_jump`, the armed/sticky state) is gone. Tests: `tools/test_ugen_pass.py`, the 30 cases as synthetic ucode (all match 4.1; the old rule misses the 8 that T-25608 listed) plus the rule's parts. End to end, the real uopt output of the 30 case files gives 4.1's state in all 30.
- Census (full rebuild, 566 procedures with a `Umov`): against the T-25608 rule only main `strCallback` (F to B) and `func_8005938C` (FFF to BBF) change; main fails, 26/27. `func_8006B0C8` stays BBBB.
- Re-steered without new FAKEs: `strCallback` test as `if ((D_800B5900 & 1) && D_80125C5C != 0)` (no `odd` local), `func_8005938C` test as `if (r->active && (r->flags & 0x80))`. Before, uopt wrote `odd != 0` and `r->active != 0` as `ldc 0; x; neq`, a register compare that reset B after the arm. Both arms stay, re-marked FAKE T-26000 (75 and 62 rows without them). Both bodies also match under the old rule.
- Obsolete FAKEs removed: the empty tests of main `func_80054FC0` (T-25105) and TACO `func_80145650` (T-25136) only fed the old lock; both MATCH without them.
- Finding (IDO 4.1, [[ido-52-evaluation]] "T-26000"): 4.1 also sets F at a branch on a value (`and`, `ilod`, bit fields, `shr`, ...; 5 new case files). With the respelled tests 4.1 gives F for `strCallback` and FFF for `func_8005938C` without the arms. Not modelled: it would put `func_8006B0C8`'s compaction copy (after a one-bit field test) in F, where the original is B. Follow-up candidate: measure the 4-byte copy's state in 4.1 and stored compares, then decide whether the value-branch F can be added.
- Proof: clean rebuild 27/27 sha1 OK; `tools.test_ugen_pass` OK (24 tests); full suite 11 failures + 1 error, the same as main (test_build_deps, test_protoverride); check_headers, migrate_globals --check, sync_protos --check-branch OK; funcdiff `--resolve --version default` MATCH for func_8006B0C8, strCallback, func_8005938C, func_80054FC0, func_80145650.

