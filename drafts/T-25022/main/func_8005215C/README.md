# T-25022 reload-boundary checkpoint

Base883760224; exclusive source src/main/8004F870.c. Production source and shared headers are unchanged. All measurements compile the complete translation unit with the supplied body replacing its INCLUDE_ASM, through the normal Docker compiler and passes. No assembly-fallback MATCH is counted.

`body.c` is the accepted semantic reconstruction checkpoint: mode is re-read for the initial zero test after k_disp_inc, and again before the second final input test after possible func_80044750/func_80045414 or k_disp_goto_line_end calls. `partial-reload.c` is lower-score diagnostic source only: its final mode cache still crosses these calls. `path-local-reloads.c` reloads specifically after func_80045414 and the first goto-line-end call rather than unconditionally at the join; normal optimized output equals body.c. Preserve both representations for control-flow analysis.

All candidates preserve full-width F61+2 and F61+3 comparisons. The correct64-byte frame and homes result+0x34, first text buffer+0x30 and selection buffer+0x28 remain in body.c. No empty tests, padding or per-function tool changes introduced.

## Whole-TU results

Target315 words. Aligned metric uses insert/delete1, substitution2; printed counts are separate funcdiff.diff_lines lengths. These are not positional mismatch counts.

| Candidate | Built words | Aligned | Printed |
| --- | ---: | ---: | ---: |
| T-25021 baseline |313|138|138|
| Initial reload only |314|127|127|
| Both reload boundaries, body.c |310|179|179|
| Path-local later reloads |310|179|179|
| Assign first reload back to shared scalar |313|146|146|
| Both repairs plus assign first reload |309|198|198|
| Direct initial conditions, no cached assignment |313|140|140|
| Separate mode local u8/u32 |309|220|220|
| Separate mode local s32 |310|201|201|
| Initial mode reuses phase scalar |314|127|127|
| Initial mode reuses upper-bound scalar |313|140|140|
| Final mode reuses phase scalar |310|179|179|
| Explicit raw-phase temporary |314|295|305|
| All mode reads via volatile byte view, diagnostic |316|257|281|
| Mode reads via GameState pointer |318|241|243|
| Nonvolatile byte address view |310|187|191|
| Final F62 stores via byte address view |310|179|179|
| Private unsigned mode-field declaration control |309|198|198|
| Signed mode reads, incorrect load-width diagnostic |315|166|168|

Direct initial conditions combined with unconditional later reload:309/192/192. The unsigned declaration was changed only in private preprocessed input, never in shared headers. Signed diagnostic emits lb instead of observed lbu and is not eligible despite matching instruction count.

## Read boundaries and callee effects

The target re-reads GameState+0x3C after the initial conditional k_disp_inc before testing mode zero. In the final handler, an initial mode read precedes the first F88 test, a reload follows func_80045414(9,0,0) before clearing F62, and another follows k_disp_goto_line_end. No-call paths retain the original mode value at the join before the second F88 test.

Checked matched definitions: k_disp_inc writes text_list_count.pos; k_disp_goto_line_end also writes that position. Neither directly writes mode. func_80045414 case9 queues command9 via func_80045318, which updates the CD ring slot and tail; it does not directly write mode on that path. func_80044750 calls func_80079B10, which calls func_8007B5EC. Transitive audio effects were not fully audited. Consequently a direct mode mutation by these callees is not established. The target's distinct loads and opaque-call boundary remain required reconstruction evidence; caching cannot be accepted solely for score.

## Remaining divergence and bounded consultation

Private raw artifacts: build/T-25022/{baseline,reload-initial,reload-tail,local-tail,trace}. Normal and traced compiler objects for body.c resolve identically:310 words, zero aligned/printed differences. Trace tool itself excludes a trailing NOP in its headline309/314 count; use the table above for consistent full-span metrics. Compiler is normal IDO5.3 -O2 -mips1 -G0 -non_shared -Wo,-nokpicopt -cckr with repository passes, no flag change.

First raw difference is instruction4: initial phase-byte address/load uses v1 instead of a0; subsequent F61 load uses a2 instead of v1. First structural difference is the initial mode condition at target instruction105: candidate v1 is copied into v0 between the two comparisons. With both reload repairs, later mode reads sink below the F88/result guards, and the initial final-mode read no longer stays live across the first guard. This removes the target's eager reload placement and several load-delay NOPs. Do not begin a generic register sweep before resolving that control-flow distinction.

A bounded Astra consultation should ask at most three questions:

1. Which source-level distinction makes the final mode value live at the first guard and join while still reloading only along the two call paths? Compare body.c and path-local-reloads.c with faithful private pre/post-pass output; they currently optimize identically.
2. Can the initial mode conversion web's v1-to-v0 copy be removed through a justified original type/read representation, retaining unsigned byte loads? Unsigned-field, byte-address and separate-role controls above bound the obvious tests.
3. Does the initial phase/count coloring follow an actual source-lifetime difference once the mode boundaries are recovered? Trace has phase-expression priority1.5 and count-expression priority2; do not alter priorities with dummy uses.

No match or coverage increase claimed. Root owns review, integration and full-executable gates.
