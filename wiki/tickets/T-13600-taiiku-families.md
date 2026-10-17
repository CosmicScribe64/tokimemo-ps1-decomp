---
id: T-13600
title: Recover TAIIKU source families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[bunka-family-results]]", "[[tickets/T-12020-codex-cx2]]"]
---

## Goal
Add verified TAIIKU linked bytes by reconstructing reusable scene, selector and type source families.

## Acceptance criteria

- [ ] New matches pass default full-TU literal proof and TAIIKU SHA1.
- [ ] Existing matching C remains unchanged; headers, globals and prototypes checked.
- [ ] Standards review completed and findings resolved.

## Notes
Baseline 5ccb8765 (latest origin/main). Production ownership: src/ovl/TAIIKU and include/ovl/TAIIKU.h. Use one Docker compiler, existing image, no host dependencies. Read merged Bunka family and cx2 evidence before candidate selection; avoid known register-only mutation dead ends.

## Comments

### 2026-10-15 initial family evidence

All probes run in the existing Docker image with one compiler; no production changes yet. TAIIKU scene handlers 41CDC/40A90 corrected full-TU baselines are 98/220 diff lines. Bunka's GameState-pointer form does not improve them; the shared principal gap is unexpected IDO runtime unrolling of the two-actor byte-flag loop. Address-valued, canonical actor-pointer and bitfield forms do not close it, so no further isolated loop mutation is planned.

Actor initializer 43814 reads two nine-halfword tables at A2F0/A304, copied as sixteen bytes plus a halfword. Fresh m2c treats them as scalars, and its halfword pointer steps are scaled twice. Recovering table types, indexed local arrays and DisplayActor44 fields reduces a 217-line typed draft to 75 lines. Remaining differences include frame/table homes, loop branch shape and global-tail constant allocation; it remains scratch-only. Existing TaiShorts12 is already a nine-halfword type and can be reused if a production match is found.

The controller/reset family 4607C/46338 is the next bounded test. Existing TaiikuController and TaiikuSlot1C document 0x1C strides; 4607C additionally confirms controller bytes at01/02 and slot fields0E/0F/18 from byte/halfword stores. Scratch models preserve those widths without changing shared declarations. Sources and probes are retained in build/taiiku13600.

### Controller and held-out initializer result

The reset 4607C can be expressed through 0x1C controller/slot views with the observed widths, reaching 52 full-TU diff lines after making its loaded stat a word local while retaining the explicit halfword truncation after division. Its loop still normalizes to a constant-equality comparison rather than the target signed bound; the other residual gaps follow register allocation and constant ordering. A three-record indexed source form gives the same 52 lines, and a halfword counter with signed bound worsens to117. No extra counter mutation or padding is accepted.

Held-out 46338 confirms two twelve-halfword local tables at A5C4/A5DC (size0x18 each). Typed pointer and indexed forms remain264/256 lines, dominated by stack/table homes, loop branch form and tail allocation. This family test yields no accepted matches. Untouched larger siblings3D9B4/3DDB8 in80138950 are the next source inspection.

### Larger polygon/coordinate family

Untouched 3D9B4 (1,028 bytes) uses a PsyQ POLY_FT4 allocation and six arrays of eighty signed halfwords at99E4/A84/B24/BC4/C64/D04. Its sibling3DDB8 writes the same arrays in groups of four positions. The0xA0 spacing and stride-two walks establish the array bounds and element widths. Scratch type recovery uses the existing SDK POLY_FT4 (include/psx_types.h), repairs fresh m2c's doubled pointer scaling, and uses the canonical SetShadeTex name. The full-TU result is346 lines, with substantial frame and register-range differences; no source is accepted.

Bounded total at this checkpoint: **0 matched functions / 0 verified bytes**. All production files and headers are text-identical to5ccb8765. Driver scripts and outputs remain ignored under build/taiiku13600, with missing global declarations supplied only in temporary compiler context. No toolchain flags, assembler output, or coverage accounting changed.

### Ranked lower-complexity follow-up

The queue has84 remaining functions,66,732 bytes; three failed families do not establish exhaustion. Candidate ranking weights complete local types and matching sibling donors before size:

