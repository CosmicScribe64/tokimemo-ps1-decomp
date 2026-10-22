# T-25033 SHOUGATU E50C hands-on matching checkpoint

Exclusive source src/ovl/SHOUGATU/8013E330.c, func_8013E50C,472 bytes.
Best current guard:118 target/117 candidate words, five printed +/- lines,
aligned cost5. Missing entry move a2,zero at0x8013E57C, then target sb a2
at0x8013E5BC and move a0,a2 at0x8013E5C4 versus hardware zero. Every other
resolved instruction key agrees after alignment. Frame58/table homes40/30
exact. No C match or new coverage. Preserve this guard through further work.

Source SHA256 eef07f82f90f49390dfc3c70bd83995217d81235e8cdd7147b9e043e20d47a04.
Field correction unk_10F8, canonical no-arg check_para_limit and caller FAKEs
are intentional. The overridden integer func_800847B8 result is ignored;
it retains the loop index in v1. The u8 status result narrowing spends dead
expression temporaries; byte-view first argument fixes later reload base.
Shared headers/tools unchanged. Default keeps assembly fallback.

## Measured path, same whole TU

| Source/control | Candidate words / target118 | Printed lines / aligned cost |
| --- | ---: | ---: |
| T-24523 corrected archive baseline |124|152/152|
| Archive plus canonical check_para_limit() |117|99/99|
| Corrected newer in-tree declaration layout, old helper args |124|150/150|
| Corrected in-tree layout plus canonical helper |117|93/93|
| Integer-return view of func_800847B8 (related GYOZI precedent) |117|87/87|
| Plus u8 narrowing of status result |117|9/9|
| Plus byte-view first status argument (BEST) |117|5/5|
| First status call(girl,sp40.v,girl), explicit ignored arguments |118|42/10|

T-24523 archive omitted newer T-24709 declaration placement. Current i/girl
above both tables restores40/30 rather than48/38. Preserve it. Last test
emits a2-zero only at the first call, not entry, still uses sb zero and
move a0,zero, and regresses the reload base. Do not repeat that three-arg
form, zero-local widths/promotion/assignment grids or obsolete table layouts.
37 tracked prior narrow E50C bodies had no canonical zero-arg final helper.

## Faithful current boundary and old ledger

Private build/t25033/trace has full TU draft.c, input draft.B, optimized
draft.B.after, native draft.o.trace, allocator draft.log, decoded function
excerpts and measurement.json. Stock capture in build/t25033/stock retains
G/T/initial-as1 log. Four relevant stock/trace sections match byte-for-byte
(.text/.rodata/.rel.text/.rel.rodata), recorded trace/fidelity.json.
Input girl home M25/-8 has zero entry store; optimized output removes it
and starts a new literal-zero merge value. Old expression-table entry alone
does not establish live allocation (see analogous T-25025 GYOZI analysis).

Read drafts/T-24907/NOTES.md and wiki/tickets/T-25025-gyozi-zero-reaching-definition.md:
zero locals, global promotion, byte-view *=0, assignment-expression and
two-argument call experiments previously failed in that relative. This does
not prove every distinct source mechanism impossible. T-25026 counter-fold
consultation is a different site; do not repeat its simple dead/live backedges.

Existing raw source T-17500 has spurious status/helper args. Callees are
matched get_g_zyotai_s(s32) at80051A68 and check_para_limit(void) at80084D3C;
actual incoming unused registers are not a semantic parameter contract.
Target/source files and callers are available in the isolated worktree.

## Current checks and continuation

Actual in-tree full-TU C funcdiff reproduces5/5,117/118. Default SHOUGATU
checksum passes (assembly fallback); headers/global/prototype guards pass,
0 new disagreements/15 known. No full27 worker build. No raw output committed.

Human/root authorized one GPT-6 Astra implementation worker as sole writer
of this file/tree during the next attempt; Sol coordinator idles. Initial
30-minute sustained attempt, continue on actual measured progress. Work
target CFG/lifetimes/ABI and novel plausible C/control/representation forms;
mark FAKEs. No other agents/functions, inline asm, patched instructions,
per-function flags, global compiler changes or extra observable stores/calls.
Preserve best guard even if controls regress. A match requires literal full-TU
and default-C/private linked proof, with ASM fallback excluded. Root integrates.
