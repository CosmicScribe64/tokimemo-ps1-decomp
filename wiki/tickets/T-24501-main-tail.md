---
id: T-24501
title: Match MAIN remaining functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[matching-notes]]"]
---

## Goal

Match remaining MAIN functions with plausible C and preserve all 27 executable checksums.

## Acceptance criteria

- [ ] Literal default C proof for every promoted function.
- [ ] Full 27 executable checksums and header/prototype checks pass.
- [ ] Code review against CODING_STANDARDS.md before Done.

## Notes

Isolated worktree `/private/tmp/tokimemo-t24501`, baseline `2c7293275`. First target `func_8006C988` in `src/main/800674B0.c`: 11-row pointer/value ordering tail from T-24301.

## Comments

### 2026-10-20 investigation

- `func_8006C988`: guarded baseline reproduced; targeted shared-value lifetime, index-before-load, and source grouping did not beat the prior `s32 cur` 10-row tail. Production unchanged.
- `func_8006509C` (1,076 bytes): existing draft full-TU resolved edit cost 99 (`funcdiff.diff_score`, insert/delete 1, substitution 2); pointer/local order, typed index, and count hoist variants did not improve. Moving the second loop initializer before its entry test reduced cost to 95 but remains too far to guard.
- `func_800654D0` (1,072 bytes): clean reconstruction from the original assembly replaces the raw m2c unrolled loops with indexed loops over four `s16[16]` menu arrays and actors 80+/64+. Initial cost 216; typed pair indexing avoids CSE of `i * 2` into the source scale; scalar-after-arrays declaration puts arrays at original offsets. Further structural work in progress. Scratch files stay untracked.

### First verified match

`main:func_800654D0`: `funcdiff --resolve --version default` reports `MATCH (linked function bytes equal target (1072 bytes))`. `tools/check_headers.py` reports `headers OK`; `tools/sync_protos.py --check-branch` reports zero new disagreements and `branch check OK` (14 known baseline cases). Plain C replaces its INCLUDE_ASM. Two FAKE comments mark the preserved unused scalar and pre-call index zero that reproduce the original frame and saved index; details in [[matching-notes]]. Full 27-unit build pending before handoff.

Full verification completed: fresh `ninja -j 6` exited zero, with 27 checksum `OK` lines in `.scratch/t24501/full27.log`. Parent reviewed the 43-line source diff against CODING_STANDARDS.md and accepted the typed actor/pair-array views, loops, and both marked FAKE comments with no outstanding source finding. No shared headers or production compiler/tooling changes. Focused match commit is ready for integration while additional MAIN work remains In Progress.

### Second verified match

`main:make_color_bar16`: literal default `funcdiff --resolve --version default` reports `MATCH (linked function bytes equal target (1540 bytes))`. Plain signed-channel interpolation through the existing `Pix2Out` replaces the 1,540-byte asm body; no FAKE or shared declarations. Typed signed channels, direct half-count loop expressions, alpha field order, and declaration order close the raw m2c draft. Details and discarded paths are in [[matching-notes]]. Header check passes; final prototype and 27-unit hash verification pending.

Parent source review accepted the interpolation body with no findings but found the missing shared declaration. The canonical symbol is `make_color_bar16 = 0x8004BBA8` (`config/symbol_addrs_obin.txt`), with no prototype or aliased declaration in `main_api.h`. Added its exact definition prototype there with parent approval; callers are NAME_ENT `80139740.c` and SHUGAKU `80135430.c`. Pre-header explicit 27/27 hashes passed; rebuilding and verifying callers after the shared declaration.

Final pre-header prototype branch check passed with `0 new disagreement(s), 15 known`; no new prototype disagreement was introduced. All 27 image hashes were explicitly recomputed as expected. Shared declaration validation is running after that review fix.

Post-declaration verification completed: fresh affected rebuild `ninja -j 8` exited zero, then `funcdiff --resolve --version default main:make_color_bar16` again proved all 1,540 linked bytes. Explicit SHA1 recomputation passed 27/27 images, including both caller units NAME_ENT and SHUGAKU. Header check passed; prototype branch check reports zero new disagreements (15 known). Parent source review had no body findings, and its only declaration finding is resolved by the canonical prototype.

### Bounded post-match repairs

