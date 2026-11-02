---
type: tool
updated: 2026-11-01
sources: ["tools/emu/Dockerfile", "tools/emu/run.sh", "tools/emu/emu.py", "tools/emu/emu.lua", "tools/emu/route.py", "tools/emu/route.lua", "tools/emu/routes/main.route"]
---

# Emulator runtime probe for naming (T-20500)

Use the headless emulator to inspect a function or global at runtime. It can stop at a main-executable or overlay address; read registers, arguments, pointed-to memory and the call stack; record address reads or writes; press pad buttons on specified frames; capture screenshots; and save or load states. Ticket: [[tickets/T-20500-emulator-assisted-naming]].

## Setup

- `tools/emu/Dockerfile` builds PCSX-Redux from commit `8288e927` (2026-09-25) plus upstream fix `3a24bbcc`, which fixes an SPU crash when loading a state into a new process. It also builds OpenBIOS from nugget `d9384092` under the MIT license. No Sony BIOS is used or needed.
- The emulator runs natively on arm64 (Apple Silicon), with the interpreter CPU, no window (`-no-ui`), and a dummy audio driver. The `tokimemo-emu` image is 1.7 GB. Its first build takes about 40 minutes at `-j2` and leaves about 8 GB of build cache. `docker builder prune` frees that cache, but later patches then require a full rebuild.
- Supply your disc through `disc/*.cue`, `--iso`/`--cue PATH` (a cue inside the repository, e.g. a patched disc from `tools/patch/disc.py`), or `EMU_CUE=/any/path/disc.cue tools/emu/run.sh ...`, which mounts that cue's folder read-only at `/cue` (T-39504). Symbols and call stacks also need the extracted `disc/files/SLPM_86.053` and `disc/files/CDROM/EXEDIR/*.EXN` ([[disc-layout]]). A normal build produces linker maps at `build/SLPM_86.053.map` and `build/ovl/*.map`, which give exact function names. Without maps, names marked `?` are guessed from the previous `jr $ra`.
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

## Route (T-39504)

`tools/emu/route.py` plays a scripted route through the main screens and takes screenshots, so two discs (Japanese, English) can be compared screen by screen. Ticket: [[tickets/T-39504-emulator-screen-route]].

```sh
EMU_CUE=/path/to/tokimemo.cue tools/emu/run.sh route run --label jp            # from power-on
tools/emu/run.sh route run --label en --build en --cue build/disc/en.cue      # another disc
tools/emu/run.sh route run --label jp2 --resume build/emu/route-jp:holiday_1  # from a saved point
tools/emu/run.sh route compare build/emu/route-jp build/emu/route-en          # side by side
tools/emu/run.sh route check                                                  # parse, list the steps
```

- Output `build/emu/route-<label>/`: numbered PNGs (`NNN_<checkpoint>.png`), `result.json` (checkpoints in order with name, frame, file, the scene key and the cursor; the step log; every change of the scene key; the `state` points), `sheet.html` and `sheet.png` (contact sheet), `states/*.sstate`, the run's own memory cards (`cards/`, new and empty at the start) and `emulator.log`. `compare` writes `build/emu/compare-<a>-<b>/compare.html` and `compare.png`, one row per checkpoint name, "not reached" where a run has no such shot. All of it is game data: never commit it.
- The route is the text file `tools/emu/routes/main.route`; the step syntax is in the `route.py` docstring. Steps wait on the game's own bytes, not on frame counts: the scene key (GameState `game_mode`, `mode_stage`, `mode_step`, `substep` and the loader's `ovl_target_game_mode`), the menu selection `menu_cursor_0/1` (`pick N` steps it with the d-pad in the keyboard menu mode, the default), the free cursor `cursor_x/cursor_y` (`steer`), and the typewriter `text_state` (`talk`: one screenshot per whole page, then CIRCLE). Longer English pages only make a run longer.
- `goto LABEL COND max=N` loops: Shiori's answer to a date is random, so a refused call is made again the next Sunday (up to three times), and the date is skipped when no call succeeds. A shot taken again in a loop gets `_r2`, `_r3`.
- Name entry is per build (`call name_entry`, `BUILDS` in `route.py`): `jp` types on the hiragana page (cells from the gojuon table), `en` on the English letter page. The letter-page layout is an assumption (`LATIN_ROWS`, A-Z then a-z row by row) until [[tickets/T-39502-english-text-runtime]] fixes it.
- `--relink DIR` (T-39505): the disc holds a relinked build (`build/en`, `build/jr`, [[relink]]); the scene key, `menu_cursor`, `cursor_x/y`, `text_state` and hex addresses in conditions are read at their addresses in that build (translated through `tools/english_link.py`). Needed for every relinked disc, the English one included: the bss moves behind new code. The `emu.py` run log (mode byte, resident overlay) still assumes the default addresses and is wrong for such a disc.
- `--resume RUN:STATE` loads a state of an earlier run and copies its earlier screenshots; cards start empty, so resume only from states before the save.
- Settings the route makes: text speed fast, voice OFF (option 6; the default is "cut with CROSS"), album slot 1.
- Tests: `tools/emu/test_route.py` (parsing, waits and jumps as jobs, resume, PNG and sheets; no game data).

