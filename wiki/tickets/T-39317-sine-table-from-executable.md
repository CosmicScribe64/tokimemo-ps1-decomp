---
id: T-39317
title: Read the libgte sine table from the installed executable
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[engine-porting-contract]]", "[[psyq-sdk]]"]
---

## Goal

`port/godot/core/native_math.gd` commits `RSIN_QUARTER_TABLE`, Sony SDK data copied from the executable (since f8497f8ca, 2026-10-27). That breaks the rule against committing original data. Read the table from the user's installed executable at runtime instead, and remove it from Git (from the current tree; history rewriting is out of scope).

## Acceptance criteria

- [ ] The table is read from the installed executable at its original address and verified by a pinned SHA-256; the committed constant is gone.
- [ ] Every user of `Math.rsin` keeps identical results: core suite, engine diff, opening frame timing (0 mismatches) and the April loop route (IDENTICAL).
- [ ] Without the installed executable, `rsin` refuses with a precise error instead of guessing.

## Comments
