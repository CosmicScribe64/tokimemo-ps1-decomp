---
id: T-16000
title: Repair thirteen remaining main executable draft failures
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[exhaustive-drafts]]"]
---

## Goal
Produce full-translation-unit compiling C alternatives for the thirteen main-executable failures after bounded census repairs. Preserve original attempts and archive every outcome, source/context and provenance. No stub bodies or assembly fallback substitutes; nonmatching drafts add no coverage.

## Acceptance criteria
- All thirteen alternatives compile using the existing Docker toolchain.
- Complete context, declarations, diagnostics, hashes and resolved comparison are retained.
- Results are published on main and all residual reconstruction limitations documented before stopping.

## Result
All thirteen full translation units compile and have saved resolved comparison scores. Source-only alternatives and replay tooling are preserved under drafts/T-16000; production source and verified coverage are unchanged. Reconstruction correctness remains separate from compilation; see [[stopping-point-2026-10-15]].
