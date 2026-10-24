---
id: T-20500
title: Emulator-assisted naming and constants pass (phase 2 head start)
status: In Progress
assignee:
created: 2026-10-16
updated: 2026-10-24
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

- [x] Emulator spike: game boots headless in Docker with the open BIOS; memory watch and breakpoints work
- Moved 2026-10-24: the GameState naming pilot (rename table and enums, 27/27) is now part of
  [[tickets/T-27004-shared-core-names]], which waits for the rename tool (T-27002).
- [ ] Decision on scope for the full pass

## Comments

2026-10-24: Emulator spike done (branch w15-emu, worktree tokimemo-wt/w15-emu). PCSX-Redux 8288e927 plus
upstream fix 3a24bbcc, built from source in `tools/emu/Dockerfile` (arm64 native, interpreter, `-no-ui`), with
OpenBIOS built from nugget d9384092; no Sony BIOS. The game boots under OpenBIOS to the intro movie, the NAME_ENT
attract prologue and the title menu. `tools/emu/run.sh` (wrapper for `tools/emu/emu.py` and the Lua job runner
`tools/emu/emu.lua`) gives `boot`, `break` (main exe and overlay code, overlay guarded by its code bytes; `--width`
for ranges), `watch` (writes or reads), `trace`, `report` and `gdb` (GDB server, tested with gdb-multiarch in the
image), plus `--input`/`--mash` pad presses, `--shot` screenshots, `--state`/`--save-state`, `--until-overlay`,
`--dump`. Reports give arguments, pointed-to memory, a prologue-walk call stack, the resident overlay and
GameState field offsets. Proofs: `disp_string` at the title menu (a1 alternates two buffers 0x14 apart);
NAME_ENT `func_80142E24` (once per frame in the prologue, calls `set_kanji_string` with x=-144, y=0x14/0x24 and a
Shift-JIS string); writer of the mode byte `D_800E738E` (`func_80042960+0x7C` via `main_state_set`). How to use,
results and limits: [[emulator]]. Limits: about 3x real time without breakpoints, much slower with hundreds of
breakpoints; REA's GDB tools need a Linux host GDB (use gdb-multiarch in the image). Containers run with 2 CPUs.
Next: the GameState naming pilot (second criterion).

2026-10-24 (w19-conventions): the GameState naming pilot moved to T-27004. ADR 0001
(`docs/adr/0001-phase-2-naming.md`) lets no rename land before the rename tool (T-27002), and T-27004's criteria
already cover the GameState fields and the first enums. The scope decision (last criterion) is the ADR's; tick
it when T-27001 is Done.
