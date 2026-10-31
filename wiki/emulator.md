---
type: tool
updated: 2026-10-29
sources: ["tools/emu/Dockerfile", "tools/emu/run.sh", "tools/emu/emu.py", "tools/emu/emu.lua"]
---

# Emulator runtime probe for naming (T-20500)

Use the headless emulator to inspect a function or global at runtime. It can stop at a main-executable or overlay address; read registers, arguments, pointed-to memory and the call stack; record address reads or writes; press pad buttons on specified frames; capture screenshots; and save or load states. Ticket: [[tickets/T-20500-emulator-assisted-naming]].

## Setup

- `tools/emu/Dockerfile` builds PCSX-Redux from commit `8288e927` (2026-09-25) plus upstream fix `3a24bbcc`, which fixes an SPU crash when loading a state into a new process. It also builds OpenBIOS from nugget `d9384092` under the MIT license. No Sony BIOS is used or needed.
- The emulator runs natively on arm64 (Apple Silicon), with the interpreter CPU, no window (`-no-ui`), and a dummy audio driver. The `tokimemo-emu` image is 1.7 GB. Its first build takes about 40 minutes at `-j2` and leaves about 8 GB of build cache. `docker builder prune` frees that cache, but later patches then require a full rebuild.
- Supply your disc through `disc/*.cue`. Symbols and call stacks also need the extracted `disc/files/SLPM_86.053` and `disc/files/CDROM/EXEDIR/*.EXN` ([[disc-layout]]). A normal build produces linker maps at `build/SLPM_86.053.map` and `build/ovl/*.map`, which give exact function names. Without maps, names marked `?` are guessed from the previous `jr $ra`.
- `tools/emu/run.sh <subcommand>` builds the image if `tools/emu/Dockerfile` changed, then runs `tools/emu/emu.py` inside it. Containers get 2 CPUs (`EMU_CPUS`), and the emulator runs under `nice`. Results, screenshots, save states and memory cards go to gitignored `build/emu/`. Save states and screenshots are game data. Never commit them.

## Usage

Frames are vsyncs (60 per second), counted from the start of a run; with `--state` the state is loaded on frame 1. Every run prints a short report and writes `build/emu/<run>/result.json`.

```sh
# states to start from (once: about 75 s, 85 s and 10 s)
tools/emu/run.sh boot --until-overlay NAME_ENT --save-state build/emu/states/name_ent_loaded.sstate
tools/emu/run.sh boot --frames 4500 --save-state build/emu/states/prologue.sstate
tools/emu/run.sh boot --state build/emu/states/prologue.sstate --frames 460 --mash START:30:120:390 \
    --save-state build/emu/states/title_menu.sstate          # title menu (game start / option)

# what does func_X do: arguments, pointed-to memory, call stack (-v: all registers, all hits)
tools/emu/run.sh break disp_string --hits 2 --state build/emu/states/title_menu.sstate
tools/emu/run.sh break func_80142E24@NAME_ENT --hits 3 --state build/emu/states/name_ent_loaded.sstate
tools/emu/run.sh break 0x80132000@NAME_ENT --width 0x30000 --state ...   # any NAME_ENT code run
# who writes (or --read reads) a global or a GameState field
tools/emu/run.sh watch 0x800E738E:1 --hits 8 --frames 3700
# which functions run in a scene
tools/emu/run.sh trace --all-named --state ...                 # every named main-exe function
tools/emu/run.sh trace --all-named --overlay NAME_ENT --state ...
# pad input and screenshots (buttons: START SELECT UP DOWN LEFT RIGHT CROSS CIRCLE SQUARE TRIANGLE L1 L2 R1 R2)
tools/emu/run.sh boot --state ... --input 100:DOWN,160:CROSS/10 --shot 120,200 --frames 300
tools/emu/run.sh report build/emu/<run>/result.json            # print a report again
```

