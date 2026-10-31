---
type: tool
updated: 2026-10-30
sources: ["port/godot/engine/engine.gd", "port/godot/engine/game_mode.gd", "port/godot/engine/cd_queue.gd", "port/godot/engine/reset_iris_wipe.gd", "port/godot/engine/_cd_drive.gd", "port/godot/tests/engine/engine_diff.gd", "port/godot/tests/engine/engine_diff_runner.gd", "tools/emu/func_capture/capture.lua", "tools/emu/func_capture/run.sh", "tools/emu/func_capture/pack.sh", "tools/port/run_engine_diff.sh", "tools/port/gen_engine_tables.py", "tools/port/check_engine_signatures.py", "port/godot/engine/_libc.gd", "config/port/engine-diff/"]
---

# Engine porting contract (T-39301, layer 2 of [[port-completion-plan]])

How the main-executable C (`src/main/*.c`) is ported to GDScript so that several workers can port files in parallel and every file behaves the same way. Ticket: [[tickets/T-39301-engine-api-core]]. The memory it runs on: [[game-memory-model]]. Worked examples: `engine/game_mode.gd`, `engine/cd_queue.gd`.

## Layout

```
port/godot/engine/engine.gd           the facade (owns memory, disc pack, renderer, CD drive, service slots)
port/godot/engine/<c_file>.gd         one module per src/main/<c_file>.c, functions under their C names
port/godot/engine/_*.gd               helpers that are not C files (never scanned as modules), e.g. _cd_drive.gd, _libc.gd, and the SDK layer _sdk_gpu.gd, _sdk_etc.gd, _sdk_card.gd, _sdk_gte.gd (registered by engine.gd as modules sdk_gpu, sdk_etc, sdk_card, sdk_gte); _frame_sizes.gd and _symbols.gd are generated (tools/port/gen_engine_tables.py)
port/godot/tests/engine/<c_file>_test.gd         hand-written checks on invented memory (auto-discovered)
port/godot/tests/engine/engine_diff*.gd          the function-differential harness (tier 1, see below)
config/port/engine-diff/<c_file>.json            which captures cover a module and which RAM may differ, with reasons
tools/emu/func_capture/                          capture tooling (emulator); tools/port/run_engine_diff.sh runs the harness
```

The facade finds modules by scanning `res://engine/*.gd` (not `engine.gd`, not `_*.gd`), so adding a C file edits no shared file.

## One module per C file

```gdscript
extends RefCounted
## Port of src/main/game_mode.c (original object 0x80042540-0x80042A00, T-39301).
const Math = preload("res://core/native_math.gd")
const C_FILE: String = "src/main/game_mode.c"
const C_FUNCTIONS: Array[String] = [...]     # every function of the file that this module ports
const C_UNPORTED: Array[String] = []         # functions of the file not ported yet (empty when the file is complete)
const C_SIGNATURES: Dictionary = {"mode_step_set": {"args": ["s32"], "ret": "s32"}, ...}
var engine # GameEngine (engine/engine.gd); set when the engine registers the module

func mode_step_set(step: int) -> int: ...
```

- **Names.** The file is `<c_file>.gd` for `src/main/<c_file>.c`; each function is a method with the exact C name (`cd_read_sectors`, not `readSectors`). Private helpers start with `_`. A function of another C file is never ported here, even if it is convenient: call it through the facade and let its owner port it (see Claims).
- **Header comment.** First lines: `Port of <C path> (original object <range from config/objects/main.txt>, T-39301)`, what the file does, the contract and test pointers. Each ported function carries a comment naming the C behaviour it reproduces when the GDScript is not a line-by-line reading (quirks, `FAKE` idioms of the matching C, observed asm).
- **`C_FUNCTIONS`** is the complete list of ported functions. The facade refuses to start (a `registration_errors` entry the core suite asserts empty) if a name is not a method or two modules list one name. A file ported only in part lists the rest in `C_UNPORTED`.
- **`C_SIGNATURES`** gives each function's argument types (`s32 u32 s16 u16 s8 u8 ptr`) and return type (`void` or a C type). It is checked against the C by `tools/port/check_engine_signatures.py --repo .` (Docker; argument count and return type of the definition in `C_FILE`): a function the C declares non-void cannot be marked void to skip `v0`. A non-void C function that never returns a value says so with `"ret_undefined": "<reason>"` (then `v0` is not compared); `ret_defined: [values]` lists the values the C returns on its defined paths when only some paths fall off the end (`cd_command_push`); `"ret_v1": true` makes the harness compare the second return register (`engine.ret_v1`). `call_c` checks the argument count (a mismatch is a refusal) and narrows the return value to the declared type (sign-extension of `s8`/`s16`, masks of `u8`/`u16`/`u32`/`ptr`); the harness compares the narrowed values and also fails a capture whose original `v0` has bits outside the type.

