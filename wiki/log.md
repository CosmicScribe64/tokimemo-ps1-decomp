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

## [2026-10-09] ticket | T-0009, T-0011 -> In Progress; T-0012 created
Moved [[tickets/T-0009-progress-report-script]] and [[tickets/T-0011-game-code-file-boundaries-and-compiler]] Backlog -> In Progress on [[kanban]]. T-0011 narrowed to compiler confirmation; file boundaries and Shift-JIS moved to new backlog ticket [[tickets/T-0012-game-file-boundaries-and-shift-jis]].

## [2026-10-09] build | Progress script, funcdiff, ten getters decompiled
Added `tools/progress.py` (+ `ninja progress` target in `configure.py`) and `tools/funcdiff.py` (per-function object diff); `src/game.c` now has 10 getters as C (`include/game.h` holds the externs). sha1 still OK. Documented in [[build-system]]; new pages [[matching-notes]] and [[decompile-workflow]]; [[toolchain]] updated. Tracks [[tickets/T-0009-progress-report-script]] and [[tickets/T-0011-game-code-file-boundaries-and-compiler]].

## [2026-10-09] decision | Compiler not confirmed by gcc
Setters, `x = y`, `x += c` and `&sym` returns do not match with any gcc in `/opt/gcc` (best 4 of 10); original uses `$t6`-first allocation, `$at` store expansion with filled delay slots, `lh`, `or` for move. Details and hypothesis in [[matching-notes]]. gcc 2.7.2-psx kept as the working setup for trivial functions.

## [2026-10-09] ticket | T-0009, T-0011 -> In Review
Moved [[tickets/T-0009-progress-report-script]] and [[tickets/T-0011-game-code-file-boundaries-and-compiler]] In Progress -> In Review on [[kanban]]; running the code-review gate next.

## [2026-10-09] lint | Code review of T-0009 and T-0011
Ran a code review (medium) on the working tree against CODING_STANDARDS.md. Fixed: `tools/funcdiff.py` (position-independent branch targets, internal labels no longer split functions, dead line), `tools/progress.py` (verifies a C body or INCLUDE_ASM exists, size header required, no dependence on the sha1 check), ticket criteria and comments, `include/game.h` width note, [[build-system]] wording, small commits. New backlog ticket [[tickets/T-0013-identify-original-compiler-pipeline]].

## [2026-10-09] ticket | T-0009, T-0011 -> Done
Moved [[tickets/T-0009-progress-report-script]] and [[tickets/T-0011-game-code-file-boundaries-and-compiler]] In Review -> Done on [[kanban]] after review fixes. Created [[tickets/T-0013-identify-original-compiler-pipeline]] (Backlog).

## [2026-10-09] ticket | T-0013 -> In Progress
Moved [[tickets/T-0013-identify-original-compiler-pipeline]] Backlog -> In Progress on [[kanban]]; testing IDO (decompals/ido-static-recomp) and other non-gcc candidates against the shelved functions in [[matching-notes]].

## [2026-10-09] decision | Game code is IDO-family; build switched to IDO 5.3
IDO 5.3 with `-EL` (decompals/ido-static-recomp v1.2) reproduces the signatures gcc could not; `src/game.c` now builds with IDO 5.3 through asm-processor (per-file toolchain in `configure.py`, `ido` mode in `tools/cc.py`, guard in `include/include_asm.h`, prelude `include/asmproc_prelude.inc`). 9 more functions match, sha1 OK. Not stock IDO 5.3: non-leaf frames are 16 bytes larger and `func_80042400` has less address CSE. Evidence and hypotheses in [[matching-notes]]; [[toolchain]], [[build-system]], [[decompile-workflow]], [[overview]] updated. Ticket [[tickets/T-0013-identify-original-compiler-pipeline]].

## [2026-10-09] ticket | T-0013 -> In Review
Moved [[tickets/T-0013-identify-original-compiler-pipeline]] In Progress -> In Review on [[kanban]]; code-review gate next.

