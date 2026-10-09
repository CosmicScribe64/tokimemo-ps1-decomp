---
id: T-1300
title: Tooling: reuse C across identical functions
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[build-system]]"]
---

## Goal

Overlays and the main exe contain many byte-identical helper functions at different addresses. Build `tools/dupes.py`: fingerprint every function of the split asm with relocations masked, group identical ones, and copy already matched C into the unmatched members (rename, translate symbols by relocation position, declare externs per CODING_STANDARDS 8a), verified per object by the build.

## Acceptance criteria

- [x] `tools/dupes.py` report mode lists groups, plans and skipped (ambiguous) functions.
- [x] `--apply` copies C and header declarations; `--check` builds each object and reverts copies that break a match.
- [x] Unit tests on synthetic asm: `tools/test_dupes.py`.
- [x] Clean build (`rm -rf asm build`, configure, full `ninja`): 27/27 sha1 OK; `ninja progress` delta recorded.
- [x] Usage documented in [[decompile-workflow]] and [[build-system]].
- [x] Inline code review against CODING_STANDARDS.md.

## Notes

Fingerprint: instruction words with %hi/%lo/%gp_rel low 16 bits, jal/j target and branch-to-symbol offset masked, plus a marker of which words are relocated; functions with jump tables or data are left out. Symbols are identified by address when they are main-exe symbols (below 0x800F6000, so `func_8007ED84` and `bg_read_sub2` are one symbol) and by name otherwise; the mapping must be one-to-one with equal addends.

## Comments
- Run (clean build, `rm -rf asm build`, configure, full `ninja` without `-k`): 50 groups with matched and unmatched members, 185 unmatched members; 170 planned, 15 skipped (conflicting target declarations such as `func_8004284C` void vs s32, `FlushCache` vs `func_...` not one-to-one, a field name missing from the closure); `--apply --check` kept 163 and reverted 7 (sha1 mismatch in DATE, EVENT, GEKO, KANGEI; header conflict in GYOZI, SHOUGATU; GEKO type error). 27 of 27 sha1 OK. `ninja progress`: 714 -> 877 of 6962 (main 269/834, overlays 608/6128). Four overlays had no header (ETC, GEKO, TACO, TAIIKU): the tool created `include/ovl/<NAME>.h` and the `#include`.
- Inline review against CODING_STANDARDS.md: C89 only (copied from matched C, no FAKE/NON_MATCHING/`//` text added), declarations once per closure (`tools/check_headers.py` OK in the build), placeholders kept (target's own D_/func_ names), tool is Python 3 run in Docker with 13 unit tests (`tools/test_dupes.py`), no game data staged. Finding fixed during review: decl copies carried the source's trailing comments and caused duplicate declarations; comments are now stripped and duplicates are detected by symbol. Follow-up: rejected and skipped functions need hand work.
