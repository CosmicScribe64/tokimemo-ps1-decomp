# REGISTER: T39410-menus-fixes-v1

What the integrator (who owns `tools/translation_inventory.py`, `translation_inventory.json`, the image text catalog and
`inventory_decisions.json`) must change to take this checkpoint. Nothing in those files was edited here.
All PNGs are English-only (no original pixel), port-made, unreviewed (`human_approved: false`).

## 1. Register the ledger

- Add `translations/en/graphics/artwork/checkpoints/T39410-menus-fixes-v1/ledger.json` to `LEDGERS`. It holds 31 candidates (30 frame/cell drafts and the ALL_C banner of `catalog-127`, three crops).
- `fallback-ledger.json` (17 same-size drafts) is not for `LEDGERS`; register it only if the English build cannot widen sprites (sections 5, 6).
- Ids follow the existing candidate patterns (`title-NNNN`, `option-label-NNNN`, `name-label-NNNN`, `exam-0001`, `filesel-frame-N.base`, `filesel-page-information-0`, `kasetu-0`, catalog item `catalog-127`). The new English letter page label has no source frame: `name-label-abc.unlit` / `name-label-abc.lit`; `candidate_key` in `tools/translation_inventory.py` maps `name-label-<page>.<state>` to the NAME_ENT atom `name_ent-<page>` (added in this pass).

## 2. Supersedes (declared in `ledger.json`, 40 references)

- `T39004-2026-10-29` / `title-drafts-v1`: `title-0001`, `title-0002`, `title-0003`, `title-0004`
- `T39004-2026-10-29` / `exam-drafts-v1`: `exam-0001`
- `T39004-2026-10-29` / `option-drafts-v1`: `option-label-0063`, `option-label-0064`, `option-label-0079`, `option-label-0080`, `option-label-0103`, `option-label-0104`, `option-label-0113`, `option-label-0114`
- `T39004-2026-10-29` / `name-drafts-v2`: `name-label-0015`, `name-label-0016`, `name-label-0017`, `name-label-0018`, `name-label-0019`, `name-label-0020`, `name-label-0021`, `name-label-0022`, `name-label-0023`, `name-label-0024`, `name-panel-0000`, `name-panel-0001`, `name-panel-0002`, `name-panel-0014`
- `T39004-2026-10-29` / `name-reviewed-development-v1`: `name-label-0015`, `name-label-0017`, `name-label-0019`, `name-label-0021`, `name-label-0023`, `name-panel-0001`
- `T39004-2026-10-29` / `filesel-drafts-v2`: `filesel-frame-19.base`, `filesel-frame-20.base`
- `T39004-2026-10-29` / `sports-drafts-v2`: `kasetu-0`
- `T39004-filesel-v4` / `filesel-indexed-v4`: `filesel-page-information-0`
- `T39004-bs-v1` / `bs-v1`: `all-c-banner.source-page-3`, `all-c-banner.source-page-4`, `all-c-banner.source-page-5`

Name-entry panels `name-panel-0000/0001/0002/0014`: the `name-drafts-v2` face is half the Japanese size. The retained `name-reviewed-development-v1` panels (11-12 px, same size as the Japanese) stay the active candidates for 0000, 0002 and 0014; v2 is superseded here. Panel 0001 (birthday page) is replaced by `name-panel-0001` of this checkpoint: its grid headers read `Month` / `Day` instead of `Birth month` / `Birth day` under the `Birthday:` field. The ten `name-label` selector cells (both older revisions) are replaced by one revision, so lit and unlit states match.

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
- Rows 107-111 (name selector labels): english Hiragana / Katakana / Symbols / Kanji / Done; suggested graphic `56x16` full words (fallback `32x16`: Hira / Kata / Sym / Kanji / Done; row 109 edited from `Symb`, which read as cryptic). There is no Latin/English page selector cell in NAME_ENT frames 15-24. The English build adds the letter page and selects it by default; its label `ABC` is drawn here (`name-label-abc.unlit` / `.lit`, 56x16, fallback 32x16) in the same cell, face and colours as the other labels. The catalog has no row for it: add one under section 4 (`ABC`, new cell, normal / lit) when the build adds the page.
- Rows 113-116 (title): `GAME START` / `OPTIONS` bold outlined lettering at the Japanese height (replace the `112x16` / `80x16` plain-caps note).
- Row 118 (EXAMINE 0001): english `points`; suggested graphic `points` in a widened cell (see section 5), fallback `pt` in `8x16`. Code side: the suffix cell is fixed art, so a score of 1 would read `1 points`. The English build handles the singular (draws `point` / `pt`, or skips the suffix cell for 1); no art change here.
- Row 123 (sports KASETU card 0): title `Scavenger Race` (owner decision: 借り物競走 / 借りもの競争 is `Scavenger Race` everywhere), drawn as outlined capitals; the row says `(yellow ink 2)` and is edited to the outlined dark ink. The script used `Scavenger Hunt` for the same event in `translations/en/supplemental/tai_tx.md` row `0030` and `translations/en/supplemental.jsonl`; both now say `Scavenger Race` (no other instance in `translations/en`).
- Row 127 (ALL_C banner): suggested graphic says bold yellow/gold; the banner keeps the Japanese blue / red / purple, because the strip holds no gold index and the gates allow only indices the strip already uses. Stroke 3 texels, 18 rows tall like the Japanese.

