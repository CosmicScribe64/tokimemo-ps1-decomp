---
id: T-14100
title: Resolve DATE near matches with data-flow hypotheses
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12100-codex-cx10]]", "[[matching-notes]]"]
---

## Goal

Test reasoned type and data-flow hypotheses for retained DATE guards, then pivot to new DATE families if bounded attempts fail. Existing four T-12100 matches are baseline, not new gains.

## Acceptance criteria

- [ ] New promotions have full-TU default literal-byte and DATE SHA1 proof.
- [ ] Existing matches preserved; failures recorded with evidence.
- [ ] Standards review before Done.

## Notes

Isolated branch codex/date-14100 from 589bacb8. Build dependency commits 462d25d5 and e0f0317b reused by cherry-pick because the broad ff96bee3 merge conflicts in shared wiki files.

## Results

- New natural dispatch matches: func_8015A288 (160 bytes), func_80154004 (180 bytes), total 340 new bytes. Full translation-unit NON_MATCHING comparison and then promoted default-build comparison both prove literal linked-byte equality. Production `ninja -j1 build/ovl/DATE.ok` passes the DATE SHA1. Logs: build/t14100/dispatch.log, dispatch-separate.log, default-proof.log, date-sha1.log.
- Both use real fixed-size function-pointer table aggregates and a genuine selector local declared before the table, following the already matched dispatch family. No FAKE shaping. 54004 must use two sequential range tests: func_8006B900 may mutate the selector, which is reread before the second range test.
- Bounded 47958 hypothesis: separate block-scoped signed/unsigned fade values isolate narrowing from unrelated cases. Four full-TU probes (s32/u32/u16 in cases 22/38, s32 in all three fade cases) fail, growing/changing the frame and rotating downstream temporaries; 493/493/259/733 output lines respectively versus 14-line baseline. Original source restored exactly. Logs and source snapshots in build/t14100. This does not establish a compiler defect.
- Source review against CODING_STANDARDS.md: C89, fixed types, address-order implementations, genuine aggregates, no generated assembly edits, no game data, no new API conflicts. Awaiting independent integration review before Done.