| Rank | Candidate | Bytes | Type/donor evidence | Prior explicit failure found |
|---|---|---:|---|---|
|1 (superseded)|46A60|300|Controller state/animation helper; matching46884/4692C/46994/46B8C share controller and actor views|Subsequent full census found T-4100 and wave2/w7 register failures in data/t0018-cases|
|2 (superseded)|47928|324|Four-slot helper;47520/4756C and47A6C provide matching slot access donors|Subsequent census found T-9570 time-boxed code-generation mismatch|
|3 (superseded)|47318|520|Three-controller slot family;470A0/47120/47210 match nearby|Subsequent census found T-9570 time-boxed code-generation mismatch|
|4 (superseded)|44E54|496|Race state/slot family, matching44768/44834/448D4 nearby|Subsequent census found T-10010 compile_error in log|
|Deferred|368B8/37228/3CF88/3C780/3D114|various|High raw queue rank|Documented previous compiler/register failures; do not repeat|

Concrete next family: controller/slot state helpers46A60 and47928, starting from existing types and matching sibling access forms.

### Full prior-evidence census and first verified wins

Initial helper ranking searched only selected notes and missed historical case/ticket evidence. Corrected census of the84 baseline functions (66,732 bytes):70 have unit-tagged wiki evidence,2 only ambiguous address mentions,12 no mention in the searched pages (log/index/kanban excluded; one of the twelve,44E54, has a logged compile failure). These are evidence counts, not an exhaustion claim. Full inventory: [[data/t13600-remaining-census]]. The concrete fresh lower-complexity family is adjacent scene dispatchers33494/33644; both have complete canonical GameState views and matching sibling dispatch donors in80132BD0.c.

- **func_80133494: literal linked MATCH,432 bytes.** Canonical fields alone give zero full-TU differences.
- **func_80133644: literal linked MATCH,520 bytes.** Replace m2c's cross-branch goto in outer case0 with an ordinary inner switch on GameState.unk_F74. That removes14 full-TU lines; the GameState-pointer alternative worsens to36. The inner-switch source form follows the verified Bunka direct-dispatch family.

Default targeted ninja reports TAIIKU.bin SHA1 OK. funcdiff --resolve --no-build reports both linked function byte ranges equal the target, total **2 functions /952 bytes**. Headers/globals pass. An inverse replacement audit proves all source text outside the two INCLUDE_ASM replacements is unchanged. No header changes, FAKE workarounds, flags or raw assets. Logs: build/taiiku13600/first-wins-build.log, first-wins-proof.log and first-wins-protos.log. Standards self-check covers C89, canonical globals, original symbol/order preservation and no unrelated source edits; independent review remains pending.

Failed helper probes46A60/47928 retain14/65 lines respectively; those bodies remain scratch-only.

### Third verified scene dispatcher

**func_80139330: literal linked MATCH,544 bytes.** Its direct canonical-GameState draft leaves four +/- lines (one load/store pair); grouping the stores on one source line does not help. Writing the four x-coordinate accesses through the existing DisplayActor44 view reaches zero without artificial statements or formatting. Added canonical forward declarations in TAIIKU.h for the dispatcher and its five already-defined void helpers; whole-overlay SHA1 proves these declarations preserve all pre-existing C output. Source text outside the one INCLUDE_ASM replacement is identical.

Default ninja: TAIIKU SHA1 OK, headers/globals pass. Literal proof: build/taiiku13600/second-proof.log; build log: second-build.log. Cumulative gain: **3 functions /1,496 verified linked bytes**. Independent review remains pending.

Further bounded scratch results: the complete-type state twins3BB3C/3C33C remain141/139 lines after fixed-point union and canonical fields; race initializer44E54 remains139 after actor views and explicit resource snapshots. Fresh32BD0/34F98 inspection finds substantial copied-table/unrolled-stat reconstruction, so neither is treated as a simple dispatcher. These candidates remain assembly-backed. Larger sibling39610's natural thirteen-character stat-loop source is under test.

### Bounded follow-up and disk checkpoint

The larger scene sibling39610 confirms the thirteen-character GameState stat-loop family. Canonical fields and ordinary loops give68 lines; placing the zero initialization inside its threshold branch gives16, with the target64-byte frame and stat loops matching. Remaining differences are the D499D4 pointer register and associated spill/reload order. Indexed, reordered and return-type variants give16-28 lines. No register-only workaround is accepted, and this candidate remains assembly-backed.

