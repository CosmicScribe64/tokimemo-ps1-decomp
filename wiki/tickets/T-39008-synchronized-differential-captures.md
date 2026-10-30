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
