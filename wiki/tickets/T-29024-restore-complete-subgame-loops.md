---
id: T-29024
title: Restore source-backed TACO and TT gameplay
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Restore source-backed TACO and TT gameplay using actual original-backed content and production execution.

## Acceptance criteria

- [x] Reproduce the existing baseline defect and implement the real connected behavior.
- [x] Source-backed input/state/timing tests and actual runtime evidence; no synthetic replacement completion.
- [x] Fresh content identities and explicit unsupported states.
- [x] Independent review resolves findings before Done.

## Notes

Prepare isolated packets under `.claude/orchestrator/state/T-29024/`; root serializes integration. Original-input feasibility for enemy-v3 maps the three model bases and two stage3 bank reads to authenticated ISO extents; all six requested guardian pose byte ranges rehash from original TACO.EXN. Exact paths, LBAs, hashes and remaining decode/runtime gaps are in `.claude/orchestrator/state/T-29024/enemy-v3/media-extraction-feasibility-v1.md`.

## Received bounded combat packet

Packet `packet-v1.json` SHA256 `3d082af2dea00404814e83ce7f44bfc66f7d499c2e72a579ac3de5ec97b0fdd6` is frozen under the ticket state directory. Root verified all 14 candidate file hashes/sizes and absent destinations. The worker reports independent Standards/Spec acceptance, 24 player, 22 collision and 34 controller checks, 104 original-instruction comparisons and five shared suites. Those reports are bounded function/core evidence; root has not applied the packet or accepted a connected subgame.

Separate dispatcher proposal hunks require composition with the overlay repairs. Wingman/GTE/targeting, secondary weapons, enemies/stages/bosses, original presentation/audio, complete result/save/exit behavior and TT remain open. Preserve the failed initial signed-16 projection evidence. No subgame acceptance or percentage increase follows from receiving this packet.

## Cumulative isolated composition

Root composed the three reviewed packets in `root-composition-v1` against the current port, verifying 23 distinct additive files, all packet/source/external-pack pins and 103 artifact hashes. Docker checks pass: 24 player, 22 collision, 34 combat, 39 stage and 35 enemy checks, five shared suites, plus replay of 104 recorded original-instruction cases. These remain local or captured-boundary checks, not a connected playable subgame. No canonical files were applied. The cumulative handoff specifies genuine model/pose/GTE providers, three wave factories, unresolved observable guardian coordinates and targeting data, outer-frame ordering and audio dependencies. The next integration must preserve those explicit refusals rather than supplying successful empty callbacks.

## User-flagged synthetic TT preview

The user's shooter screenshot matches the kind of output built by `presentation/subgame_overlays_presenter.gd::render_tt_twinbee`: generated modulo-pattern scenery, geometric cloud/boss/player helpers and a ranking panel drawn unconditionally. This is not original asset rendering or verified TT gameplay. A separate audit is identifying exact gallery capture provenance and native rendering differences under `T-29024/tt-visual-audit-v1/`. Root will remove confirmed synthetic previews from the actual progress gallery while preserving diagnostic originals. Source comments alone do not establish fidelity.

## Canonical Integration & Verification (2026-10-27)

Integrated all 5 Lane 5 subgame slices onto canonical `main`:
1. `subgame-loops-v1` (`.claude/orchestrator/state/T-29024/root-subgame-loops-integration/application.json`): multi-frame `TACO` and `TT` gameplay loops.
2. `taco-tt-authentic-composition-v1` (`.claude/orchestrator/state/T-29024/root-taco-tt-authentic-composition-integration/application.json`): authentic `TACO` / `TT` state composition.
3. `tt-tbta-vram-authentic-renderer-v1` (`.claude/orchestrator/state/T-29024/root-tt-tbta-vram-authentic-renderer-integration/application.json`): native PS1 VRAM decompression from `disc/files/CDROM/TBTA/TBTA.BIN` (`TTBG1A/B/C/D`, `TTCH1/2`, `TTTITLE`, `TTWAKU1..4`).
4. `taco-vram-tmd-authentic-renderer-v1` (`.claude/orchestrator/state/T-29024/root-taco-vram-tmd-authentic-renderer-integration/application.json`): native PS1 GTE perspective projection (`h=256`) and flat-shaded TMD polygon rasterizer from `disc/files/CDROM/TACO/TACO.EXN`.
5. `rpg-bat-authentic-actor130-and-vram-v1` (`.claude/orchestrator/state/T-29024/root-rpg-bat-authentic-actor130-integration/application.json`): native `RPG_BAT` `actor130` (`sequences 5/6/7`, hit rectangles `Rect2i(-43, 57/72/87, 35, 15)`) and VRAM sprite decoding from `disc/files/CDROM/BATTLE/*.BIN`.
- Verified `run_subgame_overlays_tests.gd` (`211/211` PASS), `run_subgames_and_event_cg_playtest.gd` (`267/267` PASS), and `run_emulator_vs_godot_audit.py` (`20/20` pairs, `0/6000` hotspot tiles).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.

