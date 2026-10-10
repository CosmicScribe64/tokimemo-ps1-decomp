---
type: research
updated: 2026-10-09
ticket: T-3110
sources: ["tools/ido_eval.py", "tools/ido_eval_cases/", "wiki/data/t0018-cases.md", "wiki/original-compiler.md", "disc/files/CDROM/EXEDIR/O.BIN"]
---

# IDO 5.2 and 4.1 against the game code (T-3110)

Question: does IDO 5.2 (C 3.18, the release the O.BIN version stamps point at, [[original-compiler]]) or IDO 4.1 (C 3.10/3.12) reproduce the game code more closely than the build's IDO 5.3? Ticket: [[tickets/T-3110-test-ido-52-and-41]]. Tool: `tools/ido_eval.py`; readable C for blocked functions: `tools/ido_eval_cases/` (46 files, m2c drafts cleaned until IDO 5.3 compiles them).

## Short answer

- **IDO 5.2 generates the same code as IDO 5.3.** With the frame pass, all 2564 matched C functions are byte-identical. Without it, only the 504 frameless ones are. All 46 blocked cases give the same result as 5.3. 5.2 does not make +16 frames, does not fix the T-0018/T-1321 cases or the constant reuse, and does not fix the function-pointer-table frame. Its one difference is the version stamp: it writes 3.18, the value in O.BIN.
- **IDO 4.1's code generator (ugen) makes the original's +16 frames natively.** With 5.2's front end and uopt, 4.1's ugen gives exactly the frame pass's frame size and `$sp` offsets in all 2564 matched functions. But it changes the code of 80 of them (register reuse, unaligned-store expansion, sign-extension temporaries), and 5.3 already matched those against the original. The full 4.1 suite is further off: its front end puts string literals and `const` data in `.data`, while O.BIN keeps string literals in `.rdata`.
- **No compiler or pass mix reproduces what T-1321's `cvt_pass.py` does.** That pass removes the unsigned-load widening and turns off copy propagation for switch temporaries. Its 26 pass-only ETC/TACO functions fail under 5.3, 5.2, 4.1 and every mix tried.
- Recommendation: **(b) keep IDO 5.3 + the frame pass + the T-1321 ucode pass.** The evidence for it is below. Switching to 5.2 gains no function and adds an emulator plus a licensing step. The 4.1 result supports the frame pass: real IDO 4.1 code produces the pass's frame layout in every matched function, which is the strongest evidence so far that the pass models a real compiler behaviour (CODING_STANDARDS 7a).

## Provenance (private use only, never committed)

Downloaded by the coordinator after the user's decision (2026-10-09), into the gitignored `tools/local-compilers/` (archives kept next to the extracted trees).

| compiler | source URL (as used by decomp.me) | sha256 of the archive | size | contents |
|---|---|---|---|---|
| IDO 5.2 | `https://github.com/LLONSIT/qemu-irix-helpers/raw/refs/heads/n/qemu/ido5.2.tar.xz` | `870cd946d31d6abee24d47926328d15909771520240a0a4f5cb7d27aea90e1ad` | 16.2 MB | 172 entries: IRIX 32-bit big-endian MIPS ELF executables (`usr/lib/driver`, `cfe`, `uopt`, `ugen`, `as1`, ...), IRIX runtime (`lib/rld`, `lib/libmalloc.so`, `usr/lib/libc.so.1`, libraries, compressed ABI files), and x86-64 Linux builds of `qemu-irix` and `qemu-irix-4.0` |
| IDO 4.1 | `https://github.com/decompme/compilers/releases/download/compilers/ido4.1.tar.gz` | `16008087602fe1a39cdd7880a3c6987891ddb594cce4416670f2e96456e9b4d4` | 10 MB | 185 entries: IRIX big-endian MIPS ECOFF executables (`usr/bin/cc`, `acpp`, `accom`, `ccom`, `uopt`, `ugen`, `as1`, ...; `file` reports "version 3.12"), IRIX runtime (`lib/libc_s`, `lib/cpp`, libraries), x86-64 `qemu-irix-4.0`, and `ecoff_tool.py` (ECOFF to ELF converter) |

Neither archive contains a license, copyright or readme file (the only readme-like file is the compressed `usr/lib/nonshared/README.z` in 5.2).

