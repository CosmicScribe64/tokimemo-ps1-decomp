---
id: T-12040
title: Codex cx4 RPG battle matching
status: In Review
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over the eleven functions in ../codex/cx4-list.txt, owning only RPG_BAT objects 801340F0, 8015A1F0, 8014DF60 and 801321D0. Branch cx4 starts at w1-good.

## Acceptance criteria

- [x] Investigate assigned functions and document guarded drafts and blockers.
- [x] Every promoted function passes linked funcdiff and overlay checksum verification.
- [x] Final configure/build verifies 27 binaries, headers, globals and branch prototypes.
- [ ] Submit for standards review.

## Notes

Docker only, ninja -j2 while workers share resources. Traced uopt copied by orchestrator to build/scratch/r9-uopt/variants/trace. No new dependencies or toolchain installation. Baseline log: build/scratch/baseline.log.

## Comments

Independent review required before Done.

Baseline completed: Docker configure/ninja -j2 passes all 27 binary checksums, headers and globals (build/scratch/baseline.log).

### func_80132EE0 matched (256 bytes)

The saved m2c draft retained an initial counter local across calls, creating a stack spill and stale-value behavior. Replaced those uses with the current shared D_8015EDB0.unk_04 field, matching the neighboring movement helpers. Linked funcdiff MATCH256 and RPG_BAT SHA1/globals OK. No FAKE. Evidence build/scratch/32EE0-match.diff and 32EE0-build.log.

Initial full-file measurements: 5A74417,4E30010,3415C22,3457022,5A8B427,32EE041; other saved drafts require missing declarations or current aggregate members before scoring. Literal diff rows supersede older ticket counts.

### func_8014E300 matched (692 bytes)

The first position/displacement store pair needed an aggregate view of the shared EC28..EC57 work range. A local RpgMotion30 pointer makes both loads and stores use that range, preventing as1 from hoisting the second coordinate load ahead of the first displacement store. Source-only or destination-only views and line joining leave ten rows. Plain C matches692 linked bytes; full header rebuild27/27, headers/globals OK. No FAKE. Evidence build/scratch/third-variants.log, 4E300-match.diff and 4E300-build.log.

### func_801321D0 matched (376 linked bytes)

The saved draft preserved the dispatcher selector across a side-effecting call and reconstructed a temporary range-test goto. A direct condition followed by switch(D_8015EDAC) reproduces the original reload and table dispatch. Added the missing shared halfword declaration and later helper prototypes. Plain C linked MATCH376; full rebuild27/27, headers/globals OK. No FAKE. Evidence build/scratch/321D0-match.diff and 321D0-build.log.

### func_8013415C matched (1044 linked bytes)

Autotweak273 candidates left22 rows. Traced uopt identified the explicit increment-result locals as competing register webs. Replacing both increment/copy sequences with D_8015EE0C += amount and testing the global removes the v0/v1 swap; changing only one produces66 rows. Natural C, no FAKE. Linked funcdiff MATCH1044 and RPG_BAT SHA1/globals OK. Evidence build/scratch/increment-twins.log, 3415C-regdiff.log, 3415C-match.diff and 3415C-build.log.

### func_80134570 matched (1044 linked bytes)

Applied the twin's two compound increments and direct counter tests. Plain C linked MATCH1044, RPG_BAT checksum/globals OK. No FAKE. Evidence build/scratch/increment-twins.log, 34570-match.diff and 34570-build.log.

### func_8015A1F0 matched (1252 linked bytes)

Restored selector reloads after calls, replaced three increment-result locals with direct compound updates, and expressed the zero-counter initialization as a normal if assigning sixty. The m2c var_at negation temporary created a competing register web; removing it while using the global expression is exact. Plain C linked MATCH1252, full header rebuild27/27, headers/globals OK. No FAKE. Evidence build/scratch/5a-fifth.log, 5A1F0-match.diff and 5A1F0-build.log.

### func_80132348 matched (1404 linked bytes)

The same compound-update recovery reduces147 rows to12. Its remaining load/store schedule is solved with a shared EC50..EC6F displacement/rectangle view, following 4E300's evidence. All function accesses through the view match; changing only the close store pair worsens allocation. Plain C linked MATCH1404, full header rebuild27/27, headers/globals OK. No FAKE. Evidence build/scratch/32348-increments.log, 32348-view.log, 32348-match.diff and 32348-build.log.

### Remaining bounded passes

All eleven assigned functions attempted. Seven exact promotions total6068 linked bytes. One existing guard retained; three functions remain INCLUDE_ASM with scratch-only drafts. No new FAKE code.

| Function | Final outcome | Experiments and blocker |
|---|---|---|
| func_8015A744 | Existing guard,17 listing lines | Signed/unsigned return views and new exact0x44-byte record/shared-work-range views unchanged. Two register copies and global-store scheduling remain; 5a744-view.log, return-variants.log. |
| func_8015A8B4 | Assembly retained; corrected scratch draft50 rows | Saved27-row m2c draft reads the next record at the tail, but the original saves the current record's halfword. Corrected reload/saved-coordinate forms differ by one scheduling insertion and missing a0 copy, causing aligned-row cascade. Local widths, order, early index and record-value forms did not close it. An unsigned24-row variant changes comparison/division semantics and was rejected. 5a8-repair.log, 5a8-signed-close.diff, 5a8-index.log. |
| func_80132B34 | Assembly retained; scratch154 rows | Known constant-folding gap. Repaired current aggregate fields then tried six signed/unsigned/narrow-local and pointer forms; all fold derived values rather than retaining the original addiu expressions. 32b34-final.log. |
| func_801328C4 | Assembly retained; scratch97 rows | Fixed erroneous pointer increment(8 words versus8 bytes), full64-byte copy rather than split60+4, direct counter update, initializer types, declaration cleanup, tail-loop forms and phase local. Remaining copy-address CSE, stack homes, counter allocation and evaluation order. Larger variants107..194 rows discarded. 328-copy.log, 328-init.log, 328-loops.log. |

All logs/drafts are under ignored build/scratch/. Compiler experiments used Docker and the existing orchestrator-supplied traced uopt at build/scratch/r9-uopt/variants/trace. No new toolchain/dependencies, no inline assembly, no per-function compiler tweaks. Small numerical eight-direction initializer trials stayed scratch-only; no new data/config changes were retained.

### Review handoff

Ticket and card moved to In Review; independent CODING_STANDARDS review remains required. Seven functions have individual linked-byte proofs and immediate commits. Final configure/build, current guarded score and prototype checks recorded below after completion.

Final verification passed: configure/ninja -j2, then explicit invalidation of verification stamps and a full rebuild reran all 27 binary SHA1 checks, headers and globals successfully (build/scratch/final-build.log, final-all-checks.log). Branch prototype check passes with zero new and fifteen known disagreements (final-protos.log). Current full-TU func_8015A744 draft remains17 listing lines (final-guard.diff). `git diff --check` clean; branch source diff contains no added inline assembly. All seven promoted functions have no new FAKE annotations or artificial source tricks.
