---
type: index
updated: 2026-10-15
---

# Wiki Index

Read this first. Update on every ingest or new page.

## Core
- [[stopping-point-2026-10-15]] - final handoff: 1,066/1,066 compiling draft scopes, 63.7% verified bytes, reconstruction limitations and restart instructions; user-requested stop.
- [[overview]] - project goal, stack, pointers to fact pages
- [[SCHEMA]] - wiki layout summary (full rules in AGENTS.md)
- [[log]] - append-only chronological record
- [[kanban]] - ticket board (columns must match ticket frontmatter status)
- [[data-types]] - structs and arrays recovered from the original's access patterns, `tools/type_recovery.py` (T-5000)
- [[game-state]] - `GameState D_800E6280`, the main game-state struct (0x800E6280..0x800E7D10): field map, evidence, confidence, `tools/migrate_globals.py` (T-5100); one view for every unit, access audit (T-7010)
- Repo-root `README.md`, `ROADMAP.md`, `CONTRIBUTING.md`, `LICENSE` (CC0) - public face of the project (T-0901)
- Repo-root `CODING_STANDARDS.md` - coding conventions and review checklist (T-0007)
- [[ref-english-patch]] - facts-only reference from the English-patch internals notes: ~150 addresses by area (overlay loader, CD, boot, text renderer, font, script/dialogue, menus), current symbol names (T-11090)

## Tickets
- [[tickets/T-15500-bunkasai-character-flags|T-15500]] BUNKASAI record-ten flag match (In Review; 820 bytes).
- [[tickets/T-15400-bunkasai-label-draft|T-15400]] BUNKASAI label encoding recovery (In Review; 440 coverage bytes).
- [[tickets/T-15300-fresh-structural-near-matches|T-15300]] Fresh structural near matches: BUNKASAI func_801438E4 matched, 852 coverage/literal bytes; TACO probes continue (In Progress).
- [[tickets/T-15800-compile-remaining-overlay-drafts|T-15800]] All 23 assigned failed overlay scopes now compile as source-only alternatives, with exact contexts and evidence (In Review).

- [[tickets/T-15300-fresh-structural-near-matches|T-15300]] Fresh structural matches: BUNKASAI 852 + BUNKAKEN 440 coverage/literal bytes; unfinished TACO draft archived with context (In Review).

- [[tickets/T-15000-kangei-near-match|T-15000]] KANGEI func_80135CE0 matched: ordinary duplicated branch stores, 432 coverage/literal bytes (In Review).

- [[tickets/T-14501-remaining-draft-proof|T-14501]] Verify exhaustive draft candidates: 30 literal matches / 19,220 inventory coverage bytes in 9 units (In Review).
- [[tickets/T-13500-incremental-build-dependencies|T-13500]] Incremental build dependencies (Done; 27/27 checks, ETC rebuilds 13 objects instead of 443).
- [[tickets/T-13701-name-ent-record-types|T-13701]] NAME_ENT record and actor type recovery (In Review; coherent typed draft, no match).
- [[tickets/T-13700-name-ent-families|T-13700]] NAME_ENT family reconstruction (In Review; zero accepted bytes).
- [[tickets/T-12903-option-small-handlers|T-12903]] Match small OPTION audio handlers (In Progress).
- [[tickets/T-12902-small-draft-triage|T-12902]] Triage saved small OPTION and audio drafts (In Review; 264 linked bytes).
- [[tickets/T-12901-small-option-main-first|T-12901]] Verify close OPTION and audio drafts (In Review; 2 matches / 548 linked bytes).
- [[tickets/T-12900-small-option-main|T-12900]] Small OPTION and main audio matching (In Progress).
- [[tickets/T-12903-option-small-handlers|T-12903]] Match small OPTION audio handlers (Done; bounded pass reviewed).
- [[tickets/T-12902-small-draft-triage|T-12902]] Triage saved small OPTION and audio drafts (Done; bounded pass reviewed).
- [[tickets/T-12901-small-option-main-first|T-12901]] Verify close OPTION and audio drafts (Done; bounded pass reviewed).
- [[tickets/T-12900-small-option-main|T-12900]] Small OPTION and main audio matching (Done; 3 matches / 812 bytes total across this pass).
- [[tickets/T-12500-largest-dialogue|T-12500]] Largest dialogue matching pass (In Review; diagnostic pass, zero new bytes).

