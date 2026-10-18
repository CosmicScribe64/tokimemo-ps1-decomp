---
id: T-24045
title: Recover TEL profile screen from retained near miss
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[matching-notes]]", "[[tickets/T-19027-w3-d-p27]]"]
---

## Goal
Close TEL func_8013D03C in src/ovl/TEL/8013CCD0.c from T-19027's typed 49-row draft and the adjacent matched function's shared-source-line store recipe.

## Acceptance criteria
- [x] Read prior negatives and reproduce the retained source through one serial Docker compiler.
- [x] Any promotion is actual unguarded normal default C, proven with literal linked funcdiff and TEL checksum.
- [x] Preserve useful source-only evidence and leave In Review for root standards review.

## Notes
Start 68126eccd. Only this function in the assigned TEL file, no headers without root notice. Prior frame padding/volatile base are disclosed FAKE; birthday-word reuse and the base-store schedule remain unresolved. No push/merge.

## Results

Matched func_8013D03C as actual unguarded production C: 2128 literal linked bytes. Current [13][7] icon table accesses, late base-pointer assignment, explicit three-arm age chain, and a marked FAKE nonvolatile pointer read view for the birthday month/day pair close the draft. The inherited padding and volatile base remain marked FAKE. The read view is compiler-specific and must be assessed in root standards review; it is not claimed as original source.

Approved TEL.h additions are TelSignDays (sixteen u32, size0x40) and D_80146FD0 typed extern, supported by copy extent and target indexed reads. No other source/header changed. Docker ninja -j1 freshly compiled production C, linked TEL and passed checksum/headers. Literal tools/funcdiff.py func_8013D03C --unit TEL --resolve --no-build --version default reports MATCH (2128 bytes). No target guard/fallback remains. Negative hypotheses and reproduction in drafts/T-24045/README.md; matched source-only body retained. In Review pending root review, no push/merge.

## Root review and publication

Reviewed against CODING_STANDARDS.md: C89, shared type extent, no assembly/compiler changes, three explicitly marked FAKE workarounds with negative evidence. Volatile-removing view accepted only as disclosed target-compiler fakematch, not portable/original source. Independent Docker build,27 hashes, headers/globals/prototype guards and actual-C gate pass. Default literal MATCH2128 bytes; published f828daec1. Moved In Review to Done.
