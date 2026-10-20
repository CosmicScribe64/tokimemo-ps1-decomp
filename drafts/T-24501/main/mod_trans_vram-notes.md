# T-24501 mod_trans_vram source-reconstruction packet

Source-only, **not matching**. Production src/main/8004C3F0.c remains ASM; no header/owner/prototype changes. Function752B, target188 words. Private target asm/nonmatchings/main/8004C3F0/mod_trans_vram.s; get_fnt target in same directory. No raw dumps committed.

## Proven real objects

The caller's glyph object is256 bytes, not raw draft s32[1]. get_fnt's target mirror paths both read0x100 bytes from caller buffer, and base raster path can write16 rows*16 bytes (cell16/divisor1). LoadSquare transfer reads2*((cell/2)/divisor)*cell =cell²/divisor bytes, max256. Candidate uses u32[64] for measured word alignment; original C element type is unknown. Extent is proven independently of frame, not padding. Target buffer SP88..188; target frame1A0.

Initializer func_8004E44C in src/main/8004C3F0.c and target multiply prove record stride20. Scratch ModRec14 view: glyph pointer table+0, tile-word pointer+4, format byte+8, unknown byte+9/halfword+A, cols/rows bytes+C/+D, cell-size byte+E, unknown byte+F and initializer halfwords+10/+12. Fields/size documented in primary source; D_801217A0 remains canonical byte-array owner. Do not change shared owner before match/overlap review.

Matched donor srn_vram_set in same TU supplies typed record and field-width/loop lessons (wiki notes/matching-06 T-17015). T-17015 did not attempt mod_trans_vram due time limit. Callee get_fnt remains ASM and has no canonical header prototype currently; its pointer args/return should not be changed globally without proof. Existing LoadSquare prototype is void(u16,u16,u16,u16,void*).

## Replay

In existing Docker image with usual work/disc mounts, repository root:

    python3 drafts/T-24501/mod_trans_replay.py
    python3 drafts/T-24501/mod_trans_replay.py --source drafts/T-24501/main/mod_trans_vram-reduced.c --scratch build/scratch/t24501-mod-reduced

Default primary direct-field switch:232/196 words, frame1A0. Reduced control242/192/frame198, not adopted. Resolved score is not literal proof. Replay retains whole TU and writes private variant.c/guarded.o/resolved.diff under scratch; production unchanged. After archive, syntax check only; no new matching compile.

| Control | Cost | Target/candidate words | Frame | Finding |
| --- | ---: | --- | --- | --- |
| Original T-17000 full TU, bad4B glyph buffer |243|188/197|A8|Inherited pointer/scalar draft |
| Typed20B record + real256B buffer |232|188/196|1A0|Proves large frame's actual object |
| Merge duplicate row/cell spill shadows |242|188/192|198|Fewer ops but loses target value families |
| Direct format-field switch from232 (primary) |232|188/196|1A0|Selector moves v0->ugen t2, targett3 |

The direct switch changes actual instructions but total score remains232. Prior temporary ring is already one behind (else cell16 t0 versus targett1), so selector's remaining t2/t3 is not another promotion failure. Preserve that causal distinction, no selector spelling grid.

## Caller/value ownership evidence

Only current C caller srn_vram_set passes descriptor0, low16 tile index, page, x, y. Record selected using oldarg0, then oldarg1 tile index must survive cell-size8/16 selection: target keeps index in s0 and derived cell-size in a1, then computes map pointer rows*index*cols. Primary follows these semantics, retaining captured cell/row families from inherited draft.

Target get_fnt's size argument reloads SP78. LoadSquare dimensions and step use the a1-derived cell-size value; target restores a1 from SP78 at inner exit. Outer row is initially a0, captured to SP194 before inner calls, used in row*cell y base, and restored at outer join. Removing the explicit saved-row and saved-size copies changes cell to a stack-only range and row spill to7C, losing target roles even though four excess words disappear. It does not establish that those two copies are authentic source variables; a proper representation of logical value families/alias scopes remains unrecovered.

## Measured primary homes

| Object | Target | Primary |
| --- | --- | --- |
| Frame |1A0|1A0|
| Glyph buffer |88..188|9C..19C|
| Ratio byte |87|9B|
| Record pointer spill |74|6C|
| Saved cell-size copy |78|84|
| Saved outer row |194|19C|

Hex SP offsets. Large frame alone is not a match. Primary196 versus188 words still has broad load/operation ordering differences around loop setup, call preservation and outer reloads. Signed cell/xoffset halves followed by unsigned division through u8 ratio must stay; switch cases/default, get_fnt then LoadSquare order, packed tile-word extraction and record reloads remain. No array-capacity/declaration/padding grid, forced register, inline asm or compiler changes.

Private controls .scratch/t24501/mod232.c (typed baseline with selector local), mod242.c (reduced copies), current mod_trans.c (direct-field primary); diffs mod_trans_{original,typed,no_spill_shadows,direct_format}.diff. Use --source on original drafts/T-17000/main/mod_trans_vram/body.c for baseline. Read-only next consultation should explain actual two cell/row value families and alias scope before more source experiments.

## Frozen sibling transfer

func_8004A414132 already has explicit x/y casts at all coordinates plus narrowed dimensions. Its trace already establishes a narrow-y range; no target-supported missing primitive-field narrowing follows from successful4A814 erased-conversion mechanism. No cast sweep or new4A414 compile was performed; saved-rank problem remains separate and frozen.
