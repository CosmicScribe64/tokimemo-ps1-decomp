---
id: T-21014
title: "Mark or rewrite the single-use GameState pointer in func_80042134"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-12010-codex-cx1]]"]
---

## Goal

Mark or rewrite the single-use GameState pointer in func_80042134. In src/main/800420D0.c (T-12010, matched 404 bytes) func_80042134 declares `GameState *g = &D_800E6280` and uses it only in the loop-tail condition `while (g->unk_110F == 0)`, while every other access is `D_800E6280.`; the loop also uses an m2c `loop_1:` label with two gotos. The T-12010 ticket claims 'No FAKE', but this is a redundant pointer view that project precedent marks FAKE. Add a FAKE comment with reason and ticket (comment-only change), or find a natural form (for(;;) with a consistent pointer) that still matches; verify with funcdiff --resolve and the main SHA1 in Docker. Also look at func_80049044 in src/main/80047550.c (`u8 *p` to unk_03A, borderline) and mark it if it is the same pattern.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-12010-codex-cx1|T-12010]].

## Comments
