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
- [[tickets/T-0008-overlay-load-address-and-split|T-0008]] Overlay load address and split (Backlog)
- [[tickets/T-0009-progress-report-script|T-0009]] Progress reporting script (Done)
- [[tickets/T-0010-sdk-lib-object-boundaries|T-0010]] SDK version and lib object boundaries (Backlog)
- [[tickets/T-0011-game-code-file-boundaries-and-compiler|T-0011]] Compiler confirmation on first game functions (Done)
- [[tickets/T-0013-identify-original-compiler-pipeline|T-0013]] Identify the original compiler pipeline (In Review)
- [[tickets/T-0012-game-file-boundaries-and-shift-jis|T-0012]] Game file boundaries and Shift-JIS (Backlog)

## Entities / concepts / sources
- [[disc-layout]] - disc images, extraction, file list
- [[executable]] - SLPM_86.053 header, memory map, bss, segment layout
- [[overlays]] - the 26 .EXN overlays
- [[toolchain]] - Docker image, pinned versions, compiler choice (IDO 5.3 for game code)
- [[psyq-sdk]] - SDK era, libraries, code anchors
- [[matching-notes]] - compiler verdict, evidence, matched/unmatched functions, idioms
- [[decompile-workflow]] - m2c -> edit -> build -> funcdiff -> commit
- [[build-system]] - configure.py / ninja pipeline and gotchas
- Raw source (plain path): `raw/disc-findings.md`

## Tooling