## [2026-10-09] lint | Code review of T-0013
Ran a code review (Standards + Spec, since fa2f49e). Fixed the `configure.py` docstring length, reworded the partial acceptance criterion and created [[tickets/T-0014-find-exact-ucode-compiler]] (Backlog) for the frame-size and address-CSE gap. Remaining judgement calls accepted and recorded in [[tickets/T-0013-identify-original-compiler-pipeline]].

## [2026-10-09] ticket | T-0013 -> Done, T-0014 created
Moved [[tickets/T-0013-identify-original-compiler-pipeline]] In Review -> Done on [[kanban]]; added [[tickets/T-0014-find-exact-ucode-compiler]] to Backlog.

## [2026-10-09] docs | toolchain line updated after T-0013
Updated the stack description in AGENTS.md and CODING_STANDARDS.md to IDO 5.3 for game code, gcc + maspsx for SDK libs, per [[tickets/T-0013-identify-original-compiler-pipeline]] and [[toolchain]].

## [2026-10-09] ticket | T-0400 -> In Progress
Created [[tickets/T-0400-leaf-function-batch-1]] and moved it to In Progress on [[kanban]]. Added `tools/list_leaves.py` (leaf listing, smallest first; 177 leaves outside the SDK skip range at start).

## [2026-10-09] build | T-0400 leaf batch 1: 40 functions matched
Matched 40 leaf functions in `src/game.c` (smallest first, outside 0x80080000-0x80086810), sha1 OK after each group. Idioms, failures and the T-0014 address-CSE pattern recorded in [[matching-notes]]; new externs in `include/game.h`, new `Entry8` struct. Tooling: `tools/list_leaves.py`.

## [2026-10-09] ticket | T-0400 -> In Review
Moved [[tickets/T-0400-leaf-function-batch-1]] In Progress -> In Review on [[kanban]]; code-review gate next.

## [2026-10-09] lint | Code review of T-0400
Ran a code review against CODING_STANDARDS.md and [[tickets/T-0400-leaf-function-batch-1]]; two minor findings fixed (extern comment in `include/game.h`, trailing newline).

## [2026-10-09] ticket | T-0400 -> Done
Moved [[tickets/T-0400-leaf-function-batch-1]] In Review -> Done on [[kanban]].
## [2026-10-09] ticket | T-0008 -> In Progress
Moved [[tickets/T-0008-overlay-load-address-and-split]] Backlog -> In Progress on [[kanban]]. Work on branch t0008-overlays.

## [2026-10-09] ingest | Overlay loader, load addresses and entries
Read the loader in the main exe (`func_80078C48`, `func_8007959C`, `func_80079070`, `func_8004636C`, `func_80042540`): overlays are read by sector number (0x60 sectors) into 0x80132000 and entered through `D_80123110`; the last 4 bytes of each file are a checksum the loader verifies. `jal` targets confirm 0x80132000 for 24 overlays, EVENT/GYOZI fit 0x800F6000/0x80134000 only. O.BIN is an unloaded developer build with a symbol table. Written up in [[overlays]]; follow-ups [[tickets/T-0200-event-gyozi-loader-and-address]] and [[tickets/T-0201-obin-format-and-symbols]].

## [2026-10-09] build | Overlays built byte-identical
`tools/gen_overlay_configs.py` writes `config/overlays/*.yaml`, `config/overlays/*.sha1` and `config/overlays.txt`; `configure.py` gets one split/compile/link/sha1 target per overlay (IDO 5.3 C in `src/ovl/`). All 26 overlays and `SLPM_86.053` pass `sha1sum -c` after a clean rebuild. `generate_asm_macros_files: False` added to stop splat overwriting `include/include_asm.h`. See [[build-system]] and [[overlays]].

