---
id: T-39410
title: Fix the MAINROM English art after the owner review
status: In Review
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39400-finish-missing-english-art]]", "[[tickets/T-39009-mainrom-indexed-drafts]]", "[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Redraw the MAINROM items the 2026-11-01 owner and reviewer notes sent back (Save labels, magazine banners, issue banners, genre tags, stat labels, game covers), fix the stale weekday bindings, and move the text dictionary to the approved stat names.

## Acceptance criteria

- [x] Save labels 57-59: "Save" in the Japanese rows on the lettering layer; the card layer (frame border and icon) is untouched; all six palette variants.
- [x] Banners 126-133: one bold 17-row face at one size, one wording rule (完成 Opens, 特集 Special, 紹介 Guide), one file per variant.
- [x] Issue banners 122-125: season name over ISSUE restores 月号 (round 2; round 1 used even digits plus "Issue").
- [x] Genre tags 134-145: 1 px margin on both sides, centred (owner decision 2026-11-01); `TAG_ALIGN` is the one switch.
- [x] Weekdays 82-88: `weekday-resolution.json` shows every variant resolves to the approved two-letter set; the twelve stale three-letter drafts are listed for the integrator to supersede.
- [x] Stat labels 61: Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress, left-aligned, clear of the arrow marker.
- [x] Covers 151-154: English titles (SUIKODEN, PARODIUS, POWER / PROS, TWINBEE / PUZZLE; round 2) in each logo's colours; the rest of the cover art is unchanged.
- [x] Text dictionary: Health, Humanities, Sports in the three stat records and their three executable-offset twins.
- [x] Drafts reproducible with `tools/translation_mainrom_fixes.py`, tests in `tools/test_translation_mainrom_fixes.py`, checkpoint `T39410-mainrom-fixes-v1` with ledger and `REGISTER.md`; no shared registration file edited.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

- Checkpoint: `translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1` (192 PNGs, 32 frames x 6 variants). The integrator step is in its `REGISTER.md`.
- The SLPM tables the translation tree builds from are gitignored (`SLPM_*`), so the text change was made in `translations/en/returned-v3.jsonl` directly. The inventory pins that file's hash, so `test_translation_inventory` fails until `translation_inventory.json` is rebuilt (REGISTER.md).
- All new lettering is port-made and unreviewed; nothing is approved or integrated. Round 2 made the covers bolder (see below).

## Round 2 (second review, 2026-11-01)

Owner decisions from chat and the second independent review (`scratchpad/art-qa2/notes.md`, MAINROM items). The checkpoint `T39410-mainrom-fixes-v1` was updated in place: same candidate ids and `supersedes` list, new hashes for the 17 changed frames (0122-0133, 0144, 0151-0154; 102 files). Save labels, stat labels and the other tags are byte-identical.

- 0144 Pro Wrestling: now "Wrestling" on one line, rows 4-11 like 0145 "Baseball", so 4 px clear on top and bottom (the original keeps 1 and 2) and 1 px each side. The W is the 5 px port-made W. "Pro / Wrestling" cannot fit: two 7-row lines with shadows need 16 rows. A 5 px W also leaves "Wrestling" 2 px too wide even with narrow e, s, n, g and r. The fix: a 2 px t with its crossbar on the left, and the pairs r-e and s-t kerned to 0 px (`KernedFace`).
- Headlines 0126-0133: flush left at the column where the Japanese starts (x 0 in all eight); same face and size. `KARAOKE BOX OPENS` fits 0132 at the shared size (113 of 152 px).
- 0128: the Q has a tail. It leaves the bowl at the bottom right and runs 2 rows below the baseline (shadow on row 22).
- Edge tones (nit 14): each banner's three extra tones (highlight = fill - 1, mid = fill + 1, soft = shadow + 1 in every banner CLUT) go on the cut corners and shadow steps. That is 18-29 px per banner (the Japanese has 20-60). The manifest lists the count for each banner.
- Issue badges 0122-0125: SPRING / SUMMER / AUTUMN / WINTER over ISSUE, in two 6-row bold lines on rows 1-6 and 9-14, the rows the Japanese uses. The face is a wide 2 px-stem `bold6w`, one size for all four; SUMMER and AUTUMN set the width (44 of 56 px). Each line carries the badge's ramp, dark at its edges and bright in the middle.
- Covers: SUIKODEN uses 8-row semi-bold capitals (4 px wide, 2 px main stem) with the logo's ramp, and the GENSOSUIKODEN subtitle stays. PARODIUS uses 10-row semi-bold capitals, yellow to orange with a white outline and a navy shadow. POWER / PROS (owner decision; was PRO BASEBALL) uses two lines of 2 px-stem capitals, yellow on a red outline with a green shadow, left-aligned and 6 px clear of the baseball. TWINBEE / PUZZLE uses 7-row and 6-row 2 px-stem capitals with the navy outline. Eight 5 px letters do not fit the logos' 40 px, so the two eight-letter titles are semi-bold, not full 2 px. `plan_cover` refuses any title pixel within 1 px of kept artwork.
- Catalog: `image_text_catalog.jsonl` and `.md` rows 0122-0125 (season wording), 0132 (KARAOKE BOX OPENS) and 0153 (Power Pros) were edited together. The 0144 suggestion column still says Wrestl. / ProWres; it was not edited (out of scope for this round).
- `superseded_candidates` also reads the inventory's `superseded_records` for this ledger, so the rebuilt `supersedes` list matches the registered one now that the checkpoint is in the inventory.
- `translation_inventory.json` is not rebuilt here: `test_committed_inventory_matches_inputs` fails until the integrator regenerates it. `test_translation_tree` errors in this worktree because the gitignored `translations/en/system/dictionaries/slpm_86_053.md` is missing; the catalog edits did not cause that error.

- Variant colour audit (coordinator request): 0061 icon-51 was reported with light English labels. The PNG and the preview pixels are black (080808, index 9 through the icon-51 CLUT, as the Japanese uses), so there was nothing to fix. The light look was an artefact of viewing the downscaled preview sheet. New build test `test_every_variant_uses_only_the_colours_of_its_japanese`: for all 192 frame and variant files, the colours inside the edit regions are a subset of the original's colours in the same regions. There are 0 violations.

## Round 3 (final review, 2026-11-01)

QA pass 3 (`scratchpad/art-qa3/notes.md`) left one should-fix and two nits. The checkpoint was updated in place; only the six 0144 files changed.

- 0144: the t was full height with a left-only crossbar fused to the s ("stl" read "sll"). It is now the family's t (centred 3 px crossbar, hooked foot), one row shorter than the l. The s has a flat one-pixel top, so the t's left arm keeps 1 px clear of it, corners included (s-t kerned to 0 px with no contact). With every letter at its narrowest legible form and a 5 px W, the ink is exactly 30 px. To keep col 31 clear, the g's 4 right-edge shadow pixels are clipped (`clip_shadow`, recorded as `clipped_shadow_pixels`). A two-sided t with clean gaps cannot reach 30 px including the shadow: e, s, n and g are already 3 px, and the only kerning pairs without contact are r-e and s-t.
- REGISTER.md now records that icon-51 STP follows the colour (0 ambiguous colours across 32 frames, QA pass 3), which closes G1.
- The 0144 catalog suggestion (`.jsonl` and `.md`) is now "Wrestling".
- `translation_inventory.json` is not touched here, so `test_committed_inventory_matches_inputs` fails until it is regenerated. The other tests pass: 128 run, including the reproduction and variant colour audit.

## Comments

### 2026-11-01 worker agent: code review fixes

Findings from the code review of `tools/translation_mainrom_fixes.py` and its checkpoint; no PNG changed (all 192 are byte-identical, checked by rebuilding and diffing).

- Original pixels (must-fix). The ledger said `original_images_included: false`, but every PNG is a whole frame render that keeps the original art outside its edit regions (for example the covers 151-154 keep the cover art outside the title rows). The earlier ledger flag meant "no unmodified source export", which was never defined. Now `ledger.json` says `original_images_included: true`, carries `source_pixels_note`, and each asset records `source_pixels` (opaque texels, and how many equal the source texel at the same position, from `translation_assets.source_pixels`). `REGISTER.md` section 5 lists the counts per frame.
- Basis for committing whole frames: the owner asked for translated assets to be kept in the repository (`translations/en/graphics/artwork/checkpoints/T39004-2026-10-29/README.md`, "Published at the user's explicit request", "English derivatives ... retain source artwork"; [[tickets/T-39004-english-asset-completion]], the publication of 2026-10-29; `publication_authorization` in the T39004 ledgers). The earlier MAINROM drafts it supersedes are whole frames too. Counter-example: in T-39004 the frame-60 composites were cut down to the three edited parts after a review (CODING_STANDARDS section 10); that was a 40x139 composite of 3,243 original pixels out of 3,319, not a small frame. The art is not re-masked here; whether the whole-frame practice holds for the final release is the owner's call.
- `REGISTER.md` is generated from data (supersedes, stale weekday drafts, drawn text per frame, tag settings, source-pixel counts). The reasoning and dated notes that the old 80-line text carried stay in this ticket: the text dictionary edit (item 8) and the inventory hash pin are in the notes above; the icon-51 STP finding is in Round 3; the catalog rows (0122-0125, 0132, 0140, 0144, 0153) were edited in rounds 2 and 3.
- Tool: change-history comments removed (round and item numbers); `round2_faces` is now `small_bold_faces`; `publish` stages in a `.partial` folder and renames; tests: the palette-count test is renamed to what it checks (the first twelve dimensions; the palette check is `test_every_variant_uses_only_the_colours_of_its_japanese`), the pinned `len(stale) == 12` is explained, and a build test reproduces ledger, manifest, `weekday-resolution.json` and `REGISTER.md` byte for byte.
- `test_committed_inventory_matches_inputs` pins the ledger hash and fails until `translation_inventory.json` is regenerated after the merge.
