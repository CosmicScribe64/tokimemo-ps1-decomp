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

## Code-review fix pass (2026-11-01)

Findings of the `CODING_STANDARDS.md` review of the BS stills and signs tooling, fixed in place (same ids, same
checkpoint folders):

- Fill provenance (must-fix): `brush.inpaint` no longer adds the residual of a clean donor pixel 3 to 12 px away (the
  P_01_03 crop had 15,862 such pixels). The grain is synthetic: seeded Gaussian noise (seed = first 32 bits of the SHA-256
  of the plan id), blurred to the lag-1 correlation of the clean neighbours' fine-scale residual (below 1.5 px), coloured
  by its covariance, scaled to its local strength; statistics from clean pixels inside the plan rectangle, outliers
  dropped (`grain_model`, `synthetic_grain`). The hard-coded `original_pixels: 0` is gone from both tools; manifests
  record the computed `crop.source_equal_pixels` (stills: 0 for all nine; signs: 0 to 4126, since a plane or diffusion
  face equals a flat original colour in places) and `crop.fill` (method statement; for grain fills the seed, statistics
  and `shift_correlation`, the largest correlation of fill grain and source grain at any shift up to 12 px: 0.10 to 0.31
  now, 0.88 to 1.0 for the old donor fill). Tests recompute both from the crops and the source decode.
- Previews checked at 1x and zoomed against the old crops (scratchpad `art-fix3/stills/round1..3`). Round 1 (statistics
  from every clean neighbour) made the monument noisy; round 2 limited them to the plan rectangle and dropped outliers;
  round 3 took the grain below 1.5 px only, so the stone sheen no longer counts as grain. Sky, stone, plate and chalkboard
  now read as natural. Changed crops: gate plate, roster, monument, chalkboard; unchanged: the four plane fills and
  P_01_03B (flat sky, zero grain). T-39400 sign crops unchanged; their manifest and ledger changed.
- Citations are (file, function, call text, line); `tools/translation_bs_common.py` `check_citations` checks the call
  inside the function body only, the line is kept in manifests for humans.
- Shared code moved to `tools/translation_bs_common.py` (citations, geometry proof, stream facts, macroblocks via
  `translation_bs_drafts.edited_macroblocks` / `bounding`, `rgb8`, previews, `publish`, environment);
  `translation_bs_font` takes an optional `kern` mapping, the stills copies of the kerned layout are gone.
  `translation_bs_drafts.gates` is not reused: it gates a re-encoded splice, and these tools write no BS file.
- Manifests record Pillow 11.1.0 / numpy 2.2.4; the byte-exact publication comparisons skip with a message when the
  running versions differ (the Dockerfile is not changed).
- Nits: `fit_plane` returns the sample count of its final coefficients; the sign class-mark removals say Laplace fill;
  unused `texture`, `STILL_SIZES['P_01_02']`, publish `revision`, side_by_side `rect`, brush `n` removed; font docstring
  corrected; change-history comments and the REGISTER "(done in ...)" note removed; `translation_inventory.py`
  `UNREGISTERED` holds exact ids and the orphan layout comment is gone.
