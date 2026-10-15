---
type: research
updated: 2026-10-15
---

# GEKO and DATE2 source-family work

Scope [[tickets/T-13900-scenes-families]], children [[tickets/T-13901-geko-families]] and [[tickets/T-13902-date2-families]]. Base 0e670a13. Owned source only GEKO and DATE2 C and overlay headers. Prior broad sweep GEKO 80133C50 scored 164 and failed bitfield compilation; no DATE2 candidate was in that sweep. Reuse Bunka consistent aggregate pointers, bitfield widths and state ordering where supported by actual target assembly. Experiments retained in build/scenes13900.

DATE2 func_8013820C: direct case stores replace the m2c shared goto (217 -> 2 rows); corrected overlay-local D_8013B5FC to s16 (target sh) removes final difference. D_8013B5D0 is s16 from its target store. Literal full-TU default proof reports MATCH, 768 linked bytes; DATE2 SHA1 OK. Evidence build/scenes13900/first-proof.log and first-build.log. GEKO 801367EC remains 44 rows with direct aggregate and 50 with pointer; no win counted.

GEKO func_80141374: replacing the m2c duplicated loop cursor with the single s16 induction variable from the target closes 39 -> 0 rows. Uses the existing GameState name array. Full-TU default literal MATCH 416 bytes and GEKO SHA1 OK (build/scenes13900/geko-proof.log, geko-build.log). Existing matching source unchanged. The recovered month table / label table / large string buffer compile but remain nonmatching; discarded production candidates, retained scratch evidence.

GEKO dispatch/indicator family: func_801351F8 (328 bytes) and func_8013882C (596 bytes) both literal MATCH; GEKO SHA1 OK, build/scenes13900/dispatch-proof.log and dispatch-build.log. The established copied function-table form plus explicit used selector fixes the former stack offset. Direct global flag updates remove the m2c byte temporary/redundant moves in both functions (24 -> 2 -> 0 and 24 -> 0). No FAKE source forms. Current accepted total four functions / 2108 linked bytes.
