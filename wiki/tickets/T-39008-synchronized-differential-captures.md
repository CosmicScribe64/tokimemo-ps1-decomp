---
id: T-39008
title: Restore synchronized PS1 and port differential captures
status: In Progress
assignee: root
created: 2026-10-29
updated: 2026-10-29
links: ["[[emulator]]", "[[port-testing]]"]
---

## Goal

Compare original PS1 execution and actual port presentation at documented simulation boundaries, separating host elapsed time, emulated vsync, game updates, and asynchronous service completion.

## Acceptance criteria

- [x] Restore the pinned emulator in Docker and capture original gameplay.
- [ ] Bind visual captures to original state, inputs, CPU cycles and vsync; capture corresponding port state and viewport.
- [ ] Compare ordered state/RNG/service events without dropping divergent events or shifting frames to conceal bugs.
- [ ] Produce reviewed visual differences with explicit viewport/color rules and separate timing diagnostics.
- [ ] Explain a reproducible contributor workflow and limits of OpenBIOS timing.

## Notes

User explicitly requested timing isolation and visual differential testing. Existing emulator supports vsync inputs, breakpoints, RAM dumps and screenshots; the cleanup removed its image. Existing visual audit paths point to deleted historical captures and cannot establish current parity.

Restoration evidence: `build/emu/original-school-settled-v1` captures the actual first school scene after normal controller entry; the independent continuation below verifies live original state. This completes emulator restoration only. Paired port captures still differ, and ordered timing/state parity remains open.

R24 screenshot repair is awaiting independent review: invalid dimensions/formats/raw lengths become explicit unavailable image records without losing state observations. Valid records carry PNG hashes. Sixteen screenshot/sampling/projection tests pass; actual repaired-run verification remains pending.

## Independent restore lifecycle check (2026-10-29)

Fresh reviewer ran four input-free VSyncs from `original-school-firstline-v1.sstate` with every committed NAME projection range, screenshots1/2/4 and `--sample-every 1`. Evidence: `build/emu/review-restore-lifecycle-v1`. Actual frame1 is explicitly `before_restore` at cycles521480/mode0; a single `state_restore` lifecycle event records cycles521480→5456556950 at host frame1. Frames2..4 are `restored`, cycles5457121353/5457685756/5458250145, overlay mode21. All dump lengths/status and the samples-file SHA validate. The strict full-map projection succeeds for all four rows and retains lifecycle labels/raw clocks. This independently verifies the fix without discarding the pre-restore observation.

The frame1 GPU shot has width/height0 and is not usable visual evidence. Restored shots are320×240. The runner nevertheless lists a PNG path for the zero-sized shot; consumers must reject zero-sized captures. This is distinct from state-capture validity and remains a harness diagnostic weakness. No paired timing or scene acceptance is claimed by this four-frame continuation.

## Independent exact-pair gate (2026-10-29)

Added `tools/port/compare_capture_pair.py`, seven synthetic regression tests and `tools/port/README.capture-pair.md` after root approved the design. Input pins actual receipts/images, explicit source field maps, checkpoint expectations and semantic compatibility. Equivalent field values use strict JSON types, rejecting bool/int or float/int coercion. Raw clocks/lifecycle remain unchanged. Synchronization remains an explicitly external reviewed declaration; the tool does not prove it. Missing/invalid evidence exits2; actual differences, incompatible semantics or unverified synchronization exit1. Synthetic exact identity exits0 only in its declared fixture scope. Pixel comparison includes all RGBA channels, outer edges and16×16 tile locations, with no scaling/masking/tolerance.

The existing actual v3 first-school pair fails as intended: original320×240 versus port viewport1024×768, title timer184 versus109, and text32 versus27 explicitly reported as incompatible domains (native includes title glyphs, port dialogue work omits them). No resized image was substituted. Local input/report: `build/emu/first-school-pair-review-v3.json` and `-report.json`.

The newly captured actual renderer image from `name-emulator-pair-v6/first-school-native.frame.png` permits source-resolution comparison: exact320×240 pixels differ at56273/76800 positions, maximum channel difference255, with differences in all300 tiles. Title counter184 versus116 still differs; text domains remain32 versus27, and timing synchronization is unverified. Gate exits1. Local input/report: `build/emu/first-school-pair-review-native320.json` and `-report.json`. This establishes the mismatch, not its allocation among rendering/state/timing causes. The port receipt states actual renderer after frame_post_draw, resized=false, and binds PNG/raw pixel hashes.

