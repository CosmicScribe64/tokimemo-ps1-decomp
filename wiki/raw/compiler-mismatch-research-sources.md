# Raw source note: compiler mismatch handling (T-0015)

Collected 2026-10-09 by reading web pages and source only. No binaries, OS images or archives were downloaded. Every claim has a URL. "UNVERIFIED" marks claims I could not trace to a primary source. GitHub facts come from the raw files and api.github.com listings of the named repos at their default branch on that date.

## 1. Compilers used by early Japanese PS1 developers

### Sony NEWS workstations and the SN Systems PC kit
- Wikipedia's SN Systems article says Sony had provided MIPS workstations (it says R4000-based, which conflicts with other sources saying R3000) for PS1 development, Psygnosis disliked the cost and asked SN Systems for a PC-based system, and at the January 1994 Winter CES Sony's Japanese executives chose SN Systems' tools over their own workstation plan. It cites Next Generation issue 6 (June 1995) pp. 49-50. https://en.wikipedia.org/wiki/SN_Systems (secondary; the magazine itself was not read, UNVERIFIED).
- Wikipedia's Sony NEWS article says early PlayStation dev kits were based on NEWS hardware, citing Retro Reversing. https://en.wikipedia.org/wiki/Sony_NEWS
- Retro Reversing says the prototype MW.3 kit resembles the NEWS line and "may" be the same machine with PlayStation hardware added; it also says SCE Japan worked with SN Systems on the PC kit. It says nothing about which compiler ran on the NEWS. https://www.retroreversing.com/official-playStation-devkit
- NEWS-OS history: 4.x (1990) shipped as CISC (68k) and RISC (MIPS R3000) variants; 5.x/6.x are SVR4.2 based. https://en.wikipedia.org/wiki/NEWS-OS and Watanabe's release list http://katsu.watanabe.name/doc/sonynews/newsos.html (lists "NEWS-OS Release 4.0R ... R3000").
- NEWS-OS 4.2.1R `cc(1)`: "the RISC NEWS ucode C compiler", output MIPS extended COFF; passes named in its `-t` table include cpp, cfe-like front end, ujoin, uld, usplit, umerge, uopt, ugen, as0, as1, ld; supports `-EB`/`-EL`, `-mips1` (default), `-O0/-O1/-O2/-O3`, `-G num` (default 8), `-framepointer`, `-trapuv`, `-Olimit`, `-Wc,...` pass options. https://typewritten.org/Manual/Sony/NEWS-os/4.2.1R/en_US/man1/cc.html
  - This is the same cfe/uopt/ugen/as1 family as IDO. The man page does not mention `-non_shared`, `-Wo,-no_const_in_reg` or the stack frame layout.
- No source found that says a shipped PS1 game was built with the NEWS-OS compiler. UNVERIFIED hypothesis only.

### SN Systems / Psy-Q / PsyQ gcc
- The Internet Archive "PS1 SDKs" item lists "GNU C Compiler Version 2.60 (World) (Disk 1)", "Programmer Tools Runtime Library 3.3 DTL-S2190", "Programmer Tool - Runtime Library Version 2.0 (Japan)" and "SDevTC - Version 2 (Japan)". Only the listing was read. https://archive.org/details/ps1_sdks
- A psxdev.net thread says PsyQ shipped varying gcc versions and that components can be mixed between SDK CDs (gcc 2.8.0 with Runtime Libraries 4.3, gcc 2.8.1 with 4.4). Forum post, secondary. https://www.psxdev.net/forum/viewtopic.php?p=22710
- decompals/old-gcc builds the PS1 gcc variants; its `psx` patches add a `mips-sony-psx` target whose header is copied from the SGI "Iris version" config ("Definitions by GIL for PSX"), little-endian, R3000, soft float, `-G` gp opt. https://github.com/decompals/old-gcc and https://raw.githubusercontent.com/decompals/old-gcc/master/patches/psx.patch (versions in the repo: 2.5.7, 2.6.0, 2.6.3, 2.7.2, 2.7.2-cdk, 2.8.0, 2.8.1, 2.91.66, 2.95.2).
- Metrowerks sold a CodeWarrior PS1 MIPS compiler (not ucode family). Found only in a search snippet of https://www.mactech.com/content/md1-pr-metrowerks-supports-playstation-0 (the page timed out when fetched). UNVERIFIED.

