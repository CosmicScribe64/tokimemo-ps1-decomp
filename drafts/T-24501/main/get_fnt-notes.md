# T-24501 get_fnt natural raster/mirror reconstruction

Source-only, not matching:23 resolved cost,258 candidate words versus259 target (1036B). Production src/main/8004C3F0.c remains ASM; no header/prototype/compiler change. Private target asm/nonmatchings/main/8004C3F0/get_fnt.s, starts8004D6B8. Original source drafts/T-17000/main/get_fnt/body.c. Related matched donor srn_vram_set in same TU and new typed caller packet mod_trans_vram explain byte raster format; mod_trans remains ASM/frozen.

## Proven object and operations

Both mirror branches memcpy256 bytes from caller-provided output into a local temporary at targetSP8C. Therefore buffer extent256 is measured, not frame padding. Base extraction copies width/divisor bytes per row, caller can request width16/divisor1 for256 raster bytes. Caller owns and supplies output; function does not allocate or replace it. Scratch signature u8* output/source follows byte-address operations, leaving canonical ABI untouched until match.

Base source stride is128/divisor; tile column uses(index%(128/width))*width/divisor, tile row(index/(128/width))*width, each row offset shifted7 then signed-divided. Signed arithmetic and memcpy order retained.

Vertical mirror copies width bytes per row from temporary row0 upward to destination row(width-1) downward. Target cursor increments by width BYTES, not raw draft's s32* increment (fourfold scale). Target derived counter begins1, compares old value then increments in delay slot: size8 copies8 rows, size16 copies16. Docker arithmetic check confirms destination sequences7..0 and15..0. Inherited source incremented before compare, producing7/15 rows; natural for0..<width fixes that semantics.

Horizontal mirror is the target's fixed eight-byte mapping each row, not an invented general reversal: output offsets4,5,6,7,0,1,2,3 read temporary3,2,1,0,7,6,5,4. All target accesses LBU/SB, not raw draft's word casts/stores. Natural row loop lets IDO unroll by two with an odd-row prefix as target does. Both256B temporary copies retained, both flags independent and execute in order. No buffer/type/declaration grids or padding.

## Controls

| Full-TU source | Cost | Words | Frame | Finding |
| --- | ---: | ---: | --- | --- |
| Original raw draft |343|360|see private baseline|Manual unroll and invalid raster view|
| Typed bytes/256B, inherited unroll |341|358|198|Type fix alone still double-unrolls|
| Natural loops, one shared row (get_fnt-152.c) |152|263|188|Natural loops remove double-unroll; vertical induction constant hoisted across raster|
| Explicit descending index/source cursor |300|267|190|Destination additionally strength-reduced into saved pointer; reject|
| Separate vertical only (get_fnt-182.c) |182|261|190|Raster/horizontal sharing still creates raster next-index copy|
| Three distinct counters (get_fnt-232.c) |232|257|188|Direct raster addiu restored; loses target vertical logical-row copy and horizontal saved ownership|
| Separate raster/shared mirror (primary get_fnt.c) |23|258|188|Target saved roles and raster/vertical dataflow restored|

Shared mirror variable naturally produces derived next-index plus logical-row copy in vertical path and keeps horizontal row in s0. Separate raster removes unwanted constant1 across its memcpy loop. This is measured lifetime recovery, not forced allocation. Explicit cursor negative preserved as get_fnt-cursor-negative.c; no further controls after23.

## Narrow residual

All offsets hex. Target frame1A0, candidate188. Temporary target8C..18C, candidate88..188. Width stack parameter target1B0/candidate198, flags1B4/19C follow frame delta. Saved-register spill homes match: s0..s7 at14..30 and ra34. No invented locals added to fill remaining frame24B gap or move buffer4B.

Raster and vertical resolved instructions match aside from frame-derived offsets. Target horizontal zero-height branch enters duplicated epilogue after early LW ra,34; candidate branches to common epilogue without that early load. Odd-row target has MOVE v0,s5 before byte writes; candidate omits it (nop scheduling instead). Second unrolled row's target MULTU s4,t9 versus candidate MULTU t9,s4. Branch displacements follow the one-word count difference. All saved roles match including width s4, output s5, temporary s6, flags s7; raster counter s0, byteoffset s1, divisor s2, copylength s3. No local experiment on these residuals yet.

## Reproducer

Run Docker existing tokimemo-decomp-arm64 with usual work/disc mounts and repository cwd:

    python3 drafts/T-24501/get_fnt_replay.py
    python3 drafts/T-24501/get_fnt_replay.py --source drafts/T-24501/main/get_fnt-152.c --scratch build/scratch/font152

Default full-TU source retained including matched neighbours/ASM/data; private scratch object and resolved.diff produced. Source-only replay, no literal match claimed. Private measured directories .scratch/t24501/font-{original,typed,loops,cursor,scopes,distinct,mirrors}; primary source .scratch/t24501/get_fnt-mirrors.c. No raw assembly/compiler dumps committed. Frozen pending bounded frame/late-expression consultation; no further source forms without evidence.
