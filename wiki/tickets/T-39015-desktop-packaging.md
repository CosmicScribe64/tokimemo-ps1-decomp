---
id: T-39015
title: Unify reproducible desktop packaging
status: In Progress
assignee: Claude (fan-out worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-pause-2026-10-29]]", "[[port-adversarial-review]]"]
---

## Goal

Resolve the packaging findings in the adversarial review: one documented image/build route, actionable missing-prerequisite errors, a reproducible Linux desktop export with the native audio extension, and an honest macOS status.

## Acceptance criteria

- [ ] Implementation follows original source with explicit rejection where unsupported; no synthetic readiness, injected state or fabricated fields.
- [ ] Docker tests pass, including negative cases; evidence limits stated.
- [ ] Independent adversarial review against CODING_STANDARDS.md with all findings resolved.

## Notes

Started in the 2026-10-29 fan-out; worked in an isolated worktree, integrated by root after review.

### 2026-10-29 — packaging worker (worktree `.claude/worktrees/wf_75c1998c-8be-9`)

**Image route (R5).** One route, all from repository Dockerfiles: `sh tools/port/run.sh build` (`tokimemo-port`), `sh tools/port/run.sh build-content` (`tokimemo-content`, no archive needed), optional `sh tools/port/run.sh build-gui-test`. `run.sh`, `build_desktop.sh` and `verify_desktop.sh` check for a stopped Docker daemon and missing images, and print the exact build command. `tools/port/SETUP.md` has a new "Desktop export" section. `godot.lock.json` still records the historical qualification image name and ID; it was left as a record.

**Staging (R3 remainder).** `prepare_desktop.py --target linux-arm64|macos`, plus a `player` scene that keeps `res://shell.tscn`. Linux arm64 stages `bin/liboriginal_audio...so`, `original_audio.gdextension` and its `.uid` only when the SHA-256 values match `LIBRARY_SHA`/`DESCRIPTOR_SHA` parsed from `services/first_arrival_bgm_bridge.gd`, and only when the descriptor declares exactly the two Linux arm64 entries. Any other `.so` is rejected. macOS is code-only. The receipt schema is v2 (`target`, `export_preset`, `native_files` with SHA-256 values, `original_game_data_included: false`). After the copy, the staged native files are checked against the pins again. There are 15 Docker unit tests (7 new, including negatives): `PYTHONPATH=/repo/tools/port:/repo python3 -m unittest tools/port/test_prepare_desktop.py` in `tokimemo-content`.

**Linux arm64 export.** `build_desktop.sh linux-arm64 player OUT` makes a release export: `player/tokimemo-port.arm64`, `.pck`, and the reviewed library (`f1eecf80…`) beside the executable. Linux x86_64 templates are in the image, but the target is refused with exit 3: there is no reviewed x86_64 library, `native_audio/build.py` is arm64-only, and the bridge gates on arm64.

**Defect found and fixed (concurrent file, minimal).** Exported players do not carry `res://bin/*.so` in the pack: Godot copies GDExtension libraries beside the executable. The bridge hashed `res://bin/...so`, so every exported player rejected original BGM with "original opening BGM native library differs from the reviewed component". I added `library_path()` to `services/first_arrival_bgm_bridge.gd`; under the `template` feature it resolves the file beside the executable. The pins and the editor/test path are unchanged. The source-project `tests/run_first_arrival_bgm_load.gd` still passes the hash gate.

**Missing prerequisites (checked).** A missing Godot, staging or GUI-test image exits 1 with the build command. `linux-x86_64` exits 3 with the reason. An existing run directory exits 2. `run.sh test` with a missing image prints the build command.

**Verification (Docker, fresh setup; final build `desktop-linux-arm64-v5`, run `desktop-verify-v5`).** I ran a fresh `setup_content.py` from the original 7z in the worktree. It extracted to the worktree's ignored `disc/` and `asm/` and wrote its output to `.asset-cache/packaging-v1/setup-cache/port/installed-fresh` (manifest `26b5c97d…`). It is partial: `holiday_scene` and `hud_context` are missing, so setup exits 3, as documented. `verify_desktop.sh` with that bundle and `TOKIMEMO_VERIFY_OPENING=1` passed these checks:
- An empty per-user dir routes to first-launch setup, and the extension loads.
- A copy without the `.so` reports the extension load error.
- The bundle publishes through `InstalledPackStore.publish_stage` (`ready`, `launch_ready`).
- The exported player, run headless, routes `playable -> res://app/new_game_entry.tscn` with `failed_gate: ""`.
- Under Xvfb/PulseAudio, X11 Start presses reach the opening. The null-sink monitor recorded 1,919,142 samples in 20 s (peak 14761; 1,917,628 non-zero), and there was no "New Game stopped". The same checks also passed on the earlier v3 build (run v4).

