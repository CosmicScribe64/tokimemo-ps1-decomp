---
type: research
updated: 2026-10-15
---

# GEKO and DATE2 source-family work

Scope [[tickets/T-13900-scenes-families]], children [[tickets/T-13901-geko-families]] and [[tickets/T-13902-date2-families]]. Base 0e670a13. Owned source only GEKO and DATE2 C and overlay headers. Prior broad sweep GEKO 80133C50 scored 164 and failed bitfield compilation; no DATE2 candidate was in that sweep. Reuse Bunka consistent aggregate pointers, bitfield widths and state ordering where supported by actual target assembly. Experiments retained in build/scenes13900.

DATE2 func_8013820C: direct case stores replace the m2c shared goto (217 -> 2 rows); corrected overlay-local D_8013B5FC to s16 (target sh) removes final difference. D_8013B5D0 is s16 from its target store. Literal full-TU default proof reports MATCH, 768 linked bytes; DATE2 SHA1 OK. Evidence build/scenes13900/first-proof.log and first-build.log. GEKO 801367EC remains 44 rows with direct aggregate and 50 with pointer; no win counted.

GEKO func_80141374: replacing the m2c duplicated loop cursor with the single s16 induction variable from the target closes 39 -> 0 rows. Uses the existing GameState name array. Full-TU default literal MATCH 416 bytes and GEKO SHA1 OK (build/scenes13900/geko-proof.log, geko-build.log). Existing matching source unchanged. The recovered month table / label table / large string buffer compile but remain nonmatching; discarded production candidates, retained scratch evidence.

GEKO dispatch/indicator family: func_801351F8 (328 bytes) and func_8013882C (596 bytes) both literal MATCH; GEKO SHA1 OK, build/scenes13900/dispatch-proof.log and dispatch-build.log. The established copied function-table form plus explicit used selector fixes the former stack offset. Direct global flag updates remove the m2c byte temporary/redundant moves in both functions (24 -> 2 -> 0 and 24 -> 0). No FAKE source forms. Current accepted total four functions / 2108 linked bytes.

## Bounded negative ledger

All scores below are full translation-unit resolved diagnostic diff rows, not linked match proofs. Rejected sources remain only in ignored build/scenes13900; no guarded drafts or speculative declarations entered production.

| Candidate | Evidence and best bounded outcome |
| --- | --- |
| GEKO 801367EC, 872 list bytes | Existing GameState fields plus missing overlay byte declaration: direct 44, consistent pointer 50. String argument scheduling remains; no repeated broad sweep. |
| GEKO 80144198, 216 list bytes | Actual twelve-halfword month table reconstructed. Typed s32 year/month, direct returns, K&R definition probes: 81 -> 76 -> 74. Stack placement/register shape unresolved. |
| GEKO 80137580, 176 list bytes | Fresh direct source 58; then prior T-10910 copy-chain ledger located and read. No allocator sweep repeated; existing guarded source unchanged. |
| GEKO 80141870, 268 list bytes | Existing aggregate normalization: 81. Control-flow/result recovery needed. |
| GEKO 80134228, 244 list bytes | Existing aggregate typed draft 39; direct globals/postincrement probe 40. Not promoted. |
| GEKO 8013B10C, 388 list bytes | Recovered actual 42-entry call table and tested forward void declaration: 63. Boolean/call result and byte-view effects remain; experimental prototype removed. |
| GEKO 80133AA0, 432 list bytes | Target lh shows overlay byte declaration is too narrow at this call site. Halfword plus reloads 81, s32 fallthrough result 67. Experimental header change removed; existing matching C untouched. |
| GEKO 80133598, 452 list bytes | Recovered Rec38 flag and halfword accesses; one saved status byte gives 22, direct destination-field amount gives 4 (only two stack-byte offsets). Plausible quotient/amount locals give 8/18; no padding or artificial local accepted. |
| DATE2 801385CC, 756 list bytes | Four 21-byte labels, x/y/height arrays and four halfword widths reconstructed. Labels/layout/natural-loop probes 207 -> 199. Needs real local/layout/ABI reconstruction; speculative width declaration removed. |
| DATE2 801388C0, 380 list bytes | Actual GsRecF90 paired-record flag and label width array: 120. No source promoted. |
| DATE2 801330D4, 388 list bytes | Fresh draft exposes missing local score array and stride-indexed character attributes. Compile failure retained; no codegen conclusion. |
| DATE2 80136860, 560 list bytes | Aggregate typed draft 192. Timer and reward control-flow reconstruction still needed. |
| DATE2 801395C8, 2648 list bytes | Observed loop bound 81, byte stride31 and 0xA30 frame support actual 81x31 string buffer. First typed matrix 679; shared source locals and switch loops reduce to432. Existing integer globals hold pointer tables; casts used only in scratch. Needs original local expression lifetimes and call ordering, not padding guesses. |

Fresh drafts for GEKO 80132D04 (800), 80139E1C (808), 8013E950 (856), 801420E4 (1260), 80144270 (1184), DATE2 80133AF0 (3680), 801348F0 (2048), 801350A4 (3480) were inspected for source families and missing types but not claimed matched or exhausted. GEKO 80143F48 needs the actual packed date/activity fields and copied byte table. No broad automatic mutation farm ran. These are useful follow-up targets for actual layout recovery; this bounded pass does not establish a compiler impossibility or exhaustion of either scope.

## Final handoff

| Unit | Function | Literal linked bytes | Commit |
| --- | --- | ---: | --- |
| DATE2 | func_8013820C | 768 | 1cd70b47 |
| GEKO | func_80141374 | 416 | b234c5e5 |
| GEKO | func_801351F8 | 328 | 1e2eb462 |
| GEKO | func_8013882C | 596 | 1e2eb462 |

Total **four functions / 2108 literal linked bytes**. List/instruction sizes can include embedded rodata and differ from the literal function bytes above. Final proof build/scenes13900/final-proof.log, affected overlay checks final-build.log; header/global/prototype logs pass with 0 new / 15 known disagreements. Body-audit.log confirms all **102 pre-existing C bodies** in three touched files are text-identical to 0e670a13 (19 DATE2, 44 + 39 GEKO). No shared canonical-header changes, no generated assembly/build-tool changes, no new FAKE forms and no disc data staged. This pass verifies the two affected overlays, not all 27 binaries. The overall 80% target remains ongoing with the integration chat.

Independent standards and spec reviews of 0e670a13...1e2eb462 found no actionable findings. Standards review checked CODING_STANDARDS.md, source and proof logs; spec review checked ownership, verification and preservation. Original checkout remained read only. Branch codex/scenes-13900 is ready for parent integration; no PR or push performed.
