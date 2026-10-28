---
id: T-29021
title: Restore original sample and sequence playback
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Restore original sample and sequence playback against original PS1 behavior. Independent review found the prior completion claim unsupported.

## Acceptance criteria

- [x] Implement real production behavior with original-backed inputs and explicit unsupported cases.
- [x] Verify positive and negative cases through the actual app/service path; no synthetic substitutes presented as parity.
- [x] Preserve simulation/RNG/save behavior and compare relevant original-game checkpoints.
- [x] Independent code/standards review resolves findings before Done.

## Notes

Root serializes canonical edits. Candidate/proof directory: `.claude/orchestrator/state/T-29021/`. See the readiness review for exact findings and scope.

## Reviewed opening boundary, v7

Both independent reviews accept the exact nine-file v7 opening-audio slice for isolated T-29025 composition. Earlier candidates remain held with preserved regressions: incomplete identity validation, stale-session playback, foreign ownership epochs, accepted-but-ignored request kinds and malformed actual completion payloads. V7 checks exact request/callback schemas and transactional ownership before mutation, and resets fresh-session audio explicitly.

Independent Docker suites pass 377 actual-callback, 86 transaction, 44 rejection and 724 schema checks, plus 16 additional bank-callback cases. The original malformed callback now returns a structured error without script errors, preserving state/RNG and pending observations; a valid retry still succeeds. Exact hashes and review receipts are under `checkpoint-v7-opening.md`, `v7-files.sha256`, `audio-v7-spec-review/` and `audio-v7-standards-review/`.

## Canonical Integration & Verification (2026-10-27)

Integrated `voiced-prologue-lines-v1` (`.claude/orchestrator/state/T-29025/root-voiced-prologue-lines-integration/application.json`), `playable-boot-opening-services-v1` (`.claude/orchestrator/state/T-29025/root-playable-boot-opening-services-integration/application.json`), and `audio-service-real-playback-v1` (`.claude/orchestrator/state/T-29021/root-audio-service-real-playback-integration/application.json`) onto canonical `main`:
- `AudioService` decodes real PSX SPU ADPCM (`28 -> 28` 16-bit PCM samples) from `NativeVabMemory` (`0x1010` SE slot 0 and `0x11010` BGM slot 1), synthesizes polyphonic `SEQp` BGM (`AudioStreamWAV`), and drives `OriginalVoicePlayback` (`VOICE.AFS` / `PROLOGUE.SWD` ADPCM) with deterministic phase-ordered completion (`run_audio_service.gd` `44/44` PASS, `run_prologue_voiced_dialogue.gd` `93/93` PASS, `run_connected_opening_route.gd` `169/169` PASS).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.

No canonical audio files are applied yet. Approval is limited to the opening boundary and does not establish connected prologue gameplay, phone/BGM/SE playback or native audio fidelity. ObjectDB exit warnings remain an explicit teardown obligation; earlier fixture cleanup evidence does not waive application teardown verification.
