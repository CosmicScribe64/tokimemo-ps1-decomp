---
id: T-39302
title: Build one generic PS1 renderer for every scene (M2)
status: In Progress
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[phase3]]", "[[ps1-renderer]]", "[[game-memory-model]]", "[[emulator]]"]
---

## Goal

Draw every scene from VRAM and the game's own primitives, in place of one renderer per scene. This is layer 3 of [[port-completion-plan]]. Design, rules, API and results: [[ps1-renderer]].

## Acceptance criteria

- [x] A VRAM model (1024x512, 16-bit) and a C++ GDExtension rasterizer for the primitives the game issues: sprites, tiles, flat, Gouraud and textured polygons, lines, the four semi-transparency modes, dithering, and 4/8/15-bit textures with CLUTs, drawn in ordering-table order. (`tools/port/native_render/gpu.hpp`, `extension.cpp`; walks an ordering table in the game memory or takes a raw GP0 stream.)
- [x] Built reproducibly in Docker next to the native audio extension. (`tools/port/native_render/build.sh`; same compiler image and pinned godot-cpp; two builds gave the same SHA-256.)
- [x] Output is the native 320x240 Image, and exact against emulator captures on sampled frames. (29 frames, 58 comparisons, 52 exact; the other 6 differ in one pixel; differences explained in [[ps1-renderer]].)
- [ ] Translated artwork is applied by texture hash at VRAM upload. Not started: it hooks the upload (`load_image`, GP0 A0) once scenes upload through this renderer.

## Notes

Test commands (Docker only):

```sh
bash tools/port/native_render/build.sh --output .asset-cache/m2-renderer/<new dir>
tools/port/run_render_tests.sh <that dir>/project/bin/liboriginal_render.linux.template_release.arm64.so   # 38 C++ tests (537 checks) and 170 Godot checks
```

Library SHA-256 of the final sources: `863ca5effa3ef4bac1d7e484f85a6282d648e705b2149702e05e65cc33a91c07` (Linux arm64, template_release, g++ in `tokimemo-content`). The library is not installed under `port/godot/bin/`; installing it is the integrating work's decision. No existing scene was switched to it.

Captured frames (game data, kept under `.asset-cache/m2-renderer/`, never committed). "real" is the VRAM the game had; "noisy" repeats the draw over both frame buffers filled with noise. Differing = pixels of all 1024x512 that differ from the emulator's VRAM after the call.

