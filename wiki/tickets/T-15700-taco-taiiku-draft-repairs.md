---
id: T-15700
title: Compile all 28 failed TACO and TAIIKU draft scopes
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-14500-exhaustive-drafts]]", "[[tickets/T-13600-taiiku-families]]", "[[tickets/T-13800-taco-families]]"]
---

## Goal

Repair all 17 TACO and 11 TAIIKU failed scopes, retaining complete real C, source/preprocessed contexts, complete compiler diagnostics and provenance. Do not optimize matching or replace bodies with stubs/assembly.

## Acceptance criteria

- [x] All 28 compile, or precise per-scope remaining blockers with sustained reconstruction attempts.
- [x] Inspect existing alternatives and historical C before regenerating.
- [x] Preserve initial archives and existing production matches.
- [x] Docker only, serial compiler, full-TU comparisons; no unproved matching claims.
- [ ] Independent standards review before Done.

## Notes

Reservation extracted from latest best-catalog: TACO17/TAIIKU11. Frozen source census 48362009; current unrelated accepted changes do not touch owned TUs. Read previous family tickets and every prior alternative's available diagnostics. Known historical taco-13800/taiiku-13600/cx2/cx9 sources contain no implemented C definition for these exact targets. Start from immutable initial body/declarations and capture complete diagnostics, since earlier helper-tail truncation obscures actual errors.

## Comments

2026-10-17: Closed after review. All 28 TACO and TAIIKU scopes compile as archived full-TU alternatives, no production file changed, and 16 of the 28 targets are now plain C on main (the drafts fed T-19029 and others); the rest were redrafted by T-16550 and T-16560. The one finding is that the 28 comparison.txt files carry 9,828 rows of original instruction words (CODING_STANDARDS section 10), handled in the drafts-archive scrub followup opened from T-14600.

## Results

**28/28 compile** (TACO17, TAIIKU11), covering 29584 target code bytes. No zeros or matching-coverage gain. All target bodies are real compiler input; no stubs or ASM substitutions. Full contexts, complete compiler diagnostics, comparisons, hashes/provenance and reproduction drivers are in drafts/T-15700. Initial census archives and production C/header files are unchanged.

| Scope | Code bytes | Diagnostic rows |
| --- | ---: | ---: |
| TACO:func_80137264 | 812 | 172 |
| TACO:func_8013A790 | 1492 | 574 |
| TACO:func_8013D528 | 784 | 259 |
| TACO:func_8013D838 | 832 | 274 |
| TACO:func_8013DEE4 | 1336 | 429 |
| TACO:func_8013E778 | 812 | 263 |
| TACO:func_8013EAA4 | 1948 | 626 |
| TACO:func_801424A8 | 880 | 260 |
| TACO:func_80145650 | 1908 | 709 |
| TACO:func_801487A4 | 948 | 366 |
| TACO:func_801497EC | 912 | 348 |
| TACO:func_8014CB24 | 1756 | 598 |
| TACO:func_80153B38 | 832 | 293 |
| TACO:func_80154960 | 1556 | 192 |
| TACO:func_801553AC | 1084 | 364 |
| TACO:func_80159260 | 1048 | 308 |
| TACO:func_8015ACCC | 1260 | 515 |
| TAIIKU:func_80132BD0 | 592 | 278 |
| TAIIKU:func_80132E20 | 692 | 525 |
| TAIIKU:func_80134060 | 1348 | 364 |
| TAIIKU:func_80135F3C | 1676 | 556 |
| TAIIKU:func_80138DF8 | 1336 | 236 |
| TAIIKU:func_8013D114 | 320 | 98 |
| TAIIKU:func_8013FC5C | 488 | 148 |
| TAIIKU:func_80143DD0 | 384 | 107 |
| TAIIKU:func_80143F50 | 956 | 241 |
| TAIIKU:func_80144464 | 572 | 185 |
| TAIIKU:func_80147B14 | 1020 | 540 |


Complete diagnostics revealed ten initially recoverable inputs after declaration/signature corrections, then eighteen remaining reconstructions. Earlier truncated helper diagnostics obscured the errors. Repairs use real SDK SVECTOR records and observed coordinate arrays; exact 11/23-byte unaligned copies, 46-byte halfword tables, existing selection table types; TacoRnd scalar member, TAIIKU packed word/first byte views, and three genuine TcPos by-value arguments. No matching optimization or artificial padding.

The archived func_80154960 TU completes helper func_80156178's consumed integer return by returning its existing last func_8015ACCC result. This is explicitly recorded as a companion ABI/type completion in context/provenance; the production matched helper stays unchanged. All other current matching C stays unchanged. Full-TU compile and diagnostic score are evidence of compilability only, not semantic or literal-byte matching proof.

## Review

Final serial Docker compile/comparison succeeded for all28. C89, existing SDK types, no target stubs/inlineASM/fallback, no binaries/assets staged. Initial attempts retained, table/vector extents tied to observed accesses/copies, production changes absent. Independent standards review pending; In Review.