- [[tickets/T-11830-tt-wave1-handoff|T-11830]] TT: the seven wave-1 TT matches handed over by the maintainer (Done; TT 65.3%).
- [[tickets/T-12010-codex-cx1|T-12010]] Codex cx1 main helper matching (In Review).
- [[tickets/T-12020-codex-cx2|T-12020]] Codex cx2 TAIIKU matching pass (In Review).
- [[tickets/T-12030-codex-cx3|T-12030]] Codex cx3 main menu functions (In Review).
- [[tickets/T-12040-codex-cx4|T-12040]] Codex cx4 RPG battle matching (In Review;7 matches,6068 linked bytes).
- [[tickets/T-12050-codex-cx5|T-12050]] Codex cx5 main menu matching pass (In Review; 208 verified bytes).
- [[tickets/T-12090-codex-cx9|T-12090]] Codex cx9 BUNKA_SD matching pass (In Progress).
- [[tickets/T-12100-codex-cx10|T-12100]] Codex cx10 DATE movie/effect functions (In Progress).
- [[tickets/T-12200-acceleration-audit|T-12200]] Measured acceleration investigation in an isolated worktree (Done; 18.29x stripping speedup, 229 tests).
- [[tickets/T-12070-codex-cx7|T-12070]] Codex cx7 TACO actor matching (In Review; three matches, 4,252 linked bytes).
- [[tickets/T-12090-codex-cx9|T-12090]] Codex cx9 BUNKA_SD matching pass (In Review; six matches, 4172 text bytes plus 124 table bytes).
- [[tickets/T-12100-codex-cx10|T-12100]] Codex cx10 DATE movie/effect functions (In Review).
- [[tickets/T-11820-tt-batch-9|T-11820]] TT batch 9: never-attempted functions and func_801333D4 (Done; 3 matches, TT 60.6%).
- [[tickets/T-11810-tt-batch-8|T-11810]] TT batch 8: Opus on untried pipeline drafts and the closest guarded drafts (Done; 7 matches, TT 59.1%).
- [[tickets/T-11800-tt-batch-7|T-11800]] TT batch 7: bulk_m2c rerun and two Opus passes (Done; 10 matches, TT 56.7%).
- [[tickets/T-11600-w1-ovl-event-8010fe10|T-11600]] Wave 1 package w1-p04: EVENT 8010FE10.c work list (Done; 10 of 12 matched, 3,676 bytes).
- [[tickets/T-11610-w1-main-80042540|T-11610]] Wave 1 package w1-p05: main 80042540.c and 80058D20.c (Done; 7 of 10 matched, 80042540.c all C).
- [[tickets/T-11570-w1-ovl-name_ent-80139740|T-11570]] Wave 1 package w1-p01: NAME_ENT 80139740.c guarded drafts (Done; no match, selector-copy family traced to uopt copy propagation).
- [[tickets/T-11620-w1-ovl-tt-80133970|T-11620]] Wave 1 package w1-p06: 4 of 12 TT functions matched (2,052 bytes), notes in [[matching-notes]] (Done).
- [[tickets/T-11630-w1-ovl-tt-8013c6d0|T-11630]] Wave 1 package w1-p07: TT `8013C6D0.c` (Done; 6 of 8 matched, one FAKE).
- [[tickets/T-11590-w1-main-8007c030|T-11590]] Wave 1 package w1-p03: main 8007C030.c work list (Done; 15 matches, `s32` without return keeps the chain temporary in v0).
- [[tickets/T-11580-w1-ovl-date-80132000|T-11580]] Wave 1 package w1-p02: DATE 80132000.c work list (Done; 11 matches, place-handler table-reader family).
- [[tickets/T-11560-tt-batch-6|T-11560]] TT batch 6: bulk_m2c over TT, then Opus on the closest drafts (Done; 4 matches, TT 48.6%).
- [[tickets/T-11550-tt-opus-near-miss-pass|T-11550]] TT Opus near-miss pass with all solvers (Done; 5 matches, TT 48.0%).
- [[tickets/T-11540-tt-order-search-pass|T-11540]] TT order_search pass (Done; func_8013DE10 matched, five drafts improved, stale func_8014CC04 draft removed).
- [[tickets/T-11260-fast-scoring-order-search-permute|T-11260]] Fast scoring for order_search/permute/farm (In Review; `order_search.py --fast` opt-in with exact rescoring, the stripped compile scores differently; permute/farm have no scoring loop).
- [[tickets/T-11250-faster-candidate-scoring|T-11250]] Faster candidate scoring for the solvers (In Review; cached strip + parallel prefetch, `tools/fastscore.py`, about 25x).
- [[tickets/T-11520-tt-batch-4|T-11520]] TT batch 4 (Done; Opus near-miss pass + large-function Sonnet pass, 2 matches / 2,468 bytes, TT 43.8%).
- [[tickets/T-11180-twin-guided-matching-gemini-39|T-11180]] Twin-guided matching (Gemini) 39 (In Progress).
- [[tickets/T-11530-order-search-tool|T-11530]] Tooling: order_search.py, statement and operand order search (Done; validated on two TT matches).
- [[tickets/T-11510-tt-batch-3|T-11510]] TT batch 3 (Done; Opus near-miss pass + 14-function Sonnet list, 4 matches / 2,996 bytes, TT 41.6%).
- [[tickets/T-11500-tt-batch-2|T-11500]] TT batch 2 (Done; Opus near-miss pass + Sonnet list, 12 matches / 7,160 bytes, TT 38.9%).
- [[tickets/T-11100-big-functions-gemini-33|T-11100]] Big functions (Gemini) 33 (In Review).
- [[tickets/T-11140-small-functions-gemini-38|T-11140]] Small functions (Gemini) 38 (Done; 11 matches / 3,444 linked bytes (3,652 list bytes), 5 guarded near misses).
- [[tickets/T-11130-big-functions-gemini-37|T-11130]] Big functions (Gemini) 37 (In Progress).
- [[tickets/T-11110-small-functions-gemini-34|T-11110]] Small functions (Gemini) 34 (In Progress).
- [[tickets/T-11090-ingest-english-patch-notes|T-11090]] Ingest the English patch notes as a reference (Done; 50 symbols renamed, [[ref-english-patch]]).
- [[tickets/T-11070-small-functions-gemini-30|T-11070]] Small functions (Gemini) 30 (In Progress).
- [[tickets/T-11060-big-functions-gemini-29|T-11060]] Big functions (Gemini) 29 (In Progress).
- [[tickets/T-11000-tt-overlay|T-11000]] TT overlay (Done; 12 matches / 5,476 bytes, 13 guarded near misses, TT 32.5%).
- [[tickets/T-11170-cheat-sheet-auto-tweaker|T-11170]] Cheat-sheet auto-tweaker for near-miss drafts (In Review; `tools/autotweak.py`).
- [[tickets/T-11190-bulk-m2c-pipeline|T-11190]] Bulk m2c and auto-solve pipeline for small functions (In Review; `tools/bulk_m2c.py`).
- [[tickets/T-10700-windows-host-baseline-build|T-10700]] Windows host setup and baseline build: docker.sh MSYS paths, 32-bit gcc cpp on WSL2 mounts (Done; 27/27, 5,176/6,958).
- [[tickets/T-10890-big-functions-astra-21|T-10890]] Big functions (astra) 21 (In Progress).
- [[tickets/T-10900-big-functions-astra-22|T-10900]] Big functions (astra) 22 (In Progress).
- [[tickets/T-10860-big-functions-4-astra|T-10860]] Big functions 4 (astra), five owned objects (Done; 88 matches / 68,856 code bytes).
- [[tickets/T-10870-big-functions-5-astra|T-10870]] Big functions 5 (astra), ten matches / 11460 bytes (Done).
- [[tickets/T-10880-big-functions-6-astra|T-10880]] Big functions 6 (astra), four owned objects (Done).
- [[tickets/T-10840-wave-13-15-tool-fixes|T-10840]] Wave 13-15 tool fixes (Done; literal-byte funcdiff proof, guarded C, prototypes, seed tolerance, full-file regdiff).
- [[tickets/T-10730-big-functions-2-astra|T-10730]] Big functions 2 (astra), three integrated matches + verified DATE patch (Done).
- [[tickets/T-10830-big-functions-3-astra|T-10830]] Big functions 3 (astra), 29 matches and movie layouts in six owned objects (Done).
- [[tickets/T-10630-big-functions-astra|T-10630]] Big functions (astra), six owned objects (Done).
- [[tickets/T-10400-regdiff-explain-register-choice|T-10400]] regdiff: explain why a draft gets different registers (Done; `tools/regdiff.py`, 2 functions matched with it, `sync_protos --add-global` guard fix, clean 27/27).
- [[tickets/T-10330-wave-12-list-3|T-10330]] Wave 12: list 3 (In Progress).
- [[tickets/T-10210-wave-11-tool-fixes|T-10210]] Wave-11 tool fixes (Done).
- [[tickets/T-10200-orphan-jump-tables|T-10200]] Orphan jump tables: enable 21 instruction-exact functions (Done; +22 functions / 27,036 bytes, clean 27/27, 5,099/6,958).
- [[tickets/T-10100-wave-9-10-tool-fixes|T-10100]] Wave 9/10 tool fixes (Done; 649 tests, final header regressions, 27/27 checksums).
- [[tickets/T-10020-wave-11-list-2|T-10020]] Wave 11: list 2 (In Progress).
- [[tickets/T-10010-wave-11-list-1|T-10010]] Wave 11: list 1 (Done).
- [[tickets/T-10030-wave-11-list-3|T-10030]] Wave 11: list 3 (Done; 12 functions / 4,172 bytes).
- [[tickets/T-10040-wave-11-list-4|T-10040]] Wave 11: list 4 (Done; 29 active matches / 28,220 bytes).
- [[tickets/T-9930-wave-10-list-3|T-9930]] Wave 10: list 3 (In Progress).
- [[tickets/T-9910-wave-10-list-1|T-9910]] Wave 10: list 1 (In Progress).
- [[tickets/T-9920-wave-10-list-2|T-9920]] Wave 10: list 2 (In Progress).
- [[tickets/T-9940-wave-10-list-4|T-9940]] Wave 10: list 4 (In Progress).
- [[tickets/T-9960-wave-10-list-6|T-9960]] Wave 10: list 6 (In Progress; 21 large functions).
- [[tickets/T-9800-uopt-register-allocation-trace-and-uniform-patch|T-9800]] uopt register allocation: working trace, eight rejected uniform candidates (Done; 27/27 clean build)
- [[tickets/T-9810-permuter-farm-for-near-misses|T-9810]] Permuter farm for near misses (Done; 1,090 queued, four verified seed wins, 65 tests)
- [[tickets/T-9760-wave-9-list-6|T-9760]] Wave 9: list 6 (Done; nine functions / 3,148 bytes)
- [[tickets/T-9740-wave-9-list-4|T-9740]] Wave 9: list 4 (Done)
- [[tickets/T-9720-wave-9-list-2|T-9720]] Wave 9: list 2 (Done; 11 functions / 2,812 bytes).
- [[tickets/T-9730-wave-9-list-3|T-9730]] Wave 9: list 3 (Done; +5 functions / 1,828 bytes)
- [[tickets/T-9770-wave-9-list-7|T-9770]] Wave 9: list 7 (Done; 6 functions / 1,108 bytes)
- [[tickets/T-9750-wave-9-list-5|T-9750]] Wave 9: list 5 (Done; 7 functions / 3,256 bytes)
- [[tickets/T-9710-wave-9-list-1|T-9710]] Wave 9: list 1 (Done; +10 functions / 5,596 bytes).
- [[tickets/T-9600-wave-8-tool-fixes|T-9600]] Wave-8 tool fixes (Done; seven reports covered, 587 tests, no function gains)
- [[tickets/T-9530-wave-8-list-3|T-9530]] Wave 8: list 3 (Done; deep pass +13 functions / 3,344 bytes, batch total 22 / 7,384)
- [[tickets/T-9570-wave-8-list-7|T-9570]] Wave 8: list 7, 14 functions matched (4,628 bytes), 5 T-0018 rows (Done; deep second pass)
- [[tickets/T-9540-wave-8-list-4|T-9540]] Wave 8: list 4 (Done; deep pass +10 functions, 1628 bytes)
- [[tickets/T-9580-wave-8-list-8|T-9580]] Wave 8: list 8 (Done; deep second pass: +9 functions, 3,400 bytes)
- [[tickets/T-9510-wave-8-list-1|T-9510]] Wave 8: list 1 (Done; second pass +15 functions / 4,556 bytes, 19 functions across both passes)
- [[tickets/T-9560-wave-8-list-6|T-9560]] Wave 8: list 6 (Done; second deep pass)
- [[tickets/T-9550-wave-8-list-5|T-9550]] Wave 8: list 5 (Done; deep pass +16 functions/4,208 bytes; total +20/5,748 bytes)
- [[tickets/T-9400-wave-7-tool-fixes|T-9400]] Wave-7 tool fixes (Done)
- [[tickets/T-9410-register-allocation-shapes-after-wave-7|T-9410]] Register-allocation shapes after wave 7 (Done; +5 functions)
See [[kanban]]. Template: [[tickets/_template]].
- [[tickets/T-9200-gcc-built-sdk-objects-in-overlays|T-9200]] gcc-built SDK objects inside overlays: fingerprint, per-object toolchain, gcc 2.7.2/2.8.1 on arm64 (Done)
- [[tickets/T-9140-wave-6-list-4|T-9140]] Wave 6: list 4, 28 functions matched (OLH help pages, text/state templates, data scalars); idioms in [[matching-notes]] "Wave 6, list 4" (Done)
- [[tickets/T-9030-wave-5-tool-fixes|T-9030]] Wave-5 tool fixes (Done)
- [[tickets/T-9010-per-object-data-shared-lui-at|T-9010]] Per-object .data ranges and data islands; shared `lui $at` matched (Done)
- [[tickets/T-9110-wave-6-list-1|T-9110]] Wave 6, list 1: 29 functions matched (6,688 bytes), new idioms in [[matching-notes]], 31 rows in [[data/t0018-cases]] (Done)
- [[tickets/T-9000-t0018-register-order-rule-or-build-step|T-9000]] T-0018 register order under K&R: 20 rows matched as source idioms, clusters, no new pass (Done)
- [[tickets/T-9020-wave-5-codegen-shapes|T-9020]] Wave-5 codegen shapes: idioms for 8 shapes, 13 functions (Done)
- [[tickets/T-9360-wave-7-list-6|T-9360]] Wave 7 list 6: 3 matched (TAIIKU `func_80146B8C` via data island, `icon_disp_switch` via local `GameState *g`, GEKO `func_8013A6D0`), 26 T-0018 rows; [[matching-notes]] "Wave 7, list 6" (Done)
- [[tickets/T-9340-wave-7-list-4|T-9340]] Wave 7: list 4, 9 functions matched (2,960 bytes), permuter-found FAKE idioms and the `% 240` temporary-shift pattern in [[matching-notes]] "Wave 7, list 4" (Done)
- [[tickets/T-9210-wave-6-codegen-shapes|T-9210]] Wave-6 codegen shapes: check_para_limit takes no argument, index as a member, indexed loops for the stack-argument slot, K&R definitions, element chains; [[matching-notes]] "Wave-6 codegen shapes" (Done)
- [[tickets/T-9320-wave-7-list-2|T-9320]] Wave 7 list 2: 39 functions (TACO range-chain template, two-counter table loops, repeated expressions instead of temps, permuter burn tricks); [[matching-notes]] "Wave 7, list 2" (Done)
- [[tickets/T-9380-wave-7-list-8|T-9380]] Wave 7, list 8: 27 functions matched (about 5,800 bytes), permuter-found idioms, 4 data islands, 21 rows in [[data/t0018-cases]]; [[matching-notes]] "Wave 7, list 8" (Done)
- [[tickets/T-8080-wave-5-list-8|T-8080]] Wave 5: list 8 (Done)
- [[tickets/T-7010-game-state-view-audit|T-7010]] Game-state struct audit: base vs separate symbols (Done)
- [[tickets/T-8010-wave-5-list-1|T-8010]] Wave 5: list 1 (Done)
- [[tickets/T-6070-wave-4-list-7|T-6070]] Wave 4: list 7 (Done)
- [[tickets/T-7030-tooling-wave-4-bug-fixes|T-7030]] Tooling: wave-4 bug fixes (Done)
- [[tickets/T-8020-wave5-list-2|T-8020]] Wave 5, list 2: 21 functions matched (Done)
- [[tickets/T-5100-game-state-struct|T-5100]] Recover the main game-state struct (Done)
- [[tickets/T-6040-wave-4-list-4|T-6040]] Wave 4: list 4, 28 functions in 36 files (Done)
- [[tickets/T-5000-type-recovery-arrays-structs|T-5000]] Type recovery: arrays and structs from access patterns (Done)
- [[tickets/T-2060-wave2-shougatu|T-2060]] Wave 2: SHOUGATU, 152 functions (Done)
- [[tickets/T-0001-project-scaffolding|T-0001]] Project scaffolding (Done)
- [[tickets/T-0002-disc-extraction-and-exe-identification|T-0002]] Disc extraction & exe identification (Done)
- [[tickets/T-0003-docker-toolchain-image|T-0003]] Docker toolchain image (Done)
- [[tickets/T-0004-splat-config-and-split|T-0004]] splat config & split (Done)
- [[tickets/T-0005-build-system-and-checksum-matching|T-0005]] Build system & checksum matching (Done)
- [[tickets/T-0006-identify-psyq-libs-sdk-version|T-0006]] Identify PsyQ libs/SDK version (Done)
- [[tickets/T-0007-write-coding-standards|T-0007]] Write CODING_STANDARDS.md (Done)
- [[tickets/T-0008-overlay-load-address-and-split|T-0008]] Overlay load address and split (Done)
- [[tickets/T-0009-progress-report-script|T-0009]] Progress reporting script (Done)
- [[tickets/T-0010-sdk-lib-object-boundaries|T-0010]] SDK version and lib object boundaries (Done)
- [[tickets/T-2040-wave-2-etc|T-2040]] Wave 2: ETC (Done)
- [[tickets/T-3050-run-per-object-migration-after-wave-2|T-3050]] Run the per-object migration on the whole tree after wave 2 (Ready)
- [[tickets/T-3051-review-low-confidence-object-boundaries|T-3051]] Review low-confidence object boundaries and orphan rodata chunks (Backlog)
- [[tickets/T-3052-per-object-data-bss-split|T-3052]] Split .data and .bss per original object (Done via T-9010)
- [[tickets/T-1300-reuse-c-across-identical-functions|T-1300]] Tooling: reuse C across identical functions (Done)
- [[tickets/T-3320-tooling-near-duplicate-function-reuse|T-3320]] Tooling: near-duplicate function reuse (Done)
- [[tickets/T-0300-sdk-object-split-remaining-libs|T-0300]] Object-level split of libcd, libsnd, libspu, libgs, libgpu, libpress (Backlog)
- [[tickets/T-0301-sdk-rodata-data-split|T-0301]] Split SDK rodata and data per library and object (Backlog)
- [[tickets/T-0302-sdk-version-conflict|T-0302]] Resolve mixed SDK vintages (Backlog)
- [[tickets/T-0011-game-code-file-boundaries-and-compiler|T-0011]] Compiler confirmation on first game functions (Done)
- [[tickets/T-0013-identify-original-compiler-pipeline|T-0013]] Identify the original compiler pipeline (Done)
- [[tickets/T-0014-find-exact-ucode-compiler|T-0014]] Find the exact MIPS ucode compiler (Done; frame follow-up T-0100)
- [[tickets/T-0012-game-file-boundaries-and-shift-jis|T-0012]] Game file boundaries and Shift-JIS (Done)
- [[tickets/T-0500-per-file-game-rodata-data-bss-split|T-0500]] Per-object C files and rodata for main and overlays (Done)
- [[tickets/T-0200-event-gyozi-loader-and-address|T-0200]] EVENT/GYOZI loader and load address (Backlog)
- [[tickets/T-0201-obin-format-and-symbols|T-0201]] O.BIN format and symbols (Done)
- [[tickets/T-0600-apply-obin-renames|T-0600]] Apply the O.BIN rename list after the game.c split (Done)
- [[tickets/T-0750-overlay-batch-b-tel-olh-en-nichi|T-0750]] Overlay batch B: TEL, OLH, EN_NICHI (Done)
- [[tickets/T-0601-obin-med-confidence-review|T-0601]] Review medium and low confidence O.BIN mappings (Backlog)
- [[tickets/T-0602-progress-overlays|T-0602]] Extend progress reporting to the 26 overlays (Done)
- [[tickets/T-0100-older-mips-compiler-emulation|T-0100]] Run an older MIPS ucode compiler for the +16 frame (Backlog)
- [[tickets/T-0015-research-compiler-mismatch-handling|T-0015]] Research: compiler mismatch handling (Done)
- [[tickets/T-0016-frame-layout-emulation-pass|T-0016]] Frame-layout emulation pass (Done)
- [[tickets/T-0017-const-in-reg-loop-hoisting|T-0017]] Reconcile -Wo,-no_const_in_reg with loop hoisting (Done; solved by -Wo,-nokpicopt)
- [[tickets/T-0018-ugen-temp-register-order|T-0018]] ugen temporary register order differs; global register promotion gap (Backlog)
- [[tickets/T-0950-match-nokpicopt-unblocked-functions|T-0950]] Match functions unblocked by -Wo,-nokpicopt (Backlog)
- [[tickets/T-0700-overlay-batch-a|T-0700]] Overlay batch A: RENSYU, OMIMAI, VALEN, MASTER (Done)
- [[tickets/T-0101-splat-overwrites-include-asm-h|T-0101]] Stop splat from overwriting include_asm.h (Done via T-0008)
- [[tickets/T-0901-public-docs|T-0901]] Public docs: LICENSE, README, ROADMAP, CONTRIBUTING, AI disclosure (Done)
- [[tickets/T-0902-ci-progress-report|T-0902]] CI: encrypted game bundle and decomp.dev progress report (Done)
- [[tickets/T-0903-public-release-audit|T-0903]] Public-release audit (Done)
- [[tickets/T-1030-overlay-batch-g-bunka-sd-date2|T-1030]] Overlay batch G: BUNKA_SD, DATE2 (Done)
- [[tickets/T-1050-overlay-batch-i-kangei-shugaku|T-1050]] Overlay batch I: KANGEI, SHUGAKU (Done)
- [[tickets/T-1200-fix-conflicting-extern-declarations|T-1200]] Fix conflicting extern declarations after batch merges (Done)
- [[tickets/T-2010-wave2-date|T-2010]] Wave 2: DATE, 278 functions matched (Done)
- [[tickets/T-1320-tooling-work-queue-and-blocker-detector|T-1320]] Tooling: work queue and blocker detector (Done)
- [[tickets/T-1321-register-promotion-build-step|T-1321]] Build step for the register-promotion gap: cause found, unsigned-load conversion pass kept out of the build (Done)
- [[tickets/T-3000-rematch-rv-functions-with-cvt-pass|T-3000]] Re-match R/V-flagged functions with cvt_pass.py, retune the detector (Backlog)
- [[tickets/T-3001-shared-constant-registers|T-3001]] Constants reused across stores and compare/store types (Done: toolchain, no fix)
- [[tickets/T-5020-loop-unrolling-and-lui-sharing|T-5020]] Loop unrolling and lui sharing (Done: lui sharing is an as1 difference, loops mostly source idioms)
- [[tickets/T-3002-remaining-promotion-shapes|T-3002]] Register shapes left after the unsigned-load conversion pass (Backlog)
- [[tickets/T-3100-identify-original-compiler|T-3100]] Identify the original game-code compiler (Done)
- [[tickets/T-3110-test-ido-52-and-41|T-3110]] Test IDO 5.2 and 4.1 against the game code (Done)
- [[tickets/T-2070-wave-2-tt|T-2070]] Wave 2: TT, 49 functions matched (Done)
- [[tickets/T-2090-wave2-main-executable|T-2090]] Wave 2: main executable (Done; 61 functions)
- [[tickets/T-1330-tooling-m2c-context-and-permuter|T-1330]] Tooling: m2c context and decomp-permuter (Done)
- [[tickets/T-1310-tooling-object-trailing-padding|T-1310]] Tooling: object-trailing padding (Done)
- [[tickets/T-1340-tooling-jump-table-functions|T-1340]] Tooling: jump-table functions, rodata islands (Done)
- [[tickets/T-2030-wave-2-taco|T-2030]] Wave 2: TACO, 70 functions matched (Done)
- [[tickets/T-2000-wave2-event|T-2000]] Wave 2: EVENT, 421 functions matched (Done)
- [[tickets/T-2100-wave2-small-overlays|T-2100]] Wave 2: small overlays, 94 functions (Done)
- [[tickets/T-3200-catalog-game-versions|T-3200]] Catalog game versions (Done)
- [[tickets/T-3310-native-docker-image|T-3310]] Native Docker image for Apple Silicon (Done)
- [[tickets/T-3330-local-fptab-frame-layout|T-3330]] Local function-pointer table frame layout (Done)
- [[tickets/T-3340-shared-main-prototypes-and-byte-queue|T-3340]] Tooling: shared main-exe prototypes (`include/main_api.h`) and byte-weighted queue (Done)
- [[tickets/T-4060-wave-3-list-6|T-4060]] Wave 3 list 6: 26 functions in DATE, DATE2, GYOZI, MASTER, OMIMAI, RPG_BAT, SHOUGATU, TT, VALEN and main (Done)
- [[tickets/T-4020-wave-3-list-2|T-4020]] Wave 3, list 2: 60 functions in DATE, SHUGAKU, EVENT, TACO, TT, GYOZI, GEKO, ENDING (Done)
- [[tickets/T-4080-wave-3-list-8|T-4080]] Wave 3, list 8: 32 functions matched in 800451D0, 8007C030 and 17 overlay files (Done)
- [[tickets/T-4100-wave-3-list-10|T-4100]] Wave 3, list 10: 115 functions matched in 27 files (Done)
- [[tickets/T-6020-wave-4-list-2|T-6020]] Wave 4, list 2: 13 functions matched in 34 files (Done)
- [[tickets/T-5010-t0018-register-order-second-attempt|T-5010]] T-0018 register order, second attempt: entry and compare rules, unit-private data, cvt pass in the build, 44 functions (Done)

