---
id: T-13500
title: Track compiler header dependencies for incremental builds
status: In Review
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
- [ ] Full 27 binary checks, baseline rebuild counts and standards review recorded.

## Notes

Isolated branch codex/build-deps-13500 at origin/main 5ccb8765. Own configure.py, tools/cc.py, dedicated tests and wiki only. No game-source, compiler-flag or matching-pass changes.

## Comments

- Initial inspection: main objects depend on every header; all C objects depend normally on headers.ok. IDO supports -M but rejects -MD. Its -M output is one target/dependency per line, with unescaped path spaces. asm-processor compiles a temporary transformed source with the input directory added after -Iinclude; dependency discovery must use that same input and search path.

- Validation: 8 new synthetic Ninja integration tests and 17 existing cc tests pass in tokimemo-decomp-arm64. Full updated build passed 27/27 binary checks; next build no-op. Baseline also 27/27.
- Baseline/updated dry-run compiles/tasks: source 1/5 -> 1/6 (header checker newly reruns); main_api.h 443/526 -> 442/525; ETC.h 443/526 -> 13/18. Actual updated source/ETC builds 8.981s/12.990s with checksums OK. All ETC compiles belong to ETC; no main/other overlay link.
- Implementation ready for standards/spec review against 5ccb8765; no production C/header or toolchain/matching-pass changes. See [[build-system]] for the dependency contract and test scope.
