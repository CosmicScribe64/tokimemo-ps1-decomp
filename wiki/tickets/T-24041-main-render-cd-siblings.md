---
id: T-24041
title: Recover MAIN renderer and CD dispatcher
status: Done
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-20
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

## Independent root combined verification

Production changes reviewed against CODING_STANDARDS.md: existing SDK/aggregate types and necessary declaration views retained, new LINE_G2 layout documented, matching-specific forms explicitly marked FAKE, no per-function compiler exception. All new targets lack assembly fallbacks and pass normal linked byte proofs. Combined integration passes all27 original binary hashes and header/global/prototype guards. No unresolved findings for the verified matches. Published on main after all checks; unresolved siblings remain excluded.
## Closure

Renderer final guards pass: target source gate, normal default literal MATCH348 B, MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26, headers/globals/main_api and whitespace. Renderer win pinned e33246ea20de74a06e9f57b51379b3bf708c79a1.

CD dispatcher is source-only, production unchanged. The repaired foundation scores512 with frame104 vs target80; removing artificial temporaries scores499 with frame88. Correct typed ring indexing, full word payload-header read, four-byte location/eight-byte response locals, retry-loop cleanup, and exact exhausted hypotheses are in drafts/T-24041. No speculative CD header edits. Bounded pass closed; independent review pending for code and checkpoint.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
