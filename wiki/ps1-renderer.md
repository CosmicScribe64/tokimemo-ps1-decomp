---
type: tool
updated: 2026-10-30
sources: ["tools/port/native_render/gpu.hpp", "tools/port/native_render/extension.cpp", "tools/port/native_render/test_gpu.cpp", "tools/port/native_render/compare_capture.cpp", "port/godot/presentation/ps1_renderer.gd", "tools/emu/gpu_capture/"]
---

# Generic PS1 renderer (T-39302, layer 3 of [[port-completion-plan]])

One renderer for every scene: a 1024x512 16-bit VRAM and a GPU primitive rasterizer that draws the game's own ordering tables, in place of one renderer per scene. Ticket: [[tickets/T-39302-generic-ps1-renderer]]. Presentation only: it reads the game memory ([[game-memory-model]]) and never writes game state. No existing scene uses it yet; that is the engine work ([[tickets/T-39301-engine-api-core]]).

## Pieces

| File | What it does |
|---|---|
| `tools/port/native_render/gpu.hpp` | The model, header-only C++17, no Godot dependency: VRAM, GP0 stream decoder, ordering-table walk, rasterizer, GP1 display state, primitive counters, dirty rectangle. |
| `tools/port/native_render/extension.cpp` | GDExtension class `OriginalGpu` (library `liboriginal_render`, entry `original_render_init`) over the model. |
| `port/godot/original_render.gdextension` | Manifest (Linux arm64). The library is not installed under `port/godot/bin/`; tests mount a candidate there. |
| `port/godot/presentation/ps1_renderer.gd` | `Ps1Renderer`, the typed wrapper the port calls. |
| `tools/port/native_render/build.sh`, `build.py` | Docker build with the godot-cpp commit and SCons wheel pinned in `tools/port/native_audio/dependency-provenance.json`. `--test` runs the C++ tests. |
| `tools/port/native_render/test_gpu.cpp` | 38 C++ tests, 537 checks, hand-computed pixels. |
| `port/godot/tests/render/run_ps1_renderer_tests.gd`, `tools/port/run_render_tests.sh` | 170 Godot checks through the wrapper and the built library; the script runs both test sets in Docker. |
| `tools/port/native_render/compare_capture.cpp` | Draws one captured ordering table and diffs the result with the emulator's VRAM. |
| `tools/emu/gpu_capture/` | PCSX-Redux capture of RAM and VRAM before and after a DrawOTag call (`capture.lua`, `run.sh`, `extract.py`), the noise-background pass (`poison.py`, `capture_frame.sh`), and a synthetic ordering table of invented primitives (`synth_ot.py`, `synth_frame.sh`). |

## API (`Ps1Renderer`)

`open()` loads the extension and creates the GPU. VRAM: `set_vram`, `vram`, `load_image`, `store_image`, `move_image`, `fill_rect`. Drawing: `gp0_word`, `gp0_stream` (raw little-endian GP0 words), `gp1`, `draw_ordering_table(ram, address)` and `draw_ordering_table_from_memory(memory, address)` (tag word: next pointer in the low 24 bits, payload word count in the high 8; the walk ends after the node whose next pointer has bit 23 set). The walk is bounded: a loop (a node reached twice, found with a visited bitmap), more than `max_nodes` nodes or more than `max_words` words (default 131,072) stops it and sets `error`. An unterminated polyline is cut at 4,096 words, counted as `POLYLINE_TRUNCATED`, and its remaining words are skipped up to the terminator. Failed calls set `Ps1Renderer.last_error`; methods called before `open()` report instead of crashing; a display switched off by GP1(03h) reads as black. Read-out: `display_info`, `read_image(rect)`, `display_image()` (RGBA8, native 320x240 unless GP1(08h) says otherwise). Diagnostics: `environment`, `counters` (primitive counts by name), `dirty_rect`.

**Colour convention.** A 5-bit channel `c` becomes `c << 3` (0..248). This is the rule of the emulator screenshots (`tools/emu/emu.py`) and of most presenters (calendar, dialogue, encounters), and it is what `compare_capture` and the Parity comparisons see. `replicate_bits = true` gives `(c << 3) | (c >> 2)` (0..255), the rule of the MDEC exporter and the opening actor renderer. VRAM itself is always compared as 16-bit words, so the choice only affects the Image.

## Rules implemented

