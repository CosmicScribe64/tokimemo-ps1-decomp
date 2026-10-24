---
type: tool
updated: 2026-10-24
sources: ["src/main/8007C030.c", "src/main/8004E500.c", "src/main/8004F870.c", "src/ovl/NAME_ENT/80132000.c", "tools/assets/text.py", "tools/assets/text_export.py", "tools/assets/script_profiles.py", "tools/assets/text_source.py"]
---

# Comprehensive text inventory and export

T-28001, part of [[asset-export]], remains In Progress. The user requires comprehensive source-backed
text coverage for a future full translation. A bounded parser or a successful file parse is not completion.
There is no translation/import/assembler yet and no engine-specific code. Real output is only under ignored
`.asset-cache/text/`; this page and the tests contain no game dialogue or lyrics.

## Run and API

Use the existing Docker image:

```sh
docker run --rm --cpus=2 -v "$PWD":/work -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.text_export disc/files --include-source --output .asset-cache/text
```

The CLI prints counts; the catalog contains UTF-8 text, exact raw bytes, commands and provenance.
`tools.assets.text_export.export_text(input_dir, output_dir=None, profiles=None, include_source=False)`
returns a JSON-compatible catalog. The input is an extracted ISO root such as `disc/files`. Without an
output directory it returns data without writing. Files are read-only and JSON output is deterministic.
The compact JSON avoids repeating indentation for tens of thousands of records.

Profiles use paths beginning with `CDROM/`. `--profiles` accepts a JSON list matching
`tools/assets/script_profiles.py`: `id`, `path`, optional `sha256`, `load_address`, and `sections` with
`id`, `text_table`, `command_table`, `voice_table`, `group_count`. Addresses are JSON integers.
Supplying a layout does not prove it is correct for another release. Built-in layouts are pinned to
original-file hashes and cite loaders/initializers; checksum mismatches fail clearly.

Readers:

- `text.parse_script(data, path, profile)`: linked dialogue tables.
- `text_fixed.tokenize_fixed_text(raw, renderer, context=None)`: direct glyph loading, `sndi`, `sndisp`, printf syntax.
- `text_source.inventory_source_text(repository, files_root=None, output_path=None)`: executable/overlay source literal inventory and reference evidence.
- `text_tables.parse_character_lists(data, profile=None)`: external name-entry character lists.
- `text_font.font_metadata(data)`, `font_reference(raw, metadata)`: original font table and bitmap references.
- `script_validation.validate_script(data, path, profile, parsed=None)`: independent structural and semantic checks.
- `script_coverage.audit_all(data_root, repo_root=...)`: source-root and unclaimed-region diagnostics.

## Coverage matrix and remaining gates

| Source/category | Current coverage | Completion evidence or remaining gate |
|---|---|---|
| `CDROM/SERIFU/DATE_*.BIN` (11) | All 293 source-initialized and consumed triads | Six omitted roots per girl were found by the source audit and added; existing IDs retained |
| `CDROM/SERIFU/EVENT_*.BIN` (12) | All 79 source-initialized triads | 47 directly consumed and 32 without a direct assignment; both sets retained |
| Other SERIFU dialogue (21, including P_BIRTH) | Pinned linked-table profiles | Independent initializer/root audit underway |
| `CDROM/SERIFU/TAKU3.BIN` | 40 numeric choice-result tables | Typed numeric data, 668 padding bytes and checked-CD trailer; no missing dialogue parser |
| `CDROM/BUNKA/*.BIN` (46) | Pinned linked-table profiles | Direct/indexed loader mapping checked; independent initializer audit underway |
| Executable + 26 overlays | Source literal/label inventory and original-byte verification | Ambiguous references, inline construction and non-string directives remain explicit audit work |
| Menus/UI, names, places, system/options/debug | Included in the source/context inventory | Context classification and fixed-renderer joining are under review |
| `CDROM/FUKUTEST/NAME_KAN.NBN` | 45 counted lists, 5,414 glyph entries | All offsets/counts/terminators and strict Unicode/font links verified |
| Runtime player names and context substitutions | Symbolic control tokens and source bindings | Runtime user values are not fabricated or translated |
| Original font | 46 range entries, 3,418 12 x 12 glyphs | Each code unit can link to its original 18-byte bitmap extent |
| Japanese text baked into images | Separate media lane owns asset/region catalog | Font IDs can be joined; this reader does not claim image/OCR completeness |

