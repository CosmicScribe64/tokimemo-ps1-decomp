---
id: T-25055
title: Match TEL qualification dialogue through causal compiler evidence
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25024-tel-qualification]]", "[[playbook]]"]
---

## Goal

Match TEL `func_80136724` (6,580 bytes) in `src/ovl/TEL/80132000.c` using new causal compiler-pass evidence. Exclusive isolated lane on `codex/t25055` in `/private/tmp/tokimemo-t25024`; previous checkpoint remains preserved at `3b257980c` on `codex/t25024-tel-qualification`.

## Acceptance criteria

- [x] Literal default C and resolved linked bytes match.
- [x] Coordinated full build passes all 27 checksums and declaration guards.
- [x] Direct standards/spec review resolves all findings before Done.
- [x] Preserve measured candidates and exact replay evidence; no unsupported coverage claims.

## Notes

Baseline inherited from T-25024: hand 1,643 words / 1,372 changed positions / 1,038 aligned cost; pointer 1,643 / 1,403 / 970, against 1,645 target words. Production now uses matching default C. Causal range reconstruction replaced the exhausted month/source-shape grids. Docker only, existing image, read-only original disc, at most j2; no shared header/build-tool edits.

## Result and validation

Default C matches all 6,580 linked bytes, including padding. Full ninja build passes all 27 checksums. After final source cleanup, fresh hashes of all 27 build binaries match again; TEL SHA1 is d37c3ae6beb7ab361f39366b8274cde0f7c96b5c. Header/global guards pass, prototype branch check has 0 new disagreements and 15 known. Exact evidence and Docker replay are in drafts/T-25055. Ticket remains In Review for root integration.

Direct global selection in case0x40 reserves the missing caller register and reduces aligned cost970 to127. Sharing maximum and total restores size, leaving41 changed positions. A defined unsigned-false girl test raises that global's allocator priority from0.25 to0.4 and changes t1 to t0, with selector t0 to t1, lim a0 to v0, and hp a1 to a0; one comparison remains. hp*1 restores its operand order. All matching-only forms and inferred frame space are marked FAKE.

ABI cleanup controls individually and together remove the third strcat month arguments without any changed instruction. The two integer casts on set_kanji_string's pointer arguments are also neutral and removed. func_80052060 table-pointer casts remain because its matched definition takes four s32 arguments. Existing shared declarations are unchanged. Both maximum arms define total; all32 appointment-word bits are assigned before the by-value call. The selector writes cross no calls or intervening observable alias reads.

## Comments

2026-10-22: Claimed from main `633d95356`; branch switch preserves all T-25024 archives. No production change or C match claimed.

2026-10-22: Coordinator directly reviewed standards and spec. Findings about unused frame slots, inferred text extent, inherited typed-text casts and extra strcat arguments are resolved. No remaining code finding. Worker reviewed the replay scripts and archive; no original assembly, ucode, binaries or full-TU dumps are committed. Final code is restricted to the owned function.

Root acceptance: independent default-C linked6580-byte MATCH, fresh27/27 binary hashes, headers/globals,0new/15known prototype disagreements and branch check pass on combined main. Direct source review confirms full appointment-word initialization, marked defined identity controls, final ABI cleanup and no unrelated production edits. Done. Entire80132000.c is C; worker owns last TEL function in8013CCD0.c underT-25060.
