---
id: T-24006
title: Recover MAIN render family
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: []
---

## Goal

Recover verified C in src/main/8004C3F0.c: mod_set_gpu, mod_set, mod_trans_vram, srn_set, get_fnt, srn_tpage_show. Base f704501. Preserve prior work; no func_8006D7EC probes.

## Acceptance criteria

Default linked literal verification and affected MAIN checksum for retained matches. Standards review; provenance and bounded probe results.

## Notes

Exclusive source ownership, minimal shared header edits, Docker tools and ninja -j1.

## Comments

## Results

0 of 6 matched; no production source/header changes. Five source-only typed hypotheses retained in drafts/T-24006/ with stable aligned costs, instruction counts, source hashes and base SHA provenance. See [[main-family-results]] for the type evidence and concrete remaining frame/register/loop blockers. Prior archives and commits untouched; func_8006D7EC not probed.

### Verification

Docker IDO 5.3 probes, no host dependencies or compiler exceptions. Default target build ninja -j1 current; fresh MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 equals expected. Fresh linked literal default funcdiff MATCH for all six assigned ASM functions and four existing C siblings. Headers/globals OK; branch prototypes 0 new disagreements and 15 known. Default ASM matches are preservation evidence only.

### Review status

Moved to In Review for parent integration review. Self-audit: no production code edits, no raw bytes/generated assembly/assets, no changes to old archives, C89 source hypotheses, offsets/sizes documented for new record/request types, exploratory unused get_fnt pointer slots marked FAKE. New SDK packet types reuse psx_types.h. No new verified-byte claim.
