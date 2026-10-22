---
id: T-25047
title: Match TACO pointer-walk entry home store
status: Done
assignee: Astra
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24205-cloud-packet-taco-801511c0]]", "[[tickets/T-25043-taco-constant-tail]]"]
---

## Goal

Match `func_801511C0`, the last guarded function in `src/ovl/TACO/80149F90.c`, without shared-header or compiler changes.

## Acceptance criteria

- [x] Literal default C matches164 linked bytes and complete TACO checksum.
- [x] Declaration guards and independent standards/spec review pass.

## Notes

Historical T-19003/T-22025/T-24205 and newer T-25011/T-25016 ledgers read before controls. Baseline preserves exact registers but places arg1 home store after four setup instructions; removing the empty test puts the home store correctly but spends an extra ugen temporary on a parameter-home reload. Do not repeat init/declaration order grids, loop spellings, empty-test forms, return views or engine runs already documented.

## Comments

- 2026-10-22: Parent assigned exclusive same-file continuation after T-25043 match. Claimed function before experiments; private tree remains `t25043-taco`, branch `codex/t25043-taco`.

- 2026-10-22 new controls: full-TU baseline41 words/cost2 (five positional rows); no empty test41/cost12 (six temporary-register rows). A volatile self-store creates44 words/cost17 and is rejected. Taking the parameter address in a late-unreachable call gives no-test41/cost12. An unsigned always-false empty test before setup gives41/cost2, as do a late-unreachable early return and always-true wrapper; a late-unreachable helper call reverts to no-test41/cost12. Thus simple late CFG boundaries switch between the two historical shapes, without combining early home store and original temporary numbering. Read T-24906 and its -S evidence before continuing: the next control is a late-folding identity definition of the parameter, not another ordering grid. Production remains unchanged.


## Match and proof

Default C for func_801511C0 matches all164 literal linked bytes (41 instructions). Docker `ninja -j2 build/ovl/TACO.ok` reports checksum OK; `check_headers.py` and `migrate_globals.py --check` pass; `sync_protos.py --check-branch` reports0 new disagreements/15 known and branch check OK. All changes remain in the one owned source file; no declarations, compiler flags/passes or build rules changed.

Removing the old empty arg1 test restores the original early parameter-home store. Its forwarded/deleted home reload spends one ugen temporary. Initialize the existing loop counter with `i = (u32)arg0 < 0`, a zero that survives uopt, and initialize n by ten alternating additions/ORs with i. The nine intermediate results consume nine more temporary slots, completing the ten-slot ring; as1 removes the identity chain and the last OR gives the original OR-move encoding. Keep the existing same-line initialization layout. Production source marks all controls FAKE/T-25047.

Semantics: the unsigned comparison is false for every s32 arg0. Therefore every +i/|i is +0/|0, preserves arg0, and cannot overflow. The six-iteration loop and all calls are unchanged. No runtime instruction, stack slot, load, store, branch or call is added. This is an artificial allocation control, not a claim about original author intent.

## Remaining negative ledger

Resolved aligned cost is reported independently of instruction count. Private full-TU inputs/output remain under build/t25047; no assembly/game-data dumps are committed.

| Control | Words / cost | Result |
| --- | --- | --- |
| baseline / plain no-test |41/2;41/12|Late store / early store with one extra temporary |
| unsigned late-zero argument definition from arg0 |41/2|Returns to late store |
| unsigned late-zero self definition |41/34|Allocation changes |
| late-unreachable arg1=0 |41/2|Late store remains |
| conditional identity parameter definition |41/36|Extra saved register and changed frame |
| late-zero loop initializer; one, two or nine nested Boolean tests |41/12|No ring slots consumed |
| register-qualified parameter |41/12|No effect |
| direct late-false stores, one/two/nine |41/12|Ugen drops unreachable arm before spending slots |
| register-zero conditional stores, one/two/nine |46/21;49/24;70/41|Assembler retains dead code and branch |
| zero counter shifts in n sum, one/three/nine |42/15;46/19;58/31|Shifts of zero survive; rejected |
| zero counter shifted in conditional scalar update |45/30|Branch and changed allocation remain |
| direct unsigned-zero sum in n |41/12|No extra slots |
| n=arg0+i / n=(arg0+i)+i |41/14;41/14|Second consumes one slot; final add-move encoding differs |
| index pointer plus unsigned/volatile tail view |41/12;42/13|No gain / extra instruction |
| two alternating +i/OR-i operations |41/12|One intermediate advances ring as predicted |
| ten alternating +i/OR-i operations |41/0|Nine intermediates wrap ring; exact words |

## Review

2026-10-22 root independent standards/spec review: no findings. Confirmed unsigned-zero identity for every arg0, no new overflow or effects, original loop unchanged, clear FAKE explanation and source grouping. Inherited unrelated T-24705 comment was not introduced by this change. Ticket remains In Review solely for root full27 integration. Local code-review skill was read earlier; parent owns independent review and instructed no additional worker agents.

Root acceptance2026-10-22: reviewed final identity chain (always-zero i, defined +0/OR0, marked FAKE), no standards/spec findings. Independent default-C literal164bytes and fresh27/27 binary hashes pass with T-25048 integrated. Headers/globals and prototypes0new/15known pass. Moved Done; entire80149F90.c now matched. Worker continues T-25051.
