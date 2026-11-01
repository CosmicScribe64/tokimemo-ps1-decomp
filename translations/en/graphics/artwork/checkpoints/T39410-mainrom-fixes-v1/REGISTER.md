# T39410 register note for the integrator

This checkpoint redraws the MAINROM items the 2026-11-01 owner review sent back. It does not edit any shared
registration file. Every PNG is an unreviewed draft (`human_approved: false`), one file per palette variant.
Run `python3 tools/translation_mainrom_fixes.py --output .asset-cache/<new dir> [--publish <new dir>]` in the
Pillow Docker image to rebuild it.

## 1. Supersedes (translation_inventory.py / inventory_decisions.json)

Register `translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1/ledger.json` as a ledger and bind
`mainrom-fix-NNNN.<variant>` to the item `mainrom-NNNN` (frame NNNN) for NNNN in 0057, 0058, 0059, 0061, 0122, 0123, 0124, 0125, 0126, 0127, 0128, 0129, 0130, 0131, 0132, 0133, 0134, 0135, 0136, 0137, 0138, 0139, 0140, 0141, 0142, 0143, 0144, 0145, 0151, 0152, 0153, 0154. Candidate ids
follow `mainrom-fix-<frame>.<palette_id>` (`candidate_key` needs that pattern). The `supersedes` list of the
ledger names the 165 earlier candidates they replace (each asset also lists the ones for its own variant);
mark every one `superseded` with `superseded_by` this ledger:

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

Every variant of every frame is covered, so the variant groups of the inventory items can be one per palette
(the previous `hud-43|hud-44|icon-52|icon-53` groups of frame 126 are now four files; the manifest's
`identical_variant_outputs` names the variants whose files are byte-identical because their source image is).

## 2. Weekday items 82-88 (mainrom-0082 .. 0088)

`weekday-resolution.json` proves that every variant resolves to the approved two-letter file
(T39004-date-panel-v1; base and icon-52 share one file). The stale three-letter drafts are the 12 batch-002
historical assets still listed on the items (`english.historical_assets`). Mark each of them superseded by
`T39004-date-panel-v1/ledger.json` and drop them from the items so no packer can choose them:

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

## 3. Catalog wording (image_text_catalog.jsonl / .md; not edited here)

| frame | catalog now | drawn |
| :--- | :--- | :--- |
| 0126 | Central Park Guide (suggestion column says "Central Park") | CENTRAL PARK GUIDE; 紹介 is a feature heading like the others, so it keeps "Guide" |
| 0127 | Art Museum Completed | ART MUSEUM OPENS |
| 0128 | Aquarium Feature | AQUARIUM SPECIAL |
| 0129 | Stadium Completed | STADIUM OPENS |
| 0130 | Botanical Garden Feature | GARDEN SPECIAL |
| 0131 | Bowling Alley Feature | BOWLING SPECIAL |
| 0132 | Karaoke Box Completed | KARAOKE OPENS |
| 0133 | Planetarium Feature | PLANETARIUM SPECIAL |
| 0122-0125 | Mar / Apr / May Issue ... | 3-4-5 Issue, 6-7-8 Issue, 9-10-11 Issue, 12-1-2 Issue |
| 0061 | Condition / Liberal Arts / Athletics | Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress |
| 0140 | Classical | Classical (the T-39009 draft said Classic; the full word fits) |
| 0151-0154 | Gensosuikoden, Gokujou Parodius, Jikkyou Powerful Pro Yakyuu '95, TwinBee Taisen Puzzle-Dama | SUIKODEN, PARODIUS, PRO BASEBALL, TWINBEE PUZZLE |

Wording rule of the headline family: 完成 -> "Opens", 特集 -> "Special", 紹介 -> "Guide". "Hum." is used in
frame 61 because "Humanities" does not fit the 26 px a label strip leaves before the arrow marker; the text
dictionary has the full word.

## 4. Inventory decisions

- Frame 0057-0059: the earlier reading note (セーブ中, "Save" kept) still holds; "Save" is now in the Japanese band.
- Tags 0134-0145 are centred with a 1 px margin on both sides (owner decision 2026-11-01). If the in-game check
  prefers left alignment, set `TAG_ALIGN = 'left'` in the tool and rebuild; only the tag files change.
- All 0151-0154 title lettering is thin 1 px capitals (eight letters must fit 40 px); it keeps each logo's colours and
  the cover art outside the title rows and the bottom strip exactly as T-39009 drew them.
- Sheet scope: every row records `sheet_scope`; frame-composite-only drafts must be integrated as whole-frame
  replacements, as before.
- The text dictionary change (item 8) is already in `translations/en/returned-v3.jsonl`: records
  `dictionary:SLPM_86.053:parameter_names:00/01/04` and `SLPM_86.053:00072ad0/00072ad8/00072af0` now read Health,
  Humanities, Sports (they read Condition, Liberal Arts, Athletics); the other stat names already matched. The SLPM
  tables that `translation_tree.py` builds from are gitignored (`SLPM_*`), so the JSONL was edited directly.
  `translations/en/manifest.json` was already out of date with the file and is unchanged.
- Rebuild `translations/en/graphics/translation_inventory.json` (it pins the `returned-v3.jsonl` hash):
  `test_translation_inventory.InventoryTests.test_committed_inventory_matches_inputs` fails until it is rebuilt,
  because the JSONL hash changed (inventory `ed180f9f...`, now `7f3ed304...`).
