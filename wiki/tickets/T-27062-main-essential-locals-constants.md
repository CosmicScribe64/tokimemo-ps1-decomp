---
id: T-27062
title: Prepare essential main locals and proven constants
status: In Progress
assignee: parallel-main-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27050-parallel-main-reconciliation]]", "[[tickets/T-27059-all-main-c-file-names]]"]
---

## Goal

Revalidate the frozen 247 symbol rows after the main filename moves, then prepare separate source-backed packets for essential game-logic locals/parameters and meaningful constants in m-menutext, m-sysmenu, m-hud and m-holiday. Root owns application, builds, commits and shared wiki/board/log updates.

## Acceptance criteria

- [ ] All four frozen symbol plans pass against logical filenames; concrete conflicts are recorded separately.
- [ ] Essential local/parameter rows use supported function-scoped syntax and pass plans.
- [ ] Proven constant sites have exact context, domain, value, signedness and existing-owner reuse; unsupported tooling sites are explicit.
- [ ] Independent Sol review checks meanings, comparisons, writes and collisions before handoff.
- [ ] No source/header/symbol/ledger edits, application, build, clean or commit in this lane.

## Notes

The earlier 247-row symbol tables and seven reviewed filename rows remain frozen. New supplementary packets and additions to the four owned specs are separate work. Current logical source names and applied ledgers win.

## Comments

2026-10-24: Root requested this follow-up while integrating the 32-file main batch. T-27062 is created before preparation; root registers its board card. All analysis/tool dependencies stay in the existing Docker image with --cpus=2.