## Entities / concepts / sources
- [[register-cheatsheet]]: one-page register-miss guide, symptom to C change (regdiff loop)
- [[uopt-register-allocation]] - T-9800 allocator trace, formula and uniform patch experiments
- [[register-allocation-wave7]] - T-9410 row/assembly clusters, bounded C experiments and verified idioms
- [[disc-layout]] - disc images, extraction, file list
- [[source-files]] - the `src/main/<address>.c` files and the original objects of main and overlays (T-0500): boundary evidence and confidence, alignment handling, what is not split
- [[source-files]] - the 28 `src/main/<address>.c` files: boundary evidence, alignment handling, what is not split
- [[versions]] - T-3200: the 5 distinct releases in versions/ (Rev 1/Shokai, Rev 2, Rev 4, Best), lineage, hashes, code differences against SLPM_86.053, recommendation; facts in `config/versions.txt`, `tools/identify_version.py`
- [[executable]] - SLPM_86.053 header, memory map, bss, segment layout
- [[overlays]] - the 26 .EXN overlays: loader, load addresses, entries, split and build
- [[obin]] - O.BIN: ECOFF format and header fields (`obin_syms.py --headers`), symbol table, mapping onto the main exe, stats, generated rename list
- [[gcc-objects]] - PsyQ-gcc-built objects inside overlays: fingerprint (`tools/gcc_fingerprint.py`), verdicts, per-object toolchain table (T-9200)
- [[toolchain]] - Docker image (native arm64/amd64, T-3310), pinned versions, compiler choice (IDO 5.3 for game code), frame-layout emulation pass
- [[psyq-sdk]] - per-library SDK versions, lib/object layout, signature method
- [[matching-notes]] - compiler verdict, evidence, frame layout rule and evidence table, constants in registers (T-0017), register promotion of globals (T-0018), work queue and T-0018 detector (T-1320), matched/unmatched functions, idioms
- [[ido-52-evaluation]] - T-3110: IDO 5.2 and 4.1 (decomp.me archives, private) against the game code: 5.2 = 5.3 byte for byte, 4.1 ugen reproduces the frame pass, nothing reproduces the T-1321 behaviours; recommendation, CI options, licensing facts
- [[matching-notes]] - compiler verdict, evidence, frame layout rule and evidence table, constants in registers (T-0017), register promotion of globals (T-0018), work queue and T-0018 detector (T-1320), matched/unmatched functions, idioms, local function-pointer tables (T-3330), wave 3 list 2 patterns (T-4020), selector register rule (T-5010), wave 4 list 4 patterns (T-6040)
- [[original-compiler]] - T-3100: which compiler built the game code (O.BIN version stamps 3.18 = IDO 5.2-generation MIPS suite, big-endian ECOFF link host), header field table, ranked hypotheses, promotion experiments, rules for the T-1321 build step
- [[compiler-mismatch-research]] - T-0015: PS1/Konami compilers, +16 frame candidates, how other decomps treat compiler differences, licensing, ranked recommendation
- [[decompile-workflow]] - queue.py work list -> m2c -> edit -> build -> funcdiff -> commit
- [[data/shared-at-groups]] - data table: address groups the original stores through one `lui $at`, per original object (T-5020; struct/array candidates)
- [[data/t0018-cases]] - data table: functions skipped for the T-0018 register-promotion gap (calibrates `tools/queue.py`)
- [[data/t11620-func_801441BC-draft.patch]] - T-11620: unmatched TT draft of func_801441BC (break-block and constant-range gaps)
- [[data/t11620-func_80143A44-draft.patch]] - T-11620: unmatched TT draft of func_80143A44 (106 lines, pad & 2 part)
- [[data/t3330-fptab-proof.patch]] - T-3330: C for 18 table-copy and spill functions with the index-declared-first idiom, to apply after T-1321
- [[build-system]] - configure.py / ninja pipeline and gotchas
- [[decompile-workflow]] - m2c -> edit -> build -> funcdiff -> commit; how to decompile a switch (T-1340)
- [[build-system]] - configure.py / ninja pipeline and gotchas, jump tables and rodata islands (T-1340)
- [[ci]] - GitHub Actions workflow, encrypted game bundle, objdiff report for decomp.dev
- Raw sources (plain paths): `raw/disc-findings.md`, `wiki/raw/compiler-mismatch-research-sources.md`, `wiki/raw/ai-disclosure-research.md`, `wiki/raw/original-compiler-sources.md` (T-3100)

