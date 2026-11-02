# T39410 register note for the integrator

Every PNG is an unreviewed draft (`human_approved: false`), one file per palette variant. This note holds the
data the integrator needs; why each item is drawn as it is lives in `wiki/tickets/T-39410-mainrom-art-fixes.md`.
To rebuild, run `python3 tools/translation_mainrom_fixes.py --output .asset-cache/<new dir> [--publish <new dir>]`
in the Pillow Docker image.

## 1. Ledger and supersedes (translation_inventory.py / inventory_decisions.json)

Register `translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1/ledger.json` as a ledger and bind
`mainrom-fix-NNNN.<variant>` to the item `mainrom-NNNN` (frame NNNN) for NNNN in 0057, 0058, 0059, 0061, 0122, 0123, 0124, 0125, 0126, 0127, 0128, 0129, 0130, 0131, 0132, 0133, 0134, 0135, 0136, 0137, 0138, 0139, 0140, 0141, 0142, 0143, 0144, 0145, 0151, 0152, 0153, 0154. `candidate_key` needs
the id pattern `mainrom-fix-<frame>.<palette_id>`. The ledger's `supersedes` list names the 165 earlier
candidates these files replace (each asset also lists the ones for its own variant). Mark each `superseded`
with `superseded_by` this ledger:

- mainrom-0057: mainrom-draft-0057.base, mainrom-draft-0057.hud-43, mainrom-draft-0057.hud-44, mainrom-draft-0057.icon-51, mainrom-draft-0057.icon-52, mainrom-draft-0057.icon-53
- mainrom-0058: mainrom-draft-0058.base, mainrom-draft-0058.hud-43, mainrom-draft-0058.hud-44, mainrom-draft-0058.icon-51, mainrom-draft-0058.icon-52, mainrom-draft-0058.icon-53
- mainrom-0059: mainrom-draft-0059.base, mainrom-draft-0059.hud-43, mainrom-draft-0059.hud-44, mainrom-draft-0059.icon-51, mainrom-draft-0059.icon-52, mainrom-draft-0059.icon-53
- mainrom-0061: legacy-rebind-parameters-panel-296x67, mainrom-draft-0061.hud-43, mainrom-draft-0061.hud-44, mainrom-draft-0061.icon-51, mainrom-draft-0061.icon-52, mainrom-draft-0061.icon-53
- mainrom-0122: mainrom-draft-0122.base, mainrom-draft-0122.hud-43, mainrom-draft-0122.icon-51
- mainrom-0123: mainrom-draft-0123.base, mainrom-draft-0123.hud-43, mainrom-draft-0123.hud-44, mainrom-draft-0123.icon-51, mainrom-draft-0123.icon-52, mainrom-draft-0123.icon-53
- mainrom-0124: mainrom-draft-0124.base, mainrom-draft-0124.hud-43, mainrom-draft-0124.icon-51
- mainrom-0125: mainrom-draft-0125.base, mainrom-draft-0125.hud-43, mainrom-draft-0125.hud-44, mainrom-draft-0125.icon-51, mainrom-draft-0125.icon-52, mainrom-draft-0125.icon-53
- mainrom-0126: legacy-rebind-central-park-guide-136x24, mainrom-draft-0126.hud-43, mainrom-draft-0126.icon-51
- mainrom-0127: mainrom-draft-0127.base, mainrom-draft-0127.hud-43, mainrom-draft-0127.hud-44, mainrom-draft-0127.icon-51, mainrom-draft-0127.icon-52, mainrom-draft-0127.icon-53
- mainrom-0128: mainrom-draft-0128.base, mainrom-draft-0128.hud-43, mainrom-draft-0128.icon-51, mainrom-draft-0128.icon-52, mainrom-draft-0128.icon-53
- mainrom-0129: mainrom-draft-0129.base, mainrom-draft-0129.hud-43, mainrom-draft-0129.icon-51
- mainrom-0130: mainrom-draft-0130.base, mainrom-draft-0130.hud-43, mainrom-draft-0130.hud-44, mainrom-draft-0130.icon-51, mainrom-draft-0130.icon-52, mainrom-draft-0130.icon-53
- mainrom-0131: mainrom-draft-0131.base, mainrom-draft-0131.hud-43, mainrom-draft-0131.icon-51, mainrom-draft-0131.icon-52, mainrom-draft-0131.icon-53
- mainrom-0132: mainrom-draft-0132.base, mainrom-draft-0132.hud-43, mainrom-draft-0132.icon-51
- mainrom-0133: mainrom-draft-0133.base, mainrom-draft-0133.hud-43, mainrom-draft-0133.hud-44, mainrom-draft-0133.icon-51, mainrom-draft-0133.icon-52, mainrom-draft-0133.icon-53
- mainrom-0134: mainrom-draft-0134.base, mainrom-draft-0134.hud-43, mainrom-draft-0134.hud-44, mainrom-draft-0134.icon-51, mainrom-draft-0134.icon-52
- mainrom-0135: mainrom-draft-0135.base, mainrom-draft-0135.hud-43, mainrom-draft-0135.hud-44, mainrom-draft-0135.icon-51, mainrom-draft-0135.icon-52
- mainrom-0136: mainrom-draft-0136.base, mainrom-draft-0136.hud-43, mainrom-draft-0136.hud-44, mainrom-draft-0136.icon-51, mainrom-draft-0136.icon-52
- mainrom-0137: mainrom-draft-0137.base, mainrom-draft-0137.hud-43, mainrom-draft-0137.hud-44, mainrom-draft-0137.icon-51, mainrom-draft-0137.icon-52
- mainrom-0138: mainrom-draft-0138.base, mainrom-draft-0138.hud-43, mainrom-draft-0138.hud-44, mainrom-draft-0138.icon-51, mainrom-draft-0138.icon-52
- mainrom-0139: mainrom-draft-0139.base, mainrom-draft-0139.hud-43, mainrom-draft-0139.hud-44, mainrom-draft-0139.icon-51, mainrom-draft-0139.icon-52
- mainrom-0140: mainrom-draft-0140.base, mainrom-draft-0140.hud-43, mainrom-draft-0140.hud-44, mainrom-draft-0140.icon-51, mainrom-draft-0140.icon-52
- mainrom-0141: mainrom-draft-0141.base, mainrom-draft-0141.hud-43, mainrom-draft-0141.hud-44, mainrom-draft-0141.icon-51, mainrom-draft-0141.icon-52
- mainrom-0142: mainrom-draft-0142.base, mainrom-draft-0142.hud-43, mainrom-draft-0142.hud-44, mainrom-draft-0142.icon-51, mainrom-draft-0142.icon-52
- mainrom-0143: mainrom-draft-0143.base, mainrom-draft-0143.hud-43, mainrom-draft-0143.hud-44, mainrom-draft-0143.icon-51, mainrom-draft-0143.icon-52
- mainrom-0144: mainrom-draft-0144.base, mainrom-draft-0144.hud-43, mainrom-draft-0144.hud-44, mainrom-draft-0144.icon-51, mainrom-draft-0144.icon-52, mainrom-draft-0144.icon-53
- mainrom-0145: mainrom-draft-0145.base, mainrom-draft-0145.hud-43, mainrom-draft-0145.hud-44, mainrom-draft-0145.icon-51, mainrom-draft-0145.icon-52, mainrom-draft-0145.icon-53
- mainrom-0151: mainrom-draft-0151.base, mainrom-draft-0151.hud-43, mainrom-draft-0151.hud-44, mainrom-draft-0151.icon-51, mainrom-draft-0151.icon-52, mainrom-draft-0151.icon-53
- mainrom-0152: mainrom-draft-0152.base, mainrom-draft-0152.hud-43, mainrom-draft-0152.hud-44, mainrom-draft-0152.icon-51, mainrom-draft-0152.icon-52, mainrom-draft-0152.icon-53
- mainrom-0153: mainrom-draft-0153.base, mainrom-draft-0153.hud-43, mainrom-draft-0153.hud-44, mainrom-draft-0153.icon-51, mainrom-draft-0153.icon-52, mainrom-draft-0153.icon-53
- mainrom-0154: mainrom-draft-0154.base, mainrom-draft-0154.hud-43, mainrom-draft-0154.hud-44, mainrom-draft-0154.icon-51, mainrom-draft-0154.icon-52, mainrom-draft-0154.icon-53