Current script-root audit: 90 containers, 646 sections, 41,228 records and 30,036 text records.
There are no unprofiled SERIFU or BUNKA files. Every script string strictly decodes and re-encodes as
Shift-JIS. Unclaimed byte counts and diagnostic candidate scans are retained as leads; zero candidates
is never treated as proof of completeness. Final completion requires the source/root/context audits and
independent review, including the image-text lane's separate inventory.

## Linked script format

`talk_run` (`src/main/8007C030.c`, around line 2035) reads three parallel two-level tables:
`text[group][line]`, `command[group][line]`, `voice[group][line]`. They contain little-endian absolute RAM
pointers; commands are bytes. Command `0xFF` ends a group. Its ignored text argument does not establish
a valid corresponding string slot. Voice references are pairs of encoded 32-bit clip words; reply
commands 2 and 5 use one pair per variant. Clip words are preserved, not guessed to be WAV paths/offsets.

`talk_render` scans two-byte units with single-byte name codes and space/q separators. Codes P/p/m/F/G/Y
and context buffers S/O/W/A/M remain symbolic. Space/q delimit rows; q separates reply variants; choice
commands preserve source options plus the selectable count and runtime order policy. Conditional trailing
`nn` applies only to commands 0, 1, A, B, C, D. A leading n suppresses a reply but the original reply scanner
still advances over it in two-byte units. Command 7 has four inline speaker markers with fixed byte lengths.
See [[spec/m-dates-b]] sections 7.1-7.5 for source-backed command/voice/selector behavior.

Every record keeps path/section/group/line ID, source hash, command/offset, pointer locations, exact raw
text and terminator, strict source Unicode, ordered control/glyph tokens and voice pairs where used.
Unknown commands, code units, renderer inputs and unclaimed extents remain explicit. Successful parsing
does not prove every group executes, every choice is reachable or a runtime buffer has a particular value.

## Source Unicode and rendering are separate

The glyph loader (`src/main/8004E500.c`, `load_string`) consumes pairs and rejects codes below the first
font range. Source/build encoding is Shift-JIS (`tools/cc.py` also uses `shift_jis`). Unicode output uses
Python's strict mapping and requires exact re-encoding. There is no CP932 fallback; differing CP932 mappings
are exposed as alternatives. Unknown bytes never become replacement characters. The strict source view,
annotated renderer view and original bytes remain separate, and raw bytes are authoritative for rebuilding.

Three original tab bytes are malformed inputs to the renderer, not a newly discovered tab command:
AS_S6W offsets 11084/11184 and HI_S6 offset 23438. Source Unicode preserves the tabs. The renderer scan
advances by two and the glyph loader rejects the resulting `0x09xx` code; later invalid units are an
alignment cascade. No bytes are deleted or repaired. HI_S6's affected group is explicitly source-reachable.

`file_checksum_ok` checks a byte sum separately from the ordinary checked-CD loader's trailer marker
(`src/main/800451D0.c`). FU_S6 has an original sum mismatch with a valid marker; the reader reports both
facts rather than assuming ordinary loading rejects it. Nine source choice strings are empty, and one
three-row source string uses a two-option command; these are reported separately from selectable counts.

## Fixed text, source inventory and image links

`sndi`/`sndisp` have a narrower P/G policy than `talk_render`: first even-position P, and G only under the
appropriate style and post-expansion parity. `sndisp` has a distinct byte counter/reset rule for row spaces.
The fixed reader preserves copy limits and unresolved runtime binding lengths, rather than applying talk
rules to all strings. Direct `load_string` leaves name letters literal and retains trailing-byte/NUL behavior.
Printf-family format tokens keep their exact syntax and argument counts; formatting is never executed.

