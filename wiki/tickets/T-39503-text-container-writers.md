---
id: T-39503
title: Write every text container back from records
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Writers for every container that holds translated text (SERIFU, BUNKA, MASTER, overlay tables, dictionaries, C strings) that rebuild the original bytes exactly from the decoded records, then accept English through a pluggable encoder and report the size each file needs.

## Acceptance criteria

- [ ] Identity round-trip is byte-exact for every container.
- [ ] Every record id in `translations/en/*.jsonl` maps to exactly one written location.
- [ ] Size report for English: which files grow past their sectors or memory slots.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
