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

## [2026-10-10] ticket | T-9150 (Ready -> In Progress)
Wave 6 list 5 started; see [[tickets/T-9150-wave-6-list-5]].

## [2026-10-10] ticket | T-9150 (In Progress -> Done)
Wave 6 list 5: 24 functions, 4,764 bytes matched, 14 T-0018 rows, idioms in [[matching-notes]]; review recorded in [[tickets/T-9150-wave-6-list-5]].
## [2026-10-10] ticket | T-9130 wave 6 list 3 started
Created [[tickets/T-9130-wave-6-list-3]] (In Progress), worktree w6-3.

## [2026-10-10] build | T-9130 wave 6 list 3 matched
37 functions (10,736 bytes) matched in worktree w6-3: GYOZI, DATE2, EVENT, OPTION, KANGEI, TACO, RPG_BAT, BUNKAKEN, TT, SHOUGATU, GEKO, SHUGAKU. 27 rows added to [[data/t0018-cases]], notes in [[matching-notes]] ("Wave 6, list 3"). Ticket [[tickets/T-9130-wave-6-list-3]] reviewed inline and moved to Done. DATE `func_8015A980` and TT `func_8014D260` are gcc SDK code and cannot be built with IDO.
## [2026-10-10] ticket | T-9180 created (In Progress)
Wave 6 list 8, worktree w6-8: [[tickets/T-9180-wave-6-list-8]].

## [2026-10-10] build | T-9180 wave 6 list 8
38 functions matched (13,328 bytes) in 36 files: see [[tickets/T-9180-wave-6-list-8]] and [[matching-notes]] ("Wave 6, list 8 (T-9180)"). Main `src/main/80057390.c` now defines its own `.data` (`tools/data_island.py`), `config/symbol_addrs_types.txt` exports `D_800B5920`. 22 tried functions have rows in [[data/t0018-cases]].

## [2026-10-10] ticket | T-9180 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, `headers OK`, `globals OK`, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9180-wave-6-list-8]]; no open findings. Branch w6-8, not merged.
## [2026-10-10] ticket | T-9170 wave 6, list 7 (In Progress -> Done)
31 functions matched (7344 bytes) from list 7: shared-base accesses, `s32` without return, remainder-first unrolling, string call sequences (OLH help pages, NAME_ENT album prompts, GEKO/EVENT/ETC setups). 24 T-0018 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] ("Wave 6, list 7"). Clean rebuild 27/27 OK, `ninja progress` grand 4298/6958 functions; inline review recorded in [[tickets/T-9170-wave-6-list-7]]. Branch w6-7, not merged.
## [2026-10-10] ticket | T-9110 wave 6 list 1 (In Progress)
Created [[tickets/T-9110-wave-6-list-1]], worktree w6-1.

## [2026-10-10] build | T-9110 wave 6 list 1
29 functions matched (6,688 bytes) in list 1 (leftovers of wave 5): main `src/main/80047550.c`, `src/main/80058D20.c`; overlays TT, ETC, EVENT, GEKO, OLH, RPG_BAT, SHOUGATU, TAIIKU. New idioms in [[matching-notes]] ("Wave 6, list 1 (T-9110)"): one-word struct copy for `lw at`, compiler temporaries as non-locals, switch fall-off returns, pointer parameter prologue, absolute literals for other-overlay data, array view for load order, same-line scheduling, matrix frames. `TaiikuBig` and `D_80122CA0` moved to `include/main_api.h`. 31 rows appended to [[data/t0018-cases]] (v0/v1 and a-register shifts, unfolded constant indexes, `li k; multu`, shared `lui $at` outside the data range).

## [2026-10-10] ticket | T-9110 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9110-wave-6-list-1]]; clean rebuild and checks below. Branch w6-1, not merged.
## [2026-10-10] ticket | T-9120 wave 6 list 2 (In Progress)
Started [[tickets/T-9120-wave-6-list-2]] on branch w6-2.

## [2026-10-10] build | T-9120 wave 6 list 2
39 functions matched (4267 -> 4306): RPG_BAT 23 (dialogue pages `func_80137280` family, state machines), OLH 6 help pages, DATE 2, NAME_ENT 2, GYOZI, main `func_8004482C` (T-9010 island of `src/main/80043510.c`), ETC, SHOUGATU, TACO, VALEN. Patterns (template pages, `(u8)` around a masked call result, `D |= a; D &= ~b;`, bit-field flag stores, one-line loop header, table-frame idioms) in [[matching-notes]], "Wave 6, list 2"; the failures are rows in [[data/t0018-cases]]. TACO `func_8015CF30` is hand-scheduled libgte assembly. Changed declarations: `include/main_api.h` (`func_8009ED74`), `include/ovl/{ETC,NAME_ENT,OLH,RPG_BAT,TACO}.h`; `config/SLPM_86.053.yaml` (island line by `tools/data_island.py`).

## [2026-10-10] ticket | T-9120 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers and globals OK, `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9120-wave-6-list-2]]; no open findings. Branch w6-2, not merged.
## [2026-10-10] ticket | T-9140 wave 6 list 4 (In Progress)
Created [[tickets/T-9140-wave-6-list-4]], worktree w6-4.

## [2026-10-10] build | T-9140 wave 6 list 4
28 functions matched (7,804 bytes), progress 4295/6958: GYOZI `func_8013B43C`, main `strSync` and 80079B10/800563F0/80053650 helpers, 13 OLH help pages (mechanical template), DATE2/EVENT/RPG_BAT/BUNKA_SD/ENDING/BUNKASAI init and dispatch functions. Idioms and the open near misses in [[matching-notes]] ("Wave 6, list 4 (T-9140)"); 22 rows in [[data/t0018-cases]]. Declarations changed: `include/main_api.h` (scalar views of splat data pieces, `func_8007BF04` takes `s16`), `include/ovl/OLH.h`, `DATE2.h`, `EVENT.h`; `config/symbol_addrs_main.txt` gained `D_800E7D13`.

## [2026-10-10] ticket | T-9140 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers and globals OK, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9140-wave-6-list-4]]; no open findings (two FAKE marks, `func_80056284` and `func_8013FD90`). Branch w6-4, not merged.
## [2026-10-10] ticket | T-9160 (Backlog -> In Progress)
Wave 6 list 6 started: [[tickets/T-9160-wave-6-list-6]].

## [2026-10-10] build | T-9160 wave-6 list 6
20 functions matched (4267 -> 4287): SHOUGATU 3, SHUGAKU 1, TACO 6, TAIIKU 4, main 2, EVENT 1, GEKO 1, OPTION 1, EN_NICHI 1. Three data islands cut (TAIIKU/80142240, OPTION/801389A0, EN_NICHI/80132000); owned variables defined in `src/ovl/TAIIKU/80142240.c`, `src/ovl/OPTION/801389A0.c`, `src/ovl/EN_NICHI/80132000.c`. 35 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] ("Wave 6, list 6 (T-9160)"): owned data with direct use fixes `$v0`/`$v1`, one-statement `switch` for the out-of-line block, `D_8014A13C[0x90]` as one object for the shared `lui $at`.

## [2026-10-10] ticket | T-9160 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, `headers OK`, `globals OK`, `sync_protos.py --check-branch` OK. Inline review against CODING_STANDARDS.md in [[tickets/T-9160-wave-6-list-6]]; no open findings. Branch w6-6, not merged.

## [2026-10-10] ticket | T-9220 (Backlog -> In Progress)
Round-6 tool fixes started: [[tickets/T-9220-wave-6-tool-fixes]].

## [2026-10-10] build | T-9220 tool fixes
Seven fixes in `tools/funcdiff.py`, `tools/data_island.py`, `tools/sync_protos.py`, `tools/queue.py` with tests; `include/main_api.h` gained `D_800B66E0`/`D_800B66F4` (moved from `src/main/800674B0.c`). Details in [[tickets/T-9220-wave-6-tool-fixes]]; workflow notes in [[decompile-workflow]].

## [2026-10-10] ticket | T-9220 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers OK, globals OK, progress 4521/6958, `sync_protos.py --check-branch` OK. Inline review recorded in the ticket, no open findings. Branch r6-fixes, not merged.
## [2026-10-10] ticket | T-9200 (Backlog -> In Progress)
Tooling round 6, gcc-built objects inside overlays: [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]]. Branch r6-gcc.

## [2026-10-10] decision | T-9200 which wave-6 "gcc" functions are gcc
`tools/gcc_fingerprint.py` ([[gcc-objects]]): 8 overlay functions carry the PsyQ fingerprint. The six rotation helpers are hand-written (not gcc output at any version 2.6.3 to 2.91.66); TAIIKU `func_801488F0` is gcc 2.8.1-psx C; TT `func_8014D260` is gcc 2.7-class C. TACO `func_801420DC`/`func_801421AC` are IDO C, not gcc.

## [2026-10-10] build | T-9200 per-object toolchain and gcc on arm64
`config/toolchains.txt` read by `configure.py`; `tools/Dockerfile` builds gcc 2.7.2-psx and 2.8.1-psx from source on arm64 (output identical to the amd64 binaries on 110 sources); `tools/cc.py` gcc path runs maspsx `--expand-div`. Matched TAIIKU `func_801488F0` (gcc), TACO `func_801420DC`, `func_801421AC` (IDO). Clean build 27/27 OK, progress 4521 -> 4524. Details: [[toolchain]].

## [2026-10-10] ticket | T-9200 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md recorded in [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]]; no open findings.
## [2026-10-10] ticket | T-9210 wave-6 codegen shapes (new, In Progress)
Tooling round 6, worktree r6-shapes. Six shapes left open by wave 6 ([[matching-notes]] T-9110..T-9180 sections). Ticket [[tickets/T-9210-wave-6-codegen-shapes]].

## [2026-10-10] build | T-9210 wave-6 codegen shapes (partial)
Idioms in [[decompile-workflow]] and [[matching-notes]] "Wave-6 codegen shapes (T-9210)". 15 functions matched in branch r6-shapes (TACO 2, GEKO 3, GYOZI 3, SHUGAKU 1, TT 4, EVENT 2) plus the K&R rewrite of TT `func_80148974`; `tools/check_headers.py` reads K&R parameter declarations correctly (test added). Sweeps of shape 1 and shape 5 run in branches r6-shapes-m1 and r6-shapes-m5, not merged yet. Ticket [[tickets/T-9210-wave-6-codegen-shapes]] stays In Progress.

## [2026-10-10] ticket | T-9210 (In Progress -> In Review -> Done)
Merged origin/main; clean rebuild 27/27 OK, headers OK, globals OK, `--check-branch` OK, progress 4539/6958. Inline review in [[tickets/T-9210-wave-6-codegen-shapes]], no open findings. Sweep branches had no confirmed matches; nothing cherry-picked.

## [2026-10-10] ticket | T-9350 wave 7 list 5 (In Progress)
[[tickets/T-9350-wave-7-list-5]] created, branch w7-5.

## [2026-10-10] build | T-9350 wave 7 list 5 (16 functions)
16 functions matched (about 4880 bytes) in branch w7-5, 20 rows in [[data/t0018-cases]], notes in [[matching-notes]] ("Wave 7, list 5"). Progress 4574/6958.

## [2026-10-10] ticket | T-9350 (In Progress -> In Review -> Done)
Inline review in [[tickets/T-9350-wave-7-list-5]], no open findings. Clean rebuild of all 27 units not finished (Docker host overloaded); every touched unit rebuilt from clean and OK.
## [2026-10-10] ticket | T-9360 In Progress
Wave 7 list 6 started, see [[tickets/T-9360-wave-7-list-6]].

## [2026-10-10] build | T-9360 wave 7 list 6 (partial)
3 functions matched (TAIIKU `func_80146B8C`, main `icon_disp_switch`, GEKO `func_8013A6D0`, 648 bytes); 26 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] "Wave 7, list 6 (T-9360)". Clean rebuild 27/27 OK, headers and globals OK, `--check-branch` OK, progress 4561/6958.

## [2026-10-10] ticket | T-9360 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md in [[tickets/T-9360-wave-7-list-6]], no open findings.
## [2026-10-10] ticket | T-9340 wave 7 list 4 (In Progress)
Created [[tickets/T-9340-wave-7-list-4]]; work list of 142 functions in 36 files.

## [2026-10-10] build | T-9340 wave 7 list 4
9 functions matched (2960 bytes) in branch w7-4: main `func_8006B5BC`, OPTION `func_80137864`, SHOUGATU `func_80134400`, ENDING `func_80137340`/`func_801380B0`/`func_80139498`, GYOZI `func_8013D294`, TT `func_80144094`/`func_80145E60`. About 30 rows added to [[data/t0018-cases]]; notes in [[matching-notes]] "Wave 7, list 4 (T-9340)".

## [2026-10-10] ticket | T-9340 (In Progress -> Done)
Clean rebuild 27/27 OK, headers OK, globals OK, `sync_protos --check-branch` OK, progress 4567/6958. Inline review in [[tickets/T-9340-wave-7-list-4]], no open findings.
## [2026-10-10] ticket | T-9370 wave 7 list 7 started
Ticket [[tickets/T-9370-wave-7-list-7]] In Progress.

## [2026-10-10] build | T-9370 wave 7 list 7: 19 functions matched
SHUGAKU 1, TACO 5, GYOZI 1, TT 3, EVENT 2, RPG_BAT 5, KANGEI 2 (6,780 bytes); notes in [[matching-notes]] "Wave 7, list 7 (T-9370)": unroll left-over stores, locals for hoisted globals, statement order = register numbering, unsigned counter for a separate constant, unused-array frame padding, permuter finds (`*a = *b = 0`, `(P + idx)->f`, `^ 0`). Four fakematches marked `FAKE` (TACO `func_80148308`, `func_80151F94`, `func_80153068`, RPG_BAT `func_80157CCC`). 24 rows appended to [[data/t0018-cases]]; one tooling gap: TACO `func_8013F250` matches in C but its strings are the orphan rodata chunk `8015DA90`. Clean rebuild 27/27 OK, headers OK, globals OK, `--check-branch` OK.

## [2026-10-10] ticket | T-9370 (In Progress -> In Review -> Done)
Inline review against CODING_STANDARDS.md in [[tickets/T-9370-wave-7-list-7]], no open findings. Not merged or pushed.
## [2026-10-10] ticket | T-9330 wave 7 list 3 (In Progress)
[[tickets/T-9330-wave-7-list-3]] opened for work list 3.

## [2026-10-10] build | T-9330 wave 7 list 3
17 functions matched (6,244 bytes) in branch w7-3: main `check_para_limit`, DATE 2, EVENT 2, ETC 1, EN_NICHI 3, RPG_BAT 2, TACO 5, VALEN 1. 23 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] (Wave 7, list 3). Files touched: `src/main/8007C030.c`, `src/ovl/*`, `include/ovl/*.h`, `config/symbol_addrs.txt`.

## [2026-10-10] ticket | T-9330 (In Progress -> In Review -> Done)
Inline review in [[tickets/T-9330-wave-7-list-3]], no open findings; clean rebuild 27/27 OK.
## [2026-10-10] ticket | T-9310 started
[[tickets/T-9310-wave-7-list-1]] In Progress (wave 7 agent 1).

## [2026-10-10] build | T-9310 wave 7 list 1 (30 functions)
30 functions matched (10,492 bytes, 4558 -> 4588 of 6958) in branch w7-1: main 4, EVENT 5, DATE 2, DATE2 1, GEKO 2, OLH 2, OPTION 6, RPG_BAT 2, SHUGAKU 1, TACO 2, TAIIKU 2, TT 1. Idioms in [[matching-notes]] ("Wave 7, list 1") and [[decompile-workflow]] ("Wave-7 codegen shapes"); 24 rows in [[data/t0018-cases]]. Sources changed: `src/main/800451D0.c`, `src/main/8004F870.c`, `src/main/800737A0.c` and overlay files under `src/ovl/`; headers `include/main_api.h` and `include/ovl/*.h`.

## [2026-10-10] ticket | T-9310 (In Progress -> In Review -> Done)
Clean rebuild (`rm -rf asm build`, configure, ninja) all sha1 OK, headers OK, globals OK; `sync_protos.py --check-branch` OK; `migrate_globals.py --check` OK. Inline review in [[tickets/T-9310-wave-7-list-1]], no open findings. Not merged or pushed.

## [2026-10-10] build | T-9310 wave 7 list 1, update (34 functions)
Four more matches after the entry above: EVENT `func_800F8C2C`, TACO `func_801573C8`, `func_80157AE0`, `func_8015800C` (4,908 bytes). Totals: 34 functions, 15,400 bytes, 4558 -> 4592 of 6958; clean rebuild all OK again. Details in [[matching-notes]] ("Wave 7, list 1") and [[tickets/T-9310-wave-7-list-1]].
## [2026-10-10] ticket | T-9320 wave 7 list 2 (In Progress)
Ticket [[tickets/T-9320-wave-7-list-2]] created; worktree w7-2 built 27/27.

## [2026-10-10] build | T-9320 wave 7 list 2
39 functions matched in branch w7-2 (18116 bytes; EVENT 5, TACO 19, TT 2, TEL 2, SHOUGATU 2, KANGEI 2, GEKO 1, RPG_BAT 1, OLH 1, main 4). New patterns in [[matching-notes]] "Wave 7, list 2" (TACO range-chain template, two-counter table loops, repeated expression against temp variable, permuter burn tricks). 29 rows added to [[data/t0018-cases]]. Clean rebuild 27/27 OK, progress 4597/6958.

