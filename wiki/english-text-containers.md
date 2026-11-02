---
type: reference
updated: 2026-11-01
sources: ["tools/patch/text/", "tools/patch_text.py", "tools/test_patch_text.py", "translations/en/returned-v3.jsonl", "translations/en/supplemental.jsonl"]
---

# English text containers: writers, coverage and sizes

Ticket [[tickets/T-39503-text-container-writers]], part of [[tickets/T-39500-english-translation-patch]]. The writers rebuild every file that holds a translated record from its records plus its non-text structure, prove the rebuild byte-exact with the original records, then accept English through a pluggable encoder. Readers they build on: [[asset-export]], [[assets-text]]. Original facts (trailer, load addresses): [[ref-english-patch]].

## Running

```
tools/docker.sh python3 -m tools.assets.text_export disc/files --output .asset-cache/text --include-source \
    --source-repository <snapshot>        # 8 minutes; needs asm/ (run the build first)
tools/docker.sh python3 -m tools.assets.text_units .asset-cache/text/catalog.json --format jsonl
TOKIMEMO_SCRATCH=<dir with track1.bin> tools/docker.sh python3 -m tools.patch_text lba-map /scratch/track1.bin
tools/docker.sh python3 -m tools.patch_text all       # roundtrip, coverage, sizes
tools/docker.sh python3 -m unittest tools.test_patch_text -v
```

The export checks hashes of the source files it cites. Three files changed after those pins (`src/main/hud_panels.c`, `include/main/message.h`, `include/main/portrait.h`, commit 92f7cc070), so the 2026-11-01 run used a snapshot of `src/ include/ config/ asm/` with those three taken from 92f7cc070^. The units it produced carry the same 55,403 ids as `returned-v3.jsonl`, and every protected token there matches the unit bytes at its offset. Reports go to `.asset-cache/patch-text/` (ids, offsets, counts; no text). The LBA map is optional; without it the size report omits the read-site checks.

## Writer API

- `containers.build_containers(disc_root, units, supplemental)` returns `{path: Container}`. A container has an `image.Image` and the `Record` of every id that points into the file.
- `Image.write({record id: payload})` returns `WriteResult(data, placements, overflows, grown_bytes, free_bytes_left)`. A payload is the string without its terminator; ids left out keep their original payload.
- `segments.source_segments(unit)` and `translations.target_of(row, source_segments)` give lists of `Text(text)` and `Control(kind, name, raw)`. `segments.encode(segments, encoder)` makes the payload. An encoder is any object with `name`, `encode_text(str) -> bytes` and `encode_control(Control) -> bytes`. `ShiftJisEncoder` is the original encoding. `ProvisionalAsciiEncoder` is the size model: one byte per ASCII character, typographic characters folded, controls unchanged. T-39502 replaces it with the runtime encoding.
- `c_literals.locate_sites(repo, units)` and `c_literals.rewrite_sources(repo, sites, payloads)` write `source_literal` records into `src/`. `render_literal(bytes)` gives a C literal for any byte string.
- `coverage.coverage_map`, `coverage.check_ids`, `coverage.text_gaps`, `coverage.magazine_tables`, `sizes.english_payloads`, `sizes.size_report`.

## Container models

| Kind | Files | Model |
|---|---|---|
| talk_script | 98: SERIFU 44, BUNKA 46, MASTER 8 | `parse_script` with the profiles of `tools/assets/script_profiles*.py`. Each string is a relocatable slot, and the text-list word that reaches it is a pointer. Every byte the parser claims (root tables, group lists, command bytes, voice pairs) is fixed structure. The free space is each string, the zero bytes after it up to the next 4-byte boundary, and the zero tail before the trailer. Strings are laid out again in original order, 4-aligned. A string that does not fit goes to the tail, then to appended sectors. |
| talk_script_supplemental | 6: `SHIAI/SIAI_TX`, `TAIIKU/F_DANCE`, `KISA23`, `KIYOTAI`, `TAI_TX`, `TARA_OBJ/KARAKASI` | No profile. Load base 0x801E8000 (TAI_TX 0x801F0000): at that base, a word equal to base + offset reaches every string of the file, and KARAKASI's stated pointer offsets give the same base. These files are relocatable through those words. Only the strings and the final zero run are free, because alignment bytes there may belong to a byte table. |
| overlay | 25 `.EXN` (O.BIN is reference-only) | In-place slots. Code (`%hi/%lo`), fixed strides or tables reach these strings. The capacity is the string plus the zero bytes after it up to the next 4-byte RAM boundary or the next string. Fixed-stride slots (`dictionary_slot` with a slot of more than 4 bytes) have the stride as capacity and keep their original bytes after the new NUL. |
| main_executable | `SLPM_86.053` (base 0x80040800 for file offsets) | As overlays. |
| binary_table | `FUKUTEST/JOHO.NBN` | Fixed-stride slots in place. |
| speaktst | 12 `SPEAKTST/TEST_*.BIN` | Layout-only candidates kept in place. They are out of scope for the first release (T-39500). |
| c_source | 2,201 `source_literal` records | `rewrite_sources` replaces the run of adjacent string tokens at `source_character_offset` with one rendered literal. Shift-JIS pairs are written as UTF-8 characters (`tools/cc.py` turns them back into octal), printable ASCII as itself, and every other byte as `\ooo`. |

