---
id: T-28001
title: Export text and dialogue for translation
status: In Progress
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-24
links: ["[[asset-export]]", "[[disc-layout]]", "[[tickets/T-0002-disc-extraction-and-exe-identification]]"]
---

## Goal

Recover text/script encodings and control codes from source, implement a verified export with stable IDs/source locations and machine-readable commands, without losing unknown bytes or committing game text.

## Acceptance criteria

- [ ] Every text-bearing format/region has source-backed coverage: all SERIFU files, other scene containers, executable and overlays.
- [ ] Comprehensive coverage matrix includes dialogue/choices, menus/UI, names/places, system/options/debug and control/substitution context.
- [ ] Deterministic complete text catalog has hashes, offsets, stable IDs, exact original bytes and explicit rendering/control semantics; unresolved cases block completion.
- [ ] Synthetic tests and full local real-input validation run in Docker; no copyrighted assets or game text committed.
- [ ] Independent standards/correctness review of comprehensive coverage resolved before Done.
- [x] Preserve current decomp/build behavior; engine-specific code and full translation remain out of scope.

## Comments

2026-10-24 scope correction: the user explicitly requires comprehensive text handling for a full
future translation. The initial bounded subset and its clean review are an implementation checkpoint,
not completed acceptance. The 44 unsupported SERIFU files are blocking work. Continue through all
script/container regions plus executable/overlay UI, names, menus, system/options/debug text. Source
and bitmap catalogs must cross-reference text/font regions where proved; the separate media lane owns
the asset-by-asset Japanese-in-images catalog. Byte coverage alone does not prove text completeness.
Three disjoint lanes now trace DATE/EVENT profiles, remaining SERIFU profiles and executable/overlay
source-backed text. Parent extends the parser and maintains aggregate coverage. Status stays In Progress.

2026-10-24: User authorized agents for asset extraction/readers, especially translation-ready text and reusable modern exports. No direct Godot work. Root owns shared tracker/index/log and commits.

2026-10-24: Dispatched parallel chat 01a16f34-8c5e-7523-a4ff-82fed7cb1639. Engine-independent tools only; default real outputs under ignored .asset-cache; root integrates source/tests/docs.

2026-10-24 implementation checkpoint: `tools/assets/text.py`, `text_export.py`, `script_profiles.py`
and `tools/test_asset_text.py` add explicit linked-table reading and JSON catalog export; API and
source evidence in [[assets-text]]. Initial Docker synthetic suite 15/15. Bounded BUN_PC validation:
83 groups, 190 records, 107 text records, 0 unknown tokens; every text token sequence reconstructs
its source bytes. The verified example is in BUNKA (LBA 0x49BA), not SERIFU as the older spec says.
Remaining container layouts are explicitly unsupported. Independent Standards review passed with
no findings; correctness review and final inventory are pending. Integration owner maintains shared
index/kanban/log/status and moves the ticket only after review.

2026-10-24 final handoff: added verified `CDROM/SERIFU/P_BIRTH.BIN` layout, 13 sections/35 groups,
114 records/79 text records, alongside BUN_PC. Final Docker suite 18/18; real export 2 supported files,
44 explicitly unsupported SERIFU files, 0 missing. Combined 304 records/186 text records, no unknown
tokens or commands. All token byte sequences round-trip; repeated JSON export is byte-identical.
Real output is only `.asset-cache/text/catalog.json` (ignored). No build/clean, source-game edits,
commits, dependency installs or engine files were made by this lane.

Independent Luna Standards review: final pass, no findings. Independent Sol 6.1 correctness review:
three findings resolved and rechecked (two-byte stepping on reply n markers, requiring one full-width
glyph rather than two half-width codepoints, lexical cache containment against symlink ancestors).
No remaining significant findings within the documented subset. Ticket left at In Progress for the
integration owner to serialize shared board/log/index/status changes and completion.

