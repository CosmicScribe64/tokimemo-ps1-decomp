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
