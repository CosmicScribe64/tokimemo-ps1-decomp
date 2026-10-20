# T-24501 CD dispatcher checkpoint

Replay in the existing Docker image from repository root:

```sh
python3 drafts/T-24501/replay.py func_800455C4
```

Expected resolved cost: **26**, 691 target/candidate instructions. This is a diagnostic; no production source or header edits. `replay.py` hoists the one explicit SDK prototype override before full-TU includes. The inherited header has a guard; the SDK implementation actually returns v0. No compiler changes.

Remaining instruction-word substitutions (target -> candidate, hexadecimal stack offsets):

- Rows315/362/368: source-address local44 ->48.
- Rows317/361/367/372: sector-count local48 ->4C.
- Rows527/528: wait counter2C ->44.
- Rows541-544 and561-564: same operations/registers, different order. Target materializes flags-address `$at` and constant2 before loading header.status; candidate loads header.status first. Canonical status RMW, constant-first OR, and dependence on assigned flags all leave cost26. Nested assignment is worse66; moving flags before head increment is worse42. Status bit-field worsens124.

Frame50, status-buffer30 and position-buffer38 are already exact. Moving counter declaration after buffers produces incorrect buffer homes; removing one/both unused slots, or adding first gap with counter last does not recover target. Do not run broad permutations.

Structural recovery439 ->26: reversed entry operand order; correct masked24-bit end-address formula retaining masked value before80000000 adjustment; pointer arguments replacing stale m2c integer casts; SDK wait return view; preincrement global retry; remove spurious arguments from void helpers46478/88070; recover eight-byte queue header view while keeping canonical command array; phase store placements separated by branch and call boundaries. Never hoist phase assignment across SDK calls.

Future production review: new queue view belongs game.h; three diagnostic globals need canonical declarations; SDK return signature must be audited for all callers before shared header change. Unused frame slots must be specifically measured and FAKE-ticketed. Current draft pad comment is provisional, not an accepted layout solution.