Where the URLs come from: decompme/compilers at commit `fdd6793`, `platforms/n64/ido5.2/Dockerfile` and `platforms/n64/ido4.1/Dockerfile` (https://github.com/decompme/compilers). decomp.me runs both with the bundled qemu-irix (`cromper/cromper/compilers.py` in https://github.com/decompme/decomp.me): `qemu-irix -L <root> <root>/usr/lib/driver ...` for 5.2, `qemu-irix-4.0 -silent -L <root> <root>/usr/bin/cc ...` plus `ecoff_tool.py --convert-elf` for 4.1. No IRIX OS image is needed: the archives contain the IRIX runtime libraries that the compilers load.

### Running them in Docker

Two lines were added to `tools/Dockerfile` in the worktree, used only as image `tokimemo-decomp-ido52`, and not committed:
```
FROM ubuntu:24.04 AS u2404              # before FROM ${BASE}: glibc >= 2.38 for qemu-irix-4.0
RUN apt-get update && apt-get install -y --no-install-recommends libglib2.0-0t64 && rm -rf /var/lib/apt/lists/*
...
RUN apt-get update && apt-get install -y --no-install-recommends libglib2.0-0 patchelf && rm -rf /var/lib/apt/lists/*
COPY --from=u2404 /usr/lib/x86_64-linux-gnu /opt/u2404/lib
```
- `qemu-irix` (5.2) needs `libglib-2.0.so.0`.
- `qemu-irix-4.0` needs glibc 2.38 and re-executes itself for every pass. `tools/ido_eval.py` starts it through a wrapper that runs the 24.04 loader with `--argv0` set to the wrapper (patchelf was tried and crashed).
- Problems found while running the archives, and how `ido_eval.py` works around them:
  - IDO 4.1's `acpp` (GNU cpp 1.35) hangs under `qemu-irix-4.0` on any `#include`. The 4.1 front end therefore gets a `.i` that the host `cpp -P -undef -D__sgi ...` produced.
  - The archive's `ecoff_tool.py` imports `datetime.UTC`, which needs Python 3.11; the image has 3.10, so the script patches that attribute in.
  - The converter writes ECOFF `R_REFWORD` relocations as ELF type 3; the script changes them to `R_MIPS_32`.
  - The converted symbols have no sizes; the script derives them.
  - The converter keeps the section name `.rdata`; the script renames it to `.rodata`.
- Each compile runs the front end's driver with `-Hc -K` (cfe/accom, uopt and ugen; the files are kept). It then reruns uopt and ugen from another version when a mix is asked for, optionally runs `frame_pass.transform` on the binasm, and finishes with the chosen version's own `as1`. The ucode and binasm formats of 4.1, 5.2 and 5.3 are interchangeable in both directions.

## Results

Flags everywhere: `-c -EL -O2 -mips1 -G 0 -non_shared -Wo,-nokpicopt -Xcpluscomm` (the build's `IDO_CFLAGS`). The uopt of 5.2 and of 4.1 both accept `-nokpicopt`. Pass mixes are written front end/uopt/ugen/as1.

### Version stamps (`ido_eval.py stamp`)

| compiler | object format | symbolic-header vstamp | notes |
|---|---|---|---|
| IDO 5.3 (static recomp) | ELF, `.mdebug` | 0x0313 = **3.19** | |
| IDO 5.2 | ELF, `.mdebug` | 0x0312 = **3.18** | the value in O.BIN (a.out and symbolic header) |
| IDO 4.1 | ECOFF | 0x030C = 3.12 | the file header is stored **big-endian** in a little-endian (`-EL`) object, f_magic 0x0162, f_flags 0x8000, opthdr 0x38 |

The 4.1 object header has the same properties as O.BIN: big-endian header bytes on a big-endian host, the 0x8000 flag bit, and opthdr 0x38. That supports the [[original-compiler]] reading of the O.BIN `.comment` header (a big-endian MIPS link host).

### Regression: the 2564 matched C functions (`ido_eval.py regress`)

Each `src/main/*.c` and `src/ovl/*.c` goes through asm-processor and the padding passes of `tools/cc.py`. Every function the file defines in C is then compared, with `objdump -dr` and addresses stripped, against the ninja-built object, which passes the sha1 check against the original (27 of 27 OK).

| configuration | identical | notes |
|---|---|---|
| 5.3 + frame pass (the build) | 2564 / 2564 | harness check |
| 5.3, no pass | 504 / 2564 | the frameless functions |
| **5.2 + frame pass** | **2564 / 2564** | |
| 5.2, no pass | 504 / 2564 | same as 5.3: 5.2 has IDO 5.3's frames |
| 5.2/4.1/5.2/5.2 (4.1 uopt) + frame pass | 2564 / 2564 | 4.1's uopt behaves like 5.x's here |
| **5.2/5.2/4.1/5.2 (4.1 ugen), no pass** | **2484 / 2564** | frames native; in the 80 that differ, `ido_eval.py frames` finds **0** functions whose frame size or `$sp` offsets differ from the frame pass |
| 5.2/5.2/4.1/4.1 (4.1 ugen and as1), no pass | 2482 / 2564 | same, plus 2 as1 differences; again 0 frame differences |
| 4.1, no pass | 928 / 2564 | 10 files (1518 functions) do not compile: 6 have pointer assignments that 4.1's accom rejects as errors (IDO 5.3 only warns, e.g. `D_800CA138 = &D_800CA14C;`), and 4 have asm-processor rodata placeholders that 4.1 puts in `.data` (`subsection not found`). 928 of the 1046 compiled functions are identical |
| 4.1 + frame pass | 210 / 2564 (first run) | the frame is added twice |

Kinds of the 80 differences with 4.1's ugen (`funcdiff.py`): `andi a0,a0,0xff` reusing the argument register where the original has `andi t6,a0,0xff` (`func_8004E750`, the function IDO 7.1 also misses); `sll v0,v0,16; sra v0,v0,16` in place where the original uses a fresh temporary (`func_80140788`, `func_8014261C`); `swl/swr` pairs for stores the original writes with `sw` (`SetWorkBase`, `strInit`); temporary numbering and `lui` order (TT, RPG_BAT, TAIIKU).

So 4.1's ugen has the original's frame layout and an older register allocator; the original has the frame layout and the 5.x register allocator.

### Blocked functions (`ido_eval.py cases`)

46 functions: 42 rows of [[data/t0018-cases]] (25 `promo`, 14 `regorder` including the constant-reuse rows, 3 `reverse`) and 4 from the wave-2 batch notes in [[matching-notes]] (the frame cases `func_80137C28`, `func_8013BC74` and `func_80133D14`, and the `0x38` multiply `func_801374A8`). Each is one readable C file, compiled by every configuration and compared with its original `.s`. A cell is MATCH or the number of differing lines. The C is m2c-level, so a non-zero count under 5.3 also contains source-level noise. What matters is whether any configuration does better than 5.3 + pass on the same C.

| configuration | MATCH | better than 5.3 + pass | worse |
|---|---|---|---|
| 5.3 + pass | 2 (`func_80132E5C`, `func_80132DC4`) | - | - |
| 5.2 + pass | 2 | 0 | 0 (all 46 counts identical) |
| 5.2, no pass | 0 | 0 | 38 (frame) |
| 4.1, no pass | 1 | 0 | 17 |
| 5.2/5.2/4.1/4.1, no pass | 1 | 0 | 5 |
| 5.2/5.2/4.1/5.2, no pass | 1 | 0 | 5 |
| 5.2/4.1/5.2/5.2 + pass | 2 | 0 | 0 |
| 5.2/4.1/4.1/4.1, no pass | 1 | 0 | 5 |

Per-case counts are in `build/t3110/cases/cases.md` after a run (not committed). Frame questions:
- **Function-pointer table** (SHOUGATU `func_80137C28`, the 44 SHOUGATU and ~35 GYOZI/~25 GEKO table-copy functions). The original has the table at `sp+0x2C` in a 0x90 frame. 5.3 + pass, 5.2 + pass and 4.1's native ugen all give `sp+0x28` in 0x88. The 4 bytes are not a compiler property of any of these versions, so they come from the source (an extra 4-byte local or temporary).
- **Spill slot** (GEKO `func_8013BC74`). The original spills at `sp+0x28`; every configuration spills at `sp+0x2C`. Same conclusion.

### The T-1321 ucode-pass behaviours

T-1321 (branch o-t0018, `tools/cvt_pass.py`) found two IDO 5.3 details behind the T-0018 gap:
- cfe widens every unsigned 8/16-bit load (`LOD; CVT`), and uopt allocates the widened value instead of the global;
- uopt's copy propagation merges a switch temporary into its global.

The pass undoes both. Test: `src/ovl/ETC.c` and `TACO.c` from commit `16bbea4` of that branch (312 C functions, 26 of which match only with the pass), with that commit's headers, compiled without the pass and compared with the original bytes:

| configuration | identical of 312 | pass-only functions recovered (of 26) |
|---|---|---|
| 5.3 + frame pass | 286 | 0 |
| 5.2 + frame pass | 286 | 0 |
| 5.2/4.1/5.2/5.2 (4.1 uopt) + pass | 286 | 0 |
| 5.2/5.2/4.1/4.1 (4.1 ugen/as1) | 280 | 0 |
| 4.1/5.2/5.2/5.2 (4.1 front end) + pass | 271 | 0 |
| 4.1/4.1/5.2/5.2 + pass | 271 | 0 |
| 4.1 | 268 | 0 |

The functions each configuration misses are a superset of the 26 that 5.3 misses. Neither 5.2's cfe nor 4.1's accom drops the widening, and neither version's uopt stops the switch-temporary propagation. So the T-1321 behaviours are not those of IDO 5.2 or 4.1 either.

## What this says about the original compiler

- Release 3.18 (stamp) with code that 5.3 (3.19) reproduces exactly wherever no known gap applies. That fits an IDO 5.2-generation suite, and 5.2 behaves identically to 5.3 on everything measured.
- Its frames follow a rule that 4.1's ugen (3.12) applies everywhere, while 5.2/5.3's ugen does not. So the original's ugen keeps the older frame layout but has the newer register allocation. Neither stock IRIX release is that combination, which supports the [[original-compiler]] hypothesis of a non-IRIX (Sony NEWS-based?) 3.18 build. UNVERIFIED: no such compiler is available.
- The unsigned-load and switch-temporary differences (T-1321) and the constant reuse rows are in no available release. They stay toolchain emulation passes.

## Recommendation

**(b) Keep IDO 5.3 + `tools/frame_pass.py` + the T-1321 pass (`tools/cvt_pass.py`, which replaced the uopt-patch idea).**
- 5.2 instead of 5.3 changes no output: 0 functions gained, 0 lost. It would only add qemu-irix to the build, slow compiles (each pass runs emulated), and add a licensing step.
- 4.1, alone or mixed, loses 80+ matched functions.
- The frame pass can now cite a real compiler: in all 2564 matched functions, 4.1's ugen produces the same frame sizes and `$sp` offsets as the pass. Recording this in [[toolchain]] and [[matching-notes]] is enough. `ido_eval.py frames` reruns the check whenever a qemu setup is available.
- (c) A mix (for example 5.3 front end and uopt, 4.1 ugen, no frame pass) would replace the pass with real compiler code but costs the 80 functions above. Not recommended.

### What CI would need for option (a), for the user to decide (not implemented)

CI builds from a clean clone and cannot use `tools/local-compilers/`. Two ways:
1. **Fetch from decomp.me's public URLs in CI.** The workflow downloads the two archives above, checks the sha256 values recorded here, and extracts them into the image (or Dockerfile stages do it). The repository then holds only URLs and checksums, like decompme/compilers' own Dockerfiles. Dependencies: the URLs must stay up. The 5.2 file is a "raw" link to a branch (`n`) of a personal fork, uploaded 2026-06-04, so the checksum would catch a change but nothing guarantees it stays. The runner needs the glib/glibc additions above.
2. **Put the archives in the private encrypted CI bundle** (`tools/make_ci_bundle.sh`, [[ci]]), next to the game files in the private data repository. They are never public and do not depend on third-party URLs. The project then keeps its own copy of SGI binaries, and the bundle script, the size budget and the secrets cover one more kind of data.

## Licensing (facts, not legal advice)

What the archives contain: see the provenance table. They are SGI's IRIX compiler binaries (IDO 5.2, IDO 4.1) together with IRIX runtime pieces (`rld`, `libc.so.1`, `libmalloc.so`, `libc_s`, static libraries) and Linux builds of qemu-irix (a QEMU fork that runs IRIX user programs). Neither archive includes a license or notice file.

Who distributes them publicly:
- decompme/compilers (https://github.com/decompme/compilers): the Dockerfiles that fetch the archives; IDO 4.1 is an asset of its `compilers` release (release published 2023-10-18). The GitHub API reports no license for the repository. decomp.me (https://decomp.me) offers `ido4.1` and `ido5.2` presets that run these archives (`cromper/cromper/compilers.py`, https://github.com/decompme/decomp.me).
- LLONSIT/qemu-irix-helpers (https://github.com/LLONSIT/qemu-irix-helpers), a fork of irixxxx/qemu-irix-helpers. The repository's GPL-2.0 license covers its scripts; its README describes the scripts and does not mention the archives. Branch `n`, directory `qemu/`, holds `ido4.1.tar.xz`, `ido5.2.tar.xz`, `ido5.3_c++.tar.xz`, `ido6.0`, `mipspro7.4.4` and an `ido_root.tar.xz`. `ido5.2.tar.xz` was added on 2026-06-04 ("Add files via upload").

What other decomp projects do:
- n64decomp/sm64 (https://github.com/n64decomp/sm64) commits the IRIX IDO 5.3 binaries and runtime (`tools/ido5.3_compiler/`, with `lib/rld`) together with an SGI Freeware Legal Notice (`LICENSE.md`). Historically it ran them under qemu-irix.
- n64decomp/libreultra (https://github.com/n64decomp/libreultra) commits IRIX IDO binaries (`tools/ido/usr/lib/cfe`, `uopt`, `ugen`, `as1`, `libc.so.1`, ...) and runs them with qemu-irix (Makefile `QEMU_IRIX ... -L $(IRIX_ROOT)`). Version not checked.
- decompals/ido-static-recomp (https://github.com/decompals/ido-static-recomp) commits the IRIX IDO 5.3 and 7.1 binaries as recompilation input (`ido/5.3/`, which carries the SGI Freeware Legal Notice copied from an archived SGI page; `ido/7.1/` carries none). It publishes recompiled native Linux/macOS binaries as release assets. Most current N64 decomps use those (for example mkst/sssv, LLONSIT's Wave Race 64 Makefile). This project uses its v1.2 5.3 and 7.1 builds ([[toolchain]]).
- This search found no public project that builds with IDO 5.2 or 4.1 from its own repository. decomp.me is the visible distributor and user.

How this differs from ido-static-recomp:
- ido-static-recomp translates the IRIX MIPS programs into C and compiles them into native programs, so no emulator and no IRIX runtime libraries are needed. It covers only 5.3 and 7.1.
- The 5.2/4.1 archives are the original IRIX executables plus IRIX runtime libraries, run under an emulator. They are SGI binaries just as the recompilation inputs are. The recompiled programs are derived from SGI binaries.
- Only the ido-static-recomp 5.3 input directory and the sm64 copy carry an SGI license text. The 5.2/4.1 archives carry none.

The four ways this project could use them, and what each involves:

| use | what is stored where | facts relevant to the risk |
|---|---|---|
| Private testing (this ticket) | archives in the gitignored `tools/local-compilers/` of one machine; the repository has the URLs, checksums and a harness | nothing from SGI is in the repository or its history; this is what decomp.me users do through the website |
| Commit them | the repository and its history | never: AGENTS.md and the user's rule forbid it; once in history, removal needs a rewrite |
| Fetch in CI from the public URLs | URLs and checksums in the workflow; binaries only on the runner | same model as decompme/compilers' Dockerfiles; the public workflow file and logs show the download; depends on third-party hosting |
| Private encrypted CI bundle | encrypted archive in the private data repository ([[ci]]) | not public; the project holds its own copy; same handling as the game files already in the bundle |

## Reproduce

```
IMG=tokimemo-decomp-ido52 tools/docker.sh sh -c 'python3 configure.py && ninja'          # reference objects, 27/27 OK
IMG=tokimemo-decomp-ido52 tools/docker.sh python3 tools/ido_eval.py regress 5.2 frame build/t3110/r
IMG=tokimemo-decomp-ido52 tools/docker.sh python3 tools/ido_eval.py regress 5.2/5.2/4.1/5.2 noframe build/t3110/r
IMG=tokimemo-decomp-ido52 tools/docker.sh python3 tools/ido_eval.py frames build/t3110/r 5.2_5.2_4.1_5.2-noframe
IMG=tokimemo-decomp-ido52 tools/docker.sh python3 tools/ido_eval.py cases tools/ido_eval_cases build/t3110/cases
IMG=tokimemo-decomp-ido52 tools/docker.sh python3 tools/ido_eval.py stamp 5.2
```
A full `regress` run takes about 15 minutes on Apple Silicon (amd64 emulation plus qemu-irix).