## 4. Inventory decisions (`inventory_decisions.json`)

- None required for the frame drafts: their ids map to existing atoms and no atom changes status class. If the English build keeps `Hum.` anywhere, record it as an override there, not in art. The two `name-label-abc.*` cells create one candidate-derived atom, `name_ent-abc` (NAME_ENT, no frame, no catalog line; it builds as candidate-only). When the catalog row for `ABC` is added, bind it there or with an `extra_atoms` entry.

## 5. Required sprite widths (English build must widen these entries)

| item | source width | required width | note |
|---|---|---|---|
| `exam-0001` | 8 | 32 | points |
| `option-label-0103` | 56 | 72 | Kiyokawa Nozomi; 5 tiles of 16 (source 4) |
| `option-label-0104` | 56 | 72 | Kiyokawa Nozomi; 5 tiles of 16 (source 4) |
| `option-label-0113` | 70 | 83 | Tatebayashi Miharu; 6 tiles of 16 (source 5) |
| `option-label-0114` | 70 | 83 | Tatebayashi Miharu; 6 tiles of 16 (source 5) |
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
| `name-label-abc.unlit` | 32 | 56 | ABC |
| `name-label-abc.lit` | 32 | 56 | ABC |

- `exam-0001`: `points` is 25 texels of ink plus a 1 texel left margin; the cell is drawn 32 wide (8-texel multiple). The rest of the score cells stay 8x16, so the build must place the suffix cell with 32 texels of advance (or 26 minimum).
- `name-label-0015..0024`: all ten selector cells 56x16 (outline stretched, interior transparent); the selector positions need 24 more texels each.
- `filesel-page-information-0`: the `system-tab` region in `filesel_page_regions.json` is now `[17, 88, 42, 11]` (was `[17, 88, 39, 11]`): the word is shifted right two texels, and the three extra columns are white tab interior in the source. Done in this pass: `tools/translation_filesel_indexed.py` reserves the rect (`reserve` on the system-tab item) and the region map, the `T39004-filesel-v4` manifest `page_regions_sha256`, its edit regions and the ledger `manifest_sha256` pins were regenerated (PNG outputs unchanged). The build refuses a region map without it.

## 6. OPTION.NBN texture room

Counted from the sprite parts of the restored OPTION.NBN (16x16 cells over four 128x128 pages): 256 cells in total, 204 used by sprites, 52 free. The 32 English label pairs of the option drafts need 218 cells unshared (every English sprite owns its tiles; the Japanese sprites share kanji tiles), while their Japanese labels occupy 111 cells of which 102 are free once replaced; with the free cells that is 154 available. There is clearly not enough room for all the option drafts at tile granularity, so the build has to repack the page (or the pixel-level text strips) regardless of the two widened names; the two widened names add 2 cells over the 16-texel rounding of their source widths. Family-name-only fallbacks at the source widths are therefore provided in `fallback-v1` (`option-label-0103/0104/0113/0114`).

## 7. Names: one spelling everywhere

- Option cast sprites, option character sprites and the movie pills agree: actress names equal on both sides (True), every option character name is a pill character name (True). Checked at build time against `translation_movie_pills.CUES`; the pills needed no change (they already read Kanazuki Mami and Teppozuka Yoko with full given names).
- Script rule: family name first, no long-vowel marks or doubled vowels, no initials. `Himoo` is the script spelling and stays.

## 8. Per item