Archived four readable, non-production drafts under `drafts/T-24501/main/`: CD dispatcher `func_800455C4` (resolved edit cost 439), `palette_load_vram` (clean 419, padded experiment 394), `make_color_bar` (330), and `_sprite_set_light_effect1` (374). Additional typed menu `func_80076000` remains at 214 after 11 bounded view compiles; existing `func_80054E6C` guard remains at 84. Exact hypotheses and remaining structural misses are in [[matching-notes]]. Production source and headers are unchanged during this phase; this diagnostic checkpoint needs no executable rebuild. Continue with smaller untried helpers rather than repeating documented row grids.

### Third match: palette helper

`menu_bar_color` (84 bytes) now passes literal default linked proof. `D_800E3680[16] = {0}` owns exactly 0x40 initialized data bytes through a YAML island at 0xA2E80; previous raw range and next owner `80059710` retain their boundaries. A marked redundant nibble mask restores the last five register differences. Parent approved the missing exact canonical prototype in `main_api.h`; post-header full build, 27 hashes and branch checks are running. Parent code review is pending. Source/config details and boundary proof are in [[matching-notes]].

Post-header verification completed: fresh affected 520-step build exited zero; explicit SHA1 recomputation passed all 27 executable images, including all four call sites' units. Literal default proof again reports all 84 linked bytes equal target. Header check passes; prototype branch check reports zero new disagreements (15 known). Parent reviewed the exact source/config/header diff and linked data boundaries against CODING_STANDARDS.md, with no blocking findings. Removed the cosmetic blank after the FAKE comment; the final source was recompiled during literal proof. Focused match is ready for integration; broader ticket remains In Progress.

Bounded status-display reconstruction `func_8004443C` remains asm. Clean array/RECT scratch cost 109; alternate declaration order 92 but wrong stack homes. Setup-order, scalar-gap and grouped-record hypotheses do not fix broad register/home differences. Clean draft archived with exact requirements in `drafts/T-24501/main/func_8004443C.c`; no production/header change. See [[matching-notes]].

Bounded ring alias follow-up `func_80075C84`: local struct views over keys, position or both leave resolved cost 154 and missing reload unchanged. Integer-base and position-relative diagnostic views are worse (204/191). No promotion or shared edit; details in [[matching-notes]]. Same data-ownership history scan finds `InitMouse` already fixed and `sndisp` still structurally blocked.

### Fourth literal match: menu action dispatcher

Resumed `func_80076000` (1,196 bytes) beyond the aggregate 214 cost. Full-TU regdiff proved 298/298 instructions with 11 order-only regions. Packed `u16` icon bit-fields give 164; explicit signed-byte selector views give 138 and a 132-instruction exact prefix; shift-back flag extraction restores discarded ugen temporaries and closes to zero. Production literal default private link proves all 1,196 bytes. Pre-review ninja, explicit 27/27 hashes, headers and prototype branch checks pass. Parent reviewed the body, requested relocating `MenuIconBits` to `game.h` and comments on flag extraction; both fixes applied, final affected rebuild running. Details in [[matching-notes]].

Final post-review checks pass: affected 486-step build, explicit 27/27 executable SHA1 hashes, repeated literal default 1,196-byte proof, headers, and prototype branch check (zero new, 15 known). Parent re-reviewed moved type/comments and resolved its only finding; no remaining blocking standards findings. Focused menu dispatcher match ready for integration; ticket stays In Progress.

Neighbor `func_800767A4` clean reconstruction now matches all 233 instruction operations, order and registers. Only frame offsets remain (resolved cost40): buffers exact, array/picks four bytes low, frame eight bytes low. A 33-ID diagnostic/aligned union leaves only three picked-value homes four bytes high (cost30); target has a word before the 32-ID array. Archived clean body and bounded frame hypotheses in `drafts/T-24501/main/func_800767A4.c` and [[matching-notes]]. Asked parent for a focused frame-layout consult; no production/header changes.

### Fifth literal match: choice-string setup

Parent's focused frame consult closed `func_800767A4` (932 bytes) using two split unused scalar slots, preserving the 32 candidate IDs and all target stack homes. Promoted exact zero-cost candidate, added T-24501 to all FAKE comments, and this worktree's literal default link proves 932 bytes. Parent reviewed body/padding with no remaining findings. Added approved canonical prototype and replaced inherited suffix string data include with a correctly encoded 20-byte C string definition; final full checks running at two jobs. Details and original candidate SHA256 are in [[matching-notes]].

Final fifth-match verification completed: post-header/string 520-step build at two jobs, explicit 27/27 executable SHA1 hashes, repeated literal default 932-byte private-link proof, headers, and branch prototypes (zero new, 15 known) pass. Parent reviewed the actual source/header/string diff with no remaining standards findings. Focused source/header/wiki pin ready for integration; broader ticket remains In Progress.

