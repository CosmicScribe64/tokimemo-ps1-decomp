---
id: T-25034
title: MAIN src/main/80046500.c full-width tile extent recovery
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25032-main-candidate-audit]]"]
---

## Goal

Match func_80046AC8 using proven full-width tile extent tests, narrowing only for TILE stores. Exclusive file src/main/80046500.c. Preserve newest baseline and corrected semantic source regardless score. Inspect exact first divergence after whole-TU evidence; bounded45minute investigation, no broad grids/shared-header/build-rule/Astra changes.

## Acceptance criteria

- [ ] Four extent arithmetic/test paths preserve target signedness and full-word branch values.
- [ ] Whole-TU baseline and corrected candidate measured; default C literal proof required for promotion.
- [ ] Root review and integration gates before Done.

## Notes

T-25032 pin62948769f proves retained s16 extent defect in newest T-24501 source. One worker, isolated tree, root owns integration. No root checkout edit or push.

## Active reconstruction

Whole-TU baseline reproduced target674/built680words, aligned738/printed796. Changing only extent s16->s32 yields671words,613/649; all four tile branches now use full signed words without halfword narrowing. Cases0/1 compute offset minus signed radius plus120/160; cases2/3 signed negated offset minus radius plus120/160, store low16 only. Corrected baseline retained privately regardless score.

Continuing sustained matching at root direction, not stopping after the repair. Target-guided channel/callback lifetime probes expose mask elimination versus signed conversions; primitive-pointer role reuse reduces frameB8->B0. No match or production/shared-header change. Detailed controls remain in build/T-25034 pending stable checkpoint.

## Astra hands-on continuation

Human explicitly requests Astra and sustained resolution. Prior time/probe stopping limits are superseded: continue this exclusively owned function until literal semantic matching implementation and checks, or a concrete external blocker actually prevents further work. Ordinary matching difficulty and an unresolved packet are not resolution. One Astra worker now owns source experiments; Sol coordinator stops writing source. No extra workers, shared-header/build-rule changes or root integration.

Latest corrected private source671words, aligned367/printed389 (target674), frameB0 versusA8. Shared primitive, color/UV and reverse-radius/extent roles plus direct low-channel expression improve613->405->367. Full-width extent repair remains. Preserved current source at drafts/T-25034/main/func_80046AC8/body.c; detailed handoff/controls privately in build/T-25034/handoff.md. Matching continues rather than closing on this checkpoint.

### Latest frame correction

Astra verified saved direct-low-channel built output: frame is already0xA8, not the earlier0xB0 recorded in the handoff. Incoming argument homesB8/BC/C0/C4 match target. Shared extent role plus removal of the separate low-channel local closes frame growth. Local spill homes and color-expression ranges remain; prior logs are preserved rather than rewritten. Full-word extent semantics remain intact.

## User-directed wrap-up

User requested wrap-up, commit and push on 2026-10-22, superseding sustained matching. Astra stopped all experiments. Conservative candidate674target/671built padded words,361aligned/381printed; forward-join alternative674/670,360/380. Both retain full-word extents and target0xA8 frame. Sources, findings and Docker replay are archived under drafts/T-25034. Ticket remains In Progress: no match, promotion or coverage credit. Root owns main integration.

## Resumed after newer human instruction

Root relayed a newer direct human instruction after wrap-up: "Keep them going wihtout stopping until problems are figured out and matches made". This supersedes the wrap-up above. Reused the existing Astra worker on the same func_80046AC8 from checkpoint d8e482c4a (integrated by root as ff8a1c912). Sole source writer, no duplicate coordinator experiments. Continue through failed controls and checkpoints until literal semantic matching plus checks or a concrete external blocker. Prior source and negative evidence remain preserved; root owns main integration.

### Resumed conversion finding

Astra's mask-before-shift control moves the extra copy with the expression tree: fp receives the inner mask, the outer shift uses a temporary and copies back; shift-then-mask had the reverse order. Explicit unsigned-byte consumers leave the best object unchanged. Evidence points to expression/result placement rather than extraction arithmetic. Next mechanism is assignment propagation through primitive color fields, preserving required stores and full-word extents. No new best or match yet.

### Mask-node allocation is insufficient

Faithful unsigned-mask trace: bit53=SHR(arg6,16), bit54=CVT(bit53), bit55=AND(bit54,255). Bit55 itself is assigned fp (priority7.625, uses5/defs2), yet output remains SRA fp; ANDI temporary,fp,255; MOVE fp,temporary. Earlier signed-mask trace assigned outer CVT bit63 to fp. This shifts the next investigation to normal backend expression/result placement: allocating the mask node is not enough. Natural primitive-field propagation, nested required assignments, macro-style grouping and unsigned remainder extraction did not fix it. A distinct reverse-radius local left UV at v1 and enlarged the frame. Astra continues output-ucode/normal lowering inspection; no production changes or match.

