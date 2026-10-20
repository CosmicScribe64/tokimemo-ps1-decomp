---
id: T-24036
title: GEKO and SHUGAKU sibling completion
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[scenes-family-results]]", "[[shugaku-family-results]]"]
---

## Goal

Recover GEKO func_80140040 and SHUGAKU func_80134C98 from matched siblings, preserving the integrated GEKO func_8013FD88 body. No other function/header changes without parent notice.

## Acceptance criteria

- [x] Actual unguarded C plus default literal linked proof and unit checksum for claimed wins.
- [x] Useful failed source forms preserved.
- [x] Header/global/prototype checks pass.
- [x] Independent review.

## Notes

Merged latest main from a clean tree (db89e0ce); wiki-only conflicts preserve both histories, no C conflict. Read scenes/shugaku families, T-17700/T-18000 and older GEKO7-row reload/scheduling negatives before reconstruction. One Docker compiler/ninja-j1; bounded20min.

## Verified batch1

SHUGAKU func_80134C98 matches244 default linked bytes using direct matched func_80133F6C structure, canonical GameState fields and its required extra state/flag stores. No FAKE or header change. Target ASM removed; Docker ninja-j1 SHUGAKU checksum passes, --resolve --version default --no-build reports linked bytes equal target244. Headers/globals pass; prototype branch check0 new/15 known. GEKO's matched func_8013FD88 remains identical to main.

## Bounded final result

One of two matched: SHUGAKU func_80134C98,244 default linked bytes, pin e8662459b. Direct matched sibling func_80133F6C plus required extra state/flag stores matches with canonical GameState fields; no FAKE/header change. The older typed temporary draft differs46/46 rows/aligned. Default --resolve --version default --no-build proves244 bytes after actual target ASM replacement; SHUGAKU SHA1 passes e9c64e952a1ef5dfea5e9b3e6a659aa6ccb8eb8c. Headers/globals pass; prototype branch check0 new/15 known.

GEKO func_80140040 remains guarded, zero additional coverage. Latest-main mixed record-field guard actually scores91/91 despite the historical7-row comment. Porting the matched EVENT byte-view access recipe restores7/7; production guard refreshed and byte-view FAKE explicitly marked (same T-20900 rationale as sibling). No matched neighbor changed: func_8013FD88 source remains identical to main. Corrected word-field aliases worsen110; the malformed scalar .w experiment failed loudly and is preserved as a diagnostic negative.

At GEKO tail, the original branches over the opaque call then unconditionally reloads the counter before loading ra. Current code duplicates the tail and hoists the ra load on the no-call path. Meaningful tail locals, branch-local increments and alias reads do not match. Whole signed counter alias forces unconditional reload but leaves8 register rows; a scratch-only signed declaration still gives7, so no header override was added. This distinguishes alias effects from the declared sign and is a bounded negative, not compiler-impossibility evidence. No volatile, compiler change or inline assembly used.

Clean parent-authorized merge db89e0ce incorporated latest main before work; four wiki-only conflicts preserved both histories, with no C conflict. Read index, scenes/shugaku family results, T-17700/T-18000 and prior tail evidence before probes. Source-only unresolved form and old typed control retained here; every loose input/script/source/diagnostic remains intact in ignored build/T-24036-probes/.

## Independent root review

Reviewed production source/header changes against CODING_STANDARDS.md; no unresolved findings. Actual unguarded C target passed default linked proof and all27 binary hashes plus headers/globals/prototype guards. Preserved guarded siblings and explicit existing FAKE documentation. Ticket remains In Review because the assigned sibling is unresolved. Published on main.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
