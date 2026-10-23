---
id: T-25200
title: Wave 9 neighbor conveyor - wave 8 queue from the tail, plus near-miss closers
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25100-wave-8-finish]], [[tickets/T-25201-wave-9-closers]], [[status]]]
---

## Goal

User request (2026-10-23): run neighbor work next to the wave 8 conveyor ([[tickets/T-25100-wave-8-finish]]) so the remaining functions are covered sooner. Wave 9 takes wave 8 packages that nobody started from the tail of the wave 8 order (smallest first) under the package's own wave 8 ticket, and runs fresh-eyes closers on guarded near misses (25 rows or fewer) that merged wave 8 packages left behind ([[tickets/T-25201-wave-9-closers]]).

## Acceptance criteria

- [ ] Every package wave 9 took is merged or recorded with notes, its ticket Done.
- [ ] Every near-miss closer job is recorded in T-25201.
- [ ] Every match passed the literal default-C checks, 27/27 sha1, header/global/prototype checks.

## Notes

Coordination: local `.claude/orchestrator/bin/w9_next.py` picks the next job under `state/git.lock` and blocks taken packages in `state/blocked_packages.txt`; `bin/mkwt-tool.sh` re-checks that list under the same lock, so wave 8 skips them. Worktrees `w9-*` (packages) and `w9c-*` (closers). Three workers, same usage gate as wave 8. Wave 8 fix carried here: agent reports name functions as `UNIT:func`; wave 9 compares bare names, so its closers trigger.

## Comments

- 2026-10-23 (orchestrator): wave 9 (run wf_17134ab3-58f) stopped after about 10 hours when a job-picker agent ended without its structured result, which ended the whole workflow. Results: 17 packages taken from the tail of the wave 8 queue (15 verified matches, TACO 801447D0's merge re-run by hand after the stop), 6 closer jobs ([[tickets/T-25201-wave-9-closers]], 5 verified matches). Its last pick found no untouched package left. The interrupted ENDING 80133C10 job moved to [[tickets/T-25311-w10-ENDING-80133C10]]. Replaced by [[tickets/T-25300-wave-10-second-pass]], which wraps every agent call.
