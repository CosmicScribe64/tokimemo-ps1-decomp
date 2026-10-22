# T-25024 TEL qualification

Pinned base: `883760224a9ff9712c4850ca7662c5ce3df2625d`. Exclusive target: TEL `func_80136724`, 6,580 code bytes / 1,645 words. Production source remains unchanged and uses INCLUDE_ASM. No match or C coverage is claimed.

## Sources and metric

`hand-baseline.c` adapts tracked T-23001 hand.c. `pointer-baseline.c` adapts tracked T-20830 best.c. Both retain the original logic and current GameState members. Adaptation replaces the obsolete TelApptWord/type-member spellings with existing TelDateEntry and resolves old helper names through symbol configuration (including rand/strcpy/strcat). No header change is needed. The existing T-19022 padding FAKE is retained. Original declarations, buffer extents and pointer use are reconstructions, not proof of the original source.

Primary metric is **changed ordinal instruction positions**: compare complete-TU resolved normalized keys at identical ordinal positions, counting unmatched tail positions once. This intentionally penalizes insertion shifts. Secondary metrics are funcdiff printed lines and aligned insertion/deletion cost (substitution costs two). They are not interchangeable. None proves literal linked bytes.

| Source | Target / candidate words | Changed positions | Printed lines | Aligned cost |
| --- | --- | --- | --- | --- |
| T-23001 hand baseline | 1645 / 1643 | 1372 | 1048 | 1038 |
| T-19022 frame draft, adapted diagnostically | 1645 / 1646 | 1422 | 1401 | 1035 |
| T-20830 pointer baseline | 1645 / 1643 | 1403 | 984 | 970 |
| Pointer + void rank-helper ABI control, rejected | 1645 / 1643 | 1405 | 988 | 974 |

The pointer body has lower aligned cost but **more changed positions** than the hand body. Preserve both. This is qualification, not a byte-coverage improvement. JSON files record exact candidate and complete-TU source SHA256. The frame adaptation was exploratory and is not selected; its input is tracked `drafts/T-19022/TEL/func_80136724.c`, with the header/type definitions removed and TelApptWord/member names mapped to TelDateEntry. Its SHA256 is in frame-result.json.

## Recovered-body audit

Private recovered input: `/private/tmp/t25016-recovered-20261022/shape/beds/m2c/func_80136724-body.c`; queue at `corpus/QUEUE-merged.md`. Neither is copied here.

- `D_800E738D - D_800E6280 = 0x110D`. Current `include/main_api.h` declares `u8 unk_110D`. Original entry at 0x80136740 uses LBU; increment/store at 0x80136844/54 uses LBU/SB. Both archived candidates use the field. The undefined-symbol failure is obsolete migration debt, not evidence of proximity.
- Raw `? sp68` and `? sp48`, the scalar `u8 sp78` used as a string buffer, and `(sp + girl)->unk68` are semantic holes. Existing matched `src/main/8004F870.c:func_80053418` writes 16 bytes and 16 halfwords; matched TEL `src/ovl/TEL/8013CCD0.c:func_8013CCD0` uses those arrays. Target passes sp+0x68 and sp+0x48, then loads one rank byte. Candidates use u8[16], s16[16] and indexed rank. The 0x104 text extent is inferred from adjacent frame homes, not established by a bounded string-write proof.
- Raw sp44/sp45/sp46/sp47 scalar aliases reconstruct one packed 32-bit entry, not independent initialized scalars. Existing TelDateEntry and matched GEKO `src/ovl/GEKO/80142760.c:func_80143F48` supply the nine field widths and by-value GsWord ABI; matched MAIN `func_8006B5BC` consumes it. All 32 bits are assigned before the call. Candidates preserve target field assignment order.
- Raw case0x40 goto/var_t2 paths contain a dubious byte-offset dereference. Current D_80141D80 is a pointer array; candidates use D_80141D80[selector] and the two conditional selector=12 arms. Raw D_80141134 flattened indexing becomes the existing byte[][21] table. Target scaling agrees. Raw missing case0xFF is restored with default, as target dispatch explicitly tests0xFF.
- No M2C_ERROR occurs in the raw body; the unknown types and stack expression remain hard holes despite that. Neither archived candidate has M2C_ERROR, unknown types, invalid sp expressions or unresolved old GameState names.
- Address accounting finds 229 calls, 30 distinct callees, with the same address multiset in raw, both candidates and target JALs. All source callees resolve via current symbols/headers. Nested call expressions mean textual order is not execution order. This is accounting, not proof of argument ABI or whole control flow. Manual checks cover rank buffer pointers, the packed GsWord call, string pointers, menu's signed halfword arguments and four-argument TEL renderer. Other K&R signatures remain as currently declared; no new ABI was invented.
- The two matched MAIN rank callers with void views motivated one bounded ABI control. It worsens the pointer metric; the same-overlay matched caller retains the current s32 view. No override is selected or added to production.

