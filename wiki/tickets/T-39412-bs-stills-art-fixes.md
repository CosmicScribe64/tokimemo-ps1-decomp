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

Tools: `tools/translation_bs_stills.py` (plans, build, publish), `tools/translation_bs_brush.py` (texture-keeping erase, brush stroker), tests in `tools/test_translation_bs_stills.py`. Checkpoint: `translations/en/graphics/artwork/checkpoints/T39410-stills-fixes-v1/` (9 RGBA crops since the second pass, alpha 255 only on port-made pixels, so no original pixel is committed; `REGISTER.md` lists what the integrator changes). The shared registration files were not touched.

## Second pass (2026-11-01): owner and reviewer notes

Owner chose layout A for the close-ups; the stacked and large alternatives are deleted from the checkpoint (and their
`UNREGISTERED` skip from `tools/translation_inventory.py`). Changes, all in the same checkpoint and with the same ids:

- D_03_08 placard: KIRAMEKI / HIGH in two lines of 5 px pixel letters. The old stray dot was a Japanese pixel at x 244,
  outside the old rectangle; the rectangle now covers the whole white face.
- I_17_01: FINE ARTS in 6 px pixel letters, near-black, 1 px letter gaps, 2 px margins.
- D_19_02: LOVELY SHOP in 5 px pixel letters with 1 px gaps; the faint Japanese is erased by an absolute rule.
- P_01_01 gate plate: both lines in one face and set width, each centred between the plate's side edges (equal clearance
  at the top and bottom of the letters, at least 7 px); broad-nib brush (`nib` option of `brush_coverage`); fill from plate
  pixels only.
- P_01_03 / P_01_03B: same broad-nib brush, lighter and more open; MOGAMI and MIZUKAWA moved in from the top-left; ink is
  each still's own dark grey. P_01_03B keeps the roster line's position.
- C_01_28: the date column (月 / 日 / vertical weekday brackets ︵︶; the cut glyph at the ledge is the closing bracket)
  becomes a blank date `( ) / ( )` in chalk; fill from board pixels only.
- D_24_03 stays TICKET: TICKETS needs 25 px in 3 px letters on a 20 px face.

New code: pixel-font runs (`PIXEL_FONT`, `pixel_rows`, `pixel_coverage`), `fill_from` and polygon-limited fill
(`brush.inpaint(exclude=...)`), gate centring (`edge_span`, `line_margins`, `gate_centres`). Catalog rows catalog-139 and
catalog-147 reworded (jsonl and md). `translation_inventory.json` is not regenerated here: its matches-inputs test fails
until the integrator rebuilds it. Tests: `tools/test_translation_bs_stills.py` and `tools/test_translation_bs_signs.py`
59 OK; translation discovery 351 run, failures only the inventory matches-inputs test and the `test_translation_tree` and
`test_translation_filesel_drafts` failures that HEAD has too.

## Third pass (2026-11-01)

Third pass (final reviewer nits): monument remnant left of CENTRAL (top of 市, x 229-234) force-erased and refilled
without grain; LOVELY SHOP narrowed (2 px L, 2 px word space) to 3 px clear at each end; gate plate letters enlarged
(cap 13.5, leading 14; ink at least 6.7 px from every plate edge) with A-T and V-A kerning (PRIVATE); P_01_03 sky at the
cut left-edge glyph refilled without grain (no streaks). New erase key `smooth` (rectangles filled without donor grain).
