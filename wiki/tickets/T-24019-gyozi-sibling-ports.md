---
id: T-24019
title: GYOZI sibling completion ports
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[event-family-results]]", "[[shougatu-family-results]]"]
---

## Goal

Bounded matched-sibling ports for GYOZI func_8013907C, func_80139E4C, func_8013E824 and func_80142A8C. Only their four translation units are owned.

## Acceptance criteria

- [x] Remove target INCLUDE_ASM only for independently verified default literal C matches; affected unit SHA-1 passes.
- [x] Source-only failed forms and prior evidence retained.
- [x] Header/global/prototype checks pass.
- [x] Independent review.

## Notes

Read wiki index, event-family-results (GYOZI family), shougatu-family-results, T-17600, T-13201, T-22022 and matched siblings. Earlier negatives include GYOZI39E4C split stores33/temporary mode view27 and E824 narrow result/split mask24; do not misreport default ASM fallback as recovered C. One Docker compiler/ninja-j1; bounded25min.

## Verified batch 1

func_80142A8C matched 392 default linked bytes. Direct port of DATE func_80152E34 uses existing GyoziWork, SDK POLY_G3, the local work pointer and ordinary comma colour assignments. No new FAKE, header edit or compiler change. Explicit target INCLUDE_ASM removed from src/ovl/GYOZI/801418B0.c; default no-build funcdiff reports linked bytes equal target, GYOZI SHA1 check passes, headers/globals pass, prototype branch check zero new/fifteen known disagreements.

## Bounded final result

Result: one of four matched, 392 default linked bytes. Production change is only func_80142A8C in src/ovl/GYOZI/801418B0.c; the other three functions remain ASM. No new FAKE or header changes in production.

The triangle helper is a direct matched DATE func_80152E34 port using SDK POLY_G3 and existing GyoziWork. The local work pointer and comma colour stores give0 rows; direct global reads give22. Default explicit --resolve --version default --no-build comparison proves392 literal linked bytes after removing the target INCLUDE_ASM. Docker ninja-j1 GYOZI SHA1 passes: 43f92523116e7b6cd772d4397712a52eeda89703. Headers/globals pass; prototype branch check zero new/fifteen known disagreements.

Unresolved current rows/aligned:
- func_8013907C: original direct port170/170, s32 definition168/168; natural control flow58/16; meaningful s32 call-result local before the saved byte56/12 recovers original sp+0x2B. Correct byte-table indexing is retained in body.c. A cached u8 selector lowers row count52 but raises aligned edits14, so the meaningful-result form is the better structural lead. The remaining differences are call-argument base allocation, LUI placement and early switch-index shift scheduling. The byte table declaration extern u8 D_80147224[] was only a scratch prefix; it needs an overlay-header declaration before promotion.
- func_80139E4C: direct stores247/171; separate flag stores33/31; reading flag+selector through existing GyoziWork27/27. Earlier T-13201 had the same27-row result; this is reproduction, not new progress. body.c uses the known flag byte at work+0x70E through unk_410[0x2FE], an unresolved field view. The separate-symbol control is also archived. Remaining differences are two default-case LUI loads hoisted into branch delays and resulting branch offsets. Source-line grouping does not fix them.
- func_8013E824: direct sibling-style stores188/188; narrow unsigned result followed by split mask24/24, reproducing T-13201. u16 gives the same; signed byte worsens237/201. Meaningful result-copy types, declaration order and grouped mask/init do not close the24 rows. The prologue lacks target's result copy and uses exchanged result/counter registers; case bodies otherwise match. No claim of compiler impossibility.

Read wiki/event-family-results.md (GYOZI family), shougatu-family-results.md, T-17600, T-13201, T-22022 and current siblings. The ranking input is the primary tree's build/complete-siblings.json. No earlier archived negatives were modified.

## Independent root integration review

Production changes reviewed against CODING_STANDARDS.md. No unresolved findings; existing SDK types and aggregates retained, necessary ABI views explained, FAKE forms marked, no compiler/config exceptions. All27 binary hashes and header/global/prototype guards pass. Each new target lacks its INCLUDE_ASM fallback and passes normal-default literal linked-byte proof. Root published verified code on main; unresolved family functions are not counted.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
