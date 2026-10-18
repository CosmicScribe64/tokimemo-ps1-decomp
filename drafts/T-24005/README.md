# T-24005: matched BUNKAKEN func_80143DD0

The source is now unguarded C in src/ovl/BUNKAKEN/80143DD0.c. Explicit default linked verification matches all 3,232 instruction bytes (808 instructions); the queue's 3,532-byte size included jump tables. The historical recovery and rejected forms below explain the final narrow-selector lifetime fix.

## Provenance and recovery

T-9960 reported an eight-line near miss but explicitly never committed its scratch source. T-18500 retained a typed 330-cost draft. Starting from that archive, recover the original flag fields (the archived fixed test incorrectly read GameState.unk0; the dynamic test omitted its record offset), use b2 stores, remove inferred extra get_g_zyotai_s arguments, use the old-style func_800847B8 call view, cast the already-shifted nibble to s16 in case 22, compare the later dynamic b2 test explicitly with 1, and access the selected heroine directly instead of keeping m2c's temp_a0/temp_a3 copies. These yield 82 then 66 aligned cost. Narrowing the final selector to u8 yields 8 diff rows (four actual words).

The two seed self-stores remain required for the target stores at 0x80144830 and 0x80144924; both carry FAKE comments and ticket references. At that recovery checkpoint, the guarded function was not counted as a C match.

## Historical four-word residue

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

The earlier T-9960 five-minute permuter also failed on this exact residue. No new brute-force/permuter pass was run. Local scripts, objects and instruction diagnostics are ignored under build/t24005. The earlier default proof was assembly fallback only; the closure proof below is unguarded C.


## Closure: narrow-selector lifetime

With `u8 temp_t4`, insert `if (temp_t4) {}` immediately after the masked assignment and before the switch. The test disappears from machine code but separates the mask and switch-index temporaries: the mask and bound compare use t4, and the shift writes t5. Full-unit resolved score is 0. This is explicitly marked FAKE; the original source remains unknown. The two self-stores and the minimal K&R call view remain unchanged.

The closer ran 14 sequential full-unit probes and stopped at the first zero. With a u32 selector, empty tests of the selector or selector/call-result equality before or after the switch give 72; an empty test of only the call-result local gives 66. Explicit selector bounds around the switch give 182. The first u8 probe, the empty truthiness test before the switch, matches.

Verification: `python3 tools/funcdiff.py --unit BUNKAKEN --version default --resolve func_80143DD0` reports `MATCH (linked function bytes equal target (3232 bytes))` with the guard and fallback removed. `ninja -j1 build/ovl/BUNKAKEN.ok` passes the overlay SHA1. Headers and globals checks pass; the prototype branch guard reports 0 new / 15 known disagreements. No compiler flags, generated assembly, or toolchain passes changed.