### Konami and other PS1 decomps: which compilers were identified
| Game | Evidence | URL |
|---|---|---|
| Vandal Hearts (Konami, 1996) | Makefile uses gcc 2.6.3 (`cc1_v263_decompals`), `-O1 -G8`, `-msoft-float`, maspsx `--aspsx-version=2.21`; `audio.c` uses gcc 2.5.7 `-O2 -G0`; ASPSX 2.21 from PSYQ 3.3 / DTL-S2190 | https://github.com/shao113/vh (Makefile, README) |
| Silent Hill (Konami, 1999) | ninja_config.py: `gcc-2.8.1-psx` and `gcc-2.7.2-cdk` cc1, `-O2`, `-G8` exe / `-G0` overlays, maspsx | https://github.com/shdecompilations/silent-hill-decomp (ninja_config.py) |
| Castlevania SotN (Konami, 1997) | Makefile downloads release `cc1-psx-26` (gcc 2.6.x PSX cc1) and uses maspsx | https://github.com/Xeeynamo/sotn-decomp (Makefile, README) |
| Metal Gear Solid (Konami, 1998) | Builds with the original PsyQ binaries (a `psyq_sdk` repo, Wine on Linux/macOS); no gcc version stated in the README | https://github.com/FoxdieTeam/mgs_reversing (README) |
| Evo's Space Adventures | gcc 2.95.2 psx + maspsx (splat examples list psyq 4.6/gcc2.95) | https://github.com/mkst/esa |

- No Suikoden decomp was found by GitHub repository search (queries "suikoden decompilation psx"). UNVERIFIED either way.
- No PS1 game built with IDO or any ucode-family compiler was found. GitHub repository searches for "ps1 ido compiler decompilation" and "psx mips ucode compiler decomp" returned nothing relevant. Evo's Space Adventures is the nearest case: N64 version IDO, PS1 version gcc. The negative result is not proof.
- Namco's Ridge Racer / Rage Racer decomps are reported (news articles, secondary) to use asm in C wrappers because modern compilers do not reproduce Namco's tools: https://www.generationamiga.com/2026/08/10/30-years-later-ridge-racer-has-finally-reached-100-decompilation/ . Compiler not named there.

## 2. Compilers that reserve extra frame bytes

