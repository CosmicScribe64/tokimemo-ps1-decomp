# T-25027 sndi full-TU checkpoint

Continuation from6fbe9a03e, exclusively within src/main/8004F870.c on the isolated codex/t25022-reload worktree. Production source, shared headers and tools are unchanged. No match or coverage increase is claimed.

Read drafts/T-24501/main/sndi-notes.md before work. The current tracked body is the later integer-address/u8-parity checkpoint, not the older210-row body. Reproduced it using the complete current translation unit and repository compiler/passes: target263words (1052bytes), candidate265words, aligned150/printed150. The older branch-reset draft gives260words,211/211. Aligned uses insert/delete1, substitution2; printed counts are funcdiff.diff_lines lengths. Archived canonical127/raw274 numbers are not comparable.

## Source and ABI audit

Recovered /private/tmp/t25016-recovered-20261022/shape/beds/m2c/sndi-body.c and sndi-header.h were read privately. The recovered header is a broad TU fragment, not a standalone canonical declaration source. Recovered body has five one-byte buffer placeholders, duplicated trailing strcpy arguments, compiler-managed spill roles, pointer values stored in a scalar, and a lost first found-flag join. Its scalar spD4/sp84 zero tests must become first-byte tests when those placeholders are repaired into arrays; testing an array address would be wrong.

Actual target accesses are byte reads/writes, not halfword text cells: every scan uses LBU and each placeholder store SB. strncpy copies64bytes into input at sp+D4. Quote/stage/result/name bases are AC/84/5C/48, frame118. The40/40/40/20 capacities remain inferred from adjacent homes, not proven source declarations; inherited FAKE comments are retained. Matched sndisp supplies u8-buffer/byte-index family evidence. Current get_p_name accepts destination u8* and u32 selector; get_g_name accepts destination u8* and s32 selector; target passes two arguments. Strcpy/strcat use two arguments, strncpy three, set_kanji_string five. No caller/header repair is required.

Target behavior: mode low nibble15 performs P then G substitution; other modes only P. Only even byte positions match. Preserve the old P-found value for fallback-copy decision before clearing found for the G scan. P/name append resets index before the final suffix strcat; exhausted P scan resets index before the found join. Current source preserves observable substitution behavior; the recovered raw join does not. Private repaired recovered body compiles260words,315/315, with frame140 rather than118. This is a control/reference baseline, not an improvement or source to promote.

## Concrete control repair

The target G-scan entry reads stage[0], independently of the carried index. The tracked while(stage[i]) condition retains an index-derived pointer induction variable in the generated G loop. `body.c` instead expresses the observed control directly:

- if stage[0] is nonzero, enter a do/while;
- compute the body cursor from stage+index;
- test stage[index] only at the loop latch.

All incoming paths have index0, so this preserves behavior. It restores recomputed indexed cursor structure for the G loop and gives264words, aligned123/printed123. Frame118 and all five buffer bases remain correct. Input bounds, byte accesses, found sequencing, call arguments and quote/render cases are unchanged.

`join-reset.c` also resets index unconditionally after the first scan, before testing found, following the target's exhausted-scan reset placement more closely. It gives264words, aligned125/printed127. Keep this alternate; do not discard target timing solely for a small score change. Reset is a nonescaping scalar assignment, and both sources are semantically equivalent.

Bounded follow-ups in the repaired entry context:

| Source | Built words | Aligned | Printed |
| --- | ---: | ---: | ---: |
| Tracked baseline |265|150|150|
| Guard repaired, body.c |264|123|123|
| Guard plus word parity |261|186|186|
| Guard plus join reset |264|125|127|
| Guard, word parity, join reset |261|186|188|

Word parity removes the three old-index copies, but keeps parity promoted in v1 and cursor in a2 rather than recovering target cursor v1/transient parity. This control does not establish a match. No declaration permutations, broad shape/register grid, shared-header mutation, extra workers or Astra consultation were used.

## Exact remaining blocker

Private normal full-TU source/object/target/built/diff are in build/T-25027/stage-zero-guard/. Trace is build/T-25027/trace/sndi/{draft.c,draft.B,draft.o,draft.o.trace,draft.log}. The trace object and normal object resolve identically:264words, zero aligned/printed differences. Regdiff's263/262 headline omits the trailing NOP; use263target/264candidate full-span counts above.

First raw mismatch is scan-entry branch displacement at instruction20, reflecting the subsequent code-size mismatch. First register mismatch at instruction21: converted input base a0 versus target a1. Fresh trace reproduces the known base7.333 versus character-constant6.667 priority ordering. Parity priority20 takesv1; cursor priority11 takesa2. Thus the corrected second entry makes concrete progress but does not solve the previously frozen cursor/parity conflict. Target cursors also spill into30/34 across substitution calls; current shared cursor home is44. First P append lacks the target's explicit v0-zero before suffix strcat, while the candidate has an extra old-index copy in each scan.

No further source-level typing/semantic hole was found that justifies reopening the old induction/register sweeps. This checkpoint is a measured control repair plus precise unresolved blocker, ready for root review; it is not a default-C proof. SDK string return declarations have not been changed or guessed.
