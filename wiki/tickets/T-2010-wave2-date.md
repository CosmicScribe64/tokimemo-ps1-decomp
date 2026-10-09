---
id: T-2010
title: "Wave 2: DATE"
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the remaining unblocked functions of `src/ovl/DATE.c` (queue flags R/V excluded). DATE2 is a separate file and not part of this ticket.

## Acceptance criteria

- [ ] At least 60 functions matched (or the unblocked queue for DATE exhausted).
- [ ] Clean build (`rm -rf asm build`, configure, `ninja` without `-k`): 27 of 27 sha1 OK, header check passes.
- [ ] Inline review against CODING_STANDARDS.md done, findings fixed.
- [ ] T-0018 cases appended to [[data/t0018-cases]].

## Notes

## Comments
