---
id: T-25023
title: Close BUNKA_SD scheduler residue in src/ovl/BUNKA_SD/801388D0.c
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25020-bunka-sd-trace-closure]]", "[[tickets/T-24927-func-80139290-801388D0]]", "`plan-remaining`"]
---

## Goal

Investigate only func_80139290 from the four-position guarded draft at main 883760224. Retain bb42/44 li fillers while ordering bb43/45 year loads before the store-address lui.

## Acceptance criteria

- [x] Establish an untested causal source lever, or prepare a bounded Astra packet promptly.
- [x] Measure whole-TU resolved C; assembly fallback MATCH does not count.
- [x] Preserve source, exact metric, evidence and earliest divergence; root owns integration checks.

## Notes

Exclusive C file: src/ovl/BUNKA_SD/801388D0.c. Isolated worktree /private/tmp/tokimemo-t25023, base 883760224. One active worker; no old workers revived. No shared headers/tools, option/syntax sweeps, all-arm empty tests, volatile-year or real-local repeats.

## Initial diagnosis and consultation

Whole-TU guarded C reproduced at four changed zero-based positions 166/167/171/172, eight printed lines, aligned cost 4, 379/379 instructions. Earliest divergence 0x80139528 is year lbu versus store-address lui. Fresh print-only tracer matches stock .text/.rodata/.rel.text/.rel.rodata. Single object refreshed; no source experiments or shared changes. No untested causal lever established outside the prior negative ledger. Bounded read-only Astra packet: drafts/T-25023/README.md; private capture build/t25023-cap. Root authorized this consultation explicitly; Sol idles during it.

## Bounded result

Read-only GPT-6 Astra consultation: no new justified source lever, no edits or compiles. Global scan follows original INST order; local scheduling priority is independent. Current bb43 takes li t2,0x60 at INST 4 after rejecting store-address lui; bb45 takes li t8,1 at INST 2. All-arm empty tests replace these with the earlier year-address lui t0. Correction to older prose: li t2,0x60 aftercycles is 0, not 2. The local tie is lbu versus store preparation.

Revisit only with uopt-output/ugen evidence for separating constant emission from the year memory-expression group without introducing another allocated range (especially bb45 store value versus store address). A sufficient IR ordering is required li before year lui, then year lbu before store-address lui; no source recipe was established. No source experiments performed in T-25023, no source/header/tool changes, no C match or coverage. Exact packet and private trace references are in drafts/T-25023/README.md. Awaiting root review; all integration checks remain root-owned.

## Verification

Single-object baseline rebuild and whole-TU resolved C measurement completed in Docker. Print-only trace fidelity checked on all four relevant sections. Wiki lint: 0 broken links, 0 status mismatches; one existing large-page warning (data/t0018-cases). `git diff --check` clean. No full build or repeated declaration checks for this documentation-only result; root performs central integration checks.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