| Frame | Source state (.asset-cache/emu/) | DrawOTag call | Primitives | Real: changed by emulator | Real: differing | Noisy: changed by emulator | Noisy: differing |
|---|---|---|---|---|---|---|---|
| f-command-a | `newgame-golden-v1/states/setA/command-stage1.sstate` | 5 | POLY_G4:4 TILE_1:30 SPRT:244 SPRT_16:46 | 4651 | 0 | 76560 | 0 |
| f-command-b | `newgame-golden-v1/states/setB/command-stage1.sstate` | 60 | POLY_G4:4 TILE_1:30 SPRT:244 SPRT_16:46 | 4694 | 0 | 76240 | 0 |
| f-endsel | `endings-saves-golden-v1/states/select-0528.sstate` | 10 | POLY_G4:4 TILE_1:30 SPRT:245 SPRT_16:46 | 4581 | 0 | 76240 | 0 |
| f-finish | `finish-landing-m3/prefix-stage15.sstate` | 20 | POLY_G4:2 POLY_FT4:8 TILE_1:15 SPRT:77 SPRT_16:20 | 8469 | 0 | 76560 | 0 |
| f-graduation | `seasonal-events-golden-v1/runs/graduation-980301/stop.sstate` | 20 | TILE_1:1 SPRT:6 FILL:1 | 0 | 0 | 76800 | 0 |
| f-nameready | `newgame-golden-v1/inputs/name-ready-v1.sstate` | 10 | POLY_G4:7 TILE_1:110 SPRT:109 SPRT_16:212 | 0 | 0 | 76239 | 0 |
| f-namesel-c | `title-filesel-golden-v1/setC/final.sstate` | 10 | POLY_G4:14 POLY_FT4:16 POLY_GT4:4 TILE_1:41 SPRT:17 SPRT_16:70 FILL:1 | 0 | 0 | 76797 | 0 |
| f-notice | `seasonal-events-golden-v1/runs/notice-0724/stop.sstate` | 15 | POLY_FT4:10 TILE_1:1 SPRT:13 FILL:1 | 0 | 0 | 76800 | 0 |
| f-phone | `dates-phone-golden-v1/states/tel-rival-0x45.sstate` | 10 | POLY_G4:5 TILE_1:35 SPRT:164 SPRT_16:56 FILL:1 | 4603 | 1 | 76798 | 1 |
| f-prologue | `newgame-golden-v1/states/setA/stage20-entry.sstate` | 30 | POLY_G4:2 TILE_1:16 SPRT:70 SPRT_16:22 | 2755 | 0 | 76559 | 0 |
| f-route | `route-golden-three_year_default_continuous-v6/month-002.sstate` | 30 | POLY_G4:3 TILE_1:7 SPRT:164 FILL:1 | 4547 | 0 | 76797 | 0 |
| f-schoolevent | `school-events-golden-v1/runs/se-g1b/stop.sstate` | 10 | POLY_G4:3 TILE_1:7 SPRT:164 FILL:1 | 4574 | 0 | 76798 | 0 |
| f-vn-a | `shiori-landing-m1/states/A-stage10-sub8.sstate` | 10 | POLY_G4:2 POLY_FT4:8 TILE_1:20 SPRT:75 SPRT_16:30 | 2490 | 0 | 76240 | 0 |
| g-endsave | `endings-saves-golden-v1/states/s1-after-save.sstate` | 25 | POLY_G4:4 TILE_1:30 SPRT:245 SPRT_16:46 | 4490 | 0 | 76239 | 0 |
| g-grad | `seasonal-events-golden-v1/runs/graduation-980301/stop.sstate` | 3 | TILE_1:1 SPRT:6 FILL:1 | 0 | 0 | 76798 | 0 |
| g-jun | `seasonal-events-golden-v1/inputs/june01-start.sstate` | 4 | POLY_G4:3 TILE_1:7 SPRT:164 FILL:1 | 4582 | 0 | 76797 | 0 |
| g-may | `seasonal-events-golden-v1/inputs/may01-start.sstate` | 2 | POLY_G4:3 TILE_1:22 SPRT:164 SPRT_16:30 | 4579 | 0 | 76240 | 0 |
| g-meeting | `seasonal-events-golden-v1/runs/meeting-a/stop.sstate` | 12 | POLY_G4:2 TILE_1:4 SPRT:57 FILL:1 | 2436 | 0 | 76798 | 0 |
| g-nameb1 | `newgame-golden-v1/states/nameB1/final.sstate` | 15 | POLY_G4:3 TILE_1:21 SPRT:130 SPRT_16:34 | 0 | 0 | 76239 | 0 |
| g-route-c5 | `route-golden-three_year_default_continuous-v5/final.sstate` | 40 | POLY_FT4:17 TILE_1:1 SPRT:24 FILL:1 | 213 | 0 | 76796 | 0 |
| g-route-final | `route-golden-three_year_default-v2/final.sstate` | 8 | POLY_G4:4 TILE_1:30 SPRT:245 SPRT_16:46 | 4524 | 0 | 76239 | 0 |
| g-se-cal | `school-events-golden-v1/runs/se-cal5/after-shiori.sstate` | 20 | POLY_G4:2 TILE_1:17 SPRT:41 SPRT_16:28 | 0 | 0 | 76241 | 0 |
| g-se2 | `school-events-golden-v2/runs/se2-g2b/stop.sstate` | 25 | POLY_G4:3 TILE_1:27 SPRT:87 SPRT_16:42 FILL:1 | 2690 | 0 | 76799 | 0 |
| g-shiori-m2 | `shiori-landing-m2/prefix-stage7.sstate` | 40 | POLY_G4:2 POLY_FT4:8 TILE_1:28 SPRT:75 SPRT_16:46 | 2513 | 0 | 76239 | 0 |
| g-smoke | `route-golden-smoke-v1/final.sstate` | 15 | POLY_G4:3 TILE_1:7 SPRT:167 FILL:1 | 4543 | 0 | 76798 | 0 |
| g-tel-shiori | `dates-phone-golden-v1/states/tel-shiori-0x45.sstate` | 30 | POLY_G4:5 TILE_1:35 SPRT:164 SPRT_16:56 FILL:1 | 4603 | 1 | 76798 | 1 |
| g-tel-yoshio | `dates-phone-golden-v1/states/tel-yoshio-0x45.sstate` | 60 | POLY_G4:5 TILE_1:35 SPRT:164 SPRT_16:56 FILL:1 | 4325 | 1 | 76797 | 1 |
| g-vn-c | `shiori-landing-m1/states/C-stage10-entry.sstate` | 3 | POLY_G4:2 POLY_FT4:8 TILE_1:12 SPRT:75 SPRT_16:14 | 2505 | 0 | 76239 | 0 |
| t1 | `title-filesel-golden-v1/setA/final.sstate` | 3 | POLY_G4:7 TILE_1:110 SPRT:109 SPRT_16:212 | 28 | 0 | 76558 | 0 |

