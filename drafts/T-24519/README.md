# MASTER344E0 sibling transfer

Base9013099115dc8cc3ff07a98061a39d32ff95fe0c, branch codex/t24519-master-sibling,
isolated /private/tmp/tokimemo-t24519. Production source/headers/tools/config are
unchanged. This is a structural source checkpoint, not a verified C match.

Read the current function catalog, T24048 history and T24517 recovered363F4 loops.
The retained body starts with T24048 selected.c and replaces only the clone/point
initialization region with indexed loops from363F4. It removes the three declared
pointer locals, retaining the existing real index i and RNG-result value locals.
All344E0 reward branches, check_end_k conditions, late actions and calls remain its
own. Neither donor is assumed to be a matched function.

| Diagnostic | T24048 control | Indexed transfer | Target |
| --- | ---: | ---: | ---: |
| Aligned cost |428|338|0|
| Instruction positions |730|728|728|
| Frame bytes |72|64|64|
| Leading equal diagnostic positions |2|289|728|

The target clone spill at sp34, later reused by speed, and point spill sp30 now
agree exactly: clone store/reload at354/357; point store/reload370/382; speed
store/reloads372/375/383. These are zero-based instruction positions. The target
clone pointer sltu/bnez comparison and point-loop call/store order are restored.

Corrected masks report171 register-only rows and0 other rows over728 positions.
REGIMM condition bits and syscall/break code bits are retained; COP formats are
asserted absent. funcdiff normalizes section-owned strings/tables, so this is
not literal linked-byte equality. The original first289 diagnostic words agree.

The first residual is the already-consulted aligned68-byte copy end temporary at
row289: target addiu t5,t3,60, candidate addiu t6,t3,60, same source/destination.
No copy-operand, copy-ring, return-view or backend grid is repeated here. The
initial copy/clone region has changing temporary mappings; later region430..727
uses one consistent target-to-candidate bijection across calls:
t0->t5,t1->t4,t2->t2,t3->t3,t4->t6,t5->t7,t6->t8,t7->t9,t8->t0,t9->t1.
The byte-counter v1 ranges already agree. No independent later source issue is
established, and arbitrary register remapping is not a fix. No backend defect or
source impossibility is inferred.

## Replay and review

Use the existing Docker image, this checkout mounted at /work, original disc at
/work/disc:ro. After configure and the MASTER target build:

    python3 drafts/T-24519/replay.py

The driver inserts only this narrow body plus the required existing overlay
forward declaration into current context. Output is ignored build/t24519/checkpoint.
Fresh unchanged MASTER SHA1 and header checks pass; the narrow body compiles.
No full27 rebuild is warranted for this archive-only checkpoint with unchanged
production. Parent base carries the verified integrated production provenance.

Standards/spec self-review found no unresolved issue: C89, no inline assembly,
compiler patch, new FAKE workaround, raw ASM/binary/ucode dump, duplicated context
or unintended production change. Ticket remains In Review for coordinator review.
