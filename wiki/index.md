---
type: index
updated: 2026-10-09
---

# Wiki Index

Read this first. Update on every ingest or new page.

## Core
- [[overview]] - project goal, stack, pointers to fact pages
- [[SCHEMA]] - wiki layout summary (full rules in AGENTS.md)
- [[log]] - append-only chronological record
- [[kanban]] - ticket board (columns must match ticket frontmatter status)
- Repo-root `README.md`, `ROADMAP.md`, `CONTRIBUTING.md`, `LICENSE` (CC0) - public face of the project (T-0901)
- Repo-root `CODING_STANDARDS.md` - coding conventions and review checklist (T-0007)

## Tickets
See [[kanban]]. Template: [[tickets/_template]].
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
- [[tickets/T-3052-per-object-data-bss-split|T-3052]] Split .data and .bss per original object (Backlog)
- [[tickets/T-1300-reuse-c-across-identical-functions|T-1300]] Tooling: reuse C across identical functions (Done)
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
- [[tickets/T-1321-register-promotion-build-step|T-1321]] Build step for the register-promotion gap, deferred (Backlog)
- [[tickets/T-3100-identify-original-compiler|T-3100]] Identify the original game-code compiler (Done)
- [[tickets/T-2070-wave-2-tt|T-2070]] Wave 2: TT, 49 functions matched (Done)
- [[tickets/T-2090-wave2-main-executable|T-2090]] Wave 2: main executable (Done; 61 functions)
- [[tickets/T-1330-tooling-m2c-context-and-permuter|T-1330]] Tooling: m2c context and decomp-permuter (Done)
- [[tickets/T-1310-tooling-object-trailing-padding|T-1310]] Tooling: object-trailing padding (Done)
- [[tickets/T-1340-tooling-jump-table-functions|T-1340]] Tooling: jump-table functions, rodata islands (Done)
- [[tickets/T-2030-wave-2-taco|T-2030]] Wave 2: TACO, 70 functions matched (Done)
- [[tickets/T-2000-wave2-event|T-2000]] Wave 2: EVENT, 421 functions matched (Done)
- [[tickets/T-2100-wave2-small-overlays|T-2100]] Wave 2: small overlays, 94 functions (Done)
- [[tickets/T-3200-catalog-game-versions|T-3200]] Catalog game versions (Done)
- [[tickets/T-3330-local-fptab-frame-layout|T-3330]] Local function-pointer table frame layout (Done)

## Entities / concepts / sources
- [[disc-layout]] - disc images, extraction, file list
- [[source-files]] - the `src/main/<address>.c` files and the original objects of main and overlays (T-0500): boundary evidence and confidence, alignment handling, what is not split
- [[source-files]] - the 28 `src/main/<address>.c` files: boundary evidence, alignment handling, what is not split
- [[versions]] - T-3200: the 5 distinct releases in versions/ (Rev 1/Shokai, Rev 2, Rev 4, Best), lineage, hashes, code differences against SLPM_86.053, recommendation; facts in `config/versions.txt`, `tools/identify_version.py`
- [[executable]] - SLPM_86.053 header, memory map, bss, segment layout
- [[overlays]] - the 26 .EXN overlays: loader, load addresses, entries, split and build
- [[obin]] - O.BIN: ECOFF format and header fields (`obin_syms.py --headers`), symbol table, mapping onto the main exe, stats, generated rename list
- [[toolchain]] - Docker image, pinned versions, compiler choice (IDO 5.3 for game code), frame-layout emulation pass
- [[psyq-sdk]] - per-library SDK versions, lib/object layout, signature method
- [[matching-notes]] - compiler verdict, evidence, frame layout rule and evidence table, constants in registers (T-0017), register promotion of globals (T-0018), work queue and T-0018 detector (T-1320), matched/unmatched functions, idioms, local function-pointer tables (T-3330)
- [[original-compiler]] - T-3100: which compiler built the game code (O.BIN version stamps 3.18 = IDO 5.2-generation MIPS suite, big-endian ECOFF link host), header field table, ranked hypotheses, promotion experiments, rules for the T-1321 build step
- [[compiler-mismatch-research]] - T-0015: PS1/Konami compilers, +16 frame candidates, how other decomps treat compiler differences, licensing, ranked recommendation
- [[decompile-workflow]] - queue.py work list -> m2c -> edit -> build -> funcdiff -> commit
- [[data/t0018-cases]] - data table: functions skipped for the T-0018 register-promotion gap (calibrates `tools/queue.py`)
- [[data/t3330-fptab-proof.patch]] - T-3330: C for 18 table-copy and spill functions with the index-declared-first idiom, to apply after T-1321
- [[build-system]] - configure.py / ninja pipeline and gotchas
- [[decompile-workflow]] - m2c -> edit -> build -> funcdiff -> commit; how to decompile a switch (T-1340)
- [[build-system]] - configure.py / ninja pipeline and gotchas, jump tables and rodata islands (T-1340)
- [[ci]] - GitHub Actions workflow, encrypted game bundle, objdiff report for decomp.dev
- Raw sources (plain paths): `raw/disc-findings.md`, `wiki/raw/compiler-mismatch-research-sources.md`, `wiki/raw/ai-disclosure-research.md`, `wiki/raw/original-compiler-sources.md` (T-3100)

## Tooling
- `tools/identify_version.py` (T-3200): identify which release a disc image or folder is, from `config/versions.txt`; tests `tools/test_identify_version.py`