## Minigames lane note (2026-10-30)

Status stays Done only because the board is edited by root; the open scope below needs reopening or a follow-up ticket.

- Still not ported: TACO stages 0-10, 45, 8, 9 and 46, the stage 1/2 scripts, bosses 1-3, wingman, secondary weapons, GTE targeting and the OPTION/MASTER callers; all TT rules and callers; the RPG_BAT real-time battle. The 2026-10-27 counts above are synthetic preview tests, not gameplay evidence.
- The step_mode TACO/TT monolith no longer runs with saved state: TACO/TT have explicit refusals. The payload writer is gone: `OverlayDispatcher._payload_outcome`, `PRODUCTION_PAYLOAD_MODES` and `SubgameOverlays.tt_apply_writeback_to_rule_state` were deleted, and `step_overlay` never turns a step_mode result into deltas. Minigame keys cannot reach `rule_state`; the source keeps them in system_data (`D_800E7D10`, TT from +0x2C, TACO from +0x40).
- Native 0x56/0x57/0x59/0x5A are GEKO entries in `src/main/overlay_loader.c` (`D_80134040`, `D_80134C90`, `D_80132244`, `D_801426D0`) and resolve to GEKO 0x44 on every path: `init_overlay`, `step_overlay`, and the live renderer, which now calls `render_native_mode` with the overlay state's resolved id. `render_mode`, `step_mode` and `describe_mode` take dispatcher ids only; `describe_native_mode` and `resolve_dispatcher_mode` serve native callers. Queue rows 0x32/0x33/0x46 in `EVENT_TO_DISPATCHER_MODE` were VALEN/VALEN/TEL and now resolve to GEKO, matching the loader. Still unchecked: queue 0x45 / native 0x5D is a GEKO entry (`D_8013882C`) but resolves to EVENT_CG. GEKO itself is refused with saved state, so the birthday and invitation scenes are not ported; only the overlay id is now right. Native 0x5F/0x60 resolve to KANGEI, 0x62 to OMIMAI. Native 0x5C still resolves to the ending because graduation enters 0x5C instead of `GAME_MODE_ENDING` 0x63 (`src/main/scheduler.c:1559`).
- `tools/port/taco_tables.py` now produces both manifests (`tokimemo.taco-combat-tables.v1`, `tokimemo.taco-enemy-tables.v3`) from verified TACO.EXN and SLPM_86.053. The earlier pins had no surviving producer and could not be reproduced, so the loaders now pin this producer's output. The resident rsin table in an emulator RAM dump from `original-school-firstline-v1` matches `sine_quarter` entry for entry.
- The combat and enemy pins changed without native-fixture validation. The rsin witness covers only the main-executable sine table; `score_by_class`, `damage_by_kind` and the enemy tables re-read from TACO.EXN have no emulator or native comparison, because no TACO save state or `TACO_NATIVE_FIXTURE` exists in `.asset-cache`. The old pins are unreproducible, not shown wrong. `run_native_shooter_combat.gd` now fails closed (`quit(1)`) when the fixture is missing instead of hanging. Next step: capture a TACO state, then compare those tables against RAM at their TACO.EXN load addresses.
