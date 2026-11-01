# REGISTER: T39410-menus-fixes-v1

What the integrator (who owns `tools/translation_inventory.py`, `translation_inventory.json`, the image text catalog and
`inventory_decisions.json`) must change to take this checkpoint. Nothing in those files was edited here.
All PNGs are English-only (no original pixel), port-made, unreviewed (`human_approved: false`).

## 1. Register the ledger

- Add `translations/en/graphics/artwork/checkpoints/T39410-menus-fixes-v1/ledger.json` to `LEDGERS`. It holds 28 candidates (27 frame/cell drafts and the ALL_C banner of `catalog-127`, three crops).
- `fallback-ledger.json` (15 same-size drafts) is not for `LEDGERS`; register it only if the English build cannot widen sprites (sections 5, 6).
- Ids follow the existing candidate patterns (`title-NNNN`, `option-label-NNNN`, `name-label-NNNN`, `exam-0001`, `filesel-frame-N.base`, `filesel-page-information-0`, `kasetu-0`, catalog item `catalog-127`), so `candidate_key` needs no change.

## 2. Supersedes (declared in `ledger.json`, 39 references)

- `T39004-2026-10-29` / `title-drafts-v1`: `title-0001`, `title-0002`, `title-0003`, `title-0004`
- `T39004-2026-10-29` / `exam-drafts-v1`: `exam-0001`
- `T39004-2026-10-29` / `option-drafts-v1`: `option-label-0063`, `option-label-0064`, `option-label-0079`, `option-label-0080`, `option-label-0103`, `option-label-0104`, `option-label-0113`, `option-label-0114`
- `T39004-2026-10-29` / `name-drafts-v2`: `name-label-0015`, `name-label-0016`, `name-label-0017`, `name-label-0018`, `name-label-0019`, `name-label-0020`, `name-label-0021`, `name-label-0022`, `name-label-0023`, `name-label-0024`, `name-panel-0000`, `name-panel-0001`, `name-panel-0002`, `name-panel-0014`
- `T39004-2026-10-29` / `name-reviewed-development-v1`: `name-label-0015`, `name-label-0017`, `name-label-0019`, `name-label-0021`, `name-label-0023`
- `T39004-2026-10-29` / `filesel-drafts-v2`: `filesel-frame-19.base`, `filesel-frame-20.base`
- `T39004-2026-10-29` / `sports-drafts-v2`: `kasetu-0`
- `T39004-filesel-v4` / `filesel-indexed-v4`: `filesel-page-information-0`
- `T39004-bs-v1` / `bs-v1`: `all-c-banner.source-page-3`, `all-c-banner.source-page-4`, `all-c-banner.source-page-5`

Name-entry panels `name-panel-0000/0001/0002/0014`: the `name-drafts-v2` face is half the Japanese size. The retained `name-reviewed-development-v1` panels (11-12 px, same size as the Japanese) stay the active candidates; only v2 is superseded here and no new art was made. The ten `name-label` selector cells (both older revisions) are replaced by one revision, so lit and unlit states match.

## 3. Catalog wording and binding edits (`translations/en/graphics/image_text_catalog.jsonl`, 1-based line = row)

### Option cast list (rows 72-85): corrections

The bound frames below are what the rows say today (`frame_ids_normal___highlight`); the art column is what the restored sprites show (checked by eye, all 14).

