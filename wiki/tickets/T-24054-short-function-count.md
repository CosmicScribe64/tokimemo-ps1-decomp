---
id: T-24054
title: Recover short MAIN OPTION KANGEI functions
status: In Review
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-18
links: ['[[main-family-results]]', '[[option-family-results]]', '[[kangei-family-results]]']
---

## Goal

Seek verified function-count wins in main func_80043010 (176 B), OPTION func_80132000 (176 B), KANGEI func_8013852C (180 B), restricted to their three assigned translation units. One Docker compiler; switch targets without a concrete lead.

## Acceptance criteria

- Unguarded C source gate, normal default literal proof and affected unit checksum for every win.
- Preserve useful precise negatives and existing production fallback otherwise; independent review before Done.

## Notes

Read historical family notes and prior target drafts first. OPTION selector/brightness store lifetimes are a known52-row blocker; no blind temporary-type sweep. Coordinate shared headers before editing.

## Results

No new C function. KANGEI typed arithmetic recovery improves current baseline34 to6 to4 aligned cost; remaining sum temporary and scheduling gap. MAIN array/named raw bytes70/64; OPTION exact-width/selector forms52. See drafts/T-24054 for source, complete probe metadata and tested reproducer. No production/header edits. Await independent archive/standards review.

## Verification

Docker reproducer returns cost4; default affected unit SHA1s pass: main e823bd844a8f8fa4d05483b59c66bc54b8393b26, OPTION 3fd443d1e32404e5febd57d0586ef1c70ce8a811, KANGEI b94bbaceb7083c4b56d3b2ff56313cbcfbe6ac83. Header/global/prototype guards pass. These checks validate unchanged fallback units, not a new C match.