## Tooling
- `tools/order_search.py` (T-11530): exhaustive or hill-climb search over marked statement orders (`/* ORDER */`) and operand orders (`ORDER_SWAP`), scored with funcdiff's resolved comparison; tests `tools/test_order_search.py`; [[decompile-workflow]] ("Statement and operand order search").
- `tools/orphan_rodata.py` and `tools/object_layout.py` (T-10200): reference-based orphan ownership and configured text tail alignment; [[build-system]], [[decompile-workflow]].
- `tools/header_merge.py` (T-10100): rebuild both-changed headers of a two-parent merge as ours plus whole other-side additions; `--check` previews; tests `tools/test_header_merge.py`; [[matching-notes]].
- `tools/identify_version.py` (T-3200): identify which release a disc image or folder is, from `config/versions.txt`; reads bin/iso/cue, folders and zip/7z/chd (T-3300); tests `tools/test_identify_version.py`
- `tools/prepare_disc.py` (T-3300): finds the game in `game/`, identifies it, unpacks it into `disc/`; ninja step `build/disc.stamp`; tests `tools/test_prepare_disc.py`
- T-3300 tooling fixes: `tools/m2c.py --unit`, `funcdiff.py --resolve/--no-build`, K&R in `srcscan.py`, archives in `identify_version.py`; tests `tools/test_srcscan.py`, `tools/test_funcdiff.py`; details in [[matching-notes]] ("Tooling fixes T-3300")
- `tools/identify_version.py` (T-3200): identify which release a disc image or folder is, from `config/versions.txt`; tests `tools/test_identify_version.py`
- `tools/migrate_globals.py` (T-5100): rewrites the old `D_` names of aggregate fields (`GameState`) into field accesses, removes their declarations; `--check` in ninja (`build/globals.ok`); config `config/migrate_globals.txt`; tests `tools/test_migrate_globals.py`; [[game-state]]
- `tools/aggregate_audit.py` (T-7010): how the original reaches the fields of each aggregate (direct, indexed, base-relative), read-modify-write pairs kept or hoisted, bit-field sites; used by `migrate_globals.py --check` for `keep` lines; tests `tools/test_aggregate_audit.py`; [[game-state]]
- `tools/sync_protos.py` (T-3340): lists, generates and checks `include/main_api.h`, the one declaration of every main-exe symbol; overrides, `--prune`, `--snapshot/--compare`; tests `tools/test_sync_protos.py`; rules in CODING_STANDARDS 8a and [[decompile-workflow]]
- T-5030 tooling fixes: `tools/funcloc.py` (find the C file by definition; used by `funcdiff.py`, `permute.py`), `funcdiff.py --unit` and string/rename resolution, `permute.py all`/K&R, `sync_protos.py` speed-up and `--check-branch`, `queue.py` cache; tests `tools/test_funcloc.py` and the tests of each tool; details in [[matching-notes]] ("Tooling fixes T-5030"), [[decompile-workflow]] ("Before finishing")
- T-7030 tooling wave-4 fixes: `sync_protos.py` keeps comments, `dupes.py`/`neardupes.py` copy game-state fields, overrides, file-local types, address literals and prototypes (163 + 25 functions applied), `funcdiff.py` builds `expected/` itself, `permute.py` time limit and build verification, `m2c.py` callee argument counts and post-increment order; tests `tools/test_dupes.py`, `test_neardupes.py`, `test_funcdiff.py`, `test_permute.py`, `test_sync_protos.py`, `test_m2c_args.py`; details in [[matching-notes]] ("Tooling fixes, wave 4 (T-7030)") and [[decompile-workflow]]
- `tools/queue.py` (T-1320, T-3340): `--by bytes`, `--plan N --agents K`, retuned R/V/U/T flags; calibration in [[matching-notes]]
- `tools/queue.py` (T-1320, T-3340, T-5010): `--by bytes`, `--plan N --agents K`, retuned R/V/U/T flags (entry rule in `tools/entry_rule.py`, tests `tools/test_entry_rule.py`); calibration in [[matching-notes]]

