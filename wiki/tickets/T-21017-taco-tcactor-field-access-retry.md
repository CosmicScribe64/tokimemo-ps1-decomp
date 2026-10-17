---
id: T-21017
title: "Retry TcActor field access in TACO func_8015C2CC and func_8015C718"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-12070-codex-cx7]]"]
---

## Goal

Retry TcActor field access in TACO func_8015C2CC and func_8015C718. These two functions (src/ovl/TACO/8015ABF0.c, matched in T-12070) still reach D_8015EDB4 through byte-offset views marked FAKE, but the TcActor type exists since T-16560 and CODING_STANDARDS section 8 wants the real aggregate unless the build proves otherwise. Only func_8015B59C has a recorded measurement (direct fields differ by 30 rows). Try TcActor fields per access in C2CC and C718 (and re-measure B59C), keep a byte-offset view only where the build needs it, and write the row count in the FAKE comment. Docker build and funcdiff required; bytes must not change.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-12070-codex-cx7|T-12070]].

## Comments
