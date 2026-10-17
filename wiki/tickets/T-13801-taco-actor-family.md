---
id: T-13801
title: Reconstruct TACO actor initialization and update family
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-13800-taco-families]]"]
---

## Goal
Inspect assembly and matched siblings, select a bounded actor/table family, and verify candidates in full translation-unit context.

## Acceptance criteria
- Record successful and failed evidence; accept only literal linked matches plus overlay SHA1.
- Review against CODING_STANDARDS.md before Done.

## Notes
Existing TcActor and DisplayActor44 are primary type leads. Prior cx7 owns no new work here.

## Comments
Claimed before source work.

### Initial family selection
Fresh m2c reads for 3D528/3D838 reveal unresolved SDK projection locals and matrix identity copies; deferred. Selected adjacent boss actor transforms 4A480 (796 bytes), 497EC (912) and 487A4 (948): all copy D_80125C60 into MATRIX, rotate, transform TcObj50 positions, and update TcActor fields. Existing TcPos/TcObj50/TcActor provide the required layouts. Initial natural 4A480 full-TU compile needed the existing same-TU 5163C definition declared before its first new caller; overlay header declaration added. v2 has matching broad instruction shape but +8-byte frame, scalar lifetime/register differences and two aggregate-base comparisons. No promotion yet. Evidence build/t13801/{a480-v1,a480-v2}.diff; candidates restore production source after each comparison.

### Actor aggregate/lifetime results
4A480 improves to three differing instruction rows in full-TU context: separate x/y/index locals retained, matrix aggregate copy and TcPos vectors, no cached selector/index locals; mixed pointer addition order is required. Remaining one commuted actor-add and a two-row constant/store-address scheduling swap. Natural direct end reread changes the register web and shrinks the frame, so it cannot be substituted blindly. Named comparison symbols are temporarily declared only in scratch tests; any accepted source must add canonical main declarations with complete checks. Bounded evidence: build/t13801/a480-{unify,both,storeorders,endbytes}.diff. No production function promoted.

2026-10-17: Research handoff, no source reached main. Main's guarded func_8014A480 draft (T-19003) sits at 12 differing lines with spill slots 4 bytes off, while this ticket's form without cached index/selector locals had the right frame and three rows; it survives only in taco-family-results and the T-11920 backup (taco-13800.tar.gz, build/t13801). Opened a follow-up to carry that form into the draft; func_801487A4 and func_801497EC are covered by T-19013 and T-19019. Marked superseded.

Followup opened: [[tickets/T-21008-taco-8014a480-three-row-form|T-21008]].

## Diagnostic handoff

No production function accepted: zero new matched bytes. Best 497EC has one commuted pointer-add row; 4A480 has three rows. 87A4 natural full-TU candidate has 62 rows. See [[taco-family-results]] for bounded experiments and ignored build/t13801/reprobe.py for reproduction. All production source/header trial edits restored. Final default overlay verification and independent standards/spec review are recorded below.

### Final verification and review

Docker reprobe.py reproduces 497EC one row and 4A480 three rows, no compile errors, and restores source bytes. Subsequent `ninja -j1 build/ovl/TACO.ok build/headers.ok build/globals.ok` exits 0; TACO SHA1 OK (1419e93cbad6c0df790577701181be04e0747969). Logs build/t13801/{reprobe,final-verified}.log; the earlier full rebuild records headers OK. Production src/include diff versus 97ec697f is empty. No whole-game checksum claim is made.

Independent GPT-6.1 Sol medium standards review: no findings. Spec review: primary matching goal remains unmet (zero accepted bytes); the stale pending-validation statement is now corrected. Tickets remain In Review as a research handoff, not Done. No shared declarations, guards or FAKE workarounds were installed. Original checkout remains unchanged by this task.