- [[tickets/T-9520-wave-8-list-2|T-9520]] Wave 8: list 2 (Done; second pass adds 11 functions / 3,548 bytes; batch 17 / 7,168).
- [[tickets/T-9610-aggregate-and-local-copy-types|T-9610]] Aggregate and local-copy types (Done).

- [[tickets/T-9780-wave-9-list-8|T-9780]] Wave 9: list 8 (Done; 18 matches / 8580 bytes, four-hour session).

- [[tickets/T-10850-wave-17-astra|T-10850]] Wave 17 (astra, easiest first), Done: 5 matches, 936 bytes.

- [[tickets/T-10910A-big-functions-astra-23|T-10910A]] Big functions (astra) 23 (In Progress).

- [[tickets/T-11020-big-functions-sonnet-25|T-11020]] Big functions (Sonnet) 25, Done: 11 matches / 7,520 bytes (EVENT and DATE scene setup), six near misses guarded; idioms in [[matching-notes]].
- [[tickets/T-11030-big-functions-sonnet-26|T-11030]] Big functions (Sonnet) 26, Done: 2 matches / 1,552 bytes (main func_80072338, DATE func_8013CDF8), two near misses guarded; open frame and mult-hoist puzzles in [[matching-notes]].

- [[tickets/T-11120-small-functions-sonnet-36|T-11120]] Small functions (Sonnet via Antigravity) 36 (In Progress).

