---
id: T-24601
title: Match glyph raster extraction and mirroring
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]", "[[tickets/T-24501-main-tail]]"]
---

## Goal

Promote the T-24501 `get_fnt` reconstruction after closing and verifying its remaining compiler differences.

## Acceptance criteria

- [x] Default C matches all 1,036 literal linked bytes.
- [x] All 27 executable checksums and declaration guards pass in Docker.
- [x] Coding-standards and scope reviews have no unresolved findings.

## Notes

The existing MAIN worker and coordinator were both idle after interrupted turns when inspected. Their isolated checkout is untouched. This completion session adopts the committed source-only packet; only the verified final body will replace assembly in the main checkout.

Recovered behavior: copy rows of a glyph into caller output; independently apply vertical and fixed eight-byte horizontal mirror operations using a measured 256-byte temporary. Width-first multiplication fixes the second unrolled-row operand order. An integer function without a return preserves the original result-register liveness and exit scheduling. The original source's additional locals are unknown: a marked 20-byte pad above the temporary and four bytes below reproduce the measured 0x1A0 frame and 0x8C temporary offset.

## Comments

- 2026-10-20: Full-TU scratch result is zero resolved cost, 259 instructions. Literal linked proof, production checks and review pending; not yet credited as matched C.
- 2026-10-20: Production `funcdiff.py --resolve --version default main:get_fnt` reports literal MATCH (1,036 bytes). Docker `ninja -j 4` succeeds, including all 27 checksum targets, headers and globals. `sync_protos.py --check-branch`: zero new disagreements, 15 known. Moved to In Review; separate standards and spec reviewers both report no code findings.
- 2026-10-20: Review acceptance checks are complete. Standards: no findings; C89, SDK use, prototype consistency and both FAKE explanations accepted. Spec: no findings; signed raster addressing, both independent mirror flags and the fixed eight-byte permutation preserved. Moved In Review -> Done.
