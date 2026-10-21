---
id: T-25013
title: "NEXT STEP: compiler-level levers (uopt/ugen/as1 emulation rules) to unblock the last ~119 functions"
status: In Progress
priority: high
type: tooling
assignee: octa
created: 2026-10-21
updated: 2026-10-21
depends_on:
  - T-25012
links: [[[status]], [[plan-remaining]], [[build-step-gaps]], [[as1-scheduler-rule]], [[matching-notes]], [[tickets/T-25012-class-c-d-reconstruction]], [[tickets/T-25011-class-a-closers-as1-levers]], [[tickets/T-24929-uopt-zcopy-scope]]]
---

## Why this is the next step

Source-level work is exhausted. After the 2026-10-19/21 sessions (T-24142, T-25011, T-25012) every guarded near miss was swept (autotweak 280-1,940 variants each, permuter 300-600 s, order_search, hand variants) and the unguarded functions that were reconstructed from the asm stop 70-100 rows away for the same reasons. `tools/twin_sweep.py` finds no matched twin for any of the 119 unmatched functions. Further hand work with the current build pipeline will keep producing guards, not matches. What is left needs the build pipeline to model the original compiler better, one uniform rule at a time (the way T-24929 did for ETC `func_8013CCE4`).

## Rules (do not skip)

- A build-step change is allowed only as ONE uniform rule that (a) has evidence it models the original compiler, (b) changes no already-matched function (compare every object before and after; `drafts/T-24929/objcmp.py` does this), and (c) passes 27/27 SHA1. No per-function or per-file options (CODING_STANDARDS section 7a, [[plan-remaining]] ground rules).
- Where the rules live: `tools/cvt_pass.py`, `tools/ugen_pass.py`, the frame pass and `tools/cc.py`. Existing precedents: T-24929 (switch compares), T-24103 (`_constant_first` before `rpar`), the cvt pass NO_WIDEN helpers.
- Use `tools/regdiff.py` with `TOKIMEMO_SCRATCH=$PWD/build/scratch/<ticket>` and the traced uopt copied to `$TOKIMEMO_SCRATCH/r9-uopt/variants/trace` (source: `/private/tmp/t24047-uopt/variants/trace`; if that copy is gone, rebuild it from the instrumented uopt of T-9800, see [[ending-allocator-trace]]). Check `regdiff` before trusting `score.py`'s `struct` count, which masks registers.

## Levers identified (one candidate target each, evidence in the linked notes)

1. **Keep a redundant store of a promoted global.** main `func_8006A044`: the original keeps F64[1] live in `$v0` after every OR (`or t5,v0,zero; ori; andi v0`) and re-stores it; ours drops the redundant stores (12 instructions missing, about 87 rows). Also helps ETC `func_8013F054` (8 instructions missing). Notes: `drafts/T-25012/main/func_8006A044/`, `drafts/T-25012/ETC/func_8013F054/`.
2. **8-byte shift of the locals area.** main `func_80062948`: the original's locals sit 8 bytes lower (0x70/0x80/0xA0/0xC0 in a 0xD0 frame; ours 0x78/0x88/0xA8/0xC8) and an unused local only grows the frame. Find what the original compiler reserves there (outgoing-argument area or a spilled temporary) and model it. Notes: `drafts/T-25012/main/func_80062948/`. Related: NAME_ENT `func_80138C1C`/`func_801398EC` frame work (T-24132), TAIIKU `func_8013E1FC` (T-24142 `ox`/`oy` hole, best draft was in the lost scratch; the findings are in [[log]] 2026-10-19).
3. **No CSE of relational booleans.** ENDING `func_801387B0` (about 80 rows): the original recomputes `xor; sltiu` for each of four `0xC - (flag == 1)` compares while sharing the `lbu`/`andi`; uopt CSEs the boolean from the second compare on.
4. **as1 block-weight / delay-slot picks.** KANGEI `func_8013852C`, KANGEI `func_80134724` (4 rows), VALEN-style dispatch `sll` slot picks, GEKO `func_80136C20` and `func_801367EC` (selector `lui` should fill the branch slot, block weight 7 but the scan stops at +6), TACO `func_801511C0`/`func_8013B460`, GYOZI `func_80140E0C`/`func_8013907C`. See [[as1-scheduler-rule]]: the visit-order / weight pass is the lever to trace.
5. **Register colouring order.** Unconstrained uopt ranges are coloured in first-mention order, not priority (regdiff finding, T-25012). GYOZI `func_801412A4` (ugen temporaries t7 vs t5), BUNKA_SD `func_80134D94`/`func_80139290`, GEKO `func_80144198`-style param-in-register (`sel` kept in `$a2` instead of spilled), `D_800E7384`-style direct-symbol reads (`keepproof.py` says NOT PROVEN for BUNKA_SD `func_80134D94` and MASTER `func_80132444`).