The overlays and the main executable are built from the decomp, so their binary writer is the in-place fallback and the identity proof of the extents. English that grows past its capacity there is a relink (T-39502), and the writer reports it as an overflow.

### Roles

Every record has exactly one role:

- `owner` (53,178): the record writes its slot.
- `alias` (2,345): another record covers the same bytes, and the alias names its owner. The cases: 1,852 compiled extents of a C literal (the literal owns the bytes), 198 extents under a typed table entry, 286 BUNKA_SD stage views (`skip1` begins one byte into the extent), 5 RPG initial buffers, and 4 suffix extents that code reaches at +3 inside a 25-byte table entry (`SLPM_86.053:00075628` and three others). An alias's translation is not written. Where it differs from its owner's, the consumer that reads the alias needs a code change.
- `derived` (25): `conditional_runtime_variant`. The game patches the owner at run time (date birthday initial glyph, school-trip line shortening, new-year selected line). The patch code must still find its bytes in the English owner (T-39502).
- `source_only` (349): C literals without a proven binary extent (EVENT/GYOZI and others). They are written to `src/` only.
- `reference` (14): `O.BIN` (never loaded; a copy of OLH strings) and the NAME_KAN description row.

## Control codes

The handoff split (`tools.assets.text_handoff.template`) is the segment split. A marker `[[Tnnn:name]]` is a `Control` with the bytes in `protected_tokens.raw_hex`. Counts are over `returned-v3.jsonl`.

| Kind / name | Bytes | Count | Where | Meaning |
|---|---|---|---|---|
| separator / row | 0x20 | 51,641 | talk, SPEAKTST | ends a display line |
| separator / choice_option | `q` (6 rows: 0x20) | 3,873 | talk (commands 3, 8, 19) | next choice option |
| separator / reply_variant | `q` | 3,323 | talk (commands 2, 5) | next reply variant, one voice pair each |
| substitution | `P p m F G Y S O W A M` | 5,248 | talk | name and value codes: P player form, p family, m given, F full, G girl, Y Yoshio, S O W A M context buffers ([[ref-english-patch]]) |
| conditional_choice_zero | `nn` at the end | 14 | talk (commands 0, 1, 10-13) | choice branch zero |
| reply_skip_marker | `n` + next byte | 14 | talk (commands 2, 5) | variant to skip; the scanner steps two bytes |
| inline_label | one of `> - + <` | 197 | talk (command 7) | speaker label; the renderer reads a fixed 4, 6, 2 or 4 label bytes after the marker |
| format_directive | `%d %s %x %08x` | 184 | C strings, tables | printf conversions |
| row_separator | 0x20 | 116 | fixed-renderer strings | line break of the narration renderer |
| source_storage_control | 0x0A, 0x09, 0x0D, 0x0C, 0x1A, 0x1B | 425 | C strings, data | raw control bytes kept as storage |
| terminal_byte_before_nul | 0x20 | 23 | fixed renderer | an odd final byte that the 2-byte loader reads together with the NUL |
| conditional_substitution | `G` | 4 | TEL | girl name inside a conditional text |
| unknown | odd byte pairs | 25 | talk | bytes the 2-byte stepping reads off a pair boundary |
| unknown_renderer_bytes | whole slot | 557 | `overlay-table:` slots | see below |
| opaque (writer only) | whole payload | 0 translated | empty or undecodable slots | rebuilt as is and never translated |

Findings for T-39502:

- In talk scripts, 0x20 and the letters `q n P p m F G Y S O W A M` are control codes. English letters and spaces as plain ASCII would be read as controls, so the runtime encoding has to move one of the two sets. `ProvisionalAsciiEncoder` ignores this collision; it is a size model only.
- Inline labels have a fixed byte length set by the marker, and the English labels (`Teacher: `) do not keep it.
- The fixed-slot overlay tables have no glyph view, so the handoff wrapped all 557 of their Japanese texts as one `unknown_renderer_bytes` marker, and the translators left that marker next to the English. `translations.target_of` drops such a marker when it decodes as Shift-JIS text and the target has other text (problem `wrapped_source_text_dropped`). Otherwise the Japanese would be written back.
- Three BUNKA talk records (`AS_S6W` g0078/g0079, `HI_S6` g0176 l0010) originally retained `unknown` tokens containing bytes of 0x80 or above. The English build now replaces these malformed source fragments through source-and-target-hash-guarded repairs in `translations/en/anomaly-repairs.json`; a fourth repair removes KOSIEN's dangling apostrophe. Raw sources are unchanged. Actual-disc audit of `build/anomaly-repair-proof` verifies all 31,471 strings in 105 script/profile containers with exact target equality and zero original-font bytes. Executable text and artwork are outside that audit's scope; see [[tickets/T-39510-no-japanese-left-check]].
- Three supplemental rows (the vertical-dots rows of `F_DANCE`, `KISA23`) give English without markers for source row breaks. They are written as plain text, and their spaces become row breaks.