- `title-0001..0004`: bold skeleton capitals, 3 texel stroke, rows 2-13 with a 1 texel dark outline (rows 1-14, as the Japanese), per-row gradient sampled from the Japanese lettering (grey on 0002/0004, blue on 0001/0003). Highlighted frames keep the slab pixels untouched and the slab hides the lower rows exactly as it hides the Japanese (only the top eight fill rows show).
- `catalog-127`: one-line `ALL CLEAR CONGRATULATIONS!`, 18 rows tall, stroke 3, centred with 15 texel margins; page 5 crop now holds the tail and the exclamation mark. Indexed gates pass (`english_container_sha256` in the manifest).
- `exam-0001`: `points` (menu face, source pink, no shadow like the source) and the `pt` fallback.
- `option-label-0063/0064/0079/0080`: respelled; `0103/0104/0113/0114`: full names (widened, one clear texel right of the shadow column).
- `name-label-0015..0024`: `Hiragana`, `Katakana`, `Symbols`, `Kanji`, `Done` in unlit and lit states, centred, at least 3 texels clear of the outline; one extra texel between H and i (the H shadow had closed the gap; the same for A-B and B-C in ABC). `name-label-abc.unlit/.lit`: `ABC`, same cell, face and colours (outline and colours of frames 15 / 16).
- `filesel-frame-20.base`: given-name buttons in the tight face, glyphs unscaled, 2 or more texels clear of each face edge, nothing in the bevel rows.
- `filesel-frame-19.base`: Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress in the same face; `Stats` header as before.
- `filesel-page-information-0`: System tab word moved right two texels (ink starts at x=19; the Japanese ink touched x=17).
- `name-panel-0001`: the development-v1 birthday page with its grid headers `Birth month` / `Birth day` re-lettered `Month` / `Day` (native font, same ink, shadow and origins; the build first proves the old headers are that font at those origins). Catalog row 106 edited to match. The new draft has no agent review, so `name_ent-0001` no longer carries the development-v1 bounded in-game agent review.
- `kasetu-0`: title `SCAVENGER RACE` as outlined capitals (one-texel paper strokes, dark outline), rows 41-54 and letter-spaced like the outlined Japanese title; body text unchanged.

## 9. Transparency and STP

Checked for every PNG of both folders against the `.stp.png` mask exported beside its source (bit 15 of the texel word after the runtime palette load):

| source | STP in the source | items |
|---|---|---|
| `exam.frame-01` | clear on every texel | `exam-0001` |
| `filesel.frame-19.base` | clear on every texel | `filesel-frame-19.base` |
| `filesel.frame-20.base` | clear on every texel | `filesel-frame-20.base` |
| `filesel.source-page-information-0` | clear on every texel | `filesel-page-information-0` |
| `kasetu.instruction-card-0` | set on every texel | `kasetu-0` |
| `name.frame-01` | clear on every texel | `name-panel-0001` |
| `name.frame-15` | clear on every texel | `name-label-0015`, `name-label-abc.unlit` |
| `name.frame-16` | clear on every texel | `name-label-0016`, `name-label-abc.lit` |
| `name.frame-17` | clear on every texel | `name-label-0017` |
| `name.frame-18` | clear on every texel | `name-label-0018` |
| `name.frame-19` | clear on every texel | `name-label-0019` |
| `name.frame-20` | clear on every texel | `name-label-0020` |
| `name.frame-21` | clear on every texel | `name-label-0021` |
| `name.frame-22` | clear on every texel | `name-label-0022` |
| `name.frame-23` | clear on every texel | `name-label-0023` |
| `name.frame-24` | clear on every texel | `name-label-0024` |
| `option.frame-063` | clear on every texel | `option-label-0063` |
| `option.frame-064` | clear on every texel | `option-label-0064` |
| `option.frame-079` | clear on every texel | `option-label-0079` |
| `option.frame-080` | clear on every texel | `option-label-0080` |
| `option.frame-103` | clear on every texel | `option-label-0103` |
| `option.frame-104` | clear on every texel | `option-label-0104` |
| `option.frame-113` | clear on every texel | `option-label-0113` |
| `option.frame-114` | clear on every texel | `option-label-0114` |
| `title.frame-01` | clear on every texel | `title-0001` |
| `title.frame-02` | clear on every texel | `title-0002` |
| `title.frame-03` | clear on every texel | `title-0003` |
| `title.frame-04` | clear on every texel | `title-0004` |

- Transparent texels are BGR555 word `0x0000` (alpha 0) with STP clear, as in every source; the widened cells add only transparent texels.
- No source and no English output uses opaque black, so no `0x8000` word is needed; every opaque English colour has exactly one STP value in its source, so mapping colours back to the source CLUT gives every texel the source STP bit (clear everywhere except the KASETU card, where the source export sets it on every texel and so must the English card).
- `catalog-127` (ALL_C): only indices the source strip already uses, palette unchanged; indices 6, 36, 37, 38, 39, 67, 69, 70, 71, 146, 148, none word 0, STP clear on all.
- The build fails if an output breaks any of these (`stp_audit`); each manifest item records its counts under `stp`.