## [2026-10-09] ticket | T-0008 -> In Review; T-0200, T-0201 created
Moved [[tickets/T-0008-overlay-load-address-and-split]] In Progress -> In Review on [[kanban]]; added [[tickets/T-0200-event-gyozi-loader-and-address]] and [[tickets/T-0201-obin-format-and-symbols]] to Backlog.

## [2026-10-09] lint | Code review of T-0008
Ran a code review (Standards + Spec, since 159adc9). Fixed the generator import placement and the index status; added the shared-symbol address caution to [[overlays]]. Remaining judgement calls recorded in [[tickets/T-0008-overlay-load-address-and-split]].

## [2026-10-09] ticket | T-0008 -> Done
Moved [[tickets/T-0008-overlay-load-address-and-split]] In Review -> Done on [[kanban]] after the review; main exe and all 26 overlays pass sha1.

## [2026-10-09] fix | progress denominator
On a clean build splat skipped asm for decompiled functions, so tools/progress.py undercounted (0/772). Set `disassemble_all: True` in config/SLPM_86.053.yaml and made tools/progress.py read asm/matchings too; now 62/834. See [[build-system]].
## [2026-10-09] ticket | T-0014 to In Progress
[[tickets/T-0014-find-exact-ucode-compiler]] started on branch t0014-compiler: investigate the +16 frame and address-CSE differences ([[matching-notes]]).

## [2026-10-09] decision | uopt -no_const_in_reg for game code
Per [[tickets/T-0014-find-exact-ucode-compiler]]: the original's missing global-address CSE is reproduced by IDO 5.3 uopt's `-no_const_in_reg`. Added `-Wo,-no_const_in_reg` to `IDO_CFLAGS` in `tools/cc.py`; matched `func_80042400` in `src/game.c` (`D_800E7D10` declared in `include/game.h`). `ninja` sha1 OK; all 23 C functions MATCH. Updated [[matching-notes]] and [[toolchain]].

## [2026-10-09] query | +16 stack frame characterized, not reproduced
Measured where the extra 16 bytes sit (non-leaf: above the saves; leaf with a frame: below the saves) and tried every cfe/uopt/ugen option of IDO 5.3 (and main ones on 7.1): no change. A ucode `DEF Mmt` +16 patch between uopt and ugen reproduces non-leaf functions exactly (diagnostic only, fakematch, not adopted). Older MIPS compilers are not available as static recomps. Details, ranked hypotheses, next experiments and fallbacks in [[matching-notes]]; follow-up [[tickets/T-0100-older-mips-compiler-emulation]]. Also noted the splat `include/include_asm.h` overwrite in [[build-system]], follow-up [[tickets/T-0101-splat-overwrites-include-asm-h]].

## [2026-10-09] ticket | T-0014 to In Review; T-0100, T-0101 created
[[tickets/T-0014-find-exact-ucode-compiler]] In Progress -> In Review on [[kanban]]. New Backlog tickets [[tickets/T-0100-older-mips-compiler-emulation]] and [[tickets/T-0101-splat-overwrites-include-asm-h]]; [[index]] updated.

## [2026-10-09] ticket | T-0014 review resolved -> Done
code-review of [[tickets/T-0014-find-exact-ucode-compiler]]: added a comment on the named temp in `src/game.c` (`func_80042400`), an Idioms entry in [[matching-notes]], fixed wording in [[toolchain]] and the ticket (AC1 partial). Moved In Review -> Done on [[kanban]]; the frame question continues in [[tickets/T-0100-older-mips-compiler-emulation]].

## [2026-10-09] merge | t0014-compiler into main
Merged branch t0014-compiler (`-Wo,-no_const_in_reg` in tools/cc.py). Resolved conflicts in include/game.h, [[index]], [[kanban]] and [[log]] by keeping both sides. [[tickets/T-0014-find-exact-ucode-compiler]] and [[tickets/T-0008-overlay-load-address-and-split]] are in Done. [[tickets/T-0101-splat-overwrites-include-asm-h]] Backlog -> Done, already fixed by T-0008.