### CD dispatcher checkpoint

Resumed structural repair of `func_800455C4`, reducing439 to26 across691 instructions. All remaining differences are nine stack offsets and two four-instruction reorderings. Source-only draft/replay and precise rejected hypotheses preserved; awaiting bounded compiler consult. Production unchanged; ticket remains In Progress. See [[matching-notes]].

### Sixth literal match: radial texture generator

`func_80046754` (884 bytes) clean packed-word loops pass literal default proof. Byte-sized color and halfword upload coordinates recover allocation; separate branch-local row initializers preserve the final repeated zero. Parent reviewed actual C89 source and canonical prototype with no findings; no FAKE/padding. Post-header full27 and guards running at two jobs. See [[matching-notes]].

Final sixth-match checks pass: post-header484-step rebuild at two jobs, explicit27/27 SHA1 hashes, headers/main_api and branch guard (zero new,15 known). Parent code review has no findings; literal default884-byte proof already passed after final source/header edit. Focused pin ready; ticket remains In Progress.

Renderer neighbor `func_80046AC8` checkpoint archived at resolved738 after typed fields/semantic locals and causal texture-row lifetime repair. Broad frame/order and suspect inherited narrowing remain; no production/header edit. Bounded CD consult found no improvement over26. Exact negatives and replay in [[matching-notes]]; reporting checkpoint before another function for cheaper-first routing. Ticket remains In Progress.

Escalated Luna T-24570's MAIN54E6C metric-view result through one actual record-assignment test after clean handoff. Fresh baseline84/87words, probe105/90words; frame remains40 versus target80. No real64B local/argument structure follows from surviving accesses. Exact diagnostic replay preserved; source/header unchanged. No old pointer/padding grids repeated. See [[matching-notes]] and [[tickets/T-24570-main-80054e6c-metric-record-view]].

Active `sndi` semantic recovery: correct first-stage found-flag/reset join, proper buffer objects; best210/259words versus target263, exact observed buffer homes/frame118 after moving existing index scalar before arrays. More faithful branch-specific reset211/260 also retained. Separate scan-local cursors/parities do not help; no production/header edits. Continue source-level induction/join recovery, no Astra or padding grid. See [[matching-notes]].

`sndi` primary210/259 and branch-reset211/260 archived with exact replay. Last approved conditional increment hypothesis fails303/262/frame120; two-base/index forms remain unequal. Freeze before bounded induction consultation, no more source-spelling tests. No production change; preserve corrected flag semantics and inferred-capacity warnings. See [[matching-notes]].

`func_800552DC` loader672B: typed record reconstruction reaches resolved0/168words and private default-C literal672B proof; header/prototype/global guards pass. Parent source/header standards review has no findings. Affected MAIN checksum passes (`ninja -j2 build/SLPM_86.053.ok`); root centrally checks all27 before integration. Record boundaries and FAKE integer bank grouping documented in [[matching-notes]].

Inverse `func_80054FC0` source-only typed reconstruction frozen at 113/200 words (target 199), exact frame and first 45 resolved rows. Byte-loop and unsigned-bound source recovery retained; narrow copy-ring/epilogue packet prepared for parent consultation. No production/header changes. See [[matching-notes]].

Circular GPU grid `func_8004A414` source recovery 390->164, exact 256-word count/frame 98/x-y homes. Natural bounds/distances and typed primitive retained in source-only packet; remaining CSE spill block/register ownership requires bounded consultation. Production/header unchanged. Inverse54FC0 consult negative 113/200; no further probes. See [[matching-notes]].

Astra grid source improves164->132/256 with exact frame/all homes; adopted unchanged and independently replayed. One focused trace has identical ordinary output and identifies pre-colouring priority order; no source experiment justified yet. See packet provenance/rank table.

TILE sibling `func_8004A814` source-only300->114, exact257-word count/frameB8/x and RGB homes; all saved-register roles match. Uniform+4 CSE/depth homes and ugen/scheduling remain; packet/replay archived for bounded consultation. Target dimension quirk, six abs calls and signed/clamped colors preserved. No production/header changes. See [[matching-notes]].

TILE adopted real abs_y local reaches94/257 with every stack home/saved role exact; ordinary replay and diagnostic pre-as1 capture output agree. First apparent preload difference originates in late latch; earliest emission ring difference is geometry-y. Provenance and exhausted controls recorded; no speculative edge load.
