---
id: T-16000
title: Repair thirteen remaining main executable draft failures
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
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

## Comments

2026-10-17: Closed after review. All thirteen MAIN drafts compile and are archived under drafts/T-16000 with the limits documented; wave 3 (T-17005, T-17007, T-17010, T-17012) used them, and func_8007A50C was matched from one. Findings: difference.txt holds 7,675 rows of original instruction words and tools/main_draft_repairs.py has no test and exits 0 on failure; both are in the drafts-archive scrub followup opened from T-14600. Commit 944d3fdd6 lacks a ticket id in its message.
