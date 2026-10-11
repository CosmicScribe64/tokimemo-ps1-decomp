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

## [2026-10-09] ticket | T-0015 created, In Progress
Created [[tickets/T-0015-research-compiler-mismatch-handling]] and moved its card to In Progress on [[kanban]]. Research only; no code touched.

## [2026-10-09] ingest | Compiler mismatch research
Read primary sources (decomp repos, tool READMEs, NEWS-OS/Ultrix man pages, gcc 2.8.1 mips.c) and wrote `raw/compiler-mismatch-research-sources.md` plus [[compiler-mismatch-research]]. Findings: Konami PS1 decomps (Vandal Hearts, SotN, Silent Hill) use PsyQ gcc, not ucode; no PS1 game built with IDO found; NEWS-OS 4.x `cc` is a documented ucode compiler with -EL/-mips1, a plausible but unverified origin; nothing found that documents a +16 frame; maspsx/asm-processor/old-gcc patches are accepted toolchain emulation; "fakematch" has no written definition outside `CODING_STANDARDS.md`. Licensing notes recorded, unverified items marked. Indexed in [[index]].

## [2026-10-09] ticket | T-0015 -> In Review
Moved [[tickets/T-0015-research-compiler-mismatch-handling]] In Progress -> In Review on [[kanban]]. Recommendation ranked in [[compiler-mismatch-research]]; follow-up decisions belong to [[tickets/T-0100-older-mips-compiler-emulation]].
## [2026-10-09] ticket | T-0010 -> In Progress
Moved [[tickets/T-0010-sdk-lib-object-boundaries]] Backlog -> In Progress on [[kanban]].

## [2026-10-09] ingest | PsyQ signature sets (lab313ru/psx_psyq_signatures)
Downloaded the public signature JSONs (no SDK binaries) into the gitignored `tools/psyq_sigs/` and matched them against `disc/files/SLPM_86.053` with `tools/psyq_sigmatch.py` and `tools/psyq_sigfuzzy.py`. Findings in [[psyq-sdk]]: libgte matches PsyQ 3.4 exactly, libc/libcard 3.3, libgpu <= 3.61, libcd/libsnd/libpress newer-looking; game/SDK boundary 0x80086810 confirmed; lib order refined (libgs, libgte, libetc added). Ticket [[tickets/T-0010-sdk-lib-object-boundaries]].

## [2026-10-09] decision | SDK region split into 65 asm segments, 166 real names
`config/SLPM_86.053.yaml` replaces `sdk_libs`/`libapi_stubs` with per-library and per-object asm subsegments; `config/symbol_addrs.txt` names 166 functions. `configure.py` now reads the asm file list from the yaml, `tools/asm.py` assembles text without gas's 16-byte padding (needed for objects whose size is not a multiple of 16), and the yaml sets `generate_asm_macros_files: False` because splat overwrote `include/include_asm.h`. sha1 OK. See [[build-system]], [[executable]].

## [2026-10-09] ticket | T-0300, T-0301, T-0302 created; T-0010 -> In Review
Follow-ups [[tickets/T-0300-sdk-object-split-remaining-libs]], [[tickets/T-0301-sdk-rodata-data-split]], [[tickets/T-0302-sdk-version-conflict]] added to [[kanban]] Backlog. Moved [[tickets/T-0010-sdk-lib-object-boundaries]] In Progress -> In Review; code-review gate next.

## [2026-10-09] lint | Code review of T-0010
Ran a code review (Standards + Spec). Fixed: acceptance criteria restored in [[tickets/T-0010-sdk-lib-object-boundaries]] with unmet items pointing to [[tickets/T-0300-sdk-object-split-remaining-libs]], [[tickets/T-0301-sdk-rodata-data-split]], [[tickets/T-0302-sdk-version-conflict]]; pins in [[psyq-sdk]] given confidence levels; SDK names moved to `config/symbol_addrs_sdk.txt` (main config only, overlays reject out-of-segment symbols); signature tools share `tools/psyqsig.py`.

## [2026-10-09] ticket | T-0010 -> Done
Merged main (overlays, 26 configs); clean rebuild gives 27/27 sha1 OK and `ninja progress` 63/834. Moved [[tickets/T-0010-sdk-lib-object-boundaries]] In Review -> Done on [[kanban]].

## [2026-10-09] lint | Code review of T-0015
Ran the code-review gate inline (Standards + Spec, wiki-only change). Hedged unverified claims on [[compiler-mismatch-research]] (NEWS-OS dev kits, CES 1994, Evo N64 IDO, emulator and frame-access suggestions). `raw/compiler-mismatch-research-sources.md` left unchanged. Recorded in [[tickets/T-0015-research-compiler-mismatch-handling]]; ticket In Review -> Done on [[kanban]]; index line updated.

## [2026-10-09] ticket | T-0016 created, -> In Progress
Created [[tickets/T-0016-frame-layout-emulation-pass]] (option 1 of [[compiler-mismatch-research]]) and placed it in In Progress on [[kanban]].

## [2026-10-09] query | Frame rule derived from the original code (T-0016)
Scanned every framed function of the main segment and the 26 overlays (3312 of 6891; scratch scripts in build/exp, not committed). Rule, table and exception in [[matching-notes]] ("Frame layout emulation"): hole after the save block when `$ra` is saved, at the bottom otherwise; 3286 non-leaf with no access in the hole, 24 leaf never below 0x10; one gcc-style exception.

## [2026-10-09] decision | Layer: IDO binasm via an as1 shim (T-0016)
`cc -S` text reassembles to different code (vreg records lost) and the ucode `DEF Mmt` +16 route cannot do leaves, so [[tickets/T-0016-frame-layout-emulation-pass]] uses `tools/frame_pass.py` between ugen and as1. `tools/cc.py` sets USR_LIB to a symlink farm with the as1 shim; `configure.py` lists the module as an implicit input. Documented in [[toolchain]] and [[build-system]].

## [2026-10-09] build | Frame pass: 12 non-leaf functions match, progress 75/834
Matched in `src/game.c`: func_80041584, func_80041878, func_80042458, func_8004B338, func_80052DA4, func_8006CCE4, func_80077F2C, func_8007B5CC, func_8007ECD0, func_80083378, func_800462C8, func_80046318 (headers `include/libapi.h`, `include/libgpu.h`, prototypes in `include/game.h`). Unit tests `tools/test_frame_pass.py` (19). Findings that are not the frame (constant hoisting flag, ugen temporaries) in [[tickets/T-0017-const-in-reg-loop-hoisting]] and [[tickets/T-0018-ugen-temp-register-order]]; the three framed TAIIKU leaf functions are `NON_MATCHING` in `src/ovl/TAIIKU.c`.

## [2026-10-09] decision | CODING_STANDARDS section 7a (toolchain emulation pass vs fakematch)
Added to `CODING_STANDARDS.md`; [[tickets/T-0100-older-mips-compiler-emulation]] stays open (note added).

## [2026-10-09] ticket | T-0016 -> In Review; T-0017, T-0018 created
Moved [[tickets/T-0016-frame-layout-emulation-pass]] In Progress -> In Review on [[kanban]]. New Backlog tickets [[tickets/T-0017-const-in-reg-loop-hoisting]] and [[tickets/T-0018-ugen-temp-register-order]]; [[index]] updated.

## [2026-10-09] ticket | T-0016 -> Done
User chose to keep `-Wo,-no_const_in_reg`. The framed-leaf criterion of [[tickets/T-0016-frame-layout-emulation-pass]] moves to [[tickets/T-0017-const-in-reg-loop-hoisting]]. Card moved In Review -> Done on [[kanban]]. Pass lives in tools/frame_pass.py; rule in [[matching-notes]] and [[toolchain]].

## [2026-10-09] ticket | T-0201 -> In Progress
Moved [[tickets/T-0201-obin-format-and-symbols]] Backlog -> In Progress on [[kanban]]. Work in worktree t0201-obin; no edits to src/ or include/.

## [2026-10-09] ingest | O.BIN format, symbols and mapping
Parsed disc/files/CDROM/EXEDIR/O.BIN: a little-endian MIPS ECOFF with an mdebug external symbol table (2146 names), an OLH skeleton at 0x80132000 and the symbol map of a developer build of the main exe. Wrote tools/obin_syms.py, tools/obin_map.py, tools/test_obin_tools.py, config/symbol_addrs_obin.txt and config/obin_renames.txt (370 high-confidence renames, not applied). Page [[obin]]; updated [[overlays]] and [[index]]. New Backlog tickets [[tickets/T-0600-apply-obin-renames]] and [[tickets/T-0601-obin-med-confidence-review]].

## [2026-10-09] ticket | T-0201 -> In Review
Moved [[tickets/T-0201-obin-format-and-symbols]] In Progress -> In Review on [[kanban]]; awaiting the code-review gate against CODING_STANDARDS.md.

## [2026-10-09] ticket | T-0201 -> Done
Review fixes applied to tools/obin_syms.py, tools/obin_map.py and tools/test_obin_tools.py; [[obin]] gained the game-code spot check (12 of 24 corroborated, 0 refuted) and marks the OLH mapping as manual. Review recorded in [[tickets/T-0201-obin-format-and-symbols]]; all 27 sha1 checks OK. Card moved In Review -> Done on [[kanban]]; [[index]] updated.
## [2026-10-09] ticket | T-0012 -> In Progress
Moved [[tickets/T-0012-game-file-boundaries-and-shift-jis]] Backlog -> In Progress on [[kanban]]. Baseline clean rebuild OK (27 sha1).

## [2026-10-09] build | Shift-JIS strings readable and matching (T-0012)
Set `string_encoding`/`data_string_encoding: SHIFT-JIS` in `config/SLPM_86.053.yaml` and in `tools/gen_overlay_configs.py` (all `config/overlays/*.yaml` regenerated). Cause of the +0xC70 bytes: gas emits the UTF-8 bytes of a `.asciz` literal. Fix: `tools/asm.py` re-encodes non-ASCII characters of string literals to Shift-JIS octal escapes before `as`; tests in `tools/test_asm.py`. Main exe and 26 overlays still sha1 OK. Documented in [[build-system]] and [[executable]].

## [2026-10-09] decision | Game code split into 29 files (T-0012)
`src/game.c` replaced by `src/main/<address>.c` (29 `c` subsegments in `config/SLPM_86.053.yaml`). Boundaries: 27 alignment-padding gaps found with `tools/game_boundaries.py`, plus the entry point `0x800420D0` (rodata-consistent). Two odd nop gaps and all hidden boundaries left unsplit. rodata/data/bss kept whole, follow-up [[tickets/T-0500-per-file-game-rodata-data-bss-split]]. Evidence in [[source-files]]; `configure.py` reads the yaml, `tools/cc.py` pads each object's `.text` to 16, `tools/progress.py`, `tools/list_leaves.py`, `tools/funcdiff.py` and `objdiff.json` work per file.

## [2026-10-09] build | Clean rebuild after the split
`rm -rf asm build; configure; ninja`: main exe and all 26 overlays sha1 OK; `ninja progress` 75/834 (2432/284428 bytes); `func_80043504` as an empty C function was verified to match too (reverted to keep 75).

## [2026-10-09] ticket | T-0012 -> In Review; T-0500 created
Moved [[tickets/T-0012-game-file-boundaries-and-shift-jis]] In Progress -> In Review on [[kanban]]. New Backlog ticket [[tickets/T-0500-per-file-game-rodata-data-bss-split]]; [[index]] lists [[source-files]].

## [2026-10-09] decision | Entry-point split rejected, review fixes (T-0012)
The 0x800420D0 boundary (entry point plus rodata start) was merged back into `src/main/80041000.c`: no padding evidence, so 28 files remain ([[source-files]]). Added `tools/srcscan.py` (shared INCLUDE_ASM/size scanning for `tools/progress.py`, `tools/list_leaves.py`, `tools/funcdiff.py`), `tools/test_cc.py` for the `tools/cc.py` padding pass, a padding-pass section in [[toolchain]] and [[matching-notes]], concrete rodata reasons in [[tickets/T-0500-per-file-game-rodata-data-bss-split]]; stale `src/game.c` text rewritten.

## [2026-10-09] build | Final clean rebuild
`rm -rf asm build; configure; ninja`: 27 of 27 sha1 OK (main exe + 26 overlays, with the padding pass active); `ninja progress` 75/834, 2432/284428 bytes.

## [2026-10-09] ticket | T-0012 -> Done
Code review (CODING_STANDARDS.md and spec) findings resolved, recorded in [[tickets/T-0012-game-file-boundaries-and-shift-jis]]. Card moved In Review -> Done on [[kanban]].

## [2026-10-09] ticket | T-0600 applied the O.BIN renames, Done
[[tickets/T-0600-apply-obin-renames]] In Backlog -> In Progress -> Done on [[kanban]]. All 370 renames from `config/obin_renames.txt` applied through `config/symbol_addrs_obin.txt` (added to `config/SLPM_86.053.yaml` and `configure.py`), sources `src/main/*.c` and `include/game.h` and wiki pages rewritten by script; none skipped (no collisions). Names recorded as hypotheses in [[obin]] (12 of 24 spot checks corroborated, 0 refuted); convention added to `CODING_STANDARDS.md` section 4; [[build-system]] updated. Clean rebuild: 27 of 27 sha1 OK, `ninja progress` 75/834, 2432/284428. Code review done inline, no open findings.

## [2026-10-09] ticket | T-0602 created, In Progress
New ticket [[tickets/T-0602-progress-overlays]] (Backlog -> In Progress on [[kanban]]): overlay rows, subtotals, grand total and an asm-only SDK line for `tools/progress.py`; overlay splat configs get `disassemble_all` (`config/overlays/*.yaml`, `tools/gen_overlay_configs.py`).

