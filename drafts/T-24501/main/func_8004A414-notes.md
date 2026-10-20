# T-24501 circular GPU grid lifetime packet

Source-only, **not matching**. Production src/main/80049FF0.c stays ASM. Function is 1,024 bytes. Private target is asm/nonmatchings/main/80049FF0/func_8004A414.s and immutable MAIN executable; no raw dump is committed. Donor func_80049FF0 supplies typed primitive/GameState/ordering-table access, and func_80046754 supports natural geometric loop reconstruction. This does not reuse the exhausted func_8004A1A0 setup framework.

## Reproduction

Run in existing Docker image from repository root, usual work/disc mounts:

    python3 drafts/T-24501/grid_replay.py

Default full-TU resolved cost 164, target/candidate 256 words, frame 0x98, x/y homes 0x8C/0x88. This is diagnostic relocation normalization, not literal proof. Replay stores its private complete TU/object/diff under build/scratch/t24501-grid; no production source edits. `--source PATH` permits the retained controls. Whitespace-only indentation cleanup of the archived body changes no statement lines or expressions; no post-freeze matching compile was run.

## Recovered source

POLY_GT4 (0x34 bytes), GameState pointer, masked/clamped signed grid steps, repeated square-bound expressions, circular squared-distance test and two RNG calls through the texture lookup arguments. Primitive coordinates use target s16 narrowing of x/y and dimensions, with full-width signed halves. All UV/RGB fields and transfer order are retained. The nested second rand() is evaluated before coordinate loads by this compiler's argument evaluation; an explicit second RNG local is unnecessary and inflated the first source's lifetimes. No claim of a runtime semantic bug in that nested expression remains.

Natural bound expressions, rather than named min/max locals, restore the target's ugen bound registers. Inlining squared-distance/radius expressions removes duplicate explicit-local and optimizer-spill ownership and reaches the exact frame. Moving the two real pointer declarations before x/y restores their observed homes. No arbitrary padding, volatile or forced registers, no compiler modification.

## Bounded controls

| Full-TU source | Cost | Target/candidate words | Frame | Finding |
| --- | ---: | --- | --- | --- |
| Original T-17000 raw draft | 390 | 256/260 | 0xC8 | Initial integer-pointer/scalarized baseline |
| Inherited loop with typed fields and GameState pointer | 379 | 256/257 | 0xC8 | Typing alone does not repair lifetimes |
| Clean typed, explicit narrow/distance/RNG locals | 353 | 256/257 | 0xB8 | Excess source locals |
| Existing expressions, guarded grid loops | 353 | 256/257 | 0xA8 | Named bounds still promoted/duplicated |
| Direct GameState fields (old bound form) | 370 | 256/260 | 0xA0 | Loses target cached-base behavior; rejected |
| Natural bound expressions | 222 | 256/256 | 0xA0 | Bound arithmetic/registers restored |
| Natural squared-distance expressions | 174 | 256/256 | 0x98 | Exact frame, x/y homes +8 |
| Pointer locals before x/y (primary) | 164 | 256/256 | 0x98 | Exact x/y homes |

Private checkpoints: .scratch/t24501/grid222.c and grid174.c; primary measured .scratch/t24501/grid.c. Earlier clean guarded source grid_guarded.c; typed inherited grid_inherited_typed.c. Original baseline is drafts/T-17000/main/func_8004A414/body.c. Corresponding grid_{original_fulltu,inherited_typed,guarded_loops,expression_bounds,distance_expressions,pointer_homes}.diff files remain private. No old pointer/base/padding grids or broad type sweep.

## Exact residual

First 35 resolved rows agree. At rows 35/37 target stores max_x(t4) to SP+0x7C, then x(v0) to SP+0x8C in the branch delay; candidate stores x first, max_x to SP+0x70 in the delay. This is source lifetime/scheduling recovery, not a proven compiler defect.

| Object/range | Target | Candidate |
| --- | --- | --- |
| Frame | 0x98 | 0x98 |
| x/y homes | 0x8C/0x88 | 0x8C/0x88 |
| max_x CSE spill | 0x7C | 0x70 |
| min_y/max_y CSE spills | 0x78/0x74 | 0x6C/0x68 |
| dx squared/radius squared spills | 0x6C/0x68 | 0x60/0x5C |
| ordering byte offset | 0x50 | 0x54 |
| narrowed x/y registers | s5/s3 | s4/s5 |
| GameState base | fp | s6 |
| narrowed width/height | s6/s7 | s7/fp |
| shade 0x80 | s4 | s3 |

Remaining primitive arithmetic and access/control shape is recovered, but constant/store scheduling and texture lookup argument scheduling also differ with these lifetimes. No further trace or variants after the pointer-home test. A bounded consult should seek a real source lifetime/aggregate representation explaining this consistent spill/rank pattern, not sweep declarations or assert a backend bug.
