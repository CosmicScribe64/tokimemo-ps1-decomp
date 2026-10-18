# T-24046 screen page renderer checkpoint

No new C match. Production src/main/8004C3F0.c and headers are unchanged. srn_tpage_show.c is a source-only normal-toolchain candidate, not a verified reconstruction.

## New concrete findings

The old T-24006 source makes `(arg0 & 0x20) * 4` an s8 before adding it into texture-page Y. That incorrectly changes 128 to -128; the target keeps a positive word and later spills it. The new source retains word arithmetic and narrows only at GPU UV stores.

The old signature has eight parameters but skips the unused argument 5 ABI slot in its names. Actual target clut inputs are arguments 6/7, and depth is argument 8. Adding the unread s32 arg5 restores their stack homes; the unused slot's semantic type is unknown, and s32 only models its word position.

Target divisor/page-width homes plus mode branch constants recover pointer-local-before-scalar declaration order and divisor-before-width assignment order. Separate source locals for every m2c spill are wrong: a single reused POLY_FT4 and inlined repeated pure expressions let IDO introduce the common-expression spill slots itself, reducing frame 200 to the exact target 120. Consecutive separate RGB assignments match the target's early blue loads better than putting the blue source assignment at the delayed byte-store location.

## Current measurements and bounded negatives

All costs are current-context full-file aligned insert/delete edits (substitutions cost two). The old body was freshly rescored 1032. Reused packet/state pointers score 1026 with frame 200. Named corrected derived values score 637 / frame 168; inline expressions 562 / frame 120; ABI/declaration/mode fixes 524; consecutive RGB order 504 / frame 120. The best candidate is 3248 instruction bytes (812 instructions) against 3224 (806), so it remains broad. A final named word-offset lifetime replacing the explicit state pointer scores 523 and was rejected. No arbitrary register or statement permutation search ran.

Remaining differences are value-web/register choices, derived-value homes and repeated GPU initialization/call scheduling. Exact frame/prologue and corrected ABI do not constitute a C match. Do not repeat the old signed-offset or missing-argument foundations; do not reintroduce m2c source variables for compiler spills without new evidence.

Reproduce in Docker by adding psx_types.h to a scratch translation-unit copy of src/main/8004C3F0.c, replacing only the target INCLUDE_ASM with the archived body, then running tools/viewsweep.py main:srn_tpage_show --src SCRATCH --budget 1. Normal default linked literal proof plus full MAIN checksum would still be required before production promotion. No assembly listing or raw game data is archived.
