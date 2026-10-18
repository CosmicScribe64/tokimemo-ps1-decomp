# T-24029 seasonal scene sibling checkpoint

No new C match. Production sources are restored to their original INCLUDE_ASM forms. No shared headers were changed. These source-only bodies are recovered drafts, not verified reconstructions.

## Foundation and measurements

Read T-15100, T-17500 and T-17600 first. The old unaligned drafts reported 701 differing rows for GYOZI func_801368B8 and 995 for SHOUGATU func_801350F8. Those older contexts/metrics differ; do not treat the following costs as directly comparable performance improvements.

Current full-file viewsweep aligned edit costs: GYOZI 586 (as-is, three-view budget); SHOUGATU 662 after typed/control-flow repair, 656 after a meaningful switch-value local initialized before string copies. Both are broad mismatches. One compiler was used serially.

Changes: existing DisplayFixActor44 supplies real 32-bit position/velocity accesses where the old draft used scalar halfword declarations or byte-pointer casts; independent velocity thresholds replace decompiler var_at flags; the narrow postincrement counter replaces a temporary holding its next value; existing DisplayActor44 supplies actor fields; the SHOUGATU string table uses its real s[7][5] member. Both use s32 fall-through returns. Target code explicitly leaves zero at exits; explicit return 0 and implicit return views were checked, with no route to matching. SHOUGATU signed versus unsigned timer access gave the same cost. No random register or statement-order search was run.

## Blockers / next useful hypotheses

The local string initialization prologue differs in copy scheduling and temporary assignment. SHOUGATU originally copies buffers at stack +0x84, +0x6C, +0x5C and a 35-byte table at +0x38; the plain initialized char arrays do not reproduce all copy scheduling. The initialized step local improves only six edits. The broad actor-field base and selector live-range differences remain; the two siblings are not directly portable verified C foundations.

GYOZI scratch-only declarations are kept at the top of its archived body. No production header expansion is warranted for a broad draft. Main-range legacy helpers belong in main_api.h if a later exact recovery needs them; preserve unknown names. D_8012E674 and D_8012E680 are observed word text-state accesses, but declaration ownership still needs confirmation before production use. The anomalous case-six boolean OR 8 is present in the target and retained rather than silently corrected.

Reproduce by replacing only the corresponding target INCLUDE_ASM with body.c (omit its leading comment if desired) in a scratch translation-unit copy, then run tools/viewsweep.py UNIT:FUNCTION --src SCRATCH --budget 1 in Docker. Do not install these as unguarded production C.
