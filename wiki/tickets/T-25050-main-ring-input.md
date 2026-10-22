---
id: T-25050
title: Recover MAIN ring input function
status: In Review
assignee: Codex Astra worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[matching-notes]]", "[[tickets/T-25048-main-80076fdc]]"]
---

## Goal

Match func_80075C84 in src/main/80075320.c using canonical declarations and the frozen default compiler pipeline. Exclusive worker ownership is this function only in /private/tmp/tokimemo-t25044.

## Acceptance criteria

- [x] Literal default-C bytes match all 796 bytes.
- [x] MAIN checksum and declaration guards pass in Docker.
- [x] Source reviewed against CODING_STANDARDS; separate source/evidence pins supplied to root for full27 integration.

## Notes

Read T-24303, T-24501, T-25016 and current matching ledger before experiments. Existing two-symbol canonical ring removes the position reload after each key store; old single-symbol KeyRing changed neighboring matched function output. Plain local struct views and integer-pointer base variants have documented negative evidence and will not be repeated. No shared-header edits, compiler overrides or neighboring source ownership.

## Comments

2026-10-22: Selected the last remaining same-file function after accepted T-25048. Investigate the alias/reload mechanism with canonical globals; root owns full integration.

Canonical baseline reproduces338 diagnostic lines. Narrow volatile increment reload202 adds unwanted address instructions. Casting the complete key-store integer address (rather than the previously rejected cast of just the base) gives203 and retains the required t4 array base. Adding the raw input-word empty use restores switch selector allocation and reaches31 diagnostic lines; residuals are the hoisted outer-loop position load, one extra instruction and increment scheduling. These source controls remain private pending literal verification; likely FAKE mechanisms will be documented explicitly if retained.

Loop controls: score-store integer alias44, integer-cast position load98, volatile position load29 (extra promoted address and changed spill), natural indexed traversal115, split initialization31, position-address empty use31, self-store31, raw position empty use45. Complete integer address rooted at position-minus-eight168 changes key-base addressing, agreeing with the historical base warning; rejected. These are diagnostic-line counts, not literal proof. Parent suggests a new always-zero unsigned comparison in the position address to isolate late-folded loop dependency; testing without changing global declarations.

Parent's late-zero address dependency29 prevents hoisting but promotes the position address into ra; integer address form also29. A direct scalar load followed by an impossible unsigned-index branch and then the +7 gives28 diagnostic lines at exact instruction count; load stays direct. Traced ordinary compilation reproduces it: pointer induction v0, j v1, k a0; target j v0, k v1, pointer a0. k priority113.3 versus j136.7; extra definitions/control affect allocation. One spill is2C versus28. The remaining cursor increment occurs at the wrong loop point; reconstructing the target's early increment at the key lookup is the next independent source hypothesis.

Reordering the two real loop increments to k++, j++ fixes scheduling: four diagnostic lines remain, only a1 spill2C versus28. Explicit row-pointer control30 and postincrement-at-read128 rejected. Reusing k as the switch selector leaves the same four lines. Natural index without explicit k fails strength reduction (95 signed/158 unsigned; invariant-first association unchanged158). Expanded score assignment keeps the spill mismatch and additionally swaps two expression temporaries8. Testing the known real-local declaration-order spill mechanism from T-25048; no padding or extra locals.

Literal scratch match achieved:796 linked bytes equal target. Captured ordinary compiler ucode clarifies the spill: CFE local extent16, uopt spill offset-28, finalsp+2C. The target needs one more local extent word; a real s32 random local owning rand()'s result gives zero diagnostics at the unchanged frame and literal796 proof. No padding. Earlier interpretation that a local needed removal was disproven by this capture; negatives retained.

Promoted C uses three documented FAKE mechanisms: raw input-word empty use, integer key-store addresses preserving alias reloads, and the unsigned-impossible position store at the outer-loop tail (no emitted branch/store). The real loop increment order is k++, j++; no volatile, header or compiler changes. Parent review requested, default affected-unit gate running.

Address audit: actor halfwords8011ECF6/8011ECFA; input word800E7208; key bytes800B6990 and position800B6998;13x7 pattern table800B3778. Record word800E643C+i*0x38 and flags800E6448+i*0x38; result801230D0, bytes800E71DF/800E7313/800E738D, flag halfword800E6376, adjustment halfword800E639E. The record's first two canonical s16 members are intentionally accessed as the observed full s32 word. No table-name or normalized-relocation substitution is accepted as literal evidence.


Final gate: default C matches all 796 literal linked bytes; MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 passes. Headers/globals pass; prototypes report 0 new disagreements, 15 known. Parent standards/spec review reports no findings. The ticket remains In Review pending root's independent full27 integration.

Necessity controls removed the now-redundant integer key-address expressions: canonical array indexing still matches. Final source has only the raw input empty test and impossible unsigned tail-store FAKEs. Replay counts: current 0; remove raw use 180; add integer alias 0; remove tail store 148; inline random result 4; reverse increments 4 diagnostic lines. Earlier three-FAKE source and its tail-removal 35-line control are historical, superseded by this simpler final body. Source-only evidence is drafts/T-25050; private compiler traces and listings stay under build/.

Root independently reviewed the final simplified production diff again: no findings. Source-only pin 2ebd6fd3a contains only src/main/80075320.c. Wiki lint reports 0 broken links, 0 status mismatches and 2 existing large-page warnings. Evidence prose review retains the literal technical word "key"; heading and long historical-count paragraph were simplified for readability.
