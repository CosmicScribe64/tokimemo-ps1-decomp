---
id: T-25046
title: Match SHOUGATU func_8013EA98
status: In Progress
assignee: Astra implementation, Codex coordinator
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24523-seasonal-source-hypotheses]]", "[[tickets/T-25038-shougatu-selection-loop]]"]
---

## Goal

Match the remaining `func_8013EA98` in owned `src/ovl/SHOUGATU/8013E330.c`. Continue the same Astra worker through a verified literal match or a concrete external blocker requiring human action, without time/probe stopping limits.

## Acceptance criteria

- [ ] Default C and independently linked target bytes match.
- [ ] Affected overlay checksum, neighboring matches and declaration checks pass.
- [ ] Standards review resolves all findings; root owns combined integration before Done.

## Notes

Start from preserved clean E6E4 pin `ca9569534cca891cbb8b0bab8e279ef9035e9199`. Root reviewed its source/header diff with no findings and is integrating it with all27 validation. Do not overwrite the verified E50C/E6E4 bodies.

T-24523 records a corrected typed EA98 sibling port at resolved cost284 with copy/frame differences, but no committed source archive for that port. Locate private preserved evidence if available; the older T-17500 body/334 score is a separate stale context, not the corrected284 body. Read the exact ledger before reconstructing or testing.

Use target/callee evidence before transferring the T-25033/T-25038 access-class or return-view controls. No broad candidate screening or duplicate coordinator compiler tests. All tools/analysis run in Docker. Any additional shared-header changes need coordination; the existing debug declaration guard is reserved for root integration.

## Comments

2026-10-22: Ticket and In Progress board card created before source edits. Same Astra worker retains exclusive implementation ownership of the source file and task evidence. Root owns main integration.

Recovered the exact prior corrected-body lineage from `/private/tmp/tokimemo-t24523/build/t24523-sho-ea.c` and its generator/scorer. Target review found a remaining transplant error: `unk_722` must be0x18 (target load8013ED54/store8013ED6C), not E6E4's0x11. Fresh baseline corrects that and uses canonical `check_para_limit()`, removes duplicate scratch declarations, and retains the current verified full TU. Result:282 aligned /286 printed,232/230 words, frame0x60 versus target0x58 and tables0x48/0x38 versus0x40/0x30. Target reloads the random word every retry and keeps the club selector shared; draft hoists the random word and promotes the selector into the later status argument. Separate causal access-class controls are under test. No production body changed.

Faithful native-uopt capture (`build/t25046/trace`) reproduces stock282 cost. Input local-area `.def` is0x28: i at-2, count at-4, status at-8, tables at-0x18/-0x28. Optimized `.def` is0x38, with no surviving stack accesses to the added16 bytes. This growth precedes the uniform frame pass; declaration spelling alone is not its explanation.

Focused negative ledger: exact selector byte store, exact/array u32 random read, count reused for status and single do-while OR condition do not improve282. Count reuse changes table homes but keeps frame0x60. Status argument byte view regresses297. Typed pointer store adds a saved register/frame0x70 (276); all-selector raw-symbol and aggregate-array diagnostics regress315/311. Club-selector byte views give281 and the right count a0 but add incorrect selector moves/reloads. Volatile random is diagnostic only: it restores ra copy allocation and230 words, cost216, but uses a wrong base-relative random load and wrong live-range ordering; adding status byte view costs243. Per-call void847 cast generates jalr/address setup (302); a clean private direct void declaration is output-identical282. Thus the cast is not evidence against the direct return view, and neither is promoted.

The entry selector test regresses291; signed retry-store view274, explicit flag temporary320, comparison-width masks320/frame0x70, and a reused status local for the three selector checks is identical282. None is promoted. No shared compiler/tool changes are proposed.

A private diagnostic tracer identifies the four hidden homes: random-times-one/address share -44, extracted flag -48, selector conversion -52, previous-selector conversion -56. Faithfulness remains282/286. The selector conversion is filled before club5, shared by club6/previous, refreshed after the forced-selection store and used directly as status RPAR; target instead reloads the status argument after the merge. Removing the inherited random multiply-by-one gives215/215,231 words, restoring the target random reload and ra table-copy allocation, although the sum operands reverse and frame/ranges remain wrong. Combining it with status byte view gives240/228 words; adding byte views to the847 argument and adjacent selected copy is identical240. Next control isolates the three loop reads from call access classes. Production remains unchanged.