Root's R24 repair independently passes its three tests and an actual two-frame restore (`review-shot-validity-v1`): shot1 marked unavailable with raw preserved/no PNG; restored shot2/final320×240 PNG hashes validate. Its report no longer advertises the unusable image. Combined current comparator/decoder/screenshot suite passes18 tests. Root independently reviews the comparator before integration.

2026-10-29 independent MDEC diagnosis: captured pre-GPU decoded words for school20×16×160 and NAME_SAK20×16×240 using exact delay-slot callsite/pointer/RECT validation. First JAL capture rejected because MIPS load-delay a1 was stale. Source release/file identities preserved in decoded-word receipts. Original school region22950/22950 matches captured words; port6972 matches. Added explicitly non-acceptance RGB555 diagnostic and separate experimental scalar candidate at root authorization; production exporters untouched. Current previews mismatch38121school/42535NAME words; candidate still mismatches9463/10698 and remains unaccepted. Fourcandidate+threediagnostic+sevenpair tests pass14. See review report for pinned independent sources and exact scope; root owns shared log.

2026-10-29 independent accurate-model execution: ignored, attributed DuckStation reference harness at pinned4ac22708f3f0 exactly matches candidate on51200 school+76800 NAME words,1728 color vectors and57 IDCT blocks. Provenance and comparison receipts are in `.asset-cache/mdec-reference/duckstation-4ac22708f3f0/`. Current upstream license is CC-BY-NC-ND-4.0; adapted reference harness remains local/ignored, never production. Shared BS-to-runlevel transport remains a limitation; no hardware-truth claim. Redux AAN captures still differ exactly and raw RGBA gate still fails. Production inventory311v1+2v3 versus fullv7's10v1 requires preservingv3 support; candidate remainsv1/2-only and experimental. Detailed immutable-model proposal and exporter/store/hash migration dependencies appended to [[port-adversarial-review]]. Root review required before production changes; root owns shared log.

2026-10-29 follow-up: candidate now reuses owned v3 DC parser and passes both original TACOv3 files exactly against pinned accurate reference (76800 words each). Source320×240 calls and exact8/9-sector extents at0xb378/0xb380 verified; receipts refreshed. Six candidate tests plus parser/diagnostic/pair total24 pass. This removes the need for the previously proposed legacyv3 fallback, pending review. Production decoder remains unchanged; raw Redux scene mismatch remains. Root owns shared log.

2026-10-29 root-authorized controlled migration: shared exporter now uses reviewed fixed-point model with both implementation hashes, preserved full padded rows and explicit unverified hardware/Redux/raw transport scope. Immutablev11 NAME/board stores pass plus145board assertions. Source-pinned fresh command0/7 producer added as separatev4 identity, whole original pack validated in Python/Godot including SD, changed proof/route/model/payload rejection and failed-reopen clearing. Four fresh real-pack tests pass; generated manifest70453a62d88bb6b9c1bf4478dee9d07a5be12fdd93b4225b5e7f7c4b26ad4839. Full bundle role wired; root review/final rebuild pending. Earlier77producer tests include9 historical-fixture skips, not acceptance. Details and discovered/resolved SD schema regression in [[port-adversarial-review]]. Root owns shared log.

2026-10-29 pause: root independently accepted bounded model migration/fresh school recovery and seasonal integrity repair. Fullv12 build completed (O11+school role), only holiday_scene/hud_context producer gaps remain. Current authenticated stores pass;41decoder/fresh-bundle/comparison tests pass; nine seasonal adversaries pass with mandatory explicit success marker/noSCRIPT ERROR. Retained raw320pair still exits1. Model runlevel transport claim remains false and runtime rejects fabricated true. All reviewer handles settled; no HUD implementation started. Detailed source audit/next steps and fidelity limits in [[port-adversarial-review]]. Root owns push and shared handoff/log.

## Rei opening stages 8-10: emulator checkpoints compared with port (2026-10-29)