2026-10-24 comprehensive continuation: source-correlated direct and indexed loaders now identify all
46 BUNKA containers, all 11 DATE and 12 EVENT files, and 20 other linked SERIFU containers plus the
previous P_BIRTH profile. All 45 SERIFU files are classified (44 scripts, TAKU3 numeric choice data),
90 script files overall, zero unprofiled files. EVENT needed its actual consumed per-girl triples;
testing unused initializers against the wrong girl's file gave a misleading first-pass failure.
This is script-container progress, not a comprehensive completion claim. New source inventory,
font-to-script bitmap links and runtime/context validation are under active review. Three original
tab bytes in AS_S6W/HI_S6 cause known renderer errors; raw bytes and strict source Unicode remain
separate from the faithful renderer view, with no replacement-character decoding or data repair.

2026-10-24 deeper audit: the first all-container pass still omitted six source-consumed DATE roots
per girl; those are being added with stable IDs rather than treating a successful file parse as
completeness. The external NAME_KAN.NBN reader now validates 45 counted lists/5,414 full-width entries
and their relative relocation table; all entries link to 3,415 distinct original font glyphs.
The font bank has 3,418 glyphs and the catalog exposes stable font/source-offset IDs for media links.
Source Unicode, renderer behavior and printf/fixed-line control views remain distinct. Expanded
synthetic parser/font/name-table/fixed-renderer/validator suite: 48 passing tests in Docker. Source
literal/reference audit and orphan text/root audit are still active completion blockers.

2026-10-24 integration freeze requested by root: prepare a source-only, SHA-manifested reviewed
checkpoint for the parser/profile/dictionary/font/source-byte-inventory tools. T-28001 remains In
Progress; semantic context, AST, MIPS and runtime mutation work continue separately. The checkpoint
exporter deliberately omits unfinished semantic-context enrichment and keeps completeness false.
It does not include the common media CLI or extracted game text/assets. Source-root integration
tests use the explicit `TOKIMEMO_TEXT_SOURCE_ROOT` read-only evidence snapshot rather than requiring
copyrighted fixtures in the tooling snapshot. Final checkpoint paths/hashes/tests are in its manifest.

2026-10-24 frozen integration milestone: `.asset-cache/text/tooling-milestone-04c895babbe95133/`
contains 24 source/test/docs files plus numeric review metadata, with source-set SHA256
`04c895babbe9513338fd38c789eb13afa3dfe3d78ea4e8567418e96821850e15`. Independent isolated Docker
rerun passed 75/75. The frozen exporter intentionally defers semantic enrichment and reports
completeness false. Its real-input validation verifies 90 scripts / 646 sections / 11,192 groups /
41,228 records / 30,036 text records, token raw round-trip, zero structural/source decoding failures,
45 external character lists / 5,414 entries, 198 typed dictionary records, 3,418 font glyphs, and
one numeric choice-data file. It contains no extracted game assets or real text fixtures. Root
owns the commit and common CLI alignment; ongoing edits remain in the working tree.

Continuation after freeze: static MIPS evidence validates 612,332 instruction PCs and identifies
1,217 potential text-call contexts (1,165 records), with explicit call-register/delay-slot evidence,
207 unresolved constant targets and no memory-value guessing. AST processing parses all 437 C files
from the frozen source-evidence tree; 524 known-role candidate arguments include 156 unresolved
expressions. Both tools are outside the integration checkpoint. Semantic role/dataflow, typed
pointer selectors, runtime text mutations and bitmap-region completeness remain completion gates.
The DATE/DATE2/SHUGAKU/SHOUGATU byte/command patches are now being recovered rather than assuming
static strings alone preserve every runtime text form.

2026-10-24 continuation integration: the working exporter now includes the independently reviewed
MIPS call-argument evidence and reports blocked proofs/unresolved constant targets in its completion
metadata. The sealed 24-file milestone is unchanged. Focused Docker tests: 17/17, including an invented
export integration regression. The MIPS scope is display/format/diagnostic calls, with read length and
runtime reachability explicitly unproved. AST flow/type gates and runtime mutation inventory continue.

Expanded working-tree regression suite: 106/106 tests pass in Docker, including the frozen source-root
audit and synthetic AST/context/MIPS/mutation cases. This test count includes work beyond the sealed
checkpoint. The continuation review identified a remaining semantic distinction between a symbol used
as an index and a pointer value passed to a consumer; the source/AST context gate is being tightened.