## Results (2026-11-01)

Round trip with the original records, byte-exact: 98/98 talk scripts, 6/6 supplemental talk files, 25/25 overlays, 12/12 SPEAKTST, JOHO.NBN and the main executable. That is 143 files, 53,178 slots and 31,334 pointer words. The talk-script rebuild clears every string and lays the strings out again from scratch, so this also proves the free-space and alignment model. All 2,201 C literals render back to their bytes, and in 223 of the 227 files the rendered source is the original text. The four other files had octal escapes, which became UTF-8. Writing every literal in rendered form and rebuilding with ninja passed every SHA-1 check (main and 25 overlays; `src/` restored afterwards). One BUNKA file (`FU_S6.BIN`) has the trailer marker but a stale sum; the writer keeps a stale sum stale and recomputes valid ones.

Coverage: all 55,911 ids (55,403 + 508) map to exactly one container and role. No id is unmapped, none is duplicated, and every mapped id has a translation row.

Text without records (NUL-terminated, kana-rich runs in data parts):
- `EVENT.EXN` (9 runs) and `GYOZI.EXN` (62 runs): place and gift names. Neither overlay is ever loaded ([[overlays]]), so no fix is needed for the patch.
- `B_JIKKEN.BIN`, `B_SUISOU.BIN`, `SD_P23.NBN` and `SHATEKI.BIN` give a few runs each. Checked by hand, these are pixel data that happens to decode.
- No other container has text without a record.

Magazine page (`hud_magazine_page_build`, src/main/hud_screens.c), slot by slot:
- `D_800B32CC` is one word holding `&D_800B328C`, a table of 16 pointers to 4- or 8-byte numeral strings; month numbers use indices 0-11 (`% 12`) and save-slot numbers use `file_index - 1`.
- `D_800B5C2C[13][17]` and `D_800B5D1C[13][17]`: 13 issues, 221 bytes each and 3 pad bytes. They are the typed tables `profile_display_lower` and `profile_display_upper` (`dictionary:SLPM_86.053:profile_display_*:NN`).
- `D_800B5EA4[13][3][36]`: 39 slots, 14 of them blank (full-width spaces).
- All 81 slots have a record. `magazine_issue_index` takes 0-12 (`D_800B5D0C` has 12 bytes and an FF).

Sizes with the provisional encoding:
- Talk files grow from 2,523,136 to 2,732,032 bytes (+208,896). 32 of the 104 files gain sectors, 102 sectors in all. 30,511 pointer words change in 103 files.
- In 30 files a CD read call site in `src/` gives a sector count that is now too small. Twelve BUNKA `*_S6S/I` files are read through tables (`tour_invite_script_sectors/lba`), not through literal calls.
- Ten `DATE_*.BIN` files (load address 0x80198000) need 26-31 sectors. The largest original read into that address is 24 sectors, so whether the buffer can grow is not established.
- Overlays: in-place tables and extents total 52,748 bytes in the original and 50,156 in English. 235 records (1,091 bytes) overflow their in-place capacity and need the relink. C-literal growth per overlay is in `sizes.json`.
- Net growth fits the zero tail of the 0x30000 slot for all 25 overlays. The largest is SHOUGATU (+536 against 112,076 bytes) and the tightest is TACO (0 against 288). Symbols reach into the tail for EVENT, OLH and RENSYU.
- Main executable: C literals shrink by 664 bytes, and in-place data goes from 7,760 to 7,360 bytes. 49 table or extent records (148 bytes) overflow in place.
- Main headroom: 27,336 bytes lie between the bss end (0x8012B538) and the overlay load address (0x80132000). No main symbol lies there.
- C literals over all units: -3,120 bytes.
- SPEAKTST (out of scope): 4,009 records overflow in place.

## Assumptions and limits

- In-place capacity counts the zero bytes up to the next 4-byte boundary as padding. A byte-aligned object there would be overwritten; the relink avoids the question.
- Supplemental pointer words are found by value, and a data word that happens to equal base + offset would be rewritten. Every string had at least one such word, and no other words were checked.
- The gap scan is a heuristic (font glyph pairs, at least 3 distinct kana, kana at least 30 %, full-width Latin under 30 %).
- Load-buffer capacity is not proven. The report gives each file's own read sizes and the largest original read into the same address.