Differences: 6 comparisons (f-phone, g-tel-shiori, g-tel-yoshio; real and noisy) have one differing pixel, (220,140) of the phone backdrop gradient (a Gouraud quad, dithered): emulator 5-bit 24, renderer 23. The exact value there is 195.4977 and the emulator's scanline interpolation gives 196, the renderer rounds the exact value to 195. Synthetic ordering table (invented primitives over a real VRAM): polygons 1 of 17,842 changed pixels differ, semi-transparency 0 of 21,447, textured 830 of 21,012 (scaled and rotated mappings, interpolation rounding), lines 144 of 12,679 (stepping), texture flips 559 of 12,564 (the emulator ignores the flip bits), tiles, sprites, window and wrap, clipping and offset, mask bits, fill and copy 0 of about 84,000. Details and sources: [[ps1-renderer]].

Open points for a decision: hardware rounding of interpolated colours and texel coordinates, line stepping and the dither/blend order are not settled by public documents or by the emulator (it is not the hardware); the renderer matches the emulator where the game uses them. Installing the library, and the texture-hash hook, belong to the engine work.

## Comments

- 2026-10-30: In Progress. Built the model, extension, wrapper and tests on branch `port/m2-renderer`; compared 29 captured frames and a synthetic table with the emulator (table above). Review against CODING_STANDARDS.md and the wiki rules is still to do before In Review.
- 2026-10-30: Merged `port/opening-to-gameplay` (`9f340f2ab`) and reran the gates: core suite 9 suites pass; opening frame timing `pad 2, 104 substeps, 6709 frames to 12/0/0, 0 mismatches`; renderer tests 35 C++ (512 checks) and 161 Godot checks pass. Ticket stays In Progress (texture-hash hook, library installation and review open).
- 2026-10-30: Independent review fixes (no high findings; 3000 fuzz iterations clean under ASan and UBSan): ordering-table walk now detects loops (visited bitmap) and has a word cap, unterminated polylines are capped at 4,096 words and skipped, public-API coordinates are masked before use, `draw_otag` validates the RAM size itself, GP1(09h) gates texture disable, GP0 command mirrors decoded, capture return breakpoints are registered per distinct return address, noise rectangles derive from the draw area and display registers and a differing noisy table fails the script, compare_capture rejects odd or missing arguments and an `--env-addr` without E-words, `Ps1Renderer` sets `last_error`, guards use before `open()` and shows a switched-off display as black, `.gitignore` covers the library and the test script cleans its mount-point file with a trap. Provenance: the emulator was used only as a black box (running the game under it and reading VRAM and RAM from its save states); no PCSX-Redux source code was read or copied, and the renderer's rules come from psx-spx and from comparing outputs.