Native capture runs in Docker `tokimemo-emu` (PCSX-Redux 8288e927+3a24bbcc, OpenBIOS), with no pad input; START is never pressed because it skips the prologue (`prologue_mode_handler`). The runs start from the existing `build/emu/states/original-school-firstline-v1.sstate` (sha256 b9c8ad5b…). A 3900-vsync prefix gives `.asset-cache/emu-rei-stage8-10-v1/stage7-line11-prefix.sstate` (c1561b0b…). Pass A2 samples 2600 vsyncs. Pass B uses the same start, samples every vsync with dumps `800E7374:32 8014D180:12 801220E0:4 80120650:72 80129628:2`, and takes 159 shots (`pass-b/result.json` ce6db4b9…). All output is under `.asset-cache/emu-rei-stage8-10-v1/`. Port side: `.asset-cache/port/rei-lines16-v3` (`rei-failure.json` + `stageS-K.png`). That port run ended in failure: `rei-failure.json` has message `Rei observer stopped at 10/0/8: source_transaction: active original BGM actual bank changed` and `state.frame_count` 4907, the same frame as the `stage10-8` record. Pair builder: `tools/port/rei_stage_capture_pairs.py`, with 18 synthetic tests; with the 7 `test_compare_capture_pair` tests, 25 pass. Each synchronization guard was removed in turn in a scratch copy, and each removal fails at least one test. The comparator is unchanged. Current outputs: `.asset-cache/emu-rei-stage8-10-v1/pairs-v2/` (`port-png-bindings.json` bce68a9a…, `sync-evidence.json` e551fa49…). The earlier `pairs/` directory is kept unchanged; its `stage10-8` pair is superseded.

Observer revision: the receipt was written by an uncommitted observer revision whose hash is unknown. `tests/opening/run_installed_rei_banner.gd` at HEAD c1c2a7aa5 (sha256 2381dc77…) records keys only while `mode_stage == 8`, or 9 with dialogue, so it cannot emit `stage10-*`. The working-tree file (sha256 d43b8644…, `target`/`SUBSTEPS` logic) can, but it was modified at 18:42:27, after the receipt was written at 18:12:54, so it may not be the producing revision. `port-png-bindings.json` pins that file's hash under `observer` with `modified_after_receipt: true`. Both revisions write the key as `"stage%d-%d" % [state.mode_stage,state.substep]`, which is what the binding decodes. A rerun with a committed observer is needed to pin the producing revision.

### Replay determinism

Replays from the same state reproduce every sampled game-state byte (`800E7374:32`, `8014D180:12`, `801220E0:4`) at every vsync: coarse vs prefix over 3899 vsyncs, pass A2 vs pass B over 2199 vsyncs. Two things vary between runs:

- CPU cycle counts at the callback drift by ±11.
- The lip-sync frame byte `0x80120668` (actor96 frame) differs on 14–25 vsyncs per replay pair, because it is driven by the XA level (`getCDlevel`).

So mouth frames are not replay-stable. Pass B images and RAM come from the same callback.

### Synchronization rule and evidence

Pass B, and pass A2, put every stage/substep transition at the same frame_count. The rule:

1. Native iteration F is the first vsync sample whose post-update RAM holds the port key (mode 0x15, stage S, step 0, substep K).
2. The previous sample must be frame_count F-1, and every PC in the window must be inside VSync (0x800AD950-0x800ADB30, `asm/libetc.s`).
3. The image is the shot at frame_count F+2, and F..F+2 must be single-vsync iterations.

The display latency of 2 is measured from four discrete onsets (`pairs-v2/sync-evidence.json` `latency_probe`; the probe is unchanged from `pairs/`). Each shows its largest pixel change at E+2:

| Onset event | Iteration E | Region | Changed pixels at E+0..E+3 |
|---|---|---|---|
| Stage8 white boxes (case 1 first run) | 12204 | scene | 9115 / 9146 / **33133** / 319 |
| Line 13 text clear | 12509 | text window | 417 / 238 / **1100** / 238 |
| Line 14 text clear | 13067 | text window | 340 / 238 / **1770** / 459 |
| Line 16 text clear | 13899 | text window | 340 / 238 / **1706** / 459 |

This matches the order in `draw_frame`: DrawSync, then VSync, then GsSwapDispBuff (`func_8009AC20`), then GsDrawOt.

I first tried a latency of 3, fitted to a gray-ramp model of the stage8 flash. I rejected it, because the onsets above contradict it and the centre pixel mixes background brightness with the additive flash. Stage 9-3 also shows that a background-layer change becomes visible one iteration later than text, sprites and prims; see defect D2.

Synchronization stays `unverified` until an independent reviewer accepts this rule. Three checkpoints are rejected, with no manifest:

- `stage8-0`: iteration 12139 spans vsyncs 256–258 with the PC outside VSync, during the stage-7 load.
- `stage10-0`: never observed at a vsync.
- `stage10-1`: iteration 13500 spans vsyncs 1617–1619, and 13501 is held for 2 vsyncs.