- Targets: `0xADDR`, `func_XXXXXXXX` or any name from the symbol files or the maps. Overlay code needs its overlay (`TARGET@NAME` or `--overlay NAME`): every overlay shares one slot ([[overlays]]), so a hit counts only when the 16 bytes at the address are that overlay's code.
- `--dump HEXADDR[:LEN]` saves memory at each hit (with `boot`: at the end); `--ram-dump` saves the 2 MB RAM; `--save-state` writes a state when the run stops; `--no-stop` keeps running to `--frames` after the recorded hits.
- `--poke FRAME:HEXADDR:HEXBYTES` writes main RAM at the start of that vsync (frame 2 or later, after `--state`); `result.json` lists each write under `pokes` with the bytes it replaced. A run with pokes starts from a constructed state: say so wherever its output is used as a golden (e.g. `.asset-cache/emu/school-events-golden-v2`).
- The report names data addresses as symbol plus offset, and addresses inside GameState as fields: `D_800E7388 = GameState+0x1108` ([[game-state]]).
- Each run also logs which overlay landed in the slot on which frame and every change of the loader's mode byte `D_800E738E`.

GDB: `tools/emu/run.sh gdb --state ...` starts the emulator with the PCSX-Redux GDB server on `--port` (3333, published on localhost). `gdb-multiarch` is in the image; tested in one container: `python3 tools/emu/emu.py gdb --state S &`, then `gdb-multiarch -batch -ex 'set architecture mips:3000' -ex 'target remote localhost:3333' -ex 'break *0x8004E788' -ex continue -ex 'info registers' -ex 'x/s $a3'` (breakpoint, registers and memory work; `detach` is not supported). The REA GDB tools need a Linux host GDB, so they do not work from this macOS host.

### Function captures (`tools/emu/func_capture`)

`tools/emu/func_capture/run.sh` captures function hits for the engine's differential harness; see `engine-porting-contract`. Two environment options:
- `MEMCARD_DIR=<dir>` (relative to the data root, or absolute) copies `memcard1.mcd` and `memcard2.mcd` from that directory into the new capture's output directory, so the emulated cards start with that content. The source files are never touched; without the option the cards start blank.
- `CONTAINER_PREFIX=<p>` names the container `<p>-<name>` (also in `tools/port/run_engine_diff.sh`), so parallel workers can tell their containers apart.

## Spike results (2026-10-24)

- The game boots under OpenBIOS: Konami logo and intro movie (MDEC/STR works), NAME_ENT loads at frame 3538 (mode byte 0x11), the attract prologue runs, START reaches the title menu. Speed without breakpoints is about 3x real time (900 frames from a state in 18 s, 3600 frames from power-on in 70 s).
- `disp_string` (O.BIN name) runs once per frame on the title menu, called from `func_80043510+0x11C`; its `a1` alternates between 0x800E8C70 and 0x800E8C84 (two buffers 0x14 bytes apart, one per frame), `a2`=0x7F, `a3`=5.
- On the title menu no code runs in the overlay slot (400 frames, range breakpoint), so the title menu is main-exe code; NAME_ENT code runs in the attract prologue.
- NAME_ENT `func_80142E24` runs once per frame on frames 7 to 249 after the NAME_ENT load state (243 calls, via the entry `func_80144AD0` from the main loop `func_80042540+0x220`), then stops; on its last frame it calls `set_kanji_string` (O.BIN name) with `a0`=-144, `a1`=0x14 then 0x24 (0x10 apart, probably one text line), `a2`=0 and `a3` a Shift-JIS string in NAME_ENT rodata (`D_8014C6F4+0x24`).
- The mode byte `D_800E738E` (GameState+0x110E) is zeroed twice during boot by BIOS code and set to 0x11 at frame 3533 by `func_80042960+0x7C`, called from `main_state_set+0x4C`.

## Limits

### Current differential capture work (T-39008)

Screenshot records now include CPU cycles, PC, loader mode and the requested `--dump` ranges sampled in the same Lua callback as the image. They are taken at vsync after input overrides are applied, before the next CPU tick: an input issued on that vsync has not yet changed the rendered game. A breakpoint hit remains a different observation; its final screenshot can be a later frame. Do not attach earlier breakpoint state to that screenshot.

For NAME entry, capture `--dump 800E7374:32 --dump 8014CC70:96 --dump 800B3F60:12 --dump 80120650:2176`. Source checkpoints are game mode0x14, stage/step pairs(1,1),(1,3),(1,5),(1,7),(1,9),(2,1),(2,3); mode/stage/step/substep bytes are0x800E7388/89/8A/8D. Text count/position and actor animation state matter when comparing a stable screen. Memory sampled at vsync still needs correlation with the displayed draw buffer; it is not automatically the state that generated every pixel.

The pinned image has been rebuilt and the original game reached NAME entry and the first school scene through ordinary controller input. The run-local compatibility CUE removes only unsupported `FLAGS DCP`, preserves track/index records and records source/track hashes; originals remain unchanged. Select an explicit `--iso disc/disc.cue` when multiple valid CUE files exist.

