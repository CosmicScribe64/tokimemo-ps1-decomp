# T39410-stills-fixes-v1: what the integrator must register

Ticket T-39412. Drafts only: port-made, unreviewed, `human_approved: false`, nothing integrated or checked in the game.
Rebuild with `tools/translation_bs_stills.py build --output .asset-cache/<new dir>` (Docker, disc mounted); the manifest hash
in `ledger.json` and the crop hashes are reproduced by `tools/test_translation_bs_stills.py`.

## Crop format (new)

Every crop is an RGBA PNG of the plan rectangle. Alpha 255 = port-made pixel, write it over the decoded still; alpha 0 =
keep the source pixel (RGB is 0 there and carries no picture data). So the crops hold no original pixel and the stone,
paper, sky and chalkboard texture comes from the source decode at patch time. The packer must composite, not replace the
rectangle. The T-39400 crops were opaque RGB rectangles; the five replaced ones below are RGBA.

## 1. Ledger to add

Add `translations/en/graphics/artwork/checkpoints/T39410-stills-fixes-v1/ledger.json` to `LEDGERS` in
`tools/translation_inventory.py` (9 candidates since the second pass; done in d38555585). The four rejected roster
alternatives are deleted from the checkpoint, so the ledger needs no `UNREGISTERED` skip any more (removed).

## 2. Supersedes (leave these ledger rows out, as the 3-wide-M and Clear drafts were)

| Replaced id | Checkpoint | New id |
| --- | --- | --- |
| `bs-d-15-03-monument` | `T39400-bs-signs-v1` | `bs-d-15-03-monument-v2` |
| `bs-d-03-08-march-placard` | `T39400-bs-signs-v1` | `bs-d-03-08-march-placard-v2` |
| `bs-d-24-03-ticket-plaque` | `T39400-bs-signs-v1` | `bs-d-24-03-ticket-plaque-v2` |
| `bs-d-19-02-maiko-board` | `T39400-bs-signs-v1` | `bs-d-19-02-maiko-board-v2` |
| `bs-i-17-01-hanging-sign` | `T39400-bs-signs-v1` | `bs-i-17-01-hanging-sign-v2` |
| `bs-c-01-28-final-exams` | `T39004-bs-v1` | `bs-c-01-28-final-exams-v2` |

Add the five T-39400 ids to an `UNREGISTERED`-style skip for that ledger (prefix match is enough) and the T39004-bs-v1 id
`bs-c-01-28-final-exams`. The other T-39400 crops (the 12 other sign crops incl. the six P_01_02 header marks) stay. The
T39004-bs-v1 `all-c-banner` row is unrelated and stays.

## 3. Catalog / inventory changes

| Catalog item | Now | Change |
| --- | --- | --- |
| catalog-129 (P_01_01 gate plate) | deliberately-untranslated | `candidate-only`; reason: owner asked for it 2026-11-01 (note 19); draft `bs-p-01-01-gate-plate`, brush lettering KIRAMEKI PRIVATE / HIGH SCHOOL on the white plate, plate border and stone untouched. Delete the "retention is a style decision" reason. |
| catalog-131 (P_01_03, P_01_03B) | deliberately-untranslated | `candidate-only`; reason: owner note 21; calligraphy names drafted in two alternatives for the owner to pick (see section 4). Delete the "new art, not lettering" reason. Also fix the catalog row text: it reads 水川 めぐみ / 最上 美穂; the picture reads 水川美穂 (Mizukawa Miho) and 最上 with the given name cut off by the left edge (Mogami), 本田めぐみ (Honda Megumi). |
| catalog-130 (P_01_02) | candidate-only (header marks only) | scope stays as is: the student names were attempted and are not legible enough, see section 5. Add a note, do not mark partial/complete beyond the header marks. |
| catalog-147 (C_01_28) | candidate-only, partial | now covers the exam heading and the date column (月 / 日 / ︵︶ became a blank date `( ) / ( )`, second pass; catalog row text updated): drop the "coverage partial" note and the date-column sentence in the reason; the draft id becomes `bs-c-01-28-final-exams-v2`. The old reason's splice facts (ring 1, 19298 bytes) described the T-39004 re-encoded crop and no longer apply: the new draft is a pixel overlay and the re-encode is the patch step. |
| catalog-143, 139, 145, 144, 149 | candidate-only | catalog-139 now reads KIRAMEKI / HIGH in two lines (second pass; catalog row text updated). Keep the status; point the evidence to the `-v2` ids (texture kept) and update the reasons that said the face is rebuilt from a fitted plane. catalog-143 now reads CENTRAL / CITY ART / MUSEUM in three lines (the two-line CENTRAL CITY / ART MUSEUM was cramped); say so wherever the old wording is quoted. |

`decisions/bs-signs.json` (T-39400 record) should get a T-39412 entry with the facts above.

## 4. P_01_03 and P_01_03B: layout A (owner decision 2026-11-01)