**Control.** The same export built with the HEAD bridge passed the entry checks but failed the opening check: "New Game stopped at source_transaction: original opening BGM native library differs from the reviewed component", with all-zero PCM. Evidence is in `.asset-cache/packaging-v1/evidence/`.

**Evidence limits.** Opening key presses are timed blindly: the exported player has no observer, and templates ignore `--script`. The publication step runs the pinned editor binary against the staged export project, because templates ignore `--script`. It shares the exported player's per-user root, which the script asserts. The audio check proves only non-silent output after the BGM gates. It does not prove PCM parity, timing or hardware device behavior. The checks do not establish gameplay readiness. The export is unsigned. Exports are not bit-reproducible: the binary and library hashes are stable, but the `.pck` hash changed between two exports of identical staged source (v3 `5fad736a…`, v5 `abe7f60e…`). Each build records its hashes in `player.sha256` and `source-receipt.json`. Running `setup_content.py` in Docker directly (named container) was equivalent to `setup_content.sh`; the wrapper itself was not invoked. Core Godot suite (`run.sh test`): 5 suites pass.

**macOS feasibility.** Exporting from Linux works: `build_desktop.sh macos player OUT` produces an unsigned universal `player.zip` with no native files. The BGM bridge rejects the platform, so the opening stops at its BGM gate. A macOS backend is not achievable in the current Docker images. These pieces are missing:
1. A macOS cross toolchain image: osxcross (clang, cctools-port/ld64) with a packaged Apple SDK. Command Line Tools SDKs exist on this host (`MacOSX27.0.sdk`), but the SDK licence limits use to Apple hardware, and the SDK must never be committed.
2. An `arch`/`platform` parameter in `native_audio/build.py` and SConstruct. Both are in the concurrent `native_audio` area and currently hard-require Linux arm64.
3. A universal `.dylib` or `.framework` with ad-hoc signing (Godot's built-in signer can do this on export), plus `macos.*` descriptor entries and new pins.
4. Bridge changes: the platform gate, plus a library path under `Contents/Frameworks`.
5. Real macOS device playback acceptance, which Docker cannot provide.

R4 stays open; macOS is documented as unsupported for player distribution.

### 2026-10-29 — packaging worker, review round 2 (worktree `.claude/worktrees/wf_75c1998c-8be-20`)

This round fixes the five review findings on the first patch. It supersedes the reproducibility limit above.

**Headless guard for the exported library path (finding 1).** I moved the platform and hash gates into `static func native_component_error()` in `services/first_arrival_bgm_bridge.gd`. `prepare()` calls it, so behavior is unchanged. This is a concurrent file; the hunk is small and applies beside the SHIORI_UNSUPPORTED edits. Under `TOKIMEMO_ENTRY_DEBUG=1`, `shell.gd` prints `NativeAudioGate: {"library_path", "error"}` at boot. `verify_player.py` checks that line on every run, so a run without a bundle exercises the gate:
- The exported player must hash the library beside the executable, with `error: ""`.
- The copy without the library must report "original opening BGM native library differs from the reviewed component".

With a bundle, the opening check now always runs, and a missing GUI image fails with the `build-gui-test` command. Control: an export built with `library_path()` reverted to `return LIBRARY` fails the run without a bundle: "bridge hashes 'res://bin/…so', not /build/player/…so" (`desktop-verify-control-r1`).

**Reproducible export (finding 2).** The cause was `[node]` entries without `unique_id`: Godot 4.7.2 assigns random IDs to them during export. The pinned editor generated IDs by re-packing `shell.tscn`, `app/first_launch_setup.tscn`, `app/new_game_entry.tscn` and `app/playable.tscn`. I added only those `unique_id=` attributes, because a full re-save would also reorder properties and add `grow_*` to `new_game_entry.tscn`. `prepare_desktop.py` now rejects any staged `.tscn` node without a stored `unique_id`; two unit tests cover this, one of them negative. `build_desktop.sh` exports twice from the same staged project and fails unless both hash the same. The macOS zip is compared by its entries, because the zip stores export-time dates; the measured zip bytes differed (`desktop-macos-r1`). Linux arm64 builds r1, r2 and r3 are byte-identical across independent stagings and imports: pck `b108188f…`, executable `f252aaad…`, library `f1eecf80…`.

**GUI image override (finding 3).** `Dockerfile.gui-test` now uses `ARG BASE=tokimemo-port` / `FROM ${BASE}`, and `run.sh build-gui-test` passes `--build-arg BASE="$IMAGE"`. Checked with a retagged base (the build log shows `FROM tokimemo-pkg20-base`) and with a missing base (exit 1 with the build command).

**Ticket conflict (finding 4).** The patch no longer adds this ticket file. These notes are appended to the existing untracked ticket instead.

**Python verification (finding 5).** `drive_opening.sh` and the inline in-container shell are replaced by `tools/port/desktop_verify/verify_player.py`, which has a docstring. `verify_desktop.sh` is now only a thin docker-run wrapper. The script waits for the entry diagnostic before it sends keys, using line-buffered stdout through `stdbuf -oL`, because release exports do not flush prints. It no longer sleeps a fixed 8 s.

**Verification (Docker).**
- Staging unit tests: 17 pass in `tokimemo-content`.
- `run.sh test`: 5 suites pass.
- `tests/run_first_arrival_bgm_load.gd` in the source project still passes the hash gate.
- `verify_desktop.sh desktop-linux-arm64-r3` with no bundle (`desktop-verify-r1`): PASS.
- With the `installed-fresh` bundle (`desktop-verify-r3`): all checks pass. Opening PCM: 1,920,366 samples, peak 14801, 1,919,934 non-zero, and no stop.
- Evidence is in `.asset-cache/packaging-v2/evidence/`.

**Remaining limits.**
- Key timing after the entry loads is still blind, and the audio check proves only non-silent output.
- The export is unsigned.
- The macOS zip is reproducible by entry content, not by bytes.
- The two test scenes under `tests/` still lack IDs; they are never staged.
- macOS native audio (R4) stays open, as described above.

2026-10-29 integration into main: the `services/first_arrival_bgm_bridge.gd` hunk (`library_path()`, `native_component_error()`) was not applied because concurrent opening work owns that file; it is kept as `.asset-cache/fanout-patches/packaging.deferred-bgm-bridge.patch` and no longer applies cleanly to the concurrent version. Until it lands, an exported Linux player still hashes `res://bin/...so` inside the pack, `shell.gd` prints a `NativeAudioGate` error naming the missing gate instead of failing with a script error, and `verify_player.py` rejects the export at that check.

2026-10-29 integration follow-up: `tools/port/test_prepare_desktop.py` now loads `prepare_desktop.py` by path with `importlib`, as `test_build_text_pack.py` does, so it no longer needs `PYTHONPATH`. In `tokimemo-recovery-splat`, `python3 -m unittest tools.port.test_holiday_scene_content tools.port.test_build_installed_runtime tools.port.test_build_text_pack tools.port.test_prepare_desktop tools.port.test_rei_stage_capture_pairs tools.port.test_compare_capture_pair` runs 75 tests from the repo root with no extra environment: OK, 3 skipped. `tools/port/test_prepare_desktop.py` alone runs 17 tests, OK, in `tokimemo-content`. The deferred bridge hunk is still unmerged, because the opening workflow still owns `first_arrival_bgm_bridge.gd`. `verify_desktop.sh` cannot pass until that hunk lands.

## Comments

2026-10-29 opening implementer: the deferred bridge hunk (`.asset-cache/fanout-patches/packaging.deferred-bgm-bridge.patch`) is merged by hand into `port/godot/services/first_arrival_bgm_bridge.gd`: static `library_path()` and `native_component_error()`, with `prepare()` calling the gate. Behaviour and pins are unchanged (tracked LIBRARY_SHA f1eecf80…). The opening candidate copy `.asset-cache/port/opening-xa-schedule-bridge-v1/candidate_bgm_bridge.gd` carries the same hunk and differs only in LIBRARY_SHA (67211ee6…). `build_desktop.sh linux-arm64 player .asset-cache/port/desktop-linux-arm64-t39002-v1` succeeded. `verify_desktop.sh … .asset-cache/port/desktop-verify-t39002-v1` (no bundle) PASSES: the bridge gate accepts the library beside the executable, and the copy without the library is rejected. `tests/run_first_arrival_bgm_load.gd` and `run.sh`-equivalent core suites pass on the tracked tree. The bundle (opening audio) variant was not run, because no `installed-fresh` bundle is in this worktree.

2026-10-29 opening implementer: `services/first_arrival_bgm_bridge.gd` changed again (T-39002 review: `prepare_pending` waits for `AudioRules.bank_poll`). The packaging hunk is unchanged. New pinned candidate `.asset-cache/port/opening-hold-bank-bridge-v1/candidate_bgm_bridge.gd` (25e44749…) differs from the tracked file only in LIBRARY_SHA (67211ee6…); it supersedes opening-xa-schedule-bridge-v1 for opening routes. build/verify_desktop were not rerun for this change.
