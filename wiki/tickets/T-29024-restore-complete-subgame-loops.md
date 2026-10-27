---
id: T-29024
title: Restore source-backed TACO and TT gameplay
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Restore source-backed TACO and TT gameplay using actual original-backed content and production execution.

## Acceptance criteria

- [ ] Reproduce the existing baseline defect and implement the real connected behavior.
- [ ] Source-backed input/state/timing tests and actual runtime evidence; no synthetic replacement completion.
- [ ] Fresh content identities and explicit unsupported states.
- [ ] Independent review resolves findings before Done.

## Notes

Prepare isolated packets under `.claude/orchestrator/state/T-29024/`; root serializes integration. Original-input feasibility for enemy-v3 maps the three model bases and two stage3 bank reads to authenticated ISO extents; all six requested guardian pose byte ranges rehash from original TACO.EXN. Exact paths, LBAs, hashes and remaining decode/runtime gaps are in `.claude/orchestrator/state/T-29024/enemy-v3/media-extraction-feasibility-v1.md`.

## Received bounded combat packet

Packet `packet-v1.json` SHA256 `3d082af2dea00404814e83ce7f44bfc66f7d499c2e72a579ac3de5ec97b0fdd6` is frozen under the ticket state directory. Root verified all 14 candidate file hashes/sizes and absent destinations. The worker reports independent Standards/Spec acceptance, 24 player, 22 collision and 34 controller checks, 104 original-instruction comparisons and five shared suites. Those reports are bounded function/core evidence; root has not applied the packet or accepted a connected subgame.

Separate dispatcher proposal hunks require composition with the overlay repairs. Wingman/GTE/targeting, secondary weapons, enemies/stages/bosses, original presentation/audio, complete result/save/exit behavior and TT remain open. Preserve the failed initial signed-16 projection evidence. No subgame acceptance or percentage increase follows from receiving this packet.

## Cumulative isolated composition

Root composed the three reviewed packets in `root-composition-v1` against the current port, verifying 23 distinct additive files, all packet/source/external-pack pins and 103 artifact hashes. Docker checks pass: 24 player, 22 collision, 34 combat, 39 stage and 35 enemy checks, five shared suites, plus replay of 104 recorded original-instruction cases. These remain local or captured-boundary checks, not a connected playable subgame. No canonical files were applied. The cumulative handoff specifies genuine model/pose/GTE providers, three wave factories, unresolved observable guardian coordinates and targeting data, outer-frame ordering and audio dependencies. The next integration must preserve those explicit refusals rather than supplying successful empty callbacks.

## User-flagged synthetic TT preview

The user's shooter screenshot matches the kind of output built by `presentation/subgame_overlays_presenter.gd::render_tt_twinbee`: generated modulo-pattern scenery, geometric cloud/boss/player helpers and a ranking panel drawn unconditionally. This is not original asset rendering or verified TT gameplay. A separate audit is identifying exact gallery capture provenance and native rendering differences under `T-29024/tt-visual-audit-v1/`. Root will remove confirmed synthetic previews from the actual progress gallery while preserving diagnostic originals. Source comments alone do not establish fidelity.