Source inventory records assembly labels/directives, byte-verified original offsets and hashes, C literal
sites, matching function assembly, and ambiguous/unmapped references. Include paths and build macros are
separate exclusions. Adjacent C literals are concatenated, character constants are not mistaken for strings,
and unsupported escapes/encodings are reported. Generated assembly string guesses alone are not proof of
semantic text use or exhaustive coverage of data encoded without string directives.

The original font bank lives in `SLPM_86.053`: ranges at VMA 0x800B402C and bitmap bytes at 0x800B6D50.
Stable IDs `font:SLPM_86.053:gNNNN` identify glyphs; source SHA256, byte offset, 18-byte stride and 1bpp MSB-first
12 x 12 geometry are in catalog metadata. The game uploads padded 16 x 12 blocks. Script/character-list
units link to those IDs, allowing the media catalog to share exact font identities. Other baked lettering
needs its own media asset/region evidence; a script or font match does not establish bitmap lettering.

## Verification

Run the synthetic tests in Docker:

```sh
docker run --rm --cpus=2 -v "$PWD":/work -w /work tokimemo-decomp-arm64 \
  python3 -m unittest discover -s tools -p 'test_asset_text*.py' -v
```

Tests use invented strings/bytes, source metadata and explicit corruption cases. They cover bounds,
terminators, controls/trail bytes, reply voice extents, source/renderer distinctions, codec ambiguities,
fixed rendering/format grammar, font references, external character lists, output containment, deterministic
exports and independent source-root checks. Final full-input counts/review results are recorded in
[[tickets/T-28001-text-script-export]]. Shared index/board/log/status updates belong to the integration owner.

## Per-container script matrix

All entries below use pinned source hashes and explicit linked-table roots. The independent root
audit is separate from byte accounting. Original renderer defects are retained in their source files.

