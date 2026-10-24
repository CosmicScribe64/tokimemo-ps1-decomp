---
id: T-26000
title: Adopt IDO 4.1's block-copy state rule in tools/ugen_pass.py
status: In Progress
assignee:
created: 2026-10-23
updated: 2026-10-24
links: [[[tickets/T-25608-gate-main-func_8006B0C8]], [[ido-52-evaluation]], [[toolchain]]]
---

## Goal

Replace the block-copy state rule in `tools/ugen_pass.py` with the one IDO 4.1's ugen follows on all 30 test cases (T-25608 check, [[ido-52-evaluation]] "T-25608: 4.1 ugen on the rule candidates"): a procedure starts in `B`, an indirect store sets `F`, a conditional branch comparing two registers sets `B`, nothing else changes the state. The T-25608 rule that matched main func_8006B0C8 is a narrower fit that disagrees with 4.1 in 8 synthetic shapes (none in matched code).

## Acceptance criteria

- [ ] main `strCallback` (T-25303) and `func_8005938C` (T-25147), whose FAKE dead loops were written for the old detector, re-matched under the 4.1 rule (or shown to need another FAKE).
- [ ] The 4.1 rule implemented uniformly with unit tests (the 30 cases as synthetic tests), all 27 sha1 unchanged.
- [ ] CODING_STANDARDS.md 7a documentation updated ([[toolchain]], the pass docstring).

## Notes

Prototype: `drafts/T-25608/ido41/ugen_pass_41rule.py` (on the w16-ido41 branch, merged as notes).

## Comments
- 2026-10-24 (orchestrator): an Opus agent is on it (workflow wf_3d2d513f-e21, worktree w17-ugen41): implement the 4.1 rule, 30 synthetic tests, re-match strCallback and func_8005938C, two reviews, merge.
