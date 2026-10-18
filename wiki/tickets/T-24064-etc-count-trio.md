---
id: T-24064
title: Match ETC model initialization loop
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013F980 (252 bytes) in src/ovl/ETC/8013F980.c. Preserve matched func_8013FC64 and every other body. Former trio target func_80140AE4 remains deferred; func_80141ABC is separately preserved as unresolved in T-24067 and receives no new probes.

## Acceptance criteria
- Reuse built worktree on codex/complete-etc-model-loop from frozen T-24067 842583d66; preserve ancestry and inspect latest primary/origin main ownership and source before edits.
- Read current wiki, ETC family and prior T-13103/T-17300 negatives before novel probes; prioritize siblings and evidence-backed types over broad sweeps.
- One GPT-6.1 Sol medium worker/compiler; Docker tools only, serial ninja -j1. Reserved DATE work remains frozen.
- Coordinate exact shared-header/config changes before editing. Actual wins remove INCLUDE_ASM/guard and pass explicit default linked funcdiff, ETC checksum and header/global/prototype guards.
- Pin useful source/reproducer or verified wins promptly. Parent owns independent review/integration/push; ticket stays In Review until reviewed.

## Notes
80140AE4: prior unsigned-compare constant sharing, one-higher ugen temporaries, selector/result/width/clamp controls and ordinary switches exhausted (48 baseline versus switch54/58 rows). 8013F980: pointer do-while shape recovered but constant 0x801FA808 folds versus original saved pointer; later hand draft still prologue scheduling. 80141ABC: real eight-byte unaligned album copy, byte-only destination record and word counter reached27 rows; saved-register order, copy temporary and argument scheduling remain after pointer-order/end-pointer sweeps.

## Comments
Parent resumed only func_8013F980 after the unresolved T-24067 checkpoint. One existing Sol medium worker continues on this one function; no batches or added agents. DATE is independently published; preserve its source. The historical deferral and trio below record prior scope, not current authorization.

Verified252-byte default C win: matched MAIN func_8006190C/T-17010 supplies the integer TMD cursor, preventing the header-count load from folding into an absolute load. Real TaiikuBig/u32 pointers advance by one record/four words, correcting old draft scale errors. Source initializer order reduces10→4 rows; FAKE initializer/do same-line grouping resolves the final two setup instructions (original physical line layout unknown). The plain existing main_api declaration also matches, so the approved local three-argument override is unnecessary and was omitted; no interface/header/config change.

Docker ninja -j1 build/ovl/ETC.ok passes; actual unguarded `tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8013F980` reports MATCH252 linked bytes. Header/global/prototype guards pass (0 new/15 known disagreements). Exact replacement audit preserves every other TU byte, including matched func_8013FC64; target guard/fallback absent. Evidence: build/etc24064/default-build.log, default-proof.log, headers.log, globals.log, prototypes.log, body-audit.log. In Review pending parent independent review; other40AE4 deferred and41ABC unresolved, no new work on either.

Source-only provenance is drafts/T-24064/README.md, with exact10→4→0 private contexts, plain-header control, FAKE line-layout rationale and actual-default commands. Reviewed production checkpoint remains0f34968776ed3fc3254f5ac4f83ae6862f4932b1; documentation follows separately without source amendments.

User deferred this ETC package while the sole worker focuses on DATE func_80144E20. Status is Ready, not Done; no ETC compiler probes or production edits began in this package. Current read-only notes are the existing 48-row album interaction, pointer-loop prologue lead and 27-row unaligned album-copy history linked in the ticket. Resume only after new coordination.

Parent reports main 1911789b2, 6,507 functions / 1,851,416 verified bytes (81.2%). This package claims none of those prior gains. No additional DATE compiler or trace investigation.
