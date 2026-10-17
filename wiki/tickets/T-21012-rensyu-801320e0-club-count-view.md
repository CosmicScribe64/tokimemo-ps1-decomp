---
id: T-21012
title: "RENSYU func_801320E0 case 5: use the RensyuClubCount view instead of a raw u16 cast"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-15600-fresh-near-matches]]"]
---

## Goal

RENSYU func_801320E0 case 5: use the RensyuClubCount view instead of a raw u16 cast. src/ovl/RENSYU/80132040.c:88 reads the club counter as `*(u16 *)&D_800E6280.unk_66C[...].unk_02 & 0xF` while RensyuClubCount (include/ovl/RENSYU.h, T-19020) already models that halfword (count is the low 4 bits at +2) and is used at lines 243 and 314. Replace the cast with `((RensyuClubCount *)&D_800E6280.unk_66C[...])->count + 5`, keep it only if the default linked proof and RENSYU SHA1 still match, otherwise mark the cast FAKE with this ticket and the reason.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-15600-fresh-near-matches|T-15600]].

## Comments
