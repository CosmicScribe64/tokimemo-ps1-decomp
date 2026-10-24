---
id: T-27050
title: Parallel main-module reconciliation
status: In Progress
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

m-holiday, m-hud, m-sysmenu and m-menutext executable-table preparation; no source application. A second work package records logical source filenames, topic-header partitions and broad evidence-backed constants inventories for these same units.

## Acceptance criteria

- [x] Source-backed outputs with exact coverage and open work; no false completion claims.
- [x] Reconciled names respect applied ledgers and ADR ownership, Docker dry runs where relevant.
- [x] Independent review of prepared executable batches before application.
- [x] No conflicting source/shared-file edits; primary chat applies/commits and maintains shared wiki.
- [x] Exact reviewed filename/header layouts and site-scoped proven constants inventories in all four specs, separate from frozen symbol tables.

## Comments

2026-10-24: User explicitly authorized fan-out across multiple chats. Shared-checkout worker edits are limited to assigned tables/specs and this ticket; root serializes applications, builds, indexes, board and log.

2026-10-24 reconciliation checkpoint: three bounded Luna agents cover m-holiday, m-hud and m-sysmenu; this lane covers m-menutext. `m-menutext-apply.tsv` has 48 rows (10 function, 38 data), including all nine remaining placeholder definitions and the existing glyph expansion rename. Docker `tokimemo-decomp-arm64 --cpus=2` plan with ledger `m-menutext` exited 0, 48 rows, no PROBLEM. The owned spec distinguishes this executable subset from the historical 153-row draft and lists all 16 omitted data rows plus separate field/constant/comment/file work. Viewer names now use menu_ and the player-club query uses girl_ per object ownership; current talk_voice_play/talk_text_step names are preserved. No source application or build was performed. Shared wiki/log maintenance remains with the integration owner; independent review remains pending.

2026-10-24 independent Sol 6.1 review: PASS for all four tables and reconciled specs, read-only against current bodies and Standards/ADR/glossary. Final inventory is 247 rows (183 function, 64 data): m-menutext 48 (10/38), m-sysmenu 98 (93/5), m-hud 52 (52/0), m-holiday 49 (28/21). All 173 placeholder definitions in the four owned units are covered (9/91/45/28 respectively); ten additional function rows correct existing names. No O.BIN old symbol is targeted; combined five-column/old-key/new-name checks found no duplicates. Findings resolved: legal settings_/portrait_/holiday_ ownership; direct-or-table voice source words; exact mascot/help hit bounds; magazine issue-year gates and no spring-1998 bit12 write; birthday-window/caller-write attribution; appointment prefix scan/early-return/no-write behavior; correct 512-byte HUD clear vs 128-byte upload; empty-log first=255,last=0. Hint-topic labels were independently checked against their data labels without reproducing text. Prior emulator evidence is explicitly historical and was not rerun.

Final menutext, HUD and holiday Docker plans exited 0 with no PROBLEM (48 rows/18 files, 52 rows/122 files and 49 rows/5 files). Sysmenu's 98-row final re-run is pending after two earlier attempts overlapped the integration owner's enum application; those errors involved mixed MODE_/GAME_MODE_ snapshots rather than rename collisions. No build, rename application, source/header/symbol/ledger edit, git mutation or shared-wiki write was performed by this lane. Status stays In Progress until the integration owner serializes the board/log transition and application work.

2026-10-24: Parallel chat 01a16f17-083d-73c0-bee3-1c5cdd62caa4 dispatched. No source application or shared wiki mutation permitted; primary chat integrates reviewed outputs.

2026-10-24 final plan: sysmenu re-run exited 0, 98 rows/11 affected files, zero PROBLEM. All four final plans now pass; the earlier transient enum-snapshot failures are superseded. Final tables are application-ready reviewed subsets. Application, source build proof, commits and shared wiki/board/log upkeep remain exclusively with the integration owner. This ticket is not a claim that full phase 2 or even every per-unit follow-up is complete.

2026-10-24 user-priority extension: retain the reviewed 247 symbol rows as a frozen delivery; add separate source-file proposals and reviewed header/constant recommendations. Three Luna authors continue their disjoint specs, parent handles menutext, and independent Sol 6.1 reviews all four recommendations. Supported file rows use object stems, never fake new-header rows. Header extraction remains a future pure declaration move with view snapshots before changes. Constants inventory spans meaningful IDs, masks, bounds, thresholds and formulas at exact source contexts, reuses current shared enums, and preserves widths/signs; no blanket replacement is authorized. Initial file plans pass for menutext (3 rows/8 files) and holiday (1 row/3 files). No moves, header edits or literal edits have been applied.

2026-10-24 second-work-package final handoff: independent Sol 6.1 review PASS for all four source/header layouts and constants inventories; no remaining findings. Four separate *-files.tsv tables contain seven supported object-preserving file rows: text_system, vram_viewer, menu_speaker_girl (menutext3); hud_panels/hud_screens (HUD2); holiday (1); home_screens (sysmenu1). Docker plans all exit0/no PROBLEM: menutext3rows8files, HUD2rows6files, holiday1row3files, sysmenu1row3files. Unique old/new stems and exact object ranges independently checked. Header recommendations partition interfaces by topic, keep shared types canonical, avoid umbrella/type cycles, preserve missing/implicit-int and override/() views, and snapshot before any moves. HUD now recommends five API topics plus shared type homes; known WordView/BirthdayBits, MenuItem, PrimArena, Fix32 and save-record edges are explicit. Constants inventories cover all owned objects, distinguish caller context from literal sites, reuse current GameMode and other existing enums, mark UiSe/HudIcon/CardResult/ClickMode as future coordination when not yet defined, and retain unsigned/signed and partial-word-write details. These inventories are not executable const rows and contain no blanket replacement. Frozen 247-row symbol delivery is unchanged. Root still owns all source application, const-row generation, header extraction, moves, clean builds, commits and shared board/index/log edits.
