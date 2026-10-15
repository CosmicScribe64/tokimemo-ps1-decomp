---
id: T-12703
title: Transfer proven scene idioms to held-out BUNKASAI family
status: In Review
assignee: bunkasai_family
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12700-bunka-families]]"]
---

## Goal
Test newly demonstrated BUNKAKEN scene idioms on remaining handlers in BUNKASAI 8014DD90.c, starting with one target and expanding on close evidence.

## Acceptance criteria
- Literal linked-byte verification and BUNKASAI SHA1 for each accepted function.
- Preserve existing matching code and unsuccessful experiments.
- Standards review before Done.

## Notes
One compiler; Docker only. Stop after two distant misses. No shared-header changes without coordination.

## Results
First held-out `func_8014DF04` reached 14 rows using the aggregate-pointer family form. The remaining two byte OR stores became existing CharFlags b12/b13 assignments, reaching zero. This extends the already established typed-store mechanism. Same fresh transform gives zero for `func_8014E2AC` and `func_8014FC70`.

All three accepted bodies preserve existing source around their former INCLUDE_ASM sites. Docker `build/b12703/proof.log`: BUNKASAI SHA1 OK; literal linked MATCH for 4DF04 (936 bytes), 4E2AC (832), 4FC70 (884): **3 functions / 2,652 bytes**. No shared-header, compiler or generated-assembly changes. Initial 4E9D0 candidate remains 128 rows; scratch retained under build/b12703.

Final family result: fourth target `func_8014E9D0` reaches zero after replacing m2c's temporary/goto into another case with ordinary direct dialogue-value stores. This removes an artificial join, with no forced registers or FAKE constructs. All four are literal MATCH in `build/b12703/final-proof.log`: 936 + 832 + 1,364 + 884 = **4 functions / 4,016 bytes**. BUNKASAI SHA1 and headers OK. File 8014DD90.c is now entirely C. All source changes ready for parent commit and standards/spec review; no compiler running.
