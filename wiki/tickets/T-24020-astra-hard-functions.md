---
id: T-24020
title: Investigate three largest hard functions
status: In Progress
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24021-dialogue-scheduling]]", "[[tickets/T-24022-tel-frame]]", "[[tickets/T-24023-event-handler]]"]
---

## Goal

Work ahead on dialogue, TEL and EVENT, one hard target at a time, using new evidence-backed hypotheses.

## Acceptance criteria

- [x] Preserve useful source and negative evidence for all investigated targets.
- [x] No wins claimed; unchanged main/TEL/EVENT SHA1 and header/global/prototype guards pass.
- [ ] Standards review before Done; parent handles integration.

## Notes

Exclusive reservations: `src/main/8007C030.c`, `src/ovl/TEL/80132000.c`, `src/ovl/EVENT/8010FE10.c`. Resumed scope adds only `func_8007132C` in `src/main/8006CB30.c` and only `func_80142C9C` in `src/ovl/GEKO/80142760.c`; the parked `func_8006D7EC` is excluded.
Branch `codex/astra-hard-functions`, worktree `/private/tmp/tokimemo-astra-hard`, base `w2-good` at `8052373f747054e2682b37dfbbdbe3be9f88b80a`.
Existing Docker image only, one compiler or `ninja -j1`. Disc copied by APFS clone. No integration/main changes, pushes or worktree deletion.

## Comments

- Read current index, standards, local setup, build gaps and dialogue negative evidence before probes.

## Checkpoint

All three bounded investigations are complete and await parent review. No C bytes added. Dialogue and TEL retain their previous baselines; EVENT has a source-only reconstruction at aligned cost 214 (baseline 1618), 826 printed rows, 1712/1712 instructions. The corrected source and independent measurement are in `drafts/T-24023/`.

Production `src/`, `include/`, `config/` and `tools/` are unchanged from base. Docker checks: main, TEL and EVENT SHA1; headers; globals; prototype branch check (0 new, 15 known). Scope and whitespace checks pass. No push, merge, compiler patch, host dependency install or extra agent. Reservations remain recorded for parent coordination; no worktree deletion.