## Memory: C globals, structs and pointers

Everything the C reads or writes lives in the engine's `GameMemory` (`engine.memory`, 2 MiB at 0x80000000, [[game-memory-model]]). A port function never keeps game state in GDScript members.

| C | GDScript |
|---|---|
| `game_state.field` | `var gs = engine.memory.game_state()`; `gs.get_field("field")`, `gs.set_field("field", v)`; arrays `gs.get_field("a", i)`; nested struct `gs.at("mode_step_timer").set_field("w", 0)` |
| a global (`cd_state`, `talk_voice_wait_frames`) | `engine.memory.read_global("cd_state")` / `write_global("cd_state", v)`; arrays by index; `global_address(name)` for `&global` |
| a struct array element (`cd_command_ring[i].lba`) | `engine.memory.view("CdCommand", global_address("cd_command_ring") + i * 0x18)` |
| a data symbol the generated layout lacks (a local `extern`) | a constant with its address and a comment citing `config/symbol_addrs*.txt` (see `CD_READ_RETRY_COUNT` in `cd_queue.gd`); never edit `core/game_layout.gd` (generated by `tools/port/gen_game_memory.py`) |
| a pointer | an `int` holding the PS1 address (`0x801A0000`); read with `memory.u8/u16/u32/s8/s16/s32(address)`, write with `write_u8/…`, `read_bytes/write_bytes` for ranges. A string literal is its rodata address (`STR_LOAD_FUNC_EXEC = 0x800AF390`, found in `config/objects/main.txt` and the executable image), never a GDScript String |
| a pointer to a local (`u8 buf[0x20]` passed to a callee) | stack memory at `engine.frame_base() + original offset`, see Stack frames |
| code addresses (`((s32 (*)()) ovl_entry)()`) | `engine.call_address("ovl_entry", address)` |

**Integer widths.** GDScript ints are 64-bit; the C width is applied where the C applies it: stores through `write_*` / `set_field` keep the low bits (a `u8` field wraps at 0x100), arguments narrow where the C narrows (`Math.u8(arg0)` for a K&R `u8` parameter), arithmetic that can overflow uses `core/native_math.gd` (`add32`, `sub32`, `sll32`, `sra32`, `mult32`, `divmod32`, `u16`, `s16`, …). `/` and `%` truncate toward zero like C; a division by zero is a refusal, never a result. Return values are narrowed to the C return type.

**`switch` becomes `match`**, one arm per `case` group, in the C order. A case list too long for one line is a constant array used with a guard (`var m when m in OVERLAY_MODES:`, `game_mode.gd`), because a GDScript pattern list cannot span lines. `break` is the end of the arm; there is no fall-through to port unless the C has one (then say so).

**Stack frames.** `call_c` owns them: before the call it subtracts the callee's original frame size (`engine/_frame_sizes.gd`, generated from the `addiu $sp, $sp, -N` prologues of the asm by `tools/port/gen_engine_tables.py`; no frame size is a registration error) and restores `sp` on every exit, refusals included. A function whose C frame holds an object whose address is passed on (a local buffer) puts it at `engine.frame_base() + <original offset>` (the offsets are in the C comments and the asm). No manual push/pop. Stale stack bytes the C copies (`cd_command_push` copies 16 bytes of a 4-byte parameter) are then the same bytes the original copies. The harness sets `sp` to the captured entry sp.