## [2026-10-10] ticket | T-9320 (In Progress -> In Review -> Done)
Inline review in [[tickets/T-9320-wave-7-list-2]], no open findings; `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK. Not merged.
## [2026-10-10] ticket | T-9380 started
[[tickets/T-9380-wave-7-list-8]] In Progress: wave 7 list 8.

## [2026-10-11] build | T-9380 wave 7 list 8 (27 functions matched)
Matched BUNKA_SD `func_801345BC`, DATE `func_8013FA6C`, `func_8013A344`, `func_8013D174`, `func_80136A74`, `func_8014E38C` and the ten-function date-spot family (`func_80137920` + nine twins), SHOUGATU `func_80133D98`, NAME_ENT `func_8014A848`, `func_80146BCC`, EN_NICHI `func_801383A0`, `func_80137E2C`, RPG_BAT `func_8013A2F4`, `func_8013D59C`, `func_801439E0`, ENDING `func_80137160`, TT `func_8014AFB8`, `func_8014D168`. Three drafts behind NON_MATCHING (EN_NICHI `func_80137C58`, TT `func_8014CC04`, `func_8014B088`). Data islands DATE `8014CC40`/`80132000`, NAME_ENT `80144AD0`, EN_NICHI `80136B10`. Patterns in [[matching-notes]] "Wave 7, list 8"; 21 rows in [[data/t0018-cases]]. Tooling bug: `tools/data_island.py` omits the INCLUDE_RODATA lines of pointer-only labels (NAME_ENT/80144AD0), the overlay sha1 then fails.

## [2026-10-11] ticket | T-9380 (In Progress -> In Review -> Done)
Clean rebuild 27/27 OK, headers OK, globals OK, `--check-branch` OK, progress 4585/6958. Inline review in [[tickets/T-9380-wave-7-list-8]], no open findings. Not merged or pushed.

## [2026-10-11] ticket | Claim wave-7 tool fixes
Read shared brief, standards and wiki; created [[tickets/T-9400-wave-7-tool-fixes]] in In Progress. Baseline clean Docker build passed all 27 checksums.

## [2026-10-11] build | Wave-7 tooling regressions and orphan placement
Fixed exact duplicate-copy selection, complete data-island includes, K&R canonical declarations, funcdiff labels/external and empty strings, permuter false rejection, and shared-symbol guard. Synthetic Docker tooling suite passed; additional one-overlay-file split regression passed. OPTION table noise and GEKO string noise now resolve. An isolated TACO split at 8013F250 rebuilt its sha1 and assigned both orphan string blocks to the new island; source-owned files unchanged. Recorded the supported recipe in [[build-system]] and current data-island guidance in [[decompile-workflow]]; details in [[matching-notes]] and [[tickets/T-9400-wave-7-tool-fixes]].

## [2026-10-11] ticket | T-9400 In Progress -> In Review
Final clean Docker build passed 27/27 sha1, headers/globals/symbols stamps exist, progress 4729/6958 unchanged, branch check has 0 new disagreements (17 known). All 30 tooling test modules passed, plus targeted reruns after later changes. Inline standards review recorded in [[tickets/T-9400-wave-7-tool-fixes]], no unresolved findings; the TACO C draft is a follow-up using the proven split recipe in [[build-system]].

## [2026-10-11] lint | T-9400 ticket and board
Checked the touched ticket/board/index links and status: one T-9400 card, correct Done column, linked ticket present; relevant wiki pages refreshed. Existing log entries left unchanged.

## [2026-10-11] ticket | T-9400 In Review -> Done
Inline CODING_STANDARDS review resolved all findings; 46 funcdiff tests passed on final code, final ninja recheck passed all 27 checksums and branch check OK. [[tickets/T-9400-wave-7-tool-fixes]] complete. Uncommitted tooling/tests/wiki edits left for orchestrator; no source/header/config-object changes or function gains.
## [2026-10-11] ticket | T-9410 register-allocation investigation

Created [[tickets/T-9410-register-allocation-shapes-after-wave-7]] and moved its card to In Progress. Read the tooling7 shared brief and existing [[decompile-workflow]] evidence; generating original assembly before clustering [[data/t0018-cases]].

## [2026-10-11] query | T-9410 original-assembly inventory and baseline

New tools/register_shapes.py classifies historical rows plus original-assembly features, without claiming compiler causes. Inventory: 1,088 rows, 718 distinct functions, 344 already C and 374 open. Largest open buckets: constant allocation/sharing 82, selector/compare chain 60, pointer/base/CSE 51. Family annotations use their first named function; duplicate rows remain auditable by line. Baseline Docker configure/build passed all 27 checksums. See [[tickets/T-9410-register-allocation-shapes-after-wave-7]].

## [2026-10-11] build | T-9410 ENDING remainder match

ENDING func_80138278 now passes tools/funcdiff.py --resolve and the overlay SHA1. A dead `if (D_800E6280.unk_10F8) {}` before the unsigned remainder moves the dividend to v1 and removes the extra temporary; marked FAKE, T-9410. The 144-variant scratch sweep found the same result with u32/s32/register u32 remainder locals and one or two redundant tests. Return-type changes conflicting with the existing header were compile errors, not evidence. The TT draft sweep tried 180 variants without a match. Details will be consolidated in [[decompile-workflow]] and [[tickets/T-9410-register-allocation-shapes-after-wave-7]].

## [2026-10-11] build | T-9410 selector and second ENDING match

Main func_8005AE60 matches with a plain u8 local selector loaded after func_8005AD70; resolved function diff and main SHA1 pass. ENDING func_80138FC8 also passes its resolved diff and overlay SHA1 with the dead dividend test. Added [[register-allocation-wave7]] with the baseline cluster counts and bounded experiments, and documented both idioms in [[decompile-workflow]]. Fade/girl: 576 variants, no match. DATE draft needed its six destination accesses adapted to the existing s16 arrays before codegen experiments were valid; 46 compiling variants still do not match.

## [2026-10-11] build | T-9410 ENDING family and narrow compare constant

The dead dividend test also matches ENDING func_801391E4 after restoring the ordinary scene if/else chain. func_8013855C additionally needs an s16 local holding 1 for two comparisons; s32/u32 locals and direct literal operand swapping did not fix the branch operands. Both source tricks are marked FAKE. Five gains total including the main selector; two historical rows marked resolved by an appended ledger in [[data/t0018-cases]], preserving the original rows. Added synthetic unit/duplicate/guarded-draft/missing-asm tests for tools/register_shapes.py.

## [2026-10-11] ticket | T-9410 In Review

Moved [[tickets/T-9410-register-allocation-shapes-after-wave-7]] and its kanban card to In Review. All five gained functions pass resolved diffs and affected-unit SHA1 checks. The clean build and final guards are running. Updated [[register-allocation-wave7]] with valid experiment counts, the two resolved row anchors, all five gains, and the final GameState overlap counts. No cluster is claimed impossible from C.

## [2026-10-11] build | T-9410 clean proof and review complete

Clean Docker configure/ninja build after deleting asm and build: 27/27 SHA1 OK. build/headers.ok and build/globals.ok exist; tools/sync_protos.py --check-branch exits zero; tools/test_register_shapes.py passes all three synthetic tests; git diff --check passes. Progress: 4734/6958, 729612 bytes, +5 functions/+2768 bytes over baseline. Inline standards/spec review found no unresolved findings. Moved [[tickets/T-9410-register-allocation-shapes-after-wave-7]] and its card to Done; full evidence in [[register-allocation-wave7]]. Uncommitted changes left for the orchestrator; no existing r7-fixes tooling files changed.

## [2026-10-11] ticket | T-9530 In Progress
Claimed wave-8 list 3: 136 functions in 36 owned C files. Read the shared brief, standards, [[decompile-workflow]], and [[register-allocation-wave7]]. Created [[tickets/T-9530-wave-8-list-3]] and its kanban card.

## [2026-10-11] decision | T-9530 bounded source probes
Fresh m2c drafts expose known register/pointer shapes and missing record views. Use explicit assembly paths with the required unit to avoid repeated global assembly scans. Probe candidate C with the project's compiler in temporary Docker files while the baseline builds; only adopt candidates after full build and resolved funcdiff verification. Details will be recorded in [[matching-notes]] and [[tickets/T-9530-wave-8-list-3]].

## [2026-10-11] build | T-9530 baseline and duplicate scan
Docker incremental baseline passed all 27 linked-unit SHA1 checks, headers, and globals. Scoped dupes scan in the owned files found no applicable copies. A source probe for TAIIKU `func_801345A4` matches; linked verification is pending. [[tickets/T-9530-wave-8-list-3]].

## [2026-10-11] build | T-9530 first match and source idioms
TAIIKU `func_801345A4` passes resolved funcdiff and the complete 27-unit build. Probes also match TAIIKU `func_80134D94` after reusing its loop counter, GYOZI `func_8013CB18` with the existing GirlFlag4 view, and TACO `func_80153774` with explicit range chains. These candidates await linked verification. All 136 list entries received a fresh m2c attempt; recoverable drafts received bounded compiler probes. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 six more candidates adopted
Adopted the two TAIIKU/GYOZI probes, the TACO range chain, and three RPG_BAT switches in the owned C files. The RPG drafts omitted a third call argument even though the callee's matched definition takes three; restoring `500` produces exact source probes. Appended overlay declarations and queued the missing main function declaration for canonical generation. Full linked verification is next. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 seven linked matches and final near misses
All seven adopted functions pass resolved funcdiff; the full Docker build passes 27/27 SHA1 checks, headers, and globals. A final GYOZI byte-store-in-argument probe also matches and is being adopted. Three bounded 60-second permutation searches produced no score-zero result. Fresh register and recovery evidence stays in the scratchpad until summarized in [[tickets/T-9530-wave-8-list-3]] and [[matching-notes]].

## [2026-10-11] build | T-9530 array access fixes the final flag-load order
KANGEI `func_80138D40` matches as a signed-halfword loop with flags accessed through the declared byte array, avoiding the load hoist caused by separate scalar names. Adopted it and GYOZI `func_8013D764`, appended their overlay prototypes, and started configure plus the final incremental build. Nine candidates, 4,040 bytes. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] ingest | T-9530 bounded register retry evidence
Appended ten fresh retry rows to [[data/t0018-cases]] after consulting the T-9410 clusters and original assembly. Limited claims to observed allocation, pointer, frame, and scheduling differences; malformed drafts and broader control-flow recovery remain separate time boxes. The ENDING permutation search produced tied score-45 candidates only. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] lint | T-9530 inline review corrections
Applied the local code-review skill's standards/spec axes inline, as the wave brief requires. Removed KANGEI's obsolete NON_MATCHING alternative now that the default C matches; marked GYOZI's redundant byte-lvalue cast FAKE and tracked its argument-sharing purpose. A final scratch unprototyped brightness-call experiment isolates ENDING's remaining v0/a1 global allocation, appended as the eleventh T-0018 retry. All 136 outcomes are recorded in [[tickets/T-9530-wave-8-list-3]]. Final revalidation follows these review corrections.

## [2026-10-11] ticket | T-9530 In Review
Moved [[tickets/T-9530-wave-8-list-3]] and its kanban card to In Review after all listed functions were matched or time-boxed. Nine linked matches / 4,040 bytes; eleven T-0018 rows. Declaration and aggregate guards pass. Review corrections are complete; final rebuild and checksum revalidation are pending before Done.

## [2026-10-11] query | T-9530 final duplicate scans
Owned-file duplicate scans applied zero copies before and after configure; scoped near-duplicate report found zero fillable functions. Their printed matched count remains the assembly-directory baseline (4734), while source progress reports 4743. This is recorded as a tooling limitation in [[tickets/T-9530-wave-8-list-3]]; no tool or unowned source was changed.

## [2026-10-11] build | T-9530 final proof and Done
Final incremental Docker ninja passes; independent current-binary SHA1 verification is 27/27 OK. All nine gained functions match resolved funcdiff, including review-touched GYOZI/KANGEI functions rechecked after corrections. Headers/globals stamps exist, branch prototype guard reports zero new disagreements, aggregate check and git diff --check pass, staging is empty. Inline standards/spec review against CODING_STANDARDS.md has no unresolved findings. Moved [[tickets/T-9530-wave-8-list-3]] and its kanban card to Done. Progress: 4743/6958, 733632/2279368 bytes (32.2%); +9 functions / 4040 bytes, eleven T-0018 retry rows. Changes remain uncommitted for the orchestrator.

## [2026-10-11] ticket | T-9530 reopened for a deep second pass
User committed the first pass and requested sustained per-function iteration plus missing aggregate/local-copy recovery. Reopened [[tickets/T-9530-wave-8-list-3]] and moved its card to In Progress. Started 09:15:09 UTC; work for at least three hours (12:15:09 UTC) or until the list is exhausted. Rank cached resolved diffs, then inspect target asm alongside candidates and verify every retained match in the actual linked unit. Changes remain uncommitted.

## [2026-10-11] query | T-9530 rejected-draft ranking
Ranked 39 compilable remaining drafts by resolved differing-line count; 88 recovery candidates remain. The closest SHUGAKU function differs by two branch operand swaps. Started a five-minute permutation run and systematic experiments with live current-state locals, signedness, declaration order, return type, and structured/goto control flow while reading its original assembly. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 deep-pass SHUGAKU permutation gain
After ten minutes of typed selector/control-flow/call-view experiments, the 300-second permutation search found `func_80132BCC` with a comma expression in its first call argument. Retained it with a FAKE explanation. Actual resolved funcdiff matches and Docker ninja verifies the SHUGAKU binary SHA1. Gain: one function / 80 bytes. The first close SHUGAKU function still differs only by two branch operand swaps after 200 structured probes; further comma-expression probes follow this new evidence. ENDING typed scroll-loop trials and its five-minute permutation run continue. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 ENDING argument and TAIIKU aggregate gains
Retained ENDING `func_80139EC4` (120 bytes) with the existing unprototyped callee and a marked comma argument. Recovered `TaiRandomTable` (eleven ten-halfword rows) and retained TAIIKU `func_80135D24` (212 bytes) using an explicit column-call assignment. Actual funcdiff and each overlay SHA1 pass. Second-pass total: three functions / 412 bytes. Recorded the scroll-loop and TACO predicate/increment deep time boxes; missing DATE/GYOZI function-pointer tables and TT records are being recovered in scratch. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 callback table type recovery
GYOZI `func_801362A0` now matches with a 30-entry local callback table and one byte index before the table. Generated the two opaque main callback-address declarations with scoped `sync_protos --fix`; actual resolved diff and GYOZI SHA1 pass. Second-pass gain: four functions / 696 bytes. TACO's next near miss has a scratch permutation match undergoing layout minimization. Reconstructed the missing PsyQ 3.x POLY_G4 declaration once in include/libgpu.h for drawing-helper trials, as CODING_STANDARDS section 3 requires; discard it if no dependent match is retained. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 TACO score-load permutation gain
Minimized the permutation output for TACO `func_8013920C` to one shared call/load source line plus its scalar address view, both marked FAKE. Actual resolved funcdiff matches and the whole TACO binary SHA1 passes. Five second-pass matches / 1,088 bytes. TT record views, TAIIKU copied lookup tables, the DATE callback table, and SDK quadrilateral field forms remain in deep scratch iteration. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 copied lookup arrays match
TAIIKU `func_80138680` matches naturally after recovering both word-table copies, reusing the selector for its result, and declaring key before selector. Used record fields from their actual array bases; generated D_80122E94 in main_api.h. Actual resolved diff and TAIIKU SHA1 pass. Six second-pass functions / 1,472 bytes. Recorded sustained negative trials for the TT state and phantom-constant functions and ENDING's quadrilateral helper; failed C stays assembly. Regenerating and checking owned-file duplicate opportunities. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 direct callback elements and motion-word union
Retained EVENT `func_800FA6A0` (228 bytes) through direct table-element comparisons with a reused byte index, and TAIIKU `func_80136B18` (156 bytes) through the recovered word/high-halfword union. Both actual resolved comparisons and full unit SHA1 checks pass. Eight second-pass gains / 1,856 bytes. Removed the unused callback local and exploratory SDK declaration. Corrected the split postprocessing omission and reran failed verification sequentially after a Ninja deps-log race. Fresh scoped duplicate scans apply zero copies; their inventory double-counts stale asm artifacts after resplit, unlike funcdiff's filtering. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 sustained second-pass timeboxes

Recorded the DATE branch-order, TAIIKU table/loop/counter, TT constant-order and ENDING brightness experiments in [[tickets/T-9530-wave-8-list-3]]. Failed bodies remain assembly. The counter routine has shared `$at` bases but no object-owned data range. Further typed-record experiments reduce the EVENT brightness loop to four edits and TT state dispatcher to twelve; verification remains pending.

## [2026-10-11] build | T-9530 EVENT record pointer and chained colours

EVENT `func_800FA194` passes resolved funcdiff and the overlay SHA1 after using a local Rec24 base and chained colour stores. Its neighbour `func_800FA0D0` passes the resolved comparison with explicit before/after callback-index locals; checksum verification is in progress. Removed unused draft locals, keeping ordinary formatting and no FAKE. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | T-9530 explicit fifth switch case and pointer tables

The second EVENT loop's checksum passes. BUNKA_SD `func_80132DFC` matches its 500 bytes after the missing explicit case 4 is restored; both unit and linked original-assembly reference SHA1 agree, and linked funcdiff resolves the unsupported object PC16 relocation. SHOUGATU `func_8013315C` matches 172 bytes with the corrected pre-increment test, pointer-table indexing and s32/no-return idiom. Twelve deep-pass gains / 2,924 bytes, with failed bodies still assembly. [[tickets/T-9530-wave-8-list-3]], [[matching-notes]].

## [2026-10-11] build | Wave 8 list 3 deep-pass checkpoint

All 27 unit SHA1 checks pass after twelve second-pass gains. Owned-file duplicate and near-duplicate scans find no applicable copies. Recorded sustained calendar, brightness, actor and control-record experiments and their closest resolved differences in [[tickets/T-9530-wave-8-list-3]]. Failed drafts remain scratch-only; aggregate repair continues.

## [2026-10-11] build | T-9530 late record repairs and inline review

Recorded the EVENT initializer, TAIIKU motion/actor, BUNKA_SD offset, GYOZI girl-selector and main SDK-line experiments in [[tickets/T-9530-wave-8-list-3]]. Added two new rows and historical match resolutions to [[data/t0018-cases]]. TT's 0x54-byte state machine now matches in cleaned scratch C after direct timer pre-decrements and a bound initialized in the for header; real unit verification is pending. Inline review added guards to the new overlay type blocks and their missing offset comment; all 27 unit checks pass. [[matching-notes]].

## [2026-10-11] ticket | T-9530 second deep pass reviewed and complete

The second pass reached its three-hour minimum (09:15:09–12:15:09 UTC). Thirteen additional matches / 3,344 bytes are left uncommitted, bringing the batch to 22 / 7,384. TT's actual resolved comparison passes; final sequential Docker Ninja checks all 27 unit SHA1 values, headers and globals. Prototype branch and GameState checks pass. Inline CODING_STANDARDS review findings are resolved, failed drafts remain INCLUDE_ASM, and only two new T-0018 rows were added. Returned [[tickets/T-9530-wave-8-list-3]] and [[kanban]] to Done, updated [[index]] and appended the final result to [[matching-notes]]. Progress: `grand total   4756/6958   736976/2279368  32.3%`. Remaining data-ownership blocker and object PC16 resolver issue are documented in the ticket.

## [2026-10-11] query | T-9530 final owned duplicate scan

The final thirteen-gain exact-owned-file scan finds zero copy plans and applies zero changes. Verified header/global stamps, clean whitespace and empty staging; recorded the evidence in [[tickets/T-9530-wave-8-list-3]].

Final sequential `ninja progress` completed successfully (`deep-final-progress-sequential.log`) and confirms `grand total   4756/6958   736976/2279368  32.3%`.
## [2026-10-11] ticket | T-9570 wave 8 list 7 started

Created [[tickets/T-9570-wave-8-list-7]] in In Progress and synchronized [[kanban]]. Read [[decompile-workflow]], [[register-allocation-wave7]] and CODING_STANDARDS.md; work restricted to list-7 files under the shared wave-8 brief.

## [2026-10-11] build | T-9570 first bounded attempts and duplicate scan

The ownership-restricted duplicate pass applied zero copies. First list-7 drafts were built and rejected where they differed or required type repair; no match is claimed. Recorded the table-base, shared constant and m2c draft limitations in [[matching-notes]]. Scratch evidence is in scratchpad/w8-7/. The initial full build stopped on an experimental candidate; it will be rerun after candidates are settled.

## [2026-10-11] query | T-9570 source-shape follow-ups

Completed bounded first-pass attempts through list position 55. OPTION `func_8013A550` improves when the parameter is updated directly, but explicit return/goto variants and a 60-second permuter run do not match its epilogue; the best permuter score was 560, not a match. GYOZI `func_80142A8C` remains open after parameter, local-order and color-expression variants; all experimental C was restored. Draft type repairs now use the established D_8011ECD0 byte table for its undeclared field labels. Details in scratchpad/w8-7/ and [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 matched KANGEI:func_80134BC4

src/ovl/KANGEI/80134120.c: 492-byte function replaced with C and verified by relocation-resolved funcdiff; tracked in [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] query | T-9570 work-list first pass complete

All 129 entries have a bounded outcome: 128 records in scratchpad/w8-7/results-1.jsonl through results-7.jsonl, plus the separately investigated GYOZI func_80142A8C. KANGEI func_80134BC4 is the one first-pass C match (492 bytes); 36 entries require substantial manual type or copy repair and 91 compiled/failed drafts were reverted. TT func_8014BF54's first draft incorrectly kept its angle before a call; the original reloads it afterwards, so a corrected follow-up is being checked. See [[tickets/T-9570-wave-8-list-7]] and [[matching-notes]].

## [2026-10-11] build | T-9570 matched TT:func_8014BF54

src/ovl/TT/8014AF00.c: 348-byte C replacement verified by funcdiff; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 TT follow-up and register-shape inventory limit

TT func_8014BF54 (348 bytes) matches after restoring the angle load after the call, correcting unsigned halfword reads, preserving the first two loads' source order and reading the cleared halfword back into the counter. A further BUNKASAI bit-field rewrite fixes the flag store but leaves selector allocation; TT func_8014BB30's sign-shift test fixes its branch shape but its multiplication operands remain under investigation. `register_shapes.py` cannot complete its historical inventory: missing asm/ovl/VALEN/nonmatchings/VALEN/80132760/func_80133670.s. No tools changed. `sync_protos.py --check-branch` and `migrate_globals.py --check` pass. See [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] query | T-9570 register-order evidence filed

Appended four reverified rows to [[data/t0018-cases]] for the TAIIKU base rematerialization, GYOZI shared store/call value and SHOUGATU/BUNKASAI selectors. Read [[register-allocation-wave7]] first; the inventory command failed on an unrelated missing VALEN asm row, recorded in [[matching-notes]]. Rows describe observed differences, not proof that no C spelling can match. All four functions stay INCLUDE_ASM.

## [2026-10-11] ticket | T-9570 ready for inline review

Moved [[tickets/T-9570-wave-8-list-7]] and its [[kanban]] card to In Review. Full incremental Docker ninja completed successfully; kept functions are KANGEI func_80134BC4 and TT func_8014BF54 (840 bytes). Rejected candidates restored. Inline standards/spec review and final branch checks follow.

## [2026-10-11] ticket | T-9570 wave 8 list 7 done

Inline standards/spec review found no unresolved findings. Moved [[tickets/T-9570-wave-8-list-7]] and its [[kanban]] card to Done; updated [[index]]. Final gain: 2 functions, 840 bytes; four rows appended to [[data/t0018-cases]]. Full Docker ninja passed 27/27 SHA1 checks; header/global stamps exist, fresh branch/global checks pass, and both real function diffs pass after cleanup. Progress: 4736/6958 functions, 730452/2279368 bytes (32.0%). All 127 remaining list entries are time-boxed and stay assembly. No commits, merges, tool/config changes or raw-source edits.

## [2026-10-11] lint | T-9570 closing consistency check

Checked the new ticket, kanban card and index status, append-only log/row additions, changed C ownership and whitespace. Ticket/card agree on Done. Matching idioms and the register_shapes limitation are linked through [[matching-notes]]; scratch evidence remains outside the tracked tree.

## [2026-10-11] query | T-9570 complete ordered attempt audit

Re-read the assigned 167-line list: 129 functions, 35 file declarations, 3 comments. Matched all entries against prior batch and manual records, with no unattempted functions. Added the complete ordered evidence ledger to [[tickets/T-9570-wave-8-list-7]]; final retained C remains 2 functions, 840 bytes, uncommitted.

## [2026-10-11] build | T-9570 continuation finish checks

Incremental Docker ninja succeeds with no pending work; fresh branch-prototype and global checks pass, header/global stamps remain present. Progress remains 4736/6958 functions and 730452/2279368 bytes (32.0%). Ordered audit and finish evidence are recorded in [[tickets/T-9570-wave-8-list-7]] and [[matching-notes]]; ticket remains Done and changes uncommitted.

## [2026-10-11] ticket | Reopen T-9570 for deep second pass

Reopened [[tickets/T-9570-wave-8-list-7]] and moved its card to In Progress. The user requests sustained candidate iteration, five-minute near-miss permutation and aggregate-type recovery for at least three hours unless all candidates are exhausted. Prior first-pass outcomes were too shallow.

## [2026-10-11] build | T-9570 deep-pass match RPG_BAT:func_8013C32C

src/ovl/RPG_BAT/8013B5B0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match SHOUGATU:func_80137DE4

src/ovl/SHOUGATU/80137BB0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 selector match and aggregate retry

Recorded the newly verified SHOUGATU selector idiom in [[matching-notes]]. The 300-second search found a real-build match; cleanup trials establish that both marked FAKE operations are required. TAIIKU's D_8014A400 runner record is being reconstructed from its 0x1C stride and neighboring accesses, with the D_8014A608 three-word copy represented as a real aggregate.

## [2026-10-11] query | T-9570 sustained source and permutation experiments

Updated [[tickets/T-9570-wave-8-list-7]] with resolved-diff ranking, targeted control-flow/type/declaration/expression experiments and 300-second searches. TT reaches eight instruction-line differences; RPG_BAT's omitted call argument is repaired before searching. KANGEI's rectangle is represented by the SDK RECT. Unmatched candidates are restored. Aggregate recovery proceeds from TAIIKU's neighboring source and target assembly.

## [2026-10-11] build | T-9570 deep-pass match TAIIKU:func_80146994

src/ovl/TAIIKU/801452D0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match SHOUGATU:func_80137E54

src/ovl/SHOUGATU/80137BB0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match GYOZI:func_8013AC38

src/ovl/GYOZI/8013A920.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 byte-chain gains and serialized builds

Recorded two additional verified scene-initializer matches in [[matching-notes]] and updated [[index]] to seven functions and 2,364 bytes. Several simultaneous local experiments corrupted the disposable Ninja dependency log, producing empty-output failures and a Ninja crash. Preserved that log in external scratch and switched the scratch experiment runner to a build lock; failed runs will be repeated and do not count as negative code-generation evidence.

## [2026-10-11] decision | T-9570 reject conflicting shared declarations

The generated trial declarations for D_80120872 and D_80120888 conflict with EVENT's existing table views at overlapping overlay addresses. Removed those trial declarations without changing EVENT or shared configuration. Their raw signed-width views remain experimental; no conflicting declaration is retained as a match. This is recorded under [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match KANGEI:func_8013454C

src/ovl/KANGEI/80134120.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 recovered KANGEI local table layout

The four typed copies in src/ovl/KANGEI/80134120.c match with one preceding scalar, bringing the retained total to eight functions and 2,836 bytes. Moved recovered overlay types and externs into include/ovl/KANGEI.h; indexed member and selector-local cleanup verification follows. See [[matching-notes]] and [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match KANGEI:func_8013454C

src/ovl/KANGEI/80134120.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match TT:func_8014B088

src/ovl/TT/8014AF00.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 remove KANGEI padding trick

The real selector-before-table form and ordinary indexed members pass the function diff and KANGEI checksum. Recorded the clean retained form in [[matching-notes]]; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] query | T-9570 aggregate retry evidence

Recorded the current aggregate, signed-width, control-flow and declaration-order searches in [[tickets/T-9570-wave-8-list-7]]. Closest open drafts now include SHOUGATU initialization scheduling (two differing lines), TT retained constant conversion (two lines) and BUNKA_SD typed table loop (18 lines). Near misses receive 300-second searches; rejected code is restored.

## [2026-10-11] decision | T-9570 TT wide unsigned dimension local

The wide local removes the allocator-call conversion mismatch in src/ovl/TT/8014AF00.c. Updated [[matching-notes]] and [[index]] to nine verified functions and 3,108 bytes; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 recovered records and deeper retry queue

Nine functions and 3,108 bytes are retained with individual resolved diffs and unit SHA1 verification. RPG_BAT four-corner and reward/input-table copies, GYOZI six 0x44 records, DATE initializer globals and XA eight-byte records are now repaired for deeper comparisons. Invalid drafts with unmigrated aliases are corrected before counting negative results. SHOUGATU initializer reaches two differing lines, then completes a 300-second permutation search; a separate scene initializer has a resolved match pending a clean overlay SHA1. Experiments remain isolated by source file and serialized during builds; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match SHOUGATU:func_8013A688

src/ovl/SHOUGATU/8013A500.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 clean SHOUGATU scene verification

Recovered the provisional resolved scene match only after the rejected initializer was restored. Fresh SHOUGATU SHA1 passes; [[matching-notes]] and [[index]] now show ten functions and 3,404 bytes. Independent permutation searches now avoid shared Ninja state and their candidates are explicitly verified afterward; build experiments retain the scratch helper's file lock.

## [2026-10-11] build | T-9570 aggregate time-box audit

Restored RPG_BAT input-combination, weighted-reward and action drafts after sustained control-flow/type/declaration experiments; best resolved diffs remain 165, 113 and 42 lines respectively. TAIIKU's first camera family is restored after 36 variants (best 38); the next uses an actual 0x44 scene record and the timer family repairs m2c's incorrectly merged scalar stores with typed controller/action members. These are time-boxed negative results, not matches; [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match BUNKA_SD:func_80137620

src/ovl/BUNKA_SD/801369D0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 coordinate-base load order match

BUNKA_SD's repaired table function is now byte verified: the remaining twelve-line mismatch was fixed by loading the coordinate base in a separate statement before adding the index. Updated [[matching-notes]]; see [[tickets/T-9570-wave-8-list-7]]. The next closest recovered type is TAIIKU's scene record (four lines, one stack-slot and one scheduling difference).

## [2026-10-11] build | T-9570 second-pass type and scheduling work

The eleven retained functions total 3,840 bytes. Restored the large RPG_BAT render/dispatcher/table, KANGEI state, DATE2 counter and TT trail experiments after type/control-flow variants; negative results remain in the scratch ledger. Continued DATE's fourteen-line initializer with signed flag views and unsigned byte-store promotion, and repaired the main tokimeki table's four-byte record view and TACO's missing forward declaration before retrying. Remaining main matrix copies now include their translation words rather than m2c's disconnected scalar locals; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] ingest | T-9570 remaining aggregate reconstruction

Read the TAIIKU initializer's target stores: m2c collapsed distinct byte/halfword/word members into repeated assignments to the first symbol. Reconstructed their widths and offsets in 0x1C controller/runner/action records, plus the 0x44 scene views and 8/16/20/36/48-byte local table copies used by five remaining aggregate drafts. All are experimental until resolved comparison and checksum verification; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match TAIIKU:func_801485C4

src/ovl/TAIIKU/801452D0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 typed camera match

TAIIKU func_801485C4 passed resolved funcdiff and unit SHA1 (180 bytes), increasing retained totals to twelve functions and 4,020 bytes. Declaration and pointer initialization order solved both remaining differences. Added the record definition to include/ovl/TAIIKU.h and recorded the idiom in [[matching-notes]]; [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 strict movie comparison and aggregate retries

The movie loop's low absolute D_7FFFFF relocations are unresolved by funcdiff's normal address resolver. Built a scratch expected object directly from the immutable original executable bytes, preserving section contents and its original address, to compare with --resolve --expected; this now reports only the true sp+0x30 versus sp+0x34 spill difference. No tool or generated asm changes. Continued valid retries for the remaining main/TAIIKU/TACO/GYOZI aggregate families; failed forward declarations and byte-array scalar uses were repaired rather than recorded as codegen failures. See [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] decision | T-9570 scheduling time-box and cleanup

Added a fifth append-only T-0018 case for SHOUGATU func_801326F8 after four source-iteration families and multiple five-minute permutation searches (two remaining lines). Its corrected flag and compare types are documented in the scratch ledger. Removed two unused m2c locals and their dead index multiplication from the verified BUNKA_SD table match, pending fresh resolved diff and SHA1. Six of the 36 aggregate first-pass rejects still await their queued valid comparisons; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 movie loop provisional match

Reversing the timeout/VLC declaration order moves the spill to sp+0x34 and strNextVlc now matches the strict original-byte expected object with --resolve. The main SHA1 attempt failed while other main drafts were active, so the helper restored the provisional result. It will be recovered and counted only after a clean whole-main verification. [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 deep-pass match DATE:func_80144720

src/ovl/DATE/80141EE0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 DATE initializer match

The DATE initializer's signed flags, unsigned girl-store view and scene byte member pass resolved funcdiff and the unit SHA1. Moved its new scene type and overlay-local byte declaration to include/ovl/DATE.h. Thirteen retained functions total 4,456 bytes; [[matching-notes]], [[index]], [[tickets/T-9570-wave-8-list-7]]. BUNKA_SD's removal of unused m2c locals also passed fresh resolved diff and SHA1.

## [2026-10-11] build | T-9570 deep-pass match main:strNextVlc

src/main/800563F0.c: real funcdiff and unit SHA1 passed; see [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 clean main VLC match and declaration cleanup

The recovered strNextVlc match passed strict resolved original-byte comparison and the clean main SHA1; fourteen functions total 4,628 bytes. Moved its prefix type to include/game.h and documented the declaration-order idiom in [[matching-notes]]. Restored experimental main_api.h additions and used sync_protos --add-global to retain only D_80122CE8, needed by the verified SHOUGATU scene. DATE's new type now documents all offsets. [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 final aggregate forward declarations

All main and TACO aggregate repairs now have valid differing-instruction comparisons. TAIIKU's two remaining table/scene drafts needed forward declarations for later void helpers, and its timer needs a void declaration before the earlier caller; these are retried separately after the experimental C is restored. Only the timer's valid comparison is still pending. The ordinary build retains only fourteen verified matches and the single generated D_80122CE8 addition. [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] build | T-9570 complete deep-pass coverage and final verification

All 36 aggregate/local-copy rejects have valid resolved comparisons after reconstruction; no such row is left at a missing-type compile error. The final timer family improved from 142 to 56 lines but remains INCLUDE_ASM. Restored all unsuccessful C and moved the retained TAIIKU scene type to its overlay header. Started the scoped duplicate scan, final all-unit Ninja, prototype branch guard and migration guard. Deep experiments ran from 09:07:34 until after 12:16 UTC; [[tickets/T-9570-wave-8-list-7]].

## [2026-10-11] ticket | T-9570 deep second pass complete

Inline CODING_STANDARDS review is complete with no unresolved findings. Fourteen retained functions (4,628 bytes; twelve/3,788 beyond the original pair) pass fresh resolved comparisons and all 27 unit SHA1 checks. The movie timeout uses a strict original-byte expected object to bypass the reported low-constant resolver bug. Headers/globals pass; prototype branch check has zero new and seventeen known disagreements; scoped dupes applied zero copies; final incremental Ninja and git diff --check pass. All 129 list functions have attempts, 62 have deep valid comparisons, and all 36 aggregate rejects received reconstructed types and valid retries during more than three hours of deep work. Five T-0018 rows added. Updated [[tickets/T-9570-wave-8-list-7]], [[kanban]] and [[index]] to Done. Progress: grand total 4748/6958, 734240/2279368 bytes, 32.2%. All changes left uncommitted.
## [2026-10-11] ticket | Start wave-8 list 4

Created [[tickets/T-9540-wave-8-list-4]] In Progress and synchronized [[kanban]] and [[index]]. Read [[decompile-workflow]], [[register-allocation-wave7]], CODING_STANDARDS.md and the shared wave-8 brief. Started Docker configure/build to generate the initially absent assembly.
## [2026-10-11] build | Wave-8 list-4 baseline and first bounded attempt

Docker configure/ninja completed with all 27 unit SHA1 checks OK and headers/globals OK. Started ordered drafts for [[tickets/T-9540-wave-8-list-4]]. Repaired TT func_80141A1C's m2c byte-buffer and pointer expressions; resolved diff still shows real mask/register/frame differences, so restored INCLUDE_ASM. Evidence recorded in [[matching-notes]] and the w8-4 scratchpad.
## [2026-10-11] decision | Wave-8 list-4 ABI and near-match limits

Prepared all 176 assigned m2c drafts and bounded repaired-C comparisons. Three 40-second permuter attempts (SHUGAKU func_80138DD8, TT func_801475C8, main func_80085368) found no verified score-zero candidate. Restored all sprite_brightness declaration/callee/call-site experiments: a matching caller alone did not preserve the existing callee or direct-call encoding. Documented evidence and the register_shapes.py positional-argument discrepancy in [[matching-notes]] for [[tickets/T-9540-wave-8-list-4]].
## [2026-10-11] decision | Complete the wave-8 list-4 time boxes

All 176 functions in [[tickets/T-9540-wave-8-list-4]] reached a bounded draft/repair/comparison attempt. Additional source-idiom retries for five early candidates and the three permuter runs found no retained match. Restored every experimental C/header edit. Appended three measured register/constant recheck rows to [[data/t0018-cases]] and recorded limits in [[matching-notes]]. Started the exact-owned-files duplicate check before final verification.
## [2026-10-11] ticket | Wave-8 list-4 inline review

Owned-files dupes --apply --check reports zero plans/copies. Moved [[tickets/T-9540-wave-8-list-4]] to In Review and synchronized [[kanban]]/[[index]]. Source, headers, configs and tooling are restored; only wiki records remain. Inline review and final Docker branch/global/build verification are in progress.
## [2026-10-11] query | InitMouse ownership follow-up

Read the small main InitMouse result: the original shares one $at base for D_800E36C0/D_800E36C4, as recorded in [[data/shared-at-groups]], while config/objects/main.txt supplies no data range for its 80059710 object. The m2c draft also collapses the two stores onto element 0. Recorded this concrete non-T-0018 blocker in [[matching-notes]] for [[tickets/T-9540-wave-8-list-4]]; no config or source changes made.
## [2026-10-11] build | Wave-8 list-4 final proof and review complete

Final Docker ninja passed 27/27 SHA1 checks; headers/globals stamps exist. sync_protos.py --check-branch reports zero new disagreements (17 known), migrate_globals.py --check passes, and git diff --check is clean. Grand total remains 4734/6958 functions and 729612/2279368 bytes (32.0%): 0 new functions/0 bytes. Inline standards/spec review found no unresolved findings. Completed all 176 time boxes, added 3 measured rows in [[data/t0018-cases]], recorded limits and tooling observations in [[matching-notes]], and moved [[tickets/T-9540-wave-8-list-4]]/[[kanban]] to Done with [[index]] synchronized. Source/headers/configs/tools are unchanged; wiki edits left uncommitted for the orchestrator.

## [2026-10-11] ticket | Reopen wave-8 list 4 for a deep second pass

User review found the initial draft comparisons too shallow. Reopened [[tickets/T-9540-wave-8-list-4]] and synchronized [[kanban]] and [[index]]. Deep pass starts at 09:08 UTC; rank resolved differences, iterate source/type/control-flow changes, and run five-minute near-miss searches. Minimum three hours unless the assigned list is exhausted.

## [2026-10-11] query | Rank rejected wave-8 list-4 drafts

[[tickets/T-9540-wave-8-list-4]] deep pass ranked successful draft builds by resolved differing lines; smallest cases and neighbor-type evidence recorded in [[matching-notes]]. Sustained SHUGAKU comparison experiments are running, with a 300-second permutation search next. Scratch evidence is in scratchpad/w8-4/deep-rank.jsonl and deep-shugaku.log.

## [2026-10-11] build | Match SHUGAKU final predicate byte

[[tickets/T-9540-wave-8-list-4]] gained func_80138DD8 (104 bytes) with a u8 local for the final compare. funcdiff --resolve MATCH and SHUGAKU unit SHA1 OK. Source and idiom recorded in [[matching-notes]]; testing multiline formatting before retaining the final form.

## [2026-10-11] build | Measure return scheduling and shared-base near misses

[[tickets/T-9540-wave-8-list-4]]: normal_date_girl_in_init is four resolved lines away after return/store repairs; its five-minute search found no improvement and source/prototype were restored. Corrected InitMouse is one extra lui away. SHUGAKU gain survives multiline formatting. Measurements and pending follow-ups recorded in [[matching-notes]].

## [2026-10-11] build | Match EVENT copied function table

[[tickets/T-9540-wave-8-list-4]] gained func_80102118 (172 bytes): existing FnTbl39 assignment and scalar-before-table declaration order. Resolved diff MATCH and EVENT SHA1 OK. [[matching-notes]] records the type/context result. Deep pass now retains 2 functions, 276 bytes.

## [2026-10-11] build | Match OPTION counted sprite loop

[[tickets/T-9540-wave-8-list-4]] gained func_8013357C (108 bytes). The s32 return spelling and counted loop preserve the target's dead initial counter load. Resolved diff MATCH and OPTION SHA1 OK; [[matching-notes]] records the return-value limitation and the failed sustained normal-date/ETC follow-ups. Running gains: 3 functions, 384 bytes.

## [2026-10-11] query | Recover aggregate layouts and time-box TT initializer

[[tickets/T-9540-wave-8-list-4]]: TT func_801475C8 finished 708 seconds of 56 initialization/type/loop variants and a 300-second search; best still eight resolved lines, restored. Aggregate repair now uses neighboring matched judging buffers, Rec24/TaiikuBig elements, copied function-table extents, SDK RECT and a five-halfword CD-level record. Scratch repair scripts retain experiments only after resolved comparison and unit SHA1; [[matching-notes]] will record verified gains and bounded failures.

## [2026-10-11] decision | Verify promoted-index brightness alias

[[tickets/T-9540-wave-8-list-4]]: func_80085368 resolves exactly with a scoped promoted-index declaration for the existing func_80084C98/sprite_brightness alias. Added its main-exe alias address to config/symbol_addrs.txt after the initial unit link reported an undefined reference. Main executable SHA1 then passed. The sequential trial will restore this first body on its initial failed check, so it must be reapplied and reverified before counting it. No gain counted yet; follow-up compares the second caller's operand order.

## [2026-10-11] build | Match second brightness caller

[[tickets/T-9540-wave-8-list-4]] gained main func_800853FC (156 bytes). The promoted-index alias view removes redundant call-site narrowing; a post-call u8 selector local fixes compare operands. Real resolved diff and main executable SHA1 pass. [[matching-notes]] records the scoped-view artifact; retained gains total 4 functions, 540 bytes.

## [2026-10-11] build | Retain first brightness caller and check five gains

[[tickets/T-9540-wave-8-list-4]] gained func_80085368 (148 bytes), reapplied after alias-link repair and reverified by resolved diff plus main SHA1. Total retained gains: 5 functions, 688 bytes. Started full incremental ninja for this five-match batch; [[matching-notes]] records the result and pending owned duplicate scan.

## [2026-10-11] query | Rank repaired aggregate drafts without disturbing full build

[[tickets/T-9540-wave-8-list-4]]: isolated Docker compilation compares the repaired aggregate drafts while full ninja checks the retained batch. The OPTION flag counter is ten lines away (equivalent base/displacement spelling); ETC scene links 24 lines; TACO aggregate arguments 38 lines. Several compile failures expose missing scalar backing declarations or late overlay overrides, being repaired before judging code generation. Added explicit field/array repairs for all 33 drafts containing opaque/local-copy markers; other type-error drafts remain separate. Scratch evidence is aggregate-isolated.log and aggregate-scores.txt. Isolated matches will still require real source resolved diff and unit SHA1.

## [2026-10-11] build | Verify five retained gains across all units

[[tickets/T-9540-wave-8-list-4]] five-gain incremental ninja completed successfully: 26 overlay SHA1 checks OK plus the already-current main executable SHA1 OK (27 units total). Header/global checks passed. Running exact-owned-files dupes and real-source verification of the isolated s16 loop match. New near-miss type/control experiments remain scratch-only until verification.

## [2026-10-11] build | Match signed halfword array-clearing loop

[[tickets/T-9540-wave-8-list-4]] gained main func_8008585C (140 bytes), correcting m2c's double-scaled table access and retaining the original s16 counter. Resolved MATCH and main SHA1 OK. [[matching-notes]] records the pending final array-view cleanup. Running gains: 6 functions, 828 bytes. The first duplicate invocation used multiple arguments where the CLI requires one comma-separated --files value; no edits occurred, corrected scan remains pending.

## [2026-10-11] query | Narrow judging and EVENT table frame gaps

[[tickets/T-9540-wave-8-list-4]] repaired aggregate retry ranks geko_judge at ten resolved lines and EVENT func_800FD00C at twelve. The judging buffers are 16 bytes too high, with a counter increment scheduled across one branch; EVENT's table is four bytes too low and one byte load needs a v1 base. Testing scalar/buffer/table declaration order and real local value types. Corrected EVENT func_800FEAE8's copied object: 41 unsigned-halfword bound pairs (0xA4), not a function table, plus an SDK RECT passed by value. Scratch failures from wrong wrapper/include order or incomplete types are not counted as negative code-generation evidence.

## [2026-10-11] build | Match SHUGAKU copied dispatch table

[[tickets/T-9540-wave-8-list-4]] gained func_80139D40 (332 bytes). Added a guarded overlay-local 68-pointer aggregate, byte index/kind locals, and one indirect-call argument. Resolved diff MATCH and SHUGAKU SHA1 OK; [[matching-notes]] records the ABI and frame findings. Running gains: 7 functions, 1160 bytes. OPTION flag-counter first sustained trial finished 465 seconds including 300-second permutation with no gain, restored; a corrected root-base follow-up is running.

## [2026-10-11] decision | Correct the frame-variant generator before counting its evidence

[[tickets/T-9540-wave-8-list-4]]: the first judging frame matrix used a declaration block in the wrong order, so its buffer-length/order replacements did not apply. Only its counter-placement variations are valid evidence. Corrected the generator to replace the actual declaration block and started isolated comparisons of the proper variants. All unsuccessful real-source trials remain temporary and are restored automatically. Continued aggregate repairs include a distinct OPTION scene descriptor (RGB bytes at +0x10, pointer at +0x14), missing backing-table declarations in isolated context, and removal of m2c's mistaken scalar dereferences.

## [2026-10-11] build | Bounded callback and judging retries

[[tickets/T-9540-wave-8-list-4]]: EVENT func_800FD00C's 24 actual declaration/type variants reached eight resolved lines; its 300-second permutation ended at raw score 10 and was restored. Inspecting that candidate with resolved comparison rather than assuming a positive raw score fails. Started the corrected judging buffer/declaration matrix and a scoped aggregate flag-block view. Larger aggregate drafts have been repaired with copied-table, coordinate-array, scene-record and SDK RECT types; failed type-only trials are kept in scratch, not retained source.

## [2026-10-11] build | Match OPTION counted flags with aggregate view

[[tickets/T-9540-wave-8-list-4]] gained func_80136F3C, 260 bytes. Recovered word-at-zero and fourteen flags at +0x14 as FlagsWork24; a scoped generated-header override preserves other callers' word view. The original peeled-loop address and field displacement now match. Resolved diff MATCH and OPTION SHA1 OK. Running gains: 8 functions, 1420 bytes; [[matching-notes]] records the type and bounded failed byte-pointer experiments.

## [2026-10-11] query | Extend recovered aggregate retries

[[tickets/T-9540-wave-8-list-4]]: corrected the final OPTION four-record byte-flag cursor to Rec44 (func_8013493C now compiles, 152 resolved differing lines). MouseState now has an inferred four-byte output aggregate, tested against the original signed packet loads and neighboring MouseOut. The halfword queue uses a count plus 32 entries, inferred from matched enqueue/reset functions; testing the saved first-entry load independently from the shift loop. EVENT func_801022C0's copied 53-pointer table compiles and member-based Rec24 writes reduce the difference to 36 lines; further declaration/expression iteration remains.

## [2026-10-11] query | Packet layouts from matched TT neighbors

[[tickets/T-9540-wave-8-list-4]]: matched TT func_8013EF38 shows the +8 buffer selector and 0x80B8 buffer stride, with packet cursor at +4. Defined scratch render-context/buffer and 0x30-byte packet views for func_80141A1C and func_80141EBC; both now compile with the signed-coordinate and unsigned-clut fields recovered from loads. Tag-member assignment improves the first comparison to 188/189 lines, still far behind the close candidates. Queue count/32-entry and MouseState four-byte output variants also compile; their failed trial types remain scratch-only.

## [2026-10-11] decision | Retained-source review cleanup

[[tickets/T-9540-wave-8-list-4]]: removed duplicate include pairs, marked OPTION's s32/no-return allocation artifact, and replaced the main clearing function's unmarked first-symbol expression with an explicitly scoped minimum word-array view. func_8008585C still reports resolved MATCH. The exact-owned-files duplicate plan has zero copies (six other candidates are skipped outside the accepted scope/types). Failed judging and callback variants were restored; the next sustained actual trial covers repaired ETC scene links, the nibble comparison, the main selector and MouseState.

## [2026-10-11] decision | Record sustained near-miss limits

[[tickets/T-9540-wave-8-list-4]] added four measured deep-pass rows to [[data/t0018-cases]] for normal_date_girl_in_init, TT func_801475C8, ETC func_801451D4 and geko_judge. Rows distinguish compiling variants from rejected prototypes and bounded experiments from impossibility claims, following [[register-allocation-wave7]]. The two first-pass rows now matched are explicitly resolved without deleting history. Positive raw permutation candidates are being rechecked with resolved comparisons; zero raw score is not used as the only gate.

## [2026-10-11] query | Positive permutation score hides a resolved candidate

[[tickets/T-9540-wave-8-list-4]]: normal_date_girl_in_init's raw-score-20 output compares at zero resolved lines in isolated compilation. It uses an explicitly FAKE alias of a local zero and s32/no-return spelling; replaced bare int locals with s32 and rechecking before actual full-source/unit verification. It is not yet counted as a retained gain. The current SHOUGATU func_8013AE0C trial reaches four resolved lines with a dead raw-value test, improving its earlier twenty-line candidate; its mandatory five-minute permutation is running.

## [2026-10-11] query | Stage two zero-difference candidates

[[tickets/T-9540-wave-8-list-4]]: fixed-width local-zero alias variant of normal_date_girl_in_init still compares at zero in isolated compilation; its void spelling lacks one instruction, so the staged candidate retains the explicitly FAKE s32/no-return view. SHOUGATU func_8013AE0C also reaches zero by reversing the compared operands after the dead raw-value test. Neither is counted until real funcdiff and unit SHA1 verification. The sustained main selector trial completed with fourteen resolved lines and 300-second permutation, restored; MouseState's actual typed trials continue.

## [2026-10-11] decision | Complete the minimum deep-pass duration

[[tickets/T-9540-wave-8-list-4]] passed the three-hour minimum at 12:07:54 UTC. MouseState completed 24 actual typed/flow variants and 300-second permutation in 590 seconds; best nineteen resolved differing lines, all experimental C/API types restored. More than 36 aggregate/local-copy drafts were retried with recovered table, queue, packet and field views; far candidates remain scratch-only. Two staged zero-difference candidates are now in real-unit verification. OPTION's already-matched record loop also compares at zero using documented struct members; checking that cleanup before retaining it.

## [2026-10-11] build | Verify two late gains and record-member cleanup

[[tickets/T-9540-wave-8-list-4]] gained normal_date_girl_in_init (112 bytes) and SHOUGATU func_8013AE0C (96 bytes), each real resolved MATCH plus unit SHA1 OK. Gains now total 10 functions, 1628 bytes. OPTION's previously matched 108-byte draw loop also passes with documented OptDraw44 members. [[matching-notes]] records the source idioms; [[data/t0018-cases]] preserves historical rows and adds resolution notes plus the bounded main selector case. Header branch check reports zero new disagreements and seventeen known; GameState check passes.

## [2026-10-11] query | Resolve positive-score rechecks

[[tickets/T-9540-wave-8-list-4]] rechecked positive raw-score outputs for the main selector, MouseState, geko_judge, EVENT callback and ETC scene-link function after fixed-width normalization. Only the already-kept initializer compares at zero. EVENT func_800FD00C's best output was additionally tested in the real source: four resolved lines remain in the u8 load's lui register, and the entire draft/header view was restored. Added its bounded narrow-global row to [[data/t0018-cases]], bringing this ticket's historical rows added to nine.

## [2026-10-11] ticket | Review the deep-pass result

Moved [[tickets/T-9540-wave-8-list-4]] to In Review; [[kanban]] and [[index]] agree. Inline standards review covers ten retained definitions, documented type layouts and explicit matching artifacts. Remove one duplicated FAKE comment, then rerun resolved comparisons, exact-owned duplicate check and all 27 unit SHA1 checks before Done.

## [2026-10-11] build | Full deep-pass build is byte-identical

[[tickets/T-9540-wave-8-list-4]]: final incremental Docker ninja completed 523 steps with all 27 unit SHA1 checks OK. Removed the duplicated OPTION comment before this build. Exact-owned duplicate application produced zero plans/copies, and git diff --check is clean. Rechecking all ten retained functions with fresh resolved comparisons, branch/global guards and progress before closing the ticket.

## [2026-10-11] ticket | Complete the reviewed deep pass

[[tickets/T-9540-wave-8-list-4]] is Done after inline CODING_STANDARDS.md review with no unresolved findings; [[kanban]] and [[index]] agree. Ten functions/1628 bytes gained, every retained function freshly resolved MATCH, full 27/27 SHA1 OK, headers/globals stamps present, zero new branch disagreements (seventeen known), GameState check OK, exact-owned dupes zero copies. Corrected the new EVENT row's file path during review. [[matching-notes]] records final progress: 4744/6958 functions, 731240/2279368 bytes (32.1%). The deep pass ran from 09:07:54 to final proof at 12:27:26 UTC. Nine rows added to [[data/t0018-cases]], four subsequently resolved; all failed drafts/types restored. Changes remain uncommitted.
## [2026-10-11] ticket | Wave 8 list 8 claimed

Created [[tickets/T-9580-wave-8-list-8]] In Progress and synchronized [[kanban]] and [[index]]. Read the shared wave-8 brief, [[decompile-workflow]], [[register-allocation-wave7]], and coding standards; matching is restricted to the 36 owned C files.

## [2026-10-11] build | List 8 func_80149F48 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80149F48 (176 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80134F2C time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80134F2C (280 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 baseline and typed EN_NICHI retry

Initial Docker ninja passed 27/27 unit SHA1 checks and both guards. [[tickets/T-9580-wave-8-list-8]]: typed EN_NICHI `func_80134F2C` retry reproduced all instructions except the frame size (0x38 versus 0x30); reverted after bounded permutation. K&R permutation emitted `KeyError: arg1`.

## [2026-10-11] build | List 8 func_8013923C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013923C (100 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80132A74 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80132A74 (104 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80135A48 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80135A48 (132 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80133AD0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133AD0 (108 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80146AC0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_80146AC0 (128 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80140B20 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80140B20 (132 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80134ADC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134ADC (224 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80134874 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134874 (176 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 _sys_default_tpage_set time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/_sys_default_tpage_set (300 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 load_tpage_buf_lock time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/load_tpage_buf_lock (168 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80136A2C time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80136A2C (228 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801551D0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_801551D0 (92 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80149268 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80149268 (168 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80134D0C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134D0C (276 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80136124 time-boxed

[[tickets/T-9580-wave-8-list-8]]: GYOZI/func_80136124 (132 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801464D4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_801464D4 (292 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801453B4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_801453B4 (316 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013AF78 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013AF78 (296 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8015435C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TACO/func_8015435C (224 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80154230 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TACO/func_80154230 (236 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | DATE counter matched in list 8

[[tickets/T-9580-wave-8-list-8]]: `func_801551D0` (92 bytes) passes `funcdiff --resolve` with a local `GameState *`, implicit-int-style fall-through, and explicit zero return on the early path. Main `load_tpage_buf_lock` was reverted after its bounded retry.

## [2026-10-11] build | List 8 func_8013A610 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013A610 (256 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013A710 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013A710 (256 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801546FC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TACO/func_801546FC (292 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80138B90 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80138B90 (384 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801354E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_801354E0 (140 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 search_load_tpage_buf_lock time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/search_load_tpage_buf_lock (232 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80154820 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TACO/func_80154820 (320 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_801511B4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_801511B4 (332 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013DAC4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013DAC4 (268 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80134768 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134768 (268 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80047560 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80047560 (352 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80155004 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80155004 (368 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801349B0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801349B0 (300 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_801398EC time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_801398EC (492 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80048DD0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80048DD0 (168 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_801330D0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_801330D0 (736 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80133C1C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133C1C (248 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80135480 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHUGAKU/func_80135480 (340 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80132A74 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80132A74 (104 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80133AD0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133AD0 (108 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] ingest | List 8 matching observations

Appended the local-GameState counter idiom, EN_NICHI frame near miss, and K&R permuter failure to [[matching-notes]]. Attempt outcomes are recorded in [[tickets/T-9580-wave-8-list-8]].

## [2026-10-11] build | List 8 func_80134ADC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134ADC (224 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80134874 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134874 (176 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 load_tpage_buf_lock time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/load_tpage_buf_lock (168 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80149268 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80149268 (168 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80134D0C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134D0C (276 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80138B90 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80138B90 (384 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | First list 8 checkpoint

Docker incremental ninja completed all 27 unit SHA1 checks, including the retained DATE `func_801551D0`; header/global guards pass. [[tickets/T-9580-wave-8-list-8]] has covered the first 40 list entries, with selected type/table retries still running.

## [2026-10-11] build | List 8 func_801511B4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_801511B4 (332 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80155004 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80155004 (368 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801349B0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801349B0 (300 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80048DD0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80048DD0 (168 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80133C1C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133C1C (248 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80140D6C time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80140D6C (172 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80146400 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_80146400 (212 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80132000 matched

[[tickets/T-9580-wave-8-list-8]]: ENDING/func_80132000 (820 bytes), matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

## [2026-10-11] build | List 8 func_80042CB0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80042CB0 (864 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80134630 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134630 (312 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80042AC8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80042AC8 (268 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801337E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_801337E0 (268 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80145550 time-boxed

[[tickets/T-9580-wave-8-list-8]]: BUNKAKEN/func_80145550 (756 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80134E20 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134E20 (808 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_8013F0DC time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013F0DC (300 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013FB8C time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013FB8C (304 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013275C time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_8013275C (216 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801374C0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801374C0 (404 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80140AD8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU/func_80140AD8 (256 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801560E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_801560E0 (712 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80134BBC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134BBC (336 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013A2F8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU/func_8013A2F8 (260 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80134178 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134178 (340 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_800422C8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_800422C8 (268 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 search_tpage time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/search_tpage (616 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 search_tpage8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/search_tpage8 (616 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80057710 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80057710 (484 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80133D14 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133D14 (284 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80140314 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU/func_80140314 (508 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013D6C0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013D6C0 (308 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80046754 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80046754 (884 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80133A54 time-boxed

[[tickets/T-9580-wave-8-list-8]]: KANGEI/func_80133A54 (444 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013B4A0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013B4A0 (552 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013B3C0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: GEKO/func_8013B3C0 (336 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013A9C8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013A9C8 (732 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013BE18 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013BE18 (348 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013F454 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013F454 (348 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013D4CC time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013D4CC (352 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013B1C4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013B1C4 (600 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013E430 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013E430 (364 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80135630 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHUGAKU/func_80135630 (380 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80133970 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133970 (240 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013CEC8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013CEC8 (396 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801425C0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_801425C0 (704 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80137654 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80137654 (548 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | DATE dispatcher time-boxed

[[tickets/T-9580-wave-8-list-8]]: `func_801511B4` was within one address-register difference with the existing FnTbl40 type. Literal-byte and local-pointer retries did not match; restored its INCLUDE_ASM.

## [2026-10-11] build | List 8 func_80135ACC time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80135ACC (1160 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80134D74 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80134D74 (440 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801342CC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801342CC (420 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80135274 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80135274 (620 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80132834 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80132834 (472 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013BB28 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013BB28 (488 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801447E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_801447E0 (924 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80137878 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80137878 (512 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80150C78 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80150C78 (548 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801357AC time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHUGAKU/func_801357AC (396 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013B448 time-boxed

[[tickets/T-9580-wave-8-list-8]]: MASTER/func_8013B448 (956 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80135600 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80135600 (976 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013B6C8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013B6C8 (608 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | ENDING scene table initializer matched

[[tickets/T-9580-wave-8-list-8]]: `func_80132000` (820 bytes) passes resolved function comparison and ENDING unit SHA1. Added only overlay-local declarations in include/ovl/ENDING.h. Two retained matches total 912 bytes so far.

## [2026-10-11] build | List 8 func_80134800 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80134800 (1052 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80140574 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80140574 (280 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80132670 time-boxed

[[tickets/T-9580-wave-8-list-8]]: OMIMAI/func_80132670 (280 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80132EE8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80132EE8 (488 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80137A78 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80137A78 (424 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013B928 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013B928 (424 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80139740 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80139740 (428 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013B994 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013B994 (380 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80132B04 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ENDING/func_80132B04 (660 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801344F0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_801344F0 (660 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8004F6B0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_8004F6B0 (448 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80134470 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134470 (448 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80133FB0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133FB0 (456 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80142880 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80142880 (316 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80140430 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_80140430 (324 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801360B0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHUGAKU/func_801360B0 (720 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013ACA4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013ACA4 (724 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013FE40 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013FE40 (344 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80135EA4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHUGAKU/func_80135EA4 (524 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013F984 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013F984 (388 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013AE3C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013AE3C (676 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_8013A3E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013A3E0 (560 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801392A0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801392A0 (1060 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013ED84 matched

[[tickets/T-9580-wave-8-list-8]]: GYOZI/func_8013ED84 (3700 bytes), matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

## [2026-10-11] build | List 8 func_80134258 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80134258 (664 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80138808 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80138808 (904 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_801467C4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_801467C4 (312 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013358C time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_8013358C (596 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801335F0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: KANGEI/func_801335F0 (324 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | Large GYOZI initializer matched

[[tickets/T-9580-wave-8-list-8]]: `func_8013ED84` (3,700 bytes) passes resolved function comparison and GYOZI unit SHA1; declarations added only to include/ovl/GYOZI.h. Three retained functions total 4,612 bytes. DATE dispatcher assignment-chain retry reverted.

## [2026-10-11] build | List 8 func_8015443C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TACO/func_8015443C (704 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013DD94 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013DD94 (500 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_800F6E84 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EVENT/func_800F6E84 (376 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80148BC8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_80148BC8 (720 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80133E30 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133E30 (384 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80150460 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_80150460 (340 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_8013C9F0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013C9F0 (340 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80135FBC time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80135FBC (744 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013B77C time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013B77C (536 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801339D4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_801339D4 (724 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80137C20 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80137C20 (1368 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80138E28 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80138E28 (1044 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013F758 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013F758 (556 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801002C8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EVENT/func_801002C8 (384 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80148F7C time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_80148F7C (836 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80042134 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80042134 (404 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80144C84 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_80144C84 (1276 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80133CA8 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80133CA8 (600 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80148714 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_80148714 (1012 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013BB24 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013BB24 (756 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80139770 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80139770 (1064 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013D2AC time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013D2AC (544 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013FB28 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU/func_8013FB28 (1152 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80138300 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80138300 (1288 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_8013F494 time-boxed

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU/func_8013F494 (1328 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013BF74 time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013BF74 (1332 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_801484E0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_801484E0 (372 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80133990 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RENSYU/func_80133990 (808 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8014231C time-boxed

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8014231C (580 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80132450 time-boxed

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80132450 (780 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_80133628 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RENSYU/func_80133628 (872 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80079070 time-boxed

[[tickets/T-9580-wave-8-list-8]]: main/func_80079070 (1204 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8013CCD0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TEL/func_8013CCD0 (876 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80148B08 time-boxed

[[tickets/T-9580-wave-8-list-8]]: RPG_BAT/func_80148B08 (1128 bytes), time-boxed. Missing draft declarations need manual recovery.

## [2026-10-11] build | List 8 func_801496C4 time-boxed

[[tickets/T-9580-wave-8-list-8]]: DATE/func_801496C4 (1020 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_8014790C time-boxed

[[tickets/T-9580-wave-8-list-8]]: ETC/func_8014790C (1128 bytes), time-boxed. Draft requires unresolved types/aggregate mapping.

## [2026-10-11] build | List 8 func_80134D74 matched

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI/func_80134D74 (440 bytes), matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

## [2026-10-11] build | List 8 func_8013D6C0 matched

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT/func_8013D6C0 (308 bytes), matched. funcdiff --resolve MATCH; unit ninja SHA1 check passes.

## [2026-10-11] build | Final two list 8 source idioms matched

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT `func_8013D6C0` (308 bytes, implicit-int switch copy) and EN_NICHI `func_80134D74` (440 bytes, direct record clamp) pass resolved comparison and unit SHA1. Five retained functions total 5,360 bytes; idioms added to [[matching-notes]].

## [2026-10-11] ingest | Six list 8 register cases recorded

Appended six verified draft failures to [[data/t0018-cases]] after consulting [[register-allocation-wave7]] and the relevant source idioms. Type-only draft failures are excluded; [[tickets/T-9580-wave-8-list-8]] records all 158 attempts.

## [2026-10-11] build | List 8 func_80134630 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134630 (312 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_80134178 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134178 (340 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 func_801342CC time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_801342CC (420 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80134470 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80134470 (448 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_80133FB0 time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_80133FB0 (456 bytes), time-boxed. Draft reverted; compile/type/frame failure or timeout.

## [2026-10-11] build | List 8 func_8013AE3C time-boxed

[[tickets/T-9580-wave-8-list-8]]: TT/func_8013AE3C (676 bytes), time-boxed. Draft reverted; instruction/register/control-flow difference.

## [2026-10-11] build | List 8 bounded retries completed

[[tickets/T-9580-wave-8-list-8]]: NULL-repaired TT drafts and six source-order variants were reverted after real differences remained. Type recovery returned no proposals for the two new initializer bases. Exact-path duplicate reuse applied zero copies. Final full build and five resolved function checks started; five matches total 5,360 bytes remain.

## [2026-10-11] ticket | List 8 moved to In Review

Moved [[tickets/T-9580-wave-8-list-8]] to In Review and synchronized [[kanban]] and [[index]]. Inline source/spec review has no findings; final checksum and individual function checks are running.

## [2026-10-11] build | List 8 resolved matches and sequential build retry

All five retained functions in [[tickets/T-9580-wave-8-list-8]] pass funcdiff --resolve. Concurrent ninja linking saw a truncated EN_NICHI object while funcdiff rebuilt it. The checks have finished; the full incremental build is now rerunning without concurrent object writers.

## [2026-10-11] build | List 8 final verification passed

[[tickets/T-9580-wave-8-list-8]]: sequential incremental ninja succeeded, with 27/27 current unit checksums and header/global guards present. Prototype branch, globals, and whitespace checks pass. Final progress is 4739/6958 functions and 734972/2279368 bytes (32.2%). Five matches add 5,360 bytes; 153 other entries are time-boxed and restored. Six rows added to wiki/data/t0018-cases.md. Final coverage and review results recorded in [[matching-notes]] and the ticket.

## [2026-10-11] ticket | List 8 completed

Inline review against CODING_STANDARDS.md and the shared brief has zero unresolved findings. Moved [[tickets/T-9580-wave-8-list-8]] to Done, checked all acceptance criteria, and synchronized [[kanban]] and [[index]]. All edits remain uncommitted for the orchestrator.

## [2026-10-11] ticket | List 8 reopened for deep second pass

User requested sustained iteration after the first pass was committed. Reopened [[tickets/T-9580-wave-8-list-8]] to In Progress, synchronized [[kanban]] and [[index]], and reread standards, [[decompile-workflow]], and [[register-allocation-wave7]]. Start 09:26 UTC; minimum three hours or list exhaustion. Rank actual resolved differences first, recover aggregate types, run extended near-miss searches, and retain only verified matches.

## [2026-10-11] decision | List 8 second-pass ranking

[[tickets/T-9580-wave-8-list-8]]: ranked saved resolved diffs from the initial pass and bounded retries. Smallest remaining gaps are TT func_80134178 (2 lines), DATE func_801511B4 (4), EN_NICHI func_80132A74 (20), and NAME_ENT func_80140D6C (24). TT control-flow, constant-local, and statement-layout experiments began. Large aggregate/local-copy drafts are queued after these near misses.

## [2026-10-11] build | List 8 deep TT scheduling attempt

[[tickets/T-9580-wave-8-list-8]]: func_80134178 received 43 manual/type variants and two 300-second searches. ANSI parameters avoid the K&R permuter bug but preserve the same two-line real diff; typed 0x68 actor fields also preserve it. No verified candidate; restored src/ovl/TT/80133970.c. Type-aware GameState migration has also recovered several previously rejected drafts for renewed ranking.

## [2026-10-11] build | List 8 NAME_ENT buffer match

[[tickets/T-9580-wave-8-list-8]]: func_8013F984: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

## [2026-10-11] ingest | List 8 second-pass type and buffer findings

Recorded type-aware GameState recovery, NAME_ENT func_8013F984's verified 388-byte buffer/return-type match, and the exhausted TT scheduling experiment in [[matching-notes]]. The eight-byte NAME_ENT buffer adjustment is explicitly marked FAKE in source and ticket.

## [2026-10-11] build | List 8 deep DATE table dispatch attempt

[[tickets/T-9580-wave-8-list-8]]: func_801511B4 received about 50 manual variants and a five-minute permuter run. Real checks of both score-30 bodies retain the four-line D_800B593C address-register difference; source restored. DATE func_80150460's earlier diff is instead an unresolved M2C_MEMCPY_ALIGNED pseudo-call: neighboring matched copies use a 0x44-byte aggregate. Added a scoped record view for its real struct-copy retry.

## [2026-10-11] build | List 8 NAME_ENT buffer match

[[tickets/T-9580-wave-8-list-8]]: func_8013DD94: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

## [2026-10-11] build | List 8 NAME_ENT buffer match

[[tickets/T-9580-wave-8-list-8]]: func_8013F758: recovered byte buffer and implicit-int dispatcher; s32-buf-0x108. Resolved MATCH and overlay SHA1 pass; any enlarged buffer marked FAKE.

## [2026-10-11] build | List 8 deep SHOUGATU attempt and NAME_ENT pointer recovery

[[tickets/T-9580-wave-8-list-8]]: func_8013A2F8 improved from 13 to 2 resolved diff lines using existing CharFlags and an element assignment argument; sustained layout and five-minute permuter attempts did not fix the final constant schedule. Source restored. Assembly proves D_800B32CC is a loaded pointer-to-pointer and D_800E7D36 a byte slot; generated their main declarations with sync_protos for NAME_ENT retries.

## [2026-10-11] build | List 8 deep pass copy and allocator experiments

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT `func_8013DD94` (500 bytes) and `func_8013F758` (556 bytes) both pass resolved funcdiff and overlay SHA1 after recovering 0x108-byte local buffers; NAME_ENT deep-pass subtotal is three matches, 1,444 bytes. DATE `func_80150460` now uses a 17-word copy type, reducing the draft to six differing lines; a 300-second permuter finds no improvement and the function is restored. EN_NICHI `func_80132A74`: 37 valid K&R manual variants, compatible wide-parameter trial, and a 300-second permuter search leave 18 differing lines; generated candidates were checked against real funcdiff and the function restored. Recovered SHUGAKU copies and NAME_ENT local-array draft remain experiments pending verification. [[matching-notes]] records the successful buffer and pointer-table idioms.

## [2026-10-11] build | List 8 main buffer-table search match

[[tickets/T-9580-wave-8-list-8]]: main `search_load_tpage_buf_lock` (232 bytes) passes resolved funcdiff and main executable SHA1. Removing the redundant saved-return local recovers the 0x40 frame; `(u8)index & 0x1F` recovers the call-site move/mask sequence with the existing prototype. The shifted GameState cursor is marked FAKE. No ABI override is retained. NAME_ENT `func_8013AF78` is restored after local-array and raw/masked-value experiments. [[matching-notes]] retains the successful idiom.

## [2026-10-11] build | List 8 signed record stores and source ordering

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI `func_801344F0` (660 bytes) passes resolved funcdiff and overlay SHA1. `EnObj` fields preserve signed halfword constants, direct angle-field updates preserve the wrap temporaries, and reversing the two independent halfword stores in cases 0/2 restores the target register order. The successful form needs no FAKE beyond existing file conventions. [[matching-notes]] records the source-order effect.

## [2026-10-11] decision | List 8 aggregate views and shared declaration constraints

[[tickets/T-9580-wave-8-list-8]]: the recovered KANGEI packed records need a whole-word union view in addition to their existing halfword/byte members; the overlay-scoped unions preserve the shared GameState type. A SHOUGATU copy retry discovered that `D_8012071C`, `D_80120760`, and `D_801207A4` share symbol addresses with EVENT declarations. Generated main-API additions must use the existing s32/s32/s16 views; copying uses the recovered 0x44-byte pointer view. Header failures during exploratory declaration changes are excluded from code-generation evidence. A leftover main call declaration was removed, and the clean typed main function and executable SHA1 were reverified.

## [2026-10-11] build | List 8 name-slot scan closes the address reload gap

[[tickets/T-9580-wave-8-list-8]]: NAME_ENT `func_80140D6C` (172 bytes) passes resolved funcdiff and overlay SHA1. The 300-second permuter's candidate checks at two differing lines in the real source; joining pointer initialization with `do` fixes the initial address scheduling. Its alternate end expression retains the fresh end-address load after the loop. The byte-cursor bias, end-address spelling and same-line layout are explicitly FAKE. [[matching-notes]] records the previously open guarded draft's resolution.

## [2026-10-11] build | Wave 8 list 8 packed-mode match

[[tickets/T-9580-wave-8-list-8]]: KANGEI `func_801335F0` adds 324 verified bytes. The real two-bit flag view fixes arithmetic, and inline field indexing plus a marked dead-byte test fixes v0/v1 allocation after a five-minute unsuccessful permuter. Resolved MATCH and KANGEI SHA1 pass. Seven second-pass functions, 2,832 bytes. See [[matching-notes]].

## [2026-10-11] decision | Wave 8 list 8 remaining local-copy drafts

[[tickets/T-9580-wave-8-list-8]]: corrected SHOUGATU's 68-byte copies to use the declared parent buffer rather than adding duplicate main globals owned by EVENT. Recovered missing overlay byte declarations from the original sb stores. The large initializer now compiles at 16 differing lines. NAME_ENT's option snapshot stores likewise supply a missing word and eight byte declarations; earlier undefined-symbol runs are excluded from matching evidence. See [[matching-notes]].

## [2026-10-11] decision | Wave 8 list 8 completed type-repair timeboxes

[[tickets/T-9580-wave-8-list-8]]: restored the unverified BUNKAKEN aligned-table draft, SHUGAKU signed-record/local-copy draft, and DATE color-loop draft after sustained manual experiments and bounded searches. Nearest real diffs remain 129, 20 and 12 lines respectively. Removed their unused appended types; retained verified matches elsewhere. No original sources or compiler passes changed.

## [2026-10-11] decision | Wave 8 list 8 OMIMAI timebox

[[tickets/T-9580-wave-8-list-8]]: OMIMAI's real permuter candidate reduces the near miss to four resolved lines. Thirty-five further store/type/layout experiments did not change its remaining address register. Restored failed C and the unused appended prototype. A temporary main return-type/header mismatch was corrected; affected failed builds are not matching evidence.

## [2026-10-11] build | Wave 8 list 8 ETC initializer match

[[tickets/T-9580-wave-8-list-8]]: ETC `func_801467C4` adds 312 bytes. The record byte view and simple 14-trip for-loop reduce the diff to two lines; a subsequent five-minute permuter confirms that the remaining issue needs a type correction. An unsigned byte view of `unk_F6D` passes the resolved diff and ETC SHA1. Eight second-pass matches, 3,144 bytes; array-bound/type review continues.

## [2026-10-11] lint | Wave 8 list 8 ETC table type review

[[tickets/T-9580-wave-8-list-8]]: changed the new ETC initializer to `unk_69C[unk_71C - 2].unk_00`, using the existing declared flag table instead of indexing across the end of `unk_66C`. The binary's table address is two records before the table's base. Resolved MATCH and ETC unit SHA1 still pass.

## [2026-10-11] decision | Wave 8 list 8 SHOUGATU aggregate timebox

[[tickets/T-9580-wave-8-list-8]]: recovered the large initializer's two 68-byte local copies and removed missing-symbol failures. Thirty-two additional declaration/counter/table-layout experiments and a bounded five-minute search still differ in stack addresses and store scheduling. Restored its C; shared overlay work continues.

## [2026-10-11] build | Wave 8 list 8 SHOUGATU flag match

[[tickets/T-9580-wave-8-list-8]]: SHOUGATU `func_80140AD8` adds 256 bytes. Replacing the abstract mask/byte-or draft with the real character bitfield test and assignment, and using the implicit-integer-return shape, passes the resolved diff and overlay SHA1. Nine second-pass matches, 3,400 bytes; no scheduling trick needed.

## [2026-10-11] decision | Wave 8 list 8 main fade type experiments

[[tickets/T-9580-wave-8-list-8]]: retrying the main fade draft with signed rsin narrowing, wide versus narrow byte temporaries and return-type variants. Main return-type experiments now update the generated declaration through `sync_protos` before building, and restore the prior generated declaration if no match is found. This avoids treating a header mismatch as codegen evidence.

## [2026-10-11] decision | Wave 8 list 8 small-helper timeboxes

[[tickets/T-9580-wave-8-list-8]]: restored TT's typed phase/counter helper after 36 experiments and a five-minute search (14 lines), GYOZI's repeated-product helper after 12 view/index experiments and no type-recovery proposal (29 lines), and TACO's typed command walker after 28 parameter/pointer/constant/layout experiments (25 lines). No unverified helper C or unused TT type retained. Other candidates continue.

## [2026-10-11] decision | Wave 8 list 8 remaining SHOUGATU local copies

[[tickets/T-9580-wave-8-list-8]]: recovered both local table types and retried normalized/narrow year indexing. Narrowing before multiplication gets past the frame pass but remains 202 resolved lines away; the other correct table forms expose negative-stack-offset errors in `frame_pass.py`. Restored failed C and all unused overlay types, keeping the verified flag match.

## [2026-10-11] decision | Wave 8 list 8 fade timebox

[[tickets/T-9580-wave-8-list-8]]: 16 valid main fade type/layout experiments (plus four prior void variants) still differ in promoted byte values and coordinate temporaries. Restored the C and exact prior prototype through the generator. Moving on to EN_NICHI's actor-word flags, whose first draft incorrectly used byte loads.

## [2026-10-11] decision | Wave 8 list 8 actor flag declarations

[[tickets/T-9580-wave-8-list-8]]: EN_NICHI's ten 0x30-byte actors use word flags; the first draft inferred byte accesses and lacked viewport halfword declarations. Added the two main viewport coordinates through the generator (target lh), and missing overlay word/end-address declarations. Invalid missing-symbol attempts are excluded; retrying with the existing `EnObj` record type.

## [2026-10-11] lint | Wave 8 list 8 historical register rows resolved

[[tickets/T-9580-wave-8-list-8]]: appended three resolution rows to [[data/t0018-cases]] for NAME_ENT's repeated-end-pointer loop, SHOUGATU's flag initializer, and the search half of the main texture-page-lock family. Historical rows remain intact; the regular lock helper remains open. Verified function/unit evidence is in the ticket.

## [2026-10-11] decision | Wave 8 list 8 actor flag timebox

[[tickets/T-9580-wave-8-list-8]]: actor-word flags compile after complete declaration recovery. Twenty valid end-pointer/status/counter-view/reused-pointer/increment variants reach 83 lines, still outside the near-match search range. Restored the failed function; testing the initializer's ten-byte flag aggregate and corrected word-pointer stepping next.

## [2026-10-11] decision | T-9580 deep aggregate timeboxes and review cleanup

[[tickets/T-9580-wave-8-list-8]] records the deeper NAME_ENT option-record, TT flag-dispatch and EN_NICHI initializer experiments. Restored failed functions and unused overlay types. Removed stale commentary and unused declarations from retained matches; resolved function and unit verification is running again. The TT two-actor initializer is the current candidate, with parameter-width and declaration-order experiments.

## [2026-10-11] build | T-9580 retained deep matches survive cleanup

All nine additional functions in [[tickets/T-9580-wave-8-list-8]] pass `funcdiff --resolve` after removing unused declarations and the stale NAME_ENT guard comment. The equivalent if/else form of SHOUGATU `func_80140AD8` also matches, so no valueless return remains. NAME_ENT, main, EN_NICHI, KANGEI, ETC and SHOUGATU unit SHA1 checks all pass. Generator-only cleanup removes five unused main globals from rejected drafts, retaining only the two declarations required by matched NAME_ENT code. Final full-build verification remains pending after the last TT experiment.

## [2026-10-11] decision | T-9580 TT paired initializer timeboxed

[[tickets/T-9580-wave-8-list-8]] records 94 valid source/type experiments on TT `func_80134630`; best 45 resolved differing lines. Restored TT C and header to their committed content after the timebox. EN_NICHI frame-only miss is the final active experiment; its near draft is undergoing a 300-second search.

## [2026-10-11] build | T-9580 final declaration guards pass

`check_headers.py include src` reports headers OK; `migrate_globals.py --check` reports globals OK; `sync_protos.py --check-branch` reports zero new disagreements and 17 known baseline disagreements. [[tickets/T-9580-wave-8-list-8]] retains only its required main-API additions and packed KANGEI view. No tool/config changes were needed. EN_NICHI's final ANSI search is still running, followed by restoration and whole-build verification.

## [2026-10-11] decision | T-9580 deep pass closes after three hours

[[tickets/T-9580-wave-8-list-8]] records the final EN_NICHI frame-only timebox and restoration. Corrected ETC's subarray-boundary view to the actual +0x694 table base; the resolved diff remains MATCH. All failed drafts are reverted; nine additional matches (3,400 bytes) remain. Deep pass began 09:26 UTC and active matching ended 12:30 UTC, exceeding three hours. Final exact/near duplicate scans and full 27-unit build follow.

## [2026-10-11] lint | T-9580 inline review findings

[[tickets/T-9580-wave-8-list-8]] records the resolved ETC bounds finding and pending removal of two unused EN_NICHI temporaries. Own-file exact duplicate scan applied zero copies. Scope, C89, declaration ownership and FAKE coverage checked; near-duplicate scan and final build are still running.

## [2026-10-11] build | T-9580 duplicate scans complete

Exact and near-duplicate scans with the ticket's exact 36-file list both applied zero copies. Full sequential `ninja -j4` is running for [[tickets/T-9580-wave-8-list-8]]. No function-matching process is writing its objects concurrently.

## [2026-10-11] ticket | T-9580 In Review after deep pass

[[tickets/T-9580-wave-8-list-8]] moved from In Progress to In Review; ticket, [[kanban]] and [[index]] synchronized. Full build passed all 27 SHA1 checks. The last inline-review cleanup is being verified before completion.

## [2026-10-11] build | T-9580 final review cleanup verified

Both unused EN_NICHI temporaries were removed with a resolved MATCH. Added a repeated-inclusion guard around ETC's new prefix view. The subsequent full sequential build passes all 27 unit SHA1 checks; progress is `grand total   4748/6958   738372/2279368  32.4%`. Inline review against CODING_STANDARDS.md and the shared brief has no unresolved code/spec findings. A scratch metadata assertion was too strict because the globals marker need not track an unrelated overlay header; direct final header/global checks are running to establish their results from source instead. [[tickets/T-9580-wave-8-list-8]] awaits those checks before Done.

## [2026-10-11] build | T-9580 final source guards complete

Direct `check_headers.py include src` and `migrate_globals.py --check` both pass after the last review changes. Guard markers exist, branch prototype check has zero new disagreements, and `git diff --check` passes. All 27 final unit checksums and the recorded progress line remain valid. [[tickets/T-9580-wave-8-list-8]] has no outstanding review findings.

## [2026-10-11] ticket | T-9580 Done after deep pass

[[tickets/T-9580-wave-8-list-8]] moved from In Review to Done; ticket, [[kanban]] and [[index]] synchronized. Inline review and final checks are complete; all changes remain uncommitted.
## [2026-10-11] ticket | Start wave 8 list 2

Created [[tickets/T-9520-wave-8-list-2]] In Progress and its card in [[kanban]]. Read shared brief, [[decompile-workflow]], standards and [[register-allocation-wave7]]. Initial Docker configure succeeded; ninja is generating missing assembly and objects.

## [2026-10-11] query | Register-shape CLI differs from brief

For [[tickets/T-9520-wave-8-list-2]], tools/register_shapes.py rejects a positional function name. Its module shape() supports filtering; documented in the ticket. No tooling changed.

## [2026-10-11] query | Inspect first wave-8 drafts

For [[tickets/T-9520-wave-8-list-2]], DATE func_8014AD9C needs an SDK MATRIX copy and translation fields; GEKO func_80140FD0 accesses CharFlags.f9. NAME_ENT func_801341A8 needs its old-value snapshot restored in the draft. Documented hand-written DATE rotation helper remains assembly per [[decompile-workflow]]. Initial build checksums continue passing.

## [2026-10-11] build | Wave 8 list 2 baseline passes

Initial Docker ninja completed: 27/27 unit checksums, headers and globals OK. [[tickets/T-9520-wave-8-list-2]] now starts compile/diff attempts.

## [2026-10-11] query | Wave 8 list 2 early draft blockers

Recorded return-type and byte-pointer draft issues in [[matching-notes]] for [[tickets/T-9520-wave-8-list-2]]. No failed C retained.

## [2026-10-11] decision | Time-box early wave-8 near misses

For [[tickets/T-9520-wave-8-list-2]], early setters, NAME_ENT selector/snapshot and GEKO bit-field probes remain unresolved. Restored failed source and declaration probes by editing files back. Recovery ninja passed all units. Permuter main-setter score remained 190; resolved funcdiff isolates a final-store delay-slot swap.

## [2026-10-11] query | Wave 8 typed loops and declarations

Temporary overlay declarations support bounded checks for [[tickets/T-9520-wave-8-list-2]], headers OK. TT constant ordering is a near miss; TACO loops need four actual locals rather than m2c spill placeholders.

## [2026-10-11] build | Bounded wave-8 draft pass

Cleaned drafts through index 64 checked and restored on failure. TT constant register order recovered with a named loop bound, but instruction placement still differs. Details in [[tickets/T-9520-wave-8-list-2]]; drafts/diffs in scratchpad/w8-2/.

## [2026-10-11] build | Match NAME_ENT func_801330E8

src/ovl/NAME_ENT/80132000.c: 532 bytes; resolved funcdiff and ninja passed. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Match NAME_ENT func_801332FC

src/ovl/NAME_ENT/80132000.c: 532 bytes; resolved funcdiff and ninja passed. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Match RPG_BAT func_801481C8

src/ovl/RPG_BAT/801480F0.c: 792 bytes; resolved funcdiff and ninja passed. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Match TACO func_801361E8 with independent byte proof

src/ovl/TACO/80135720.c: full ninja and TACO checksum pass; 592 bytes from original instruction comments equal linked binary. funcdiff has a false HI16 string-only difference. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Match TACO func_80136438 with independent byte proof

src/ovl/TACO/80135720.c: full ninja and TACO checksum pass; 592 bytes from original instruction comments equal linked binary. funcdiff has a false HI16 string-only difference. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Match TACO func_80136688 with independent byte proof

src/ovl/TACO/80135720.c: full ninja and TACO checksum pass; 592 bytes from original instruction comments equal linked binary. funcdiff has a false HI16 string-only difference. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] query | Wave-8 complete assignment audit

All 142 listed functions have an m2c attempt and an initial stopping point in [[tickets/T-9520-wave-8-list-2]]. Six functions (3,632 bytes) retained so far: two NAME_ENT text pages, RPG_BAT state switch and three TACO string switches. TACO is verified by unit checksum and direct original instruction bytes despite a false funcdiff HI16 string difference. Draft type/view retries and bounded permuter output remain in scratchpad/w8-2/.

## [2026-10-11] ticket | T-9520 to In Review

All assigned functions attempted; six matches retained, 14 T-0018 rows appended and failed C/header trials restored. Inline standards/spec review has no source findings. Prototype/global guards pass; final default build pending. [[tickets/T-9520-wave-8-list-2]], [[data/t0018-cases]], [[matching-notes]].

## [2026-10-11] lint | Wave-8 cleanup and review

Failed source/header probes restored; only three owned C files and the two necessary overlay headers remain changed. The ticket and kanban both show In Review. Assignment audit covers 142 functions; 14 rows classified in [[data/t0018-cases]], scoped dupes applies zero, prototype/global guards pass. Final default ninja is rebuilding after header cleanup. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]].

## [2026-10-11] build | Wave-8 list-2 final verification

Default Docker ninja exits 0: all 27 units checksum OK, headers/globals OK. Resolved function diffs pass for the two NAME_ENT pages and RPG_BAT switch; each TACO function independently equals 592 original linked bytes. Prototype guard has zero new findings; diff whitespace check passes; scoped dupes adds zero. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]].

## [2026-10-11] ticket | T-9520 Done after inline review

Reviewed the working diff against CODING_STANDARDS.md and the shared brief, no unresolved findings. Six functions/3,632 bytes matched, remaining 136 time-boxed and restored; 14 rows appended to [[data/t0018-cases]]. Ticket/kanban/index now agree on Done; uncommitted worktree edits are ready for the orchestrator. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] query | Wave-8 measured progress and byte-count correction

`ninja progress`: grand total 4740/6958 functions, 733232/2279368 bytes, 32.2%. Actual gain over 4734/729612 is 6 functions / 3620 bytes. Earlier 3632-byte reports used the work-list extent for RPG_BAT func_801481C8 (792), while splat records 0x30C (780); the ticket and own notes now use measured function bytes. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]].

## [2026-10-11] ticket | Reopen T-9520 for sustained matching experiments

[[tickets/T-9520-wave-8-list-2]] is In Progress again; kanban and index agree. Second pass began 10:01:36 UTC and must continue to at least 13:01:36 UTC unless the list is exhausted. Ranked 26 compiling rejected drafts by resolved differing lines; nearest NAME_ENT func_80132198 has two, KANGEI func_80135C78 five, SHOUGATU func_801347E0 six. Missing aggregate and declaration failures are deferred for type recovery, rather than classified as codegen gaps. Saved ranking: scratchpad/w8-2/deep-ranking.txt.

## [2026-10-11] build | Second-pass match RPG_BAT func_801496F0

src/ovl/RPG_BAT/801496F0.c: resolved funcdiff and unit SHA1 passed; third-argument-and-existing-aggregate-fields. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] decision | Recover overlapping transform-copy locals

TACO func_8015B3B0 copies a 0x20-byte payload at sp+0x34; m2c's separate sp48/sp4C/sp50 locals actually alias its final three words. A typed aggregate merges those aliases and models the 0x50-byte object view without changing the existing TcObj50 declaration. Neighboring matched func_8015B2AC confirms object transform at +4 and position words at +0x18/+0x1C/+0x20. Trial types appended to include/ovl/TACO.h; retain only if a verified C user survives. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]].

## [2026-10-11] build | Second-pass match KANGEI func_80135C78

src/ovl/KANGEI/801355F0.c: resolved funcdiff and unit SHA1 passed; s32-no-return-baseline. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass permuter match TT func_80147074

src/ovl/TT/80146BE0.c: resolved diff and unit SHA1 passed; source review pending. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] query | NAME_ENT closest draft survives sustained spelling search

NAME_ENT func_80132198 restored after 56 manual probes and a 300-second permuter. Contextual comparison identifies li v0,1 versus preceding lui at at the initial state store; the later increment is already identical. Best remains two differing lines; best permuter output (score 175) was verified against the real build and worsened the stores. Failed C restored. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]], [[data/t0018-cases]].

## [2026-10-11] build | Second-pass permuter match TACO func_8015B3B0

src/ovl/TACO/8015ABF0.c: resolved diff and unit SHA1 passed; source review pending. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match TT func_80147074

src/ovl/TT/80146BE0.c: resolved funcdiff and unit SHA1 passed; typed-record-mask. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] decision | Correct cached probe verification

The scratch optimization that imported funcdiff.main reused its per-process _checked build cache: only the first variant in those optimized runs rebuilt. Those repeated negative variant counts are withdrawn and will be rerun with build/header caches cleared each iteration. Retained matches remain valid: the helper independently ran a fresh funcdiff subprocess and unit SHA1 before retaining them; permuter candidates likewise have fresh real-build verification. NAME_ENT's 56 subprocess probes were unaffected. No tools/ file edited. [[tickets/T-9520-wave-8-list-2]], [[matching-notes]].

## [2026-10-11] build | Second-pass match RPG_BAT func_80135E80

src/ovl/RPG_BAT/80135AE0.c: resolved funcdiff and unit SHA1 passed; member-timer-no-locals-clean. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] ingest | Recover local buffer and transform layouts

Read original NAME_ENT func_80133510 and TACO func_8015B1B8 assembly alongside their rejected drafts. Recovered six 35-byte text buffers and the text-record length/pointer offsets; recovered the halfword basis/word translation payload. Temporary overlay views are appended for genuine compiler retries, and will be removed unless verified C uses them. DATE's four-line pointer spill mismatch survives 48 further local-home variants. The cached KANGEI negative count is withdrawn in [[matching-notes]]. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match RPG_BAT func_80135B4C

src/ovl/RPG_BAT/80135AE0.c: resolved funcdiff and unit SHA1 passed; member-timer-increment. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match RPG_BAT func_8013F3F0

src/ovl/RPG_BAT/8013E4D0.c: resolved funcdiff and unit SHA1 passed; record-subgroup-s32-knr0. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] decision | Nested record group fixes RPG loop bias

RPG_BAT func_8013F3F0 passes resolved diff and the unit SHA1 after replacing a flat record view with a 96-record prefix and 64-record group. Twenty-four counter/pointer/type variants and sixteen pointer-end variants had exposed the pointer-bias distinction; the first nested-group form matches in plain C. Applying the confirmed view to the initializer and its neighboring record users. [[matching-notes]], [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] query | Extend confirmed RPG group and TT local recovery

RPG_BAT func_8013E7C0/func_8013E810/func_8013E97C already access the +0x1980 record group, independently confirming the nested type retained for func_8013F3F0. The initializer's first generated group probe used wrong constants and is discarded as evidence; corrected probes use byte 5, word 0x41000000 and halfword 0x1000 from the original. TT func_8013FF14 is within eight differing lines using a named s32 one and angle temporary; the remaining frame and pointer-home differences are under investigation. [[matching-notes]], [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match RPG_BAT func_8013E4D0

src/ovl/RPG_BAT/8013E4D0.c: resolved funcdiff and unit SHA1 passed; group-init-s32-decl0-split0-correct-values. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | T-9520 aggregate alias and corrected case experiments

EVENT `func_801035BC`: a 96-record prefix view keeps its selector load inside
the loop and reduces the draft to 19 resolved differing lines. RPG_BAT
`func_8013E54C`: corrected case 5 to vector element 5 / girl 28 before further
counter and pointer experiments. TT `func_8013FF14`: a 300-second permuter run
is testing the four-line frame-size candidate. [[matching-notes]],
[[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match TT func_8013FF14

src/ovl/TT/8013F040.c: resolved funcdiff and unit SHA1 passed; constant-period-order0-u32. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | Second-pass match TACO func_8013B170

src/ovl/TACO/8013B170.c: resolved funcdiff and unit SHA1 passed; taco-selector-u8-void-reload0-if0. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] build | T-9520 ten-gain full build and inline review

Second pass has ten verified functions / 3,416 bytes. Default incremental
`tools/docker.sh ninja` passed the main executable and every overlay; TACO was
already current after its fresh unit check. Header/global checks passed.
TACO `func_8013B170` must reread the scene selector after callbacks. Inline
review marks KANGEI's no-value s32 return and TT's constant/layout locals FAKE;
their intended original source is unknown. Unused trial declarations will be
removed before final verification. [[matching-notes]],
[[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] lint | T-9520 second-pass cleanup and bounded-case audit

[[tickets/T-9520-wave-8-list-2]]: removed all trial-only overlay declarations, appended retained views to baseline headers, and normalized new unknown-field names. [[matching-notes]] records ten verified gains (3,416 bytes) and partial aggregate-recovery coverage; [[data/t0018-cases]] gains eleven classified bounded rows, bringing this pass to twelve. Cached/type-invalid negative outputs are excluded; every unsuccessful experiment retains original source. Final fresh verification and inline review are pending.

## [2026-10-11] ticket | T-9520 second-pass In Review

[[tickets/T-9520-wave-8-list-2]] and the kanban/index move to In Review. Fresh ten-function resolved diffs and all 27 unit SHA1 checks pass after cleanup; [[matching-notes]] and [[data/t0018-cases]] record gains and bounded failures. Inline standards/spec review finds no unresolved source issue and explicitly records incomplete large-type coverage under the three-hour budget. Final SHOUGATU search and elapsed-time floor remain pending.

## [2026-10-11] build | Second-pass permuter match SHOUGATU func_80134730

src/ovl/SHOUGATU/80134120.c: resolved diff and unit SHA1 passed; source review pending. [[tickets/T-9520-wave-8-list-2]].

## [2026-10-11] ticket | T-9520 second-pass Done after three hours

[[tickets/T-9520-wave-8-list-2]], kanban and index move from In Review to Done after inline standards/spec review and final matched-tree validation. Eleven added functions / 3,548 bytes; batch total 17 / 7,168. All 27 unit SHA1 checks, default ninja and guards pass; [[matching-notes]] records final progress 4751/6958 and 736780/2279368 (32.3%). Twelve second-pass rows are in [[data/t0018-cases]]; the last SHOUGATU row is resolved by a marked FAKE. The larger unrecovered aggregate group is explicitly deferred under the three-hour budget. All changes remain uncommitted.
## [2026-10-11] ticket | Wave 8 list 1 started

Created [[tickets/T-9510-wave-8-list-1]] In Progress and its [[kanban]] card. Read [[decompile-workflow]] and [[register-allocation-wave7]]. Configuring this new worktree before drafts.

## [2026-10-11] build | Wave 8 list 1 baseline verified

Initial Docker configure and ninja completed all 27 binary SHA1 checks, with headers and global guards passing. Draft generation proceeds from explicit assembly paths for [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Wave 8 list 1 func_80105DF8

[[tickets/T-9510-wave-8-list-1]]: EVENT func_80105DF8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147444

[[tickets/T-9510-wave-8-list-1]]: TACO func_80147444 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139F8C

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139F8C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013C5C8

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013C5C8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80132A4C

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80132A4C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013AE14

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013AE14 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801428EC

[[tickets/T-9510-wave-8-list-1]]: ETC func_801428EC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139878

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139878 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80145EA4

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80145EA4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013A63C

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013A63C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013ACBC

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013ACBC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139E3C

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139E3C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142AF0

[[tickets/T-9510-wave-8-list-1]]: TT func_80142AF0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80105DF8

[[tickets/T-9510-wave-8-list-1]]: EVENT func_80105DF8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147444

[[tickets/T-9510-wave-8-list-1]]: TACO func_80147444 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139F8C

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139F8C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013C5C8

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013C5C8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013AE14

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013AE14 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142AE8

[[tickets/T-9510-wave-8-list-1]]: ETC func_80142AE8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801428EC

[[tickets/T-9510-wave-8-list-1]]: ETC func_801428EC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139878

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139878 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80145EA4

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80145EA4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80133D54

[[tickets/T-9510-wave-8-list-1]]: BUNKA_SD func_80133D54 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147FD0

[[tickets/T-9510-wave-8-list-1]]: TT func_80147FD0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013A63C

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013A63C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139E3C

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139E3C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142AF0

[[tickets/T-9510-wave-8-list-1]]: TT func_80142AF0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80136340

[[tickets/T-9510-wave-8-list-1]]: SHOUGATU func_80136340 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013CF88

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013CF88 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8015D270

[[tickets/T-9510-wave-8-list-1]]: TACO func_8015D270 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013C780

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013C780 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80054704

[[tickets/T-9510-wave-8-list-1]]: main func_80054704 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147444 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 8 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_80147444 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 4 source variants matched a resolved function diff and unit SHA1.

## [2026-10-11] build | Wave 8 list 1 func_80142AE8 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 4 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_80140810

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_80140810 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8004B590

[[tickets/T-9510-wave-8-list-1]]: main func_8004B590 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142570

[[tickets/T-9510-wave-8-list-1]]: TT func_80142570 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147D08

[[tickets/T-9510-wave-8-list-1]]: TT func_80147D08 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147A80

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80147A80 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80149144

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80149144 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80133024

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80133024 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013D788

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013D788 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801469D0

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_801469D0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801400D8

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_801400D8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801455E4

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_801455E4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8014BF2C

[[tickets/T-9510-wave-8-list-1]]: ETC func_8014BF2C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147A18

[[tickets/T-9510-wave-8-list-1]]: TT func_80147A18 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80149B08

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80149B08 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013A314

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013A314 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80054704 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 4 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_801328D0

[[tickets/T-9510-wave-8-list-1]]: DATE2 func_801328D0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013D92C

[[tickets/T-9510-wave-8-list-1]]: SHOUGATU func_8013D92C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801454CC

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_801454CC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 make_color_bar

[[tickets/T-9510-wave-8-list-1]]: main make_color_bar draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80141468

[[tickets/T-9510-wave-8-list-1]]: ETC func_80141468 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013644C

[[tickets/T-9510-wave-8-list-1]]: SHOUGATU func_8013644C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801473AC

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_801473AC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801486F8

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_801486F8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013ABAC

[[tickets/T-9510-wave-8-list-1]]: SHUGAKU func_8013ABAC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013655C

[[tickets/T-9510-wave-8-list-1]]: TEL func_8013655C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142544

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_80142544 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142DF4

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_80142DF4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B608

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B608 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] decision | Wave 8 TAIIKU data island scope

[[matching-notes]], [[tickets/T-9510-wave-8-list-1]]: Docker dry run confirms the owned data range and 60 pieces. Applying it writes config/overlays/TAIIKU.yaml, outside the brief permitted edits. Kept the configuration unchanged and recorded the follow-up route.

## [2026-10-11] build | Wave 8 list 1 func_8005478C

[[tickets/T-9510-wave-8-list-1]]: main func_8005478C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80141ABC

[[tickets/T-9510-wave-8-list-1]]: ETC func_80141ABC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B200

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B200 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B78C

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B78C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80133598

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80133598 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 make_color_bar16

[[tickets/T-9510-wave-8-list-1]]: main make_color_bar16 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_800536AC

[[tickets/T-9510-wave-8-list-1]]: main func_800536AC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013801C

[[tickets/T-9510-wave-8-list-1]]: TACO func_8013801C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B94C

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B94C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80148F08

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80148F08 matched with resolved function diff and binary SHA1.

## [2026-10-11] build | Wave 8 list 1 func_8013B3B0

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B3B0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80054704 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 1 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_8013C5C8 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 2 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_80054704 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 6 source variants did not match; restored source and header.

## [2026-10-11] build | Wave 8 list 1 func_80054704 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 1 source variants matched a resolved function diff and unit SHA1.

## [2026-10-11] build | Wave 8 list 1 func_80133308

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80133308 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B2F0

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_8013B2F0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013C0E4

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013C0E4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80134808

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80134808 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80137BB4

[[tickets/T-9510-wave-8-list-1]]: ENDING func_80137BB4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_800552DC

[[tickets/T-9510-wave-8-list-1]]: main func_800552DC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80138DF8

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80138DF8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8014E300

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_8014E300 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013C978

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013C978 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80138C20

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80138C20 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80138FC0

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80138FC0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80144B44

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80144B44 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801387B0

[[tickets/T-9510-wave-8-list-1]]: ENDING func_801387B0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80132D04

[[tickets/T-9510-wave-8-list-1]]: GEKO func_80132D04 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80147E80

[[tickets/T-9510-wave-8-list-1]]: TACO func_80147E80 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80132C3C

[[tickets/T-9510-wave-8-list-1]]: ETC func_80132C3C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013639C

[[tickets/T-9510-wave-8-list-1]]: TEL func_8013639C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801421AC

[[tickets/T-9510-wave-8-list-1]]: ETC func_801421AC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80142390

[[tickets/T-9510-wave-8-list-1]]: ETC func_80142390 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801325C0

[[tickets/T-9510-wave-8-list-1]]: GEKO func_801325C0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80054FC0

[[tickets/T-9510-wave-8-list-1]]: main func_80054FC0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80133984

[[tickets/T-9510-wave-8-list-1]]: BUNKA_SD func_80133984 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801427A8

[[tickets/T-9510-wave-8-list-1]]: TT func_801427A8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013867C

[[tickets/T-9510-wave-8-list-1]]: BUNKA_SD func_8013867C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80137FEC

[[tickets/T-9510-wave-8-list-1]]: BUNKA_SD func_80137FEC draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013F3A4

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013F3A4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80133FA4

[[tickets/T-9510-wave-8-list-1]]: BUNKA_SD func_80133FA4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8014DF60

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_8014DF60 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801427E4

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_801427E4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80149840

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80149840 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80146624

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80146624 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013DDB8

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013DDB8 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80145AC4

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80145AC4 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8014826C

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_8014826C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80136448

[[tickets/T-9510-wave-8-list-1]]: ETC func_80136448 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80141154

[[tickets/T-9510-wave-8-list-1]]: ETC func_80141154 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80139E3C

[[tickets/T-9510-wave-8-list-1]]: OPTION func_80139E3C draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_801354B0

[[tickets/T-9510-wave-8-list-1]]: BUNKASAI func_801354B0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80145378

[[tickets/T-9510-wave-8-list-1]]: NAME_ENT func_80145378 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_80146030

[[tickets/T-9510-wave-8-list-1]]: BUNKASAI func_80146030 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8013B3B0

[[tickets/T-9510-wave-8-list-1]]: TAIIKU func_8013B3B0 draft did not pass; restored INCLUDE_ASM and temporary header additions.

## [2026-10-11] build | Wave 8 list 1 func_8014DF60

[[tickets/T-9510-wave-8-list-1]]: RPG_BAT func_8014DF60 matched with resolved function diff and binary SHA1.

## [2026-10-11] build | Wave 8 list 1 func_80145378 bounded variants

[[tickets/T-9510-wave-8-list-1]]: 2 source variants did not match; restored source and header.

## [2026-10-11] ticket | Wave 8 list 1 in review

[[tickets/T-9510-wave-8-list-1]], [[kanban]]: all 156 listed functions attempted, failed edits restored. Four functions / 1,312 bytes verified individually. Started inline standards/spec review; final duplicate and full-build checks remain.

## [2026-10-11] query | Wave 8 register-shape blockers

[[data/t0018-cases]], [[register-allocation-wave7]], [[matching-notes]], [[tickets/T-9510-wave-8-list-1]]: appended nine concrete symptom rows. Used register_shapes inventory API; preserved historical rows. All blocked functions remain INCLUDE_ASM. Scoped duplicate pass rejected SHOUGATU func_80136770 on unit SHA1 and restored it; no duplicate gains.

## [2026-10-11] decision | Wave 8 inline review cleanup

[[matching-notes]], [[tickets/T-9510-wave-8-list-1]]: resolved the raw-offset finding with generated main-api symbol D_80120E49, documented the preserved counter read, and removed a draft-only comment. No tooling/config/unowned source edits.

## [2026-10-11] lint | Wave 8 list coverage audit

[[tickets/T-9510-wave-8-list-1]]: checked all 156 entries and 84,972 listed bytes against actual definitions/INCLUDE_ASM. Retained 4 functions / 1312 bytes; all other entries are time-boxed. Every retained definition stays in its owned file. No discarded draft remains.

## [2026-10-11] build | Wave 8 list 1 final verification
All four retained functions pass resolved funcdiff, all 27 binary SHA1 checks pass, and build/headers.ok plus build/globals.ok are present. sync_protos --check-branch and migrate_globals --check pass. A transient ninja deps-log error during the first main diff was resolved by rerunning it successfully. Scoped duplicate pass gained nothing (one failed candidate reverted); git diff --check is clean. Grand progress: 4738/6958, 730924/2279368, 32.1%. See [[tickets/T-9510-wave-8-list-1]] and [[matching-notes]].

## [2026-10-11] lint | Wave 8 list 1 inline standards review
Reviewed the four source bodies, generated main global, append-only overlay forwards, full 156-entry audit and allowed file scope against CODING_STANDARDS.md and the shared brief. Resolved raw workspace-offset access using D_80120E49, documented original uninitialized stack read, and removed draft noise. FAKE frame padding and volatile counter are marked with ticket/reason. No unresolved findings, tooling/config edits, raw source edits or commits. Added a table heading around the nine new symptom rows in [[data/t0018-cases]]. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] ticket | T-9510 Done
Moved [[tickets/T-9510-wave-8-list-1]] from In Review to Done after verification and inline review; synchronized [[kanban]] and [[index]]. Final 4 matches / 1,312 bytes, nine T-0018 rows, every remaining listed function time-boxed and restored. Blockers and tooling observations are recorded for the orchestrator. All edits remain uncommitted.

## [2026-10-11] ticket | T-9510 deep second pass
Reopened [[tickets/T-9510-wave-8-list-1]] after the user committed the first pass and requested sustained function-by-function experiments. Synchronized [[kanban]] and [[index]]. Re-read standards, local instructions, [[decompile-workflow]] and [[register-allocation-wave7]]. Start 10:00:31 UTC; minimum three-hour pass unless list exhausted. Rank closest drafts before aggregate recovery. New changes stay uncommitted.

## [2026-10-11] decision | Deep-pass closest draft ranking
Ranked 51 successfully compiled rejected drafts by signed differing lines in resolved funcdiff; error output is excluded. The closest are ETC func_8014BF2C (4), NAME_ENT func_801454CC (10), BUNKASAI func_80146030 (14), GEKO func_801325C0 (16), OPTION func_80139F8C and main func_8005478C (18). The 100 unrankable entries include build/type failures, deferred for manual repair. First-pass JSON and deep ranking are retained as local build outputs; see [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] decision | Deep ETC result liveness trial
ETC func_8014BF2C received sustained target-assembly comparison, a full 300-second permuter, and 14 completed control/type/return/ordering probes. Initial two branch operands use A1 instead of V0. Void return fixes the body but removes the entry V0 copy; explicit returns worsen layout. No verified match after about 17 minutes. Stopped our own identified batch container at the time box and restored src/ovl/ETC/8014ABD0.c. Next is NAME_ENT loop scheduling. See [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match NAME_ENT func_801454CC
The last random result is captured before pointer advancement; initializing pointer and counter together in the for statement gives the original prologue and delayed final byte store. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 1 functions / 280 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] decision | Bunka adjacent state words
BUNKASAI func_80146030 matches after reading D_80122EB8 and writing its +4 neighbour through a two-word aggregate, plus the existing GameState CharFlags bit-field. No dead test needed. The four-register selector mismatch was a source-type issue in this instance. Moved the documented BunkSelectWords view from the trial C to an append-only block in include/ovl/BUNKASAI.h; verifying again before retention. See [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match BUNKASAI func_80146030
A documented two-word aggregate view of D_80122EB8 and its +4 neighbour keeps the selector load before the stores and in V1; the existing GameState flag bit-field gives the original byte temporaries. The type is appended in the overlay header; no FAKE is needed. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 2 functions / 488 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match GEKO func_801325C0
Replaced scaled arithmetic through scalar record members with GameState array indexing, and cleared the observed two-byte D_80144C40 slot with a halfword view. Both this function and func_801327AC use SH at that slot; the append-only header retains its existing byte declaration. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 3 functions / 980 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | OPTION callback deep timebox
Over 50 compiled source experiments and a 300-second permuter reduced func_80139F8C from 18 differing lines to six (five with an unsuitable wide-mask candidate). Remaining argument copy/scheduling differs; restored source. Updated [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match BUNKASAI func_801354B0
Reused BunkSelectWords for the adjacent selector/result words and the existing GameState flag bit-field. This preserves the selector load in V1 before both stores, matching all instructions without a FAKE. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 4 functions / 1144 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match TACO func_801446D8
Recovered two aligned 32-byte transform workspaces and the 0x10 record stride. FAKE: two spare workspace words retain the observed frame size, and pointer initialization shares the do line to permit the original schedule. Resolved funcdiff and unit SHA1 were rerun after marking the trick. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 5 functions / 1392 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match main func_8005478C
Rewrote the two retry loops to preserve their separate counters and increment positions. FAKE: unused padding reproduces the 0x78-byte frame and SP+0x4C filename buffer. Resolved funcdiff and unit SHA1 pass after the source annotation. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 6 functions / 1640 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | SHOUGATU local callback table repaired, still unmatched
Recovered func_80136078's 50 callbacks and 0x80 argument. Tested ordinary pointer/index/return-type and declaration-order shapes; frame can be exact but register allocation still differs. Reverted source and provisional type. Updated [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match TT func_801478D0
Recovered the four-byte animation frames and the state prefix from the two-level table reads. A named frame pointer and direct byte predecrement reproduce the original register lifetimes. Moved the types to the overlay header, removed the unused draft local, and reverified resolved funcdiff plus unit SHA1; no FAKE needed. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 7 functions / 1968 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match ETC func_801421AC
A local byte pointer to the GameState step field and a void return type preserve the selector in V1, the RA delay-slot store, and the pad-word load before the final switch branch. Resolved funcdiff and ETC unit SHA1 pass without a FAKE. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 8 functions / 2452 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match ETC func_80142390
Reused the local GameState byte pointer and void callback form from func_801421AC for the second save-dialog state machine. Both resolved funcdiff checks and the ETC unit SHA1 pass after whitespace cleanup; no FAKE needed. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 9 functions / 2936 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match TAIIKU func_8013C5C8
Recovered the mixed whole-word/high-half coordinate view of D_8011ECD0 and signed coordinate-bound declarations. A 300-second permuter reached an eight-line miss; a second empty high-half condition reached exact output. FAKE: both empty conditions retain the shared V1 base and prevent constant address folding. Full resolved funcdiff and the TAIIKU unit SHA1 pass. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 10 functions / 3140 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass ten-match checkpoint
Full ninja passed all 27 unit SHA1 checks plus headers/globals guards after restoring unsuccessful production drafts. Second-pass subtotal 10 functions / 3,140 bytes. Recorded in [[tickets/T-9510-wave-8-list-1]]. Further experiments use scratch compiles until this checkpoint finishes.

## [2026-10-11] build | Duplicate clone requires a corrected GameState offset
Owned-file-only dupes proposed SHOUGATU func_80136770; resolved funcdiff rejected the copied GameState+0x10EA byte view because the target uses +0x110A. No match counted. Correcting against asm; recorded tool limitation in [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match SHOUGATU func_80136770
Owned-file duplicate of func_80137D70, repaired against target asm: it increments GameState.unk_110A, not the source function's .unk_10EA. A direct member access matches, so no byte-view hack is retained. FAKE: the copied T-2020 return-value byte cast reserves a temporary; that comment remains. Resolved funcdiff and SHOUGATU SHA1 pass after cleanup. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 11 functions / 3320 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Main checksum routine repaired and timeboxed
func_80055A38 now has a typed, instruction-equivalent scratch draft but its six-variable allocation differs. Eighteen completed source forms and a real 300-second permuter found no exact candidate; best permuter output still has 40 differing lines. Production source restored. Recorded in [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | TACO matrix-copy draft meets a conflicting header
Recovered func_801472D4's matrix and coordinate prefix types and narrow pointer strides. Target returns 0/-1, while the existing overlay header declares void(s32,...). The append-only header constraint prevents changing that committed declaration. Four completed compile attempts reject the declaration; restored source and recorded in [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Deep-pass match ETC func_801428EC
A local pointer to the byte shade makes its load use V0, then reuses V0 for the packed RGB argument; the s32 body intentionally has no return. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 12 functions / 3456 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match NAME_ENT func_80145378
The counter comparison must use the old value before post-increment; a local u8 state pointer also preserves V1 without the extra selector copy. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 13 functions / 3676 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match RPG_BAT func_801427E4
Use the neighbouring matched three-word state type for its counter fields; ++/-- inside the temporary assignment preserves call scheduling and V0 for the later reload. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 14 functions / 4264 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] build | Deep-pass match ETC func_80141468
Use a post-increment comparison on the brightness byte, then pass its stored new value to set_dec_bri; s32 with no explicit value return preserves the selector copy. Removed the unused m2c temporary and reverified. Resolved funcdiff and unit SHA1 pass. Second pass subtotal 15 functions / 4580 bytes. Updated [[matching-notes]] and [[tickets/T-9510-wave-8-list-1]]. Saved the retained body as the baseline for later probes in the same owned file.

## [2026-10-11] ingest | Wave 8 deep-pass record layout and bounded evidence
Typed NAME_ENT func_801454CC with NameRecord44 (stride 0x44, record 96 at
+0x1980); resolved function diff and NAME_ENT SHA1 still pass. Added four
deep-pass symptom rows and four historical resolutions to [[data/t0018-cases]]
after consulting [[register-allocation-wave7]] and running its inventory.
Updated [[matching-notes]] with corrected no-FAKE classifications, the score
buffer experiments, the actor loop, the conflicting void prototype, and the
duplicate-copy GameState-offset error. All failed production drafts have been
restored. Scoped duplicate rescan applied zero candidates. Final 15-function
diff sweep and 27-unit build are running for [[tickets/T-9510-wave-8-list-1]].

## [2026-10-11] build | Second-pass full verification and byte-size correction
All 15 new C functions pass full resolved funcdiff checks; sync_protos --check-branch and migrate_globals --check pass. Forced the 27 unit SHA1 checks and header/global checks through ninja; all pass. Current assembly metadata corrects two queue sizes by 12 bytes each, so the actual gain is 4,556 bytes (19 functions / 5,868 bytes across both passes). Grand progress 4753/6958, 735480/2279368 (32.3%). Recorded in [[tickets/T-9510-wave-8-list-1]] and [[matching-notes]]. Remaining BUNKA_SD experiments are isolated and have no production edits.

## [2026-10-11] ticket | T-9510 deep second pass reviewed and closed
Closed after over three hours and six minutes from 10:00:31 UTC. Inline review
against CODING_STANDARDS.md and the user/shared brief passed; cleanup, record
typing, incorrect no-FAKE notes and byte metadata findings resolved. Added 15
verified functions / 4,556 bytes (19 functions / 5,868 bytes across both passes).
All 15 resolved diffs, 27 unit SHA1 checks, prototype/global checks and diff
whitespace checks pass. Final incremental ninja has no work. BUNKA_SD's final
72 valid probes and 300-second search failed, with no production edits. Five
new deep-pass symptom rows (14 total) in [[data/t0018-cases]]. Larger remaining
type repairs stay deferred with assembly fallbacks; this is not an exhaustive
deep traversal of every historical rejected draft. [[tickets/T-9510-wave-8-list-1]],
[[kanban]] and [[index]] synchronized to Done. Verified edits are uncommitted.
Grand total 4753/6958 functions, 735480/2279368 bytes (32.3%).
## [2026-10-11] ticket | T-9560 Wave 8 list 6 started

Created [[tickets/T-9560-wave-8-list-6]] In Progress and its kanban card. Read the shared brief, coding standards, [[decompile-workflow]], [[register-allocation-wave7]] and matching history. Assigned 151 functions in 35 owned files. DATE func_8015A7E0 is a documented handwritten rotation helper and remains assembly; unresolved experiments will be restored to INCLUDE_ASM.

## [2026-10-11] build | T-9560 baseline verified

Fresh Docker configure/ninja build passes all 27 unit SHA1 checks and headers/globals checks before source changes. Generated m2c drafts in list order with unit-qualified names. Initial draft cleanup reconstructs byte displacements and typed record strides; baseline matching attempts now begin. Evidence: scratchpad/w8-6/baseline-build.log and scratchpad/w8-6/drafts.jsonl.

## [2026-10-11] query | T-9560 register inventory CLI

The brief's `tools/register_shapes.py <func>` syntax is not accepted by the current tool (argparse exits 2). Used its existing inventory API to classify the recorded rows without changing tooling. Before any new T-0018 rows, consult [[register-allocation-wave7]] and the resulting scratchpad/w8-6/shapes.json inventory.

## [2026-10-11] build | T-9560 baseline attempts 1-20

Processed assigned functions in list order through Docker m2c, aggregate migration, ninja and resolved funcdiff where the draft compiled. Results are recorded in [[tickets/T-9560-wave-8-list-6]]; rejected drafts were restored to INCLUDE_ASM. Scratch evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 scoped duplicate attempt rejected

Ran tools/dupes.py --apply --check with the exact 35 owned file paths. One EVENT copy candidate (func_8011B5A8) failed the unit verification and was reverted by the tool; zero functions retained. All 151 unit-qualified m2c drafts are now captured. Restoring the baseline build after the rejected copy's temporary header edits; subsequent bounded attempts build the actual object, run resolved funcdiff, and require the unit SHA1 before retaining a candidate.

## [2026-10-11] build | T-9560 baseline restored after dupes

Docker ninja -j4 passes the baseline again after the rejected duplicate candidate: all 27 unit SHA1 checks OK. No duplicate source or header changes retained. Focused variants now test a local GYOZI selector, corrected EVENT halfword updates, and reconstructed RPG_BAT reset loops before proceeding beyond the first 20 assigned functions.

## [2026-10-11] build | T-9560 focused GYOZI:func_80137F48

Tried 9 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused EVENT:func_80108A24

Tried 3 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused RPG_BAT:func_8014F540

Tried 3 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 short permuter EVENT:func_80108A24

Ran a 45-second single-worker permuter pass for the near miss. Restored the draft to INCLUDE_ASM pending candidate review. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 baseline attempts 21-40

Processed assigned functions in list order through Docker m2c, aggregate migration, ninja and resolved funcdiff where the draft compiled. Results are recorded in [[tickets/T-9560-wave-8-list-6]]; rejected drafts were restored to INCLUDE_ASM. Scratch evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused TAIIKU:func_8013384C

Tried 4 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 baseline attempts 41-60

Processed assigned functions in list order through Docker m2c, aggregate migration, ninja and resolved funcdiff where the draft compiled. Results are recorded in [[tickets/T-9560-wave-8-list-6]]; rejected drafts were restored to INCLUDE_ASM. Scratch evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused TAIIKU:func_8013384C

Tried 1 reconstructed C variants; result MATCH. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] decision | T-9560 TAIIKU promoted column local

Kept `func_8013384C` after resolved function MATCH and TAIIKU SHA1 verification. Both scalar locals precede the two table buffers, preserving the original home-slot layout. Plain C; no dummy padding. Idiom recorded in [[matching-notes]], evidence in [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ingest | T-9560 controlled GYOZI register evidence

Appended one GYOZI register-order row to [[data/t0018-cases]] after nine compiled variants and resolved real differences. Consulted [[register-allocation-wave7]] and the register-shapes inventory; no source retained. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused TEL:func_80139E3C

Tried 4 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 SHOUGATU switch verified

Kept `src/ovl/SHOUGATU/8013E330.c` switch function `func_8013EE30` after resolved MATCH and unit SHA1 success. Two retained functions now total 796 bytes; details in [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 short permuter TEL:func_80139E3C

Ran a 45-second single-worker permuter pass for the near miss. Restored the draft to INCLUDE_ASM pending candidate review. Evidence: scratchpad/w8-6/.

## [2026-10-11] ingest | T-9560 TEL selector partly resolved

Four typed TEL variants show that implicit s32 without return recovers the original selector register. Pointer temporary order remains different under both aggregate forms; a 45-second permuter pass finds no improvement. Added one controlled row to [[data/t0018-cases]] and restored the temporary table declaration and C. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 baseline attempts 61-90

Processed assigned functions in list order through Docker m2c, aggregate migration, ninja and resolved funcdiff where the draft compiled. Results are recorded in [[tickets/T-9560-wave-8-list-6]]; rejected drafts were restored to INCLUDE_ASM. Scratch evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 bounded near-miss permutations

Ran selected compiled near misses through 25-second single-worker permuter passes. Outputs are candidates pending review; source/header drafts restored. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 baseline attempts 91-151

Processed assigned functions in list order through Docker m2c, aggregate migration, ninja and resolved funcdiff where the draft compiled. Results are recorded in [[tickets/T-9560-wave-8-list-6]]; rejected drafts were restored to INCLUDE_ASM. Scratch evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused RPG_BAT:func_8014F540

Tried 4 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] query | T-9560 GPU packet type boundary

Checked the TACO sprite-builder near miss against the project SDK header. The 0x14-byte packet layout requires a missing SPRT type; this batch cannot extend that SDK header. Recorded the limitation in [[matching-notes]] rather than treating the raw byte-view draft as a controlled T-0018 case.

## [2026-10-11] build | T-9560 focused EVENT:func_8010B614

Tried 3 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused TACO:func_801423B0

Tried 2 reconstructed C variants; result MATCH. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 TACO record update matched

Kept `func_801423B0` after the direct byte-field increment and unmasked argument matched the resolved function and TACO SHA1. Three functions now total 1,044 bytes. Idiom in [[matching-notes]], verification in [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused EVENT:func_8011C470

Tried 4 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] lint | T-9560 retained source conventions

Named the new TAIIKU table field `unk_00` with its documented offset, and added explanations for the promoted-local layout and TACO byte reload. No matching trick or new FAKE was introduced. These source/header edits will be reverified in the final build; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused RPG_BAT:func_8014F540

Tried 2 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 focused EVENT:func_8011C470

Tried 2 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 typed EVENT callback view

Tested the known Rec34 table with its opaque 0x1A4-byte prefix and a shared low-bit flag view. Compared canonical and unprototyped callback declarations through sync_protos, real object diff and unit SHA1 where applicable. Result DIFF; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 bounded near-miss permutations

Ran selected compiled near misses through 25-second single-worker permuter passes. Outputs are candidates pending review; source/header drafts restored. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] query | T-9560 final register-gap evidence

Recorded three additional controlled cases in [[data/t0018-cases]], bringing this batch to five rows. Short permuter runs produced no score-zero candidate; all rejected sources and declarations remain restored. Results and caveat filed in [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ticket | T-9560 ready for inline review

Moved [[tickets/T-9560-wave-8-list-6]] to In Review and synchronized [[kanban]] and [[index]]. Audit confirms 151 assigned functions: three retained plain-C matches totaling 1,044 bytes and 148 restored assembly functions. Five rows added to [[data/t0018-cases]]. Final scoped duplicate pass and guards are running.

## [2026-10-11] build | T-9560 scoped duplicates and closing build

Exact owned-file labels passed to dupes.py --apply --check. Its sole candidate, EVENT `func_8011B5A8` from `func_8013FA6C`, failed unit verification and was automatically restored together with all temporary headers; no copies retained. Prototype branch and global checks passed. Invalidated only 27 SHA1 stamps and header/global guard stamps for the final incremental ninja; objects retained. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] lint | T-9560 inline source review

Reviewed the uncommitted diff against CODING_STANDARDS.md and the shared brief/list using the local code-review skill inline as required by the brief. Standards: zero source findings. Spec: zero source findings. Checklist and evidence recorded in [[tickets/T-9560-wave-8-list-6]]; final build validation still pending.

## [2026-10-11] query | T-9560 documented tooling discrepancy

Filed the shared brief's unsupported register_shapes.py positional function filter in [[matching-notes]]. Used the existing inventory API for historical cluster classification; no tooling changes made. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 final verification passed

Final incremental Docker ninja exited zero with 27/27 binary SHA1 checks OK, headers OK and globals OK. Post-duplicate prototype branch check passed with zero new disagreements (17 known). Three retained functions and the revised TAIIKU field name/comments are reverified. Inline Standards and Spec reviews have zero unresolved findings; results recorded in [[tickets/T-9560-wave-8-list-6]]. Final progress report pending.

## [2026-10-11] ticket | T-9560 complete

Moved [[tickets/T-9560-wave-8-list-6]] to Done after final verification and inline review; synchronized [[kanban]] and [[index]]. All 151 functions attempted, three plain-C matches (1,044 bytes), 148 restored assembly functions. Five T-0018 rows added. Final progress: 4737/6958 functions, 730656/2279368 bytes (32.1%). All 27 hashes and header/global/prototype guards pass; Standards 0 and Spec 0 findings. Results filed in [[matching-notes]]. No commit, merge or push.

## [2026-10-11] ticket | T-9560 reopened for deep second pass

User committed the first pass as 3a7461b and requested sustained source/assembly iteration rather than draft-only rejection. Reopened [[tickets/T-9560-wave-8-list-6]], synchronized [[kanban]] and [[index]]. Second pass starts 11:10:13 UTC and must continue through 14:10:13 UTC unless the list is exhausted. No commits; ownership and Docker constraints remain in force.

## [2026-10-11] query | T-9560 second-pass ranked queue

Ranked 148 rejected assigned functions using saved resolved instruction-line differences, retaining the best prior typed variant. Twenty-eight have compiled difference evidence; 43 contain aggregate/copy markers (broader than the user's 36 type-repair drafts). Closest candidates: main `func_80073AD8` (4 lines), RPG_BAT `func_80132FE0` (5), TACO `func_8013F874` (7; SDK type gap), TACO `func_80140638` (12). Start with main's four-line near miss. Evidence: scratchpad/w8-6/deep-queue.jsonl; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused main:func_80073AD8

Tried 1 reconstructed C variants; result MATCH. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] ingest | T-9560 assembly-led reconstruction fixes

Compared the two closest drafts with target assembly. Main `func_80073AD8` needs an explicit zero early result, and RPG_BAT `func_80132FE0` needs the third callback argument 0xAA. Findings filed in [[matching-notes]]; real function and unit verification recorded in [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused RPG_BAT:func_80132FE0

Tried 1 reconstructed C variants; result MATCH. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 first two deep-pass matches

Main `func_80073AD8` (472 bytes) and RPG_BAT `func_80132FE0` (296 bytes) pass resolved MATCH and unit SHA1. Retained plain C; second-pass gain 768 bytes. Findings filed in [[matching-notes]] and verification in [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 focused TACO:func_80140638

Tried 7 reconstructed C variants; result DIFF. Verified with Docker ninja and resolved funcdiff. Rejected variants and their temporary declarations restored. Evidence: scratchpad/w8-6/.

## [2026-10-11] build | T-9560 typed TACO actor-loop reconstruction

Compared `func_80140638` with target assembly and neighbouring TcActor users. Seven initial variants test pointer operand order, real fields, and rolled loop reconstruction; the typed unrolled form remains at 12 differing lines. Further experiments target indexed actor access, two counters, explicit byte-address arithmetic and statement layout. Existing funcdiff resolved APIs are reused between variants to avoid repeating the whole-repository header scan; final candidates still receive full CLI and unit SHA1 verification. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_80140638

Tried 9 source variants in 38.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_80140638

Tried 2 source variants in 28.6 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 actor-loop match

TACO `func_80140638` (412 bytes) passes full funcdiff --resolve and overlay SHA1 after typed actor indexing and count-placement experiments. Source-loop reconstruction remains uncertain; explicit groups/scheduling will be marked FAKE and reviewed. Second-pass gain is three functions, 1,180 bytes. Evidence filed in [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:join_club_select

Tried 16 source variants in 61.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:join_club_select

Tried 24 source variants in 127.2 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:join_club_select

Tried 10 source variants in 19.7 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 club selector reduced to five lines

Source/assembly experiments reduce `join_club_select` from 15 lines to five with a typed nibble store. Fifty variants cover selector/scaled-value types, pointers, control flow, callback views and statement grouping. Starting a 300-second permuter on the best draft. TACO's marked actor-loop source reverified with unit SHA1. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ingest | T-9560 deferred local-copy layouts

Recovered copy sizes and element types from target assembly and neighbouring declarations. SHOUGATU `func_8013E330` uses 28 callbacks (0x70 bytes); TAIIKU initialization uses 23 bytes and three 46-byte halfword arrays. Preparation filed in [[matching-notes]] while the club selector's five-minute permuter runs. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter main:join_club_select

Source-led near miss permuted with a 300-second limit and two workers (302.4 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] decision | T-9560 club selector deep timebox

Fifty source variants and a 300-second permuter produced no retained club-selector match. Best resolved draft improved 15 differing lines to five; source and temporary type/callback views restored. Filed result in [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]]. Continuing with main week_day_main0 result-type and frame experiments.

## [2026-10-11] build | T-9560 deep main:week_day_main0

Tried 0 source variants in 97.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] decision | T-9560 provisional main result view

sync_protos reports the intended week_day_main0 void-to-s32 definition change as RISKY and exits before the experiment harness compiles. Its source/header snapshot was restored. The user's deep-pass request explicitly authorizes type experiments; proceed with a scoped canonical result-view trial and retain it only after real function and whole-main SHA1 verification. No unrelated shared views will be accepted. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:week_day_main0

Tried 9 source variants in 156.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:week_day_main0

Tried 0 source variants in 63.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 8 source variants in 31.1 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] decision | T-9560 serialize prototype experiments

An overlapping SHOUGATU aggregate trial changed the inputs during a main return-type generation scan, so both runs are excluded from matching evidence and restored. Repeat sequentially; never overlap a prototype-generating trial with a declaration trial. The verified three second-pass matches remain intact. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:week_day_main0

Tried 16 source variants in 66.6 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] query | T-9560 switch-table reconstruction

Target RPG_BAT `func_8014E780` has an explicit selector-7/default shared entry absent from the first draft. Prepared case-7 and selector-width trials. Main weekday saved-local layout improved from 12 to eight differing lines; five-minute permutation underway. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_80154430

Tried 16 source variants in 56.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_80154430

Tried 14 source variants in 37.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter main:week_day_main0

Source-led near miss permuted with a 300-second limit and two workers (341.6 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_80154430

Tried 1 source variants in 60.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 12 source variants in 100.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_80154430

Tried 1 source variants in 69.3 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 8 source variants in 25.7 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 RPG_BAT selector match and PC16 evidence

`func_80154430` retained: 492 bytes, resolved funcdiff MATCH against a same-assembly scratch reference with local branch aliases, then RPG_BAT SHA1 OK. Native funcdiff omits PC16 relocation resolution; source tools untouched. Four second-pass matches total 1,672 bytes. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8014E780

Tried 6 source variants in 19.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 8 source variants in 20.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep BUNKA_SD:func_80138CAC

Tried 8 source variants in 15.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8014E780

Tried 1 source variants in 73.2 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 7 source variants in 69.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep BUNKA_SD:func_80138CAC

Tried 12 source variants in 30.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 12 source variants in 26.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 seventh switch entry matched

RPG_BAT `func_8014E780` retained after recovering its selector-7/default shared entry and local word declaration. Resolved comparison against same-assembly PC16 alias reference and unit SHA1 both pass. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ingest | T-9560 sprite aggregate recovered

Recovered the 0x14-byte TACO sprite primitive from allocation size and target member stores; prepared member-based reconstruction with branch-local palette stores. Two earlier type/control-flow near misses are in five-minute permutation runs. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013F874

Tried 8 source variants in 74.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter BUNKA_SD:func_80138CAC

Source-led near miss permuted with a 300-second limit and two workers (304.0 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter SHOUGATU:func_8013E330

Source-led near miss permuted with a 300-second limit and two workers (306.5 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E330

Tried 6 source variants in 44.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013F874

Tried 10 source variants in 197.6 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep BUNKA_SD:func_80138CAC

Tried 8 source variants in 81.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep GEKO:func_801403A0

Tried 4 source variants in 19.6 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 typed sprite matched

TACO `func_8013F874` verified by native resolved funcdiff MATCH and overlay SHA1. Retained the inferred 0x14-byte primitive layout; source-line scheduling dependency marked FAKE. Main weekday pointer trials now retry the selector lesson from RPG_BAT; no overlapping declaration-generation trials. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:week_day_main0

Tried 1 source variants in 47.7 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 weekday and GEKO matches

Main weekday and GEKO initialization retained after pointer-selector/declaration-order and narrow-status experiments. Both native resolved MATCH and unit SHA1 pass; main declaration generated by sync_protos. Eight additional functions, 3,044 bytes. Full incremental build checkpoint next. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ingest | T-9560 deep allocator evidence

Append three deep-attempt rows for main club selection, BUNKA_SD counter scheduling and SHOUGATU callback-table exit scheduling after the documented idioms and five-minute permutation runs. Record resolution of the historical GEKO row via a real narrow status local. See [[data/t0018-cases]], [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ingest | T-9560 record and flag views prepared

Prepared exact-size TAIIKU copied-array wrappers and a 0x44 record initialization view after checking neighbouring matched accesses. Prepared a local typed view of the existing main tail byte array for the parallel flag count, requiring no duplicate global declaration. Incremental checkpoint passed; scoped duplicate search and progress measurement underway. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TAIIKU:func_80132BD0

Tried 6 source variants in 20.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 10 source variants in 39.6 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005DDB4

Tried 8 source variants in 85.3 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TAIIKU:func_80132BD0

Tried 16 source variants in 14.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8011C470

Tried 12 source variants in 24.6 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 checkpoint 27/27

Forced only the 27 unit checksum stamps and ran incremental ninja: all 27 SHA1 checks pass. Scoped dupes attempted one EVENT copy, rejected it and restored it; scoped near-duplicate report has no fillable member. A concurrent progress read saw that temporary copy, so its extra 100 bytes are excluded until the final stable progress read. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005DDB4

Tried 1 source variants in 0.6 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TAIIKU:func_80132BD0

Tried 8 source variants in 13.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] query | T-9560 by-value constructor and paired indices

Read TACO constructor caller copies to establish the existing TcPos by-value parameter; retry member accesses and frame/local layouts. Main tail struct closes type repair and leaves only loop base-plus-offset spellings, so test independent array indices before permutation. TAIIKU tests conventional loop forms with explicitly captured early-load values. See [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005DDB4

Tried 8 source variants in 19.1 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 16 source variants in 10.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E50C

Tried 6 source variants in 10.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 8 source variants in 18.3 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8011C470

Tried 16 source variants in 22.1 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 16 source variants in 17.7 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E50C

Tried 24 source variants in 31.2 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter main:func_8005DDB4

Source-led near miss permuted with a 300-second limit and two workers (302.1 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 6 source variants in 23.6 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013E424

Tried 6 source variants in 26.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8011C470

Tried 12 source variants in 108.1 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8010B614

Tried 8 source variants in 23.2 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep SHOUGATU:func_8013E50C

Tried 24 source variants in 21.7 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005DDB4

Tried 7 source variants in 71.2 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8010B614

Tried 24 source variants in 18.3 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8010B614

Tried 12 source variants in 12.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter EVENT:func_8011C470

Source-led near miss permuted with a 300-second limit and two workers (306.2 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter TACO:func_8013E424

Source-led near miss permuted with a 300-second limit and two workers (306.7 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8014F540

Tried 16 source variants in 10.7 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8014F540

Tried 16 source variants in 47.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep EVENT:func_8010B614

Tried 24 source variants in 58.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TAIIKU:func_801330FC

Tried 8 source variants in 8.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 ninth second-pass match and restored deep trials

Main func_8005DDB4, 236 bytes, passes native resolved MATCH and main SHA1 after the five-minute permuter and index-expression experiments. Its no-op index mask is marked FAKE and reverified. Nine deep-pass matches total 3,280 bytes. TAIIKU aggregate-copy and TACO/EVENT ABI/scheduling trials were sustained and restored; see [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TAIIKU:func_801330FC

Tried 4 source variants in 8.6 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8014F540

Tried 2 source variants in 8.9 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 TAIIKU RECT and partial-unroll match

TAIIKU func_801330FC, 212 bytes, passes resolved MATCH and unit SHA1 with SDK RECT, eleven indexed stores and explicitly marked frame padding. Source annotation is being rechecked. Ten deep-pass matches total 3,492 bytes; see [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 RPG indexed clearing match

RPG_BAT func_8014F540, 156 bytes, passes native resolved MATCH and unit SHA1 after replacing pointer walks by indexed loops with one reused counter. Typed C8-byte range and missing overlay words appended; legacy scalar view and line scheduling marked FAKE and annotation rechecked. Eleven deep-pass matches total 3,648 bytes; see [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013DB80

Tried 8 source variants in 2.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013DB80

Tried 8 source variants in 15.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013DB80

Tried 8 source variants in 5.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep GYOZI:func_80137F48

Tried 32 source variants in 11.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013DB80

Tried 16 source variants in 8.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep GYOZI:func_80137F48

Tried 36 source variants in 8.4 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep TACO:func_8013DB80

Tried 36 source variants in 10.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep GYOZI:func_80137F48

Tried 48 source variants in 8.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep BUNKA_SD:func_80139100

Tried 5 source variants in 10.6 seconds; MATCH. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 BUNKA_SD timer reload match

BUNKA_SD func_80139100, 400 bytes, passes native resolved MATCH and unit SHA1. Restored the post-call reload and separate case stores; implicit s32 result recovers the exact constant and exit shape. Twelve deep-pass matches total 4,048 bytes; see [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:pre_syogatu1

Tried 8 source variants in 7.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter TACO:func_8013DB80

Source-led near miss permuted with a 300-second limit and two workers (301.0 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:pre_syogatu1

Tried 8 source variants in 6.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:pre_syogatu1

Tried 24 source variants in 10.1 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:pre_syogatu1

Tried 8 source variants in 13.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:pre_syogatu1

Tried 12 source variants in 16.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005F09C

Tried 18 source variants in 23.5 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter TACO:func_8013DB80

Source-led near miss permuted with a 300-second limit and two workers (301.8 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep ETC:func_80140AE4

Tried 20 source variants in 4.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep ETC:func_80140AE4

Tried 16 source variants in 14.8 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep ETC:func_80140AE4

Tried 16 source variants in 8.0 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter main:pre_syogatu1

Source-led near miss permuted with a 300-second limit and two workers (303.0 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005F09C

Tried 24 source variants in 13.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep main:func_8005F09C

Tried 12 source variants in 6.2 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8015A744

Tried 12 source variants in 4.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] query | T-9560 deep timebox classifications

Classified main pre_syogatu1 as pointer/base/CSE, main func_8005F09C as constant allocation/sharing and ETC func_80140AE4 as selector/compare chain through the existing register_shapes API before appending three rows in [[data/t0018-cases]]. Updated own [[matching-notes]] section with aggregate repair outcomes and K&R permuter limitation. Final scoped duplicate checks retained zero; sync_protos branch and global guards pass. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8015A744

Tried 18 source variants in 3.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8015A744

Tried 27 source variants in 4.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] decision | T-9560 inline second-pass review

Reviewed the uncommitted second-pass code against CODING_STANDARDS.md and the user/shared brief using the local code-review skill's Standards and Spec axes inline. Fixed point is committed first pass 3a7461b. Retained aggregate layouts, canonical declaration placement and matching-trick annotations checked; no unresolved code findings. Final verification is pending the last scratch permutation. Twenty-six distinct functions have recorded second-pass experiments; the user's three-hour condition, rather than list exhaustion, will bound this pass. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 five-minute permuter RPG_BAT:func_8015A744

Source-led near miss permuted with a 300-second limit and two workers (300.9 seconds including setup/verification). Draft restored; candidate evidence in scratchpad/w8-6/. See [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 deep RPG_BAT:func_8015A744

Tried 2 source variants in 1.9 seconds; DIFF. Resolved measurement through existing funcdiff APIs, final retained candidates verified by full CLI and unit SHA1. Evidence in scratchpad/w8-6/; see [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] build | T-9560 final function verification

All twelve retained second-pass functions pass fresh full funcdiff --resolve comparisons (4,048 bytes), using the documented equivalent scratch PC16 references for two RPG_BAT functions. Final RPG func_8015A744 candidate remains nonmatching after its five-minute permuter and fixed-width candidate retry; restored. Fresh all-27 unit checksum build is running, with header/global guards forced to rerun. Updated own [[matching-notes]] and [[tickets/T-9560-wave-8-list-6]].

## [2026-10-11] ticket | T-9560 second pass In Review

Moved [[tickets/T-9560-wave-8-list-6]] and its kanban card to In Review; index agrees. The requested three-hour deep-pass minimum is met. Changes remain uncommitted.

## [2026-10-11] ticket | T-9560 second pass Done

Moved [[tickets/T-9560-wave-8-list-6]] and its kanban card to Done; index agrees. The requested three-hour deep-pass minimum is met. Changes remain uncommitted.
## [2026-10-11] ticket | Claim wave 8 list 5

Created [[tickets/T-9550-wave-8-list-5]] In Progress and synchronized [[kanban]]. Read the shared wave8-brief.md and [[register-allocation-wave7]] before matching. Work is isolated to branch w8-5, with uncommitted edits for the orchestrator.

## [2026-10-11] build | Limit list-5 baseline concurrency

First configure generated build.ninja. Initial build overlapped the other wave-8 builds; stopped only this worktree's own container and restarted ninja with two jobs to limit shared-host contention. [[tickets/T-9550-wave-8-list-5]] remains In Progress.

## [2026-10-11] build | Wave-8 list-5 baseline and drafts

Main executable and completed overlay baseline checks pass. Captured list-order m2c drafts in scratchpad/w8-5/ and prepared array/member rewrites guided by [[data-types]] and [[game-state]]. [[tickets/T-9550-wave-8-list-5]] records this batch; no candidate is claimed matched yet.

## [2026-10-11] build | Wave-8 list-5 baseline passes

Docker configure and ninja -j 2 completed with all 27 unit SHA1 checks OK, build/headers.ok and build/globals.ok. Starting bounded list-order candidate verification for [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] query | List-5 reuse and aggregate evidence

Owned-file dupes.py and neardupes.py each applied zero copies. type_recovery.py found no aggregate proposal for D_800B6708, and a high-confidence unsigned-halfword array rooted at D_800B670A, stride 2, absorbing D_800B6710. Bounded C attempts will use this evidence. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] decision | First list-5 promotion time-box

Restored func_80067DD4 after a real instruction difference and bounded permuter run. Recorded its retained-conversion symptom in [[data/t0018-cases]] and [[matching-notes]]. Also recorded the register_shapes.py positional-argument discrepancy for the orchestrator. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] decision | List-5 flag-update sibling

func_80067DFC repeats the retained-conversion difference of func_80067DD4. Its bounded permuter did not match (best 105), so restored assembly and appended evidence to [[data/t0018-cases]]. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | First 30 list-5 attempts restored

Recorded first 30 bounded attempts in scratchpad/w8-5/results-1.jsonl; no C candidates survived this pass. Corrected draft preparation for union-field aliases by retaining their temporary inferred declarations during migrate_globals.py, as required by [[decompile-workflow]]. Retesting the affected drafts and near misses before continuing the remaining list. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] decision | List-5 first verified C matches

[[tickets/T-9550-wave-8-list-5]] keeps DATE `func_801572E4` (268 bytes) and TAIIKU `func_80143D24` (172 bytes), each verified by Docker resolved function diff. [[matching-notes]] records the common-table load ordering and overlapping coordinate views. Other corrected retries remain time-boxed and restored; batch checksums are running.

## [2026-10-11] build | Corrected list-5 retry batch passes

Docker ninja completed successfully with all 27 checksum checks after the ten corrected retries. Two function matches (440 bytes) remain; all failed C and temporary API declarations were restored. [[tickets/T-9550-wave-8-list-5]] continues with list positions 31–152, using typed byte-pointer fields and bounded variants.

## [2026-10-11] query | List-5 data-island evidence

Consulted `config/objects/main.txt`, `config/objects/TAIIKU.txt`, and TAIIKU's generated data piece. The existing island places `D_8014A1D0` in `src/ovl/TAIIKU/80142240.c` and its four bytes are zero. [[matching-notes]] records a bounded C-definition retry for `func_8014430C`; no tooling or raw source edits. [[tickets/T-9550-wave-8-list-5]] has completed attempts for list positions 31–50 and is checking their restored outputs.

## [2026-10-11] decision | Reuse Rec24 field layout in list-5 drafts

Compared `cal_sprite_init`'s scalar addresses with the documented 0x24-byte `Rec24` layout in `include/main_api.h`. Prepared field-based candidates for this table rather than new globals, retaining EVENT's separate data interpretation. [[matching-notes]] records the boundary evidence; [[tickets/T-9550-wave-8-list-5]] continues its bounded list sweep.

## [2026-10-11] build | List-5 positions 31–50 restored and verified

All twenty functions were attempted or time-boxed; failed drafts and temporary declarations were restored. Docker ninja passes 27 checksums. The GYOZI `func_8013E638` diff hoists the scalar `D_800F62CF`; the existing `GyoziWork.unk_F2F` member documents the correct T-9210 reload view, prepared for subsequent drafts and a bounded retry. [[tickets/T-9550-wave-8-list-5]] continues at position 51.

## [2026-10-11] query | List-5 SDK argument evidence

Read `asm/libgs.s` and the callers in the owned `80058D20` object. `initLight` explicitly clears a2 for `GsSetAmbient`; `func_8009B430` uses a0–a2 as inputs. [[matching-notes]] records a third-argument fix and a three-argument file-view retry under standards 8a; retention requires real function and unit checks. [[tickets/T-9550-wave-8-list-5]] continues the list sweep.

## [2026-10-11] build | List-5 positions 51–70 verified

All twenty entries reached a bounded result; no additional matches retained. Restored sources and header views pass all 27 checksums in Docker. [[tickets/T-9550-wave-8-list-5]] continues with positions 71–90. Scratch retries now include real Rec24 fields, GyoziWork index aliases, SDK argument corrections, named CharFlags bit tests and generated prototype guards.

## [2026-10-11] build | List-5 positions 71–90 verified

Twenty more entries were attempted or time-boxed, with no new C retained. Docker ninja passes all 27 checksums after restoring them. [[tickets/T-9550-wave-8-list-5]] continues at position 91. Full checksum rebuilds are reserved for retained matches (still within the brief's 5–10-match interval) and final verification; each candidate continues to get a real fresh-object function diff.

## [2026-10-11] decision | List-5 positions 91–110 time-boxed

Twenty entries reached bounded compiler/diff or draft-shape results; none added matching C. Source and declaration snapshots were restored. [[tickets/T-9550-wave-8-list-5]] proceeds to 111–130, then the remaining entries and the concrete type/argument/frame retries recorded in [[matching-notes]]. The last retained-match tree remains the two verified functions (440 bytes).

## [2026-10-11] decision | List-5 positions 111–130 time-boxed

All twenty entries reached bounded results and were restored; no additional C retained. [[tickets/T-9550-wave-8-list-5]] now sweeps the final 22 entries. Added a table heading for this ticket's new [[data/t0018-cases]] rows, leaving historical rows unchanged.

## [2026-10-11] decision | List-5 full sweep completed

All 152 listed functions reached a bounded compiler/diff result or draft-shape time-box; failed C and temporary declaration snapshots were restored. [[tickets/T-9550-wave-8-list-5]] now tests thirteen prepared retries with concrete type, SDK-arity, local-layout or implicit-int hypotheses. Complete per-function evidence remains in scratchpad/w8-5/results-*.jsonl; no raw sources or tooling files changed.

## [2026-10-11] decision | List-5 aggregate and SDK-arity matches

Resolved Docker function diffs additionally match `cal_sprite_init` (828 bytes) and `initLight` (272 bytes). Four functions, 1,540 bytes, are retained. The completeness audit covers all 152 entries; [[matching-notes]] records successes and the bounded member/data-island/model-view failures. [[tickets/T-9550-wave-8-list-5]] proceeds to final local-allocation retries and validation.

## [2026-10-11] build | List-5 four-match batch verified

Docker ninja -j 2 completed all 27 checksum checks, headers and globals. The final two local-allocation retries failed resolved function diffs and were restored. [[matching-notes]] records the outcomes; [[data/t0018-cases]] now has fourteen appended T-9550 cases.

## [2026-10-11] ticket | List-5 enters inline review

Moved [[tickets/T-9550-wave-8-list-5]] to In Review with matching [[kanban]] and [[index]] entries. Standards review of the uncommitted HEAD diff found undocumented incomplete aggregate views; marked DATE, TAIIKU and light scalar views FAKE and tracked the reasons. Spec review accounts for 152/152 entries and confirms allowed-file isolation. Final Docker guards, checksums and owned-file reuse scan remain pending.

## [2026-10-11] query | List-5 final owned-file reuse audit

Ran Docker dupes.py --apply --check with the exact 36 owned C paths. Applied zero copies; no additional source or declaration changes. [[tickets/T-9550-wave-8-list-5]] now runs final branch/global guards, fresh diffs for all four retained functions, incremental ninja and explicit 27-binary SHA1 verification.

## [2026-10-11] build | List-5 final verification passes

Docker branch prototypes report zero new disagreements (17 known), GameState migration audit passes, and all four fresh resolved function diffs MATCH after FAKE-comment edits. Incremental ninja exits zero; all 27 unit checksums and explicit final SHA1 verification pass with header/global stamps. Progress: grand total 4738/6958 functions, 731152/2279368 bytes (32.1%); SDK 722 functions excluded. Full evidence is scratchpad/w8-5/final-validation.log. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] ticket | List-5 reviewed and Done

Inline code-review skill against HEAD completed along standards and shared-brief spec axes. Three missing FAKE-documentation findings resolved, zero outstanding on either axis. Moved [[tickets/T-9550-wave-8-list-5]] to Done with [[kanban]] and [[index]] synchronized. All 152 entries attempted/time-boxed; four matches (1,540 bytes), fourteen [[data/t0018-cases]] rows. Changes stay uncommitted for the orchestrator; no tools, raw sources or Git state modified.

## [2026-10-11] ticket | Reopen list-5 for deep second pass

User committed the first pass as df5bc9f and requested sustained per-function experiments rather than first-mismatch rejection. Reopened [[tickets/T-9550-wave-8-list-5]] and synchronized [[kanban]] and [[index]]. Starting at 11:08:23 UTC, spend at least three hours or exhaust the remaining list; rank resolved diff sizes, compare target asm, iterate types/control flow/source order, run five-minute near-miss permutations, and retain only function-plus-unit verified matches. Aggregate/local-copy repairs follow the closest candidates.

## [2026-10-11] query | Rank rejected list-5 drafts for deep experiments

Docker parsed all first-pass result files and ranked each remaining function's best resolved diff. [[matching-notes]] records the nearest nine functions and separates compile/type failures. Starting with DATE `func_80140D38`: target code differs only by a hoisted address load in the best draft. Prepared ten control-flow/byte-local/return-view candidates; its existing object data island also contains a four-byte zero piece at D_8015B6A0, available for a bounded ownership retry. [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep list-5 first match

DATE `func_80140D38` (124 bytes) passes fresh resolved funcdiff and DATE SHA1. The `s32` fallthrough return view removes the address-load scheduling difference; byte-local alternatives and a C definition of the zero data piece failed to improve the nearest void draft. [[matching-notes]] records the successful T-9000 idiom and restored data experiment. [[tickets/T-9550-wave-8-list-5]] proceeds to the halfword swap with signed/unsigned temporary widths, declaration/load orders, line grouping and both return views.

## [2026-10-11] build | Deep halfword swap matched

Main `func_80086640` (60 bytes) passes resolved funcdiff and main SHA1 after 216 explicit variants and a five-minute-bound permutation search. Its same-line swap/pointer-alias shape is marked FAKE and recorded in [[matching-notes]] and the append-only resolution note in [[data/t0018-cases]]. [[tickets/T-9550-wave-8-list-5]] has two deep-pass gains (184 bytes); now testing flag-update return/local forms with generated canonical API views, correcting the first pass's incompatible void prototype experiment.

## [2026-10-11] query | Deep flag and threshold type experiments

Main `func_80067DD4` narrowed from seven to four resolved differing lines through promoted-byte assignment forms and corrected canonical return views; running its full 300-second permutation search. Identified four DATE2 local-copy tables as 30/51/39/81 signed halfwords from copy sizes and loop bounds. Prepared a documented 60-byte aggregate retry for the first walker, including correct word string-pointer loads and declaration/frame variants. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep promoted-byte flag matched

Main `func_80067DD4` (40 bytes) passes real resolved funcdiff and main SHA1 after explicit return/expression/local-width experiments and a 268-second permutation/verification run. A signed-halfword stage followed by a byte alias retains the target's final ANDI and byte store. Pruning redundant permutation syntax and applying the proven family shape to its sibling next. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep sibling flag and syntax cleanup verified

Removed redundant nested assignment and XOR-zero from `func_80067DD4`; resolved diff and main SHA1 still pass. Adapted the remaining promoted-halfword/byte-alias idiom to `func_80067DFC`; fresh resolved diff and main SHA1 pass (40-byte function plus original padding). Four unique deep-pass matches, 264 function bytes; starting DATE `func_80136F2C` array-base ordering experiments. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] query | Aggregate retries grounded in neighboring source

DATE2's already matched `func_80138048` copies a 13-entry callable-word struct and declares its scalar index before the table. GYOZI `func_80142D40` has the same aggregate copy pattern over 27 words; prepared a typed retry with both scalar/table declaration orders and observed frame offsets. RPG_BAT `func_801564F0` target supplies a third call argument (0x82) missing from its draft; prepared corrected call and direct byte-symbol variants. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep DATE array loop time-boxed

`func_80136F2C` remains an eight-line t0/t1 array-base swap after 28 valid explicit variants and full 300-second permutation, about 13 minutes total. Restored all source experiments; integer-return declaration conflicts excluded from codegen conclusions. Canonical API layout for the two matched main flags regenerated with tools/sync_protos from baseline declaration order (minimal type/addition diff). Continuing with the next ranked DATE function. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep DATE branch function narrowed to six lines

`func_8013E3C4` now has the original saved-state stack offset with an extra four-byte local declared first (FAKE). Seven minutes of integer/narrow local widths, declaration orders, equality operands and return views leave six lines: comparison operand order and a scheduled constant. Testing explicit else-if/shared-exit flow and source grouping, then a full five-minute permutation. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] query | Owned-file duplicate scan after deep flag matches

Read-only Docker dupes.py scan restricted to the 36 list-5 paths reports zero plans and one skipped copy. Its reported matched count is based on generated asm directories and predates the new C definitions; rerun the prescribed scoped apply/check after further matches. DATE branch-flow tests did not improve six differing lines; now running the full 300-second permutation. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | T-9550 deep DATE control-flow timeout and aggregate retry

[[tickets/T-9550-wave-8-list-5]]: restored DATE func_8013E3C4 after explicit return/frame/control-flow experiments and full 300-second permutation; closest resolved diff has six differing lines. [[matching-notes]] records the result. DATE2 threshold struct-copy repair compiles; inspecting exact frame and narrow-argument differences before further bounded experiments.

## [2026-10-11] build | T-9550 first aggregate-copy match

DATE2 func_80138E0C matches 384 bytes after signed-halfword aggregate, declaration/frame placement and redundant narrow-conversion experiments. Fresh funcdiff --resolve and full DATE2 SHA1 pass. [[matching-notes]] records the genuine copy type and the two marked FAKE details; [[tickets/T-9550-wave-8-list-5]] has the verification result. Deep pass now has five unique gains, 648 bytes.

## [2026-10-11] build | T-9550 DATE2 nested-expression copy match

DATE2 func_80138F8C matches 532 bytes after aggregate length/tail correction and named-local versus expression-temporary experiments. Real resolved diff and overlay SHA1 pass. [[matching-notes]] records the stack spill and multiplication finding; [[tickets/T-9550-wave-8-list-5]] records the gain. Six unique deep matches, 1,180 bytes.

## [2026-10-11] build | T-9550 DATE2 39-entry sibling match

The typed sibling expression immediately matches DATE2 func_801391A0 (532 bytes); real resolved diff and DATE2 checksum pass. [[matching-notes]] and [[tickets/T-9550-wave-8-list-5]] updated. Seven unique second-pass gains, 1,712 bytes; checking the 81-entry sibling next.

## [2026-10-11] build | T-9550 DATE2 threshold family complete

DATE2 func_801393B4 (532 bytes) completes the four repaired aggregate-copy walkers. [[matching-notes]] records their true copy extents, expression temporaries and marked frame/conversion details. Eight unique deep gains total 2,244 bytes. Full 27-unit/check-guard checkpoint and scoped owned-file duplicate application are running.

## [2026-10-11] build | T-9550 eight-match checkpoint passes

Docker ninja and explicit SHA1 verification pass all 27 binaries; header/global guards pass. Corrected the scratch duplicate invocation to its documented comma-separated --files argument; scoped --apply --check applies zero copies. Evidence: scratchpad/w8-5/deep-checkpoint-retry.log. [[tickets/T-9550-wave-8-list-5]] remains In Progress; ranked GEKO scheduling experiments resume.

## [2026-10-11] build | T-9550 GEKO argument-store scheduling match

GEKO func_8013CC20 matches 64 bytes via scalar-byte member store followed by plain member read as argument. Fresh funcdiff --resolve and GEKO SHA1 pass. Specific FAKE reason added; final verification will follow comments. [[matching-notes]] records the idiom. Nine unique deep gains, 2,308 bytes; repairing the ranked RPG_BAT call's missing third argument next.

## [2026-10-11] build | T-9550 RPG_BAT aggregate flag match

RPG_BAT func_801564F0 matches 620 bytes after correcting the missing call argument and using one typed record-table view for all three flag bytes. Fresh resolved diff and whole overlay SHA1 pass; no new scalar main API declarations retained. [[matching-notes]] records the aliasing evidence and marked remaining view. Ten unique deep gains total 2,928 bytes.

## [2026-10-11] build | T-9550 GEKO colour-loop match

GEKO func_80139C70 matches 228 bytes through a genuine unsigned byte local and correctly ordered assignment chain. Fresh real diff and GEKO SHA1 pass. [[matching-notes]] records the fix. Eleven unique deep gains, 3,156 bytes; bounded model-linker declaration/control-flow experiments next.

## [2026-10-11] build | T-9550 model-linker deep timeout

Restored initModelingData_init and its generated guarded three-argument SDK view after about fifteen minutes, explicit copy/ordering variants and two full five-minute permutations. Closest whole-file resolved comparison has two differing lines; no candidate retained. [[matching-notes]] records the experiment. Correcting the next ranked DATE helper's old-value postincrement comparison.

## [2026-10-11] query | Deep DATE counter and TACO aggregate evidence

DATE func_8013E6A4 compares the old counter with one before storing its increment; corrected 48 explicit return/local/byte-view variants reach twelve differing lines, solely counter/condition registers. Full 300-second permutation is running. TACO func_8013D1E0 homes all four argument registers and copies two eight-byte values with unaligned stores: the existing TcPos type fits both by-value arguments. Neighboring particle arrays have sixteen 0x14-byte entries with two TcPos members and four byte fields. Typed retries prepared; no SDK types invented. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep DATE old-counter timeout

Restored DATE func_8013E6A4 after approximately fifteen minutes of corrected control flow, local/argument allocation and full 300-second permutation. Closest resolved diff stays twelve differing lines; weighted permutation score improved 85 to 50. [[matching-notes]] records the limits. Starting the typed TACO by-value-copy retry under [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep TACO by-value aggregate match

TACO func_8013D1E0 matches 192 bytes after recovering two TcPos arguments and a 0x14-byte particle record. The integer fallthrough-return idiom resolves the last pointer/value register swap; fresh real diff and TACO checksum pass. [[matching-notes]] records the ABI evidence; [[tickets/T-9550-wave-8-list-5]] records the gate. Twelve unique deep gains, 3,348 bytes.

## [2026-10-11] build | Deep typed TACO loop reaches one instruction

Forty-eight typed counter/postincrement/layout variants bring func_8013D348 within one missing move-to-zero at loop entry. Eighteen explicit returned-value variants add saved-register work and do not improve it. A full 300-second permutation and initialization/return-shape probes follow; no failed loop body or declaration is retained. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep TACO loop timeout

Restored func_8013D348 and its unsuccessful header additions after approximately ten minutes, including full 300-second permutation and explicit initialization/induction/result variants. One entry initialization remains missing; recorded as a source/return-shape gap rather than a proved T-0018 case. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep TACO initializer sibling match

func_8013D2A0 matches 168 bytes immediately with the recovered particle aggregate, promoted narrow parameters and marked integer fallthrough idiom. Fresh resolved diff and whole TACO SHA1 pass. Thirteen unique deep gains, 3,516 bytes; the larger particle-update sibling follows. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep TACO update permutation narrows existing row

The recovered func_8013D3F4 member view reaches eighteen differing resolved lines: one moved pointer addition and condition registers. Full 300-second permutation improves weighted score 85 to 55 without matching, the same remaining shape recorded by T-9330 at the historical row. Local-width/return-type and pointer-initialization placement experiments are the final retry. Also removed obsolete flag comments and corrected two declaration indents; final real diff/checksum gates will cover these review edits. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep TACO update timeout and grid-copy retry

Restored func_8013D3F4 after approximately ten minutes, explicit allocation variants and full 300-second permutation. Whole-file insertion of the best candidate fixes the pointer order but leaves sixteen register-only differing lines; the earlier T-9330 row is preserved without duplication. Added the missing precise FAKE explanation to the matched DATE palette helper. Starting BUNKAKEN's recovered 4-by-4 grid copy. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep BUNKAKEN grid reaches final address register

The 64-byte aggregate copy and all frame offsets match. Inlining row and column expressions removes the temporary-register chain, reducing twenty-eight differing lines to four: final stack address in v0 rather than v1. Twenty further return/expression/pointer variants do not improve it; full 300-second permutation follows. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep BUNKAKEN grid timeout

Restored the grid function and declarations after approximately ten minutes: copy/frame/type repair, indexed expression rewrites, return joins and full 300-second permutation. Closest explicit view remains four differing lines; [[matching-notes]] records the bound. DATE2's three parameter-pair aggregate copies are the next repair under [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Correction: final BUNKAKEN candidate matched

The immediately preceding timeout entry misread the final scratch result. The first whole-file permutation-candidate retry actually MATCHED func_80152F4C (248 bytes), with fresh resolved diff and whole BUNKAKEN SHA1 pass. The signed returned local preserves the target's final address/value allocation. Source and grid aggregate are retained; [[matching-notes]] corrected. Fourteen unique deep gains, 3,764 bytes; [[tickets/T-9550-wave-8-list-5]] contains the successful gates.

## [2026-10-11] build | Deep fourteen-match checkpoint and swap pruning

The full Docker build and explicit SHA1 pass all 27 binaries, with header/global guards and owned-file dupes --apply --check (zero plans). Scratch evidence: deep-checkpoint-14.log. The matched main swap also passes real diff and main SHA1 after removing do/while(0), removing its redundant pointer alias and naming the locals. DATE2 func_80138A90's three recovered copies agree except for local offsets; padding types/placement are being tested. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep DATE2 parameter-copy match

func_80138A90 matches 348 bytes after recovering three word-pair aggregates and declaring one byte of padding before them. Fresh resolved diff and DATE2 checksum pass. Fifteen unique deep gains, 4,112 bytes. Updated the main swap's final FAKE explanation and removed trailing whitespace; final gates will cover those comment/format edits. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep main position helper match

func_8006C700 matches 96 bytes after 96 explicit width/order/expression variants, bit-extraction inlining and direct struct-member indexing. Generated main API unsigned halfword table declaration retained; fresh resolved diff and main SHA1 pass. Sixteen unique second-pass gains, 4,208 bytes (twenty batch gains, 5,748 bytes including the committed first pass). One further 0x44-byte aggregate flag-loop retry precedes final review. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep final candidate restored and review cleanup verified

BUNKA_SD func_80134850 remains 51 differing lines after approximately twelve minutes of record alignment, loop extent/control-flow and counter/quotient experiments; source and declarations restored. The second pass ran from 11:08:23 UTC past 14:14 UTC, satisfying the three-hour requirement. Main position cleanup removes unused locals and uses an unsigned left shift without changing bytes. BUNKAKEN's unused word demonstrably fixes the table offset; renamed to pad and documented. Both cleanup candidates pass fresh resolved diffs and unit SHA1. Added historical-row resolution prose, with zero new T-0018 rows. Full final verification follows. [[matching-notes]], [[data/t0018-cases]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] build | Deep final checks and inline review pass

All twenty retained functions pass fresh funcdiff --resolve; sixteen are new in this pass, totaling 4,208 bytes (twenty/5,748 bytes across both passes). New TACO fields renamed to unk_XX during standards review, then both affected functions and the full build rechecked. Docker ninja, explicit 27/27 SHA1 checks, header/global guards and prototype/global checks pass; zero new prototype disagreements (17 known), zero scoped duplicate copies. Final progress: grand total 4754/6958 functions, 735360/2279368 bytes (32.3%). Evidence in scratchpad/w8-5/deep-final-validation.log and deep-final-taco-review.log. [[matching-notes]], [[tickets/T-9550-wave-8-list-5]].

## [2026-10-11] ticket | T-9550 deep pass Done

Inline standards/spec review against df5bc9f resolves all findings; permitted-path audit and git diff --check pass. The sustained deep pass exceeds three hours and stops on the duration condition, with larger type repairs remaining documented rather than claimed exhausted. Sixteen additional matches retained, every failed candidate restored, no new T-0018 rows. Updated ticket, index and kanban to Done. No Git mutation; changes stay uncommitted. [[tickets/T-9550-wave-8-list-5]], [[matching-notes]], [[kanban]].

## [2026-10-11] lint | T-9550 final wiki consistency

Ticket frontmatter, index and the single kanban card agree on Done; final acceptance checks are checked and review history remains appended after the matching entries. git diff --check passes. The card returns to its committed Done position, so wiki/kanban.md has no net second-pass diff. [[tickets/T-9550-wave-8-list-5]], [[kanban]], [[matching-notes]].

## [2026-10-11] ticket | Start wave-8 tool fixes

Created [[tickets/T-9600-wave-8-tool-fixes]] and claimed its board card. Read shared tooling8 brief, standards and relevant wiki reports. Negative-SP case is SHOUGATU func_80140314 (detailed in [[tickets/T-9580-wave-8-list-8]]); InitMouse exposes the m2c table-store issue; the clone report maps GYOZI func_80137D70 to SHOUGATU func_80136770.

## [2026-10-11] decision | Wave-8 tool regression fixes and upstream workaround

Implemented the seven reports under [[tickets/T-9600-wave-8-tool-fixes]] with synthetic regressions. [[matching-notes]] records the exact frame rule, copier effective-address mapping, relocation semantics and upstream InitMouse workaround; [[toolchain]] and [[register-allocation-wave7]] updated. Baseline Docker build passed and progress is 4868/6958, 776304 bytes. Reported RPG_BAT PC16 functions, TACO string function and strNextVlc pass fresh resolved diffs with the fixes. Clean final build and review pending.

## [2026-10-11] ticket | Wave-8 fixes in review

Moved [[tickets/T-9600-wave-8-tool-fixes]] to In Review. First full Docker suite passed 584 tests, clean build passed 27 SHA1 checks, stamps exist, branch guard passes, and progress remains 4868/6958. Inline standards/spec review tightened negative-offset tracking to variable-index bases, rejecting plain sp copies, and added cast-width and 102-seed mutation regressions. Final verification follows these refinements; no source/header edits or function gains.

## [2026-10-11] build | Wave-8 fixes final proof and review

[[tickets/T-9600-wave-8-tool-fixes]] final Docker suite: 587 tests pass. Final clean configure/ninja: 27/27 SHA1 OK, headers/global stamps present, branch guard passes (zero new, 17 known), progress unchanged at 4868/6958 and 776304 bytes. Reported relocation functions now compare normally; read-only clone replay selects GameState.unk_110A. Inline standards/spec review has zero unresolved findings. No source/header edits or function gains; changes left uncommitted for orchestrator review/commit.

## [2026-10-11] ticket | Complete T-9600 and lint its wiki links

Moved [[tickets/T-9600-wave-8-tool-fixes]] from In Review to Done after final proof/review. Ticket, board and index agree. [[matching-notes]], [[toolchain]] and [[register-allocation-wave7]] document the rules and upstream m2c workaround; historical wave observations are retained and superseded by the T-9600 section. Scoped wiki lint: no missing links or contradictory current ticket status. git diff --check passes.
## [2026-10-11] ticket | T-9610 aggregate and local-copy types
Claimed [[tickets/T-9610-aggregate-and-local-copy-types]], created its board card and reviewed the shared tooling-round brief. Investigate wave-8 rejected aggregate drafts, shared SDK layouts and local frame rules; leave edits uncommitted for integration.

## [2026-10-11] ingest | T-9610 recurring shapes and shared SDK layouts
Recorded wave-8 copy/stride/packet families in [[data-types]]. Added fresh fixed-width packet and geometry declarations; moved EN_NICHI's game record into its overlay header and replaced three private SDK packet views. Baseline Docker progress is 4868/6958, 776304 matched bytes. Docker auto-detection stalled; selecting the existing linux/arm64 image explicitly works. Layout and byte verification next.

## [2026-10-11] build | T-9610 packet ABI and local-frame probes
IDO assertions pass for SDK aggregate sizes and offsets (`tools/test_psx_types.py`). EN_NICHI and TACO packet-type replacements preserve their unit checksums. Synthetic raw-vs-transformed IDO probes establish the local declaration/alignment examples now in [[decompile-workflow]]. Typed MATRIX and TAIIKU table candidates compile but still differ; no gains claimed yet.

## [2026-10-11] build | T-9610 verified callback copies and MATRIX migration
DATE func_80153CEC and GEKO func_80141760 pass resolved diff and overlay checksums (+2 functions, 356 bytes). TACO func_8015B3B0 now uses MATRIX with matching operand order. [[matching-notes]] records rejected candidates, including SHUGAKU's isolated MATCH that regresses a previously matched function; its body is restored. New ABI/frame tests pass in Docker. No existing Python tool or compiler pass was modified.

## [2026-10-11] ticket | T-9610 moves to In Review
Updated [[tickets/T-9610-aggregate-and-local-copy-types]] and [[kanban]]. Inline standards/spec review uses HEAD as the baseline and the shared brief as the acceptance gate. Begin clean 27-unit rebuild and final guards.

## [2026-10-11] lint | T-9610 inline review corrections

Reviewed the change against HEAD, CODING_STANDARDS.md and the shared brief. The first clean build passes all 27 checksums. Corrected DR_TPAGE to eight bytes after finding a discrepancy in the PsyQ 4.7 reference: independent symbol metadata records one command word. Updated the ABI assertion and [[data-types]] evidence, and removed an unused test import. Rejected candidates remain assembly; no compiler pass or per-function switch was added. Final-state clean proof follows.

## [2026-10-11] build | T-9610 final clean proof

Final-state Docker configure/ninja passes all 27 checksums; build/headers.ok and build/globals.ok exist. Six changed functions pass resolved funcdiff, including DATE func_80153CEC and GEKO func_80141760 (new matches, 356 bytes combined). ABI/local-frame/frame-pass tests pass (1/2/20); prototype branch guard reports zero new disagreements and 17 known. Progress is 4870/6958 functions and 776660/2279368 bytes. Proof logs: scratchpad/r8-types/clean-build-final.log and scratchpad/r8-types/final-checks.log. See [[matching-notes]] and [[decompile-workflow]].

## [2026-10-11] ticket | T-9610 In Review to Done

Inline standards and spec review against HEAD 1e24c0394a695c64200126de3beff00e579f930a has no unresolved findings after correcting DR_TPAGE. Updated [[tickets/T-9610-aggregate-and-local-copy-types]], [[kanban]] and [[index]]. Existing Python tools and compiler passes remain untouched; two new regression test scripts accompany the layouts and measured rules. All edits are uncommitted for orchestrator integration. Rejected bodies remain assembly; [[matching-notes]] records their residual blockers.
