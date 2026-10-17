---
id: T-21015
title: "Several data ranges per object in config/objects, and a durable range for the InitMouse island"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-12050-codex-cx5]]", "[[tickets/T-12010-codex-cx1]]"]
---

## Goal

Several data ranges per object in config/objects, and a durable range for the InitMouse island. config/objects/<unit>.txt allows one data range per object, so two real cases do not fit. (a) T-12050/T-17018: menu_bar_color in src/main/8004F870.c (84 bytes, 17 rows guarded) shares one lui $at between D_800E3680 and D_800E3684, so the original defines that data in the 8004F870 object, but 0x800E3680..0x800E36C0 lies outside its single data range (0x800E36C0 belongs to 80059710, 0x800E36F0 to 80085E30). (b) T-12010 added `data 80059710 800E36C0 800E36F0 owner-mouse/6 ...` to config/objects/main.txt by hand, a generated file; tools/object_boundaries.py --data --write (write_data) deletes all data/bss lines, so the next regeneration drops it and breaks InitMouse and the 80059710 data island. Teach object_boundaries.py/split_objects.py to derive or accept several data ranges per object (with evidence names and a unit test on synthetic input, no game data), regenerate config/objects/main.txt instead of hand edits, then retry menu_bar_color with a C definition of D_800E3680. Docker only.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-12050-codex-cx5|T-12050]], [[tickets/T-12010-codex-cx1|T-12010]].

## Comments
