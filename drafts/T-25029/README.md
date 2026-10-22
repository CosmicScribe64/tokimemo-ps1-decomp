# T-25029 strCallback small-copy mechanism

Base624541ad3, isolated codex/t25022-reload worktree. Exclusive source claim transferred from8004F870.c to800563F0.c. No production source, header or build-tool edit. One worker; no Astra or new subagents. This is mechanism evidence, not a match or coverage credit.

## Baseline and fidelity

Read T-25001/T-25010 and frozen negatives before compiling. Whole current TU through funcdiff_verify.compile_unit with NON_MATCHING enabled: target160/built160words (640bytes), aligned148, printed150. Aligned metric uses insert/delete1, substitution2. Regdiff reports75 instructions differing only in registers; its159/159 headline excludes trailing NOP. Assembly fallback was not used as C proof.

First emitted difference is instruction25: hoisted else-address LUI uses t3 instead of t2. The second rectangle word uses t2 instead of targett1. Frame38 and rectangle home30 agree. Prefix through D_80125C58 loadt9 and first copy wordAT agrees.

Private evidence is build/T-25029/: baseline/, trace/, capture/, ugen-state/. The recording wrapper delegates to the unchanged production ugen_pass; its callback resolves identically to the baseline. Existing T-24067 logging diagnostic was reused privately, changing only native logging filter from size8/source75E to size8/source28 and wrapper support for the current normal ugen shim. Reference hash stays4079660f9ebb068a791c6f54abb67cf7c3bb792207c89a6eb9ac2d9e146341ec. No emulated memory/register/allocation write was added to the logger.

Same-input stock and logged entire TU objects are byte-identical, SHA256071fba1b4e008445993d119f60634a71859ce3a11eddb2a8fc2a99fd111d6c93. Logged callback resolves160words with zero aligned difference from the normal guarded baseline. The old logger's optional section audit expected a rodata section absent from this TU; a separate whole-object equality audit completed successfully. Do not mistake that section-assumption failure for a code-generation mismatch.

## Actual copy input and output

Optimized input to ugen contains one8-byte Umov, source alignment16bits and destination32bits. Its destination is the preloaded address of local rect in a0. Source expression is the pointer loaded from D_80125C58 plus40bytes, the actual dec.slice offset. A preceding unal record remains. Normal ugen expands this directly into unaligned word-load macros from offsets40/44, first resultAT and second resultt2, with stores at0/4 of a0. As1 later expands/schedules them; the discrepancy already exists before assembly.

Normal copy-state shim versus raw original ugen on the same optimized input produces identical callback binasm records. Thus the existing sentinel state shim does not consume the extra ring slot here. Private input controls, never production code:

- Source offset40 changed to0: second word remains t2 and subsequent temporary sequence unchanged.
- Source alignment16 changed to8: same second wordt2 and subsequent sequence.
- Source alignment16 changed to32: loads become aligned LW, but second wordt2 and sequence remain.
- Copy length8 changed to4: second word is absent, but subsequent first arithmetic load still uses t3, as with the full8-byte copy. This is a fixed reservation, not a temporary caused by loading the second word.

These are backend diagnosis controls only, not valid callback reconstructions. No source casts, width/declaration/ternary grids or return-ABI changes were tested.

## Logged mechanism

The existing small-copy evaluator reference and same-input logger identify the reservation precisely:

| Stage | Free-list head/tail | Used-list head/tail | Backend slot+232 |
| --- | --- | --- | --- |
| Before destination |25/24|72/72|00010000|
| After destination |25/24|72/72|00010000|
| After source |8/24|25/25|00010000|
| Before copy selection, after double dummy |10/25|72/72|00010000|
| After second-copy scratch |11/25|10/10|00010000|

Register25=t9,8=t0,9=t1,10=t2;72 is the list sentinel. Destination evaluation does not spend a temporary here; source pointer load spends t9. The following **two dummy allocations/free operations** consume t0 andt1, and the second actual copy word is then allocatedt2. The target is consistent with the one-dummy path, whose second copy scratch would bet1 and next else-address scratcht2.