### Raw lowering contains the copy

Normal ugen input for unsigned-mask is SHR J; CVT L; AND L 255; STR R30. A signed-byte store view yields SHR J; AND J 255; STR R30 with no CVT. Both raw ugen binasm streams already contain SRA fp,source,16; AND temporary,fp,255; MOVE fp,temporary, so as1 does not introduce the copy. Capture fidelity was checked: resolved function identical, entire object differs only in .mdebug path metadata, with .text and all other sections identical. Private evidence: build/T-25034/astra-ucode-capture and astra-signed-view-capture. Next investigation: destination forwarding into the shift child. No compiler defect or pass is established.

### Destination forwarding confirmed causally

Reference normal backend immediate-op path L432A18 forwards requested destination fp into the left child. L432A58 sees the child/result registers coincide; AND is absent from the allowed in-place opcode set, so L432A9C allocates a temporary and later copies back. At f_eval entry L42D55C, child reference count>=2 cancels the requested destination (0x48).

A real C control confirms this: replacing only the fourth required red store with raw (arg6>>16) makes the normal compiler emit SRA a0 followed by ANDI fp directly, eliminating the copy. astra-shared-red-shift is668candidate words/850aligned/862printed, worse overall: raw shift becomes a0 and the fourth store uses a0, whereas target uses a temporary shift result and fp for all stores. Causal success, not promotion. Worker continues distinct masked consumers and constant-left AND spelling to avoid forwarding without a live raw-color register. No backend changes.

### Shared required high-word consumer rejected

The common high-word source feeding the required TILE flag and red mask did not preserve target-local extraction: uopt hoisted it before the quad loop and added a stack home. Candidate672words/810aligned/816printed is negative; best source unchanged. Worker returns to required UV/control lifetimes while retaining the proven destination-forwarding mechanism for color.

### UV priority change measured

Separate case breaks raise scaled-row priority8.2->9.111, above constant127(8.889) and quadrant(8.875), rotating s5/s6/s7. An explicit if-chain also rotates them. A standalone shared-u0 block with default bypass canonicalizes back to the360/380 best. UV byte-result cast leaves end_v in v1. Worker next checks single-local-read masking and a required field-store chain; no promotion.

### Cast-before-mask closes color result placement

With s32 red, `red = (s8)(arg6 >> 16) & 255` and equivalent TILE red/green extraction emits SRA into a temporary then ANDI directly into fp/s7, without the unwanted sign-extension pair or copy. Immutable astra-mask-signed-byte candidate measures674target/667built,367aligned/415printed; not overall best. Applying `(s8)arg6 &255` to blue adds an unwanted stack LB, so worker removes only that narrowing and traces the changed expression identity. This ordinary-C mechanism closes the critical red placement without synthetic sharing or backend changes.

### New best signed-mask source

Removing only blue narrowing from astra-mask-signed-byte yields immutable astra-mask-signed-shifts:674target/666built padded words,212aligned/212printed (previous best360/380). Red/green use direct ANDI into fp/s7; blue uses required word load and ANDI instead of LB. Full-s32 extent tests and0xA8 frame retained. Archived as drafts/T-25034/main/func_80046AC8/signed-mask.c with replay support. Trace of preceding candidate identifies fp bit64 as cvt(and(cvtl(shr(arg6,16)),255)),priority7.625,5uses2defs; inner signed CVTL is causal, without synthetic sharing. Matching continues UV/register/CFG reconstruction; not promoted.

### Signed-mask capture refinement

Normal ugen still emits the shift/sign-extension/mask chain with intermediate copies. The existing pipeline collapses that chain into the final temporary-shift/direct-ANDI form. Thus the signed CVTL source expression is successful through the full pipeline, but does not prove raw ugen destination forwarding changed. astra-signed-mask-capture/fidelity.json confirms resolved capture output exactly equals the666word,212/212 canonical candidate (capture comparison cost0). No compiler/tool changes.

### RGB grouping improves to198

Immutable astra-rgb-comma-group uses standard per-vertex RGB order grouped as one comma expression per vertex. Whole TU674/666words,198aligned/198printed. Colored-quad channel extraction, temporary registers red t0/green t3, and all twelve color stores agree with target. The unrelated a1=0 tpage argument still occupies the branch delay slot rather than target red shift. Updated tracked signed-mask.c; prior212 source remains in b25963c45. Matching continues.

### UV store order improves to186

