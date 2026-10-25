---
type: tool
updated: 2026-10-25
sources: ["src/main/scene_library.c", "src/main/text_system.c", "src/main/menu_speaker_girl.c", "src/ovl/name_entry/name_entry.c", "tools/assets/text.py", "tools/assets/text_export.py", "tools/assets/script_profiles.py", "tools/assets/text_source.py"]
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
  python3 -m tools.assets.text_export disc/files --output .asset-cache/text/basic
```

The CLI prints counts; the catalog contains UTF-8 text, exact raw bytes, commands and provenance.
`tools.assets.text_export.export_text(input_dir, output_dir=None, profiles=None, *, include_source=False, source_repository=None)`
returns a JSON-compatible catalog. The input is an extracted ISO root such as `disc/files`. Without an
output directory it returns data without writing. Files are read-only and JSON output is deterministic.
The compact JSON avoids repeating indentation for tens of thousands of records.
`--source-repository` selects an explicit source-evidence tree. Current profiles target the stable
Phase 2 source at `d7fb33f960395d4c32ea238aabda82f211689973` (reviewed C/header revision
`182af9d13`, accepted build-source digest `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`).
They retain original executable/overlay hashes and VMAs. Source, header, table and instruction pins
are checked before accepting the corresponding source context; another source revision needs a
reviewed rebase. Logical C folders are resolved to EXN units through configured overlay ownership.

A fresh clone needs the user's PlayStation-the-Best input under `game/` and Docker. Follow
[[build-system]] for release verification, extraction to `disc/files`, and generation of the matching
assembly. The basic export above omits C/AST/MIPS/runtime enrichment. Without selected source
evidence, fixed-table proofs report that source files were not rechecked; the stage reader requires
its pinned source files. A basic export does not reproduce the full validated source census.
Original inputs and all exported payloads remain gitignored.

After generating assembly from the exact current source with the pinned Docker tools, run:

```sh
docker run --rm --cpus=2 -v "$PWD":/work -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.text_export disc/files --include-source \
  --source-repository . --output .asset-cache/text/current-proof-run
```

The validated run used the read-only shared witness
`.asset-cache/shared/source-snapshot-phase2-687b5ce7`, with source-tree SHA256
`75eafb5958b44eef4c1ead490eea7bc06372d094e2cf939854a3ab7b67b638cd` and manifest SHA256
`3fede6401e79c19a0982a596899744433658f749c7702b7c6c28d55dd487c0eb`. Its 14,697 files include
437 current C files and 13,872 generated assembly files. This ignored witness and the original inputs
are not bundled with tooling. Export metadata records its revision, build digest and manifest hash.
The earlier f9c witness remains historical and immutable; it is no longer the target of current pins.
The validated Docker runtime is Python 3.10.12, pycparser 3.11 and gcc 11.4; no host dependencies
are required. The source-dependent readers recheck actual current code/data; snapshot creation alone
is not proof that a profile or a runtime route is correct.

Profiles use paths beginning with `CDROM/`. `--profiles` accepts a JSON list matching
`tools/assets/script_profiles.py`: `id`, `path`, optional `sha256`, `load_address`, and `sections` with
`id`, `text_table`, `command_table`, `voice_table`, `group_count`. Addresses are JSON integers.
Supplying a layout does not prove it is correct for another release. Built-in layouts are pinned to
original-file hashes and cite loaders/initializers; checksum mismatches fail clearly.

Create the compact review projection from a validated catalog:

```sh
docker run --rm --cpus=2 -v "$PWD":/work -w /work tokimemo-decomp-arm64 \
  python3 -m tools.assets.text_units .asset-cache/text/catalog.json \
  --output-dir .asset-cache/text/units
