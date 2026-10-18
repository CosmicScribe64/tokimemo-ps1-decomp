---
id: T-24041
title: Recover MAIN renderer and CD dispatcher
status: In Progress
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-18
links: ['[[main-family-results]]']
---

## Goal

Recover func_80049FF0 (348 B) in src/main/80049FF0.c using matched render siblings; then a novel bounded pass on func_800455C4 in src/main/800451D0.c. One compiler, twenty-five minutes.

## Acceptance criteria

- No target INCLUDE_ASM for any claimed C win, normal default literal proof and MAIN checksum.
- Preserve useful negative source checkpoints; independent review before Done.

## Notes

Read earlier T-9530/T-10030 renderer negatives (58+ edit baselines), T-12010 CD repair (533 listing lines), and T-17000 typed drafts. Shared header changes require prior exact type notice to root.

## Renderer win

func_80049FF0 is an unguarded348 B normal default linked literal match; full MAIN checksum passes. Recovered SDK LINE_G2 packet is exactly0x14 bytes, with offsets established by target allocation/stores. Root approved adding the missing canonical SDK type; no bundled libgpu.h reference was available under Docker /opt or repository raw/tools, so no SDK header text was copied. Existing psx_types.h reconstruction convention supplies tag/coordinate field widths. Meaningful GameState pointer and documented explicit colour masks/comma lines reproduce scheduling. K&R narrow parameter definition preserves existing unprototyped caller ABI and passes header gate; main_api.h unchanged.

The large CD dispatcher remains in progress; no claim yet. Renderer self-review: C89, shared SDK type, explicit FAKE provenance, no asm/toolchain exceptions. Independent root review pending.
