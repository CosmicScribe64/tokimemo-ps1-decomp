# T-24501 circular GPU grid lifetime packet

Source-only, **not matching**. Production src/main/80049FF0.c stays ASM. Function is 1,024 bytes. Private target is asm/nonmatchings/main/80049FF0/func_8004A414.s and immutable MAIN executable; no raw dump is committed. Donor func_80049FF0 supplies typed primitive/GameState/ordering-table access, and func_80046754 supports natural geometric loop reconstruction. This does not reuse the exhausted func_8004A1A0 setup framework.

## Reproduction

Run in existing Docker image from repository root, usual work/disc mounts:

    python3 drafts/T-24501/grid_replay.py

Default full-TU resolved cost 132, target/candidate 256 words, frame 0x98, x/y homes 0x8C/0x88. This is diagnostic relocation normalization, not literal proof. Replay stores its private complete TU/object/diff under build/scratch/t24501-grid; no production source edits. `--source PATH` permits the retained controls. Whitespace-only indentation cleanup of the archived body changes no statement lines or expressions; no post-freeze matching compile was run.

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
| Pointer locals before x/y (pre-consult) | 164 | 256/256 | 0x98 | Exact x/y homes |

Private checkpoints: .scratch/t24501/grid222.c and grid174.c; primary measured .scratch/t24501/grid.c. Earlier clean guarded source grid_guarded.c; typed inherited grid_inherited_typed.c. Original baseline is drafts/T-17000/main/func_8004A414/body.c. Corresponding grid_{original_fulltu,inherited_typed,guarded_loops,expression_bounds,distance_expressions,pointer_homes}.diff files remain private. No old pointer/base/padding grids or broad type sweep.

## Pre-consult residual (164 control)

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

## Adopted Astra correction and current residual

Parent approved adoption of /private/tmp/tokimemo-grid-consult/for_loops/body.c, SHA256 ec73d70b8675eccac913b90c663cbb8fa5a7a4d91c9bd465b0f486143dc87aea. The original packet source remains private at .scratch/t24501/grid164.c. Consult tested exactly three hypotheses: inline half_w/half_h/depth_offset arithmetic removes three real declarations (136/256); moving g initialization to first use is identical to that control; outer for loop additionally repairs initial stores (132/256). No padding/declaration permutation. Current source adopts this for_loops body unchanged.

Independent ordinary full-TU replay confirms 132/256, frame98 and first 40 resolved rows exact. All invariant homes now match: max_x7C, min_y78, max_y74, dx squared6C, radius squared68, depth50, x/y8C/88. The old spill table above is historical only.

Focused allocator trace:

    python3 tools/regdiff.py main:func_8004A414 --src .scratch/t24501/grid132-full.c --scratch .scratch/t24501/grid132-trace -v

Use existing trace uopt via /scratch/r9-uopt/variants/trace (usual parent rs mount, TOKIMEMO_SCRATCH=/scratch). Complete-TU input is the production file with the source-only body replacing its ASM marker. Instrumented .scratch/t24501/grid132-trace/func_8004A414/draft.o and ordinary .scratch/t24501/grid132-normal/guarded.o have byte-identical resolved function instructions, 256 words, both cost132. Trace fidelity established before interpretation. Private trace report is .scratch/t24501/grid132-trace.log; raw trace/full TU are not committed.

| Range | Bit | Priority | Uses/defs | Allocation order | Candidate register | Target register |
| --- | ---: | ---: | --- | ---: | --- | --- |
| Shared shade0x80 |153|240|12/0|6|s3|s4|
| Narrow x |56|200|5/1|7|s4|s5|
| Narrow y |62|200|5/1|8|s5|s3|
| GameState base |148|180|9/0|9|s6|fp|
| Narrow width |67|150|2/1|10|s7|s6|
| Narrow height |72|150|2/1|11|fp|s7|

Current priority order causes these colours before allocation. Target register order alone does not reveal original priorities or justify extra uses or rank forcing. No allocator malfunction is evidenced. Trace also classifies remaining six scheduling differences as order-only with no missing operation; constant/store and CLUT argument scheduling remain. No source variant follows from this trace yet.
