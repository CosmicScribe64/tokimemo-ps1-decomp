---
id: T-25044
title: Match parameter_up_down
status: In Review
assignee: codex-t25044
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25004-class-c-main-8005b43c-parameter-up-down-col2sepia]]", "[[tickets/T-25008-closers-main-8005b43c-8006ba40-parameter-up-down]]", "[[tickets/T-25028-read-only-candidate-triage]]", src/main/800737A0.c]
---

## Goal

Match MAIN parameter_up_down in its original translation unit, using source-only changes and existing build rules.

## Acceptance criteria

- [x] Literal default-C function bytes match.
- [x] MAIN checksum and declaration checks pass.
- [ ] Review against CODING_STANDARDS.md resolved; root integrates independently.

## Notes

Isolated branch codex/t25044-parameter, /private/tmp/tokimemo-t25044 from main 96d9f9ae6. T-25004/T-25008/T-25028 and current playbook read before controls. Historical residuals: nibble expression forwarding into a temporary and row-pointer spill due to reused index lifetime. No shared-header or compiler changes; no broad old grids.

## Comments

2026-10-22: Claimed exclusive MAIN src/main/800737A0.c function ownership. Managed worktree outside writable roots replaced by isolated /private/tmp tree; root checkout unchanged.

2026-10-22 baseline: Docker full-TU trace reproduces 309 versus 307 target instructions. The mode expression is a separate unsigned AND range, converted into signed v; both are colored, causing the t6 producer and two copies. The club row address is a colored expression range dependent on v, then spills to 0x58 rather than row home 0x4C. Moving the signed cast before the byte AND does not change output; unsigned v preserves the head defect and reverses four multu operands. These controls are rejected; no match claimed.

2026-10-22 new causal result: using one empty test of the raw mode byte before assigning the nibble, and testing v directly, removes both head copies and matches every instruction through the club address sum. It preserves the target byte v1/nibble v0 allocation. Remaining differences begin exactly at row-expression promotion (v1 versus t9), whose missing temporary then shifts downstream ring numbers. Distinct index with parameter reused as walking pointer restores the row home but changes selector/pointer precoloring; rejected. Testing a direct volatile row-home chained store next.

2026-10-22 stack isolation: distinct scalar index plus raw-byte lifetime produces a 307-word object with only four row-home differences (0x48 rather than 0x4C). A further scalar or four-byte array fixes all homes but grows the frame by eight bytes (two remaining instructions). Reordering the row declaration does not move this home. One-element index storage introduces a redundant store; pointer-array and volatile-pointer storage change expression promotion and frame sizing. A shared index/pointer union also spills the index and changes its register; rejected. Next control removes the element local by reading the signed table elements directly, while retaining a separate index lifetime.

2026-10-22 verified match: replacing the signed element scalar with direct signed table-element reads lets h retain the index lifetime without adding a local. All 307 words / 1228 literal linked bytes match. Removed the NON_MATCHING guard and fallback, restored the original declaration order, and updated the lone FAKE annotation. Default-C funcdiff MATCH; ninja -j2 build/SLPM_86.053.ok passes SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26. check_headers and migrate_globals --check pass; sync_protos --check-branch reports zero new / 15 known disagreements.

Review: coordinator examined signed s8 promotions, cast-before-shift equivalence, and absence of intervening calls between repeated element reads; no semantic issue found. Its stale-comment finding is resolved by the T-25044 raw-byte lifetime comment. C89, canonical declarations, original symbol, six real locals, no padding/volatile/union, no tooling changes, no game data. In Review pending coordinator independent full27 integration.

2026-10-22 root independent standards/spec review: PASSED, no findings. Direct s8 loads retain promotions with no intervening effects; h's unsigned halfword shift is equivalent; raw-byte empty test is marked FAKE; existing return ABI retained. In Review for root full27 gate.

Source-only pin: 62429d6d9. Preserved replay validates current0, four-row8, padded4, no-byte-use245 diagnostic diff lines (a substitution produces two lines). Wiki lint: zero broken links/status mismatches; two existing large-page warnings. Private compilation traces and generated game data are excluded.
