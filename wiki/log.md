---
type: log
---

# Log

Append-only. Entry format: `## [YYYY-MM-DD] <op> | <title>` where op is ingest, query, lint, ticket, build, decision, or setup. Use [[wikilinks]] for wiki files and plain paths (e.g. `src/main.c`) for everything else. Recent: `grep "^## \[" wiki/log.md | tail -5`.

## [2026-10-09] ticket | T-0007 Ready -> In Progress -> In Review
Wrote CODING_STANDARDS.md (repo root) and linked it from AGENTS.md. Tracks [[tickets/T-0007-write-coding-standards]].

## [2026-10-09] lint | Code review of T-0001 and T-0007
Ran a code review for [[tickets/T-0007-write-coding-standards]] (3 minor findings in CODING_STANDARDS.md/AGENTS.md, fixed) and [[tickets/T-0001-project-scaffolding]] (orphan [[SCHEMA]] page, added to [[index]]). No open findings.

## [2026-10-09] ticket | T-0001 and T-0007 -> Done
Both moved In Review -> Done on [[kanban]] after clean review. Updated [[index]]; new file CODING_STANDARDS.md.

## [2026-10-09] ingest | Disc findings
Moved `docs-staging/disc-findings.md` to `raw/disc-findings.md` (raw source, unchanged) and wrote [[disc-layout]], [[executable]], [[overlays]], [[toolchain]]; updated [[overview]] and [[index]]. Added `tools/extract_disc.py` for reproducible extraction (verified: `disc/files/SLPM_86.053` sha1 matches `config/SLPM_86.053.sha1`). Tracks [[tickets/T-0002-disc-extraction-and-exe-identification]] and [[tickets/T-0003-docker-toolchain-image]].

## [2026-10-09] decision | Executable layout, bss and segments
Header 0x20/0x24 is the rodata range; text 0x80041000-0x800AF340, rodata to 0x800B3220, data to 0x800E3800, bss 0x800E3800-0x8012B538 (size 0x47D38, from the entry code's bzero). Game code vs SDK libs split at 0x80086810 (heuristic). Config: `config/SLPM_86.053.yaml`. Details in [[executable]] and [[psyq-sdk]]. Tracks [[tickets/T-0004-splat-config-and-split]] and [[tickets/T-0006-identify-psyq-libs-sdk-version]].

## [2026-10-09] build | OK build of SLPM_86.053
`configure.py` + ninja in Docker: splat, as, gcc 2.7.2-psx + maspsx, ld, objcopy; `build/SLPM_86.053.bin` matches the original sha1 with all 831 game functions as `INCLUDE_ASM` in `src/game.c`. Fixes needed: ASCII string encoding (Shift-JIS to UTF-8 changed bytes), linking `build/undefined_syms_auto.txt`. See [[build-system]]. Tracks [[tickets/T-0005-build-system-and-checksum-matching]].

## [2026-10-09] ticket | T-0002 to T-0006 -> In Review; T-0008 to T-0011 created
Moved T-0002, T-0003, T-0004, T-0005, T-0006 (Backlog/In Progress -> In Review) on [[kanban]]. Created backlog tickets [[tickets/T-0008-overlay-load-address-and-split]], [[tickets/T-0009-progress-report-script]], [[tickets/T-0010-sdk-lib-object-boundaries]], [[tickets/T-0011-game-code-file-boundaries-and-compiler]]. `.gitignore` now excludes `/asm/`, `/build.ninja`.

## [2026-10-09] lint | Code review of T-0002 to T-0006
Ran a code review against CODING_STANDARDS.md (range 060900b..HEAD). Fixed: `tools/cc.py` (per-stage error checks, INCLUDE_ASM depfile), self-regenerating `build.ninja`, removed `--no-check-sections`, single INCLUDE_ASM macro path in `include/common.h`, `tools/extract_disc.py` (skips Form2 streams, always re-extracts, clean EOF exit), all Python deps and m2c pinned in `tools/Dockerfile` (fresh build verified), [[tickets/T-0006-identify-psyq-libs-sdk-version]] re-scoped. Clean-tree build still OK. Commit granularity finding accepted (later commits are per-topic); trailer follows the task instructions.

## [2026-10-09] ticket | T-0002 to T-0006 -> Done
Moved [[tickets/T-0002-disc-extraction-and-exe-identification]], [[tickets/T-0003-docker-toolchain-image]], [[tickets/T-0004-splat-config-and-split]], [[tickets/T-0005-build-system-and-checksum-matching]], [[tickets/T-0006-identify-psyq-libs-sdk-version]] In Review -> Done on [[kanban]] after the fixes above. Updated [[index]]. Open: [[tickets/T-0008-overlay-load-address-and-split]], [[tickets/T-0009-progress-report-script]], [[tickets/T-0010-sdk-lib-object-boundaries]], [[tickets/T-0011-game-code-file-boundaries-and-compiler]].
