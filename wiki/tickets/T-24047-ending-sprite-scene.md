---
id: T-24047
title: Recover ENDING sprite scene initializer
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[ending-family-results]]", "[[completion-run-2026-10-17]]"]
---

## Goal

Recover func_80135F4C in src/ovl/ENDING/80134D20.c as byte-identical default C.
Own only that TU. Do not edit the parked 8013A2E8 function or other assigned files.

## Acceptance criteria

- Actual unguarded default C, literal linked proof, affected binary SHA1.
- Bounded novel experiments based on recovered types/control flow.
- Review against CODING_STANDARDS.md; justified FAKE forms marked.
- Useful negative evidence and drafts retained without coverage claims.

## Notes

Pin 4496b0dc785dd6d109a39c4f3408e70d17079232, branch codex/complete-ending-large,
worktree /private/tmp/tokimemo-ending-large. Docker only, one compiler total.
Header changes require coordination. Earlier census has 795 residual rows and
known menu-byte GameState pointer issues; read T-18305 and T-20930 before probes.

## Comments

2026-10-18: Read current wiki/index, completion run, coding standards and ENDING
family findings. Original checkout remains read-only. Old MAIN/EVENT/TEL ownership revoked.

2026-10-18: Archived full-TU replay has208-byte frame (target160) and broad residual. Fresh hand source reuses one signed-halfword index, typed Rec24/DisplayActor44 fields, direct menu-byte array accesses and direct random-table assignment. It restores805/805 instructions with176-byte frame. New evidence motivates a shared size128 local across transfer calls and variable grid divisors; type/lifetime probes follow.

2026-10-18: Bitfield b14 (rather than rawword&0x4000) restores original sign-bit branch and switch register cycle; aligned full-TU cost340 to114. Direct record indexing removes two artificial pointer locals. With scalar i/size before stack tables, original160-byte frame and both table homes0x7C/0x60 match exactly; cost104,803/805 instructions. Divisor locals were unnecessary: literals are naturally hoisted, so removed. Register qualifiers and coordinate-line grouping do not improve output. No production header/toolchain changes.

2026-10-18: Unsigned16 width lvalue view with u16 shared size removes duplicate128 register; packed i/size/row homes recover exact frame/table positions. Current803/805 instructions, aligned cost64. Row quotient materialized immediately after the third-loop byte clear restores the ten-store block. Local byte-view, register hints and whole first-loop grouping have no effect. Native AS1 diagnostic attempt was rejected: -xbbdbg,8 is not accepted by existing frame shim argument parser; -R-only diagnostic object lacks .data and fails trailing-pad validation. No diagnostic output interpreted, no toolchain changes retained.

2026-10-18 correction to diagnostic note: the -R-only compile itself succeeded; the fidelity script failed when it tried to look up an absent optional .data section. Both stock and traced objects legitimately omit .data. A scratch-only wrapper now appends native -R/-xbbdbg8 after the unchanged frame transformation, and verifies .text/.rel.text/.data/.rodata inventories and bytes equal stock. Native stdout capture yields26784 DAG lines. First-loop DAG has no dependency from menu sb to sprite stores, explaining its early issue. No production compiler flags or scripts changed. Moving actor Y assignment before fade byte restores entire second loop, cost64 to49,804/805 instructions.

2026-10-18: First-loop menu-address roundtrip `(u8 *)(u32)&member` restores exact store ordering without disturbing frame/base registers when restricted to that loop. This is an allocation/alias-metadata FAKE candidate, not recovered source. Current804/805 instructions, aligned cost43. Most residual rows are branch offsets cascading from two tail LUIs speculated across the switch. Third-loop128 and499 occupy exchanged a3/t0 colours. Later byte-view reads/stores, explicit common-tail label, implicit-int diagnostic and unsigned page view do not improve. Useful preferred full-TU and normalized diff retained under build/t24047/roundtrip_first_menu*.

## Checkpoint

Preferred guarded C is preserved in src/ovl/ENDING/80134D20.c and
`drafts/T-24047/body.c`, with a full-TU replay script and bounded probe ledger.
It remains804/805 instructions, aligned cost43; original160-byte frame and
both stack table homes agree. No new verified C bytes.

Remaining causes are isolated: four words exchange128/499 between a3/t0;
two default-arm script-address LUIs move into the switch dispatch, accounting
for the other differences and branch-offset cascade. Earlier references to
"tail LUIs" mean the default arm, not the common continuation. Adding explicit
case12 alongside default blocks this speculation, yielding805 instructions and
five differing words, but changes the upper bound12 to13 and adds a table entry.
That diagnostic is explicitly rejected as a matching candidate.

Halfword quotient/page locals add narrowing operations and do not close the
constant tie. Source default-first ordering worsens output; moving default body
outside the switch leaves the same residual. Probes have been bounded to source
recovery and the measured residuals. No shared headers, compiler options, tooling
rules, other TUs or parked ENDING8013A2E8 changed. The two alias/width devices are
FAKE-marked.

## Review and validation

Independent standards review found no actionable findings: the ticketed guard,
C89 source, shared types, explicit FAKE notes, replay failure handling and
negative-evidence labels comply with CODING_STANDARDS.md. Parent scope review
confirmed only the owned function changes in production; no headers, compiler
rules, toolchain flags or other TUs changed. The matching acceptance criterion
is not met, so this ticket remains In Review with zero new verified bytes.

Docker replay reproduces804/805 instructions and aligned cost43; the rejected
case12 diagnostic reproduces805/805 and cost10. Normal ENDING rebuild passes
its SHA1. Header/global checks and branch prototype check pass (zero new
disagreements,15 known). All27 built binary hashes pass after rebuilding stale
copied GEKO/GYOZI cache outputs. Those rebuilds required no source changes.
Ignored build/t24047/ retains verification logs; git diff --check is clean.
