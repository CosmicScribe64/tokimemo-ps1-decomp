---
id: T-39602
title: Remove outdated review-build wording
status: Done
assignee: Codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Update the repository description and public patcher to describe the published English patch without obsolete review-build or work-in-progress wording. Preserve historical review records and the verified patch payload.

## Verification

README already describes the completed verification with its scope limits. Update the page introduction, manifest label and packager default together. No patching behavior or game data changes are needed.

The repository description now links to the English patch without calling it a review build. The deployed page and release manifest match the updated files byte for byte. Only index.html and the manifest label changed on gh-pages (3f7b3b4); the patch payload and all input/output hashes are unchanged. The source packager now emits the same label. Reviewed the two-line source change against CODING_STANDARDS.md; no unresolved findings. No rebuild or Docker restart was needed.
