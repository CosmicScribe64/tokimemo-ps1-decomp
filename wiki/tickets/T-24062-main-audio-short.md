---
id: T-24062
title: Recover short main audio helpers
status: In Review
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-18
links: ['[[main-family-results]]', '[[data-types]]']
---

## Goal

Seek function-count gains in func_8007B64C, func_80079C70, func_8007BCFC and func_8007BBE8, only src/main/80079B10.c. Read prior T-17000/type and failed-source evidence; focus on concrete aggregate/control-flow gaps, no blind grids. One Docker compiler; coordinate headers.

## Acceptance criteria

- Target INCLUDE_ASM absent; normal default literal proof and MAIN checksum for every promoted match.
- Preserve corrected source/reproducer if blocked; independent standards review before Done.

## Notes

Preserve 9b2a3bbed ancestry. Queue return must be read before shifting; Work80125D10 already canonical.

## Mixer candidate

func_8007BCFC scratch matches236 bytes with a real four-byte buffer, attenuation byte, paired work-field copies, required byte-store grouping and unsigned halfword view (both source constraints FAKE marked). Root approved main_api.h declaration `s32 func_80087DC8(u8 *)`: wrapper in asm/libcd_rest.s calls CD_vol then returns1; CD_vol loads exactly bytes0..3. No existing CdlATV SDK type in include/. Scratch CdlATV struct also matched but no new SDK struct introduced. Installed unguarded C for default proof.

Correction: absolute D_80125D30/34/32/36 are Work80125D10 offsets20/24/22/26; the archived T17000 fields were already correct. No production wrong-offset edit was made.

## Verified stopping point

One unguarded C match: func_8007BCFC236 bytes. Normal default literal proof MATCH, MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26. Source gate confirms no target INCLUDE_ASM. Matched siblings func_8007B5EC/B640/BDE8/B8F8 also pass default literal proofs. Header/global/prototype guards pass. Only source target changed; only shared addition is the root-approved mixer wrapper declaration. Two FAKE constraints documented in source.

Other source unchanged; best archived queue-array44, flag-loop37, corrected table-record109. Source-only reproducer/results in drafts/T-24062. User requested stopping point then pause; no next function. Await independent standards review and root all-unit integration before Done.

## Root integration review2026-10-18

Reviewed mixer against CODING_STANDARDS: canonical four-byte pointer declaration, C89, explicitly FAKE store grouping and unsigned halfword view. Actual default C literal236bytes independently verified along with all27 hashes and header/global/prototype guards. Other three targets remain unmatched source checkpoints; ticket stays In Review. Root worker stopped; only DATE and ENDING Sol workers continue.
