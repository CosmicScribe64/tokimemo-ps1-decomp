---
id: T-11320
title: Close near misses (Sonnet) 48
status: Done
assignee: Sonnet
created: 2026-10-14
updated: 2026-10-17
links: []
---

## Goal

Close the 32 near-miss functions in list 48 (worktree s48).

## Acceptance criteria

- [ ] Matched functions committed as plain C; the build is 27/27 OK.

## Notes

## Comments

2026-10-17: Cleanup pass. Only RPG_BAT func_8014E64C was matched and merged (plain C, implicit-int s32 idiom from matching-notes), and the NAME_ENT func_801460D8 draft got closer but is still guarded; no other T-11320 commit exists and the s48 worktree is gone. The remaining near misses of list 48 are covered by the wave 2/3 packages (func_801460D8 sits in w3-name-ent-p04), T-20600, T-20830 and the T-14500 inventory. Review of the merged code found nothing. Closing as superseded.