Short answer: no source found for any MIPS compiler/version that reserves an unused extra 16 bytes. Everything below is context.
- gcc 2.8.1 mips: frame = var_size + args_size + extra_size (+ saves). `args_size` is the max outgoing args (4 words minimum when a call is made, via `STACK_ARGS_ADJUST`); `extra_size` is one aligned word only with `TARGET_ABICALLS`; alloca with zero args forces 4 words. No extra 16 for leaves. https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-2.8.1/gcc/config/mips/mips.c (compute_frame_size) and .../mips.h (`STARTING_FRAME_OFFSET`, `STACK_ARGS_ADJUST`). Matches this project's earlier finding that gcc is not the compiler (T-0013).
- A teaching summary of the o32 convention (written to match mips-gcc, "not the native compiler"): outgoing area A is 0 for leaf, at least 4 words for non-leaf. https://www.cs.umb.edu/cs641/MIPscallconvention . The SPIM docs say the native compiler uses a more complex convention (search snippet, not opened). The System V MIPS ABI stack-frame PDF at https://www.cs.unm.edu/~jeffk/cs341f09/_media/sys_v_mips_abi_stack_frame_section_.pdf could not be read (compressed PDF, no poppler); UNVERIFIED.
- IDO frame logic (from this project's reading of the matching decomp of IDO 7.1 ugen): see wiki/matching-notes.md "Frame size". Matching decomp of IDO itself: https://github.com/decompals/ido-matching-decomp (cc, ugen, as0 7.1 functions matched; uopt, cfe, as1 not matched as of 2026-10-09).
- IDO version list: 4.0, 4.1.x, 5.0, 5.1, 5.3, 6.0, 6.1, 7.1, 7.1.1 (no 5.2 entry). https://tech-pubs.net/wiki/index.php/IRIS_Development_Option . I found no comparison of frame layout across them. UNVERIFIED whether IDO 4.x/5.0/5.1 differ.
- Ultrix 4.x `cc(1)` is also "RISC ucode C compiler" with `-EB/-EL` ("needed only when compiling for RISC machines from vendors other than Digital"), `-mips1`, `-O2` global ucode optimizer, `-G num`. https://typewritten.org/Manual/DEC/Ultrix/4.4/mips/man1/cc.html and https://typewritten.org/Manual/DEC/Ultrix/4.2/mips/man1/cc.html . Native DECstation target is little-endian R2000/R3000 (general knowledge, UNVERIFIED here).
- IRIX 5.3 IDO's `-KPIC`/`-call_shared` adds gp save; this project already tried it (wiki/matching-notes.md).

## 3. How matching decomps handle systematic compiler-output differences

- maspsx ("Modern ASPSX"): replaces `ASPSX.EXE` + psyq-obj-parser. It "takes the assembly code output of gcc and massages it" so GNU `as` produces the object the original PsyQ would; per-ASPSX-version behaviour table (div expansion, `$at` handling, `%hi/%lo`, `$gp` use), `--expand-div`, `--use-comm-section`, `--aspsx-version`. Used by SotN, Silent Hill, ESA, Croc, Soul Reaver, Rayman, Spyro, MediEvil. It states ASPSX "does not appear to do very much in terms of code optimisation". https://github.com/mkst/maspsx (README on branch master: https://raw.githubusercontent.com/mkst/maspsx/master/README.md)
- asm-processor: pre-processes `.c` and post-processes `.o` so assembly can be embedded in IDO-compiled C (`GLOBAL_ASM`, `INCLUDE_ASM`); handles `.late_rodata`; supports IDO 5.3/7.1; license Unlicense. https://github.com/simonlindholm/asm-processor
- ido-static-recomp: statically recompiles the IDO tools to native Linux/macOS; IDO 5.3 and 7.1; repo includes the IDO binaries under `ido/5.3` and `ido/7.1`. https://github.com/decompals/ido-static-recomp
- qemu-irix history: OoT's compiler doc says the project first ran IDO under qemu-irix (now unmaintained) and moved to ido-static-recomp. https://github.com/zeldaret/oot/blob/main/docs/compilers.md . qemu-irix is a QEMU patch for IRIX/Solaris user-mode emulation by Kai-Uwe Bloem; the N64 fork lives at https://github.com/n64decomp/qemu-irix (README).
- Compiler patching: decompals/old-gcc patches gcc source to give `mips-sony-psx` and `-cdk` targets and a little-endian mips.h (patch files above). This is the established precedent for patching a compiler to emulate a vendor build. Note: it patches source of a GPL compiler.
- Debugger tooling: n64decomp/ido is a uopt debugger (reverse-engineered), showing the community inspects and instruments IDO internals. https://github.com/n64decomp/ido
- Post-processing at the object level is normal: asm-processor (above), maspsx rewriting compiler asm (above), splat+objdiff. Ridge Racer uses asm-in-wrapper functions (secondary, above).
- Match vocabulary in primary docs:
  - zeldaret/mm CONTRIBUTING: `NON_MATCHING` for functions that do not match; `NON_EQUIVALENT` for ones that differ in behaviour too; tips: permuter, decomp.me scratch. https://github.com/zeldaret/mm/blob/main/docs/CONTRIBUTING.md
  - zeldaret/mm REVIEWING: "Files with NON_MATCHING functions have equivalent behaviour." https://github.com/zeldaret/mm/blob/main/docs/REVIEWING.md
  - pmret/papermario CONTRIBUTING: same NON_MATCHING / NON_EQUIVALENT split. https://github.com/pmret/papermario/blob/main/CONTRIBUTING.md
  - zeldaret/mm tutorial says a temp that "looks fake" may need keeping, and an "extra user stack variable" may be needed to get the right stack size, treated as plausible developer behaviour. https://github.com/zeldaret/mm/blob/main/docs/tutorial/diff_and_permuter.md ; https://github.com/zeldaret/oot/blob/main/docs/tutorial/other_functions.md
  - The word "fakematch" is not defined in any zeldaret, decompals or decomp.me document I could reach (searched the md files under docs/ of zeldaret/oot, zeldaret/mm, pmret/papermario, n64decomp/sm64, doldecomp/melee). The only definition found is this project's own CODING_STANDARDS.md section 7. The community usage "C that produces the right bytes but is not what the author plausibly wrote" is UNVERIFIED as written guidance.
  - mkst/maspsx and decompals/old-gcc never call themselves fakematches; their READMEs present them as ways to reproduce the original toolchain (maspsx: "equivalent object as what the original PSYQ SDK would create").
  - Judgement (mine, not a source): a transformation is toolchain emulation when it models a deterministic behaviour of the original compiler, applies to every function uniformly with no per-function switches, and does not change the C source. A per-function C hack that exists only to hit bytes is a fakematch.

## 4. Licensing status, as the projects describe it (not legal advice)

- ido-static-recomp: no top-level LICENSE; `gh api` license field is null. `ido/5.3/LICENSE.md` is the "Silicon Graphics Freeware Legal Notice, Copyright 1995 SGI" (copy, modify, use and distribute allowed if the notice is kept, plus any third-party terms), copied "verbatim" from a 1999 web.archive.org URL. `ido/7.1` has no such file. The recomp source README says nothing about legal status or where binaries come from. https://raw.githubusercontent.com/decompals/ido-static-recomp/master/ido/5.3/LICENSE.md ; https://github.com/decompals/ido-static-recomp . Whether the 5.3 binaries were actually part of that SGI freeware release: UNVERIFIED.
- qemu-irix: code is a QEMU patch (QEMU is GPL-2; the repo reports license "NOASSERTION"); it needs a target IRIX root filesystem that the README tells you to supply ("<target rootfs>"). https://github.com/n64decomp/qemu-irix
- PsyQ: FoxdieTeam/psyq_sdk is a public repo, described "PSYQ SDK used to build mgs_reversing", with `aspsx`, `psyq_4.3/4.4/4.5` (bin, include, lib) directories and no license (api license null). Other projects (Vandal Hearts, esa, SotN) list ASPSX/PsyQ as external files you must supply and say the repo has no game assets: https://github.com/shao113/vh , https://github.com/Xeeynamo/sotn-decomp ("This repo does not include any assets or assembly code ... A prior copy of the game is required"). Sony's stance: no primary source found. UNVERIFIED.
- gcc builds (old-gcc): built from GNU FTP sources plus patches; the repo has no license section (GPL source). https://github.com/decompals/old-gcc
- OS images (Ultrix, NEWS-OS, IRIX): no project statement found about redistribution terms. The only pointer to an NEWS-OS 4.x image is a forum thread on WinWorld (https://forum.winworldpc.com/discussion/comment/186456, appeared in search results; content not read, UNVERIFIED). Ultrix and IRIX are proprietary; terms UNVERIFIED.
- zeldaret/mm CONTRIBUTING says it only uses publicly available code and bans contributors who accessed leaked Nintendo SDK/game source; it does not address compilers. https://github.com/zeldaret/mm/blob/main/docs/CONTRIBUTING.md (from a search snippet of the mirrored file).

## Open items
- Does NEWS-OS 4.x/Ultrix 4.x cc emit the +16? Needs the compiler; untested.
- Does any IDO release before 5.3 (4.x, 5.0, 5.1) reserve +16? No docs found.
