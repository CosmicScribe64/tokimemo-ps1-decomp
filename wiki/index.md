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
- Repo-root `CODING_STANDARDS.md` - coding conventions and review checklist (T-0007)

## Tickets
See [[kanban]]. Template: [[tickets/_template]].
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
- [[tickets/T-0300-sdk-object-split-remaining-libs|T-0300]] Object-level split of libcd, libsnd, libspu, libgs, libgpu, libpress (Backlog)
- [[tickets/T-0301-sdk-rodata-data-split|T-0301]] Split SDK rodata and data per library and object (Backlog)
- [[tickets/T-0302-sdk-version-conflict|T-0302]] Resolve mixed SDK vintages (Backlog)
- [[tickets/T-0011-game-code-file-boundaries-and-compiler|T-0011]] Compiler confirmation on first game functions (Done)
- [[tickets/T-0013-identify-original-compiler-pipeline|T-0013]] Identify the original compiler pipeline (Done)
- [[tickets/T-0014-find-exact-ucode-compiler|T-0014]] Find the exact MIPS ucode compiler (Done; frame follow-up T-0100)
- [[tickets/T-0012-game-file-boundaries-and-shift-jis|T-0012]] Game file boundaries and Shift-JIS (Done)
- [[tickets/T-0500-per-file-game-rodata-data-bss-split|T-0500]] Split game rodata, data and bss per source file (Backlog)
- [[tickets/T-0200-event-gyozi-loader-and-address|T-0200]] EVENT/GYOZI loader and load address (Backlog)
- [[tickets/T-0201-obin-format-and-symbols|T-0201]] O.BIN format and symbols (Done)
- [[tickets/T-0600-apply-obin-renames|T-0600]] Apply the O.BIN rename list after the game.c split (Done)
- [[tickets/T-0601-obin-med-confidence-review|T-0601]] Review medium and low confidence O.BIN mappings (Backlog)
- [[tickets/T-0100-older-mips-compiler-emulation|T-0100]] Run an older MIPS ucode compiler for the +16 frame (Backlog)
- [[tickets/T-0015-research-compiler-mismatch-handling|T-0015]] Research: compiler mismatch handling (Done)
- [[tickets/T-0016-frame-layout-emulation-pass|T-0016]] Frame-layout emulation pass (Done)
- [[tickets/T-0017-const-in-reg-loop-hoisting|T-0017]] Reconcile -Wo,-no_const_in_reg with loop hoisting (Backlog)
- [[tickets/T-0018-ugen-temp-register-order|T-0018]] ugen temporary register order differs (Backlog)
- [[tickets/T-0101-splat-overwrites-include-asm-h|T-0101]] Stop splat from overwriting include_asm.h (Done via T-0008)

## Entities / concepts / sources
- [[disc-layout]] - disc images, extraction, file list
- [[source-files]] - the 28 `src/main/<address>.c` files: boundary evidence, alignment handling, what is not split
- [[executable]] - SLPM_86.053 header, memory map, bss, segment layout
- [[overlays]] - the 26 .EXN overlays: loader, load addresses, entries, split and build
- [[obin]] - O.BIN: ECOFF format, symbol table, mapping onto the main exe, stats, generated rename list
- [[toolchain]] - Docker image, pinned versions, compiler choice (IDO 5.3 for game code), frame-layout emulation pass
- [[psyq-sdk]] - per-library SDK versions, lib/object layout, signature method
- [[matching-notes]] - compiler verdict, evidence, frame layout rule and evidence table, matched/unmatched functions, idioms
- [[compiler-mismatch-research]] - T-0015: PS1/Konami compilers, +16 frame candidates, how other decomps treat compiler differences, licensing, ranked recommendation
- [[decompile-workflow]] - m2c -> edit -> build -> funcdiff -> commit
- [[build-system]] - configure.py / ninja pipeline and gotchas
- Raw sources (plain paths): `raw/disc-findings.md`, `raw/compiler-mismatch-research-sources.md`

## Tooling