The combined direct-random/club-byte controls restore frame0x58 and tables0x40/0x30. Status-byte gives195 (231words). Explicit unsigned casts do not replace the byte access class (215/240); signed retry-store gives187 but introduces an unwanted reload; previous/forced choice byte views give176 (230words), signed-store combination170. First range-comparison byte view is identical176. With those access classes, a single do-while OR condition improves162 and restores G-base t1; retry-flag form costs222. A separate post-status selector read is the decisive next split: volatile diagnostic69 (231words, wrong base-relative load), existing scalar alias48 (230words, downstream arithmetic exact), plus847 byte argument44 (228words). Alias remains private: coordinator requires a full literal match and standard keep necessity proof before config authorization. Explicit fresh-selector local controls regress323/327, so do not solve forwarding. Private sources/results preserve every named control in build/t25046.

Current preserved230-word candidate improves44 to38 by moving count-loop increment into the record index (`record[i++]`); the entire count-loop schedule now matches. Exact44 trace is faithful (sourceSHA df60d15ed8dde9c9f8e946f54981ace76f28d85cfe042abc7a1f1af91e213411). Previous-choice, G-base and constant9 all tie at priority2.20000005; previous-choice spans5blocks. The missing common latch is a likely cause of its register exchange, not yet proven. Current reconstruction/negative-family summary is in drafts/T-25046/README.md. Pure representation controls, aliases at first/loop reads, assignment-index sequencing, result-cast removal and empty-if latch controls do not close the CFG gap.

Retry-latch diagnosis: putting i++ in a for-loop latch restores previous-choice t2, constant9 t0 and G-base t1, confirming the tied-range/CFG interaction. It is negative overall172: addiu moves early but signed narrowing remains at the latch. Explicit word i with s16 narrowing at every update is also negative188/frame0x60 and retains the same partial hoist. Identity latch and reuse of the existing word status local do not close it. A faithful late-zero trace shows multiplication survives uopt through the added latch, then folds in ugen/as1; selector-zero extends the wrong byte range/exit merge (68), index-zero226, count-zero54. A zero conditional with both paths retrying costs84. Discarded reads and dead local assignments are identical to the underlying acceptance-loop form; empty-if reads add code. Explicit retry flag local costs98/frame0x60. Exact first signed-byte-wrap read177; a single club direct read is identical38 while both club direct reads198. Post-loop alias copy/byte argument78. Best remains38/230; no variants promoted.

Best improves38 to36 (230/230 words, exact frame/tables) by reusing u32 status for the retry counter with explicit s16 normalization; target random-sum operand order is restored. Preserved `build/t25046/unsigned-status-retry/body.c`. A faithful for-latch trace (`trace-for-narrow`, SHA17d1361f810c48bf1c8b954ea3c7e563b5f9cd19924fc9cbc33dc562cc615462) leaves ADD/CVTL/CVT together at optimized records140–145; only as1 moves ADD early. Thus earlier descriptions of partial hoisting refer to final instruction scheduling, not uopt code motion. Unsigned for-latch176/232, u16 index/signed reads259/239, shared flag/result union86/230, redundant latch normalization169/233 and selector-zero latch addition91/237 are rejected. Exact last-flag byte indexing is identical36. Next causal control asks whether an explicit shared rejection label produces a retained CFG block before register-load insertion.

Further loop-boundary controls: explicit shared rejection label86/230, separate continue/break72/232, final short-circuit late-zero104/234 and identity106/236 do not retain the target reload at the latch. Mandatory volatile-read diagnostic82/234 retains a real common block but loads zero through the G base, so it is not promoted. Existing event-copy status argument78/234 and first threshold >8 (identical36) do not solve the range boundary. Review invalidated the old fresh-selector negative: its scratch generator also changed the forced-choice global store to a local store. Corrected first-local286/240, comparison-only276/234, full-word RHS251/235/frame0x60 and reuse of existing result RHS269/235 remain negative; these correct controls preserve the forced store. Existing local-byte home298/244 also regresses. Target first rejection bypasses t4 initialization, while current first comparison consumes raw-global RLOD and store-forwards into t4; this is the exact availability difference now being investigated. Production is unchanged.
