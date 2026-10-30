---
type: checkpoint
updated: 2026-10-29
---

# Port pause checkpoint — 2026-10-29

User requested a safe pause, publication of all project work including translated artwork, and a handoff. This is a WIP checkpoint, not a playable release or G5 acceptance. It supersedes older paused-session instructions in [[port-manager-handoff]]. Read [[index]], [[status]], [[port-adversarial-review]], [[port-testing]] and the active tickets before editing. Do not trust historical G1–G4 VERIFIED headings; the actual acceptance gaps remain open.

## Resume priorities

1. Revalidate the current checkout, Docker images, processes and original data. All worker jobs were settled at pause. Do not restart a job merely because its old observation handle expired. Use Docker for tools/builds/dependencies; no host installs.
2. Continue the installed opening from the real stage9 route. Last actual run `.asset-cache/port/rei-dialogue-v2` reached stage9/substep10, then rejected the O11 read in its already-loaded older scripts. The saved exact read/publication fix passes bounded tests but has not had an actual rerun. Next expected stop is the deliberately unsupported `next_scene` O11 copy/setup. Finish that operation and stage10 rendering from the decompilation, then continue to the real room/command handoff. [[tickets/T-39002-restore-port-runtime]] contains the full implementer handoff.
3. Finish the translation inventory and all required artwork, with source/English/restored actual-game checks. Preserve historical outputs, human approvals and separate agent-review metadata. [[tickets/T-39004-english-asset-completion]] and the published translation checkpoint describe what is usable and what remains missing. Do not count candidates as finished assets.
4. Remove the remaining fresh-session HUD and holiday-scene producer gaps through real source-owned initialization and route integration. Then pursue all required gameplay families, persistent saves/relaunch, native audio/platform packaging and G5. Never replace earned routes with injected eligibility or fabricated clear-save fields.

Root should primarily orchestrate and independently review implementation. User authorized delegated opening work, translation completion, modern settings, repeated adversarial review and repository-wide deslop. The deslop pass remains incomplete; see [[tickets/T-39003-deslop-repository]]. Resume a small number of file-disjoint workers rather than creating overlapping ownership.

## Current resources and reproducibility

- Original archive: `/Users/lila/Documents/Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).7z`. Extracted originals remain under ignored `disc/`. Do not modify or publish originals, executable/overlay data, audio, extracted Japanese assets, generated assembly or captures.
- Original track1 SHA256: `28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72`. EXE SHA256: `bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae`.
- Fresh-clone workflow: `bash tools/port/setup_content.sh '/absolute/path/to/original.7z' --output .asset-cache/port/installed-resume`. See `tools/port/SETUP.md`. Output must be new. Wrapper exits3 for partial coverage while preserving output; inspect the generated coverage record.
- Latest complete local export: `.asset-cache/port/installed-runtime-v12`. It has20 available roles, including original O11 and fresh finite school media. Remaining reported gaps: `holiday_scene`, `hud_context`. This does not mean all gameplay is playable.
- O11 board manifest: `ba385b57929a8b6ea389b0018a0aa63cf588256a57a109b09e2d6c3c555df9ad`. Fresh school manifest: `70453a62d88bb6b9c1bf4478dee9d07a5be12fdd93b4225b5e7f7c4b26ad4839`. Schoolv4 proves April/no-companion command0/7 assets, not complete school routes.
- Reuse existing images: `tokimemo-port` (Godot4.7.2 Linuxarm64), `tokimemo-port-gui-test`, `tokimemo-recovery-splat` (Python/Pillow/g++), `tokimemo-recovery` (use python3), `tokimemo-emu`. `tokimemo-content` is the reproducible setup image; tools support documented image overrides.
- Godot `.gd.uid` sidecars are project metadata. `.godot/` and generated content stay ignored. Translated artwork belongs in the repository, with explicit review status; this was expressly confirmed by the user.

## Opening and native audio

The actual opening previously reached Rei stage9 entrance through all stage8 substeps with original resources and real alternate-bank playback. Latest real progression is stage9/substep10 as above. New flow replaces an incorrect synthetic eight-step implementation with the original twelve steps, actual page boundaries, actor completion, postincrement32-tick hold and exact RNG ordering.

O11 is original `CDROM/PROLODIR/O_11.NBN`, LBA0xAC1E,33 sectors to0x801E8000. Source case11 copies0x10800 bytes to0x80180000 before next-stage setup. Stage9 first loads CUT08 at0xAA85/49 sectors to0x801A4000. Do not use the former incorrect25/17-sector requests. Source actor99 completion is tested twice; do not silently change it to actor99+100. Post-switch RNG sees next_stage's reset substep0. O11 frame0 references texture pages1/2; source-pointer ownership is established, but exact physical VRAM slot allocation is not yet proven.

`rei_actor_sequences.gd` reads original O10 frame/sequence tables. It draws before stepping; flags3 and5 terminate differently. The renderer orders crowd/back actors, flash, then body/mouth/blink. Tests cover native durations, failed-reprepare clearing, source speed128 behavior and request ownership. Actual emulator actor/order comparison remains required.

