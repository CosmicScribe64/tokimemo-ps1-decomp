---
id: T-11550
title: TT: Opus near-miss pass with order_search, autotweak and regdiff
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11540-tt-order-search-pass]]", "[[decompile-workflow]]", "[[register-cheatsheet]]", "[[data/t0018-cases]]"]
---

## Goal
One Opus agent on the closest guarded TT drafts, using every solver now in `main`: `tools/autotweak.py` (cheat-sheet
edits, order stage T-11200, regdiff hints T-11220), `tools/order_search.py`, `tools/regdiff.py`, and reasoning.
Targets: `func_8013E850` (2 lines), `func_80144A04` (3), `func_8013C1FC` (4), `func_80140624` (9), `func_801392A0` (11),
`func_80142350` (11), `func_801330D8`, `func_80140920`. Bundled with T-11540 in one PR.

## Acceptance criteria

- [ ] Each target matched or its guard comment updated
- [ ] Inline review against `CODING_STANDARDS.md`
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results

## Review

## Comments