**Hot paths.** `engine.gs_get/gs_set("field")` and `memory.u8(engine.gs_address("field"))` use cached addresses (the View API allocates and looks up strings on each access: measured 1.4-1.9 us against 0.64 us for `gs_get` and 0.29 us for a cached-address read); `call_c` caches the Callable and the arity and adds the frame, arity and return handling (about 1.2 us over a direct method call: 4.8 against 3.6 us for `mode_step_next`, measured by `tests/engine/bench_engine.gd`). Use the View API for bit-fields and nested structs, the cached accessors in per-frame code.

**Time.** The only clock is `game_state.frame_count` (`engine.frame()`). Timers the engine models (the CD drive) are functions of it; wall-clock time and render frames never decide game state.

## Calling other functions

- `engine.call_c("name", [args])` calls any C function by name, in the same file or another. Use it for **every** call, including calls inside one module, so test doubles, traces and refusals work. It returns the C return value (`0` for void).
- **A callee nobody has ported is a refusal.** `call_c` latches `engine.refusal = "<caller> -> <callee>: C function <callee> is not ported (no engine module lists it)"`, **freezes the game memory** (`memory.frozen`: every `write_*`, `write_bytes` and `fill` returns without changing RAM until `engine.clear_failure()`), returns 0, and every later `call_c` does nothing. No result is invented, nothing is skipped silently, and the run must stop: the driver (frame loop, test) checks `engine.halted()` after each top-level call. A memory fault (outside RAM, misaligned) freezes and halts the same way. The state after a refusal is therefore exactly the original's state at the call of the refused function.
- **Inside a function:** no check is needed before a store (it cannot land after a refusal). Services other than RAM (`engine.cd_drive`, renderer, audio, text layer) check `engine.halted()` before they act; a new service must do the same. Do not use the return value of a refused call for anything but more refused work. **Loop rule:** every loop whose condition or body depends on `call_c`, a drive or another service, or `call_address` must test `engine.halted()` and have a bound (after a refusal those calls return 0 at once, so such a loop would spin forever). `tests/engine/engine_lint_test.gd` (core suite) fails a `while` loop in `engine/*.gd` that calls `call_c` or a service without `halted()`, and a `while true` without a `break`.
- **Function pointers.** `engine.call_address(via, address)` looks the address up in the generated name/address table (`engine/_symbols.gd`: `config/symbol_addrs*.txt` and the asm labels; `engine.function_address(name)`, `engine.function_at(address)`): a known main-executable function dispatches to `call_c` (a ported function runs, an unported one refuses by name); any other address is overlay or unmapped code and refuses with its address (overlay code is ported per overlay later) unless a test double is installed under `via`.
- **SDK functions (T-39314).** The PsyQ and BIOS calls the game's main executable makes are provided by the facade, so ported code calls them by their symbol name with `call_c` like any C function (`AddPrim`, `func_8009F004`, `OpenEvent`, `rand`; the names are those of `config/symbol_addrs_sdk.txt` and the asm labels, so an unnamed library routine keeps its `func_XXXXXXXX` name; each module header names the library function behind it). They behave as the library does on the game memory, in the same RAM words, and hand work to the services:
  - `libc` (`engine/_libc.gd`): `memcpy memmove strlen strcpy strncpy strcat strncat strcmp bzero` (PS1 semantics, `memmove` as the SDK code in `asm/libc_memmove.s`), `rand`/`srand` (BIOS A0[2f]/[30]: the LCG of `core/native_rng.gd` on the seed word in kernel RAM, `0x8000850C` for OpenBIOS, so the generator state is in the RAM image and no second RNG exists), `abs`, `printf` (the BIOS text goes to a console the port does not have and no game memory changes; the value returned is the character count, which no caller uses, so the signature says `ret_undefined`) and `sprintf` (the conversions `%% %c %s %d %i %u %x %X %o` with flags `- 0 + space`, width, precision and `h`/`l`; anything else refuses). `printf` and `sprintf` are variadic: `C_SIGNATURES` gives the required `args` and `"variadic": N` extra integer arguments, the method declares all of them, and `call_c` pads the missing ones with 0.
  - `sdk_gpu` (`engine/_sdk_gpu.gd`, libgpu): the primitive initialisers (`SetPoly*`, `SetSprt*`, `SetTile*`, `SetLine*`), `AddPrim`/`AddPrims`, `SetSemiTrans`, `SetShadeTex`, `GetTPage` (`func_8009ECB0`), `GetClut` (`func_8009ED74`), `SetDrawMode` (`func_8009D294`), `ClearOTag`/`ClearOTagR`, `DrawOTag`, `DrawPrim`, `LoadImage`/`StoreImage`/`MoveImage`/`ClearImage`, `DrawSync`, `ResetGraph(mode != 0)`, `SetDispMask`, `PutDrawEnv`, `PutDispEnv`, and the libgs leaves `GsDrawOt`/`GsSortClear`. Each writes what the library writes (headers, links, the DR_ENV packet inside the DRAWENV, the copies at 0x800DD598/0x800DD5F4, the GP1 shadow at 0x800E6160, the library's queue words at 0x800DD67C..0x800DD6A4) and sends what the library sends to the GPU to `engine.renderer`: linked lists (DrawOTag, PutDrawEnv, MoveImage, ClearImage) through `draw_ordering_table_from_memory`, CPU/VRAM copies through `load_image`/`store_image`, display control through `gp1`. The renderer draws before the call returns, so the GPU and the DMA channels are idle at every later call: the library's empty-queue fast path is the only path, and a call that finds a queued job (`0x800DD68C != 0x800DD690`) or a draw-sync callback refuses. `engine.renderer_ready()` opens the renderer on first use; without the GDExtension a renderer-bound call refuses naming itself.
  - `sdk_etc` (`engine/_sdk_etc.gd`, libetc): `VSync` (`func_800AD950`), see the blocking-wait rule; the I_MASK register the libgpu queue saves is kept there (a hardware register, not RAM).
  - `sdk_card` (`engine/_sdk_card.gd`, libapi/libcard): `EnterCriticalSection`/`ExitCriticalSection`, the BIOS events (`OpenEvent EnableEvent CloseEvent TestEvent`; the event control blocks live in kernel RAM exactly as the BIOS keeps them, table pointer at `0x80000120`), and the card jobs `_card_info`/`_card_load` as events delivered a fixed number of frames after the call (`INFO_FRAMES`, `LOAD_FRAMES`, measured on OpenBIOS: a function of `frame_count`, delivered when a poll asks). `InitCARD StartCARD _bu_init _card_auto` only record that the driver started. `_card_status _card_clear open close read write format delete` refuse by name: the captured behaviour is a blank card only (see the header of `_sdk_card.gd`), and no success path or file-job duration was measured. `engine.memory_card` (a `services/memory_card_service.gd`) must be attached for `_card_info`/`_card_load`.
  - `sdk_gte` (`engine/_sdk_gte.gd`, libgte): `rsin` and `rcos` (`func_800A0140`) read the library's sine table from game memory. The rest of libgte (the GTE register file) is not provided.
  - Every other SDK call (libgs object sorters `func_80098680..func_8009A4D0`, libspu/libsnd, libpress/MDEC, libcd's `func_8008B750`, `GetRCnt`/`ResetRCnt`, the remaining libgte) is an unported function and refuses by its symbol name. libcd is replaced by `engine.cd_drive` (limits in `engine/_cd_drive.gd`).
- **Blocking waits** (`VSync(0)`, `DrawSync(0)`, `CdSync(0)`, `CdReadSync(0)`, spin loops on hardware): the C waits inside one frame for something the frame loop produces, so a port cannot spin. Rule: a wait is modelled only when its condition is a function of `frame_count` and game memory that the model can evaluate inside the call (a drive command that completes in the call: `CdControlB`; a poll that is true by a computed frame: `CdReadSync(1)`; `DrawSync(0)`, whose condition (nothing queued, GPU idle) is true by construction because the renderer finishes a list before `DrawOTag` returns; a card event that is due by a computed frame); everything else refuses naming the wait (`func_800AD950` for `VSync(0)`/`VSync(n)`/`VSync(1)`, `GetRCnt`), and the refusal stops the run at the exact call. `VSync(-1)` returns the vblank counter word in RAM, which only the VBlank interrupt advances: the port never does, so it is a defined read of memory, not a clock. Never advance `frame_count` from inside a call to make a wait finish.
- **Services.** `engine.disc_pack` (DiscPack: `cd_read_*` read from it), `engine.renderer` (Ps1Renderer), `engine.text_layer` and `engine.audio` (the existing text-layer store and audio service, null until the integrating code attaches them), `engine.memory_card` (the virtual card, null until attached), `engine.cd_drive`. `engine.renderer_ready()` opens the renderer on first use and says whether it can take work. A module that needs a service that is absent refuses naming the function; it never skips the work. Presentation (text, audio, VRAM) goes through these slots, not through new per-scene code.
- A callee's *documented* behaviour is not enough to skip it: if a ported function's C calls `message_window_title_show(0)`, the port calls it (and refuses until it is ported).

## Testing a function

Two kinds of tests, both required before a function counts as ported.

1. **Hand checks** (`tests/engine/<c_file>_test.gd`, extends `RefCounted`, `run() -> Array[String]` of failures, auto-discovered by `tests/engine/run_engine_tests.gd`, part of the core suite). Invented memory; every expectation derived by hand from the C. Cover wraps, every `case`, early outs, the refusal paths and the unported-callee calls (a test double records the arguments: `engine.set_stub("name", callable)`).
2. **Function-differential tests from emulator captures** (tier 1 of [[port-completion-plan]]). The emulator runs the real game to a function; the capture holds the arguments, the whole 2 MiB RAM at entry, and `v0`/`v1` and the RAM at return. The runner loads the entry RAM into `GameMemory`, calls the ported function with the captured arguments, and compares the return value and the **whole RAM** with the original's, byte for byte. A difference is reported by address with its GameState field or global name.

### Capturing (Docker, `tokimemo-emu`; at most one container, `--cpus=4`, named `m2fn-<name>`)

```sh
# 8 hits of mode_step_next and 12 of game_mode_set from a save state; game_mode_dispatch: two hits per distinct
# game mode (byte at 0x800E7388); the unported callees are recorded too (--callee, --callee-ptr for a pointer)
tools/emu/func_capture/run.sh gm-title build/emu/states/restored-title.sstate --frames 1500 \
  --fn mode_step_next:8 --fn game_mode_set:12 --fn game_mode_dispatch:30:0x800E7388:2 \
  --callee message_window_title_show --callee title_mode_handler --callee-ptr ovl_entry:0x80123110 \
  --input 40:START --max-diff 0x200000
tools/emu/func_capture/pack.sh gm-title      # gzips the RAM images, writes index.json (Docker, tokimemo-recovery)
```

`--fn NAME[=0xADDR][:HITS[:KEYADDR:PER_KEY[:FROM_VSYNC]]]` (the address comes from `config/symbol_addrs*.txt`), `--input FRAME:BUTTON[:HOLD]`, `--poke FRAME:0xADDR:HEX` (a constructed state: say so in the spec and the log, as `wiki/emulator.md` asks), `--max-diff BYTES` (diffs above it drop the hit). Output: `<data root>/.asset-cache/m2-engine/<name>/<fn>/hitNNNN.{json,entry.ram.gz}`, `index.json`, `meta.jsonl`, `emulator.log`. **Captures are game data: they stay under `.asset-cache/`, never in Git.** Start from a save state close to the code (`.asset-cache/emu*/`, `build/emu/states/`): every breakpoint is checked on every instruction.

What a hit holds: `a0..a3` and the stack words at `sp+16..` (stack arguments), `sp`, `ra`, the RAM at entry, `ret_v0/v1`, `exit_diff` (the RAM at return as byte runs against the entry RAM), and for each call that the function made to a **listed callee** (the outermost call per hit): its arguments, the RAM at its entry (`pre_diff` against the function's entry RAM), its `ret_v0` and its RAM effect (`effect_diff`). Callees the port has already ported are not listed: they run in the port.

`--cover` also records the executed instruction addresses of every hit (an Exec breakpoint over the function's code range, sizes from `_frame_sizes.gd`); `tools/emu/func_capture/coverage.sh <name>...` then lists, per function, the instructions executed and the **unreached basic blocks** (Docker; reads the asm). Use it to decide what the next capture must reach; a function whose unreached blocks are not covered by a hand check says so in its spec `untested`/notes. `--ptr-bytes N` (default 64) sets how many bytes behind a pointer argument that points into the stack are recorded for each callee call; the runner compares them and the callee's stack arguments (`sp+16..`) and `v1`.

### Replaying (Docker, `tokimemo-port`, `--cpus=4`, container `m2diff-<pid>`)

```sh
tools/port/run_engine_diff.sh --file game_mode                  # one module: config/port/engine-diff/game_mode.json
tools/port/run_engine_diff.sh --file cd_queue --function cd_command_pump --show 20 --ranges
tools/port/run_engine_diff.sh --all
```

The unported callees are replayed from the capture: when the port reaches one, the harness checks that the memory equals the original's memory at that call (entry RAM + `pre_diff`) and that the arguments are equal, applies `effect_diff` and returns the recorded `v0`. A callee the C did not call at that point, a missing call and a refusal are failures. These doubles live only in the harness.

The SDK modules hand drawing to the renderer GDExtension, which is not installed under `port/godot/bin`: `run_engine_diff.sh` mounts it read-only at `res://bin/` from `RENDER_SO` (default: the `build-v3` candidate under `.asset-cache/m2-renderer`); without a library the renderer-bound functions refuse and their hits fail. `tools/port/run_sdk_render_check.sh` runs `DrawOTag` of the SDK layer over the T-39302 frame captures and compares the renderer's VRAM with the emulator's.

### The spec (`config/port/engine-diff/<c_file>.json`, committed; no game data in it)

```json
{"captures": ["gm-title", "gm-route"],
 "exclude": [{"field": "GameState.frame_count", "reason": "..."},
             {"start": "0x80000000", "length": 65536, "reason": "..."},
             {"global": "cd_sectors_left", "reason": "...", "when": {"global": "cd_state", "equals": 4}}],
 "functions": {"cd_command_pump": {"exclude": [], "not_comparable": {"cd-title:21": "reason", "cd-xa-b:*": "reason"}}},
 "untested": {"game_mode_next": "no caller in src/; covered by tests/engine/game_mode_test.gd"}}
```

- **Excluded RAM needs a reason** (an entry without one is an error), applies to every hit of the file, or per function, and may be conditional on the entry state. Exclusions are **one-sided**: the original may differ there, but the port's bytes must still equal the entry RAM (the port must not write where it claims not to), unless the entry says `"port": "free"` (the port computes that field itself, for example a timing-dependent counter). Two ranges are always excluded for the original, with their reasons printed: the dead stack `[sp - 16 KiB, sp)` (the C frames of the function and its callees spill registers there; nothing reads them afterwards; the original's callee depth is not recorded, so 16 KiB is an upper bound) and the o32 argument home area `[sp, sp+16)` in the caller's frame (the K&R code spills `a0-a3` there). The port may write only inside the frames `call_c` reserved (`[engine.min_sp, sp)`); a port write anywhere else in the dead stack fails. Clock fields (`frame_count`, interrupt-driven counters) are excluded only when a capture shows them differing, and the reason says so.
- **`not_comparable`** names hits whose result is known to differ for a stated reason (state that lives outside game memory, a timing the model approximates) **and the ranges the difference may touch**: `{"reason": "...", "allow": [{"global": "cd_queue_head"}], "ret": false}`. Everything else is still compared, and a difference outside the allowance is a failure shown under `--show`. They are run and counted, printed separately, and never dropped silently.
- A `not_comparable` entry may say `"refuses": "<text>"` instead of an allowance: the hit passes when the port's only problem is a refusal containing that text (the original served the call through a path the model refuses by design, for example a job queued behind a running GPU DMA, which is hardware state that is not in RAM). Such hits are counted and printed separately.
- **`untested`** lists a function with no comparable capture and why (never reached from the sampled states, no caller in `src/`, only hardware-state-dependent hits). Every function in `C_FUNCTIONS` needs at least one passing hit or an `untested` reason with a hand check; the runner fails a function whose hits all fail or are not comparable and is not listed.
- Hits replay in capture order on one engine per capture and function, so state outside game memory (the CD drive model) evolves as it did between the captured calls. That state cannot be loaded from RAM: a hit that starts inside a hardware operation is `not_comparable`, with that reason.
- A pass reports `bytes compared` (2 MiB per hit minus the exclusions). Report the counts per function in the ticket.

## Claims: how workers never overlap

- A work package is a set of whole C files. The orchestrator writes the package list in the ticket (one ticket per package, `T-39301-<package>`) and each worker works on its own branch `port/m2-engine-<package>` in its own worktree, cut from the integration branch after the previous package merged.
- **Shared wiki files belong to the orchestrator**: `wiki/log.md`, `wiki/index.md`, `wiki/kanban.md` and the Status section of this page. Workers never edit them; they record progress in their ticket comments (dated) and in a per-package notes section at the end of their ticket (decisions, captures used, limits, unreached blocks). The orchestrator folds that into the log and the index at merge.
- **A missing data symbol** (a global the generated layout lacks, so the C cannot be ported with a layout name) is reported in one place: a "Missing data symbols" comment in the worker's ticket (name, address, type, the C file that uses it). The orchestrator adds it to `config/symbol_addrs*.txt` and the include headers and regenerates `core/game_layout.gd` once, on the integration branch. Until then the module uses a named constant with the address and a comment citing the symbol file (as `CD_READ_RETRY_COUNT` in `cd_queue.gd`); a worker never declares its own `extern` in the C, and never edits the generated files.
- A worker edits only: its modules (`engine/<c_file>.gd` for its files and `_*.gd` helpers it adds), its own tests (`tests/engine/<c_file>_test.gd`), its specs (`config/port/engine-diff/<c_file>.json`), and its ticket and log lines. It **never edits** `engine/engine.gd`, `core/game_layout.gd`, `core/game_memory.gd`, `tests/run_tests.gd`, `tests/engine/engine_diff*.gd`, another worker's module, `src/` or `include/`. A change one of those needs goes into the ticket as a request; the orchestrator lands it on the integration branch first and the workers merge it.
- A function belongs to the module of the C file that defines it (`config/objects/main.txt`, the `/* file.c: ... */` header of the C). If a worker needs a callee from another file it calls it through `call_c` and, in its tests, replays it from a capture (the callee list) or installs a double. It does not port it "for convenience": two workers porting one function is the overlap this rule prevents. A worker that finds a function in the wrong file reports it.
- The call order between packages is free: a call to a function of a package that has not merged refuses with its name, and the differential tests replay it from the capture. Packages merge in any order; the refusals disappear as packages land.
- `git merge` of the integration branch before the final gates; core suite and opening frame timing unchanged (see the ticket).

## Status

(This Status section is the orchestrator's.)

| C file | Module | Functions | Package |
|---|---|---|---|
| `game_mode.c` | `engine/game_mode.gd` | 8/8 | base |
| `cd_queue.c` | `engine/cd_queue.gd` | 23/23 | base |
| `reset_iris_wipe.c` | `engine/reset_iris_wipe.gd` | 5/5 | base, [[tickets/T-39311-engine-draw-core]] (merged) |
| `memory_card.c` | `engine/memory_card.gd` | 40/40 | [[tickets/T-39313-engine-menus-memcard]] (merged) |
| `hud_screens.c` | `engine/hud_screens.gd` | 44/44 | [[tickets/T-39313-engine-menus-memcard]] (merged) |
| `menu_speaker_girl.c` | `engine/menu_speaker_girl.gd` | 35/35 | [[tickets/T-39313-engine-menus-memcard]] (merged) |
| `text_system.c`, `frame_render_audio.c`, `background_images.c`, `actor_sprites.c`, `gpu_primitives.c`, `gpu_effects.c`, `screen_layers.c` | `engine/<file>.gd` | 114/114 | [[tickets/T-39311-engine-draw-core]] (merged) |
| `scene_library.c`, `scene_display.c`, `hud_panels.c` | | 164 ported, review fixes in progress | [[tickets/T-39312-engine-scene-hud]] |
| PsyQ SDK calls | `engine/_sdk_*.gd` | in review | [[tickets/T-39314-engine-sdk-layer]] |

Per-function results are in each ticket. `game_mode_reset_to_title` is defined in `reset_iris_wipe.c`, not in `game_mode.c`; the engine keeps the file boundary of the C. Until [[tickets/T-39314-engine-sdk-layer]] lands, every PsyQ SDK call refuses by name, so card, GPU and VSync paths stop at their first SDK call.

## Limits and decisions

- **Refusals are the point.** Until the other files land, most paths end in a refusal; that is the correct, checkable behaviour (`cd_read_sectors` refuses at `audio_command` until `frame_render_audio.c` is ported).
- **The CD drive model** (`engine/_cd_drive.gd`) models reads (floor(14 + 0.4 x sectors) frames from the request, from the T-39002 model in `services/prologue_media_queue.gd`), the commands `cd_queue.c` issues and the XA head position (2.5 sectors per frame); CD command latency other than reads is 0 frames (T-39002 measured Pause 4 and XA start 5-6 frames; the XA timeline belongs to the audio work). Not modelled and refused: CD-DA Play, Stop, Mute, blocking `CdSync(0)`/`CdReadSync(0)`, the retry path's 512 `VSync(0)` waits. The model counts from the request (`cd_command_push` of a read) and shares the measured opening table (`PrologueMediaQueue.read_vsyncs`: `MEASURED_READ_VSYNCS`/`MEASURED_READ_SECTORS` keyed by game mode, stage and LBA, the request's mode and stage recorded at the push), so a later rewire of the opening keeps its timing exact. In the pump replays the capture hits are issued without the push, so the model counts from the issue frame and completes one frame after the original: that is the whole explanation of the 3 pump hits listed `not_comparable` in `config/port/engine-diff/cd_queue.json` (their allowance is the ring head, status and result bytes only). Real drive timing is hardware and is checked as a status sequence in `tests/engine/cd_queue_test.gd`.
- `engine.call_c` takes integers; a module needing a string or buffer passes its PS1 address.
- **Coverage tracing.** PCSX-Redux accepts an Exec breakpoint with a width, so `--cover` records every executed instruction address of a function without a per-instruction script hook; the cost is one callback per instruction while a hit is pending (fine for a few hundred hits, slow for a function that loops thousands of times per call). Unreached blocks are printed by `coverage.sh`; they are information, not yet a gate.
- The original's stack depth below a call (callees of callees) is not recorded, so the dead-stack range for the original stays an upper bound (16 KiB); the port side is exact.
- `tools/port/check_engine_signatures.py` needs the repository (not mounted in the core-suite container); run it with the other gates. The generated tables need the asm of the user's build (`asm/` is not committed): regenerate with `docker run ... python3 -I tools/port/gen_engine_tables.py --repo /work --asm /asm --out port/godot/engine`.
- The `rules/` modules of these C files (the existing `core/game_state.gd`, `core/mode_router.gd`, the media queues) are not rewired yet; the engine versions are the C-faithful ones, and the routes keep using the old code until a package moves them without changing route results.