Immutable astra-uv-after-xy moves each v3 store after its y3 coordinate assignment in all four cases. Existing as1 scheduling puts the UV store in the target y3 load delay slot, restoring four NOP/UV positions. Whole-TU674target/670built words,186aligned/186printed. Color block stays exact except hoisted a1=0. Updated signed-mask.c from immutable worker source; worker continues.

### TILE case0 scheduling improves to182

Immutable astra-tile-xy-before-width gives674target/670built,182aligned/182printed. Moving TILE case0 width assignment after y0 lets the independent constant-width store fill the target load delay and restores a1=primitive scheduling. Full-s32 extent semantics retained. Worker next tests corresponding case1 width/height grouping; private accumulated ledger is build/T-25034/astra-resume-progress.md.

### UV exit branches improve to159

Case1 width-before-height ordering first improved to178/178 at674/670words. Latest immutable astra-uv-join-after-u0 is674target/673built,159aligned/159printed: each required u0=0 store precedes its explicit goto, shared label follows case3 u0. This reproduces target UV exit branches/delay stores while preserving saved-register ranks, unlike natural breaks. One overall missing instruction is TILE case2 radius MOVE; UV endpoint remains v1 vs a1. Quad-only half-radius narrowing was negative; moving TILE case2 extent after y0 canonicalizes unchanged. Updated signed-mask.c; worker continues, no promotion.

### Missing radius move is downstream

Faithful astra-159-capture resolves exactly equal to canonical673word candidate (capture comparison0). Raw ugen emits TILE case2 MOVE v0,t8 immediately after division at record1209; final pipeline removes it, while case3 copy survives. Thus the missing instruction is downstream, not absent uopt radius allocation. Source chains/order/two-stage division controls canonicalize unchanged. Astra continues y0/extent dependency and grouping investigation.

### Integer return contract restores the radius move

Immutable astra-integer-return changes only return type to s32 with no explicit return. The previously missing TILE case2 MOVE and consequent branch distances are restored:674/674words,144aligned cost; equal-length positional printer902lines is not comparable to prior aligned printer rows. Preserve void159 as conservative baseline and integer-return.c as active branch.

Parent read-only caller audit: no declaration under include/; two known C calls are expression statements in NAME_ENT func8013B5F0 and SHUGAKU func801360B0, currently implicit-int. Target NAME_ENT JAL8013B764 immediately reaches epilogue without reading v0. SHUGAKU JAL80136344 proceeds to argument setup and func800494BC without reading v0. INCLUDE_ASM supplies no return declaration. This is compatible with ignored integer return, not unique proof of original signature. Canonical declaration/ABI checks required before eventual promotion; no shared header edited.

Root independent replay on current main confirms integer-return.c674/674words,144aligned/902positional lines. Root source search confirms two existing calls discard results and no include declaration exists. This preserves the audited research branch without claiming unique original return signature or coverage. Worker continues remaining allocation/scheduling residues.

### Early row scaling changes allocation priority

Explicit early texture-row scaling recovers target t7/t8/t9 entry temporaries, but raises the row range from9uses/priority8.2 to13uses/12.2 and moves it froms7 tos3. Four additional uses are per-case endpoint expressions. Repeating original arg0 extraction canonicalizes back without reducing the use count. Integer144 remains best; private captures and priority traces preserved. Worker continues source-lifetime reconstruction.

### UV hidden range is a base-row copy

Faithful raw ugen corrects regdiff's bit210 constant0x80120000 guess: records639/745/851/956 are COPY a1,s7 of base UV row feeding v0/v1 byte stores. Final as1 removes the copy and substitutes s7. Hidden range priority30 (8uses/4defs) overlaps endpoint bit223 priority40 and reverse radius36.67. Target endpointa1 is consistent with different ordering of these real overlapping webs. Worker now targets required base/end UV consumers, not a hypothetical unused constant.

### Standard UV ordering improves to116; user-requested wrap-up

Standard forward u0,v0,u1,v1,u2,v2,u3,v3 assignments in one comma expression per quad exchange the overlapping range priorities: base-row CVT40/v1, reverse-radius36.67/a0, endpoint ADD30/a1. Prior base-copy priority30 reflected denominator4 versus endpoint3 from later base-byte consumers. Endpoint allocation now agrees with target; final reversed store scheduling and u0 branch delays also agree, and the spurious a1=0 hoist disappears. Immutable astra-uv-standard-macro gives674/674 padded words, aligned116/printed156; archived as drafts/T-25034/main/func_80046AC8/uv-standard.c. No production changes. The user's latest instruction requests wrap-up, commit and push; experiments stopped and remaining mismatch is explicitly preserved rather than marked Done.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