The numeric MIPS target audit found six nonempty initialized text targets absent from string-directive
inventory: four UI labels and two SDK diagnostic labels emitted through numeric directives. Ten callsites
reference those targets. Of 207 unresolved call targets, 38 are inside initialized inputs and 169 outside;
the other 28 initialized targets start with zero and remain runtime-buffer evidence. A supplemental
reader will require exact original-byte, label-start, span and terminator proof, with no guessed byte scan.

Independent review passed for the runtime mutation reader: all seven source pins match the frozen
evidence tree, nine synthetic tests pass, and no actionable fidelity findings remain. It reports four
contracts / 29 target views and 14 separate conditional text views, while retaining the SHOUGATU root
union as unresolved. The 437-file candidate scan records 2,542 unresolved possible writes and explicit
prefilter/alias/control-flow limits. Working exporter now includes this report and fresh AST-gated
context analysis. Font links now include enriched source, inline-label and conditional runtime tokens;
the independent review found no incorrect links, and nine font/export tests pass.

Next-packet review: source/AST contexts, stable function-address C identities, font traversal, runtime
mutation contracts, numeric-directive supplement and final exporter integration have independent
reviews with no remaining actionable findings. The supplemental API now derives its own MIPS facts
instead of trusting supplied argument metadata. Full current suite: 123/123 tests pass in Docker,
including cross-hash-seed determinism and stale-AST rejection. The integrated original-input run is
being validated before freezing. Remaining work includes indirect/table/inline-array numeric text,
197 unresolved constant calls, source-context gaps, 2,542 possible runtime writes, the SHOUGATU selector,
live-tree mutation pin adaptation after C renames, and the separate bitmap-text inventory.

Integrated original-input validation passed: 90 scripts / 646 sections / 41,228 records / 30,036 text
records; 8,837 executable/overlay records including six supplemental strings; 437 AST files with zero
parse failures and stable inputs. Every checked source extent, source Unicode view and token sequence
passes byte/encoding round-trip checks. There are zero byte/context/MIPS/supplement proof blockers,
one runtime selector blocker, 197 unresolved constant calls, 2,261 semantically unknown records and
2,542 possible writes. The 169,742,581-byte ignored catalog has SHA256
`ea9fe4a66feb1c85e978d59d0c6e029bf27cd5cd82cb96395f7d572e18681619`.
Completeness remains false. Numeric verification metadata is in `.asset-cache/text/semantics-validation/`.

The unpublished semantic packet `039813e51e678e31` was superseded during post-seal review: implicit
global pointer loads needed exact slot-zero selection; context-owned category promotions needed
revocation on fresh reanalysis; and cast facts needed to avoid implying pointer flow for unresolved
or floating/narrowing conversions. Focused regressions now cover all three. The earlier committed
byte-fidelity milestone is unaffected. A fresh integrated run and replacement immutable packet follow;
the superseded packet's bytes remain untouched and must not be integrated.

Replacement validation passed with the conservative modeled-expression subset: 129/129 tests,
independent AST/context review 18/18, and the full original-input export again has zero extent,
Unicode, token-round-trip or proof blockers. Positive contexts require the full selector depth,
one unambiguous value-flow occurrence, argument-value selector facts and matching modeled dereferences;
inline strides/unary dereferences remain unresolved. Final counts are 8,837 source records with 2,267
unknown, 197 unresolved constant calls, four mutation rules / 29 targets / 14 conditional views,
one selector blocker and 2,542 possible writes. The replacement ignored catalog is 169,546,357 bytes,
SHA256 `c2aee8bb3df203d63680311ebd752a0d97dfa40cf3df66174362a914ca8d8d5c`.
The 90 scripts / 646 sections / 41,228 records / 30,036 text records and all 437 AST parses remain
unchanged. This supersedes the previous semantic catalog hash, and completeness remains false.