| Input | Sections | Groups | Text records | Renderer anomaly records |
|---|---:|---:|---:|---:|
| `CDROM/BUNKA/AS_S6.BIN` | 1 | 121 | 550 | 0 |
| `CDROM/BUNKA/AS_S6S.BIN` | 1 | 16 | 33 | 0 |
| `CDROM/BUNKA/AS_S6W.BIN` | 1 | 118 | 425 | 2 |
| `CDROM/BUNKA/BUN_FU.BIN` | 1 | 44 | 70 | 0 |
| `CDROM/BUNKA/BUN_PC.BIN` | 1 | 83 | 107 | 0 |
| `CDROM/BUNKA/FU_S6.BIN` | 1 | 145 | 467 | 0 |
| `CDROM/BUNKA/FU_S6B.BIN` | 1 | 179 | 653 | 0 |
| `CDROM/BUNKA/FU_S6I.BIN` | 1 | 53 | 58 | 0 |
| `CDROM/BUNKA/FU_S6O.BIN` | 1 | 171 | 601 | 0 |
| `CDROM/BUNKA/FU_S6S.BIN` | 1 | 8 | 20 | 0 |
| `CDROM/BUNKA/FU_S6W.BIN` | 1 | 138 | 439 | 0 |
| `CDROM/BUNKA/HI_S6.BIN` | 1 | 210 | 694 | 1 |
| `CDROM/BUNKA/HI_S6B.BIN` | 1 | 70 | 255 | 0 |
| `CDROM/BUNKA/HI_S6I.BIN` | 1 | 31 | 38 | 0 |
| `CDROM/BUNKA/HI_S6O.BIN` | 1 | 61 | 228 | 0 |
| `CDROM/BUNKA/KG_S6.BIN` | 1 | 108 | 386 | 0 |
| `CDROM/BUNKA/KG_S6S.BIN` | 1 | 8 | 29 | 0 |
| `CDROM/BUNKA/KG_S6W.BIN` | 1 | 110 | 499 | 0 |
| `CDROM/BUNKA/KO_S6.BIN` | 1 | 112 | 318 | 0 |
| `CDROM/BUNKA/KO_S6S.BIN` | 1 | 8 | 26 | 0 |
| `CDROM/BUNKA/KO_S6W.BIN` | 1 | 107 | 417 | 0 |
| `CDROM/BUNKA/KS_S61.BIN` | 1 | 159 | 489 | 0 |
| `CDROM/BUNKA/KS_S62.BIN` | 1 | 182 | 464 | 0 |
| `CDROM/BUNKA/KS_S6B.BIN` | 1 | 64 | 269 | 0 |
| `CDROM/BUNKA/KS_S6I.BIN` | 1 | 31 | 39 | 0 |
| `CDROM/BUNKA/KS_S6O.BIN` | 1 | 44 | 162 | 0 |
| `CDROM/BUNKA/KT_S61.BIN` | 1 | 172 | 641 | 0 |
| `CDROM/BUNKA/KT_S62.BIN` | 1 | 183 | 623 | 0 |
| `CDROM/BUNKA/KT_S63.BIN` | 1 | 100 | 386 | 0 |
| `CDROM/BUNKA/KT_S6B.BIN` | 1 | 70 | 323 | 0 |
| `CDROM/BUNKA/KT_S6I.BIN` | 1 | 31 | 38 | 0 |
| `CDROM/BUNKA/KT_S6O.BIN` | 1 | 74 | 224 | 0 |
| `CDROM/BUNKA/KY_S6.BIN` | 1 | 121 | 403 | 0 |
| `CDROM/BUNKA/KY_S6S.BIN` | 1 | 8 | 23 | 0 |
| `CDROM/BUNKA/KY_S6W.BIN` | 1 | 134 | 423 | 0 |
| `CDROM/BUNKA/MI_S6.BIN` | 1 | 67 | 292 | 0 |
| `CDROM/BUNKA/MI_S6S.BIN` | 1 | 9 | 27 | 0 |
| `CDROM/BUNKA/MI_S6W.BIN` | 1 | 63 | 253 | 0 |
| `CDROM/BUNKA/NI_S6.BIN` | 1 | 88 | 278 | 0 |
| `CDROM/BUNKA/NI_S6S.BIN` | 1 | 10 | 30 | 0 |
| `CDROM/BUNKA/NI_S6W.BIN` | 1 | 94 | 316 | 0 |
| `CDROM/BUNKA/SA_S6.BIN` | 1 | 118 | 505 | 0 |
| `CDROM/BUNKA/SA_S6S.BIN` | 1 | 8 | 29 | 0 |
| `CDROM/BUNKA/SA_S6W.BIN` | 1 | 75 | 348 | 0 |
| `CDROM/BUNKA/TWINBEE.BIN` | 1 | 1 | 9 | 0 |
| `CDROM/BUNKA/URANAI.BIN` | 1 | 7 | 7 | 0 |
| `CDROM/SERIFU/DATE_AS.BIN` | 27 | 277 | 460 | 0 |
| `CDROM/SERIFU/DATE_FU.BIN` | 27 | 291 | 476 | 0 |
| `CDROM/SERIFU/DATE_HI.BIN` | 26 | 277 | 452 | 0 |
| `CDROM/SERIFU/DATE_KA.BIN` | 26 | 279 | 454 | 0 |
| `CDROM/SERIFU/DATE_KG.BIN` | 27 | 298 | 498 | 0 |
| `CDROM/SERIFU/DATE_KI.BIN` | 26 | 291 | 500 | 0 |
| `CDROM/SERIFU/DATE_KO.BIN` | 27 | 286 | 485 | 0 |
| `CDROM/SERIFU/DATE_KY.BIN` | 27 | 288 | 472 | 0 |
| `CDROM/SERIFU/DATE_MI.BIN` | 26 | 281 | 489 | 0 |
| `CDROM/SERIFU/DATE_NI.BIN` | 27 | 285 | 469 | 0 |
| `CDROM/SERIFU/DATE_SA.BIN` | 27 | 289 | 482 | 0 |
| `CDROM/SERIFU/ENDING.BIN` | 17 | 27 | 410 | 0 |
| `CDROM/SERIFU/EPILOG.BIN` | 15 | 15 | 134 | 0 |
| `CDROM/SERIFU/EVENT_AS.BIN` | 6 | 27 | 89 | 0 |
| `CDROM/SERIFU/EVENT_FU.BIN` | 8 | 40 | 157 | 0 |
| `CDROM/SERIFU/EVENT_HI.BIN` | 7 | 28 | 98 | 0 |
| `CDROM/SERIFU/EVENT_KA.BIN` | 6 | 38 | 108 | 0 |
| `CDROM/SERIFU/EVENT_KG.BIN` | 7 | 40 | 116 | 0 |
| `CDROM/SERIFU/EVENT_KI.BIN` | 5 | 24 | 75 | 0 |
| `CDROM/SERIFU/EVENT_KO.BIN` | 7 | 33 | 118 | 0 |
| `CDROM/SERIFU/EVENT_KY.BIN` | 5 | 32 | 147 | 0 |
| `CDROM/SERIFU/EVENT_MI.BIN` | 7 | 45 | 140 | 0 |
| `CDROM/SERIFU/EVENT_NI.BIN` | 6 | 42 | 169 | 0 |
| `CDROM/SERIFU/EVENT_OT.BIN` | 8 | 65 | 191 | 0 |
| `CDROM/SERIFU/EVENT_SA.BIN` | 7 | 39 | 161 | 0 |
| `CDROM/SERIFU/GASSHUKU.BIN` | 8 | 286 | 1590 | 0 |
| `CDROM/SERIFU/GASSH_FU.BIN` | 1 | 195 | 1207 | 0 |
| `CDROM/SERIFU/GEKO.BIN` | 12 | 451 | 1105 | 0 |
| `CDROM/SERIFU/GROUP_D.BIN` | 11 | 433 | 448 | 0 |
| `CDROM/SERIFU/G_APPEAR.BIN` | 9 | 120 | 416 | 0 |
| `CDROM/SERIFU/G_BIRTH.BIN` | 13 | 330 | 584 | 0 |
| `CDROM/SERIFU/G_INVITE.BIN` | 13 | 303 | 365 | 0 |
| `CDROM/SERIFU/HAGEMASI.BIN` | 13 | 93 | 173 | 0 |
| `CDROM/SERIFU/KARADATA.BIN` | 11 | 53 | 53 | 0 |
| `CDROM/SERIFU/NYUBU.BIN` | 8 | 24 | 93 | 0 |
| `CDROM/SERIFU/OMIMAI.BIN` | 12 | 15 | 79 | 0 |
| `CDROM/SERIFU/P_BIRTH.BIN` | 13 | 35 | 79 | 0 |
| `CDROM/SERIFU/SERIFU.BIN` | 4 | 12 | 48 | 0 |
| `CDROM/SERIFU/SHOGATU.BIN` | 15 | 505 | 1491 | 0 |
| `CDROM/SERIFU/SHUGAKU.BIN` | 13 | 574 | 1236 | 0 |
| `CDROM/SERIFU/VALEN.BIN` | 13 | 65 | 188 | 0 |
| `CDROM/SERIFU/WHITE.BIN` | 13 | 109 | 251 | 0 |
| `CDROM/SERIFU/XMAS.BIN` | 13 | 136 | 359 | 0 |
| `CDROM/SERIFU/ZAKOSEN.BIN` | 1 | 2 | 7 | 0 |

`CDROM/SERIFU/TAKU3.BIN` is the separately typed numeric choice-result file.
`CDROM/FUKUTEST/NAME_KAN.NBN` contains the 45 external character lists.
Main and NAME_ENT typed term dictionaries add 198 source-backed name/place/club/grid/preset records.
