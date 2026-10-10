---
id: T-3052
title: Split .data and .bss per original object
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-0500-per-file-game-rodata-data-bss-split]]", "[[source-files]]", "[[tickets/T-0301-sdk-rodata-data-split]]"]
---

## Goal

T-0500 split the text and the rodata per original object; `.data` (overlays: `<NAME>_data`, main: `data`) and `.bss` are still one asm blob each, so a C file cannot define its initialised or static variables. Give each object its `.data` chunk the same way (a `.data` island per C file), from `.data` ownership: symbols used by one object only, pointer tables into that object's rodata, order following the objects.

## Acceptance criteria

- [ ] Per-object `.data` boundaries with evidence and confidence in `config/objects/*.txt` and [[source-files]].
- [ ] `tools/split_objects.py` writes `.data` islands; clean build 27 of 27 OK.

## Notes

Evidence seen in T-0500: the three OMIMAI switch objects each read three tables of their own `.data` (`80134A60`, `80134B60`, `80134C30`), in object order. The main exe's game and SDK data interleave (T-0301).

## Comments