Checkpoints (Japanese baseline, 2026-11-01): Konami logo, title intro, title menu, options (text speed, voice), file select (new album, slot 1, system file), name entry (page, three parts, birthday, confirm, Shiori's profile), prologue (one shot per page), command screen with the status panel, calendar, system settings, first school day, week 1 (Shiori's birthday event with two choices), Sunday holiday menu, phone book, the call to Shiori and her answer, the evening command screen, save (menu, slot, overwrite, writing, done), week 2, the date in the park (with a topic choice), load (slot, read, loaded). See the T-39504 ticket for frames and run time.

## Spike results (2026-10-24)

- The game boots under OpenBIOS: Konami logo and intro movie (MDEC/STR works), NAME_ENT loads at frame 3538 (mode byte 0x11), the attract prologue runs, START reaches the title menu. Speed without breakpoints is about 3x real time (900 frames from a state in 18 s, 3600 frames from power-on in 70 s).
- `disp_string` (O.BIN name) runs once per frame on the title menu, called from `func_80043510+0x11C`; its `a1` alternates between 0x800E8C70 and 0x800E8C84 (two buffers 0x14 bytes apart, one per frame), `a2`=0x7F, `a3`=5.
- On the title menu no code runs in the overlay slot (400 frames, range breakpoint), so the title menu is main-exe code; NAME_ENT code runs in the attract prologue.
- NAME_ENT `func_80142E24` runs once per frame on frames 7 to 249 after the NAME_ENT load state (243 calls, via the entry `func_80144AD0` from the main loop `func_80042540+0x220`), then stops; on its last frame it calls `set_kanji_string` (O.BIN name) with `a0`=-144, `a1`=0x14 then 0x24 (0x10 apart, probably one text line), `a2`=0 and `a3` a Shift-JIS string in NAME_ENT rodata (`D_8014C6F4+0x24`).
- The mode byte `D_800E738E` (GameState+0x110E) is zeroed twice during boot by BIOS code and set to 0x11 at frame 3533 by `func_80042960+0x7C`, called from `main_state_set+0x4C`.

## Limits

- Each breakpoint is checked on every instruction: 363 breakpoints slowed a 4000-frame run to 9.5 minutes. Trace a few dozen functions, or start from a state close to the scene.
- The call stack walks each function's prologue (`addiu sp,-N`, `sw ra,X(sp)`) over the static code. It stops in BIOS or kernel code (the report then prints `ra`) and at functions that do not save `ra`.
- Memory reads cover RAM, the scratchpad and the BIOS ROM; VRAM, SPU RAM and the GTE are not exposed by the job runner (PCSX-Redux has Lua and web APIs for them).
- BIOS calls run OpenBIOS code, not Sony's: watch hits inside BIOS functions (memset, CD) show OpenBIOS addresses. Timing can differ from the real BIOS.
- Save states belong to this emulator build; after a PCSX-Redux bump, old states may not load.
- EVENT and GYOZI load addresses are inferred ([[overlays]]); their breakpoints still check the code bytes.