## Earliest remaining divergence and limits

All exact comparisons first differ at zero-based word 12: target BEQ 0x10410647 branches to 0x80138074, candidate 0x10410645 to 0x8013806C; this dispatch displacement reflects the candidate being two words shorter. For the pointer body the first difference excluding branch displacements is word 85, target t2 vs candidate t1 (register-only). Register/branch-offset-masked alignment has17 hunks (hand25).

The pointer's first structural opcode difference is word 326, target address0x80136C3C: target NOP after the eligibility-table LBU, candidate `move v1,zero`. Target emits that zero assignment at word 331/address0x80136C50; candidate schedules `total=0` earlier using a separate range from the minimum calculation. The branch and table load remain. This is a moved initialization, not a newly established semantic defect. Subsequent differences include a removed redundant girl-address LUI at target word 392 (the field load remains), packed-entry load/store scheduling beginning word 455, an address-add scheduling move at507 and one case0x40 address setup move at954. No missing target load or new branch condition was found in this structural audit.

Frame remains0x198; rank, points, entry and total homes remain0x68,0x48,0x44,0x17C. No repeated month-promotion, volatile, declaration or generic source-shape probes were performed. T-24022 already exhausted the relevant month and broad state-pointer controls. Remaining allocation/scheduling residue needs a new causal source/read/ABI hypothesis; none was justified by this qualification. Do not infer that compiler changes are necessary, or promote this large draft.

## Replay and context

Run from repository root inside existing image `tokimemo-decomp-arm64` with repo at /work and original disc at /work/disc read-only. Image ID at measurement: `sha256:4eb5a43f07957beae672ee446446ddda1478e2e25b80a8c50a2b9833a0186fd1`.

```
python3 configure.py
ninja -j2 build/ovl/TEL/src/ovl/TEL/80132000.o
python3 drafts/T-25024/replay.py drafts/T-25024/hand-baseline.c
python3 drafts/T-25024/replay.py drafts/T-25024/pointer-baseline.c
python3 drafts/T-25024/replay.py drafts/T-25024/pointer-baseline.c --void-rank
ninja -j2 build/ovl/TEL.ok
```

Replay compiles the entire current production TU with only this target's INCLUDE_ASM replaced, NON_MATCHING false. It uses `order_search.compile_variant`, current preprocessing, default IDO5.3 and existing cvt/ugen/frame/trailing-pad/object-layout passes. Flags: `-c -EL -O2 -mips1 -G 0 -non_shared -Wo,-nokpicopt -cckr -Xcpluscomm -Iinclude`. No fast/single-function packet compile. No per-function flags. Generated objects/ucode/asm/full contexts stay private under build/T-25024.

Fresh default TEL checksum passes; this validates unchanged production with ASM fallback, not either C body. No all27 build was run: there is no default C promotion. Header/global/prototype guard results are recorded in the ticket. Source, context and compiler-tool hashes are in context-sha256.txt. Recreate from the pinned base to obtain the recorded complete-TU hashes.
