---
id: T-29025
title: Connect fresh original import to normal New Game
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Connect fresh original import to normal New Game using actual original-backed content and production execution.

## Acceptance criteria

- [ ] Reproduce the existing baseline defect and implement the real connected behavior.
- [ ] Source-backed input/state/timing tests and actual runtime evidence; no synthetic replacement completion.
- [ ] Fresh content identities and explicit unsupported states.
- [ ] Independent review resolves findings before Done.

## Notes

Prepare isolated packets under `.claude/orchestrator/state/T-29025/`; root serializes integration. Stage0 HUD/checker provider inventory and exact source hashes/API/gaps are recorded in `.claude/orchestrator/state/T-29025/stage0-hud-provider-handoff-v1.md`; checker source packet remains ignored under `.asset-cache/port/t29025-native-hud-stage0-source-v1/` pending integration review.

A strict isolated first-prologue HUD provider candidate is recorded under `.claude/orchestrator/state/T-29025/prologue-hud-provider-v1/`. The updated profile SHA-256 is `08e77744ce6a6896b44c466a26c084e278b26e75016b3c38434e136212d65843`. The candidate implements transaction-owned `scene_reset`/`advance_outer`, pure checker/front raster callbacks, source-pinned MAINROM/font resources, and a separate 19-effect actor upload preflight. Its focused Godot suite passes in the existing Docker image; the deterministic isolated render is `review/stage0-provider-candidate.png` (SHA-256 `086ceffe9d58ae495cb948c47f6f3b6f7798a85024aea141ceaa8da40a87eed7`). It is not integrated. P_01_01.BS full scene composition, actual GPU completion, BGM, normal route, and pixel parity remain unsupported.

Follow-up boundary note: `.claude/orchestrator/state/T-29025/prologue-hud-provider-v1/adapter-boundary-v1.md` records implemented isolated model/draw callbacks, source-ordered media upload planning, compositor ownership, and remaining readiness gaps. The first-arrival gate still blocks normal start at the source BGM boundary. Upload preflight requires actual caller-owned VRAM and scratch bytes, preserves supplied STP bits, and keeps readiness false because the native GPU pump is external.


Actor upload status correction: the separate candidate provider requires `O_01.NBN` with authenticated hash and explicit runtime scratch ownership. Palette pointer `0x80184490 + row*512` already corresponds to file offset `0x4490 + row*512`; adding the offset twice is invalid. Effects 0–16 enqueue raw GPU payloads before the grayscale CPU transform reads raw NBN offset `0x4610`; the resulting palette 13 bytes enqueue afterward. The 19 ordered effects are 18 GPU uploads and one immediate CPU transform. Tests cover pointer/offset mapping, payload bytes, and enqueue/CPU phase order. Stage-entry scratch contents remain unknown, so the caller must supply its live bytes; no default is invented. `vram_ready` remains false until a real pump completes.

## Fresh-start integration continuation (2026-10-26)

The sealed two-file entry consumer now has 37 independently reviewed lifecycle checks. The first-arrival owner is composing it with the six entry-context prerequisites, entry/prologue packets and the isolated HUD provider. Root dispatched independent HUD Standards and source/spec reviews against `prologue-hud-provider-v1/review/report.json`. Actual native upload pumping and owned VRAM/scratch propagation remain required; provider callbacks alone do not establish readiness. The media owner is tracing fresh boot/title/entry scratch writes rather than substituting zero STP bits. Original BGM preparation continues separately; a diagnostic render cannot satisfy the production audio gate. No normal-start acceptance yet.

## Fresh cold-start scratch provenance (2026-10-26)

The isolated evidence packet `.claude/orchestrator/state/T-29025/prologue-hud-scratch-flow-v1/` pins source hashes and line spans for the direct cold boot → title New Game → file-select → NAME_ENTRY → first prologue grayscale path. `game_state_init(1)` clears the whole `GameState`; `hud_vram_init` separately zeros all 128 scratch words. File-select's 16-color bar only covers 32 bytes and clears their STP flags; the remaining bytes stay zero on this scoped route. NAME_ENTRY and mode changes do not touch `scratch_words` before the first prologue grayscale transform. This is not a general runtime default: Options can overwrite all 128 words, and `game_mode_set` does not clear the buffer. Keep the upload provider's live caller-owned 512-byte scratch input and preserve supplied STP. VRAM survival and GPU completion remain unproven and owned by the first-arrival media path. Evidence report SHA-256: `9689cdd2f8b8562e37679d2116af6827d3e5e47c7b92bb77e2a38b1a6bfe8ce6`.

## HUD V3 independent correction acceptance (2026-10-26)

Original reviewer accepts the finite V3 correction with zero unresolved findings. Fresh schema330, focused351 (98,304 original-pixel comparisons/zero mismatch), upload290 and the exact preserved independent failure probe101 checks all pass cleanly. Packet `fe90d03adba0ea2a32a4a7a0e30ae9cbc804166e42a0bb6a02e882aa06d6d0cc`; provider `022da585432d352b59d7d60d4cb8a54e40740abfe837db2d6ec5954e3091103b`. Evidence is `.claude/orchestrator/state/T-29025/hud-provider-v3-independent-review/`. Original source resources and valid rendering paths remain unchanged. Ordered isolated composition with entry/context/media/GPU/upload/checker ownership is authorized; actual audio service and normal UI-start verification still gate New Game acceptance. Nothing in this component review establishes a playable fresh start.
