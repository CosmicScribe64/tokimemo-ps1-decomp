---
id: T-24525
title: Test NAME_ENT indexed post-test record loop
status: Done
assignee: Codex Sol 6.1 medium
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24515-name-ent]]"]
---

## Goal

Test one fresh structural hypothesis on NAME_ENT func_801398EC in src/ovl/NAME_ENT/80139740.c: direct-global indexed do/while instead of the successful T-24515 indexed for loop. Historical post-test control used Rec24 pointer aliasing and did not test this combination. Root assigned this file; no shared headers.

## Acceptance criteria

- [x] Full-TU control plus one indexed post-test variant; literal proof/all27 hashes and guards if matched, otherwise concise replayable blocker.
- [x] Standards review and synchronized ticket/board/log.

## Notes

Existing Docker image and compiler only, no additional declaration/register/loop grid if unchanged. Production remains original unless proven.

## Result

Negative. Full-TU indexed for control and indexed post-test variant both reproduce126 words/173 rows against123 target; decoded comparison files are identical. The changed loop preheader/test does not alter current allocation or arithmetic ordering. No further loop grid, production/header changes, literal proof or coverage claim.

Replay inside Docker with main mounted read-only at /data: import probe from /data/drafts/T-24515/replay, read /data/drafts/T-24515/func_801398EC.c and call probe for the control. For the sole variant, replace for(i=0;i<13;i++) by i=0; do and replace its closing brace with i++; } while(i!=13). Preserve direct-global indexed stores, selector and expression order. Private full-TU outputs: build/T-24515/func_801398EC/t24525-{control,indexed-posttest}/.

Self-review: docs-only delta; exact bounded negative and replay preserved, no generated data or raw listings. Root standards review pending; production source/header/config/tools unchanged.

- 2026-10-20 closing review: root standards review passes, no findings. Investigation acceptance met by the concise full-TU blocker; moved In Review to Done, no new match or coverage claim.
