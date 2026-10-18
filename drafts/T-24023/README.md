# T-24023 EVENT reconstruction checkpoint

No matching C claim; zero verified bytes added. Production retains `INCLUDE_ASM`.

`body.c` is the preferred source-only reconstruction of `EVENT:func_801103A0`, based on T-15200's typed body and the current translation unit at base `8052373f7`. It uses existing EventStateView fields, including the three final parameter-byte stores. No supplementary externs are needed. The experiment hides the existing void declaration only in its scratch translation unit; promotion would require a reviewed s32 declaration and caller validation.

## Independently measured result

- Full translation unit, normal IDO 5.3 pipeline, relocations resolved.
- Original and draft: 1,712 instructions; frame 48 bytes; saved masks at sp+0x2B and sp+0x29.
- Aligned insertion/deletion cost: **214**, down from **1,618** for T-15200.
- Printed diff rows: **826**; historical T-15200 row count was 1,930. These metrics are not interchangeable.
- All differences remain real; the draft is far above the production guard threshold.

Replay the saved artifact in Docker from the repository root:

`python3 drafts/T-24023/verify_checkpoint.py`

The authoritative measurement is `measurement.json`. No linked C match is claimed. The three unchanged production binaries passed SHA1, with header/global/prototype guards recorded in the coordinating ticket.

## Useful changes

1. Structured outer else-if chain, removing only the terminal common-tail jump of each arm. Nested exits are preserved; the transformer asserts the exact removal count.
2. Remove the synthetic `var_at` assignment/comma expressions from the seasonal predicate; preserve short-circuit evaluation order. Its now-unused declaration can also be removed without changing the score.
3. Conditional-expression date Booleans avoid a long-lived common subexpression that otherwise shifts register allocation throughout the function.
4. Explicit byte selector views restore the global range's allocation. This is marked FAKE, as is the redundant byte local that reproduces the stack-byte gap.
5. Signed conversions on the two initial girl arguments restore the original initial load bases. Aggregate fields replace the three legacy tail globals without changing the score.

## Remaining evidence and next hypothesis

The register-masked structural comparison first diverges at original instruction 1280, in the three date tests. The original materializes each month equality (`xor; sltiu; beqz`); the conditional expression emits a direct inequality branch and explicit zero path. Logical conjunctions, explicit integer/byte temporaries, separate result ranges and mixed comparison promotion types either retain a commoned Boolean or disturb allocation. The final common tail also places a month load differently. Several selector comparisons have their commutative operands exchanged; commuting the C operands does not fix them.

A useful next attempt needs to explain why the original recomputes each Boolean without creating the extra common-subexpression range. Repeating argument-cast or selector-view spelling sweeps is not justified. No global compiler change is proposed.

## Replay scripts and invalidated run

The scripts in this directory reproduce bounded stages in this order: `probe_event.py`, `probe_event_bool.py`, `probe_event_arguments.py`, `probe_event_shape.py`, `probe_event_combine.py`, `probe_event_selector.py`. Additional scripts test the final Boolean, comparison and view hypotheses. All run serially, from repository root, inside the existing Docker image, writing ignored `build/t24023/` outputs. `results.jsonl` contains only the corrected replay and later valid probes.

During review, the first experimental else-if transformer was found to remove nested jumps as well as terminal jumps. All dependent results were invalidated. The corrected transformer removes exactly one terminal jump per arm and asserts nested-jump preservation; the retained candidate and every result in results.jsonl were regenerated. Earlier raw outputs remain local and are not evidence for this checkpoint.