| row | Japanese on the row | English on the row | bound to | art is on | change |
|---|---|---|---|---|---|
| 72 | 金月真美 (藤崎) | Kingetsu Mami (Fujisaki) | 0063/0064 | 0063/0064 | respell: Kanazuki Mami (Fujisaki) |
| 73 | 長沢美樹 (如月) | Nagasawa Miki (Kisaragi) | 0065/0066 | none | no sprite shows this name; the Kisaragi role is 関根明子, so retire the row (it duplicates line 79) |
| 74 | 中友子 (紐緒) | Naka Tomoko (Himoo) | 0067/0068 | 0067/0068 | none |
| 75 | 川口雅代 (片桐) | Kawaguchi Masayo (Katagiri) | 0069/0070 | 0069/0070 | none |
| 76 | 鉄炮塚葉子 (朝日奈) | Teppouzuka Yoko (Asahina) | 0071/0072 | 0079/0080 | rebind to 0079/0080; respell: Teppozuka Yoko (Asahina) |
| 77 | 黒崎彩子 (古式) | Kurosaki Ayako (Koshiki) | 0073/0074 | 0073/0074 | none |
| 78 | よしきくりん (早乙女) | Yoshiki Kurin (Saotome) | 0075/0076 | 0083/0084 | rebind to 0083/0084 |
| 79 | 関根明子 (如月) | Sekine Akiko (Kisaragi) | 0077/0078 | 0065/0066 | rebind to 0065/0066 |
| 80 | 栗原みきこ (美樹原) | Kurihara Mikiko (Mikihara) | 0079/0080 | 0081/0082 | rebind to 0081/0082 |
| 81 | 五十嵐麗 (鏡) | Igarashi Rei (Kagami) | 0081/0082 | 0077/0078 | rebind to 0077/0078 |
| 82 | 笹木綾子 (清川) | Sasaki Ayako (Kiyokawa) | 0083/0084 | 0075/0076 | rebind to 0075/0076 |
| 83 | 津野田なるみ (伊集院) | Tsunoda Narumi (Ijuin) | 0085/0086 | 0087/0088 | rebind to 0087/0088 |
| 84 | 菊池志穂 (館林) | Kikuchi Shiho (Tatebayashi) | 0087/0088 | 0085/0086 | rebind to 0085/0086 |
| 85 | 上田祐司 (好雄) | Ueda Yuji (Yoshio) | 0089/0090 | 0089/0090 | none |
| (new) | 菅原祥子 (虹野) | Sugawara Sachiko (Nijino) | none | 0071/0072 | no catalog row exists for this sprite: add one (section 2, 122x16, suggested graphic the same words) |

Result: every frame pair 0063..0090 binds to the row that names the person drawn on it. `菅原祥子` reads Sugawara Sachiko (the pill and the sprite agree).

### Wording rows

- Row 72: `Kingetsu Mami (Fujisaki)` -> `Kanazuki Mami (Fujisaki)` (english and suggested graphic).
- Row 76 (art 0079/0080): `Teppouzuka Yoko (Asahina)` and the suggested `Y. Teppouzuka (Asahina)` -> `Teppozuka Yoko (Asahina)`.
- Rows 75, 76, 80 suggested graphics abbreviate the given name (`M. Kawaguchi`, `Y. Teppouzuka`, `M. Kurihara`): all cast rows use the full `Family Given (Role)` forms the art uses.
- Rows 92 and 97 (0103/0104, 0113/0114): suggested graphic -> `Kiyokawa Nozomi` (sprite widened to 71) and `Tatebayashi Miharu` (widened to 82); fallback `Kiyokawa` / `Tatebayashi` at the source widths.
- Row 3 (MAINROM 0061 panel) and row 104 (FILESEL 0019): stat names are Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress (rows now say Liberal Arts / Athletics / Lit. / Sci. / Sport / Condition). "Hum." was not needed on the FILESEL buttons: `Humanities` fits (38 of 43 texels). MAINROM 0061 and the text dictionary are outside this checkpoint.
- Row 104 heroine buttons: Option A (given names) is what the art uses; Option B is retired.
- Rows 107-111 (name selector labels): english Hiragana / Katakana / Symbols / Kanji / Done; suggested graphic `56x16` full words (fallback `32x16`: Hira / Kata / Symb / Kanji / Done). There is no Latin/English page selector cell in NAME_ENT frames 15-24; when the English build adds one, "ABC" in the same cell, face and colours is the consistent form.
- Rows 113-116 (title): `GAME START` / `OPTIONS` bold outlined lettering at the Japanese height (replace the `112x16` / `80x16` plain-caps note).
- Row 118 (EXAMINE 0001): english `points`; suggested graphic `points` in a widened cell (see section 5), fallback `pt` in `8x16`.
- Row 123 (sports KASETU card 0): title `Scavenger Race` (approved; the card text itself says nothing about borrowing). Use the same name wherever the event is named.
- Row 127 (ALL_C banner): suggested graphic says bold yellow/gold; the banner keeps the Japanese blue / red / purple, because the strip holds no gold index and the gates allow only indices the strip already uses. Stroke 3 texels, 18 rows tall like the Japanese.

## 4. Inventory decisions (`inventory_decisions.json`)

- None required: the ledger ids map to existing atoms, no atom changes status class. If the English build keeps `Hum.` anywhere, record it as an override there, not in art.

## 5. Required sprite widths (English build must widen these entries)