## [2026-10-09] build | Progress covers overlays and SDK libs (T-0602)
`tools/progress.py` (and `ninja progress`) now report main-game rows and subtotal, 26 overlay rows and subtotal, a grand total and a separate asm-only SDK libs line; overlay splat configs set `disassemble_all`. Added `tools/test_progress.py`. Docs updated in `README.md` and [[build-system]]. Clean rebuild: 27 of 27 sha1 OK; main 75/834 (2432/284428), overlays 8/6128 (64/1994940), grand 83/6962 (2496/2279368), SDK 722 functions / 165212 bytes not counted.

## [2026-10-09] ticket | T-0602 -> Done
Inline code review against CODING_STANDARDS.md found one issue (overlay denominator needed `disassemble_all`), fixed. Card moved In Progress -> Done on [[kanban]], see [[tickets/T-0602-progress-overlays]].
## [2026-10-09] ticket | T-0700 created, In Progress
[[tickets/T-0700-overlay-batch-a]] (overlay batch A: RENSYU, OMIMAI, VALEN, MASTER) added to [[kanban]] in In Progress. Sources: `src/ovl/RENSYU.c`, `src/ovl/OMIMAI.c`, `src/ovl/VALEN.c`, `src/ovl/MASTER.c`.

## [2026-10-09] ticket | T-0700 -> In Review -> Done
[[tickets/T-0700-overlay-batch-a]] matched 56 functions (MASTER 22, VALEN 15, OMIMAI 13, RENSYU 6) in `src/ovl/MASTER.c`, `src/ovl/VALEN.c`, `src/ovl/OMIMAI.c`, `src/ovl/RENSYU.c`; new headers `include/ovl/*.h`; pad stubs `src/ovl/pad/*.s`. 27 of 27 sha1 OK. New patterns and gaps in [[matching-notes]]. Moved In Progress -> In Review -> Done on [[kanban]] after the inline code review (no findings).

