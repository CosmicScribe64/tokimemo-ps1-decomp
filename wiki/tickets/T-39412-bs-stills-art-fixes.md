---
id: T-39412
title: Fix the BS still drafts after the owner art review
status: In Review
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39400-finish-missing-english-art]]", "[[assets-media]]"]
---

## Goal

Act on the owner's art review of the BS background stills: keep the stone texture under the museum monument lettering (and the other small signs), draw the school gate plate, translate the brush-calligraphy roster close-ups, attempt the class roster board, and bring the final-exams chalkboard to chalk size with its date column translated.

## Acceptance criteria

- [x] D_15_03 monument keeps its stone shading and is less cramped; D_03_08, D_24_03, D_19_02 (LOVELY SHOP) and I_17_01 redone without flattening the face.
- [x] P_01_01 gate plate drafted (KIRAMEKI PRIVATE / HIGH SCHOOL, brush lettering, plate and stone untouched), size proved from `prologue.c`.
- [x] P_01_03 / P_01_03B names drafted in two alternatives (rotated, stacked) plus two larger P_01_03B extras; sky kept; geometry proved from `prologue.c`.
- [x] C_01_28 chalkboard: heading at chalk size, month / day column translated, background pixels unchanged outside the strokes.
- [ ] P_01_02 student names: attempted, not legible at game resolution (see the REGISTER note); left untranslated pending the owner.
- [ ] Code review against `CODING_STANDARDS.md`; the integrator registers the ledger (`REGISTER.md` in the checkpoint).

## Notes

Tools: `tools/translation_bs_stills.py` (plans, build, publish), `tools/translation_bs_brush.py` (texture-keeping erase, brush stroker), tests in `tools/test_translation_bs_stills.py`. Checkpoint: `translations/en/graphics/artwork/checkpoints/T39410-stills-fixes-v1/` (13 RGBA crops, alpha 255 only on port-made pixels, so no original pixel is committed; `REGISTER.md` lists what the integrator changes). The shared registration files were not touched.