`stage10-8` has a valid native window (F=14066, image vsync 2188), but it is excluded: its port record (frame 4907) is the frame at which the port observer stopped with the `source_transaction` failure. The tool reports it as `port_failure_frame` and writes no manifest.

### Exact results (25 synchronized-window pairs)

Every pair exits 1 (rerun on `pairs-v2`; pixel results equal the earlier `pairs/` reports): exact RGBA, no scaling, masks or tolerance, all 300 tiles changed, synchronization unverified. Diff is differing pixels out of 76800; max is the largest channel difference.

| Key | Native F | Native vsync | Port frame | Diff | Max |
|---|---|---|---|---|---|
| 8-1 | 12203 | 323 | 3683 | 58718 | 255 |
| 8-2 | 12215 | 335 | 3695 | 58352 | 255 |
| 8-3 | 12232 | 352 | 3712 | 56221 | 255 |
| 8-4 | 12238 | 358 | 3713 | 56602 | 255 |
| 8-5 | 12450 | 570 | 3868 | 56685 | 255 |
| 8-6 | 12451 | 571 | 3869 | 56702 | 255 |
| 8-7 | 12484 | 604 | 3870 | 59184 | 255 |
| 9-0 | 12508 | 628 | 3871 | 56456 | 255 |
| 9-1 | 12509 | 629 | 3872 | 56182 | 240 |
| 9-2 | 12515 | 635 | 3873 | 57246 | 240 |
| 9-3 | 12697 | 817 | 4057 | 57755 | 255 |
| 9-4 | 13009 | 1129 | 4160 | 56817 | 255 |
| 9-5 | 13010 | 1130 | 4161 | 56729 | 255 |
| 9-6 | 13034 | 1154 | 4185 | 56783 | 255 |
| 9-7 | 13066 | 1186 | 4217 | 56785 | 255 |
| 9-8 | 13067 | 1187 | 4218 | 56174 | 255 |
| 9-9 | 13439 | 1559 | 4532 | 56162 | 255 |
| 9-10 | 13440 | 1560 | 4533 | 56026 | 255 |
| 9-11 | 13473 | 1593 | 4534 | 55698 | 255 |
| 10-2 | 13507 | 1629 | 4537 | 56554 | 255 |
| 10-3 | 13898 | 2020 | 4790 | 59107 | 255 |
| 10-4 | 13899 | 2021 | 4791 | 58608 | 255 |
| 10-5 | 14012 | 2134 | 4904 | 58723 | 255 |
| 10-6 | 14013 | 2135 | 4905 | 58713 | 255 |
| 10-7 | 14046 | 2168 | 4906 | 55946 | 255 |
| 10-8 | 14066 | 2188 | 4907 (port failure frame, excluded) | (57569) | (255) |

The 10-8 figures come from the superseded `pairs/` report and are not a result. Reports are in `pairs-v2/pair-<key>-report.json`. The prologue_line mapping is `incompatible` for 9-0, 9-7 and 10-3, where the next line starts inside the latency window, and `equivalent` for the other 22.

### What the diagnostics show

The diagnostics below are not an acceptance rule and do not change any gate. They hold the colour-expansion difference fixed as a hypothesis so that the remaining differences show:

- Every native channel value in all 26 shots has its low 3 bits zero (v<<3). These 26 native shots include 10-8; the native shot is valid, only its port frame is excluded.
- Port pixels are mostly v<<3|v>>2 (bit replication), but some, such as HUD frame (160,48,24), are exactly v<<3.
- With exact-or-replicated counted as equal, the scene area (x 1..319, y 1..159) has 0 residual pixels at 8-1, 8-3..8-7, 9-0 and 9-6..9-11.

### Defects and issues found

The port rendering code was not changed.

**D1, port state (stage8-2, port frame 3695).** The port leaves `D_801220E0` at 63; native writes 0x80.

- Native: `prim_bg_slot_setup+0xD8` (`src/main/gpu_effects.c:67-69`, called from `prologue_background_slot_set` at the end of stage8 substep1) stores 0x80 to `0x801220E0`. Watch run `watch-801220E0` hit #66 records this at vsync 332: memory before 0x3F, a2=0x80. The native window shows 128 at F=12215.
- Port: `first_arrival_media_bridge.gd` `rei_banner_effect("color_cut")` (lines 900-913) never sets `D_801220E0/E1/E2`, and the port record shows 63.
- Visible result: the port draws a dimmed and desaturated Rei with a full-brightness actor96 mouth rectangle. 28861 scene pixels differ by more than 24 (bbox 3,1-305,159).

