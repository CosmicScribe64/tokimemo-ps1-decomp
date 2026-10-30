---
id: T-39006
title: Replace fabricated production event state with original rules
status: Ready
assignee: Codex
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-adversarial-review]]", "[[port-testing]]"]
---

## Goal

Resolve review R1, R2 and R13: production event handlers must consume actual saved player/heroine state and original rules, including companion activities and cancellation. Synthetic completion cannot certify gameplay.

## Acceptance criteria

- [ ] Inventory every fabricated roster, fixed reward and handler substitution in production dispatch.
- [ ] Replace them using source-backed inputs/outcomes; cancellation preserves state unless original source specifies an effect.
- [ ] Reach affected events naturally through unchanged production handlers, validate original media and state, and test save/relaunch.
- [ ] Separate bounded simulation tests from actual-route acceptance.
- [ ] Independent code review resolves all findings against CODING_STANDARDS.md.

## Notes

Reviewer reproduced identical culture results for all-zero/all-200 parameters and full rewards on immediate Cancel for culture, exam and GEKO. Review report holds exact locations and counterexamples. Do not remove explicit unsupported guards as a shortcut.