The manifest's `identical_variant_outputs` names the variants whose files are byte-identical because their
source image is: 0057 (hud-43 = hud-44), 0058 (hud-43 = hud-44), 0059 (hud-43 = hud-44), 0122 (hud-43 = hud-44 = icon-52 = icon-53), 0124 (hud-43 = hud-44 = icon-52 = icon-53), 0126 (hud-43 = hud-44 = icon-52 = icon-53), 0128 (hud-43 = hud-44), 0129 (hud-43 = hud-44 = icon-52 = icon-53), 0131 (hud-43 = hud-44), 0132 (hud-43 = hud-44 = icon-52 = icon-53), 0134 (base = icon-53), 0135 (base = icon-53), 0136 (base = icon-53), 0137 (base = icon-53), 0138 (base = icon-53), 0139 (base = icon-53), 0140 (base = icon-53), 0141 (base = icon-53), 0142 (base = icon-53), 0143 (base = icon-53), 0152 (hud-43 = hud-44).

## 2. Weekday items 82-88 (mainrom-0082 .. 0088)

`weekday-resolution.json` shows that every variant resolves to the approved two-letter file
(T39004-date-panel-v1; base and icon-52 share one file). The 12 three-letter drafts below are batch-002
historical assets listed on the items (`english.historical_assets`). Mark each superseded by
`T39004-date-panel-v1/ledger.json` and drop it from the item:

