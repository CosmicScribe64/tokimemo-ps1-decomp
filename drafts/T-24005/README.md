# T-24005: recovered BUNKAKEN func_80143DD0 near miss

The useful source is preserved directly under NON_MATCHING in src/ovl/BUNKAKEN/80143DD0.c. It covers 3,232 instruction bytes (808 instructions); the queue's 3,532-byte size included jump tables. This work adds zero verified C bytes. The default function remains the assembly fallback.

## Provenance and recovery

T-9960 reported an eight-line near miss but explicitly never committed its scratch source. T-18500 retained a typed 330-cost draft. Starting from that archive, recover the original flag fields (the archived fixed test incorrectly read GameState.unk0; the dynamic test omitted its record offset), use b2 stores, remove inferred extra get_g_zyotai_s arguments, use the old-style func_800847B8 call view, cast the already-shifted nibble to s16 in case 22, compare the later dynamic b2 test explicitly with 1, and access the selected heroine directly instead of keeping m2c's temp_a0/temp_a3 copies. These yield 82 then 66 aligned cost. Narrowing the final selector to u8 yields 8 diff rows (four actual words).

The two seed self-stores remain required for the target stores at 0x80144830 and 0x80144924; both carry FAKE comments and ticket references. No such device is counted as a match.

## Remaining words

Only register operands differ:

- 0x80144824 and 0x80144834: target masks the call result into t4; C uses t5.
- 0x80144838: target compares t4 with 5; C compares t5.
- 0x80144840: target shifts t4 into t5; C shifts t5 into itself.

All later instructions, function extent, frame and instruction alignment agree. A u32 selector masks into t4 but also shifts into t4 and changes later temporary numbering (66 cost). A narrow selector shifts into t5 and fixes everything later, while its mask still lands in t5.

## Bounded rejected forms

Measured with full-unit order_search.make_text_scorer, resolve=True:

| Form | Cost |
| --- | ---: |
| T-18500 plus corrected calls, flags and nibble cast | 82 |
| direct selected-character accesses | 66 |
| direct unsigned call-result switches | 66 |
| u8/s8/u16/s16 selector | 8 |
| u32/s32 selector | 66 |
| cast mask result to u8 or cast selector at switch | 8 |
| branch-local masks, u8 selector | 8 |
| branch-local masks, u32/s32 selector | 66 |
| u8/u16 call-result local before mask | 66 |
| in-place call-result mask | 72 |
| cast input to u8/u16 before mask | 8 |
| cast input to s16 before mask | 72 |
| cast call results to u8 | 8 |
| input u8 cast plus u8 selector | 72 |
| s32 function return view | 8 |
| register u8 / register u32 selector | 8 / 66 |
| s8 call-result local | 66 |
| conditional-expression call, u8 / u32 selector | 8 / 66 |

The earlier T-9960 five-minute permuter also failed on this exact residue. No new brute-force/permuter pass was run. Local scripts, objects and instruction diagnostics are ignored under build/t24005. Fresh guarded proof is `python3 tools/funcdiff.py --unit BUNKAKEN --resolve func_80143DD0`; production fallback proof adds `--version default` and does not count as C coverage.
