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
`tools/translation_inventory.py` (13 candidates).

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
| catalog-147 (C_01_28) | candidate-only, partial | now covers the exam heading and the month / day column: drop the "coverage partial" note and the date-column sentence in the reason; the draft id becomes `bs-c-01-28-final-exams-v2`. The old reason's splice facts (ring 1, 19298 bytes) described the T-39004 re-encoded crop and no longer apply: the new draft is a pixel overlay and the re-encode is the patch step. |
| catalog-143, 139, 145, 144, 149 | candidate-only | keep the status; point the evidence to the `-v2` ids (texture kept) and update the reasons that said the face is rebuilt from a fitted plane. catalog-143 now reads CENTRAL / CITY ART / MUSEUM in three lines (the two-line CENTRAL CITY / ART MUSEUM was cramped); say so wherever the old wording is quoted. |

`decisions/bs-signs.json` (T-39400 record) should get a T-39412 entry with the facts above.

## 4. Alternatives the owner must choose (P_01_03 and P_01_03B)

Every plan in an `alternative_group` is an alternative drawn on the untouched source; register one of each group when the
owner chooses (register none of them until then, or register the recommended pair).

Group `p-01-03` (P_01_03, six calligraphy columns, sky kept; the right-edge column at x >= 303 is cut off by the frame and not
translated):
- `bs-p-01-03-roster-a-rotated` (a): each name, family first, runs down its slanted column as one brush line
  (MOGAMI, MIZUKAWA MIHO, HONDA MEGUMI, FUJISAKI SHIORI, HASHIMOTO YOSHIKO, TANAKA SAKI).
- `bs-p-01-03-roster-b-stacked` (b): family name over given name, horizontal, columns alternate between an upper and a lower
  band so neighbouring stacks never touch. MOGAMI has no given name (cut off).

Group `p-01-03b` (P_01_03B, Fujisaki Shiori alone on white). The game cross-fades P_01_03 to P_01_03B
(`prologue_names_crossfade`) and the name stays in place, so the pair must match:
- recommended with roster a: `bs-p-01-03b-name-a-rotated` (identical line to roster a);
- recommended with roster b: `bs-p-01-03b-name-b-stacked` (identical lines to roster b).
- extra, larger and centred, if the owner prefers a big name over the matching crossfade: `bs-p-01-03b-name-a-large`
  (one rotated line, cap 19 px) and `bs-p-01-03b-name-b-large` (two lines, cap 26 px, centred). These do not line up with
  the P_01_03 name, so the crossfade would show the name growing.

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