- mainrom-0082: draft:mainrom-weekday-mon-hud-44
- mainrom-0083: draft:mainrom-weekday-tue-base
- mainrom-0083: draft:mainrom-weekday-tue-hud-44
- mainrom-0084: draft:mainrom-weekday-wed-base
- mainrom-0084: draft:mainrom-weekday-wed-hud-44
- mainrom-0085: draft:mainrom-weekday-thu-base
- mainrom-0085: draft:mainrom-weekday-thu-hud-44
- mainrom-0086: draft:mainrom-weekday-fri-base
- mainrom-0086: draft:mainrom-weekday-fri-hud-44
- mainrom-0087: draft:mainrom-weekday-sat-hud-43
- mainrom-0087: draft:mainrom-weekday-sat-hud-44
- mainrom-0088: draft:mainrom-weekday-sun-hud-44

## 3. Drawn text

The catalog row of each frame must describe the text drawn here.

| frame | drawn |
| :--- | :--- |
| 0057 | Save |
| 0058 | Save |
| 0059 | Save |
| 0061 | Health / Art / Looks / Hum. / Sports / Guts / Science / Trivia / Stress |
| 0122 | SPRING ISSUE |
| 0123 | SUMMER ISSUE |
| 0124 | AUTUMN ISSUE |
| 0125 | WINTER ISSUE |
| 0126 | CENTRAL PARK GUIDE |
| 0127 | ART MUSEUM OPENS |
| 0128 | AQUARIUM SPECIAL |
| 0129 | STADIUM OPENS |
| 0130 | GARDEN SPECIAL |
| 0131 | BOWLING SPECIAL |
| 0132 | KARAOKE BOX OPENS |
| 0133 | PLANETARIUM SPECIAL |
| 0134 | Comedy |
| 0135 | Romance |
| 0136 | Action |
| 0137 | Suspense |
| 0138 | Horror |
| 0139 | Anime |
| 0140 | Classical |
| 0141 | Idol |
| 0142 | Rock |
| 0143 | New Music |
| 0144 | Wrestling |
| 0145 | Baseball |
| 0151 | SUIKODEN / Out in '95 |
| 0152 | PARODIUS / Out Now! |
| 0153 | POWER PROS / Out Now! |
| 0154 | TWINBEE PUZZLE / Out Now! |

## 4. Settings

- Genre tags 0134-0145: alignment `center`, margin 1 px (`TAG_ALIGN` in the tool; set it to `left` and rebuild to
  move them, only the tag files change).
- Sheet scope of the files: `frame-composite-only`, `sheet-patch`. A frame-composite-only draft is integrated as a whole-frame replacement.

## 5. Original pixels in the committed files

Edited English derivatives, not source exports: a whole frame keeps the original art outside its edit regions, and source_pixels counts the texels kept from the source. This is why the ledger says `original_images_included: true`. The owner asked for translated
assets to be kept in the repository (`translations/en/graphics/artwork/checkpoints/T39004-2026-10-29/README.md`,
T-39004); the whole-frame drafts these files supersede are committed the same way. No unmodified source export is
committed. Counts are per frame over its palette variants: opaque texels, and those equal to the source texel at
the same position.

| frame | size | variants | opaque texels | kept from source |
| :--- | :--- | ---: | ---: | ---: |
| 0057 | 24x17 | 6 | 249 | 178-238 |
| 0058 | 24x16 | 6 | 260 | 185-249 |
| 0059 | 24x17 | 6 | 270 | 190-259 |
| 0061 | 296x67 | 6 | 8414-8510 | 8100-8303 |
| 0122 | 56x16 | 6 | 229 | 17-64 |
| 0123 | 56x16 | 6 | 163-250 | 15-84 |
| 0124 | 56x16 | 6 | 248 | 14-119 |
| 0125 | 56x16 | 6 | 155-232 | 14-61 |
| 0126 | 136x24 | 6 | 1460 | 427-777 |
| 0127 | 120x24 | 6 | 1352-1354 | 379-796 |
| 0128 | 112x24 | 6 | 1387 | 349-749 |
| 0129 | 144x24 | 6 | 1120 | 208-401 |
| 0130 | 112x24 | 6 | 1166 | 385-754 |
| 0131 | 144x24 | 6 | 1242 | 179-351 |
| 0132 | 152x24 | 6 | 1480 | 306-621 |
| 0133 | 152x24 | 6 | 1564 | 316-626 |
| 0134 | 40x8 | 6 | 75 | 14 |
| 0135 | 48x8 | 6 | 83 | 21 |
| 0136 | 40x8 | 6 | 61 | 17 |
| 0137 | 40x8 | 6 | 82 | 19 |
| 0138 | 24x8 | 6 | 50 | 12 |
| 0139 | 24x8 | 6 | 60 | 14 |
| 0140 | 40x8 | 6 | 82 | 19 |
| 0141 | 32x8 | 6 | 42 | 7 |
| 0142 | 24x8 | 6 | 45 | 13 |
| 0143 | 64x8 | 6 | 93 | 28 |
| 0144 | 32x16 | 6 | 80-141 | 17-50 |
| 0145 | 32x16 | 6 | 80-142 | 23-70 |
| 0151 | 56x66 | 6 | 1344-1352 | 1038-1223 |
| 0152 | 56x66 | 6 | 1379 | 722-1281 |
| 0153 | 56x66 | 6 | 1660-1686 | 1144-1580 |
| 0154 | 56x66 | 6 | 1675-1734 | 1082-1580 |
