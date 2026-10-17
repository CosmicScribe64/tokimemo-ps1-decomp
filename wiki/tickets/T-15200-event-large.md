---
id: T-15200
title: Reconstruct large EVENT dispatcher and reusable types
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-11600-w1-ovl-event-8010fe10]]", "[[tickets/T-14300-event-families]]"]
---

## Goal

Recover func_801103A0 (6,848 code bytes) and transferable EVENT record/state types. Preserve all existing matching C. Worktree codex/event-large-15200 starts at c5859bf5.

## Acceptance criteria

- Promote only default full-TU literal linked matches with EVENT SHA1 and header/global/prototype guards.
- Save independently measured reconstructions separately from immutable archived drafts.
- Record reusable type/control-flow findings even when the large function remains unmatched.
- Standards review before Done.

## Notes

Docker image tokimemo-decomp-arm64, serial compiler. Immutable disc inputs hardlinked from original; generated assembly private. Original checkout read-only. Prior T-11600 and T-14300 consulted before experiments; baseline T-14500 score3174 is not a near match.

## Comments

2026-10-15: Claimed isolated EVENT lane. No production changes yet.

2026-10-15 first measurements: independent large-dispatcher reconstruction uses existing EventStateView/Rec34, unk_0E4.f.b1 (target sll 30/sign branch), and short-circuit assignment-expression OR arms that retain the observed writes to D_800B0895. Full-TU resolved diagnostic rows: typed Boolean correction 2,856; consistent state pointer 2,219; target mask assignment order with unassigned helper return preserved 2,210; removal of jumps to immediate enclosing-block fallthrough 1,930. These are nonmatching drafts, not verified byte gains. Prior T-12801 return0/phantom-argument/Rec34 findings reused, not claimed new.

Related func_80118010: existing Rec34 halfwords +0x02/+0x06/+0x0A replace raw casts; s16 load required at D_80124358, despite its u8 declaration. Reuse of the real result byte removes artificial locals and reproduces target frame 0x38 / sp+0x32. Removing phantom func_8004C250(0x34) argument and using consistent legacy-state fields reduces diff to 26 rows (13 instruction positions), all stride/pointer register choices. Full-TU serial Docker probes in build/t15200; production C/header unchanged.

2026-10-15 retained checkpoint: dispatcher remains separate in drafts/T-15200/func_801103A0 (1,930 diff rows, same instruction count as target; no promotion). Related func_80118010 retained as a ticketed NON_MATCHING guard (26 rows / 13 register-only instruction positions); default remains original assembly. New EventStateView fields unk_6D5[12] and unk_6E5 model the observed selector-indexed bytes at D_800B0F35 and flag D_800B0F45, preserving every following offset. Twelve slots follow the established Rec34 extent; field meanings remain unknown.

Traced uopt confirms the handler's residual: stride 0x34 chooses v1 instead of a0; three computed record addresses choose v0 instead of v1. Explicit stride local, byte/typed record pointers, integer-base cast, two-result lifetime and GsWord halfword-pair probes do not close it. The existing sibling's 0x34 call argument closes the stride register only (18 rows remain) but is not retained because its ABI is unknown. No new FAKE. Rejected probes remain in build/t15200.

2026-10-15 usage-limited handoff: parent requested immediate committed draft checkpoint, no further exploration. Supersedes guarded-source retention above: func_80118010 C is saved separately under drafts/T-15200 only; production src/ovl/EVENT/8010FE10.c restored exactly. All 29 measured alternative bodies/declarations, useful compiler contexts, replay scripts and allocator reports saved. EVENT header fields retained as reusable type evidence. EVENT SHA1 passed after rebuilding every EVENT consumer. Default code coverage gain: zero. Parent integrates and pushes.

2026-10-15 final verification: restored production C passes host `git diff --quiet c5859bf5 -- src` (Docker cannot resolve this worktree's external .git metadata). Docker `ninja -j1 build/ovl/EVENT.ok build/headers.ok build/globals.ok` succeeds; explicit `funcdiff --version default --resolve EVENT:func_801103A0 EVENT:func_80118010` proves 6,848 and 680 literal linked bytes of the unchanged ASM fallbacks, not new C matches. Prototype branch check: zero new disagreements, 15 known, OK. Preferred saved contexts independently rescore to 1,930 and 26 rows. Machine-readable hashes/check results in drafts/T-15200/verification.json; literal-proof transcript default-proof.txt. Review remains pending; no Done claim.

2026-10-17: Code review against CODING_STANDARDS.md done; the only src change is the EventStateView split in include/ovl/EVENT.h, with correct offsets and sizes, and the drafts hold C only. No bytes were gained by design; T-17400 later redrafted both functions with the same types and scores. The func_80118010 near miss (26 rows) is moved to a follow-up. Moved to Done.

Followup opened: [[tickets/T-21018-event-80118010-near-miss|T-21018]].