The non-NULL destination path in f_eval_mov skips the slot+232 assignment at42A930, then reads the reused slot at42A934. The observed value10000 makes the signed small-offset/length checks choose L42A9E0's double-dummy path rather than L42AA44's single-dummy path. This happens before actual copy-word selection. Reference and logger are the existing T-24067 mechanism, now measured on this exact callback rather than assumed from a sibling.

The bounded named-helper writer logger reports last candidate f_loadstore:L4278FC, value00010000, current00010000, age5 before destination evaluation. This is that helper's saved gp overlapping the reused frame address. The slot is unchanged through both address evaluations. The named-helper trace does not rule out an unobserved intervening same-value write, so retain its candidate-writer qualification. T-24067 already documents the analogous branch-writer/single-burn and scalar-store-writer/double-burn cases.

## Source consequence

No expression in the rectangle source address, source alignment, second-word count, callback return type or SDK call ABI is established as the extra consumer. The measured consumer is the tiny-copy evaluator's fixed dummy reservation selected by reused backend stack state. Earlier required scalar load/store emission supplies a10000 candidate; current evidence does not establish a faithful source rewrite preserving those operations while selecting the one-burn path. Do not initialize/patch the compiler slot, add a dummy store, rotate registers, change build rules or claim an older compiler is required.

Therefore zero new source hypotheses were executed: the trace does not justify one that preserves required behavior/instructions. Prior RECT source casts, odd-width/declaration/ternary controls remain frozen. Root can now compare this proven mechanism with its T-25028 findings. Production guard remains unchanged. Full executable hashes were not rerun because production inputs are unchanged.

## Exact consultation history and bounded follow-up

Searched tracked wiki/drafts and read the prior exact callback consultation at /private/tmp/tokimemo-strcallback-control/RESULT.md plus /private/tmp/tokimemo-small-copy-control/RESULT.md. That consultation only tested sentinel bypass, with the same object hash071fba1b...; it did not diagnose the f_loadstore-to-eval_mov reused-slot path. Its negative result is reused rather than interpreted as evidence for a source workaround.

Root authorized one read-only Astra consultation on the exact new packet. Sol idled during it. Astra recommended zero new probes after inspecting source, optimized ucode and the existing backend reference. Actual ordering:

- Existing conditional jumps at captured records277 and281.
- Required callback helper call at285.
- Required zero store and odd reload/mask at288–293.
- Common label and copy setup at294–301; rectangle Umov at302.
- Next required conditional jump at307, after the copy.

Neither predecessor branch can provide a branch-helper slot value after the subsequent required scalar operations. The following branch is too late. f_jump evaluates its predicate before invoking its branch emitter; the relevant small-value spill is in f_emit_branch_rrll after that evaluation. Moving the predicate into an expression does not establish a pre-copy emitter route while keeping the target branch after the copy. Adding a branch between scalar operations and copy would be new/redundant control, and duplicating the copy into arms has no established path back to the target single pre-branch copy with altered helper history. Thus no novel justified source hypothesis followed, and Sol ran zero further source probes.

The selective named-helper last-writer caveat remains. This result bounds the requested required-branch mechanism; it does not prove all source forms impossible, exclude uninstrumented same-value writes, justify initializing backend memory or support a compiler exception. Callback behavior, void ABI and production guard remain unchanged.

Pinned inputs for reproduction (full list in private build/T-25029/hashes.json): source800563F0.c SHA695098aef69493e320853119d58162be12ab46506f45f16f0d00d2b3537c0dd0; optimized ugen input13c50c67d469f418e6db99fa208126aaf0a5a1ebcd1e570b62348b128fa87b58; shim/raw binasm identical9a2461a8333a0ddf4f528832c1b5cf6332bb9d885898b14e808ff21dfdfb7330. Private native logging source7ada69bed1554ab73a61e5158a4c3c074785cb1f14bc3524830bc4de7fa37e3b, logging binary98c42695669122bdd05cce8dcdaba69cc59fde85c9010831e3452fb7700651e8. Full-TU capture/log scripts remain private diagnostic artifacts, not build tools.
