---
id: T-13500
title: Track compiler header dependencies for incremental builds
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[build-system]]"]
---

## Goal

Make source and header integrations rebuild only affected consumers while retaining the header validation gate and identical compilation/output.

## Acceptance criteria

- [x] Actual compiler includes tracked, including nested/conditional includes and missing headers.
- [x] Source, shared header, overlay header, ASM/rodata and no-op rebuilds tested with Ninja.
- [x] Header validation still runs before compiles and blocks failures.
- [x] Full 27 binary checks, baseline rebuild counts and standards review recorded.

## Notes

Isolated branch codex/build-deps-13500 at origin/main 5ccb8765. Own configure.py, tools/cc.py, dedicated tests and wiki only. No game-source, compiler-flag or matching-pass changes.

## Comments

- Initial inspection: main objects depend on every header; all C objects depend normally on headers.ok. IDO supports -M but rejects -MD. Its -M output is one target/dependency per line, with unescaped path spaces. asm-processor compiles a temporary transformed source with the input directory added after -Iinclude; dependency discovery must use that same input and search path.

- Validation: 11 new synthetic Ninja integration tests and 17 existing cc tests pass in tokimemo-decomp-arm64. Full updated build passed 27/27 binary checks; next build no-op. Baseline also 27/27.
- Baseline/updated dry-run compiles/tasks: source 1/5 -> 1/6 (header checker newly reruns); main_api.h 443/526 -> 442/525; ETC.h 443/526 -> 13/18. Actual updated source/ETC builds 8.981s/12.990s with checksums OK. All ETC compiles belong to ETC; no main/other overlay link.
- Implementation ready for standards/spec review against 5ccb8765; no production C/header or toolchain/matching-pass changes. See [[build-system]] for the dependency contract and test scope.

- Review fixes: path membership inventory now conservatively invalidates all C on file additions/removals (ordinary edits stay selective); missing watched directories have phony producers. Configure restat and co-output recovery avoid stale/no-op regeneration. New regressions cover higher-priority source-local/nested shadows, removed directories, missing inventory and unchanged-inventory regeneration. 131 related checker/prototype/padding/rodata/layout tests pass.

- Final verification after the membership fixes: all 27 binary SHA1 checks passed again. One configure regeneration followed the last configure.py edit; subsequent Ninja build is a no-op. Final touch counts remain source 1/6, main_api.h 442/525, ETC.h 13/18 (compiles/tasks).
- Code review against CODING_STANDARDS.md and the task spec completed in two independent GPT-6.1 Sol medium review agents. Initial findings (new include shadows and removed directory inputs) resolved in e0f0317b; both follow-up reviews report no remaining findings. Spec reviewer independently ran all 11 integration tests with the checkout mounted read-only: pass. No standards violations or scope creep. Total direct validation: 11 integration + 17 cc + 131 surrounding tests = 159 passing tests.
- Ready for root integration on codex/build-deps-13500. Original checkout untouched, no push/PR, no production game source/header changes, no new matched bytes. Add/remove membership intentionally rebuilds all C for correctness; ordinary edits retain selective invalidation.
