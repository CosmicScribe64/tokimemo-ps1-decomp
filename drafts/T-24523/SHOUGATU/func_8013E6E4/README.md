# SHOUGATU func_8013E6E4 source checkpoint

No match and no coverage. Production remains INCLUDE_ASM. Based on the verified GYOZI func_80140F90 selection-loop template (636b44839), repaired GameState fields and true fourteen-byte local tables.

Full-file resolved edit cost: initial direct typed port 218; moving all three scalars before the tables matches stack buffer offsets and gives 214; sequential retry/continue conditions give 187. The old T-17500 score317 uses a different context/metric and is not a direct comparison. An integer func_800AE0B0 view improves the 214 form to210 only, not a solution. Explicit stride spellings, explicit flag local/status reuse and three placements of dead-count reuse as the later stride do not improve187. No broad permutation or compiler changes.

The remaining differences include the extra saved constant56 before the retry loop (target instead uses shift/sub for the first selected-record flag load, then reuses dead count register later), cached selector across the club filters versus original rereads, and downstream temporary scheduling. Scalar declaration order already aligns both local tables. The saved-register count differs despite equal frame sizes.

Reproduce: replace only the target INCLUDE_ASM in src/ovl/SHOUGATU/8013E330.c with body.c inside a scratch full translation-unit copy, then use tools/order_search.py make_text_scorer(resolve=True,non_matching=False). The scratch-only extern and overlay prototype must be resolved canonically before any production promotion. No instruction listing, target bytes or raw assets included.
