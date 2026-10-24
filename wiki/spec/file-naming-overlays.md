---
type: spec
updated: 2026-10-24
ticket: T-27060
sources: ["src/ovl", "config/overlays", "config/objects", "docs/adr/0001-phase-2-naming.md", "CODING_STANDARDS.md"]
---

# Overlay C filenames

Current application state: all403overlaymoves are verified complete, alongside34mainobjects. The census/preparation tables below retain the pre-application mapping evidence; final root proofs are recorded at the end.

Preparation for [[tickets/T-27060-all-overlay-c-file-names]], following the user's requirement that
every game-code C file have an understandable basename. This page covers all overlay folders; main
files belong to the parallel main-file ticket. The primary chat alone applies moves and owns builds,
commits, shared wiki indexes/status/log and ticket status transitions.

## Census and boundary rules

The 2026-10-24 filesystem census has **403 C files in 26 overlay folders**. The configured C-object
set from `tools/srcscan.py` matches the filesystem exactly: no unconfigured or missing C file.
All 403 current basenames are address forms. Each original object remains intact; no function,
data, declaration or object-boundary extraction is part of this filename pass.

Every new table is `config/renames/proposals/layout-<DISC>-files.tsv`, with exactly five columns:
`kind`, `scope`, `old`, `new`, `note`. Rows use the supported `file DISC oldstem newstem evidence`
syntax and retain the current disc-named folder. There are no directory, header, symbol or literal
rows. Existing proposals, reviewed symbol batches and earlier file tables are frozen references.

Names must say what the C object does, using body/caller/data-consumer evidence. A copied proposal
name is only a lead. Mixed objects need a name that acknowledges their actual contents; small
resource binders are traced through their consumers instead of assigned generic ordinal names.
Names are checked case-insensitively inside each current folder. A technical role is preferable to
an unproved story or picture interpretation. Source evidence names labels and roles, not game text.

## Frozen mappings and dry plans

These 26 tables are the canonical mappings. Each verification-enabled Docker plan exits 0,
prints exactly its table's mappings, and reports zero problems/advisories. Changed paths include
configuration and reference rewrites, not just the C-file moves. All 372 non-TT rows are ready for
the primary applicator's live-state checks; TT's 31 reviewed rows are a separate integration decision.

| Table | C-file rows | Plan changed paths | Application state |
|---|---:|---:|---|
| [BUNKAKEN](../../config/renames/proposals/layout-BUNKAKEN-files.tsv) | 18 | 37 | ready |
| [BUNKASAI](../../config/renames/proposals/layout-BUNKASAI-files.tsv) | 20 | 41 | ready |
| [BUNKA_SD](../../config/renames/proposals/layout-BUNKA_SD-files.tsv) | 11 | 23 | ready |
| [DATE](../../config/renames/proposals/layout-DATE-files.tsv) | 19 | 40 | ready |
| [DATE2](../../config/renames/proposals/layout-DATE2-files.tsv) | 6 | 13 | ready |
| [ENDING](../../config/renames/proposals/layout-ENDING-files.tsv) | 8 | 17 | ready |
| [EN_NICHI](../../config/renames/proposals/layout-EN_NICHI-files.tsv) | 2 | 5 | ready |
| [ETC](../../config/renames/proposals/layout-ETC-files.tsv) | 13 | 28 | ready |
| [EVENT](../../config/renames/proposals/layout-EVENT-files.tsv) | 18 | 38 | ready |
| [GEKO](../../config/renames/proposals/layout-GEKO-files.tsv) | 22 | 46 | ready |
| [GYOZI](../../config/renames/proposals/layout-GYOZI-files.tsv) | 33 | 67 | ready |
| [KANGEI](../../config/renames/proposals/layout-KANGEI-files.tsv) | 8 | 17 | ready |
| [MASTER](../../config/renames/proposals/layout-MASTER-files.tsv) | 9 | 19 | ready |
| [NAME_ENT](../../config/renames/proposals/layout-NAME_ENT-files.tsv) | 4 | 10 | ready |
| [OLH](../../config/renames/proposals/layout-OLH-files.tsv) | 8 | 17 | ready |
| [OMIMAI](../../config/renames/proposals/layout-OMIMAI-files.tsv) | 4 | 9 | ready |
| [OPTION](../../config/renames/proposals/layout-OPTION-files.tsv) | 3 | 7 | ready |
| [RENSYU](../../config/renames/proposals/layout-RENSYU-files.tsv) | 2 | 5 | ready |
| [RPG_BAT](../../config/renames/proposals/layout-RPG_BAT-files.tsv) | 61 | 124 | ready |
| [SHOUGATU](../../config/renames/proposals/layout-SHOUGATU-files.tsv) | 27 | 56 | ready |
| [SHUGAKU](../../config/renames/proposals/layout-SHUGAKU-files.tsv) | 11 | 23 | ready |
| [TACO](../../config/renames/proposals/layout-TACO-files.tsv) | 38 | 78 | ready |
| [TAIIKU](../../config/renames/proposals/layout-TAIIKU-files.tsv) | 19 | 40 | ready |
| [TEL](../../config/renames/proposals/layout-TEL-files.tsv) | 3 | 7 | ready |
| [TT](../../config/renames/proposals/layout-TT-files.tsv) | 31 | 65 | separate technical file-only candidate |
| [VALEN](../../config/renames/proposals/layout-VALEN-files.tsv) | 5 | 11 | ready |
| **Total** | **403** | | **372 ready, 31 separate for primary decision** |

