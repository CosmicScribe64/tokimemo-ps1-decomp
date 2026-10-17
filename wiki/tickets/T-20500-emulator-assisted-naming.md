---
id: T-20500
title: Emulator-assisted naming and constants pass (phase 2 head start)
status: Backlog
assignee:
created: 2026-10-16
updated: 2026-10-16
links: [[speedup-research-2026-10-16]], [[ref-english-patch]], [[game-state]]
---

## Goal

Start phase 2 of the roadmap early: rename symbols and replace magic numbers with named constants, with agents
using a PS1 emulator and debugger to observe what globals, fields and functions do at runtime.

## Why it waits

User decision 2026-10-16: hold off while the wave 3 conveyor (T-16990) keeps the machine near full CPU. Start when
the conveyor is idle or between runs, or on Codex/Gemini capacity.

## Plan notes

- Licensing: no proprietary BIOS. Use an open-source emulator with an open BIOS replacement (for example PCSX-Redux
  with OpenBIOS, MIT), its GDB stub or Lua scripting for breakpoints and memory watches, and the user's own disc
  image (already local, gitignored). Build and run it in Docker per the tooling rule; check first that the game boots
  under the open BIOS.
- First target: GameState (`D_800E6280`, 0x1A90 bytes) field names and enums for girls, parameters, months and
  scenes. These names reach every overlay, and the English-patch notes ([[ref-english-patch]]) already name about 150
  addresses.
- Renames and constants do not change code bytes, but every change still needs a clean 27/27 rebuild.
- Conflicts: a tree-wide rename touches files that open package branches and contributor PRs edit. Apply it in a
  quiet window (no packages running, PRs merged), or extend the merge-time rename table (`config/obin_renames.txt`,
  applied by the merge script) to main and to struct fields.
- Expected effect on matching: small and indirect (agents read named code more easily); runtime observation of
  memory layouts can also confirm struct boundaries and field types, which does help matching.

## Acceptance criteria

- [ ] Emulator spike: game boots headless in Docker with the open BIOS; memory watch and breakpoints work
- [ ] GameState naming pilot with a rename table and enums, 27/27
- [ ] Decision on scope for the full pass

## Comments
