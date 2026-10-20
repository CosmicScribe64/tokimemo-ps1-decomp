---
id: T-24500
title: Orchestrate the remaining matching functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
---

User resumed the 100% game-code matching goal with multiple GPT-6.1 Sol medium workers. Reserve Astra for concrete blockers after the documented recipes fail.

Baseline main `2c7293275`: Docker progress reports 6,793/6,952 game functions and 2,067,004/2,276,920 bytes (90.8%). This supersedes the older status count; SDK remains separate (728 assembly functions).

Assignments: T-24501 MAIN, T-24502 TACO, T-24503 GEKO/OPTION. Each owns an isolated worktree and source files; root reviews and integrates verified commits under the integration lock. No worker may push main independently.

GitHub issue #65 claims GEKO and three OPTION functions, but the contributor reports being out of tokens until Friday. The user explicitly authorized working these areas meanwhile. Keep completed and remaining functions documented for handoff. No contributor comments or issue closure authorized. Current remote main equals local main; no open PR was returned on October 20.

Require literal default C evidence and matching binary checksums; numerical diff improvements alone are not matches. Preserve source-only drafts and useful blocker evidence. Avoid repeating exhausted compiler experiments. Keep contributor checks at the existing interval and preserve the paused integration automation.