## TT and folder gates

TT is a separate candidate, not a claim of contributor agreement. ADR-0001's disc-name map at
`docs/adr/0001-phase-2-naming.md:249` puts **"agree with its contributor"** in TT's English-folder
column, leaves its prefix blank and marks **confirm**; `GLOSSARY.md:269` reserves the TT module
stem for agreement with the contributor. Neither passage explicitly prohibits technical C basenames
inside the existing `TT` folder. These 31 rows assign no English folder, module prefix or confirmed
gameplay identity. The primary applicator decides their application under the user's instruction
to name every C file. An earlier worker interpretation extended the agreement condition to all TT
basenames; that interpretation is not an additional approval requirement.

The runtime confirmation gates on other English overlay folders remain in force. These tables make
no folder move at all, so even a provisional folder such as `girl_encounters` is not adopted here.
EVENT/GYOZI basenames do not establish that their overlays are unused. No emulator confirmation is
claimed by the filename analysis.

## Validation and application contract

Each table received a full read-only Docker `rename.py plan` with a distinct per-overlay ledger
name and saved exit/row/file/advisory results. The structural audit matches all 403 source paths,
with no duplicate old path, missing file, extra row or case-insensitive target collision. The four
frozen T-27051 tables retain their census-time SHA-256 values. No new note copies game dialogue.

Independent review applied `CODING_STANDARDS.md`, ADR-0001, the glossary and the ticket/spec to all
403 objects at the file-role level: function/call/citation inspection for each, with full-body
targeted reads of ambiguous dispatchers and binders. This does not claim every function's internals
or runtime behavior was reviewed. Findings were resolved before the final table freeze. TT's
folder/module vocabulary agreement is distinct from these technical file-only rows.

The primary applicator must rerun live plans at the final source state, take the declaration-view
snapshot, apply through `tools/rename.py` only, preserve original address-range comments and prove
the clean matching build. Each C-file move is a commit of its own under the ADR. A passing plan is
not a claim of source application, declaration-view comparison or 27/27 SHA-1 verification.

## Review handoff and proof limits

The independent review corrected semantic errors before freezing names: club/year dispatch proves
the BUNKA_SD shows; birthday detours and whole-trip dispatch define SHUGAKU's mixed objects;
SHOUGATU girl indices distinguish meeting/Christmas objects; TAIIKU consumers distinguish talk,
layers and pictures; ENDING's script binding and player-parameter branches prove
`career_and_epilogue_summary`, rather than the old audio-level interpretation. TACO and TT use
observed motion, flow and pool roles where a visual identity was unproved. SHOUGATU's provisional
seasonal-party mode uses `girl_dialogue_dispatch`, `girl_dialogue_setup` and
`girl_dialogue_outcomes`; the filenames do not promote that story hypothesis to fact.

Authoritative local proof lives in `.claude/orchestrator/state/T-27060-review/`:
`final-review.md`, `findings.md`, `final-validation.json`, `snapshot-manifest.json`, and
`plan-<DISC>.log`. Independent verdict: **Standards PASS; Spec PASS for preparation; zero unresolved
filename/evidence findings**.
The parent audit is `.claude/orchestrator/state/T-27060-parent/final-structure.json`.
Older producer plans are superseded wherever mappings changed; they do not prove the frozen tables.
The final validation records each exact table SHA-256 and captured source hashes. Final table-manifest
SHA-256: `45114816d9ee86011ea6cfa3b8db0af435fcb05543d0ecbc4d0545e60ba7fd55`.
Final captured-tree manifest SHA-256:
`5ddf8ff3d455b94fda3184ddf62eb342abdd79cc5a0a595a6a8c1d6614e31db2`.

Plans run against a stable copy of `src/`, `include/`, `config/` and `tools/`, initially captured
while HEAD was `428be0a7122fef5aa7832bbaf27e23de757b1e1d`, with final table amendments refreshed.
Concurrent primary integration changed the live checkout, so this is a reproducible captured-tree
proof, not a claim about the later live HEAD. The primary must rerun its live plans and view/hash
proofs. This worker made no source/header/tool/symbol/ledger changes, applied no moves, ran no
build/clean and made no commits. The ticket stays In Progress until the primary finishes integration
and its TT integration decision.

## Root application and matching proof

2026-10-24: all403rows applied, retaining26disc-named directories. Togetherwith34mainobjects, all437game C files have logical basenames. Current437prototype views equal original payloads after403intentionalpath-keymappings:0/0/0. CleanDocker27/27matching checks and6952/6952functions,2276920bytes; independent rehash27/27. Independent post-application Standards/Spec reviews confirm pure approvedpathmoves, originalcomments/ranges andunchangedbodies/configboundaries. TT technicalbasenames do notadopt anEnglishfolder/moduleprefix. Earlierpreparation/source-unchanged wording is historical; root sourceapplicationiscomplete. Broadersemanticnaming andheaderclosuremigration remainopen.