- [[tickets/T-12400-integrate-verified-matches|T-12400]] Integrate verified matching branches directly on main (In Review; combined 27/27 checks pass, 59.0% byte coverage).
- [[acceleration-audit]] - T-12200 measured stripping speedup, validation limits, queue/reuse priorities, and CPU allocation.
- [[tickets/T-12210-matching-bottleneck-research|T-12210]] Remaining-byte census and experiments for matching breakthroughs (Done; 3 matches, 2352 linked bytes).
- [[matching-breakthroughs]] - T-12210 remaining-byte census, three verified family matches, negative retrieval experiment, and research priorities.
- [[tickets/T-12220-scene-handler-family-experiment|T-12220]] Medium-function cohort and paired bitfield experiment (Done; 20 matches, 16,272 linked bytes).
- [[tickets/T-12230-broad-coverage-sweep|T-12230]] Broad medium/large coverage sweep (Done; 190 targets, five matches).
- [[broad-coverage-results]] - T-12230: 190-target ledger, five verified wins, and limits of broad zero-search matching.
- [[tickets/T-12300-large-unclaimed-byte-matches|T-12300]] Substantial unclaimed byte matches (In Review; 4,920 verified bytes, four functions).
- [[tickets/T-12600-byte-coverage-families|T-12600]] Increase verified byte coverage toward 80% through shared function families (In Progress).

