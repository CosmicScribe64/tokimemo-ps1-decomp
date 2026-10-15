---
type: research
updated: 2026-10-15
---

# GEKO and DATE2 source-family work

Scope [[tickets/T-13900-scenes-families]], children [[tickets/T-13901-geko-families]] and [[tickets/T-13902-date2-families]]. Base 0e670a13. Owned source only GEKO and DATE2 C and overlay headers. Prior broad sweep GEKO 80133C50 scored 164 and failed bitfield compilation; no DATE2 candidate was in that sweep. Reuse Bunka consistent aggregate pointers, bitfield widths and state ordering where supported by actual target assembly. Experiments retained in build/scenes13900.

DATE2 func_8013820C: direct case stores replace the m2c shared goto (217 -> 2 rows); corrected overlay-local D_8013B5FC to s16 (target sh) removes final difference. D_8013B5D0 is s16 from its target store. Literal full-TU default proof reports MATCH, 768 linked bytes; DATE2 SHA1 OK. Evidence build/scenes13900/first-proof.log and first-build.log. GEKO 801367EC remains 44 rows with direct aggregate and 50 with pointer; no win counted.
