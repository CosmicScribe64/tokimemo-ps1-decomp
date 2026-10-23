---
id: T-25036
title: Match MAIN counter through late constant folding
status: Done
assignee: Astra counter worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25026-main-d7ec-near-candidate]]", "[[tickets/T-24001-main-schedule-final-row]]", "[[tickets/T-24204-cloud-packet-main-8006d7ec]]"]
---

## Goal

Close the sole `func_8006D7EC` instruction mismatch in `src/main/8006CB30.c` using a source expression whose zero is unknown to uopt but simplified in a later compiler stage.

## Acceptance criteria

- [x] Literal guarded and default C match, affected executable checksum and declaration checks.
- [x] No inline assembly, instruction rewriting, compiler exception or extra observable operation.
- [ ] Independent review against CODING_STANDARDS.md before Done.

## Notes

Isolated worktree starts at 242c0c3a3. T-25026 measured 1,173 instructions with only position 94 different: target increment versus folded constant one. Prior direct-expression and live/dead-backedge tests are excluded. Investigate the new SHOUGATU late-folded-zero mechanism, then compiler-informed alternatives.

## Comments

- 2026-10-22: claimed the function; read wiki index, playbook, status, standards, local rules and T-25026/T-24001/T-24204 evidence. No source edits yet.
- Donor transfer established a new phase boundary: `(u32)D_800E6280.unk_F5F < 0` retains LES/STR R3 and later LOD R3/ADD1 in optimized ucode. Ugen emits a zero move followed by `zaddu v1,v1,1`; stock as1 initial nodes already contain `li v1,1`. The final 1,173-word body remains one instruction off. This is an as1 expansion-time fold after bypassing the earlier uopt fold. Local unsigned-day and global-address controls give the same final residue; taking the counter address perturbs allocation.
- Resolved closure: after the existing 0xFE store, `if (idx) { idx = idx + ((u32)day < 0); }` supplies an alternate definition that survives uopt but vanishes in as1. The first comparison always returns zero, so the arm is unreachable; the nested unsigned comparison is also zero. Both expressions are defined and have no observable extra effects. Both matching constructs are marked FAKE. Guard removed.
- Verification: whole-TU guarded and default C each match 4,692 literal linked bytes at 0x8006D7EC; independently linked complete MAIN SHA1 is `e823bd844a8f8fa4d05483b59c66bc54b8393b26`. Private links reuse integration objects read-only while replacing only this freshly compiled TU. Headers/globals pass; prototype branch check 0 new / 15 known. Root owns the full 27-unit integration gate.
- Root independent standards/spec review: no findings. Defined zero producer, unreachable identity, FAKE annotations and source-only scope checked. Ticket remains In Review pending root integration verification. Reproducer and negative controls are in `drafts/T-25036/README.md`.

## Root acceptance

Independent standards/spec review passed. Integrated default-C literal4692-byte proof, fresh27/27binary checksums, headers/globals and prototypes0new/15known all pass. No assembly fallback. Accepted and moved to Done.