```

JSONL and TSV retain stable original record identities, source bytes/hashes, protected control tokens,
inline-label text spans, source coordinates and review blockers. Every target remains null. TSV source
text is JSON-escaped inside its CSV-quoted field, so tabs/newlines/NULs do not split rows. Empty slots,
conditional runtime variants and unresolved storage/rendering semantics remain explicit. Original slot,
copy, row and pixel capacities describe the original layout; they impose no future translation limit.
The projection and its manifest stay under `.asset-cache/text/`. Script group-end records and zero-only
padding extents are skipped with counts; a physical source extent and its C use site remain separate
units with proven parent links where available. Review readiness is per unit and never a declaration
of comprehensive source or runtime completeness.

Readers:

- `text.parse_script(data, path, profile)`: linked dialogue tables.
- `text_fixed.tokenize_fixed_text(raw, renderer, context=None)`: direct glyph loading, `sndi`, `sndisp`, printf syntax.
- `text_source.inventory_source_text(repository, files_root=None, output_path=None)`: executable/overlay source literal inventory and reference evidence.
- `text_mips.enrich_mips_context(inventory, repository, files_root)`: original-instruction evidence for constant display, format and diagnostic arguments.
- `text_binary.enrich_binary_text(inventory, repository, files_root)`: derive fresh MIPS arguments, then recover bounded initial strings emitted through numeric directives.
- `text_tables.parse_character_lists(data, profile=None)`: external name-entry character lists.
- `text_date_tables.parse_date_tables(data, profile=None)`: DATE place and gift slots with source coordinates.
- `text_ui_tables.parse_ui_tables(data, profile)`, `verify_ui_source_proof(...)`: main/TEL profile UI fields, original loader/display instructions and source pins.
- `text_overlay_tables.parse_overlay_fixed_tables(data, profile)`: seven overlays' fixed strings, including flat `DateTxt` gift objects and fortune labels.
- `text_bunka_sd.parse_bunka_sd(data, repository, profile=None)`: source-pinned stage-show pointer tables, protected prefixes, direct text rows and voice-pair metadata.
- `text_renderer_proof.verify_renderer_source_proofs(source_root, original_root, evidence_path=None)`: original instruction and source checks for scoped custom text readers.
- `text_substitutions.verify_symbolic_substitution_contract(repository, files_root)`: original storage and reader proofs for symbolic name/context substitutions.
- `text_rpg_buffers.parse_rpg_initial_buffers(data, repository, profile=None)`: five source-pinned original initial mutable slots; runtime values and reads remain unproved.
- `text_units.export_units(catalog, output_dir=None, *, repository=..., formats=("jsonl", "tsv"))`: compact UTF-8 units with raw/control evidence, stable identities, null translation targets and explicit review flags.
- `text_font.font_metadata(data)`, `font_reference(raw, metadata)`: original font table and bitmap references.
- `script_validation.validate_script(data, path, profile, parsed=None)`: independent structural and semantic checks.
- `script_coverage.audit_all(data_root, repo_root=...)`: source-root and unclaimed-region diagnostics.
- `script_mutations.enrich_script_mutations(catalog, repository, files_root)`: pinned runtime patch slots, separate conditional views, unresolved source-write candidates.

## Custom readers and stage-show evidence

Reader roles include every source-proved string argument, scoped to its caller's link unit. The main
title wrapper forwards argument zero to the fixed glyph renderer; `x_taku_string_set` reads arguments
zero and one, and reads argument two only when argument three is three and that pointer is non-null.
RPG_BAT's `func_8013E934` reads argument two. A same-numbered overlay symbol does not inherit that
role. C sites retain conditional-read facts; MIPS facts retain argument-register and guard witnesses.
Unimplemented talk-renderer fixed views retain an explicit null renderer and a review flag.

Custom-reader proofs check the frozen C/ASM pins and original image bytes. Accepted MIPS witnesses
also bind the callee to its source declaration or glabel entry, replay unique instruction PCs, require
the call and its immediate delay slot, and retain unknown guards as conditional. A known-false guard
does not establish a string read. Symbolic S/O/W/A/M substitutions bind to five original zero-filled
buffers totaling 104 bytes; their runtime contents and effective read lengths remain unknown.

The BUNKA_SD reader separates 214 one-byte-prefix records from 72 direct text rows. The source
passes prefixed bodies to display before testing the prefix; a termination prefix therefore does not
justify dropping its body. It preserves row-break prefixes, voice-pair words, source NULs and all
uninterpreted bytes without inventing a row-to-voice assignment. Its 25 source pins and 1,453 original
instruction words are checked on each parse. Catalog links require an exact file/hash/address and
body or protected-storage byte match. Translation units contain body text and protected full-storage
evidence separately. Full-file reconstruction and runtime reachability are different gates.

## Coverage matrix and remaining gates

| Source/category | Current coverage | Completion evidence or remaining gate |
|---|---|---|
| `CDROM/SERIFU/DATE_*.BIN` (11) | All 293 source-initialized and consumed triads | Six omitted roots per girl were found by the source audit and added; existing IDs retained |
| `CDROM/SERIFU/EVENT_*.BIN` (12) | All 79 source-initialized triads | 47 directly consumed and 32 without a direct assignment; both sets retained |
| Other SERIFU dialogue (21, including P_BIRTH) | Pinned linked-table profiles | All source initializer roots match the profiles |
| `CDROM/SERIFU/TAKU3.BIN` | 40 numeric choice-result tables | Typed numeric data, 668 padding bytes and checked-CD trailer; no missing dialogue parser |
| `CDROM/BUNKA/*.BIN` (46) | Pinned linked-table profiles | 50 initializer functions yield 46 unique triads; duplicate roots retained in the audit |
| `CDROM/MASTER/*.BIN` (9) | Eight linked-table profiles: 326 groups and 815 text records; TEAR has a separate source-backed actor descriptor | Exact original hashes and initializer roots verified; actor usage does not prove whole-file text absence |
| `CDROM/SPEAKTST/TEST_*.BIN` (12) | File/hash inventory, unsupported linked layout | No source-backed talk roots; OPTION's audio-table load does not establish these files as dialogue |
| DATE place/gift dictionaries | 22 place slots and 99 gift slots, all 33 bytes wide | Includes reserved place slot and six empty gifts; source-derived girl/year/answer indices, slot padding and trailing assembly bytes retained |
| Main/TEL profile display fields | 26 main slots and 137 JOHO slots | Source pins, 1,689 loader/display instruction words and anchored main data labels verified; three whitespace-only slots preserved |
| Other overlay fixed tables | 587 slots across seven EXNs | Includes five 99-slot gift copies and two SHOUGATU fortune tables; 30 empty slots preserved; source storage views do not invent renderer semantics |
| BUNKA_SD stage-show strings | 286 body views in 11 source-backed tables | 214 protected-prefix records and 72 direct rows; full storage, NULs, pointer words and uninterpreted bytes retained; runtime reachability unproved |
| Executable + 26 overlays | Source literal/label inventory and original-byte verification | Ambiguous references, inline construction and non-string directives remain explicit audit work |
| Menus/UI, names, places, system/options/debug | Included in the source/context inventory | Context classification and fixed-renderer joining are under review |
| `CDROM/FUKUTEST/NAME_KAN.NBN` | 45 counted lists, 5,414 glyph entries | All offsets/counts/terminators and strict Unicode/font links verified |
| Runtime player names and context substitutions | Symbolic control tokens and source bindings | Runtime user values are not fabricated or translated |
| Original font | 46 range entries, 3,418 12 x 12 glyphs | Each code unit can link to its original 18-byte bitmap extent |
| Japanese text baked into images | Separate media lane owns asset/region catalog | Font IDs can be joined; this reader does not claim image/OCR completeness |

Current script-root audit: 98 containers, 654 sections, 42,369 records and 30,851 text records.
The integrated executable/overlay inventory contains 8,837 original-byte-verified extents; 1,877
remain semantically unknown. All 437 C files parse in the AST pass with stable source inputs. The
current full text suite has 257 passing Docker tests, including source-root integration and isolated
synthetic proof regressions. These counts do not clear the remaining context and runtime gates.
There are no unprofiled SERIFU or BUNKA files. Every script string strictly decodes and re-encodes as
Shift-JIS. Unclaimed byte counts and diagnostic candidate scans are retained as leads; zero candidates
is never treated as proof of completeness. Final completion requires the source/root/context audits and
independent review, including the image-text lane's separate inventory.

The preceding semantic milestone was integrated by the parent in `c56f9dcd4`; its combined
media/text suite passed 188 tests with one optional integration skip. The newer validated export
contains 1,069 dictionary slots, one actor-resource descriptor, 12 unsupported SPEAKTST files and
no missing inputs or structural failures. The source-root audit now
reports MASTER's eight primary literal initializers separately from the existing 21 other SERIFU
profiles. Its 168,596,252-byte catalog has SHA256
`2352e2bb723082e4d38997a92a1b83f234818622fdd5561daf11fc27265c11c7`.
All original extents, Unicode/token round trips, dictionary slots/tails and UTF-8 C locators verify.
The additional stage-show bank has 286 body views; its complete 196,608-byte input reconstructs
exactly from code proofs, tables, physical strings and preserved uninterpreted ranges. Every source,
renderer, substitution, stage-bank and mutation proof gate has zero blockers. Of 330 newly classified
source extents, 116 are display bodies and 214 are protected stage-show storage.

The wider facts-only fan-patch crosscheck now retains all 2,348 direct-string offsets and 545
byte-patch locations from 108 metadata files at revision `50dc2b9`. Of the direct-string locations,
the initial run matched 1,642 and identified 706 reader leads. Independently source-proved readers for
750 further slots now cover every one of the 2,348 direct-string starts, including original empty and
extra slots. Foreign offsets alone never establish a table profile or prove whole-source completeness.
Of the byte-patch locations,
306 match record starts and 239 do not; patches can also change instructions and non-string data.
No translation wording is retained or reused. The three New Year byte patches have original lengths
12, 16 and 20 bytes; the earlier crosscheck's 24/32/40 values counted hex characters.

The 170 remaining constant references outside initialized files use only addresses `0x800E6354`,
`0x800E635C` and `0x800E6364`. Frozen source maps these to the player family/given/nickname fields
at `game_state + 0xD4/0xDC/0xE4`. The other 28 unrecovered references point to initialized-zero
buffers. This distinguishes runtime values from missing nonzero static strings without closing
the runtime-construction audit. Independent review also corrected anchored BSS classification and
C source locators: byte offsets now count UTF-8 bytes, with character offsets preserved separately.
Those fixes are included in the validated current catalog. The first unintegrated overlay-table draft
used the wrong extracted revision; its pins and malformed-slot claims were discarded. The corrected
seven profiles use only `disc/files`, with all 30 source/header/assembly pins independently checked.

The compact projection contains 38,701 rows: 30,851 script records, 1,069 dictionary slots, 4,264
nonzero source extents, 2,201 C literal sites, 286 stage-show bodies, five initial-only mutable-buffer views and 25 conditional variants. All IDs are unique and
non-null; JSONL and TSV deserialize to identical values. The original-byte check covers 125 input
files with zero mismatches. It marks 31,346 rows ready for review and 7,355 flagged. Unknown overlay
renderer views remain flagged despite valid source Unicode, and every conditional variant remains
conditional. Completeness is false regardless of those per-row review counts.

## Current-source rebase verification (2026-10-25)

The current-source export preserves all 98 script profiles / 654 sections, all 8,837 physical source
extents, all 2,330 verified C identities and all 38,696 unit rows from the historical checkpoint.
Every original byte, source Unicode value, control-token payload, command/voice association, table
root and readiness/category result is unchanged. All 1,889 unknown extents remain unknown; rebasing
does not add semantic coverage. JSONL and TSV contain identical values and null translation targets.

One previously unresolved global C locator moves from `src/main/80075320.c:9` to
`src/main/holiday.c:52`; its raw bytes and Unicode are identical. It was not a stable physical/function
identity. The comparison records this one explicit locator migration rather than inventing binary
provenance. Other differences are current paths, source evidence, labels and display-symbol names.
Fifty-one script-unit token dictionaries update symbolic-buffer provenance only: addresses, offsets,
104 original zero bytes and effective-read-length limits are unchanged; physical IDs are now explicit
VMA-based legacy labels. No translation or runtime values are generated.

The rebase caught and fixed two actual discovery regressions: the matching-function index still
expected old overlay folders, and root-level SDK assembly needed exact main-YAML ownership.
Regression checks restore original function/diagnostic evidence, validate all configured C units,
reject traversal and escaping configs, and invalidate cached ownership when config bytes change.
The completed run has zero byte, Unicode, token, source/root, AST or proof blockers. Separate current
source/code/data replay reports support the metadata comparison; comparing metadata alone is not an
instruction proof. Comprehensive coverage and the twelve SPEAKTST layouts remain open.

## RPG original initial buffers (2026-10-25)

`initial_runtime_buffer_banks` describes five original initialized 40-byte slots selected by five
verified pointer words. Each actual slot contains 36 source bytes, a protected NUL and three protected
padding bytes. The reader rechecks 56 source/header/ASM/config pins, 604 original instruction words
and the exact initialized slot/pointer bytes. It preserves 220 selected data bytes; unclaimed ranges
carry offsets, lengths and hashes. This is a bounded reader, not a whole-overlay codec or absence proof.

These are mutable initial file views. The renderer is a tokenizer model with an explicitly potential
source-reader association. Runtime contents, actual reads, execution and overwrite/display lifetime
remain unproved. No text category is promoted: the five existing source extents stay unknown. Their
exact source joins add provenance only. Five new `initial_runtime_buffer` units remain flagged even
when their Unicode and glyph tokens are valid; every target remains null. The 38,696 preceding rows
are preserved exactly apart from those five added provenance contexts. Original capacities and glyph
counts are provenance, not translation limits.

The source audit also corrects the earlier SPEAKTST interpretation. OPTION's 58-sector read at
`0xA1AB` maps to `FUKUTEST/XA.NBN`. GYOZI references into TEST-file sectors call PCs `0x80051DD8`
and `0x800504CC`, which are interior addresses in the current `xa_wait` and `get_near_menu` functions.
Those call arguments do not establish a loader ABI. The earlier actor-resource description was not
proved; all twelve SPEAKTST layouts remain unresolved, with original hashes/extents retained and no
whole-file text-absence claim. No English-patch wording is used.

## Shooter native display-input cohort (2026-10-25)

`shooter_renderer_contract` verifies the native byte-atlas reader `shooter_draw_text` in TACO.
Its 53 current C/header/ASM/config pins include all 51 transitive C/header dependencies. All 161
original function instructions and six decoded byte-read facts verify: argument zero supplies the
byte pointer, the loop advances one byte through NUL, and the function selects sprite UVs and
submits draw primitives. The immutable read spec is bound to the verified entry `0x801432F0`.

The exporter supplies this additional role only after the selected source and original-code contract
passes in the current invocation. The role is scoped to TACO, the function and its exact entry;
general C consumer tables remain unchanged. Original LUI/ADDIU argument setup, JAL targets and delay
slots independently verify twelve exact source-extent addresses. Those twelve existing physical
records gain a potential static display-input classification, reducing unknown extents from 1,889
to 1,877. Failure to verify the contract supplies no role and retains a completeness blocker.

The native glyph model remains unimplemented. There is no `set_kanji_string` alias, glyph/font
equivalence, runtime value, reachability or execution claim. All 8,837 physical payloads and IDs and
all C literal sites are preserved. The same 38,701 units remain, with twelve changed source contexts
and no added rows. Every row retains its previous readiness and null target; 31,346 remain ready for
review and 7,355 remain flagged. The twelve source units remain flagged with unavailable renderer
tokenization and their existing context constraints. Original capacities remain provenance, not
translation limits. Comprehensive coverage remains false.

## Linked script format

`talk_run` (`src/main/scene_library.c`, original object VMA 0x8007C030) reads three parallel two-level tables:
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

The glyph loader (`src/main/text_system.c`, `load_string`) consumes pairs and rejects codes below the first
font range. Source/build encoding is Shift-JIS (`tools/cc.py` also uses `shift_jis`). Unicode output uses
Python's strict mapping and requires exact re-encoding. There is no CP932 fallback; differing CP932 mappings
are exposed as alternatives. Unknown bytes never become replacement characters. The strict source view,
annotated renderer view and original bytes remain separate, and raw bytes are authoritative for rebuilding.

Three original tab bytes are malformed inputs to the renderer, not a newly discovered tab command:
AS_S6W offsets 11084/11184 and HI_S6 offset 23438. Source Unicode preserves the tabs. The renderer scan
advances by two and the glyph loader rejects the resulting `0x09xx` code; later invalid units are an
alignment cascade. No bytes are deleted or repaired. HI_S6's affected group is explicitly source-reachable.

`file_checksum_ok` checks a byte sum separately from the ordinary checked-CD loader's trailer marker
(`src/main/cd_queue.c`). FU_S6 has an original sum mismatch with a valid marker; the reader reports both
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

With `--include-source`, the working exporter also rechecks MIPS instructions and constant arguments
against the original binaries. Each potential callsite records the argument register, contributing
instructions, delay slot, caller and callee. It promotes only exact extent starts with one known role.
Interior pointers and constant targets outside the inventory remain separate unresolved evidence.
The analysis covers display, format and diagnostic calls; C-string library reads are outside this
MIPS pass. It does not propagate mutable memory, follow branch paths, prove execution, or establish
the read length. Source context additionally requires typed AST value flow at the exact source call,
including argument index and structural selector path. Stable source/header fingerprints reject stale
reports. Struct members without a proved offset and unresolved expression types remain unclassified.
Of 2,331 C literal sites, 2,330 have stable function-address/ordinal identities independent of C filenames;
one global site retains an explicit unresolved identity. Original-binary text record IDs remain stable.

The earlier unresolved-target audit found 207 potential call arguments outside the string-directive inventory:
169 outside the initialized input and 38 inside it. Of the latter, 28 point at initially zero buffers;
10 calls address six nonempty label starts. Four UI labels (`D_800B5E25`, `D_800B5E3E`, `D_800B5E57`,
`D_800B5E89`) and two SDK diagnostic labels (`D_800DE2E0`, `D_800DE311`) are emitted with numeric
assembly directives. The independently reviewed supplement recovered all six records from ten call
references, with zero strict decoding failures. It re-derives MIPS facts from original instructions and
accepts only exact label starts whose contiguous initialized directive bytes and terminator are verified.
Its original MIPS-stage ledger stays intact. The scoped RPG_BAT reader adds one reference at call PC
`0x801525CC` to the runtime player-name field `0x800E6354`; the refreshed ledger therefore has 208
potential calls and 198 unresolved calls after the ten recovered references. This is another reason
a string-directive scan alone is not exhaustive.
The supplement covers exact constant MIPS arguments and complete AST-proved pointer paths to initialized
data labels. It rechecks every original pointer word and the exact terminal label prefix. Existing
leaf extents retain fresh semantics on repeated runs and lose them when source proof disappears.
Inline array strides, opaque helpers and runtime read lengths still need separate closure evidence;
recognizing a table or renderer alone does not close them.

The runtime mutation pass validates four pinned source contracts across DATE, DATE2, SHUGAKU and
SHOUGATU: 25 target views, each with separate conditional text/token evidence, including a command-byte
change. The New Year selector is narrowed to eleven normal stage-two roots under explicit entry/state
assumptions; 22 original function extents and 63 dispatch words verify the route evidence. This does
not prove runtime execution. The 437-file C scan parsed 293 candidate files and explicitly skipped
144 by its prefilter, with zero parse failures. It retains 2,141 primary unresolved write candidates,
1,381 opaque value dependencies and 131 address dependencies; those counts may overlap and are not
claims that every candidate mutates dialogue.
Alias closure and syntactic guards are diagnostic; fallthrough, goto, opaque helpers and assembly writes
remain limits. Original records, bytes and ignored storage after an inserted NUL are preserved.

The original font bank lives in `SLPM_86.053`: ranges at VMA 0x800B402C and bitmap bytes at 0x800B6D50.
Stable IDs `font:SLPM_86.053:gNNNN` identify glyphs; source SHA256, byte offset, 18-byte stride and 1bpp MSB-first
12 x 12 geometry are in catalog metadata. The game uploads padded 16 x 12 blocks. Script/character-list
units link to those IDs, allowing the media catalog to share exact font identities. Other baked lettering
needs its own media asset/region evidence; a script or font match does not establish bitmap lettering.
Font linking also covers source renderer views, inline speaker-label units and conditional runtime views
after enrichment; raw token byte sequences remain unchanged.

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
| `CDROM/MASTER/EN_GP.BIN` | 1 | 33 | 81 | 0 |
| `CDROM/MASTER/G_CON.BIN` | 1 | 31 | 77 | 0 |
| `CDROM/MASTER/KOSIEN.BIN` | 1 | 28 | 30 | 1 |
| `CDROM/MASTER/MOKUTEN.BIN` | 1 | 31 | 77 | 0 |
| `CDROM/MASTER/NOBEL.BIN` | 1 | 32 | 77 | 0 |
| `CDROM/MASTER/SUI_CON.BIN` | 1 | 33 | 87 | 0 |
| `CDROM/MASTER/TAI_MAS.BIN` | 1 | 106 | 309 | 0 |
| `CDROM/MASTER/UEKI.BIN` | 1 | 32 | 77 | 0 |

`CDROM/SERIFU/TAKU3.BIN` is the separately typed numeric choice-result file.
`CDROM/FUKUTEST/NAME_KAN.NBN` contains the 45 external character lists.
Main and NAME_ENT typed term dictionaries add 198 source-backed name/place/club/grid/preset records.
DATE adds 121 slots, main/JOHO profile fields add 163, and the seven overlay readers add 587,
for 1,069 dictionary/storage slots. MASTER/TEAR has a separate actor-resource descriptor; the twelve
SPEAKTST inputs retain unsupported layout status. No whole-file text absence is inferred from actor use.
