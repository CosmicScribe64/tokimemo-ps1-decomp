# T-25025 GYOZI zero reaching-definition checkpoint

Exclusive source `src/ovl/GYOZI/80140C90.c`, func_80140E0C.
Base 883760224 plus source-unchanged BUNKA checkpoint a9608aeba.
No production edits, new source variants, compiler changes, or consultation.

## Consistent measurement

Docker, existing tokimemo-decomp-arm64, full TU and current declarations.
`python3 tools/funcdiff.py --resolve GYOZI:func_80140E0C` reproduces:
97 target versus 96 candidate instructions, five printed +/- lines and
aligned cost 5. This is one missing entry instruction plus two substitutions,
not five changed positions in equal-length arrays. Earliest divergence is
0x80140E24: target materializes zero in a1, candidate omits it. The later
store uses zero rather than a1 and call argument uses zero rather than a1.
The target spans 388 bytes. No C match or coverage.

Current source SHA256:
`26c661cc6b8626da8b2523559be4b47c8dd7ac38badbc9a8d45ea0390853eb8b`.
Archived T-24907 v6 was inserted into a private full-TU copy, retaining all
current headers and other functions. Its full-TU source SHA256:
`6973fea1ed3063b5ec1001923f99458a0ec357e70f2e4af49803bcf949a5cda2`.
It reproduces the identical five printed lines / aligned cost 5 / 97:96
instruction result. Historical three-word and recovered six-row figures do
not use this convention; no numerical improvement is inferred from them.

## Measured definition/use boundaries

1. In v6's input to uopt, z has memory location M/block23/offset-8, one
   entry zero store, and a load at the merge converted/narrowed into the
   D_800F62CF store. The first call argument reads that global through the
   preserved byte view. Thus the input genuinely contains the requested
   reaching local definition; it is not absent because of packet extraction.
2. The native expression table after copy propagation/redundant-store
   processing retains the old z store and the old global-store(z) expression
   (bits 0/2 and 22), but also contains a new global-store(literal-zero)
   expression at bit76. These retained graph entries alone do not establish
   liveness. There is no z live range in the allocator R9SAVE records or
   assigned colours. The global index itself is bit20, constrained colour4
   (a1); it has loads/stores at the merge and later call blocks.
3. Optimized ucode has no M23/-8 z store or load. At the merge it emits
   literal zero, narrows it, and stores to R5, then loads R5 into the first
   argument R4 and writes the global via the rstr operation. This is a new
   merge value, not the original entry z definition. ucode R5/R4 here are
   physical a1/a0 register locations, distinct from allocator colour numbers.
4. Stock as1's initial-node list already has sb zero and move a0,zero in
   the merge/call block and no entry move a1,zero. This is before local or
   global as1 scheduling; another as1 filler hypothesis cannot restore the
   missing entry definition. The fresh stock compile and traced compile
   match .text/.rodata/.rel.text/.rel.rodata byte-for-byte.

This answers the requested branch: the v6 local carrier does not reach
allocation as a live entry definition. It does not prove every possible
source carrier must fail. Zero knowledge persists in the merge register and
is substituted by hardware zero before as1 receives the block.

## Exact remaining question and stop condition

No untested, evidenced source mechanism supplies a nonconstant reaching
definition while preserving the target's instruction count and entry CFG.
Do not rerun zero-local, promotion, assignment, comparison or syntax grids
from T-24907/T-25011/T-25013/T-25016.

For a further phase audit, identify which uopt statement-rewrite/dead-use
operation replaces the live z-to-global edge by bit76 and removes z before
allocation, and which ugen constant/register-value rule replaces the merge
R5 uses by hardware zero. A retained old expression-table node is not proof
that disabling allocation/coalescing would recover it. No phase-specific
instrumentation or compiler alteration was attempted here. Resume source
experiments only with a genuinely new reaching-def mechanism supported by
that evidence, with no extra target instructions. No automatic Astra.

## Private evidence and reproduction

All analysis ran in Docker. In `/private/tmp/tokimemo-t25023/build/t25025/`:
`current/` and `v6/` contain full preprocessed TU, input draft.B, native
draft.o.trace, allocator draft.log, and output objects. `v6/draft.B.after`
captures optimized ucode before cvt post-processing; the zero path is not
rewritten by that post-pass. `ucode-function*.txt` are private decoded
fourth-procedure excerpts. `v6-as1/` contains stock G/T and initial-node
trace. `measurements.json` and `fidelity.json` record the metrics and checks.
Private scripts `build/t25025-{trace,inspect}.py` use existing regdiff and
uopt_eval capture machinery plus the existing read-only trace binary at
`/private/tmp/t24047-uopt/variants/trace`. Raw data/listings stay private.