For bounded continuous observations, add `--sample-every 1` and every range listed in `config/port/name-entry-memory-projection.json` under `required_dumps`. Sampling requires a finite non-GDB run and is limited to4096 records and a64MiB estimated payload. The output `memory-samples.jsonl` has a SHA-256 recorded in the result. Use fewer frames or a larger interval for longer observations; do not silently drop divergent samples.

Decode the raw samples inside the existing Docker image:

```sh
docker run --rm -v "$PWD:/work" -w /work tokimemo-recovery \
  python3 tools/port/native_memory_projection.py \
  --map config/port/name-entry-memory-projection.json \
  --samples build/emu/review-restore-lifecycle-v1/memory-samples.jsonl \
  --out build/emu/review-restore-lifecycle-v1/projected-new.jsonl
```

The decoder validates all rows before creating a new output, rejects missing/overlapping/truncated dumps and preserves timestamps. Its field map documents source addresses; it does not independently authenticate the loaded overlay, resolve GPU draw order, align clocks or declare parity. Preserve the original run provenance alongside projections. `SystemData.seed` is the main-loop timer seed, not the BIOS RNG state.

Save-state restoration occurs after the initial callback. Samples explicitly identify `before_restore`, `restored` or `cold_boot`; `result.lifecycle` records the restoration's before/after CPU cycles. Independently verified run `review-restore-lifecycle-v1` retains its pre-restore frame1 and restored frames2..4 rather than treating all four as gameplay. Its frame1 image has zero dimensions and is unusable, despite a listed PNG path; reject nonpositive image dimensions. Restored images are320×240. Actual scanout correlation and ordered port/native event comparison remain required. See `tickets/T-39008-synchronized-differential-captures`.

The current runner rejects that unusable-image case before PNG conversion: dimensions must be positive within1024×512, format16/24bpp and payload length exact. Each new shot records `image_status`, plus `png`/`png_sha256` only when conversion succeeds. Failed image conversion preserves state and raw bytes. Independent actual restore `review-shot-validity-v1` verifies unavailable pre-restore and valid restored shots. Older result files are not rewritten or retroactively certified.

The source loop in `src/main/main_loop.c` selects draw storage, dispatches the game mode, increments `game_state.frame_count`, calls `draw_frame(1)`, then pumps CD and audio commands. `draw_frame` in `src/main/frame_render_audio.c` builds actors/quads/text/background commands, performs sync/input work, invokes `func_8009AC20` and submits the ordering table via `func_8009AD00`. The original `asm/libgs.s` implementation of `func_8009AC20` toggles its buffer index. Consequently, a VSync RAM snapshot and a screenshot taken in one callback are simultaneous observations, but not proof that the observed mutable state generated that scanout. The next synchronization step must bind mode-dispatch/draw-submission observations and buffer identity to the displayed frame; scene IDs or nearest-frame image similarity are insufficient. Keep CD/audio service completion and timer interrupts separate from logic iterations.

`tools/port/README.capture-pair.md` documents the exact single-pair image/state diagnostic. Root and reviewer reproduced a failing first-school pair at320×240:56273 differingRGBA pixels, title timer184 versus116, incompatible text-count domains and unverified synchronization. The comparator never normalizes dimensions or searches for a better-matching frame. This failure is retained while rendering and clock ownership are repaired.

- Each breakpoint is checked on every instruction: 363 breakpoints slowed a 4000-frame run to 9.5 minutes. Trace a few dozen functions, or start from a state close to the scene.
- The call stack walks each function's prologue (`addiu sp,-N`, `sw ra,X(sp)`) over the static code. It stops in BIOS or kernel code (the report then prints `ra`) and at functions that do not save `ra`.
- Memory reads cover RAM, the scratchpad and the BIOS ROM; VRAM, SPU RAM and the GTE are not exposed by the job runner (PCSX-Redux has Lua and web APIs for them).
- BIOS calls run OpenBIOS code, not Sony's: watch hits inside BIOS functions (memset, CD) show OpenBIOS addresses. Timing can differ from the real BIOS.
- Save states belong to this emulator build; after a PCSX-Redux bump, old states may not load.
- EVENT and GYOZI load addresses are inferred ([[overlays]]); their breakpoints still check the code bytes.