The fresh34F98 scene reconstructs an eleven-word local table and thirteen-character stat loops, but remains254 lines, dominated by table homes and pointer lifetimes. The smaller3D788 stat-threshold helper remains175 lines after a natural condition rewrite; using an existing fixed-point union pointer gives171. Testing an old-style callee declaration removes an unnecessary byte-argument mask but does not solve the function. No ABI/header changes are retained from that test. These bounded failures are saved in ignored build/taiiku13600; they do not establish exhaustion of the remaining queue.

At the disk checkpoint, root reports only2.9 GiB free. No further full builds or bulky outputs are planned while it investigates. All accepted source is committed; cumulative literal-verified gain remains **3 functions /1,496 bytes**. The completed RPG checkout contains about913 MiB of copied ignored disc/asm inputs, and TAIIKU contains about914 MiB. Diagnostic scripts are preserved without duplicating game data.

### Resumed resource-dispatch family

After root's cleanup restored18 GiB free, fresh45640 (1,080 bytes) was tested with ordinary nested dispatch and recovered resource arrays atA4F8[12]/A53C[4]. Both loop bounds and48/16-byte data islands establish these dimensions. ExistingA070[6] is the release-table sibling. Fresh m2c double-scales GameState.unk_1228 indexing and prints obsolete func_800485BC with stale arguments; the canonical API already exists as search_load_tpage_buf_lock(s32,u8), so no shared declaration is needed. Calling the resource API before constructing its encoded ID, and placing zero initialization inside the ready branch, reduces145 lines to114; separate pointer locals for the two resource tables reduce this to61. The remaining gaps are stack homes/spills and the end-symbol address expression. No stack-order sweep or production change is accepted.

Next fresh helper44464 (572 bytes) copies four signed halfwords fromA318 (eight-byte data island) and applies them to actor coordinates. Corrected pointer stepping, scratch-only table type, and missing actor-width/end-symbol declarations compile, but remain197 full-TU lines. Missing actor fields are signed halfword20800 (actor102 offset18) and byte20873 (actor104 offset03), with widths confirmed by instructions; no production declaration is added without a match. The helper remains assembly-backed. Drivers and source/diff outputs are retained in build/taiiku13600.

### Fresh timing/animation helpers

UntouchedBCEC (524 bytes) has an m2c LO16-collapse bug: its final two stores both printA4, while target instructions storeA4 thenA6. Correct canonical aggregate accesses, callee prototypes and the second coordinate give116 full-TU lines; ordinary state-range conditions give115. This is a distinct control-flow case, with no accepted body.

Untouched43DD0 (384 bytes) has the same animation/state update pattern as matched44B40. Its tables are signed halfwords (target lh instructions); using existing byte storage as a halfword view corrects m2c's byte load, and direct per-branch state updates reduce109 to36 lines. Remaining differences include byte truncation/store placement and m2c's redundant tail stores. Duplicating the branch stores gives58, so no artificial self-store workaround is retained. All attempts remain scratch-only; cumulative gain stays1,496 verified bytes.

A final isolation of43DD0's matching44B40 donor form keeps only the per-branch zero resets and removes the rejected extra tail-store duplication. This improves36 to20 full-TU lines; the remaining differences concern the redundant tail store/load allocation. No register sweep follows. The twelve candidates without baseline wiki mentions have now been inspected or bounded-tested (including the three verified wins); this is a limited fresh-candidate inventory, not exhaustion of the84-function baseline. Further prioritization can use the separate exhaustive draft/scoring catalog.

2026-10-17: The three matches (TAIIKU func_80133494, func_80133644 and func_80139330, 1,496 bytes) are on main as plain C with no FAKE or guard, and review against CODING_STANDARDS.md is clean apart from missing spaces before numbers in the wiki notes. The follow-up candidates it ranked (46A60, 47928, 47318, 45640, 43DD0, 44E54, 44464) were matched later by T-16553, T-16554 and T-19010, and the rest of TAIIKU lives in the draft catalog and wave queues. func_80139330 had also been matched by T-11640, so only one definition survives. Closed as Done.
