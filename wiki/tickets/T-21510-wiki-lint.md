---
id: T-21510
title: Wiki lint for tickets, kanban, index and log
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: [[SCHEMA]], [[kanban]]
---

## Goal

The AGENTS.md ticket rules are mechanical but unchecked. The 2026-10-17 cleanup found 69 tickets without a kanban
card, a duplicate card (T-20005) and a status/column mismatch (T-10910); reviewers repeatedly fixed log entries
inserted mid-file, doubled blank lines and stale index lines by hand.

## Plan

`tools/check_wiki.py` (no game data needed), run in CI and by the integration merge script:
- every `wiki/tickets/T-*.md` has exactly one kanban card, in the column equal to its frontmatter `status`;
- every card points at an existing ticket file;
- `wiki/log.md` is append-only relative to the merge base (no lines removed or inserted above the old end) and its
  entry dates do not go backwards;
- each ticket has one `wiki/index.md` line;
- no doubled blank lines between kanban cards.

## Acceptance criteria

- [ ] Tool with unit tests; passes on the current tree after the cleanup
- [ ] Wired into `.github/workflows/progress.yml` (runs for forks too) and the integration merge script
- [ ] Documented in [[SCHEMA]]

## Comments