## Tooling that saves tokens (use it, do not re-do it by hand)

`tools/lever_lab.py` (T-25013, tested in `tools/test_lever_lab.py`) measures a lever in one command and prints a short report:

```
tools/docker.sh python3 tools/lever_lab.py run base -j 4                       # once per tree state
tools/docker.sh python3 tools/lever_lab.py run myrule --env CVT_MYRULE=1 -j 4  # a variant, compared to base
tools/docker.sh python3 tools/lever_lab.py show myrule                         # reprint the report
```

It rebuilds every object, checks `0 FAILED` and 27 `.ok`, compares every matched function with the base snapshot (any change prints `RULE IS NOT UNIFORM, reject`), and re-scores all guarded functions (`ZERO`/`better`/`WORSE` against base). A no-op variant reports `0 of 7506` changed and `guarded changes vs base: 0`. Gate the rule behind an env knob while experimenting (read the knob in the pass), make it unconditional only once the report is clean, and re-run `base` then. Read only the report; do not read build logs or object dumps unless a function you target did not move.

## Method

1. Pick one lever. Take its smallest target, trace uopt/ugen/as1 on the original-shaped source and find the first divergence from the target asm.
2. Write the rule as a pass or a pass extension with a unit test in `tools/` (see `tools/test_cvt_pass.py`).
3. Rebuild everything; run the object-comparison tool over all units; run the full gate (headers, `migrate_globals.py --check`, `ninja`, 27/27).
4. Only then promote the targets the rule unblocks (each still needs funcdiff MATCH), one commit per rule.
5. Record the evidence in [[matching-notes]] (`## T-NNNN: ...`) and update [[build-step-gaps]], [[status]] and the log.

## Notes for the agent who picks this up

Cheap models stalled on this class (three Sonnet 5.5 runs, zero matches); use the strongest model available. Do not start with the source-level sweep tools again: the work they can do is done.

## Findings

### 2026-10-21 lever 1 (redundant store of promoted global, main func_8006A044): NOT closed, partial analysis
- Mini reproduction (struct with u8 `y`, `m`, `f[2]`, same if/else chain, `|=` arms, switch) compiled with tools/cc.py: our F64[1]-style byte is already promoted and stored at the same places as the original (`sb` before the next test). The only difference left is the arm shape: original `or t5,v0,zero; ori t6,t5,k; andi v0,t6,0xff; sb v0`, ours `ori t5,v1,k; sb t5` (no copy, no write-back `andi` into the promoted register). So the stores are NOT dropped; what is missing is the register copy of the variable and the narrowing write-back at the end of a path (copy propagation / dead-assignment removal of the promoted var at an exit).
- Tried: uopt `OPTN zcopy 0` (copy propagation off) for ALL procedures via an env knob in `cvt_pass.copy_propagation_options`. `lever_lab run zall`: not uniform (22 guarded changes, 3 better, 19 WORSE, e.g. main func_8006D7EC 2 -> 1630, BUNKA_SD func_80139290 14 -> 595); on the mini case it did not produce the `or t5,v0,zero` copy either. Verdict: zcopy is not the lever for this pattern; dead end. Knob reverted.
- Next idea (not done): the copy is a ucode `LOD R; STR R` pair that uopt normally collapses; trace which uopt phase removes it (instrumented uopt, [[ending-allocator-trace]]).
- Levers 2-5 not started in this session (usage budget); no rule adopted, tree unchanged apart from this note.

## Lever 1 session notes (Octa, remote, 2026-10-21)

Setup: native install of the pinned toolchain on the cloud VM (Docker cannot
create containers there: network namespace creation is blocked), packets in
scratch outside the repo, fidelity proven by reproducing the T-25012 draft
measurement for main `func_8006A044` (97 diff rows on the score.py metric,
same first divergence: month colour swap). Full gate is CI on the PR.

Findings on lever 1 (main `func_8006A044`, draft `body-global.c`):

