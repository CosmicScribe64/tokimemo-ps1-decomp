# T-24057 ENDING fade-pair source leads

The two bodies improve the recorded 24/40-row leads to diagnostic cost 4 and 12 positional rows each. Neither is a verified C match. Production retains its original guarded `func_80134B18` and assembly `func_80134C2C`; every other function is unchanged.

## Reproduce

Run `tools/docker.sh python3 drafts/T-24057/measure.py` from the repository root. The driver replaces exactly one assigned target per private full-TU copy, explicitly removes the old B18 guard when testing B18, and compiles with production guard selection. It measures the real candidate body, not the old guarded C or assembly fallback. Results are written under ignored `build/ending24057/archive/`. Errors fail before a score is printed. Both archived frames are the target's 0x30.

## Source and type evidence

Inputs are the T-20930 B18 body and C2C hand draft, the target instruction order, existing `DisplayActor44`/record-base declarations, and matched BUNKA_SD `func_801355AC` (byte promotion and zero-update analogy). The BUNKA zero-multiply idiom did not improve this pair and is not retained.

Separate real record views at `D_80120650` and `D_80120694` keep the two fade fields distinct. Explicit byte views of the GameState colour fields preserve the target's colour allocation in v1 while fade96 uses a1. The byte views are marked FAKE because their purpose is compiler promotion. C2C uses one `D_80120650` aggregate for the final flag stores to records 0 and 1; that restores their load/store order. This shared view is restricted to the flag pair, so it preserves the separate fade views. The fade update order is record96 then record97 in both archived bodies. Callback arguments continue to reload the global fade value on each call, preserving possible mutation.

## Diagnostic controls

Costs below are aligned differing-instruction diagnostics; row totals are the printable positional evidence. They are not literal linked-byte proofs or directly interchangeable with older census counters.

| Control | B18 cost/rows | C2C cost/rows |
| --- | ---: | ---: |
| Replayed scalar source | 20/24 | 30/40 |
| GameState byte views alone | 36/40 | 48/56 |
| Separate records alone | 22/28 | 34/44 |
| Byte GameState plus separate records | 4/12 | 18/28 |
| Record96-first order on C2C | n/a | 6/20 |
| Shared aggregate only for final flags (archived) | 4/12 | 4/12 |

Scalar fade-byte aliases and legal record byte-array forms reach the same 4/12 B18 and 6/20 C2C before the terminal flag fix. Mixing scalar record97 with typed record96 does not improve. Swapping B18 record updates worsens to 16/20. A real previous-fade local, loaded before colour stores and discarded before calls, worsens to 6/14 and 8/22; declaration order does not help. Moving the independent fade update before colour stores leaves B18 at 4/12, while interleaving colour stores changes allocation and worsens it.

Explicit narrowing and plain assignments leave scalar B18 at 20/24 and record96-first C2C at 22/32. The existing matched ENDING comma-argument recipe leaves the scalar results unchanged. The accurate narrow `sprite_brightness(s16,u8)` view adds index conversions (36/36 and 58/60). A scoped existing main caller view `(s32,u8)` still leaves the best early scheduling difference and is not retained. No header or compiler exception was added.

## Remaining blocker

Both archived bodies have only the same early scheduling discrepancy: the target materializes and loads fade96 between the GameState colour stores and before the record97 store. The draft places that LUI/LBU later, across the record97 store. Registers, frame, arithmetic, loop reloads and terminal operations otherwise agree. Four differing-instruction edit cost expands to twelve positional rows because moved instructions shift their neighbors. This is concrete scheduling evidence, not proof of a compiler defect or an ownership solution.

Production src/include/config are unchanged from setup 496724768. Regression checksum and header/global/prototype guards check fallback integrity only. Ignored logs are in `build/ending24057/`: `archive-proof.log`, stage logs, per-candidate diffs, `production-audit.log`, `regression-build.log`, `regression-checksum.log`, `headers.log`, `globals.log`, and `protos.log`. Only source and provenance are archived; no original assembly or game data is committed.