- Environment: draw area (inclusive), offset (11-bit signed), texture page (E1 and the polygon attribute), texture window, mask set and check, dither flag, texture flip bits, DR_MODE/DR_TPAGE. Primitives: POLY_F3/F4/G3/G4/FT3/FT4/GT3/GT4, LINE_F2/G2 and polylines (terminator `0x5xxx5xxx`), TILE (variable, 1, 8, 16), SPRT (variable, 1, 8, 16), fill, VRAM copy, CPU to VRAM (also inside an ordering table).
- Polygons: a quad is two triangles (v0 v1 v2, v1 v2 v3). Edge-function fill with the top-left rule: a pixel exactly on an edge belongs to the polygon only for a top edge (horizontal, running right) or a left edge (running up), so the right and bottom edges are not drawn and shared edges draw once. Triangles with a vertex distance over 1023 (x) or 511 (y) are skipped. Colour and texel coordinates are interpolated exactly in integers from the barycentric weights, rounded to nearest with ties up.
- Textures: 4-bit and 8-bit CLUT, 15-bit direct. An all-zero texel is transparent; bit 15 of a texel marks it semi-transparent when the primitive is, and is stored. Raw uses the texel; modulated computes `min(255, (texel5 * colour8) >> 4)`, so colour 0x80 is neutral. Sprite coordinates wrap at 256 after the texture window; the window is `(u & ~(mask*8)) | ((offset & mask)*8)`.
- Semi-transparency: `(B+F)/2`, `B+F`, `B-F`, `B+F/4`, clamped per 5-bit channel; a textured primitive blends only texels with bit 15.
- Dithering (E1 bit 9) applies to Gouraud and modulated-texture polygons and lines, not to flat polygons, raw textures, tiles or sprites. The matrix is the psx-spx one, indexed by the VRAM x, y. A semi-transparent dithered pixel blends at 8-bit precision (background << 3) and adds the dither to the blended value before truncating, as the reference emulator does; hardware order is not settled by the public documents.
- Transfers and fill wrap at the 1024x512 edges. Fill ignores the draw area and mask bits and rounds x down and width up to 16. A VRAM copy runs pixel by pixel in row-major order reading the source live, so an overlapping copy repeats pixels it has just written (also what the emulator does).

## Exactness against the emulator

Method: PCSX-Redux (`tools/emu`, interpreter, OpenBIOS) runs a save state. The emulator was observed as a black box (its VRAM output and the public hardware documents); no emulator source code was read or copied into the renderer or its tests; a breakpoint at `DrawOTag` (0x8009CB9C) saves the 2 MiB RAM and the GPU block of a save state before the call, and a second state when it returns (the DMA list is executed synchronously, so VRAM then holds the drawn frame). `compare_capture` feeds the RAM bytes and the VRAM before to the renderer, walks the same ordering table, and compares all 1024x512 pixels with the VRAM after. The draw area and offset the GPU held are the PutDrawEnv words libgpu keeps in RAM (`0x800DD5B8`). Each frame is checked twice:

1. **real**: real VRAM before. Many frames repeat the previous one, so few pixels change.
2. **noisy**: both frame buffers of the state at the return of the previous DrawOTag are filled with deterministic noise, the emulator resumes and the game builds and draws the same ordering table over it. Every covered or blended pixel has to agree; uncovered pixels stay noise on both sides.

Captures are game data and stay under `.asset-cache/m2-renderer/`. 29 frames from 28 saved states (name entry and file select, prologue, command screen, school days, phone, school events, seasonal events, VN scenes, saves, endings). Table: ticket [[tickets/T-39302-generic-ps1-renderer]]. Result: **52 of the 58 comparisons (26 of 29 frames, real and noisy) are pixel-exact over the whole VRAM; the other six (the three phone frames, real and noisy) differ in one pixel by one 5-bit step.** The noise rectangles are derived from the compare run's draw area and the GP1 display registers, and the script fails if the noisy run draws a different ordering table. The game issues only POLY_G4, POLY_FT4, POLY_GT4, TILE_1, SPRT, SPRT_16 and FILL in these frames, with 4/8/15-bit textures, semi-transparency modes 0 and 2, and dithering.

Primitives the game did not issue in the sampled frames are covered by a synthetic ordering table drawn by the emulator and by the renderer over a real VRAM (`synth_ot.py`, every texture and CLUT invented):

