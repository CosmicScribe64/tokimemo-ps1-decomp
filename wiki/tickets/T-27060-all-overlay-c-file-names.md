---
id: T-27060
title: Name every overlay C file
status: Done
assignee: parallel-overlay-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Review and prepare logical role names for every C file across all 26 overlays. Reuse existing evidence/proposals, check file bodies, preserve each original object, avoid address/numeric/generic numbered names. Runtime-unconfirmed folder moves can remain deferred; C basenames still get evidence-backed technical role names.

## Acceptance criteria

- [x] Complete source-file census and role-backed proposed mappings, unique case-insensitively per folder.
- [x] Separate supported file-only tables, no mixed symbol/header-extraction/literal edits.
- [x] Independent review and all final exact-map dry plans pass.
- [x] Parent applies with view/hash proof and the required file-only commit.
- [x] No game-code C file remains address/number-named at final phase2 closeout.

## Comments

2026-10-24: User explicitly requires understandable names for all game-code C files and asked to start now. Root is sole source applicator; worker prepares/reviews new filename tables only. Existing reviewed symbol batches remain frozen.

2026-10-24 (census and allocation): Docker filesystem/configuration census found exactly 403 C files
in all 26 overlay folders, with zero unconfigured or missing objects. Three disjoint lanes own
137 scene/club objects, 136 action/system objects and 130 calendar/results objects. Each writes only
new `layout-<DISC>-files.tsv` tables and ignored scratch; this coordinator owns [[spec/file-naming-overlays]].
Original object boundaries/current folders are preserved and all prior tables are frozen.

TT's 31 files are explicitly a separate reviewable candidate. ADR-0001's disc-name map says
"agree with its contributor"; no contributor agreement or runtime confirmation is claimed. The
other 372 files do not inherit that TT gate. The integration owner must add the new spec's shared
index/log pointers and serialize eventual status/board changes; this lane does not edit shared wiki.

2026-10-24 (independent body review): The Sol 6.1 reviewer inspected every one of the 403 objects at
the file-role level and traced ambiguous dispatchers/resource binders through callers. Findings
required substantive corrections rather than just prettier spellings: BUNKA_SD's scene A/B/C were
replaced by the dispatcher-proven literature/computer/brass and science/drama club-year shows;
SHUGAKU birthday/trip-entry/destination objects were distinguished from generic callback setup;
SHOUGATU/GYOZI meeting, Christmas and girl-dialogue flows were separated using actual loaders and
girl selectors; TAIIKU's full festival entry and its talk/layer/picture binders were distinguished;
MASTER's binder consumers, OMIMAI's dialogue resources and ETC's calendar resume were corrected.
TACO/TT numbered or visually guessed identities were replaced with source-proven flow/motion/pool
roles. No copied game text was found in the new notes. Final exact-map dry plans and freeze audit
follow these naming/evidence corrections; earlier producer plans are not proof of revised mappings.

2026-10-24 (TT scope clarification from primary): The earlier note interpreted agreement as blocking
all TT basenames. Exact ADR-0001:249 text is in the English-folder column; GLOSSARY.md:269 reserves
the module stem. The 31 technical file-only rows preserve `TT`, assign no module prefix and make no
confirmed gameplay-identity claim. Neither passage explicitly blocks these basenames. Primary
decides their application within the user's existing all-C-filenames authorization; this worker
does not add an approval requirement or claim contributor agreement.

2026-10-24 (frozen preparation handoff): All 403 mappings in 26 new filename-only tables pass the
independent Standards and preparation-Spec reviews, with zero unresolved filename/evidence findings.
All 26 full verification-enabled Docker plans exit 0, print exactly the final mappings, and report
zero problems/advisories. The final parent audit has zero structural problems and verifies the four
frozen T-27051 tables were unchanged. Final table-manifest SHA-256 is
`45114816d9ee86011ea6cfa3b8db0af435fcb05543d0ecbc4d0545e60ba7fd55`; captured-tree manifest is
`5ddf8ff3d455b94fda3184ddf62eb342abdd79cc5a0a595a6a8c1d6614e31db2`.
Proof and review paths are in [[spec/file-naming-overlays]]. All 372 non-TT rows are ready for primary
integration; 31 TT technical candidates are separately reviewed for the primary decision described
above. Tables are frozen. No moves, source/header/tool/symbol/ledger edits, builds/clean, commits or
shared wiki edits were made by this lane. Primary live plans, view/hash proof, per-file commits and
final source-name census remain outstanding; status stays In Progress.

2026-10-24 root completion: applied all403file-only rows through rename.py, including source-proven technicalTTbasenames within unchangedTTfolder. Optimized full403liveplan0problems; applycheck1286ledgerrowsOK. Fresh437declaration payloads are exact after mapping403path keys in a separate snapshotcopy:0/0/0. CleanDocker27/27checks and6952functions/2276920bytes; independent27/27rehash. Post-application Luna Standards and Sol Spec reviews pass: all403bodies exact exceptfirstcomments/approvedpathreferences, all26YAMLoffsets/types/order/discfields andobjectmetadata preserved. All437gameCfilesnowlogical, zeroaddress/number-onlybasenames. Filename-onlysourcecommit follows; widerphase2semantic/headerwork remainsopen.

2026-10-24 commit granularity clarification: as in the independently reviewed two-file pilot and32-file main sweep, this is one file-only batch commit, separate from all symbol/header/literal changes. Original object histories are traced by exact mappings, firstcomments andconfigobjects; the criterion means file-only separation, not403individualcommits.