- [[tickets/T-12700-bunka-families|T-12700]] BUNKAKEN/BUNKASAI family reconstruction (Done; 41 matches, 40,252 bytes).
- [[tickets/T-12701-bunkaken-address-family|T-12701]] BUNKAKEN source families (Done; 35 matches, 33,716 bytes).
- [[tickets/T-12702-bunkasai-family|T-12702]] BUNKASAI remaining source families (Done; two matches, 2,520 bytes).
- [[bunka-family-results]] — BUNKAKEN aggregate-address and BUNKASAI state-reload experiments (T-12700).
- [[tickets/T-12703-bunkasai-heldout-family|T-12703]] BUNKASAI held-out family transfer (In Progress).

- [[tickets/T-12800-large-event-tel|T-12800]] Large EVENT TEL and main matching (Done).

- [[tickets/T-12801-event|T-12801]] EVENT control flow matching (Done).

- [[tickets/T-12802-tel|T-12802]] TEL table and control flow matching (Done).

- [[tickets/T-12803-main|T-12803]] Main schedule matching (Done).
- [[tickets/T-12703-bunkasai-heldout-family|T-12703]] BUNKASAI held-out family transfer (Done; four matches, 4,016 bytes).

- [[tickets/T-13000-types-globals|T-13000]] Recover evidence-backed types and globals (Done).