Native candidate `d937156a7aad5ebeb11c60676a1301c470002860b6cde09fcf14696286795480` is retained in `.asset-cache/recovery/rei-fade-review-v3/candidate.so`, with receipt/tests. It implements actual Rei bank loading, audible fade, early-start and XA branches, with publication identity/replay checks. The tracked production library and its pin remain unchanged; actual candidate runs use isolated library/bridge copies. Do not claim new native source is already the packaged library. Rebuild through `tools/port/native_audio/build.sh` in Docker; independently qualify before publication. macOS native extension remains unavailable/unvalidated.

## Rendering, settings and translation

Native canvas is320×240, independently fitted within actual-window UI coordinates. Increasing the window does not reflow original decorative overlap. Actual960×720 and1280×800 entry→opening handoffs retained identical native RGBA images and centered transforms. Actual NAME mouse/keyboard navigation covers Symbols/Kanji/readings/arrows/slots; source cursorframe0 and continuous movement precede confirm hit-testing. Exact PS1 input-latch phase remains unproven. Settings preview/Keep/Revert, controller save actions and bounded voice switching have tests; full gameplay save/relaunch remains incomplete.

At pause,59 historical English assets remain (56mapped,3unmapped), with127 new candidate records. Nine native-font NAME frames have bounded agent-style review and seven-state actual source/English/exact-restoration evidence. Restoration tests explicitly hold animation during language repaint. Japanese header prompts, five NAME variants without observed use, battle Miss variants, FILESEL buttons and unresolved catalog groups remain open. Publication directories preserve drafts versus reviewed variants; agent acceptance is not human approval. See the translation ticket for final publication counts and commands.

## Differential evidence and decoder

Exact capture comparison rejects changed hashes, missing state, wrong semantic domains and unverified synchronization. It performs no scaling, masking, arbitrary frame search or tolerance. The retained first-school native320 pair fails:56,273/76,800 RGBA pixels differed before migration. The retained raw pair still exits1 after tooling migration; do not describe that old pair as a fresh rendering of v12.

The independent fixed-point decoder matches a pinned accurate-model reference exactly on four original assets and numerical edge vectors. It supports BSv1/2/3, preserves padded output, records both parser/numerical source hashes and regenerates derived identities. This is not hardware certification or Redux AAN equality. BS-to-runlevel transport still shares the parser and needs captured-game verification. Adapted third-party research harnesses remain ignored/local; original reference sources retain their upstream attribution/licensing.

Seasonal media integrity was repaired: verified byte hashes/path/size/PNG bounds, cached images, descriptor/frame/sequence checks and failed-reopen clearing. Actual inventory and nine adversarial cases pass. Seasonal inventory is separate from the full bundle and not proof of played seasonal routes.

## HUD audit for continuation

`live_hud_adapter.open_fresh` exists but is unused by production and pins an old HUD manifest. Its current test constructs command-ready dictionaries. `playable_boot` still consumes a historical captured context; the real installed opening has no completed room→mode0x34/stage1/step0 command handoff. Removing the required role alone would fabricate readiness.

Source `hud_icon_bar_update` has a32-tick hover countdown and sine pulse. Current presentation accepts only settled hover32 and unscaled icons. Preserve actual phase and retained cursor position/style; do not force synthetic settled state. Separate immutable assets from mutable source-owned session context, then test the actual fresh handoff. Detailed source references are in [[port-adversarial-review]]. No HUD implementation was started in this pause pass.

## Checks and known limitations

At checkpoint, core5 suites and differential26 tests passed. Root independently checked migrated decoder40 tests/four original outputs, fresh school4 tests/full Godot store checks, original Rei sequence/control-flow/VRAM tests, seasonal nine adversarial cases, and entry lifetime38 checks. These prove their stated scopes only. Actual routes, all translated assets, persistent clear-save/Continue/recovery, native desktop packaging and G5 are not complete. Original matching decompilation files were not changed during this checkpoint work.

Useful commands, from repository root (adjust only paths/image overrides deliberately):

```sh
docker run --rm -v "$PWD/port/godot:/workspace:ro" tokimemo-port godot --headless --path /workspace --script res://tests/run_tests.gd
docker run --rm -e PYTHONPATH=/work:/work/tools -v "$PWD:/work:ro" -w /work tokimemo-recovery-splat python -m unittest tools.emu.test_capture_ranges tools.emu.test_screenshot_evidence tools.port.test_compare_capture_pair tools.port.test_native_memory_projection tools.port.test_rgb555_diagnostic
docker run --rm -v "$PWD:/work" -w /work tokimemo-recovery python3 tools/wiki.py lint
```

For real route drivers, inspect `tools/port/drive_installed_first_scene.py`, `drive_translation_entry.py` and the installed-opening test headers. Reuse the latest actual-run setup/isolated native candidate described in T39002; do not launch an obsolete deleted-cache fixture and call its failure a new gameplay regression. All screenshots, original data and local execution evidence remain on this machine unless explicitly published as translated artwork.