## [2026-10-09] merge | ovl-batch-a, overlay link names
Merged [[tickets/T-0700-overlay-batch-a]]. Applied config/obin_renames.txt to src/ovl/*.c and include/ovl/*.h (38 references). Overlay links could not resolve main-exe names that only live in config/symbol_addrs_obin.txt and config/symbol_addrs_sdk.txt, so tools/syms_to_ld.py now writes build/main_names.ld for every overlay link. configure.py globs include/ recursively so include/ovl/*.h changes rebuild. tools/progress.py skips src/ovl/pad stubs. Clean rebuild 27/27 OK; progress 139/6962 functions, 6904/2279368 bytes. See [[build-system]].
## [2026-10-09] ticket | T-0750 created, In Progress
[[tickets/T-0750-overlay-batch-b-tel-olh-en-nichi]] Backlog -> In Progress on [[kanban]]: decompile TEL, OLH, EN_NICHI functions in `src/ovl/TEL.c`, `src/ovl/OLH.c`, `src/ovl/EN_NICHI.c`.

## [2026-10-09] build | T-0750 first batch committed
12 functions matched in `src/ovl/EN_NICHI.c`, `src/ovl/TEL.c`, `src/ovl/OLH.c`, headers `include/ovl/*.h`; `ninja` 27 of 27 sha1 OK. Commit "T-0750: match 12 functions in TEL, OLH, EN_NICHI".

## [2026-10-09] ticket | T-0750 In Progress -> In Review -> Done
[[tickets/T-0750-overlay-batch-b-tel-olh-en-nichi]] moved In Progress -> In Review -> Done on [[kanban]] after the inline code-review gate (no open findings). Result 12 matches of ~50 aimed; new toolchain gaps (u8-global compare-chain switch uses `$v1`, `* 0x44` as `multu`, one-nop alignment pad) recorded in [[matching-notes]]. Rename list for the merge is in the ticket. [[index]] lists the ticket.

## [2026-10-09] ingest | AI-disclosure wording research
Read the AI sections of eds-decomp, CBFD-Recompiled, sonicheroes, sa2, pokediamond and rebang; notes and URLs in [[raw/ai-disclosure-research]]. Used for the README section of [[tickets/T-0901-public-docs]].

## [2026-10-09] build | T-0902 CI workflow and report tooling
Added `.github/workflows/progress.yml`, `tools/make_ci_bundle.sh`, `tools/report_objs.py`; `configure.py` now writes overlay units and progress categories into `objdiff.json`. Method and rationale in [[ci]]. Simulated the job on a clean clone: 27/27 sha1 OK, objdiff report 54 units, 151 functions, 8132 bytes. Artifact name `SLPM_86.053_report`. Updated [[build-system]] and [[index]]. [[tickets/T-0902-ci-progress-report]] In Progress -> In Review.

## [2026-10-09] ticket | T-0901, T-0902, T-0903 In Review -> Done
After the inline code-review gate (no open findings) [[tickets/T-0901-public-docs]], [[tickets/T-0902-ci-progress-report]] and [[tickets/T-0903-public-release-audit]] moved In Review -> Done on [[kanban]]. T-0902 still needs the maintainer to run the printed gh commands before the first CI run.

## [2026-10-09] ticket | T-0800 created, In Progress
New ticket [[tickets/T-0800-main-exe-batch-c]] (Backlog -> In Progress on [[kanban]]): decompile functions in src/main/80041000.c through src/main/80059A20.c, worktree branch main-batch-c.

## [2026-10-09] build | T-0800 53 functions matched
Decompiled 53 functions of src/main/80041000.c to src/main/80059A20.c (main game 75/834 -> 128/834, `ninja progress`). New idioms (`u8` parameter plus `(u32)` cast for `sltiu`, RECT by value, `FileReq` stack struct) and the failure patterns (sunk prologue, hoisted constants, F-8 spill slot, register choice) are in [[matching-notes]]. Shared declarations appended to include/game.h; no config/ or tools/ changes.

## [2026-10-09] ticket | T-0800 -> In Review
Card moved In Progress -> In Review on [[kanban]], see [[tickets/T-0800-main-exe-batch-c]]. Inline code review next.

## [2026-10-09] ticket | T-0800 -> Done
Inline code review against CODING_STANDARDS.md found three issues (unmarked fakematch in `func_8004AE28`, unused declarations in include/game.h, a transient non-matching function), all fixed. Card moved In Review -> Done on [[kanban]], see [[tickets/T-0800-main-exe-batch-c]]. Final: 53 functions matched, 27 of 27 sha1 OK.

## [2026-10-09] fix | CI bundle location
tools/make_ci_bundle.sh now writes to ci-bundle/ (gitignored) instead of build/ci-bundle/, because clean rebuilds delete build/ and with it the bundle key. Updated [[ci]]. Merged [[tickets/T-0800-main-exe-batch-c]]: progress 204/6962 functions, 13836/2279368 bytes.

## [2026-10-09] fix | CI base image mirror
First CI run failed at the Docker build: Docker Hub answered 429 Too Many Requests for ubuntu:22.04. tools/Dockerfile now takes `ARG BASE` (default ubuntu:22.04) and .github/workflows/progress.yml passes public.ecr.aws/docker/library/ubuntu:22.04. Local builds are unchanged. See [[ci]].

## [2026-10-09] ticket | T-1040 started
Batch H: overlays OPTION and ENDING, aim 40+ matches. Ticket [[tickets/T-1040-overlay-batch-h-option-ending]], moved to In Progress in [[kanban]]. Sources: src/ovl/OPTION.c, src/ovl/ENDING.c.

## [2026-10-09] ticket | T-1040 done
Batch H matched 41 functions in src/ovl/ENDING.c (29) and src/ovl/OPTION.c (12), headers include/ovl/OPTION.h and include/ovl/ENDING.h, pad stubs under src/ovl/pad. All 27 sha1 OK, `ninja progress` 245/6962. Failure patterns added to [[matching-notes]]; code review gate run inline; [[tickets/T-1040-overlay-batch-h-option-ending]] moved to Done in [[kanban]].
## [2026-10-09] ticket | T-1030 started
Created [[tickets/T-1030-overlay-batch-g-bunka-sd-date2]] (Batch G: BUNKA_SD, DATE2 overlays), moved to In Progress in [[kanban]]. Sources: `src/ovl/BUNKA_SD.c`, `src/ovl/DATE2.c`.

## [2026-10-09] ticket | T-1030 done: batch G overlays BUNKA_SD, DATE2
63 functions matched (BUNKA_SD 28, DATE2 35) in `src/ovl/BUNKA_SD.c` and `src/ovl/DATE2.c`, headers `include/ovl/BUNKA_SD.h` and `include/ovl/DATE2.h`, pad stubs in `src/ovl/pad/`. `ninja progress` grand total 267/6962 functions, 19124/2279368 bytes. Patterns and failures added to [[matching-notes]] (section Overlay batch G). Inline code review passed, [[tickets/T-1030-overlay-batch-g-bunka-sd-date2]] moved to Done in [[kanban]], [[index]] updated.
## [2026-10-09] ticket | T-1020 -> In Progress
Created [[tickets/T-1020-main-batch-f]] (Batch F: `src/main/80075320.c`, `800789E0.c`, `80079B10.c`, `80085E30.c`); card in In Progress on [[kanban]].

## [2026-10-09] ticket | T-1020 -> In Review
58 functions matched in `src/main/80079B10.c`, `80085E30.c`, `80075320.c`, `800789E0.c`; new patterns in [[matching-notes]]. Card moved In Progress -> In Review on [[kanban]], see [[tickets/T-1020-main-batch-f]]. Inline code review next.

## [2026-10-09] ticket | T-1020 -> Done
Inline code review against CODING_STANDARDS.md: unused declarations in include/game.h removed, no fakematches. Card moved In Review -> Done on [[kanban]], see [[tickets/T-1020-main-batch-f]]. Final: 58 functions matched, grand total 262/6962 functions, 27 of 27 sha1 OK. Branch batch-b-f, not merged.
## [2026-10-09] ticket | T-1050 started
Created [[tickets/T-1050-overlay-batch-i-kangei-shugaku]] (In Progress): match functions in `src/ovl/KANGEI.c` and `src/ovl/SHUGAKU.c`.

## [2026-10-09] ticket | T-1050 batch I done
105 functions matched in `src/ovl/KANGEI.c` (49) and `src/ovl/SHUGAKU.c` (56), headers `include/ovl/KANGEI.h` and `include/ovl/SHUGAKU.h`; all sha1 checks OK, grand total 309/6962. New failure instances recorded in [[matching-notes]] (section Overlay batch I). Code-review gate run inline, no open findings; ticket [[tickets/T-1050-overlay-batch-i-kangei-shugaku]] moved to Done on [[kanban]].
## [2026-10-09] ticket | T-1070 started
Created [[tickets/T-1070-overlay-batch-k-name-ent-tt]] (Batch K: `src/ovl/NAME_ENT.c`, `src/ovl/TT.c`), status In Progress in [[kanban]].

## [2026-10-09] ticket | T-1070 done
Batch K: 76 functions matched in `src/ovl/NAME_ENT.c` (53) and `src/ovl/TT.c` (23), headers `include/ovl/NAME_ENT.h` and `include/ovl/TT.h`, pad stubs in `src/ovl/pad/`. 27 of 27 sha1 OK. New patterns (source-order register numbers, `case 8: default:` chain, pointer-global reload) in [[matching-notes]]. Inline review passed; [[tickets/T-1070-overlay-batch-k-name-ent-tt]] moved to Done in [[kanban]].
## [2026-10-09] ticket | T-1000 started
Created [[tickets/T-1000-main-exe-batch-d]] (In Progress): decompile `src/main/8005A0B0.c` and `src/main/80061710.c`, target 40+ matches.

## [2026-10-09] build | T-1000 batch D matches
Matched 39 functions in src/main/8005A0B0.c and src/main/80061710.c (call sequences, guards, store sequences, hit-box tests); the other 90 hit the known T-0017/T-0018 patterns, recorded in [[matching-notes]] (section Main exe batch D). Main exe sha1 OK; progress grand total 243/6962. [[tickets/T-1000-main-exe-batch-d]] moved to In Review.

## [2026-10-09] ticket | T-1000 closed
Inline code-review gate passed (no open findings); [[tickets/T-1000-main-exe-batch-d]] moved to Done on branch batch-b-d, not merged. 39 matches.
## [2026-10-09] ticket | T-1060 overlay batch J (BUNKAKEN, BUNKASAI)
[[tickets/T-1060-overlay-batch-j-bunkaken-bunkasai]]: 77 functions matched in `src/ovl/BUNKAKEN.c` and `src/ovl/BUNKASAI.c` (two clone templates: pointer setters and scene-start stubs), headers `include/ovl/BUNKAKEN.h`, `include/ovl/BUNKASAI.h`, pad stubs `src/ovl/pad/pad_BUNKAKEN_*.s`, `src/ovl/pad/pad_BUNKASAI_*.s`. All 27 sha1 OK. Notes in [[matching-notes]] (section Overlay batch J). Ticket moved to Done after the inline code-review gate.

## [2026-10-09] ticket | T-1200 started
Created [[tickets/T-1200-fix-conflicting-extern-declarations]] (In Progress): clean build fails after the batch merges with cfe "redeclaration" errors from conflicting extern types in `include/game.h` and `include/ovl/*.h`.

## [2026-10-09] build | T-1200 extern conflicts fixed
Per-symbol decisions (access widths over all users) in [[matching-notes]]: `D_801217D0` s32, `D_800CA148/14C` s16, `D_800CA160/164/168` s32, `D_80120652` u8, `D_800CA19C/1DC` u8 arrays; `func_80046318` and `func_80083440` get separate main (`include/main_only.h`) and overlay views because a `u8` prototype adds `andi` in overlay callers. Exact duplicates removed. Guard: `tools/check_headers.py`, `tools/test_check_headers.py`, ninja target `headers`, CI step; rule in CODING_STANDARDS.md 8a ([[build-system]]). Clean build without `-k`: 27 of 27 sha1 OK, 663/6962.

## [2026-10-09] ticket | T-1200 done
Inline code-review gate passed; [[tickets/T-1200-fix-conflicting-extern-declarations]] moved to Done in [[kanban]].
## [2026-10-09] ticket | T-1010 created, In Progress
New ticket [[tickets/T-1010-main-exe-batch-e]] (Backlog -> In Progress on [[kanban]]): decompile src/main/80062CD0.c and src/main/8006CB30.c, worktree branch batch-b-e.

## [2026-10-09] build | T-1010 40 functions matched
Matched 40 of 153 functions in src/main/80062CD0.c and src/main/8006CB30.c (`ninja progress` grand total 244/6962, 27 of 27 sha1 OK). Two `FAKE` marks (`bustup_wink`, `bustup_speech`), two array-form accesses for load/store ordering. New idioms and blocker list in [[matching-notes]] (section Main exe batch E); shared declarations appended to include/game.h.

## [2026-10-09] ticket | T-1010 -> In Review
Card moved In Progress -> In Review on [[kanban]], see [[tickets/T-1010-main-exe-batch-e]]. Inline code review next.

## [2026-10-09] ticket | T-1010 -> Done
Inline code review against CODING_STANDARDS.md found three minor issues (a transient `func_80066104` mismatch, unused declarations in include/game.h, a missing comment on `func_80072B5C`), all fixed. Card moved In Review -> Done on [[kanban]]; see [[tickets/T-1010-main-exe-batch-e]]. Not merged.

## [2026-10-09] build | T-1200 follow-up: batch E merge
After merge c9d2dbf the header check failed again (MASTER.h vs game.h, `func_80046318`, `func_8006612C`, duplicates). Fixed by access widths as in [[tickets/T-1200-fix-conflicting-extern-declarations]]; `tools/check_headers.py` now also checks `src` definitions against headers (caught `func_800634FC`, `func_80065900`).

## [2026-10-09] ticket | T-0017, T-0018 Backlog -> In Progress
[[tickets/T-0017-const-in-reg-loop-hoisting]] and [[tickets/T-0018-ugen-temp-register-order]] moved to In Progress on [[kanban]]: systematic study of the three remaining IDO 5.3 mismatches (u8 compare-chain `$v1`, `* 0x44` as `multu`, `-Wo,-no_const_in_reg` vs loop hoisting), worktree branch t0018-regs.

## [2026-10-09] decision | T-0017 build flag -Wo,-nokpicopt
[[tickets/T-0017-const-in-reg-loop-hoisting]]: `tools/cc.py` builds IDO code with `-Wo,-nokpicopt` instead of `-Wo,-no_const_in_reg`. The original keeps integer constants and array bases in registers but re-addresses scalar globals per access; `-nokpicopt` gives exactly that. The T-0750 `* 0x44` -> `multu` gap is the same cause, not an older ugen. Per-pass IDO 5.3/7.1 mixes and all pass options checked. Tests `tools/test_cc.py` (IdoFlags). `check_end_k` and `func_80042400` C adjusted; 13 functions newly matched (main, EN_NICHI, TAIIKU, DATE, EVENT, GYOZI, SHOUGATU; new headers `include/ovl/DATE.h`, `include/ovl/EVENT.h`, `include/ovl/GYOZI.h`, `include/ovl/SHOUGATU.h`). `ninja progress` 151 -> 164 functions. Evidence in [[matching-notes]] ("Constants in registers"), flag in [[toolchain]].

## [2026-10-09] query | T-0018 compare-chain `$v1` is global register promotion
[[tickets/T-0018-ugen-temp-register-order]]: the original register-promotes scalar globals read in several blocks of straight-line code (189 of 195 re-loading functions keep every load in one register); IDO 5.3/7.1 only in loops. No option, pass mix or C variant reproduces it; not a uniform pass. Evidence and next experiments in [[matching-notes]] ("Register promotion of globals").

## [2026-10-09] ticket | T-0017 In Progress -> In Review; T-0018 In Progress -> Backlog; T-0950 created
On [[kanban]]: [[tickets/T-0017-const-in-reg-loop-hoisting]] to In Review (review gate next), [[tickets/T-0018-ugen-temp-register-order]] back to Backlog with the evidence, new [[tickets/T-0950-match-nokpicopt-unblocked-functions]] in Backlog for the ~1200 functions the flag unblocks. [[index]] updated.

## [2026-10-09] ticket | T-0017 In Review -> Done
Inline code-review gate for [[tickets/T-0017-const-in-reg-loop-hoisting]]: two findings (frame-pass IDO snippet broken by the flag change, missing comment on `check_end_k` operand order), both fixed in tools/test_frame_pass.py and src/main/8004E500.c. Clean rebuild 27/27 sha1 OK, tests OK. Card moved to Done on [[kanban]]; [[matching-notes]] notes the frame-pass side effect.

## [2026-10-09] build | t0018-regs merged (-Wo,-nokpicopt)
Merged branch t0018-regs into main. Regressions under the new flag: `func_80042878` (fixed: `u32` local), `normal_date_bg_fadeout` and `func_80135440` (reverted to INCLUDE_ASM, `$v0`/`$v1` family, see [[matching-notes]]). Clean build 27 of 27 sha1 OK, progress 703 -> 714 of 6962. Header conflicts from the merge (`get_h_*` u32 vs s32, a duplicate) resolved ([[tickets/T-1200-fix-conflicting-extern-declarations]]).

## [2026-10-09] ticket | T-1320 work queue and blocker detector built; T-1321 created
New `tools/queue.py` (tests `tools/test_queue.py`): ranked list of the 6248 remaining `INCLUDE_ASM` functions (main + 26 overlays) with size, leaf, calls and flags L J S P R V; `--files`, `--next`, `--blocked`, `--summary`, `--calibrate`. T-0018 detector (R, V) calibrated on [[data/t0018-cases]] (42 promo rows from [[matching-notes]]) and the 714 matched functions: recall 41/42, 0 matched flagged; 1305 remaining functions flagged. Backlog ticket [[tickets/T-1321-register-promotion-build-step]] holds the evidence and the rule that skipped cases go into the data file. [[decompile-workflow]] starts from the queue; `tools/list_leaves.py` is now a wrapper. Review gate passed inline, T-1320 In Progress -> Done on [[kanban]]; clean build 27 of 27 sha1 OK.
## [2026-10-09] build | T-1330 m2c wrapper and decomp-permuter
[[tickets/T-1330-tooling-m2c-context-and-permuter]]: `tools/m2c.py` (context from `include/`, jump tables and strings from rodata, `mipsel-ido-c`) and `tools/permute.py` (decomp-permuter 8556c81 in the Docker image, compile.sh = `tools/cc.py ido 5.3`, target from the splat .s). m2c vs plain: equal on simple functions (0.927 vs 0.926), wrapper needed for the 902 jump-table functions. Permuter found verified matches for `strSync` and `func_80044700`, only a fakematch for `func_8004111C`, nothing for `LoadSquare`. Documented in [[decompile-workflow]], [[toolchain]], [[matching-notes]]. Clean build 27/27 with the new image. Moved to In Review.

## [2026-10-09] ticket | T-1330 In Review -> Done
Inline review of [[tickets/T-1330-tooling-m2c-context-and-permuter]] against CODING_STANDARDS: two findings (permuter scores ignored stack offsets; stray `--debug` files), both fixed. Card moved to Done on [[kanban]].
## [2026-10-09] ticket | T-1310 created, In Progress -> In Review
[[tickets/T-1310-tooling-object-trailing-padding]]: new `tools/trailing_pad.py` (called by `tools/cc.py`) re-inserts the original's trailing nops after every C function, read from the splat `.s`; replaces the 36 `src/ovl/pad` stubs, handles the 1-nop case asm-processor cannot. Scan: 392 functions with trailing nops (30 main, 362 overlay), all covered. Fixed `pad_text` (objcopy dropped relocations on growth). Ported 8 BUNKAKEN/BUNKASAI functions plus OLH `func_801323CC`, OPTION `func_8013A4FC`, VALEN `func_80133C44`, TT `func_801320F0` (progress 714 -> 726 of 6962). Clean rebuild 27/27 OK. Rule in [[toolchain]], [[matching-notes]], [[build-system]]; CODING_STANDARDS 7a names it as an example.

## [2026-10-09] ticket | T-1310 In Review -> Done
Inline review against CODING_STANDARDS (7a points: uniform, documented, evidence, fails loudly, tested, replaceable; C89 and `INCLUDE_ASM` layout of the ported functions; no game data committed) found nothing open; card moved to Done on [[kanban]]. Not merged.
## [2026-10-09] ticket | T-1300 reuse C across identical functions (Backlog -> Done)
[[tickets/T-1300-reuse-c-across-identical-functions]]: new `tools/dupes.py` (tests `tools/test_dupes.py`, usage in [[decompile-workflow]] and [[build-system]]) fingerprints all functions with relocations masked, groups identical ones and copies matched C into unmatched twins with symbol translation and header declarations, building each object to reject breakage. Result: 50 groups, 163 functions gained (7 reverted, 15 skipped), progress 714 -> 877 of 6962, clean build 27 of 27 sha1 OK. Inline review done, card in Done on [[kanban]]; [[index]] updated.

## [2026-10-09] tooling | docker.sh rebuilds on Dockerfile change
tools/docker.sh labels the image with the SHA-256 of tools/Dockerfile and rebuilds whenever the label does not match, so a merged Dockerfile change reaches every agent without a manual rebuild. See [[build-system]].
## [2026-10-09] ticket | T-1340 jump-table functions: rodata islands, 8 functions matched
New [[tickets/T-1340-tooling-jump-table-functions]], In Review -> Done after the inline code review (no open findings). Result: jump-table functions link byte-identically when the rodata chunk of their object is provided by the C object (`.rodata` sibling subsegment, `tools/rodata_island.py`, `tools/rodata_pieces.py`, asm-processor `.late_rodata`), see [[build-system]] and [[decompile-workflow]]. 902 jump-table functions exist (66 main, 836 overlays); matched 8 (main `func_80053DDC`, `func_8007A254`; RENSYU `func_80133E5C`, ETC `func_8014A2C4`, DATE `func_8013EB1C`, DATE2 `func_80133620`, GYOZI `func_8013AE80`, EVENT `func_8011A874`). Clean build 27 of 27 sha1 OK, `ninja progress` grand total 714 -> 722 of 6962. Limit: one island (one original object's chunk) per C file; more needs per-object C files ([[tickets/T-0500-per-file-game-rodata-data-bss-split]]; the zero padding after tables is new boundary evidence, [[source-files]]). Notes in [[matching-notes]].

## [2026-10-09] tooling | dupes.py re-run after T-1340
Re-ran tools/dupes.py --apply --check after the jump-table merge: 4 copies kept (DATE, EVENT, GEKO, KANGEI), 3 rejected. Clean build 27/27 OK, progress 901/6962.

## [2026-10-09] ticket | T-2030 Wave 2: TACO started
New [[tickets/T-2030-wave-2-taco]] (In Progress): matching `src/ovl/TACO.c`.

## [2026-10-09] ticket | T-2030 Wave 2: TACO done
[[tickets/T-2030-wave-2-taco]] In Progress -> Done after the inline code review (no open findings). 70 functions matched in `src/ovl/TACO.c` (TACO 7 -> 77 of 406), clean build 27 of 27 sha1 OK, `ninja progress` grand total 971 of 6962. 7 rows added to [[data/t0018-cases]]; new patterns in [[matching-notes]] (section "Wave 2 batch TACO"). Tooling bug: `tools/m2c.py` resolves a function name to the first overlay with that name, so a draft for a TACO function can come from another overlay.

## [2026-10-09] merge | wave 2 TACO
Merged [[tickets/T-2030-wave-2-taco]] (70 matches). Header check found draw2d3d declared u8 in game.h and s32 in overlay headers: the u8 view moved to include/main_only.h (src/main/80043510.c and 8005A0B0.c include it), OMIMAI keeps its u8 view in include/ovl/OMIMAI.h, and a duplicate back_clear_switch left include/ovl/TACO.h. Clean build 27/27 OK, 971/6962.
## [2026-10-09] ticket | T-2040 Wave 2 ETC: 190 functions matched
New [[tickets/T-2040-wave-2-etc]], In Progress -> Done after the inline code review (no open findings). `src/ovl/ETC.c`: 190 new matches (197 of 387 now C), clean build all sha1 OK, grand total 1098 of 6962. Four T-0018 rows added to [[data/t0018-cases]]; new patterns (constant-address loads, implicit-int returns, one-symbol table access for as1 scheduling, FAKE pad locals) and a `tools/m2c.py` overlay name collision bug in [[matching-notes]].

## [2026-10-09] merge | wave 2 ETC
Merged [[tickets/T-2040-wave-2-etc]] (190 matches). Header check: set_dec_bri and dec_bg_cd_read prototypes moved from game.h to include/main_only.h (ETC calls them unprototyped, MASTER keeps its u8 view of set_dec_bri in include/ovl/MASTER.h), six duplicate declarations dropped from include/ovl/ETC.h. Clean build 27/27 OK, 1168/6962.

## [2026-10-09] ticket | T-1321 register-promotion build step started
[[tickets/T-1321-register-promotion-build-step]] Backlog -> In Progress (branch o-t0018). Plan: diff IDO 5.3 output against the original over the [[data/t0018-cases]] functions with scripts, look for a uopt option or a ucode-level rule that makes IDO's own allocator promote globals outside loops, and implement it as a uniform pass if one exists.

## [2026-10-09] ticket | T-1321 unsigned-load conversion pass
[[tickets/T-1321-register-promotion-build-step]]: the register-promotion gap of [[tickets/T-0018-ugen-temp-register-order]] is not a loop-only allocator. IDO's cfe widens every unsigned 8/16-bit load (`CVT J<-L`) and uopt allocates the widened value instead of the global; IDO's copy propagation also merges switch temporaries of unsigned globals into the global. New `tools/cvt_pass.py` (uopt shim, tests `tools/test_cvt_pass.py`) removes the widening for unsigned globals, keeps IDO's `==`/`!=` operand order and unsigned loads, and turns uopt's copy propagation off (`OPTN zcopy 0`) for procedures with such a switch temporary. Matched corpus unchanged except two FAKE masks dropped (`bustup_speech`, `bustup_wink`) and two ETC switches on a local copy; clean build 27/27. 26 ETC/TACO functions matched. Evidence and guidance in [[matching-notes]], rule in [[toolchain]], CODING_STANDARDS 7a example. Follow-ups [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]], [[tickets/T-3001-shared-constant-registers]], [[tickets/T-3002-remaining-promotion-shapes]] (Backlog).

## [2026-10-09] ticket | T-1321 Done
[[tickets/T-1321-register-promotion-build-step]] In Progress -> Done after the inline code review (no open findings). Results: clean build 27/27, `ninja progress` 1168 -> 1207; 26 pass-only ETC/TACO matches plus 13 larger R-flagged RPG_BAT/TACO functions that need no pass; detector precision 40 of 59 resolved sampled functions. No uopt patch proposed (threshold prototype rejected). [[tickets/T-0018-ugen-temp-register-order]] commented. Notes in [[matching-notes]], [[data/t0018-cases]].
## [2026-10-09] ticket | T-2010 Wave 2: DATE, 278 functions matched
[[tickets/T-2010-wave2-date]] In Progress -> Done after the inline review (no open findings). `src/ovl/DATE.c`: 278 functions matched (INCLUDE_ASM 637 -> 359), 9 `regorder` rows in [[data/t0018-cases]], new patterns in [[matching-notes]] (section "Wave 2: DATE"). Clean build 27 of 27 sha1 OK, `ninja progress` grand total 901 -> 1179 of 6962.
## [2026-10-09] ticket | T-2060 Wave 2 SHOUGATU started
[[tickets/T-2060-wave2-shougatu]] created, In Progress; scope: `src/ovl/SHOUGATU.c`.

## [2026-10-09] ticket | T-2060 Wave 2 SHOUGATU In Progress -> Done
[[tickets/T-2060-wave2-shougatu]]: 152 SHOUGATU functions matched (overlay 30 -> 182 of 400), 22 T-0018 rows in [[data/t0018-cases]], notes in [[matching-notes]]. Inline review against CODING_STANDARDS: no open findings. Tooling bug: `tools/m2c.py` locates the wrong overlay's asm for shared addresses. Not merged.
## [2026-10-09] ticket | T-2070 wave 2 TT: 49 functions matched
[[tickets/T-2070-wave-2-tt]] In Progress -> In Review -> Done after the inline review (no open findings). `src/ovl/TT.c`: 277 -> 228 `INCLUDE_ASM`; clean rebuild 27 of 27 sha1 OK, `ninja progress` TT 75/303, grand total 950/6962. New patterns, left-over blockers and a `tools/m2c.py` bug (first overlay wins when a function name exists in several overlays) are in [[matching-notes]]; six `regorder` rows added to [[data/t0018-cases]].

## [2026-10-09] ticket | T-2050 Wave 2: GEKO, 176 functions matched
[[tickets/T-2050-wave2-geko]] In Progress -> Done after the inline code review. GEKO went from 367 to 191 `INCLUDE_ASM`; 8 rows added to [[data/t0018-cases]]; notes in [[matching-notes]] (m2c picks the wrong overlay for shared function names; absolute casts for other-overlay addresses). Clean build 27 of 27 OK, `ninja progress` grand total 1077 of 6962.
## [2026-10-09] ticket | T-2020 Wave 2: GYOZI (Backlog -> In Progress)
[[tickets/T-2020-wave-2-gyozi]]: match the remaining functions of overlay GYOZI. Card on [[kanban]].

## [2026-10-09] ticket | T-2020 Wave 2: GYOZI (In Progress -> Done)
[[tickets/T-2020-wave-2-gyozi]]: 178 GYOZI functions matched (GYOZI 244 of 417), 6 rows added to [[data/t0018-cases]], new patterns in [[matching-notes]] ("Wave 2: GYOZI"). Clean build 27 of 27 sha1 OK. Inline review done, no open findings. Not merged.
## [2026-10-09] ticket | T-2000 Wave 2: EVENT started
New [[tickets/T-2000-wave2-event]], In Progress. Matching functions in `src/ovl/EVENT.c`.

## [2026-10-09] ticket | T-2000 Wave 2: EVENT done
[[tickets/T-2000-wave2-event]] In Progress -> Done after the inline review (no open findings). 421 of 781 EVENT functions matched in `src/ovl/EVENT.c`, clean rebuild 27 of 27 OK, `ninja progress` grand total 1322/6962. 11 rows added to [[data/t0018-cases]]; new patterns in [[matching-notes]] (raw-address loads, `*(s32 *)&` for narrow game.h scalars).
## [2026-10-09] ticket | T-2080 wave 2: TAIIKU, RPG_BAT (108 functions matched)
New [[tickets/T-2080-wave2-taiiku-rpg-bat]], In Progress -> Done after the inline code review (no open findings). Matched 108 functions (RPG_BAT 61, TAIIKU 47 new; the three `NON_MATCHING` TAIIKU functions of T-0017 were already plain C). Clean rebuild (`rm -rf asm build`, configure, ninja) 27 of 27 sha1 OK, `ninja progress` grand total 1009 of 6962 in this worktree. New patterns and blockers (shared `$at`, record-field scheduling, loop idioms, direct-global clamps) in [[matching-notes]]; 10 rows in [[data/t0018-cases]]. Tooling bug: `tools/m2c.py` picks the first overlay alphabetically for a function name present in several overlays.

## [2026-10-09] ticket | T-3100 Identify the original game-code compiler started
New [[tickets/T-3100-identify-original-compiler]], In Progress. Scope: O.BIN ECOFF header and symbolic-table forensics (`disc/files/CDROM/EXEDIR/O.BIN`), identification-string scan of the exe and overlays, public documentation of ucode-family compilers, in-image IDO experiments; output [[original-compiler]] and rules for [[tickets/T-1321-register-promotion-build-step]].

## [2026-10-09] ingest | T-3100 O.BIN headers, version stamps and public compiler documentation
New [[original-compiler]] and raw note `wiki/raw/original-compiler-sources.md`.
- O.BIN (`disc/files/CDROM/EXEDIR/O.BIN`): a.out and symbolic-header vstamp are both 0x0312 (3.18), against 3.19 for in-image IDO 5.3 and 7.10 for 7.1. The SGI FAQ maps C 3.18 to IDO 5.2. The `.comment` header is big-endian in a little-endian file, so the link host was big-endian. All 5 procedure descriptors show the +16 frames in the 1995-07-25 developer build.
- No toolchain strings anywhere on `disc/track1.bin`.
- Tool: `tools/obin_syms.py --headers` (tests `tools/test_obin_tools.py`).
- Pages updated: [[obin]] (corrected a.out fields and linker attribution), [[toolchain]], [[matching-notes]] (T-0018 premise corrected: IDO 5.3 promotes in straight-line code at higher reference counts), [[compiler-mismatch-research]], [[index]].

## [2026-10-09] query | T-3100 promotion experiments and rules for T-1321
- In-image IDO only. `-Wo,-regr,N` gives `$v1` selectors without promotion and regresses 194 of 1647 matched functions; rejected.
- The original's switch, compare and `D++` shapes are IDO's own promoted shape at a lower reference count. Rules (promotion threshold, coloring order, not-promoted data, gate) appended to [[tickets/T-1321-register-promotion-build-step]]; follow-up for IDO 5.2/4.1 on [[tickets/T-0100-older-mips-compiler-emulation]].
- Evo's Space Adventures and CelestialAmber/tokimemo checked: both are gcc setups.

## [2026-10-09] ticket | T-3100 In Progress -> In Review -> Done
[[tickets/T-3100-identify-original-compiler]] done after the inline review (no open findings). Clean rebuild 27 of 27 sha1 OK.
## [2026-10-09] ticket | T-2100 Wave 2 small overlays (In Progress -> Done)
[[tickets/T-2100-wave2-small-overlays]]: 94 functions matched in ENDING (17), SHUGAKU (21), NAME_ENT (18), EN_NICHI (16), KANGEI (11), BUNKA_SD, DATE2 (3 each), OMIMAI, VALEN (2 each), OPTION (1); `ninja progress` 901 -> 995 of 6962, clean rebuild 27 of 27 OK, `tools/check_headers.py` OK. Patterns (hit-box tests with shared-bound temporaries, far-address reads, load-hoist trick, parameter width) and tooling notes in [[matching-notes]]; 3 rows added to [[data/t0018-cases]]. Inline review against CODING_STANDARDS found nothing open. Not merged.

## [2026-10-09] setup | versions/ folder
Added versions/ for the user to collect dumps of other releases (BIN+CUE, CHD, ISO). Everything in it is gitignored except versions/README.md, which explains the formats. The build still targets only SLPM_86.053.

## [2026-10-09] ticket | T-0500 In Progress
[[tickets/T-0500-per-file-game-rodata-data-bss-split]] Backlog -> In Progress. Scope widened by the orchestrator: find the original object boundaries of every overlay and of the main game code (text and rodata), split the C files per object so each object provides its own rodata (jump tables, strings), with a migration tool for the wave-2 tree; prove it on RENSYU, OMIMAI, OLH, TEL and `src/main/80062CD0.c`.

## [2026-10-09] decision | T-0500 one C file per original object
The overlays and the main game code are split into their original objects, each C object providing its own rodata island (jump tables, strings, constants). Rejected: one C file per overlay with an object-file pass that splits and reorders `.rodata` (a new emulation pass with no source fidelity); asm-processor `late_rodata`/`INCLUDE_RODATA` alone (places rodata inside one object only). Rationale in [[tickets/T-0500-per-file-game-rodata-data-bss-split]].

## [2026-10-09] build | T-0500 object boundaries, migration tool, proof units
`tools/object_boundaries.py` (440 objects, 357 rodata chunks, evidence and confidence in [[source-files]] and `config/objects/`), `tools/split_objects.py` (migration, idempotent, `config/labels/`), `tools/srcscan.py` C file list from the yaml files; configure, progress, queue, dupes, funcdiff, m2c, permute, trailing_pad, rodata_pieces, gen_overlay_configs, objdiff.json and CI follow ([[build-system]]). `tools/cc.py` compiles UTF-8 string literals as Shift-JIS ([[toolchain]]). Migrated: RENSYU, OMIMAI, TEL, OLH, `src/main/80062CD0.c`; a full trial migration of all 27 units on a copy built 27 of 27 OK. splat mis-split `func_80066A2C`/`func_80066A84` fixed by sizes in `config/symbol_addrs_main.txt`. 18 jump-table/string functions matched (main 2, OLH 12, OMIMAI 4). Clean build 27 of 27 OK, grand total 1184/6958 functions (was 1168/6962).

## [2026-10-09] ticket | T-0500 Done; T-3050, T-3051, T-3052 new
[[tickets/T-0500-per-file-game-rodata-data-bss-split]] In Progress -> Done after the inline code review (no open findings). New: [[tickets/T-3050-run-per-object-migration-after-wave-2]] (Ready: `tools/split_objects.py --all` after wave 2 merges), [[tickets/T-3051-review-low-confidence-object-boundaries]] (Backlog), [[tickets/T-3052-per-object-data-bss-split]] (Backlog).
## [2026-10-09] ticket | T-2090 wave 2 main executable: 61 functions matched
[[tickets/T-2090-wave2-main-executable]] Backlog -> In Progress -> Done (inline review, no open findings). 61 functions matched in 15 `src/main/*.c` files (`ninja progress` grand total 901 -> 962 of 6962), clean rebuild 27 of 27 sha1 OK. New idioms in [[matching-notes]] (section "Main exe wave 2"): byte-flag loops over the 0x38-byte entries, bit-field flag words, `u16`/`s32` parameter types that decide spill and reload code (`LoadSquare` family), indexed symbol views against load hoisting (marked FAKE). 34 T-0018 rows appended to [[data/t0018-cases]]. Tooling bugs: `tools/funcdiff.py` cannot diff names starting with `L`; `tools/srcscan.py` does not see old-style definitions (`ninja progress` aborts). Open blockers: return types fixed by other owners' overlay headers, jump-table functions need a yaml island, T-0018 register choices.


## [2026-10-09] migration | per-object C files for the whole game (T-0500)
Ran tools/split_objects.py --all after wave 2: every overlay and main-exe C file is now one C file per original object with its own rodata island. Fixed the resulting header clashes (four ETC views moved to include/main_only.h, five duplicates dropped from TACO.h/ETC.h). Clean build 27/27 OK, 2734/6958.
## [2026-10-09] ticket | T-3200 Catalog game versions (Backlog -> In Progress)
[[tickets/T-3200-catalog-game-versions]] started: nine archives in versions/ (Rev 1/2/4 zip+7z, Shokai Genteiban, Best, v1.1). Extraction in Docker to scratch only.

## [2026-10-09] ingest | T-3200 game versions catalogued
Nine archives in versions/ hold five distinct discs (compared by track-1 SHA-1): Rev 1 = v1.1 (zip, 7z and the v1.1 7z are identical), Shokai Genteiban (differs from Rev 1 by one PVD byte), Rev 2, Rev 4, PlayStation the Best. New: [[versions]] (tables, lineage Rev 1 -> Rev 2 -> Rev 4 -> Best by PVD date, Ver 1.10/1.25/1.43 and SDK RCS ids, code diffs against SLPM_86.053, recommendation to keep it), `config/versions.txt` (hashes only), `tools/identify_version.py` with `tools/test_identify_version.py`. O.BIN, EVENT.EXN and GYOZI.EXN are identical in all versions; no debug leftovers beyond what [[obin]] covers. Extracted copies deleted.

## [2026-10-09] ticket | T-3200 In Progress -> In Review -> Done
[[tickets/T-3200-catalog-game-versions]] done after the inline review (no open findings). No build files touched.

## [2026-10-09] ticket | T-3110 created, Backlog -> In Progress
[[tickets/T-3110-test-ido-52-and-41]]: test IDO 5.2 and 4.1 (from decomp.me's public compiler distribution, user decision) against the game code. Follow-up of [[tickets/T-3100-identify-original-compiler]] and [[tickets/T-0100-older-mips-compiler-emulation]].

## [2026-10-09] query | T-3110 compiler sources located; download blocked by permissions
decomp.me's distribution: IDO 5.2 from `https://github.com/LLONSIT/qemu-irix-helpers/raw/refs/heads/n/qemu/ido5.2.tar.xz`, IDO 4.1 from `https://github.com/decompme/compilers/releases/download/compilers/ido4.1.tar.gz` (decompme/compilers @ fdd6793). Both run under qemu-irix bundled in the tarball; no OS image needed. The download was refused by the session permission system; [[tickets/T-3110-test-ido-52-and-41]] waits for the user.

## [2026-10-09] query | T-3110 IDO 5.2 and 4.1 measured against the game code
New [[ido-52-evaluation]] and `tools/ido_eval.py`, with cases in `tools/ido_eval_cases/`. Archives are in the gitignored `tools/local-compilers/` (sha256 on the page).
- IDO 5.2: stamp 3.18 (= O.BIN). Byte-identical to 5.3: 2564/2564 matched functions with the frame pass, 504 without; the 46 blocked cases are unchanged.
- IDO 4.1's ugen gives the frame pass's layout in all 2564 functions (real-compiler evidence for the pass), but 80 of them differ in register allocation.
- No version or pass mix reproduces the T-1321 unsigned-load and switch-temporary behaviours, the function-pointer-table frame, or the spill offsets.
- Pages updated: [[toolchain]], [[original-compiler]], [[matching-notes]], [[tickets/T-0100-older-mips-compiler-emulation]], [[index]].

## [2026-10-09] decision | T-3110 recommendation: keep IDO 5.3 + frame pass + T-1321 pass
Option (b). The CI options for a 5.2 switch (fetch from decomp.me's URLs, or the private encrypted bundle) are listed for the user and not implemented.

## [2026-10-09] ticket | T-3110 In Progress -> In Review -> Done
Inline review against CODING_STANDARDS.md: no open findings ([[tickets/T-3110-test-ido-52-and-41]]).
## [2026-10-09] tooling | T-3320 near-duplicate function reuse
[[tickets/T-3320-tooling-near-duplicate-function-reuse]]: new `tools/neardupes.py` (tests `tools/test_neardupes.py`, usage in [[decompile-workflow]] and [[build-system]]). It fingerprints functions with ALU immediates masked as well, groups equal shapes with different constants, copies the matched C through `dupes.plan_copy` and substitutes each differing constant by value when unambiguous. `tools/dupes.py` gained an optional immediate-masking mode (`parse_asm(imm=...)`, `load_funcs(near=True)`); exact mode is unchanged. Report on the tree before the proof: 178 near groups, 19 with a matched source, 66 functions / 34592 bytes fillable (1974 constants), 10 skipped (target header declares a symbol differently 7, no C literal 1, value maps to two constants 1, missing type 1); 59 groups (193 functions, 41972 bytes) have no matched member; 1438 unmatched jump-table and string functions are not fingerprinted (same limit as dupes.py). Proof with `--apply --check` on EN_NICHI, KANGEI, SHUGAKU: 7 of 7 kept, first build, 3408 bytes. Bulk application waits for T-1321 (it changes ETC/TACO C).

## [2026-10-09] ticket | T-3320 In Progress -> In Review -> Done
[[tickets/T-3320-tooling-near-duplicate-function-reuse]] done after the inline review (unused code removed, no open findings). Proof applied in this branch: `src/ovl/EN_NICHI/80136B10.c`, `src/ovl/KANGEI/*.c`, `src/ovl/SHUGAKU/*.c` (7 functions, 3408 bytes) plus the headers the copies needed. Clean build 27 of 27 OK, headers OK, progress 2734 -> 2741 of 6958.
## [2026-10-09] merge | T-1321 merged with main
[[tickets/T-1321-register-promotion-build-step]]: merged main (per-object C, wave 2). `tools/cvt_pass.py` changes 26 of main's 2734 matched functions, so it is out of the build (tool, tests and an `extra_shims` hook in `tools/cc.py` remain); new constant-first rule in the tool. 13 plain-C matches ported to the per-object files. Clean build 27/27, check_headers OK, progress 2747/6958. Open decision moved to [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]]. Notes in [[matching-notes]], [[toolchain]].
## [2026-10-09] ticket | T-3310 native Docker image (Backlog -> In Progress -> In Review -> Done)
[[tickets/T-3310-native-docker-image]] done after the inline review (no open findings). `tools/Dockerfile` builds for amd64 and arm64 (IDO recompiled from the pinned commit on arm64; old-gcc and mkpsxiso amd64 only), `tools/docker.sh` picks the host platform (`TOKIMEMO_PLATFORM` overrides), `.github/workflows/progress.yml` stays on linux/amd64 and now labels the image. Clean build 27 of 27 OK on both images, about 3x faster natively (64 s against 199 s, then 90 s against 252 s on a loaded machine); all `.bin`/`.elf` files identical, objects identical apart from asm-processor's random temp name; 14 tool test files pass on both. See [[toolchain]] ("Native image"), [[build-system]], [[ci]].

## [2026-10-09] tooling | .gitattributes for Windows
Added .gitattributes forcing LF line endings so tools/docker.sh and the Python tools still run in Docker when the repo is cloned on Windows (WSL2 + Docker Desktop is the supported route there). See [[build-system]].
## [2026-10-09] ticket | T-3330 Local function-pointer table frame layout (Backlog -> In Progress -> In Review -> Done)
[[tickets/T-3330-local-fptab-frame-layout]]. A scan of all splat asm found 236 table-copy functions in 15 overlays, none matched. Every one has 4 or more bytes above the table, which IDO's bare-table layout never has. IDO 5.3 experiments show the rule: once a local is in memory, every declared local gets a stack slot, top-down in declaration order. Declaring the index local before the table reproduces `sp+0x2C`/0x90, and `s32 cur; s32 prev;` fixes the GEKO spill. This is a source difference, so no pass and no `tools/cc.py` change; [[ido-52-evaluation]] agrees. Found along the way: one-line test C changes as1's prologue schedule. 18 functions matched in an uncommitted tree (27 of 27 sha1 OK, 2734 -> 2752 of 6958). Their C is in [[data/t3330-fptab-proof.patch]], to apply after T-1321. Notes in [[matching-notes]] (new section "Local function-pointer tables") and [[toolchain]].

## [2026-10-09] apply | T-3330 proof patch
Applied wiki/data/t3330-fptab-proof.patch (3-way, after T-1321 and T-3320 merged): 18 local function-pointer-table functions now C using the scalar-before-table declaration idiom from [[tickets/T-3330-local-fptab-frame-layout]]. Clean build 27/27 OK, 2772/6958.

## [2026-10-09] tooling run | neardupes bulk apply
Ran tools/neardupes.py --apply --check over the whole tree after T-1321/T-3330 merged: 33 of 47 near-duplicate copies kept (20932 bytes), the rest reverted by the per-object check. Clean build 27/27 OK, 2805/6958, 13.4% of code bytes.
## [2026-10-09] ticket | T-3300 Tooling: fix bugs reported by wave 2 (In Review)
[[tickets/T-3300-tooling-fix-wave-2-bugs]]: `tools/m2c.py` (explicit/inferred unit, ambiguous names refused, `%lo` workaround for an upstream m2c bug), `tools/funcdiff.py` (L names, ninja freshness check, host message, `--resolve`), `tools/srcscan.py` (K&R), `tools/identify_version.py` (zip/7z/chd; `tools/Dockerfile` gets p7zip-full and mame-tools). New tests `tools/test_srcscan.py`, `tools/test_funcdiff.py`, more in `tools/test_m2c.py` and `tools/test_identify_version.py`. Details in [[matching-notes]] ("Tooling fixes T-3300"), usage in [[decompile-workflow]], [[versions]], [[toolchain]].

## [2026-10-09] decision | T-3300 game/ drop-in folder
`game/` (gitignored except `game/README.md`) takes the user's disc in any common form and depth; `tools/prepare_disc.py` (ninja step `build/disc.stamp`) finds it, identifies the release with `tools/identify_version.py`, and unpacks the supported one into `disc/files/`. Unsupported releases and a missing image fail with a message; an existing `disc/` and CI's restore keep working. The repo-root zip is not searched any more. README Building, [[build-system]], [[ci]], [[disc-layout]] updated. Tested with the Best 7z, zip, a CHD and a 2048-byte ISO made from the Best disc, and with the Rev 2 zip (refused).

## [2026-10-09] ticket | T-3300 In Review -> Done
[[tickets/T-3300-tooling-fix-wave-2-bugs]] done after the inline review (no open findings). Merged main; the p7zip/mame-tools apt line works on both architectures; [[decompile-workflow]] deduplicated into one ordered procedure (queue, dupes, neardupes, m2c `--unit`, funcdiff `--resolve`, permuter, T-0018 rows, fptab idiom of T-3330). Clean build on the native image from only an archive in `game/`: 27 of 27 OK.

## [2026-10-09] ticket | T-3340 Shared main-exe prototypes and byte-weighted queue (Backlog -> In Progress -> In Review -> Done)
[[tickets/T-3340-shared-main-prototypes-and-byte-queue]]. Part A: new `include/main_api.h` (1317 symbols) is the one declaration of every main-exe function and global, with the type of the main definition (call-site evidence for asm functions); `game.h` keeps the structs, `include/main_only.h` is deleted, the 10 overlay headers that never saw `game.h` now include `main_api.h`. Views an overlay was matched with stay as explicit `MAIN_API_OVERRIDE_<symbol>` defines with a reason (30 left, 14 of them EVENT data over the main bss; `sync_protos.py --prune` retired 79). New `tools/sync_protos.py` (--report --check --write --fix --prune --snapshot --compare, tests `tools/test_sync_protos.py`); `tools/check_headers.py` reads headers with a small preprocessor and runs the same rules, each message names the fix; `tools/dupes.py` declares main-exe symbols in `main_api.h`. Clean build 27/27 OK, 2805/6958 unchanged, `objdump -dr` of all 537 objects identical. Part B: `queue.py --by bytes` (expected bytes per effort, duplicate groups first), `--plan N --agents K`, retuned detector (R, V only for `lbu`/`lhu`; `U0`/`U1` switch selectors blocked-unknown; `T` hint), calibration in [[matching-notes]] (blocked set 29 of 54 recorded rows at 96.7% precision, old rule 36 of 54 at 66.7%). CODING_STANDARDS 8a, [[decompile-workflow]], [[build-system]] updated. `m2c.py` (T-3300) still includes `game.h` before the overlay header; see the ticket notes.


## [2026-10-10] ticket | T-4060 Wave 3 list 6 (In Progress -> Done)
[[tickets/T-4060-wave-3-list-6]]. 26 functions, 3832 bytes matched in 28 files of the list (DATE, DATE2, GYOZI, MASTER, OMIMAI, RPG_BAT, SHOUGATU, TT, VALEN, main 80085E30); clean build 27 of 27 OK, grand total 2831/6958. 17 rows added to [[data/t0018-cases]]. New patterns in [[matching-notes]] ("Wave 3 list 6"): adjacent globals reached through one base keep the original's load order, loop init order for the TT record loops, fptab functions with return values. Blockers: RPG_BAT counters that need an array view in the header, main-bss symbols also declared in EVENT.h, pointer-end loops that IDO unrolls and the original does not, a libgte routine in TACO. Tooling: `funcdiff.py func_X` resolves to a caller's file; objects are not rebuilt while `check_headers.py` fails (stale MATCH).
## [2026-10-09] ticket | T-4040 Wave 3: list 4 (Backlog -> In Progress)
[[tickets/T-4040-wave-3-list-4]]: 119 functions in 27 files (main 8006CB30, BUNKAKEN, DATE, ENDING, EVENT, GEKO, KANGEI, RPG_BAT, SHOUGATU, TACO, TAIIKU, TT).

## [2026-10-10] ticket | T-4040 Wave 3: list 4 (In Progress -> Done)
[[tickets/T-4040-wave-3-list-4]]: 44 functions matched (4924 bytes) in the overlays DATE, TT, SHOUGATU, KANGEI, EVENT, GEKO, BUNKAKEN, TACO, ENDING, RPG_BAT; 28 register-order rows added to [[data/t0018-cases]]; new patterns in [[matching-notes]] (u8 local for masked call results, post-increment compare, `/ -64`, counter-first slot loops, FAKE frames). Clean build 27/27 OK, inline review done, no open findings.
## [2026-10-10] ticket | T-4020 Wave 3 list 2 (Backlog -> In Progress -> Done)
[[tickets/T-4020-wave-3-list-2]]. 60 functions matched (8988 bytes; 59 of the 137 on the list plus DATE `func_8013C624`): DATE 29, EVENT 13, SHUGAKU 7, TACO 5, ENDING 2, GYOZI 2, GEKO 1, TT 1. `ninja progress` grand total 2805 -> 2865 of 6958; clean rebuild (`rm -rf asm build`, configure, ninja) 27 of 27 OK with `build/headers.ok`. 45 rows added to [[data/t0018-cases]]. New patterns in [[matching-notes]] ("Wave 3, list 2"): two-variable `x & 0x7F` with an if/else ladder, `2U` to stop constant sharing, index loops for the unrolled-by-4 shape, the ETC table-symbol trick for SHUGAKU, and natural source recovered from asm where the m2c draft hid it. Unsolved shapes: `multu` with a shared 0x38/0x24 factor, `return const` with the last store in the `jr` delay slot, saved-register constants in loops with calls. Header changes: `func_80082764` returns `s32`; new main-exe externs in `include/main_api.h`; EVENT prototypes in `include/ovl/EVENT.h`. Inline review recorded in the ticket; not merged or pushed.
## [2026-10-10] ticket | T-4010 Wave 3 list 1 (Backlog -> In Progress -> In Review -> Done)
[[tickets/T-4010-wave-3-list-1]]. 51 of the 86 listed functions (11536 of 20848 bytes) matched in main `80047550` and the overlays DATE, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, OPTION, RPG_BAT, SHUGAKU, TACO, TT; clean build 27/27 OK. New patterns (post-increment compares, EVENT debug-printf dispatchers, loop index declared first, one base symbol for neighbouring globals, libgte MATRIX locals) and tooling notes in [[matching-notes]]; 13 rows added to [[data/t0018-cases]]. `include/main_api.h` gained 95 symbols; `func_80082764` now returns `s32`, `func_800AE0B0` is unprototyped. Branch `w3-1`, not merged.
## [2026-10-09] ticket | T-4080 Wave 3: list 8 (Backlog -> In Progress)
[[tickets/T-4080-wave-3-list-8]] created; work list of 127 functions in 26 files, worktree w3-8.

## [2026-10-10] ticket | T-4080 Wave 3: list 8 (In Progress -> Done)
[[tickets/T-4080-wave-3-list-8]]: 32 of the 127 listed functions matched (4908 bytes; grand total 2837 of 6958 functions, 309952 bytes), 19 rows added to [[data/t0018-cases]], patterns and left-overs in [[matching-notes]] ("Wave 3, list 8"). Clean rebuild: 27 of 27 sha1 OK, `build/headers.ok`. Inline review against CODING_STANDARDS.md found nothing open; two `FAKE` unused locals (TAIIKU `func_80133C80`, EVENT `func_800FE090`). Tooling: `funcdiff.py func_X` resolves an overlay name to the wrong object and can print `MATCH` while the overlay fails sha1.
## [2026-10-10] ticket | T-4090 Wave 3, list 9 (In Progress -> Done)
[[tickets/T-4090-wave3-list-9]]: 59 of the 136 listed functions (8984 of 26284 bytes) matched in the main exe and 16 overlays (EVENT, GYOZI, DATE, SHOUGATU, SHUGAKU, GEKO, ENDING, MASTER, OMIMAI, DATE2, ETC, TAIIKU, EN_NICHI, TT list files); clean build 27 of 27 sha1 OK, grand total 2805 -> 2864 of 6958. New patterns in [[matching-notes]] (section "Wave 3, list 9"): `s32 f(void)` without a return value as the first fix for delay-slot and `xori`/`beqz` near misses, the decomp-permuter finds (14 `FAKE` marks), the bit-field flag loop, record loops over `D_8011ECD0`, renamed callees. 18 rows added to [[data/t0018-cases]]. Tooling: `permute.py all <func>` rejects its positional argument; `sync_protos.py --fix` is slow under load (hand-edit `main_api.h` plus `check_headers.py` is equivalent).
## [2026-10-09] ticket | T-4050 Wave 3: list 5 (Backlog -> In Progress)
[[tickets/T-4050-wave-3-list-5]]. Batch agent 5 of wave 3: 134 functions in 27 C files (main 80041000, BUNKASAI, BUNKA_SD, DATE, DATE2, ETC, EVENT, GYOZI, MASTER, OPTION, TACO, TEL, TT, VALEN).

## [2026-10-10] ticket | T-4050 Wave 3: list 5 (In Progress -> Done)
[[tickets/T-4050-wave-3-list-5]]. 67 functions matched in 27 files (63 of the 134 listed, 9232 bytes in all); `ninja progress` 2805 -> 2872 of 6958; clean rebuild 27 of 27 OK. 37 rows added to [[data/t0018-cases]]. New patterns in [[matching-notes]]: bit-field tests for `sll; bgez`, post-increment compares, `(u32)` addresses and index-through-first-symbol fakematches, table size and frame offset of local function-pointer tables, variadic `func_800AE0B0`. Inline review against CODING_STANDARDS recorded in the ticket.
## [2026-10-10] ticket | T-4030 Wave 3: list 3 (In Progress -> Done)
[[tickets/T-4030-wave-3-list-3]]. Branch w3-3 (not merged): 41 functions matched in 20 files (GYOZI 7, main 7, NAME_ENT 5, SHOUGATU 5, DATE 4, TAIIKU 4, ENDING 3, GEKO 2, TT, KANGEI, RPG_BAT, EVENT 1 each); `ninja progress` 2805 -> 2846 of 6958, clean build 27 of 27 OK. 15 rows added to [[data/t0018-cases]]. New patterns in [[matching-notes]] ("Wave 3, list 3"): globals grouped as one struct/array keep the original store/load order in some load-hoisting cases, `D++ == 0` post-increment shape, two-word fptab case with a FAKE pad, unrolled counted loops. Tooling bug found: `tools/funcdiff.py` compares the wrong overlay for function names that exist in several overlays and prints MATCH for any body; only the sha1 is reliable.
## [2026-10-10] ticket | T-4070 Wave 3 list 7 (Backlog -> In Progress -> In Review -> Done)
[[tickets/T-4070-wave-3-list-7]]. 42 of 104 listed functions matched (7160 of 24660 bytes) in 17 files of DATE, DATE2, EVENT, GEKO, GYOZI, KANGEI, OPTION, RPG_BAT, SHOUGATU, SHUGAKU, TACO and the main exe. Clean rebuild: 27 of 27 sha1 OK, grand total 2847/6958 functions. New findings in [[matching-notes]] ("Wave 3, list 7"): one base symbol for record fields stops the load hoist, bit-field access reproduces `sll/bltz` flag tests, an extra register local shifts the next slot. Eight rows added to [[data/t0018-cases]]. Inline review done, no open findings.
## [2026-10-10] ticket | T-4100 Wave 3 list 10 (Backlog -> In Progress -> Done)
[[tickets/T-4100-wave-3-list-10]]. 115 functions (21472 bytes) of the 27 owned files turned into C, 54 of them from the 131-function work list: counted loops over pointer tables, post-increment compares, the T-3330 function-pointer idiom (EVENT debug-print tables, TAIIKU local array copy), string functions, 15 jump-table functions, and 17 `FAKE` single-symbol read-modify-write accesses against IDO's load hoisting. Clean build (`rm -rf asm build; configure; ninja`) 27 of 27 OK, `ninja progress` 2806 -> 2920 of 6958. 39 rows in [[data/t0018-cases]] (selector in $v1 dispatchers, promoted `u8` across loops, shared constants); new patterns and blockers in [[matching-notes]] ("Wave 3 list 10"). Tool bug: `funcdiff.py --built` does not rebuild and printed a stale MATCH; `queue.py` over all files died in Docker.

## [2026-10-10] tooling run | dupes + neardupes after wave 3
tools/dupes.py kept 77 of 79 exact copies and tools/neardupes.py kept 17 of 31 near copies (3668 bytes). Clean build 27/27 OK, 3438/6958, 18.6% of code bytes.

## [2026-10-10] ticket | T-5030 Tooling: wave-3 bug fixes (Backlog -> In Progress -> Done)
[[tickets/T-5030-tooling-fix-wave-3-bugs]]. `funcdiff.py` and `permute.py` find the C file by definition (`tools/funcloc.py`) and refuse ambiguous names; stale objects and string relocations fixed; `sync_protos.py --fix` 5x faster, new `--check-branch` ([[decompile-workflow]] "Before finishing"); `queue.py` caches its asm scan in `build/queue-cache.json`. Details in [[matching-notes]] ("Tooling fixes T-5030").
## [2026-10-10] ticket | T-3001, T-5020 shared constants, lui $at sharing, loop unrolling (Backlog -> In Progress -> Done)
[[tickets/T-3001-shared-constant-registers]], [[tickets/T-5020-loop-unrolling-and-lui-sharing]] (T-5020 created). Corpus scans and hand-run IDO passes; results in [[matching-notes]] ("Shared constants, shared `lui $at` and loop unrolling"), idioms in [[decompile-workflow]] ("Loops: unrolled or not"), summary in [[toolchain]]. (a) constants: uopt difference, no option or C form (124 reuses against 1052 re-loads in the original). (b) `lui $at`: as1 keys the reuse on symbol plus offset in IDO 5.3, on the symbol in the original; 0 conflicts in 10989 address pairs per original object; groups in the new [[data/shared-at-groups]]; no pass (135 matched functions would change). (c) loops: `-Wo,-unrolllimit,N` and `-Wo,-nomultibbunroll` change matched functions and do not fix the blocked ones; source idioms matched main `func_800415B4` and TACO `func_80147400`; the original never unrolls a pointer loop with a run-time remainder. No flag change. Clean build 27/27 OK.
## [2026-10-10] ticket | T-5000 Type recovery: arrays and structs (Backlog -> In Progress)
[[tickets/T-5000-type-recovery-arrays-structs]] created and claimed (worktree r3-types): recover arrays and structs from the access patterns of the original asm and replace the "first symbol" `FAKE` tricks with real aggregate types.

## [2026-10-10] build | T-5000 type recovery: tool, conversions, retries
tools/type_recovery.py (tests tools/test_type_recovery.py) clusters globals from the original asm of all functions: 1724 proposals, 654 high. Converted with the same bytes: `Rec34 D_800B0A04[]`, `Rec38 D_800E643C[]`, `Rec24 D_801217D0[]`, `Work80125D10 D_80125D10`, `RpgRec18 D_8015EC58`, and the 0x44-byte tables through their bases `D_8011ECD0` and `D_800EAFA0`; 33 first-symbol `FAKE`s removed (117 to 84). Overlays take unnamed bases from config/symbol_addrs_types.txt (configure.py adds it to build/main_names.ld). Of 20 load-hoist functions retried, 7 match. New page [[data-types]]; CODING_STANDARDS section 8 rule; step 5a in [[decompile-workflow]]; notes in [[matching-notes]]. Clean build 27/27 OK, 3445/6958.

## [2026-10-10] ticket | T-5000 Type recovery (In Progress -> Done)
[[tickets/T-5000-type-recovery-arrays-structs]]: inline review recorded in the ticket, no open findings. Branch r3-types, not merged.
## [2026-10-10] ticket | T-5010 T-0018 register order, second attempt (created -> In Progress)
[[tickets/T-5010-t0018-register-order-second-attempt]]. Worktree r3-regs. Compare the original's `$v0` and `$v1` switches on every observable source property first (lesson of [[tickets/T-3330-local-fptab-frame-layout]]), then compiler rules; retune the queue detector.

## [2026-10-10] decision | cvt_pass.py in the build with entry and compare rules (T-5010)
Scripts over all 839 compare chains of the original and a per-function rebuild of the 3649 matched C functions: the `$v0`/`$v1` split has two causes. Compiler: a global first touched after a call or branch is not promoted (main-exe unsigned chains: entry `$v1` 260/283, after a call `$v0` 135/155). Source: unit-private data (used by one overlay or main file) keeps `$v0` (28/30) where shared state is `$v1` (258/270), most likely variables defined in the unit; written as a `FAKE` local copy (16 functions). Plus a compare rule (the `CVT` goes only where the value is compared or switched on). `tools/cvt_pass.py` gets the two rules and goes into `tools/cc.py` `SHIMS`; it then changes no matched function. Details: [[matching-notes]] "Selector register rule (T-5010)", [[toolchain]], [[original-compiler]].

## [2026-10-10] build | T-5010 clean build
`rm -rf asm build; configure.py; ninja`: 27 of 27 sha1 OK, headers OK, `ninja progress` 3438 -> 3482 of 6958 (44 functions, 6008 bytes: the 40 pass-only bodies of [[tickets/T-1321-register-promotion-build-step]] and 4 more). Detector (`tools/entry_rule.py`, hook in `tools/queue.py`): R/V/U1 558 -> 264 functions, any blocker 1205 -> 896.

## [2026-10-10] ticket | T-5010 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS 7, 7a, 8a, 9, 11, 13 recorded in [[tickets/T-5010-t0018-register-order-second-attempt]]; no open findings. Notes added to [[tickets/T-0018-ugen-temp-register-order]], [[tickets/T-1321-register-promotion-build-step]], [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]], [[data/t0018-cases]], [[decompile-workflow]].

## [2026-10-10] ticket | T-5100 Recover the main game-state struct (created -> In Progress)
[[tickets/T-5100-game-state-struct]] created and claimed (worktree o-gamestate): one struct for the main bss game state found by [[tickets/T-5000-type-recovery-arrays-structs]], a migration tool for the old `D_` names, the tree migrated byte-identically, then a retry of the blocked functions that touch it.

## [2026-10-10] decision | GameState base 0x800E6280, size 0x1A90 (T-5100)
The original's strength-reduced loops keep 0x800E6280 in the base register and reach arrays up to +0x183C (search_tpage, OPTION, ETC, the save routine), so the game state is one symbol at 0x800E6280; 0x800E6260/0x800E6270 are SDK bss and `D_800E6248` is a compare artifact. `D_800E7D10` is the next object (own base in ETC, `bzero(&D_800E7D10, 0xEE0)`). Layout and evidence: [[game-state]].

## [2026-10-10] build | T-5100 migration and retries
`tools/migrate_globals.py --apply`: 166 globals, 1459 uses rewritten, byte-identical after three layout fixes (members instead of arrays at +0x1093 and +0xFC, field accesses instead of byte views); one use kept on `D_800E66E8`. 191 load-order candidates tried with m2c C: 47 match (none with separate symbols); 26 U0 functions on block selectors: 1 match, not struct-related. `sync_protos.py --write` now keeps the type definitions of `main_api.h`. Clean build 27/27 sha1 OK, headers OK, globals OK, `--check-branch` OK; progress 3491 -> 3539 of 6958. Pages: [[game-state]] (new), [[data-types]], [[decompile-workflow]], [[matching-notes]], [[index]].

## [2026-10-10] ticket | T-5100 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-5100-game-state-struct]]; no open findings. Branch o-gamestate, not merged.

## [2026-10-10] ticket | T-6020 Wave 4 list 2 (created -> In Progress -> In Review -> Done)
[[tickets/T-6020-wave-4-list-2]]: 13 of 151 listed functions matched (2580 bytes) in the 34 owned files; `ninja progress` 3539 -> 3552 of 6958; clean build 27 of 27 OK. Patterns in [[matching-notes]] ("Wave 4 list 2"), six rows in [[data/t0018-cases]]. Inline review recorded in the ticket; branch w4-2, not merged.
## [2026-10-10] ticket | T-6060 wave 4 list 6 (In Progress -> Done)
40 functions, 6724 bytes matched in the main exe and DATE, ENDING, EVENT, GEKO, GYOZI, OMIMAI, RPG_BAT, SHOUGATU, SHUGAKU, TT; 17 T-0018 rows in [[data/t0018-cases]]; progress 3539 -> 3579 of 6958 functions. New patterns (jump-table switches, function-table dispatchers, unsigned compares that unshare constants, in-place loop pointer) and blockers in [[matching-notes]], "Wave 4, list 6 (T-6060)". Inline review in [[tickets/T-6060-wave-4-list-6]]; branch w4-6, not merged.
## [2026-10-10] ticket | T-6030 wave 4 list 3 (In Progress -> Done)
19 of 144 functions matched (3384 bytes, progress 3539 -> 3558 of 6958) on branch w4-3, not merged: [[tickets/T-6030-wave-4-list-3]]. New idioms (read a global back through the global, post-increment compare, `u8` switch without return, `u32` override for a switch, local bit-field views) and the unsolved shapes are in [[matching-notes]] ("Wave 4, list 3"); 5 rows appended to [[data/t0018-cases]]. Inline review recorded in the ticket.
## [2026-10-10] ticket | T-6010 Wave 4 list 1 (Backlog -> In Progress)
Created [[tickets/T-6010-wave-4-list-1]]; 127 listed functions, 39860 bytes, branch w4-1.

## [2026-10-10] build | T-6010 wave 4 list 1: 17 functions
17 functions turned into C in the 35 owned files (16 listed, 3412 bytes, plus TEL `func_801360E4`, 232 bytes); grand total 3539 -> 3556 of 6958, clean build 27 of 27 OK. New patterns (bit-field flag stores, word-aligned record copies, `*arg++` call argument, goto-shared block) and the unmatched families are in [[matching-notes]]; 9 rows in [[data/t0018-cases]].

## [2026-10-10] ticket | T-6010 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-6010-wave-4-list-1]]; no open findings. Branch w4-1, not merged.
## [2026-10-10] ticket | T-6050 Wave 4: list 5 (created -> In Progress)
[[tickets/T-6050-wave-4-list-5]] created and claimed (worktree w4-5): 166 functions, 41008 bytes in 31 files (main 80047550, 800490C0, 80049FF0 and 15 overlays).

## [2026-10-10] build | T-6050 wave 4 list 5
40 of 166 functions matched (6492 of 41008 bytes) in 31 files, plus ETC `func_8014A258` from the byte queue; progress 3539 -> 3580 of 6958. New patterns (implicit int for delay-slot copies and the post-increment boolean, constants of different types, direct globals instead of m2c locals, frame-slot FAKEs, bit-field tests) are in [[matching-notes]] ("Wave 4, list 5 (T-6050)"); 49 blocked cases in [[data/t0018-cases]]. Clean build 27/27 sha1 OK, `sync_protos.py --check-branch` and `migrate_globals.py --check` clean. Tool bug: `sync_protos.py --write/--fix` drops one comment line in `include/main_api.h`.

## [2026-10-10] ticket | T-6050 (In Progress -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-6050-wave-4-list-5]]; no open findings. Branch w4-5, not merged.
## [2026-10-10] ticket | T-6070 started (wave 4, list 7)
Ticket [[tickets/T-6070-wave-4-list-7]] In Progress in branch w4-7: 127 functions, 38952 bytes, 34 files.

## [2026-10-10] build | T-6070 matches (wave 4, list 7)
24 of 127 functions matched (5,520 of 38,952 bytes) in OPTION, ENDING, TACO, TT, RPG_BAT and main 80043510. New findings in [[matching-notes]] ("Wave 4, list 7 (T-6070)"): statements on one source line are scheduled as a unit, `u16` prototype parameters replace casts (`func_8004435C`), `for (i = 0, p = ...)` register order, implicit-int dispatchers, unused-local frame fixes; GameState members read as scalars block about 15 functions. Eleven rows in [[data/t0018-cases]].

## [2026-10-10] ticket | T-6070 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-6070-wave-4-list-7]]; no open findings. Branch w4-7, not merged.
## [2026-10-10] ticket | T-6040 Wave 4: list 4 (created -> In Progress)
[[tickets/T-6040-wave-4-list-4]] created and claimed (worktree w4-4): 185 functions in 36 files of work list 4.

## [2026-10-10] build | T-6040 wave 4 list 4 matches
28 functions of 185 matched (3972 bytes): implicit-int dispatchers, `unk_1104.u` switches, narrow locals, frame-slot locals; progress 3539 -> 3567 of 6958. Blocked shapes (shared `lui $at`, shared constants, end-symbol pointer loops, unprototyped callees, `$t9` skipped in table lookups) and 13 T-0018 rows are in [[matching-notes]] and [[data/t0018-cases]]. Clean rebuild 27/27 OK, `sync_protos.py --check-branch` OK.

## [2026-10-10] ticket | T-6040 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-6040-wave-4-list-4]]; no open findings. Branch w4-4, not merged.
## [2026-10-10] ticket | T-7000 started (shared constants, lui $at, parameter copies)
[[tickets/T-7000-shared-constants-lui-at-parameter-copies]] created and claimed by r4-consts (worktree r4-consts), In Progress.

## [2026-10-10] decision | T-7000 build in K&R mode
The original promotes unsigned char/short to unsigned int (on `lbu`/`lhu` values `sltiu` 1459 against `slti` 3, `srl` 556 against `sra` 0, `divu` 101 against `div` 4): IDO's `-cckr`. `tools/cc.py` adds `-cckr`; `tools/cvt_pass.py` first puts back the widening `CVT` of narrow global loads that IDO's K&R front end drops and the original kept (compare-operand order statistics). 14 matched functions written for ANSI promotion adjusted; clean build 27/27 OK, every other C function unchanged. [[toolchain]], [[matching-notes]] ("K&R promotion rules (T-7000)"), [[original-compiler]].

## [2026-10-10] build | T-7000 verdicts and matches
(c) parameter copies: K&R narrow parameter passed to a callee without prototype, solved. (a) shared constants: K&R constant types and chain assignment to aggregate elements; `$t6`-style temporaries open. (b) shared `lui $at`: data defined in the sharing file; needs per-object data (T-3052), no pass. 16 functions matched, progress 3721 -> 3737 of 6958. Idioms in [[decompile-workflow]] ("K&R promotion"), data note in [[data/t0018-cases]].

## [2026-10-10] ticket | T-7000 (In Progress -> In Review)
[[tickets/T-7000-shared-constants-lui-at-parameter-copies]] In Review; inline review next.

## [2026-10-10] ticket | T-7000 (In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-7000-shared-constants-lui-at-parameter-copies]]; one finding (the `tools/cc.py` docstring did not name `-cckr`) fixed. Branch r4-consts, not merged; the `tools/cc.py` flag change is for the orchestrator's review.

## [2026-10-10] ticket | T-7010 Game-state struct audit (created -> In Progress)
[[tickets/T-7010-game-state-view-audit]] created and claimed (worktree r4-gsaudit): audit how the original reaches the `GameState` fields, choose the source model, implement it, retry the functions wave 4 attributed to "GameState field vs separate symbols".

## [2026-10-10] decision | T-7010 GameState is one object in every unit
`tools/aggregate_audit.py` classified all accesses of the original to 0x800E6280..0x800E7D10: 13625 direct, 201 indexed, 2719 base-relative (129 of 277 objects, 21 of 24 units); 0 of 407 read-modify-write pairs inside the block hoisted (27 of 981 outside). IDO compiles a constant-offset member like a scalar except for alias rules, and 13 of 15 functions wave 4 blamed on "separate symbols" give the same words with the old scalars. Model: one struct reached through `D_800E6280`, no per-unit view. `migrate_globals.py --check` now rejects a `keep` line without a hoisted pair in the original object; the last one (SHOUGATU `D_800E66E8`) is gone. [[game-state]], [[data-types]].

## [2026-10-10] build | T-7010 matches
Bit-field stores (`lui rA; lbu rB`, rB != rA) and one line-scheduling case: DATE `func_8015522C`, `func_8015745C`, SHOUGATU `func_8014147C`, GEKO `func_8013E1A0`, VALEN `func_80133B6C`, ETC `func_80147D74`, KANGEI `func_80135438`, EVENT `func_8010242C`, GYOZI `func_801399C0`; progress 3721 -> 3730 of 6958. Clean build 27/27 OK, headers and globals OK, `sync_protos.py --check-branch` OK. Notes in [[matching-notes]] ("Game-state views audit (T-7010)").

## [2026-10-10] ticket | T-7010 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-7010-game-state-view-audit]]; no open findings. Branch r4-gsaudit, not merged.
## [2026-10-10] ticket | T-7020 created (In Progress)
[[tickets/T-7020-loop-unrolling-and-scheduling]]: loop unrolling the original does not do, and load/delay-slot scheduling differences (wave-4 blockers). Worktree r4-loops.

## [2026-10-10] build | T-7020 loop unrolling and scheduling
Verdict: both wave-4 blockers are source forms; no flag or pass. 28 functions matched (3721 -> 3749). Rules in [[matching-notes]], idioms in [[decompile-workflow]], [[toolchain]] updated. Ticket [[tickets/T-7020-loop-unrolling-and-scheduling]] stays In Progress (helper results pending, review not done).

## [2026-10-10] ticket | T-7020 (In Progress -> In Review -> Done)
25 helper matches applied (total 53, 3721 -> 3774 of 6958); one-line loops marked FAKE. Clean rebuild 27/27 OK, headers OK, globals OK; sync_protos --check-branch OK. Inline review in [[tickets/T-7020-loop-unrolling-and-scheduling]], no findings open.
## [2026-10-10] ticket | T-7030 Tooling wave-4 bug fixes (created -> In Progress)
[[tickets/T-7030-tooling-wave-4-bug-fixes]] created and claimed (worktree r4-fixes).

## [2026-10-10] build | T-7030 tooling fixes (wave 4)
Fixed in worktree r4-fixes: `tools/sync_protos.py` keeps comments and other lines it does not own; `tools/dupes.py` and `tools/neardupes.py` copy game-state fields, override views, file-local typedefs, address literals and prototypes (163 and 25 copies applied, 188 functions, progress 3721 -> 3909 of 6958; before 0 of 5 and 0 of 15); `tools/funcdiff.py` builds `expected/` from the original asm; `tools/permute.py` enforces `--time`, verifies score-0 candidates with ninja and funcdiff, and reports the cause of the discrepancy (several statements on one source line); `tools/m2c.py` takes callee argument counts from the callee's asm (`tools/m2c_args.py`) and repairs the post-increment order. Findings in [[matching-notes]] ("Tooling fixes, wave 4 (T-7030)"), usage in [[decompile-workflow]] and [[build-system]].

## [2026-10-10] ticket | T-7030 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-7030-tooling-wave-4-bug-fixes]]; no open findings. Branch r4-fixes, not merged.

## [2026-10-10] build | T-7000 merged with main (96cae6e) under K&R mode
Merged main (T-7010, T-7020, T-7030, 170 dupes/neardupes copies). Clean build in K&R mode: one main function broke, TAIIKU `func_80144B40` (the original's `slti` on a byte: `(s32)` cast); all dupes/neardupes copies hold; no reverts. 27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK, all tool tests pass; progress 3967/6958 (EVENT `func_80116360` and RPG_BAT `func_8014F1D4` were also matched on main). K&R rules added to CODING_STANDARDS.md section 2; queue.py hint note in [[decompile-workflow]].

## [2026-10-10] ticket | T-8050 started (In Progress)
Wave 5 list 5 batch, [[tickets/T-8050-wave-5-list-5]].

## [2026-10-10] build | T-8050 wave 5 list 5: 24 matches and 7 dupes
Matched by hand: BUNKA_SD `func_80135440`; ETC `func_801395E8`, `func_8013AEE4`, `func_8013B078`, `func_80137A14`, `func_80137C7C`, `func_801380AC`, `func_8013872C`; EVENT `func_800FCDD8`, `func_8010B490`, `func_801073FC`, `func_80106F90`; NAME_ENT nine (six dialog screens, `func_8013B5F0`, `func_8013D62C`, `func_8013E71C`); SHUGAKU `func_80133758`; TT `func_80133B70`, `func_8013F650`. Seven GYOZI twins from `tools/dupes.py`. Five near misses guarded with `NON_MATCHING`. Details in [[matching-notes]], rows in [[data/t0018-cases]], ticket [[tickets/T-8050-wave-5-list-5]]. Branch w5-5, not merged.

## [2026-10-10] ticket | T-8050 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-8050-wave-5-list-5]]; no open findings. `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK.
## [2026-10-10] ticket | T-8010 created (In Progress)
Wave 5 list 1 (141 functions, 35 files), worktree w5-1. See [[tickets/T-8010-wave-5-list-1]].

## [2026-10-10] build | T-8010 wave 5 list 1: 20 functions matched
Matched 20 functions (4048 bytes, 18 from the list): main `func_8007B358`, `func_80059308`, `func_8007A43C`; TT `func_80133058`, `func_80133288`; TAIIKU `func_80138950`, `func_80140738`, `func_8013AF74`; KANGEI `func_80134DB0`; DATE `func_801563A8`, `func_801571D4`, `func_80156494`; ENDING `func_80135DD0`; EVENT `func_801087F8`, `func_80109EE8`; OPTION `func_8013C780`; GYOZI `func_80135988`; OLH `func_801352A0`; TACO `func_801480B0`, `func_80138450`. Clean rebuild 27/27 OK, headers and globals OK, grand total 3987 of 6958 functions. Patterns in [[matching-notes]] ("Wave 5, list 1"), 19 rows in [[data/t0018-cases]].

## [2026-10-10] ticket | T-8010 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-8010-wave-5-list-1]]; no open findings. Branch w5-1, not merged.
## [2026-10-10] ticket | T-8060 wave 5 list 6 started
Opened [[tickets/T-8060-wave-5-list-6]] (In Progress) for the wave-5 batch agent 6 work list.

## [2026-10-10] build | T-8060 wave 5 list 6: 29 functions, 5980 bytes
Matched 29 functions (GEKO 7, TACO 5, TT 3, EVENT 3, SHOUGATU 2, BUNKA_SD 2, one each in SHUGAKU, KANGEI, TEL, DATE, EN_NICHI, RPG_BAT, TAIIKU) in `src/ovl/*`. Clean rebuild (`rm -rf asm build; configure.py; ninja`): 27/27 OK, headers OK, globals OK; `sync_protos.py --check-branch` OK (TACO `func_8015ACCC` declared `s32`); grand line 3996/6958 funcs, 520192/2279368 bytes (22.8%). 18 rows appended to [[data/t0018-cases]]; new patterns in [[matching-notes]] ("Wave 5, list 6"). Commits on branch w5-6, not merged.

## [2026-10-10] ticket | T-8060 done
[[tickets/T-8060-wave-5-list-6]] moved to Done after the inline review against CODING_STANDARDS.md (no open findings).
## [2026-10-10] ticket | T-8070 wave 5 list 7 started
Created [[tickets/T-8070-wave-5-list-7]], In Progress.

## [2026-10-10] build | T-8070 wave 5 list 7: 34 functions matched
34 functions (about 6860 bytes) from the list matched in 10 commits; all 27 sha1 OK after a clean rebuild; grand total 4001/6958 functions, 521056/2279368 bytes. 20 T-0018 rows appended to [[data/t0018-cases]]; idioms and blockers in [[matching-notes]] (section "Wave 5, list 7"). Ticket [[tickets/T-8070-wave-5-list-7]] reviewed inline and moved to Done.
## [2026-10-10] ticket | T-8040 wave 5 list 4 started
Created [[tickets/T-8040-wave-5-list-4]] (In Progress), 151 functions in 37 files.

## [2026-10-10] build | T-8040 wave 5 list 4 done
24 functions matched (GYOZI, TAIIKU, EVENT x2, SHOUGATU, TACO x3, ENDING x3, MASTER, DATE, TT, SHUGAKU x2, ETC x6, main get_g_name) plus 17 dupes copies in owned files. 18 blocked cases added to [[data/t0018-cases]]; patterns in [[matching-notes]]. Review recorded in [[tickets/T-8040-wave-5-list-4]], moved to Done.
## [2026-10-10] ticket | T-8030 wave 5 list 3 started
Created [[tickets/T-8030-wave-5-list-3]] (In Progress) for work list 3, branch w5-3.

## [2026-10-10] build | T-8030 wave 5 list 3 matched
30 of 146 listed functions matched (7512 bytes), progress 3997 of 6958 functions. Clean rebuild 27 of 27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK. 20 rows added to [[data/t0018-cases]]; patterns and unsolved shapes in [[matching-notes]] (section "Wave 5, list 3 (T-8030)"). Inline review recorded in [[tickets/T-8030-wave-5-list-3]]; ticket moved to Done. Branch w5-3, not merged.
## [2026-10-10] ticket | T-8020 created (In Progress)
Wave 5 agent 2, list 2 (36 files). See [[tickets/T-8020-wave5-list-2]].

## [2026-10-10] build | T-8020 wave 5 list 2: 21 functions matched
Matched 21 of 133 listed functions (6,192 bytes) in worktree w5-2: main `func_800623E4`, `func_80062840`; DATE `func_8014D818`, `func_8014DE10`, `func_8014ECF0`, `func_8014D000`; DATE2 `func_8013775C`; SHUGAKU `func_8013A314`; ENDING `func_80133030`; MASTER `func_80139810`; GEKO `func_80141514`; RPG_BAT `func_80135D90`, `func_80159D50`; ETC `func_801408F0`; NAME_ENT `func_80143498`; OPTION `func_801371FC`, `func_80133AD8`, `func_80136A50`, `func_80135C0C`; BUNKA_SD `func_80134B88`; OLH `func_801332F8`. New patterns in [[matching-notes]] ("Wave 5, list 2 (T-8020)"); 17 rows added to [[data/t0018-cases]]. Clean rebuild after `rm -rf asm build`: 27/27 OK.

## [2026-10-10] ticket | T-8020 (In Progress -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-8020-wave5-list-2]]; no open findings. Branch w5-2, not merged.
## [2026-10-10] ticket | T-8080 started
[[tickets/T-8080-wave-5-list-8]] moved to In Progress (wave-5 agent 8).

## [2026-10-10] build | T-8080 matches (wave 5, list 8)
56 functions, 16320 bytes matched in [[tickets/T-8080-wave-5-list-8]]: main 8005A0B0 menu and state machines, EVENT, GYOZI, RPG_BAT scene scripts, ETC, OLH, NAME_ENT, SHOUGATU. Progress 3967 -> 4023 functions. New patterns (`s32` return without a value, `*(u8 *)&D_800E6280.field` views, function-pointer tables with the index passed on, pad rules for menu arrays) in [[matching-notes]]; 19 open cases in [[data/t0018-cases]].

## [2026-10-10] ticket | T-8080 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers and globals OK, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-8080-wave-5-list-8]]; no open findings. Branch w5-8, not merged.

## [2026-10-10] ticket | T-9030 wave-5 tool fixes (Backlog -> Done)
[[tickets/T-9030-wave-5-tool-fixes]]: `tools/dupes.py` and `tools/neardupes.py` `--files`; `tools/sync_protos.py` no duplicate, stable order, `--only`; `tools/check_headers.py` duplicate and alias messages; `tools/funcdiff.py` header failure; `tools/queue.py` leading-nop P; `tools/permute.py` jump-table verification. All tools/test_*.py pass; clean build 27/27, headers OK, progress 4227/6958. Inline review against CODING_STANDARDS.md: no findings (tool code only, tests added, no game data). Branch r5-fixes, not merged.
## [2026-10-10] ticket | T-9010 created (In Progress)
Tooling round 5, worktree r5-data: per-object `.data`/`.bss` ownership ([[tickets/T-3052-per-object-data-bss-split]]) to unlock the shared `lui $at` groups of [[data/shared-at-groups]]. Ticket [[tickets/T-9010-per-object-data-shared-lui-at]].

## [2026-10-10] decision | IDO shares `lui $at` for data defined in the file (T-9010)
IDO 5.3's as1 shares one `lui $at` between stores to one symbol that the C file defines (initialised, static or `.lcomm`) and re-emits for externs, commons and different variables: the original's rule. No as1 pass; the data has to be defined in its object's C file. Corrects the T-5020 note in [[matching-notes]] and [[data/shared-at-groups]].

## [2026-10-10] build | Per-object data ranges and data islands (T-9010)
`tools/object_boundaries.py --data --write`: 248 `data`/`bss` lines in `config/objects/*.txt` ([[source-files]]). New `tools/data_island.py` (opt-in `.data` island per object plus INCLUDE_RODATA piece lines), `tools/data_pieces.py` (pieces and `PROVIDE` file, in the split rules of `configure.py`), `tools/split_objects.py` keeps islands, tests `tools/test_data_islands.py`. Workflow in [[decompile-workflow]], build notes in [[build-system]].

## [2026-10-10] build | T-9010 matches
Islands RPG_BAT `8014E780` and main `800451D0`; 7 functions matched (RPG_BAT `func_8014EBA8`, `func_8014EBD0`, `func_8014EB58`, `func_8014F230`, `func_8014F500`, `func_8014F524`; main `func_80046290`), 13 RPG_BAT files moved to the new struct members. Clean build 27/27, progress 4227 -> 4234.

## [2026-10-10] ticket | T-9010 and T-3052 (-> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9010-per-object-data-shared-lui-at]]; two split_objects findings fixed. [[tickets/T-3052-per-object-data-bss-split]] closed by T-9010. Branch r5-data, not merged.
## [2026-10-10] ticket | T-9000 created (In Progress)
Tooling round 5, worktree r5-regs: re-test and cluster the [[data/t0018-cases]] rows under the K&R build, decide on a register-order pass. See [[tickets/T-9000-t0018-register-order-rule-or-build-step]].

## [2026-10-10] build | T-9000 T-0018 rows re-tested under K&R
Re-tested the open rows of [[data/t0018-cases]] with the wave agents' scratch drafts (201 functions compiled, scratch harness not committed). 20 matched as source idioms (implicit `int` without a return value, bit-field flag stores, chain assignment, two unchanged drafts); `GirlFlag4` added to `include/ovl/GYOZI.h`, `Bits64B8` moved to `include/ovl/SHOUGATU.h`. Clusters and the per-cluster verdict (no uniform ucode pass) in [[matching-notes]]; guidance in [[decompile-workflow]]; note in [[toolchain]]. Clean build 27/27 OK, progress 4227 -> 4247.

## [2026-10-10] ticket | T-9000 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9000-t0018-register-order-rule-or-build-step]]; no open findings. Branch r5-regs, not merged.
## [2026-10-10] ticket | T-9020 created (In Progress)
Tooling round 5, worktree r5-shapes: [[tickets/T-9020-wave-5-codegen-shapes]] collects the eight codegen shapes wave 5 left open ([[matching-notes]], T-8010..T-8080).

## [2026-10-10] build | T-9020 wave-5 codegen shapes
Idioms in [[decompile-workflow]] and [[matching-notes]] ("Wave-5 codegen shapes (T-9020)"): `x * 0x44` shifts are uopt's loop test replacement (separate value counter), a constant stored several times is a chain assignment to array elements, the ENDING reload is an empty `if`, one `CharFlags`/`Rec38Flags` bit-field type for the Rec38 flag word, the TACO frames are the family's `TcPos` locals. 13 functions matched (4227 -> 4240): OPTION 4, DATE 2, TAIIKU 1, ENDING 4, main 2. Shapes 4, 7 and three of shape 8 stay open (rows in [[data/t0018-cases]]; gcc needs an arm64 old-gcc first). Declarations changed: `include/main_api.h` (`D_800CA21C` rows, `Rec38Flags`, `func_80042960` returns `s32`), `include/ovl/ENDING.h`; [[data-types]], [[game-state]] updated.

## [2026-10-10] ticket | T-9020 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers and globals OK, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9020-wave-5-codegen-shapes]]; no open findings. Branch r5-shapes, not merged.

## [2026-10-10] ticket | T-9180 created (In Progress)
Wave 6 list 8, worktree w6-8: [[tickets/T-9180-wave-6-list-8]].

## [2026-10-10] build | T-9180 wave 6 list 8
38 functions matched (13,328 bytes) in 36 files: see [[tickets/T-9180-wave-6-list-8]] and [[matching-notes]] ("Wave 6, list 8 (T-9180)"). Main `src/main/80057390.c` now defines its own `.data` (`tools/data_island.py`), `config/symbol_addrs_types.txt` exports `D_800B5920`. 22 tried functions have rows in [[data/t0018-cases]].

## [2026-10-10] ticket | T-9180 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, `headers OK`, `globals OK`, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9180-wave-6-list-8]]; no open findings. Branch w6-8, not merged.