1. The addressing form is not the lever. cfe reaches `unk_F64[1]` (and
   `unk_F64[0]`) indirectly (`LDA; LDC 0xf64; ADD; LDC idx; IXA; ILOD/ISTR`).
   A `cvt_pass` pre-rewrite (env-gated `TOKIMEMO_LEVER1`) that normalizes
   constant-address indirect accesses to narrow S locations into direct
   `LOD`/`STR` fires on all 19 sites and changes uopt's output, but the
   final object scores identically (97). uopt already recognizes the
   constant address (baseline output promotes via `RLOD`/`RSTR` of the S
   location), so the two forms converge. Do not pursue addressing again.
2. uopt's expression graph already models the byte as one evolving version
   chain (traced listing: expression 853, `uior` versions feeding `ustr`);
   there is no per-arm temporary in uopt for a rewrite to preserve. The
   delta is below uopt: ugen/allocator coalescing.
3. Our compiled arm is `ori t,v1,k; andi v1,t,0xff; sb v1` against the
   original's `or t5,v0,zero; ori t6,t5,k; andi v0,t6,0xff; sb v0`: the
   only per-arm difference is the original's extra copy of the home
   register before the `ori` (plus the head colour swap, lever 5). The
   copy is an allocator coalescing artifact of the original build, not a
   uopt phase collapsing a `LOD`/`STR` pair, so the trace question the
   ticket queued is answered in the negative: no uopt phase removes the
   pair; it is never created. Lever 1 as stated does not exist as a uopt
   rule; the function needs lever 5 (colouring/coalescing) instead.

Next: lever 3 (ENDING `func_801387B0`, no CSE of relational booleans) has
the cleanest remaining single-mechanism hypothesis.

### Lever 3 session notes (Octa, remote, 2026-10-21)

Target ENDING func_801387B0 (draft drafts/T-25002/ENDING/func_801387B0.c).
Baseline reproduced in the packet environment: 99 diff rows on the local
funcdiff metric (T-25002 measured 80 on the full tree; same divergence,
the boolean chain in the final else-if block).

Mechanism, from the ucode in/out dumps of the draft: cfe emits four
(and, equ) pairs, one per compare. uopt value-numbers the boolean
(flag & 1) == 1 and, in its output stream, computes it at the first two
compares, stores it to a reused local slot at the second compare, and
reloads it for the third and fourth compares. The original computes
xor/sltiu per compare from one shared andi result, so its uopt graph
shared the AND but not the EQU. Nothing downstream can express that
split: a post-pass expansion of the stored temp re-loads or re-computes
from the stack slot uopt reused, which cannot reproduce the original's
register-resident AND result held across the chain.

Option sweep: an env-gated knob in the scratch tree's cvt_pass inserted
OPTN N:V before every ENT. Every N in 0..530 at V=0, and every N in
400..464 at V=1 and V=2. No setting changed the target's output except
N=404, 405 (zcopy off, expected), 406, all worse (226/111/124 rows),
and uopt failures at N=1, 4, 5 (V=0) and N=403 (V=1, 2). The 7.1 uopt
stage swapped into the 5.3 pipeline produces the identical CSE (score
unchanged, 2 xor rows). Conclusion: lever 3 as stated does not exist in
uopt's option space; func_801387B0's boolean CSE has no uniform
compiler-level switch. Do not re-sweep.

### Lever 2 session notes (Octa, remote, 2026-10-21)

Target main func_80062948 (src/main/80061710.c). The matrices draft sits
at 40/184 words with original aggregate homes but frame 0xD8 against the
original 0xD0, every local home shifted +8. The lever as stated assumed
the original reserves 8 bytes our compiler does not, or that an unused
local grows our frame where the original's compiler elided it.

Micro-experiment in the packet environment: a function with a used
s32 buf[2] builds frame -48. Adding a declared-but-unused s32 pad[2]
changes nothing; adding an unused 32-byte struct plus an unused scalar
also changes nothing (frame still -48, ra still at 20(sp)). The IDO 5.3
pipeline already elides frame space for unused locals, so the +8 in the
matrices draft cannot come from an unused declaration. It must come
from something the draft actually materializes that the original does
not: an outgoing-argument slot for a call the original passes in
registers, or a spilled temporary the original keeps in a register.
Both are the allocation/lifetime behaviour covered by levers 1 and 5,
not a frame-pass rule. The frame pass (tools/frame_pass.py) has nothing
to fix for this function. Lever 2 does not exist as an independent
compiler lever; do not re-pursue it as one.