- [[tickets/T-13001-master-layout-pilot|T-13001]] Recover MASTER layouts and verify pilot matches (Done).

- [[master-types-pilot]] — T-13000/T-13001: three MASTER typed-table matches, 3,648 literal bytes and evidence limits.
- [[tickets/T-13400-taco-control-flow|T-13400]] Recover TACO flag handler control flow (In Review; diagnostic only, zero bytes).

- [[tickets/T-13100-etc-families|T-13100]] ETC source-family transfer, isolated worktree.

- [[tickets/T-13101-etc-career-family|T-13101]] ETC career state handlers.

- [[tickets/T-13200-event-family-transfer|T-13200]] GYOZI/SHOUGATU bounded event family transfer.

- [[tickets/T-13201-event-selector-stores|T-13201]] Event selector direct-store family.
- [[tickets/T-13102-etc-exam-family|T-13102]] ETC exam handlers.

- [[tickets/T-13202-event-sprite-initializers|T-13202]] Event sprite initializer family.

- [[tickets/T-13300-rpg-families|T-13300]] RPG_BAT source-family diagnostics (In Review; zero accepted bytes).
- [[tickets/T-13203-event-particle-family|T-13203]] Particle initialization family experiment.

- [[event-family-results]] GYOZI/SHOUGATU bounded family transfer:3 functions /3424 literal bytes, failed-probe ledger and review.
- [[tickets/T-13103-etc-memory-card-family|T-13103]] ETC memory-card state family.

- [[etc-family-results]]: eleven ETC matches / 5900 literal bytes, positive and negative transfer evidence.

- [[name-ent-family-results]] - T-13700 bounded negative evidence for prologue setup and memory-card menu reconstruction.

- [[tickets/T-13600-taiiku-families|T-13600]] TAIIKU source-family recovery (In Progress).

- [[data/t13600-remaining-census]]: baseline TAIIKU remaining functions and unit-tagged historical evidence (T-13600).
- [[tickets/T-14000-ending-en-nichi-families|T-14000]] ENDING and EN_NICHI family matching (In Progress).
- [[scenes-family-results]] - GEKO/DATE2 family work, [[tickets/T-13900-scenes-families|T-13900]].
- [[tickets/T-14100-date-near-matches|T-14100]] DATE retained near matches and new-family hypotheses (In Progress).
- [[tickets/T-14100-date-near-matches|T-14100]] DATE retained near matches and new-family hypotheses (In Review; three matches, 636 bytes).
- [[tickets/T-13500-incremental-build-dependencies|T-13500]] Compiler include dependencies reused for DATE experiments.
- [[tickets/T-14000-ending-en-nichi-families|T-14000]] ENDING and EN_NICHI family matching (In Progress).
- [[name-ent-family-results]] - T-13700 bounded negative evidence for prologue setup and memory-card menu reconstruction.

- [[tickets/T-13800-taco-families|T-13800]] TACO family reconstruction; [[tickets/T-13801-taco-actor-family|T-13801]] actor/table experiments (In Review).
- [[taco-family-results]] — bounded TACO boss transform reconstruction; zero accepted bytes, 497EC one-row and 4A480 three-row near misses (T-13800/T-13801).
- [[tickets/T-14300-event-families|T-14300]] Remaining EVENT family reconstruction (In Progress).
- [[tickets/T-14200-main-source-families|T-14200]] MAIN source-family recovery (In Progress).
- [[tickets/T-14200-main-source-families|T-14200]] MAIN source-family recovery (Done; 5 functions / 6624 literal bytes, two labeled FAKE frame homes).
- [[main-family-results]] - T-14200 MAIN verified checkpoints and rejected focused probes.

- [[tickets/T-14400-option-menu-families|T-14400]] OPTION menu/state family matching (In Progress).
- [[tickets/T-14300-event-families|T-14300]] Remaining EVENT family reconstruction (In Review; eight matches / 4,084 literal bytes).

- [[exhaustive-drafts]] — T-14500 exhaustive game-function drafts and scoring census (In Progress).
- [[tickets/T-14400-option-menu-families|T-14400]] OPTION menu/state family matching (Done; 6 functions / 5868 literal verified bytes).
- [[tickets/T-14600-date-draft-repairs|T-14600]] Repair nine DATE draft failures (In Review; all nine compile, no zero comparisons).
- [[tickets/T-14800-etc-801447b0|T-14800]] ETC func_801447B0: 648 literal linked bytes, ETC SHA1 passes (In Review).

- [[tickets/T-14700-reserved-type-repairs|T-14700]] Reserved aggregate and callback draft repairs (Done; 15/15 compile, no zero candidates, reviewed draft-only archive).
- [[tickets/T-14900-rpg-near-matches|T-14900]] RPG_BAT motion aggregate match: 1,128 linked bytes, two bounded counter blockers (In Review).
- [[tickets/T-15100-unaligned-draft-repairs|T-15100]] Nine unaligned aggregate-copy drafts compile; no zero comparisons (In Review).
- [[tickets/T-15600-fresh-near-matches|T-15600]] RENSYU/SHOUGATU: 1,256 literal linked bytes; RPG NOP gap archived (In Review).
- [[tickets/T-14700-reserved-type-repairs|T-14700]] Reserved aggregate and callback draft repairs (In Review; 15/15 compile, no zero candidates).
- [[tickets/T-15200-event-large|T-15200]] Large EVENT dispatcher and reusable types (In Review; independent drafts and type evidence, zero new matching bytes).

- [[tickets/T-15900-remaining-draft-compile-repairs|T-15900]] — Remaining mixed-overlay compile repairs (13 scopes, In Progress).
- [[tickets/T-15700-taco-taiiku-draft-repairs|T-15700]] All28 TACO/TAIIKU failed drafts now compile; no zeros (In Review).
