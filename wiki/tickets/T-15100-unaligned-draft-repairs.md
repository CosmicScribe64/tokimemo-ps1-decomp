---
id: T-15100
title: Recover nine unaligned aggregate-copy draft failures
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-14500-exhaustive-drafts]]", "[[tickets/T-14600-date-draft-repairs]]"]
---

## Goal

Recover usable compiling C for nine reserved unaligned-copy failures, preserving initial attempts and routing any zero comparison to literal proof.

## Acceptance criteria

- [x] Archive body, declarations, full contexts, diagnostics and provenance for every target.
- [x] Reconstruct genuine aggregate/string sizes from copy/tail instructions; no arbitrary padding or zero-fill stand-ins.
- [x] Record every residual blocker and send zeros to proof lane.
- [ ] Standards review before Done.

## Notes

Reservation: drafts/T-14500/unaligned-repair-candidates.json. Nine scopes in BUNKAKEN, BUNKASAI, ENDING, GEKO, GYOZI, KANGEI and SHOUGATU. Reuse T-14600's genuine byte aggregate reconstruction. Source ASM is immutable input; all compilers/dependencies run in existing Docker image, serially. No production source changes for merely compiling nonmatches.

## Comments

2026-10-17: The archive (nine compiling drafts, 17,408 target bytes, zero coverage gain) is on main, and the types-first passes (T-17500 to T-18700) redrafted the same nine scopes with similar scores; all nine functions are still INCLUDE_ASM. Review found no src or header change but two hygiene issues, opened as a follow-up: about 5,550 rows of target instruction words in the comparison.txt files, and an untested one-shot tool that reads a /ref mount. Closed as Done.

Followup opened: [[tickets/T-21009-draft-archive-hygiene-comparison-rows|T-21009]].

## Results

All nine full-TU alternatives compile serially with the existing Docker IDO 5.3 path. No zero candidates, no production edits, **0 matching coverage gain**. Previously failed input now has usable compiler drafts for 17,408 target code bytes. Complete artifacts, hashes, comparison and reproduction details are under drafts/T-14500/repairs-unaligned.

| Scope | Target code bytes | Diagnostic rows |
| --- | ---: | ---: |
| BUNKAKEN:func_80144F70 | 1424 | 426 |
| BUNKASAI:func_80142700 | 1176 | 336 |
| ENDING:func_8013A2E8 | 4648 | 1667 |
| GEKO:func_80142C9C | 3656 | 1122 |
| GYOZI:func_801368B8 | 2256 | 701 |
| KANGEI:func_8013769C | 1256 | 142 |
| SHOUGATU:func_80133B44 | 408 | 92 |
| SHOUGATU:func_801350F8 | 2308 | 995 |
| SHOUGATU:func_801359FC | 276 | 69 |


The common unsupported M2C_LWL form is now a complete genuine aggregate copy or ordinary automatic string initializer. GEKO also required byte-accurate array aliases: the 24-byte halfword table at sp420, alias sp422, 21 writable flag bytes and the sp34+13 sentinel. Its GameState walking pointer steps by bytes in the target, not entire GameState records. Festival traversal pointers likewise use byte strides. SHOUGATU's chained pointers perform byte stores as the target does.

ENDING consumes nine dialogue_render integer results, supported by existing main/8007C030.c's override; its archived full context uses the same explained ABI view. The suspicious raw decoded string is replaced with the observed 27-byte symbol aggregate. No production header changes follow from this draft-only ABI evidence.

Residual differences are broad stack layout, instruction/control-flow, register allocation and inferred-type gaps. In particular GEKO and ENDING remain distant despite compiling; do not promote these bodies or treat diagnostic compilation as semantic proof. Small SHOUGATU 359FC remains 69 rows, principally frame/local and register differences. Initial census artifacts remain untouched.

## Review

Self-check: nine final full-TU compiler/comparison runs succeeded; no M2C_LWL/M2C_ERROR placeholders in repaired bodies; genuine observed buffer extents, C89 declarations, no production source edits or raw assets. Independent standards review pending; In Review.
