---
type: research
updated: 2026-10-15
---

# TACO boss actor transform family

Bounded diagnostic pass [[tickets/T-13800-taco-families]] / [[tickets/T-13801-taco-actor-family]], based on 97ec697f. **Zero accepted matches or bytes.** Production source and headers remain identical to the baseline. All tools ran in the existing tokimemo-decomp-arm64 Docker image, sequential compiles.

## Recovered source structure

The adjacent boss transforms func_8014A480 (796 bytes), func_801497EC (912), and func_801487A4 (948) share a MATRIX copy from D_80125C60, three rotation calls using D_80128880[0], a TcObj50 translation converted through TcPos input/output, then TcActor position/selector updates. Existing aggregate layouts suffice: m2c's split matrix stores and byte-offset actor fields need no new types.

Use the canonical D_80127480[0].unk18/unk1C/unk20 members for the common origin. Actor slots use arg1 + 16. Cached actor pointers, selector locals, and pair-index locals can alter the register web and stack homes even where their loads or additions are identical. In 497EC the selector advancement also writes D_8015EDB4[17].unk84[2]. Its final three dependent-object loops start at 4, 5 and 6 and advance by 3 to 22, 23 and 24 respectively.

## Measured boundaries

| Function | Best full-TU difference | Remaining issue |
| --- | ---: | --- |
| func_8014A480 | 3 rows | Final actor-base addition commuted; constant versus address `lui` scheduling pair |
| func_801497EC | 1 row | Final actor-base addition commuted (target base + index; candidate index + base) |
| func_801487A4 | 62 rows | Natural control flow correct, but register allocation and local layout still differ |

For 497EC, two loop-counter declarations before the position/index scalars recover the observed unused local slots. Reuse the second loop counter for the third loop; removing the cached selector then gives the exact frame and spill homes. This is diagnostic source reconstruction, not a verified match. The final direct array reread instead folds a load, changes the register web and shrinks the frame by eight bytes.

For 4A480, removing the cached pair index and selector gives the observed 128-byte frame. Mixed pointer-addition source forms recover all but the three rows above. Signed/unsigned casts, byte-address and integer-address final reads, xor-zero and line-grouping trials do not solve the residual. Do not repeat these variants without a new compiler/lifetime hypothesis.

The scratch-only named comparisons at D_801274D0, D_80127520, D_80127570, D_80127610 and D_801277F0 require canonical main declarations if a future match is accepted. None were added to production; no shared header expansion is justified by an unsuccessful candidate. The temporary forward declaration for existing overlay function 5163C was also removed.

## Reproduction and evidence

Isolated checkout: /Users/lila/Documents/Projects/tokimemo-wt/taco-13800, branch codex/taco-13800. Ignored scratch build/t13801 contains candidate bodies, full resolved diffs, bounded experiment scripts, and reprobe.py. From that checkout run `docker run --rm -v "$PWD:/work" -w /work tokimemo-decomp-arm64 python3 build/t13801/reprobe.py`. It compiles each complete translation unit and restores its original source in a finally block. Temporary declarations in this diagnostic probe are not production-approved header changes. Run the default overlay build again afterward because its objects may contain the last trial.

Best evidence: a480-storeorders.diff and 97ec-nocache.diff. Broader failures remain in the same ignored directory. No generated assembly was edited; no source guard or FAKE was installed. The actual 87A4 control flow was reconstructed from assembly because m2c's compound else-if incorrectly grouped the state-29 test with subsequent flag conditions.

## Final validation and review

The saved reprobe script reproduces the one-row and three-row results without compile errors. The restored default TACO build then exits 0 with SHA1 OK, 1419e93cbad6c0df790577701181be04e0747969; header/global checks pass. Evidence: build/t13801/final-default.log and final-verified.log. No whole-game rebuild/checksum claim is made. Independent standards review found no issues. Spec review confirms the primary matching goal is unmet; both tickets remain In Review for integration disposition. Source/header diff against 97ec697f is empty.