| item | source width | required width | note |
|---|---|---|---|
| `exam-0001` | 8 | 32 | points |
| `option-label-0103` | 56 | 71 | Kiyokawa Nozomi; 5 tiles of 16 (source 4) |
| `option-label-0104` | 56 | 71 | Kiyokawa Nozomi; 5 tiles of 16 (source 4) |
| `option-label-0113` | 70 | 82 | Tatebayashi Miharu; 6 tiles of 16 (source 5) |
| `option-label-0114` | 70 | 82 | Tatebayashi Miharu; 6 tiles of 16 (source 5) |
| `name-label-0015` | 32 | 56 | Hiragana |
| `name-label-0016` | 32 | 56 | Hiragana |
| `name-label-0017` | 32 | 56 | Katakana |
| `name-label-0018` | 32 | 56 | Katakana |
| `name-label-0019` | 32 | 56 | Symbols |
| `name-label-0020` | 32 | 56 | Symbols |
| `name-label-0021` | 32 | 56 | Kanji |
| `name-label-0022` | 32 | 56 | Kanji |
| `name-label-0023` | 32 | 56 | Done |
| `name-label-0024` | 32 | 56 | Done |

- `exam-0001`: `points` is 25 texels of ink plus a 1 texel left margin; the cell is drawn 32 wide (8-texel multiple). The rest of the score cells stay 8x16, so the build must place the suffix cell with 32 texels of advance (or 26 minimum).
- `name-label-0015..0024`: all ten selector cells 56x16 (outline stretched, interior transparent); the selector positions need 24 more texels each.
- `filesel-page-information-0`: the `system-tab` region in `filesel_page_regions.json` must become `[17, 88, 42, 11]` (was `[17, 88, 39, 11]`): the word is shifted right two texels, and the three extra columns are white tab interior in the source.

## 6. OPTION.NBN texture room

Counted from the sprite parts of the restored OPTION.NBN (16x16 cells over four 128x128 pages): 256 cells in total, 204 used by sprites, 52 free. The 32 English label pairs of the option drafts need 218 cells unshared (every English sprite owns its tiles; the Japanese sprites share kanji tiles), while their Japanese labels occupy 111 cells of which 102 are free once replaced; with the free cells that is 154 available. There is clearly not enough room for all the option drafts at tile granularity, so the build has to repack the page (or the pixel-level text strips) regardless of the two widened names; the two widened names add 2 cells over the 16-texel rounding of their source widths. Family-name-only fallbacks at the source widths are therefore provided in `fallback-v1` (`option-label-0103/0104/0113/0114`).

## 7. Names: one spelling everywhere

- Option cast sprites, option character sprites and the movie pills agree: actress names equal on both sides (True), every option character name is a pill character name (True). Checked at build time against `translation_movie_pills.CUES`; the pills needed no change (they already read Kanazuki Mami and Teppozuka Yoko with full given names).
- Script rule: family name first, no long-vowel marks or doubled vowels, no initials. `Himoo` is the script spelling and stays.

## 8. Per item

- `title-0001..0004`: bold skeleton capitals, 3 texel stroke, rows 2-13 with a 1 texel dark outline (rows 1-14, as the Japanese), per-row gradient sampled from the Japanese lettering (grey on 0002/0004, blue on 0001/0003). Highlighted frames keep the slab pixels untouched and the slab hides the lower rows exactly as it hides the Japanese (only the top eight fill rows show).
- `catalog-127`: one-line `ALL CLEAR CONGRATULATIONS!`, 18 rows tall, stroke 3, centred with 15 texel margins; page 5 crop now holds the tail and the exclamation mark. Indexed gates pass (`english_container_sha256` in the manifest).
- `exam-0001`: `points` (menu face, source pink, no shadow like the source) and the `pt` fallback.
- `option-label-0063/0064/0079/0080`: respelled; `0103/0104/0113/0114`: full names (widened).
- `name-label-0015..0024`: `Hiragana`, `Katakana`, `Symbols`, `Kanji`, `Done` in unlit and lit states, centred, at least 3 texels clear of the outline.
- `filesel-frame-20.base`: given-name buttons in the tight face, glyphs unscaled, 2 or more texels clear of each face edge, nothing in the bevel rows.
- `filesel-frame-19.base`: Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress in the same face; `Stats` header as before.
- `filesel-page-information-0`: System tab word moved right two texels (ink starts at x=19; the Japanese ink touched x=17).
- `kasetu-0`: title `Scavenger Race`, same face and centring as before; body text unchanged.
