# T-24501 inverse save-image serializer packet

Source-only reconstruction, **not matching**; production stays ASM. Uses the already matched loader's game.h views. No new shared prototype is committed. Both call sites (src/main/80053650.c, func_80054884 and func_80054C74) discard the return; an explicit scratch void declaration avoids their inherited implicit-int artifact.

## Replay

Within the usual Docker image/mounts, repository root:

    python3 drafts/T-24501/save_inverse.py

Default reproduces void body + unsigned byte counter: resolved cost 113, target 199 words, candidate 200, frame 0x28. `--int-return` gives the same result. `--signed-counter --int-return` reproduces 116/199. Adding `--signed-address` to that control leaves 116/199. The script writes its private complete TU, object and resolved.diff under build/scratch/t24501-save-inverse; that full TU is an ephemeral build product, never committed. No production compiler change.

## Source recovery and boundaries

- The same matched loader views prove 20-byte Work+0x2C and 24-byte Work+0x40 objects, serialized at bank+0x28/+0x3C. Their copies are whole aggregate assignments. Last-byte protection flag and all existing name/bit/word fields retained.
- Natural byte-at-a-time checksum loop unrolls to four LBU/add operations in the target. The inherited raw draft's LW for the first byte is wrong. Manually writing four bytes in C double-unrolls (171/246 with signed counter); separate last-byte/partial-sum variables fail (190/247). Explicit signed casts on byte additions leave 171/246, with all actual reads still LBU.
- Unsigned byte counter prevents merging the loop bound's 0xF00 constant with the signed selector multiply, restoring target shift/sub/shift. Signed counter shares the bound and uses multu/mflo. Signed integer image-address control alone does not prevent this merging.

## Remaining narrow questions

The first 45 resolved instruction rows agree. In the 20-byte copy, source/destination address registers and transfer offsets/order agree, but the second transfer temporary is t5 where target uses t4. The following 24-byte copy's addresses are then one ring slot higher, and its alternating transfer register differs. This is an unrolled 20-byte copy preceded by a conditional byte remainder store; it is distinct from the earlier negative MASTER long-copy end-pointer consult. A shared cause is unproved; do not assume a compiler bug or reopen global copy-direction rules.

Checksum source now has the correct byte accesses, four-way unrolled additions, bound, pointer stride and frame. Remaining ring assignments differ, and its final checksum store/epilogue ordering produces one additional word (candidate trailing nop). Void versus s32 does not change this. No dummy padding, forced-register variables, volatile, inline assembly, or new compiler pass was attempted.

Private controls retained at .scratch/t24501/save_inverse{171,116,113}.c and corresponding save_inverse_{typed,byte_loop,unsigned_count,void}.diff. No raw instruction dump is committed. Consult should isolate the unrolled copy's skipped temporary and/or final store scheduling with at most a few causal source hypotheses; do not run a source-spelling grid.
