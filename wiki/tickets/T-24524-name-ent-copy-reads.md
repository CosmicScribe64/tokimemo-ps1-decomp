---
id: T-24524
title: Investigate NAME_ENT copy-loop source read timing
status: Done
assignee: Codex Sol 6.1 medium
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24515-name-ent]]", "[[tickets/T-24132-name-ent-final-4]]"]
---

## Goal

Bounded full-TU source hypothesis for NAME_ENT func_80138C1C in src/ovl/NAME_ENT/80132000.c. Preserve T-24515's corrected counters/endpoints and GsWord date widths. Test whether diagnostic volatile source-byte read views prevent load hoisting into the three original nop slots; do not repeat exhausted declaration/register grids.

## Acceptance criteria

- [x] Literal default C and linked-byte proof plus all27 hashes and guards for any match; otherwise concise replayable blocker.
- [x] Standards review, FAKE explanations and synchronized ticket/board/log.

## Notes

Root assigned this file after T-24502's source-only TACO checkpoint. Docker-only tools, existing image, full translation unit, no shared-header changes or inline asm. Inherited T-24515 diagnostic volatile space snapshots remain explicitly FAKE.

## Comments

- 2026-10-20: read T-24515, T-24132 and lane histories; claimed before editing. Fresh hypothesis concerns input reads inside copy loops, not the already-tested space snapshots.

## Result

Negative. Full-TU control and all three diagnostic views reproduce181 words/104 rows against185 target: volatile first-character read only, volatile second-byte reads only, and volatile entire input pointer. The load-hoisting sites and all other differences are unchanged. No production/header/tool changes, new match, literal proof or coverage credit.

Replay inside Docker with main repository mounted read-only at /data and this worktree at /work: add /data/drafts/T-24515/replay to sys.path, import probe.probe, read /data/drafts/T-24515/func_80138C1C.c and call probe with unchanged source, c = p[0] replaced by c = *(volatile u8 *)p, all p[1] reads replaced by ((volatile u8 *)p)[1], or pointer declaration replaced by volatile u8 *p. Existing helper selects only this function as C in its full TU. Private outputs are build/T-24515/func_80138C1C/t24524-{control,first-read,second-read,all-input}.

Source-read volatility does not explain the three original nop slots; no qualifier/scheduling grid justified. Existing T-24515 source remains the best reconstruction. Self-review: docs-only delta, no generated data or raw instruction listings. Await root review against CODING_STANDARDS.md before Done.

- 2026-10-20 closing review: root checked bounded full-TU negative/replay and CODING_STANDARDS.md, no findings. Investigation acceptance met by the concise blocker; no production change or coverage claim. Moved In Review to Done.