The owner chose layout A: `bs-p-01-03-roster-a-rotated` (each name, family first, runs down its slanted column as one brush
line: MOGAMI, MIZUKAWA MIHO, HONDA MEGUMI, FUJISAKI SHIORI, HASHIMOTO YOSHIKO, TANAKA SAKI) and `bs-p-01-03b-name-a-rotated`
(the identical FUJISAKI SHIORI line on P_01_03B, so the `prologue_names_crossfade` keeps the name in place). The stacked
and large alternatives were rejected and are deleted from this checkpoint. The right-edge column at x >= 303 is cut off by
the frame and not translated.

## 4a. Second pass (owner and reviewer notes, 2026-11-01)

| Draft | Change |
| --- | --- |
| `bs-d-03-08-march-placard-v2` | Two lines KIRAMEKI / HIGH in 5 px pixel letters, black, 1 px margins on the 35 x 13 white face (rect widened to [211, 57, 35, 13] so the last Japanese column at x 244, the earlier stray dot, is erased); the A is a real A. |
| `bs-i-17-01-hanging-sign-v2` | FINE ARTS in 6 px pixel letters, near-black like the kanji, 1 px letter gaps (I and N apart), 2 px margins on all sides. |
| `bs-d-19-02-maiko-board-v2` | LOVELY SHOP in 5 px pixel letters with 1 px gaps and a 3 px word space, pink of the strongest Japanese pixels; rect [136, 41, 44, 5]; every band pixel darker than a channel sum of 84 erased (the faint long-vowel bar was left by the relative rule). |
| `bs-p-01-01-gate-plate` | One face, size and set width for both lines; each line centred between the plate's side edges at its height (equal clearance, at least 7 px); broad-nib brush strokes (thick down-strokes, thin cross-strokes, tapered ends); the fill now takes only plate pixels. |
| `bs-p-01-03-roster-a-rotated`, `bs-p-01-03b-name-a-rotated` | Same broad-nib brush, lighter and more open (cap 13, gap 4) so I and M no longer merge; MOGAMI and MIZUKAWA moved in from the top-left corner (at least 3 px clear of every edge); ink is each still's own dark grey ([5, 5, 6] on P_01_03, [6, 6, 6] on P_01_03B) instead of black. The P_01_03B line is still identical in position to the roster line. |
| `bs-c-01-28-final-exams-v2` | The date column (月, 日 and the vertical weekday brackets ︵︶, the last one cut by the chalk ledge: punctuation, not a stray glyph) is erased and redrawn as a blank date `( ) / ( )` read top to bottom in chalk; the fill uses board pixels only. FINAL / EXAMS unchanged. |
| `bs-d-24-03-ticket-plaque-v2` | Unchanged: TICKETS does not fit. The face is 20 px wide; seven 3 px letters with 1 px gaps need 25 px, and the skeleton face at 18 px gives 2.6 px a letter, which does not read. |
| `bs-d-15-03-monument-v2` | Unchanged (CENTRAL / CITY ART / MUSEUM, owner). |

Third pass (final reviewer nits): monument remnant left of CENTRAL (top of 市, x 229-234) force-erased and refilled
without grain; LOVELY SHOP narrowed (2 px L, 2 px word space) to 3 px clear at each end; gate plate letters enlarged
(cap 13.5, leading 14; ink at least 6.7 px from every plate edge) with A-T and V-A kerning (PRIVATE); P_01_03 sky at the
cut left-edge glyph refilled without grain (no streaks). New erase key `smooth` (rectangles filled without donor grain).

## 5. P_01_02 student names: not done, with evidence

The board names are family and given names of about 5 x 6 px per character, blurred together at the macroblock level;
two-character family names stack in 5 px columns and nothing there can be read reliably. I read at most 最上, 水川 and
本田 in the 1-A block by inference from the close-ups, and a template match of the P_01_03 columns (rotated and scaled to
board size) against the board gave no unique peak (z about 4 for every name, all at the same texture patch). Replacing
names by inference would be invention, so no name is replaced; the readable names are in the catalog-131 close-ups. The
paper texture and the 1- marks are untouched. Decision to record: `P_01_02 student names: not legible at game
resolution, left as picture texture` under catalog-130.

## 6. Geometry (new proofs in the manifest)

P_01_01 (320x160): `prologue.c:224` reads LBA 0x494B (= 0x4954 - 9 sectors), `prologue.c:233` decodes with
`dec_bg_decode_to_vram(1)`. P_01_03: `prologue.c:413` / `:425`. P_01_03B: `prologue.c:463` / `:472`. C_01_28 (320x88):
`school_day.c:126` reads, `school_day.c:178` decodes with `dec_bg_decode_to_vram(0)` (`background_images.c`
`bg_still_decode_to_vram(0x2C0, 0xA0, 0x140, 0x58)`), 120 macroblocks plus end marker. Every stream's macroblock count
equals (width/16) x ceil(height/16) and the end marker follows. P_01_01 and P_01_03B carry zero padding and a 4-byte trailer after the end marker
(1880 and 348 trailing bytes, 4 non-zero), recorded in the manifest as `trailing_nonzero_bytes`; the T-39400 claim that only
zero bytes follow holds for the other files.