| Group | Pixels the emulator changed | Pixels differing | Notes |
|---|---|---|---|
| flat and Gouraud polygons, dither on and off | 17,842 | 1 | one Gouraud pixel, one 5-bit step |
| semi-transparent polygons, four modes, Gouraud and dither | 21,447 | 0 | |
| textured polygons (4/8/15-bit, raw, modulated, scaled, rotated, semi) | 21,012 | 830 | scaled and rotated mappings only |
| lines, polylines, Gouraud, dither | 12,679 | 144 | |
| tiles, 1x1, 8x8, 16x16, semi | 12,762 | 0 | |
| sprites, all sizes, depths, raw and modulated, semi | 15,689 | 0 | |
| texture flip bits | 12,564 | 559 | emulator ignores them |
| texture window and wrapping | 12,633 | 0 | |
| clipping, offset, oversize, one-pixel and degenerate polygons | 16,494 | 0 | |
| mask set and check | 12,461 | 0 | |
| fill, copy, overlapping copy, load inside the table | 14,387 | 0 | |

## Known differences from the emulator

- **Interpolation rounding (1 pixel in 3 real frames, 830 synthetic texture pixels, one polygon pixel).** The emulator walks scanlines with truncated fixed-point deltas, so its colour and texel coordinate can be one step off the exact value at scattered pixels (an exact `31.5` texel coordinate gives 31 where the exact rule gives 32; a colour of 195.4977 gives 196). The renderer uses the exact nearest value, which matched the emulator on every real frame except the three phone frames (pixel (220,140) of the backdrop gradient). Over the whole synthetic table, counted before the copy rule was aligned, nearest left 1,921 pixels different, floor 5,668 and ceil 5,711. Hardware rounding is not specified by the public documents; no hardware capture was available.
- **Texture flip bits (559 pixels).** The emulator draws the sprite unflipped; the renderer mirrors the texture (E1 bits 12 and 13, textured rectangles). The game never sets them in the sampled frames.
- **Line stepping (144 pixels).** The minor axis is a half-pixel-offset DDA; the emulator steps one pixel earlier at some boundaries and rounds Gouraud line colours one step differently. The game issued no line in the sampled frames.
- **Dither and blend order.** The renderer follows the emulator (blend at 8 bits, then dither). With dither after a 5-bit blend, 3,008 pixels of one name-entry frame differed.
- Texture disable (E1 bit 11) applies only after GP1(09h) allows it; `compare_capture` seeds it from GPUSTAT bit 15. Command mirrors follow psx-spx (0x81-0x9F as 0x80, 0xA1-0xBF as 0xA0, 0xC1-0xDF as 0xC0; 0xE0 and 0xE7-0xEF are no-ops).
- Not modelled: interlaced field skipping, 24-bit display, the draw-to-display flag, GP0(1Fh) interrupts, the VRAM-to-CPU transfer through GP0 (use `store_image`).

## Using it

```sh
# unit tests: C++ (UBSan, ASan) and Godot, in Docker
bash tools/port/native_render/build.sh --output .asset-cache/m2-renderer/<new dir>
tools/port/run_render_tests.sh .asset-cache/m2-renderer/<new dir>/project/bin/liboriginal_render.linux.template_release.arm64.so

# capture one frame from a save state and compare it (needs the tokimemo-emu, -recovery and -content images)
TOKIMEMO_DATA_ROOT=$PWD tools/emu/gpu_capture/capture_frame.sh <name> .asset-cache/emu/<state>.sstate <DrawOTag call number>
TOKIMEMO_DATA_ROOT=$PWD SYNTH_GROUPS=textured,lines tools/emu/gpu_capture/synth_frame.sh <name> .asset-cache/emu/<state>.sstate 10
```

Speed (arm64 container, one core): a typical command-screen frame (611 nodes, 4,650 changed pixels) draws in about 0.05 ms; a full 320x240 quad takes 0.3 ms flat or semi-transparent and 0.45 ms Gouraud, textured and dithered.

## Limits and next steps

- Linux arm64 only, like the audio extension; a macOS build is not made. The library is a candidate under `.asset-cache/`; installing it under `port/godot/bin/` is a decision for the integrating work.
- Texture-hash translation (acceptance criterion 4) is not built: it belongs at the VRAM upload (`load_image`/GP0 A0 inside `OriginalGpu`), with `translated_image_store.gd`, once scenes upload through this renderer.
- Interpolation rounding and line stepping are open against hardware. If a scene needs scaled textured polygons or lines, capture one and decide then.
