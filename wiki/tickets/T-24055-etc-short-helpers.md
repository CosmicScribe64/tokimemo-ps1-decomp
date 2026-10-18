---
id: T-24055
title: Recover three ETC short helpers
status: In Review
assignee: codex/complete-option
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]"]
---

## Goal

Count-focused bounded pass on ETC func_8014354C (248 bytes), func_80143758 (292), func_80143968 (288), exclusive src/ovl/ETC/80143240.c. Preserve1a1c433786 ancestry. No headers/config changes without coordination; one Docker compiler.

## Acceptance criteria

- [x] Read prior negatives and correct stale source/type defects first.
- [ ] Verify any promotion from unguarded default actual C plus unit SHA1 and guards.
- [x] Preserve meaningful source/reproducer if unmatched, with no compiler impossibility claim.
- [ ] Independent review before Done.

## Notes

Read ETC family, T-17300 typed/hand bodies, T-9720/T-10030 and T0018 records. 4354C previous13/18-row reports are historical; source available in T17300. Original model coordinates stride0x50, objects0x10; SVECTOR tail order is vz,vx,vy. Other drafts wrongly infer a word read for byte coordinate table; correct it before scoring. No blind sweeps.

## Comments

### Bounded result

Zero of3 matched,0 C bytes; production untouched. Fresh current-type model13/13 reproduces historical near miss, literal argument108/12, byte-pointer13, chained SDK vector15, array view13. Fill corrected byte table/current actor fields111/109; index193/191, single base194/192. Scroll corrected byte reads70/70; pre-call index109, direct counter74/72, late index89/85, word view74/72, int return70. No blind sweep or compiler impossibility claim. The final scroll's redundant initial counter writeback is explicitly FAKE; rejected alias probes also marked. Selected sources/reproducer and exact blockers documented in drafts/T-24055/README.md; all loose source/traces retained ignored.

Final saved-source scores freshly reproduce13/13,111/109,70/70. Baseline ETC SHA1 `681c93b6e57c885f86e074c885a8bf6c3cca2319`, unit build passes. No actual default C claim or fallback coverage. This source-only closure changes no interface; prior interface checks need no repeated full build. In Review pending independent archive/standards review.
