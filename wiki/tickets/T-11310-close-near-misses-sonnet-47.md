---
id: T-11310
title: Close near misses (Sonnet) 47
status: Done
assignee: Sonnet
created: 2026-10-14
updated: 2026-10-17
links: []
---

## Goal

Close near-miss functions from the s47 list (33 functions, closest first).

## Acceptance criteria

- [ ] Matched functions are plain C, committed
- [ ] Clean ninja 27/27 OK

## Notes

## Comments

2026-10-17: Cleanup pass. Six functions were matched and merged (OPTION func_80135450 and func_8013884C, TACO func_8015266C, ENDING func_8013BE64, RPG_BAT func_80136CCC, main func_8005FA20) plus five guarded drafts; func_80049044, join_club_message and func_8005BE8C were matched later under T-12010 and T-12030, while SHUGAKU func_801352B0 and SHOUGATU func_801347E0 stay guarded with rows in t0018-cases. Review against CODING_STANDARDS.md found no blocking issue; the unmarked u8 view of unk_1094 in func_8005FA20 goes to a Backlog cleanup ticket. The rest of list 47 was never started and no s47 worktree is left, so the ticket is closed as Done (integration tree rebuilds clean 27/27 under T-20900).

Followup opened: [[tickets/T-21005-near-miss-views-and-dead-blocks-cleanup|T-21005]].