**D2, port draw timing (stage9-3, port frame 4057).** The port shows the "伊集院レイ"/roses banner layer one iteration early.

- In the source, case 2 calls `dec_bg_draw(1)` (`prologue.c:1524`) before it sets `bg_layers=0x41000000` (`prologue.c:1528`).
- Native first shows the banner at the shot for iteration 12698 (vsync 818, frame_count 12700), one iteration after F=12697.
- The port draws the post-update `bg_layers` state in the same frame. 8074 pixels differ by more than 24 (bbox 1,24-319,159).

**D3, service timing.** CD reads and XA voice-start finish in 1 port frame, and lines end early. The table compares iterations per key, native versus port:

| Key | Waits on | Native | Port |
|---|---|---|---|
| 8-3 | voice start | 6 | 1 |
| 9-1 | voice start | 6 | 1 |
| 8-6 | CD read | 33 | 1 |
| 8-7 | CD read | 24 | 1 |
| 9-10 | CD read | 33 | 1 |
| 10-6 | CD read | 33 | 1 |
| 10-7 | CD read | 20 | 1 (ends at the port failure frame) |
| 8-4 | line 12 run | 212 | 155 |
| 9-3 | line 13 run | 312 | 103 |
| 9-8 | line 14 run | 372 | 314 |
| 10-2 | line 15 run | 391 | 253 |
| 10-4 | line 16 run | 113 | 113 |

The full table is in `sync-evidence.json` `key_durations`. The timing differences cause the `prologue_scene_counter` mismatches:

| Key | Native | Port |
|---|---|---|
| 9-2 | 6 | 1 |
| 9-3 | 188 | 185 |
| 9-4/9-5 | 0 | 1 |

**D4, consequence of D3 (stage10-3 and 10-4).** The port draws a Yoshio (actor96) hair fragment at x 1..32, y 1..47 (1151 pixels differ by more than 24); native has none.

- Port actor96 x is -287/-288.
- The source subtracts 0.5 px per iteration from substep 2 (`prologue.c:1741-1744`). After native's 391 line-15 iterations this places the actor near -355. That value is derived, not dumped.

**D5, RNG consequence (9-4 and 9-5).** Eye pixels differ in 166 px (bbox 148,55-186,64). The blink comes from `rand()&0xFF==0xFF` (`prologue.c:1603-1606`), and the port's RNG call count differs because of D3.

**D6, open low-amplitude differences.** Scene differences up to 24 levels:

| Keys | Pixels |
|---|---|
| 9-1 | 7993 |
| 9-2 | 15581 |
| 10-2 | 3132 |
| 10-5..10-8 (10-8 measured on the superseded failure-frame pair) | 6273 |

Example: at 10-5, pixel (42,125) is native (72,88,152) and port (82,99,156), one RGB555 step. The 9-2 difference is partly explained by the flash counter (6 versus 1). The rest is unexplained; candidates are the GPU dither or modulation rounding. The message-window interior also differs at 2.7k–4.2k pixels with no proven cause.

**C1, capture or comparator domain issues (not port defects yet):**

- The native x=0 column is (0,0,0) in all 26 shots.
- The native y=0 row is black exactly when `gs_buffer_index` (`D_80129628`) is 1 at the shot (13 of 26). Emulator artifact or real output is undetermined.
- The 8-bit conventions differ (v<<3 versus replication). Acceptance needs an explicit reviewed colour rule; the gate still has no tolerance.
- The bottom/left/right checker phase differs. It is driven by the accumulated `D_800B5BCC/D0` (`src/main/hud_panels.c:1387-1396`), which neither side captured. Add `800B5BC8:12` to the next native run and to the port receipt.

**C2, port observer receipt.** `run_installed_rei_banner.gd` writes PNGs without hashes and records no mode_stage or substep per record, so `port-png-bindings.json` adds post-hoc hashes and decodes the key. The observer should bind the PNG sha256 and the clocks itself, and the receipt should name the observer revision that wrote it.

**Not compared:** `stage8-0`, `stage10-0` and `stage10-1` (see above), and `stage10-8` (port failure frame). Timing synchronization is not claimed. These are OpenBIOS and PCSX-Redux results, not hardware.
