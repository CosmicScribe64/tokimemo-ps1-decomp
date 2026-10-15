---
id: T-13600
title: Recover TAIIKU source families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
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