Replacement packet sealed and independently packaging-audited:
`.asset-cache/text/semantic-milestone-a86733b918fce07a/`, 34 source/test/docs files, source-set SHA256
`a86733b918fce07adad19f6d2b9bdbd8aad682b3244d8c97532811d9b49aa6c7`, manifest SHA256
`a6ed5f78b3f75a777cea16a70904fdfb917b1044becd318b04dd893a4d3cc4d0`.
Exact frozen sources passed 129/129 tests from an ephemeral writable Docker copy; packet, repository
and evidence were mounted read-only. Every listed file/checksum verifies, there are no unlisted
assets/text dumps, and the common media CLI is excluded. Root owns Git and shared tracking updates.
Work continues beyond that immutable packet on indirect-pointer leaves and candidate-write value flow.

Parent integration recorded the reviewed semantic packet in `c56f9dcd4`; combined media/text checks
passed 188 tests with one optional integration skip. Working family integration now includes eight
MASTER talk inputs, a separately classified TEAR actor resource, 12 SPEAKTST files with unproven
layouts, and DATE place/gift tables. The family-only original-input run has 98 scripts / 30,851 text
records / 319 dictionary slots, zero missing files and zero structural failures. The MASTER initializer
audit and existing source-root tests pass 22/22 against the stable evidence snapshot. Full refreshed
source/context validation and independent integration review remain pending; this ticket stays In Progress.

The refreshed family/source catalog passes all original-byte, Unicode, token and dictionary-slot
checks: 98 scripts, 654 sections, 42,369 records, 30,851 text records, 319 dictionary slots and 8,837
source extents. It has zero parse/proof blockers and 25 conditional mutation views, but completeness
remains false (2,220 unknown extents, 2,136 primary write candidates, 1,381 opaque dependencies and
131 address dependencies, with possible overlap). The unsealed catalog hash is
`8acd47be9b2fc852505a9928fada3b8a8fa44ff51cbf3082e854b37f2ef0ecc1`.
Do not seal that catalog: subsequent independent review fixed UTF-8 C byte locators and anchored
noload BSS classification; 55 focused tests and independent verification pass, and regeneration is due.

The full facts-only external-offset crosscheck is now reproducible from
`.asset-cache/reference-audit/fan-direct-offsets.json` (108 files, 2,348 direct-string sites, 545
byte-patch sites; no wording). Numeric correlation finds 706 unmatched direct-string locations,
mostly fixed-width overlay dictionaries plus JOHO/main metadata. Additional source-backed readers
are in progress. All 169 out-of-initialized-file constant references are the three runtime player-name
addresses; 28 other references start at initialized-zero buffers. Neither count represents newly
discovered nonzero static dialogue. The original 633 BUNKA/SERIFU local-only offsets reconcile as
303 empty, 326 punctuation/symbol-only, one symbolic control and three already-known tab anomalies.

The corrected additional readers are independently reviewed: 163 main/JOHO fields and 587 slots
across seven overlays, including five 99-slot gift tables and two SHOUGATU fortune tables. All original
hashes, 30 overlay source pins, source dimensions, normalized year formulas and bounded ASM bytes
verify. An unpublished draft had read `versions/extracted/rev1` instead of `disc/files`; its wrong
pins/invalid-slot claims were discarded before integration. Correct original inputs have 30 empty
overlay slots and zero decode failures. All 2,348 external direct-string offsets now match exact local
record starts; 239 unmatched byte patches remain locator leads, including instructions/non-string data.

Final integrated original-input validation passes with 98 scripts / 654 sections / 42,369 records /
30,851 text records / 1,069 dictionary slots / 8,837 source extents. There are zero raw-byte, Unicode,
token, slot/tail, UTF-8 locator or proof errors. Current catalog: 164,574,642 bytes, SHA256
`ab481208f943ed55f20154be6044807f519f14b9f90c5db3b3fc2bf40559f47c`. Unknown extents are 2,219;
runtime write/dependency counts remain 2,136 / 1,381 / 131 with possible overlap. Completeness is false.

