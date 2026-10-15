---
id: T-14100
title: Resolve DATE near matches with data-flow hypotheses
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12100-codex-cx10]]", "[[matching-notes]]"]
---

## Goal

Test reasoned type and data-flow hypotheses for retained DATE guards, then pivot to new DATE families if bounded attempts fail. Existing four T-12100 matches are baseline, not new gains.

## Acceptance criteria

- [x] New promotions have full-TU default literal-byte and DATE SHA1 proof.
- [x] Existing matches preserved; failures recorded with evidence.
- [ ] Standards review before Done.

## Notes

Isolated branch codex/date-14100 from 589bacb8. Build dependency commits 462d25d5 and e0f0317b reused by cherry-pick because the broad ff96bee3 merge conflicts in shared wiki files.

## Results

- New natural dispatch matches: func_8015A288 (160 bytes), func_80154004 (180 bytes), total 340 new bytes. Full translation-unit NON_MATCHING comparison and then promoted default-build comparison both prove literal linked-byte equality. Production `ninja -j1 build/ovl/DATE.ok` passes the DATE SHA1. Logs: build/t14100/dispatch.log, dispatch-separate.log, default-proof.log, date-sha1.log.
- Both use real fixed-size function-pointer table aggregates and a genuine selector local declared before the table, following the already matched dispatch family. No FAKE shaping. 54004 must use two sequential range tests: func_8006B900 may mutate the selector, which is reread before the second range test.
- Bounded 47958 hypothesis: separate block-scoped signed/unsigned fade values isolate narrowing from unrelated cases. Four full-TU probes (s32/u32/u16 in cases 22/38, s32 in all three fade cases) fail, growing/changing the frame and rotating downstream temporaries; 493/493/259/733 output lines respectively versus 14-line baseline. Original source restored exactly. Logs and source snapshots in build/t14100. This does not establish a compiler defect.
- Source review against CODING_STANDARDS.md: C89, fixed types, address-order implementations, genuine aggregates, no generated assembly edits, no game data, no new API conflicts. Awaiting independent integration review before Done.

- Additional natural match: func_80152980, 296 literal linked default-build bytes and DATE SHA1 OK. Bit clearing uses `&= ~0x80`, retaining the original FF7F immediate. Package total 636 new bytes. Evidence: build/t14100/default2-proof.log and date2-sha1.log. Adjacent func_80158338 is saved only in scratch: 16 rows (table offset and month/day register assignment), restored to assembly.

## Larger-body follow-up

- func_80144E20 (3,192 bytes): recovered 21 real copied function-table aggregates from the immutable consecutive table ranges, repaired m2c pointer scaling and canonical GameState fields, and replaced the stale func_8007EAAC name with its confirmed normal_date_move_place name. Type-valid full-TU draft remains distant (631 output lines). Removing byte-offset/goto artifacts and using natural state increment gives 778 output lines; genuine pointer declarations before the tables reduce the table offset error from 12 to 4 bytes but leave the instruction difference distant. These are diagnostics, not matches or coverage. Full candidates, the reusable type-generation script, and logs remain in build/t14100/44e20-*, large*.log. Temporary header context and production draft restored.
- func_801449D4 (1,100 bytes): recovered real 76-entry callback table and canonical existing callback signatures. Explicit local-slot pointers make IDO reload the selector after every switch store (444 output lines). Direct local-array indexing lets alias analysis keep the selector across stores (347 lines; unsigned index 315). Index timing and genuine table-base pointer probes do not close the gap (346/446). Shared GameState pointer probes, correctly scoped to this function, remain 447/429 lines. Two earlier unscoped script attempts fail compilation in other functions and are excluded as invalid evidence. Original matching functions were restored before valid probes and the final file is restored exactly. Candidates/logs are scratch-only in build/t14100/449-*. No new prototypes or distant guards retained.

Final package: 636 newly verified bytes in three functions; existing four T-12100 matches are unchanged. Awaiting independent standards review. The original near-match guards remain unchanged.

Final cleanup verification: all three default-build full-TU literal-byte proofs pass again (160 + 180 + 296 bytes), DATE SHA1 OK, headers OK, branch prototype check OK (zero new disagreements, 15 known baseline). Logs: build/t14100/final-literal.log, final-date.log, final-headers.log, final-protos.log. No source/header changes remain beyond the three verified dispatchers. Independent review remains required for Done.
