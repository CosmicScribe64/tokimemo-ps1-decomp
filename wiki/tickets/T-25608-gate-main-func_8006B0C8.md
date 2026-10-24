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

- 2026-10-23 (w16-ido41): IDO 4.1's ugen run on the two candidate `tools/ugen_pass.py` rules (rule A: no jump
  between the store and the `<` branch; rule B: constants of 2 or more). Verdict: neither. 4.1 agrees with each
  rule on some of 24 new synthetic cases and with neither on all of them. For `func_8006B0C8`, 4.1 gives B for
  the three swap copies, as both rules do. 4.1's own rule fits all 30 cases: an indirect store sets F, and only
  a branch that compares two registers sets B. As a ucode pass it gives 26/27 binaries; only main `strCallback`
  and `func_8005938C` fail, and both have FAKE dead loops written for the build rule. Details:
  [[ido-52-evaluation]] "T-25608: 4.1 ugen on the rule candidates"; harness `drafts/T-25608/ido41/`.
