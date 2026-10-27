---
id: T-29027
title: Render original sequences and SPU instruments
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[port-readiness-review]]", "[[tickets/T-29021-restore-original-audio-playback]]"]
---

## Goal

## Bounded runtime implementation decision (2026-10-26)

Root authorized an isolated instance-owned C++ GDExtension behind typed GDScript for the audio DSP hot loop. This is a proposed audio-only exception to the standard-GDScript plan, pending independent implementation/license review before canonical integration. Use the existing Linux arm64 Docker Godot for initial execution and timing; macOS arm64 release qualification remains separate. Keep original instructions as user-supplied data and do not copy GPL diagnostic DSP into production. The design requires private engine state, real sink reservation before frame commit, epoch/reset isolation and measured per-block deadlines. No runtime capability is accepted from the reference WAV. Design: `.claude/orchestrator/state/T-29027/runtime-backend-v1/DESIGN.md`.

## Original goal

Render original BGM and effects from validated native SEQ/VAB bindings, pitches, loops, envelopes and scheduler semantics. Generated substitute tunes are not acceptable.

## Acceptance criteria

- [ ] Native-backed bank/sequence/program/sample binding and timing, with explicit unsupported behavior.
- [ ] Real original PCM output, loop/envelope/pitch/volume verification and bounded emulator comparison.
- [ ] Runtime service integration without replacing CD/voice or falsely signaling readiness.
- [ ] Independent review before Done.

## Source-only DSP correction reviewed

V2 diagnostic packet `8a9bd91a43c36153d65231556fa4d049b7745ebedfae1aac044e0db76a28d294` resolves the nonportable reverb-field traversal. Original independent reviewer verifies90 V2 files, unchanged85 V1 files, a fresh library rebuild,2,208 compiled field-order checks and normal/optimized provider suites. Full author140-second plus tail output hash is independently checked, not independently rerendered. Review handoff `7ce60310965bd6092440efc5be03b62c9aed2c65be047ac6a27190ceaec56eda`. Diagnostic scope accepted only; instance ownership, production backend/licensing/platform, block deadlines, native PCM phase and actual service/epoch integration remain open. Owner is moving to the isolated usable first-arrival backend slice rather than promoting the reference WAV to a runtime capability.