All 185 text tests pass in Docker with the stable source-root integration audit enabled. Independent
Standards/Spec reviews resolved escaped assembly-path reads, repeated leaf recovery semantics, null
dictionary views and UTF-8 locators. Compact JSONL/TSV has 38,410 unique non-null unit IDs, null targets,
matching deserialized content and zero original-byte mismatches across 125 inputs; 31,031 units are
ready for review and 7,379 explicitly flagged. Output stays ignored. A source-only checkpoint follows;
parent integration owns Git and shared log/board updates. T-28001 remains In Progress.

Sealed and independently packaging-audited checkpoint:
`.asset-cache/text/semantic-milestone-2ba1410cda39a519/`, 44 source/test/docs files, source-set SHA256
`2ba1410cda39a5191ed720b5bd2da268b985d334fa2bdeff8dfac98c96e5e3ef`, manifest SHA256
`e3441de41b7e4f6194d71b11e86ff20c57baf9aea58d685c69cfe70ad240c3f3`.
Its exact sources pass 185/185 tests from an ephemeral Docker copy with packet/repository/evidence
mounted read-only. All 48 checksum entries verify; exactly 49 files include three numeric-only reports,
manifest and checksums, with no extra assets, copied ASM, text dumps, pycache or common media CLI.
The canonical catalog was promoted atomically. Parent has the concrete handoff in
`.claude/orchestrator/state/T-28005/text-lane-handoff.json`; the prior committed packet remains untouched.

Next read-only semantic audit: 1,471 of the 2,219 unknown original extents contain Japanese letters;
846 function-bound C literal sites give specific consumer leads. Largest unknown families are
RPG_BAT (319), BUNKA_SD (282), main (116) and EVENT (107). Copy calls account for 518 `strcpy`
sites; additional custom reader calls include `func_80078970` (107), `func_800BCE10` (33),
`func_8014D11C` (27), `func_80066104` (15) and `func_8013E934` (14). These are classification leads,
not missing raw bytes or a claim that every string is displayed. Source/ASM audits are in progress
before any role/tokenizer extension. Numeric-only evidence is
`.asset-cache/text/unknown-japanese-consumers.json`; the sealed checkpoint stays untouched.

The subsequent semantic work now has independently reviewed production tooling. BUNKA_SD has an
original-input reader for 11 tables / 286 body views (214 prefixed and 72 direct), with protected
prefixes/NULs, pointer and voice metadata, 25 source pins and 1,453 original instruction words.
Scoped custom-reader contracts cover the title wrapper, conditional multi-argument choice reader
and RPG_BAT display helper. Symbolic substitutions bind five buffers to 104 original zero bytes;
runtime values and effective read lengths remain unknown. Callee identity, original words, unique
PCs, call/delay witnesses and conditional argument reads are checked before accepting MIPS evidence.
Independent Standards/Spec reviews are closed with no findings, and all 214 text tests pass in Docker
with the pinned source-root audit enabled. Full combined original-input export and unit validation
are running; the sealed 2ba checkpoint remains immutable and authoritative until the refresh passes.

The refreshed combined export passes with 286 stage-show bodies and unchanged script/dictionary
coverage. Catalog SHA256 `8876c0e4861976198075272e3f9ede187b6c01723ab3c9c735b30b6aa33ee855`,
167,927,250 bytes, is atomically promoted. All original-byte, strict Unicode/token, slot/tail,
UTF-8 locator, AST and proof checks pass with zero errors/blockers. Independent BUNKA_SD validation
checks all 286 source links and reconstructs all 196,608 original bytes. Unknown extents fall by 330
to 1,889 (116 newly classified display extents and 214 protected stage-show extents).

JSONL/TSV have 38,696 unique non-null identities and identical values, null targets, and zero physical
byte mismatches across 125 inputs; 31,346 units are ready for review and 7,350 flagged. All 2,348
external direct-string locations still match. Wider reader coverage exposes five additional primary
write candidates (2,141 total) and one RPG_BAT call at 0x801525CC to the existing runtime player-name
field 0x800E6354 (198 unresolved constant references total: 170 runtime-name addresses and 28 original
zero buffers). Opaque/address dependencies remain 1,381/131 with overlap possible. Completeness
remains false; no future layout limit or runtime execution is inferred. Source-only packet sealing follows.
