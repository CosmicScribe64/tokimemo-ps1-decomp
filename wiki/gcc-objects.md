---
type: concept
updated: 2026-10-18
sources: ["tools/gcc_fingerprint.py", "tools/aspsx_survey.py", "tools/sdk_aspsx_pass.py", "tools/test_sdk_aspsx_pass.py", "tools/test_gcc_fingerprint.py", "config/toolchains.txt", "configure.py", "tools/cc.py", "tools/Dockerfile"]
---

# PsyQ-gcc-built objects inside overlays (T-9200, T-24086)

The game code is IDO output, but four overlays (`TAIIKU`, `TT`, `TACO`, `DATE`) end their `.text` section (immediately before `.rodata`) with a precompiled PsyQ SDK module pulled from `LIBGS.LIB` or `LIBGTE.LIB` at overlay link time because `SLPM_86.053` (`O.BIN`) did not export that symbol. They cannot be matched with IDO. This page records how they are found, which toolchain each needs, and how the GCC + Sony workstation assembler pipeline matches them. Tool and build rules: [[toolchain]], [[build-system]].

## Fingerprint (`tools/gcc_fingerprint.py`)
Two instruction habits that IDO never produces, measured on the matched IDO functions of the whole tree (4521 functions, 0 hits each) and present all over the SDK asm of the main exe (libgte, libgpu, libsnd, libspu, libcd, libetc, libgs, libpress, libapi: e.g. 50 of 96 libgpu and 38 of 61 libcd functions):
- `move` spelled `addu rd,rs,$zero` (IDO: `or rd,rs,$zero`);
- `j .Lxxxx` to a label inside the function (IDO: `b`).
A hit needs a local jump, or one move per 32 instructions (three IDO-shaped functions of 230 to 1100 instructions hold one or two `addu` moves: `get_fnt`, ETC `func_801328A4`, NAME_ENT `func_80143580`). Limits: it cannot tell gcc C from hand-written assembly, and it misses gcc code with neither habit. Run `tools/docker.sh python3 tools/gcc_fingerprint.py [--matched]` after a split (`--matched` is the control, 0 hits).

## Hits (all overlays)
Every non-IDO object across all 26 overlays sits at the very end of its overlay's `.text` section (immediately before `.rodata`) and came from linking the overlay against PsyQ `.LIB` archives for a symbol not resident in `SLPM_86.053`:

| function | object | provenance | verdict |
|---|---|---|---|
| TAIIKU `func_801488F0` | `TAIIKU/801488F0` | `GsSetFogParam(GsFOGPARAM *fgp)` from `LIBGS.LIB` (calls `SetDQA`, `SetDQB`, `SetFarColor` in `libgte_reg03.s`) | `gcc 2.8.1-psx 2.79` C, matched (80 B) |
| TT `func_8014D260` | `TT/8014D260` | `ratan2(long y, long x)` + `ratan_tbl` (`D_80159B20`, 1025 `s16` entries at the end of `TT` `.data`) from `LIBGTE.LIB(RATAN.OBJ)` | `gcc 2.7.2-psx 2.56` C + `tools/sdk_aspsx_pass.py`, matched (400 B; `TT` 303/303 C, T-24086) |
| TACO `func_8015CF30`, `func_8015D0D0`, `func_8015D270`; DATE `func_8015A640`, `func_8015A7E0`, `func_8015A980` | `libgte_8015CF30..D270`, `libgte_8015A640..A980` | `RotMatrixX`, `RotMatrixY`, `RotMatrixZ` from `LIBGTE.LIB` (using the resident sine table `D_800DE500`) | not compiler output: `$t7`-first registers, `multu` for signed products, `mflo; nop; nop` before the next `multu`. Hand-written libgte assembly; reclassified from `c` to `asm` subsegments in `config/overlays/DATE.yaml` and `config/overlays/TACO.yaml` (counted under `SDK libs` in `tools/progress.py`, T-24093) |

Not gcc: TACO `func_801420DC` and `func_801421AC` (listed as gcc-shaped by wave 6; the fingerprint does not flag them). They are IDO code and match with plain IDO C (`if (D->unk80 == 3 || D->unk80 == 4)` chains; constants in registers, `nop` after `beq`), see [[matching-notes]]. A full scan of all remaining `INCLUDE_ASM` functions across `src/main/` and `src/ovl/` (T-24086, T-24093) confirms that 100% of remaining `INCLUDE_ASM` functions in `c` subsegments are Konami IDO 5.3 C.

## Which gcc
Chosen by compiling the C and comparing bytes. `func_801488F0`: 2.7.2-psx and 2.6.3-psx end the function with `addu $sp; j $31; nop`, while the original has `jr $ra` with the stack release in its delay slot; gcc 2.8.0-psx and 2.8.1-psx print that epilogue (`.set noreorder`) and match byte for byte (ASPSX version 2.79). 2.8.0 and 2.8.1 are not told apart by this function; 2.8.1 is used. `TT:func_8014D260` is a leaf function built with `gcc 2.7.2-psx` (`config/toolchains.txt`: `TT/8014D260 gcc 2.7.2-psx 2.56`).

## `func_8014D260` (matched in T-24086: SDK `ratan2` provenance and Sony workstation assembler pass)
**Status (2026-10-18, T-24086):** Matched in C (`src/ovl/TT/8014D260.c`, `400/400` linked bytes, `TT.EXN` SHA1 verified) via `gcc 2.7.2-psx` + `maspsx` + `tools/sdk_aspsx_pass.py`.

### Why `TT/8014D260` is in `TT.EXN`
In `TT`, `func_801320F0` (`src/ovl/TT/80132000.c`) computes `dx` and `dy` between two actor positions and calls `func_8014D260(dy >> 16, dx >> 16)` (`ratan2`), whose callers immediately pass the returned 12-bit angle to `rcos` (`func_800A0140`) and `rsin` (`func_800A0070`) in `SLPM_86.053` (`asm/libgte_8009FA5C.s`). Because `SLPM_86.053` called `rsin`/`rcos`, `SIN.OBJ` was linked into `SLPM_86.053` and exported in `O.BIN`. Because nothing in `SLPM_86.053` called `ratan2`, `RATAN.OBJ` was not linked into `SLPM_86.053`. When `TT.EXN` was linked against `O.BIN` + `LIBGTE.LIB`, the linker pulled `RATAN.OBJ` from `LIBGTE.LIB`, appending `func_8014D260` as the 31st and final `.text` subsegment (`0x8014D260..0x8014D3F0`, right before `.rodata`) and `D_80159B20` (`ratan_tbl`, `1025 * sizeof(s16)` = 2,050 bytes) as the final object of `TT`'s initialized `.data` section (`0x80159B20..0x8015A322`, followed only by zero-padding up to the overlay trailer).

### Why T-11890's "unreleased compiler" hypothesis was superseded in T-24086
T-11890 tested DOS/Win32 `ASPSX.EXE` (v1.05 to v2.86) and DOS/Win32 `CC1PSX.EXE` (PsyQ 3.3 to 4.6), saw that DOS/Win32 `ASPSX.EXE` did not produce the three SDK instruction habits, and hypothesized that an unreleased compiler (`cc1`) must have emitted already-expanded divides, split `%hi`/`%lo` loads, and `mflo` hazard nops.

T-24086 disproved that hypothesis and identified the actual layer responsible:
1. Inspecting `/opt/gcc/2.7.2-psx/cc1` and `/opt/gcc/2.8.1-psx/cc1` confirms `cc1` only emits unexpanded macro instructions (`div\t%0,%1,%2`, `rem\t%3,%1,%2`, `divu\t%0,%1,%2`, `remu\t%3,%1,%2`, and `lh\t%0,%1(%2)`) and never emits `break 7` or `break 6`.
2. Inspecting `asm/libspu.s` reveals it is a hybrid archive containing **both** MIPS ucode `cc`-compiled functions (the first 25 functions, `func_800949B0..func_800979B0`, with `$t6..$t9` temporaries, `b .L` local branches, and shrink-wrapped `$s0..$s4` saves) **and** GCC `cc1`-compiled functions (the last 38 functions, `func_80097FD0..func_80099540`, with `$t0..$t5` temporaries and `j .L` local jumps) — yet **both** compilers' outputs in `asm/libspu.s` share the exact same three habits! That proves the habits come from the **assembler** (Sony's MIPS workstation `as0` + `as1` derivative on Sony NEWS-OS used to build the 1995-era PsyQ SDK archives), not from `cc1`.
3. Feeding `gcc 2.7.2-psx`'s untouched `ratan2.s` output directly into **MIPS `as0` + `as1`** (`/opt/ido/5.3/cc -c`) natively reproduces:
   - **Habit #2 (`mflo`/`mfhi` hazard nops, 290/290 in `asm/lib*.s`)**: `as1` emits `mflo a0; nop; nop; j $L15; sll v0,a0,1` before GCC's `.set noreorder` branch on Divide #1/#3, and `mflo a0; sll v0,a0,1; nop; $L15:` (zero `Reuse of` nops before `sll`, one `nop` before the label) on Divide #2/#4.
   - **Habit #3 (`l* rd,sym(rs)` through `rd` when `rd != rs`, 103/103 in `asm/lib*.s`)**: `as1` (and GNU `as`) expands `lh $3, D_80159B20($2)` (`rd != rs`) as `lui v1,%hi(D_80159B20); addu v1,v1,v0; lh v1,%lo(D_80159B20)(v1)`, while expanding `rd == rs` (53/53) and stores (99/99) through `$at`.
   - **Basic-block instruction scheduling across divides (44/67 sites in `asm/lib*.s`)**: Tested on `asm/libspu.s:func_80098D00` (`0x80098F70`), `as0` + `as1 -O2` schedules `div zero,s6,v1` ahead of `lh t7,6(s5); sw t7,144(sp); lhu t7,136(sp); mfhi t1` in the exact same instruction order as the original binary.
   - **Habit #1 (`mflo`/`mfhi` placement in expanded divides, 4/4 in `TT:func_8014D260` and 63/67 in `asm/lib*.s`)**: Follows the Write-After-Read (WAR) hazard rule of the check sequence (`{rs, rt}` for signed `div`, `{rt}` for unsigned `divu`): when `rd` is not read by the check sequence, `mflo rd` / `mfhi rd` is emitted immediately after `div $zero,rs,rt` (before `bnez rt`); when `rd` is read by the check sequence (`rd == rs` or `rd == rt` for signed `div`), `mflo rd` / `mfhi rd` stays after the checks so it does not clobber `rs` or `rt` before the zero/overflow checks test it.

`tools/sdk_aspsx_pass.py` models these three workstation assembler rules uniformly on `maspsx` output in `tools/cc.py` (`compile_gcc`), with unit tests in `tools/test_sdk_aspsx_pass.py`.

## Historical T-11800 / T-11890 notes and SDK survey (`tools/aspsx_survey.py`)
Measured with `tools/docker.sh python3 tools/aspsx_survey.py` over every `asm/lib*.s` (the PsyQ library code of the
main exe, all ASPSX-assembled except `libc_sprintf`, an older non-gcc build, and the hand-written libgte matrix code).
Counts are sites; examples are in the script output.

**1. Quotient/remainder move of an expanded divide (not a uniform rule).** 67 expanded sites with a move (14 more
`div`/`divu $zero` sites have no check sequence, one has no move):

| site | early (move right after `div $zero`) | late (move after the checks) |
|---|---|---|
| `div`, `mflo`, rd = rs | 0 | 37 |
| `div`, `mfhi`, rd = rt | 0 | 1 |
| `div`, `mfhi` (rem), rd distinct | 10 | 4 (all in libspu `func_80098D00`, rt = `$t7` in each) |
| `div`, `mflo` (quotient), rd distinct | 0 | 0 |
| `divu`, `mflo`/`mfhi`, rd = rs | 7 | 0 |
| `divu`, `mflo`/`mfhi`, rd distinct | 8 | 0 |

The candidate rule "the move goes right after the divide unless the checks read rd (rs and rt for `div`, rt for
`divu`), otherwise after them" fits 63 of 67 sites. Against it: the 4 late `rem` sites (one of them, libspu `80098F2C`,
has nothing else inside the expansion); no SDK site at all for the case TT needs (signed quotient, rd distinct); and
the ASPSX fixture in the pinned maspsx (`aspsx/fixtures/div.yaml`, real ASPSX output for `div $2,$4,$6`) puts the
`mflo` after the checks in every version from 1.07 to 2.86. In 44 of the 67 sites ASPSX has moved neighbouring
instructions into the expansion (between `div` and `bnez`, or between `break 6` and the move), so the move position
looked like the work of an ASPSX instruction scheduler; the real ASPSX (next section) shows it is not ASPSX at all.
See "The real ASPSX on synthetic input" below.

**2. `mflo`/`mfhi` hazard nops (uniform in the SDK asm; not ASPSX, see below).** Rule: the two instruction slots after an `mflo`/`mfhi` hold no
`mult`/`div`-class instruction, and the window is not cut by a label or by a branch from a gcc `.set noreorder`
block (delay slot filled by gcc); the missing nops sit right before that boundary. A branch whose delay slot
ASPSX filled with a nop closes the window itself (branch + nop = two slots). 290 of 290 sites as predicted (153
before mult/div, 15 before a label, 21 before a gcc-filled branch, 30 before an ASPSX-filled branch, 70 with no
boundary in reach; the one site with a nop and no boundary, libspu `800996A4`, is a load-delay nop). Not counted:
3 sites in `libc_sprintf` and the hand-written libgte `mtx` files (no ASPSX padding at all). An early move inside a
divide expansion is followed by the checks (`bnez; nop`), which close the window (all 25 early moves fit). maspsx pads only
before mult/div. It also puts a nop between a late move and a reader of rd (`Reuse of`); the SDK has 16 late moves
followed directly by a reader of rd and none with a nop.

**3. `lb/lbu/lh/lhu/lw rd,sym(rs)` (uniform in the SDK asm; not ASPSX, see below).** 103 of 103 loads with rd != rs expand through rd (`lui rd,%hi(sym);
addu rd,rd,rs; l* rd,%lo(sym)(rd)`); 53 of 53 loads with rd == rs and 99 of 99 stores go through `$at`, as maspsx does
for all of them. The maspsx fixture `expand_lw.yaml` (`lw $2,ctlbuf($2)`, rd == rs) agrees.

## The real ASPSX on synthetic input (T-11890)
The ASPSX binaries of the maspsx author's release (`aspsx-binaries.tar.gz`, 13 versions) were run under wine in a throwaway
container outside the repo (measurement only; no binary or output is committed). Versions 2.56, 2.67, 2.77, 2.79,
2.81 and 2.86 (the Win32 ones; 1.05 to 2.34 need dosemu2 and were not run: maspsx's fixtures show that versions
before 2.30 do not pad before `mult` at all, which the SDK does 153 times) give the same output for everything below.

- **Divide (36 synthetic cases, x 2 versions):** `div`, `divu` and `rem` with rd distinct, rd = rs and rd = rt, each
  followed by a reader of rd, an independent instruction, a label, or a gcc `.set noreorder` branch. In every case
  the move comes after the checks, and a reader of rd gets a nop after the move (`mflo v0; nop; addu v1,v0,a1`).
  The SDK has 16 late moves directly followed by a reader of rd and no nop, and moves before the checks in 25 sites.
- **Hazard nops:** `mflo` then `mult` gets two nops (as maspsx); `mflo` before a label, before a gcc noreorder
  branch, or before a reorder branch gets none.
- **`l* rd,sym(rs)`:** every load (`lh`, `lw`, `lbu`, `lwl`, rd = rs or not) and every store goes through `$at`.
- **The function itself:** the gcc 2.7.2-psx output of the draft C assembles to the same 96 instructions in all six
  versions, equal to the maspsx path apart from relocation fields; the original has 97.

So maspsx models the real ASPSX correctly here, and none of the three habits of the SDK asm and TT `func_8014D260` is
ASPSX behaviour on gcc's macro output (the accepted options `-a -f -g -u -v -x -D -K -L -R -W -q` change nothing; `-0`
drops the checks). Most likely the original division code, address loads and nops came out of the
compiler already expanded (explicit `div $zero`, checks and `mflo` that the compiler scheduled; explicit
`%hi`/`%lo` through the destination; the compiler's own hazard nops), from a compiler build none of the old-gcc
releases we have reproduces (2.6.3 to 2.95.2 all print the `div rd,rs,rt` macro or `div $0` + `mflo` without checks).
An ASPSX emulation pass would model something ASPSX does not do, so none is built.

## Sony/SN cc1psx (PsyQ 3.3 to 4.6) on the same input (T-11890)
The PsyQ compiler binaries that decomp.me uses (mkst/esa release `psyq-binaries`: `psyq-compilers.tar.gz` with 4.0,
4.1, 4.3, 4.5, 4.6, and `psyq3.3`/`psyq3.5`/`psyq3.6`) were run in throwaway containers outside the repo
(measurement only, nothing committed): the Win32 4.x `CC1PSX.EXE` + `ASPSX.EXE` under wine, the DOS 3.x ones under
dosemu2 with the archive's `dosemurc`. Input: the ratan2 draft preprocessed with old-gcc cpp, and small synthetic C
functions (quotient and remainder with rd distinct and rd = rs, unsigned divide, a `short` table load by index,
`mult` + `mflo` before a branch).

| PsyQ | cc1psx | ratan2 (original: 97 instructions) |
|---|---|---|
| 3.3 | GNU C 2.6.0 (SN) | 98, 41 differing words (ASPSX 2.21: `li` as `ori`, `addiu $at` load form) |
| 3.5, 3.6 | GNU C 2.6.0 (SN) | 96, 41 differing words |
| 4.0 | GNU C 2.7.2.SN32.3.7.0002 | 96, identical to our gcc 2.7.2-psx + maspsx path (relocations aside) |
| 4.1, 4.3, 4.5 | cygnus-2.7.2 SN32 | 95, more different (compiler splits `%hi`/`%lo` and indexes through another register) |
| 4.6 | gcc 2.8 class | 71 |

What the compilers print (`.s`, all versions; `-O1`, `-O3`, `-mno-gas`, `-fschedule-insns`, `-mips2`,
`-membedded-pic`, `-mlong-calls` change nothing for 4.0):
- 3.3 to 4.5 print the `div rd,rs,rt` macro; ASPSX expands it with the move after the checks, as measured above.
  4.6 prints `div $0,rs,rt; mflo rd` and only a zero check (`bne rt,$0,1f; nop; break 7`), without the overflow check
  the original has.
- 3.3 to 4.0 print `lh rd,sym(rs)`, which ASPSX expands through `$at`; 4.1 and later split the address with another
  register (`lui`/`addiu` of the full address, then `addu` with the index and `lh rd,0(...)`). None gives the
  original's `lui rd,%hi; addu rd,rd,rs; l* rd,%lo(sym)(rd)`.
- No version prints its own hazard nops; `mflo` before a branch or label gets none after ASPSX.

No PsyQ compiler from 3.3 to 4.6 produces the original `func_8014D260` or the three SDK habits, so the question of
TAIIKU `func_801488F0` under one of them does not arise. Where the original's code generator comes from stays open
(not tried: other cc1psx builds, such as the `psyq_263_221` set decomp.me lists, and the libraries' own object files).

## Old ASPSX and the PsyQ library signatures (T-11890, issue #63)
**DOS ASPSX 1.05 to 2.34** (all DOS versions in the maspsx author's archive; there is no 2.31, 2.32 or anything
between 2.34 and 2.56), run under dosemu2 in a throwaway container. Input: the gcc 2.7.2-psx `.s` and the PsyQ 4.0
cc1psx `.s` of the ratan2 draft, plus synthetic cases (quotient with rd distinct and rd = rs, `divu`, each before a
reader or a noreorder branch; `mflo` before a label, a noreorder branch and `mult`; symbol+register loads):
- ratan2: 1.05, 1.07, 2.21 give 98 instructions (41 words differ from TT), 2.05 and 2.08 give 98 (49), 2.30 and 2.34
  give 96 (41). 2.30/2.34 equal 2.56 to 2.86 except `li` as `ori`. No version gives TT's 97.
- Divide: every version puts the move after the checks with a nop before a reader of rd (2.05 and 2.08 use `tge`
  instead of `break 6`). Hazard nops: none before a label or a branch in any version; before `mult`, two nops from
  2.30 on and none before. Loads: always through `$at` (`lui; addiu; addu` before 2.30).
None of the tested ASPSX versions reproduced the three habits on these inputs. The SDK asm cannot come from a pre-2.30 ASPSX anyway:
it has the `mflo`/`mult` pads (153 sites) and the `lui; addu` load form those versions do not produce.

**Main exe SDK functions vs the lab313ru/psx_psyq_signatures sets** (12 libraries x 16 releases, 2.60 to 4.70;
function ranges cut at named labels, `loc_`/`text_` labels are inside functions). Each of the 722 functions under
`asm/lib*.s` is compared exactly (signature wildcards ignored), then after normalising the three habits away (nops
dropped, divide checks dropped, `lui; addu; l*` through `$at` or rd folded), then as the same normalised instruction
multiset in another order:

| lib | functions | matched any way | best releases (functions matched) |
|---|---|---|---|
| libapi | 66 | 66 | all (60 to 66) |
| libgte | 198 | 174 | 3.30 to 3.61 (169 to 171); 3.70/4.00: 135 |
| libgpu | 96 | 57 | 3.70, 4.00 (53); 4.10+: 44 to 46; 3.x: 9 to 23 |
| libcd | 65 | 35 | 3.70 (26), 4.00 (24); 4.10+: 22 |
| libetc | 31 | 22 | 3.70, 4.00 (20); 3.30 to 3.61: 17 |
| libgs | 36 | 24 | 3.00 to 4.70 (17 or 18 each) |
| libpress | 22 | 12 | 3.30, 3.40 (10) |
| libsnd | 142 | 55 | 3.00 (38); 4.10/4.20 (33); 3.70/4.00 (27) |
| libspu | 63 | 11 | 3.70, 4.00 (9) |
| libc, libcard | 3 | 3 | 3.30 |

- The game's SDK is not one release: the mixed vintages of [[psyq-sdk]] show again (libgte and libc 3.3-era, libgpu,
  libcd, libetc 3.7/4.0-era), and libsnd/libspu match badly everywhere (39 % and 17 %).
- 39 functions carry a habit (an rd-through `lui; addu; l*` or an early divide move). 4 are hand-written libgte code
  that matches the signatures exactly (`RotMatrix`, `SquareRoot0`, `SquareRoot12` and one more; Sony's own asm uses the
  rd form). Of the compiled ones, 2 differ from a signature only at the habit spots (libgpu `func_8009E108`: 3.50 to
  4.00; TT `func_8014D260`: 3.70/4.00), 2 more also in instruction order (libcd `func_800893F0`: 3.50 to 4.00; libsnd
  `func_80090EDC`: 3.50), and 31 (23 in libsnd) differ more.
- Many compiled functions without habits are the official code in another instruction order (e.g. libgs
  `GsInitCoordinate2`, 3.00: the word copies of a matrix in another order). That is a compiler difference, not an
  assembler one, which suggests a different compiler build or configuration around 3.5 to 4.0 sources. The tested tools did not reproduce it; this does not exhaust every build, source form or configuration.

## Build integration
- `config/toolchains.txt` names the toolchain of an object (`<subsegment name> gcc <ver> <aspsx>`); `configure.py` validates it and passes it to `tools/cc.py`. The gcc path has no asm-processor, so only objects whose functions are all C belong there. Test: `tools/test_configure_toolchains.py`.
- `tools/cc.py` runs `maspsx` with `--expand-div`, followed uniformly by `tools/sdk_aspsx_pass.py` (T-24086; tests in `tools/test_sdk_aspsx_pass.py`), which models Sony's workstation assembler (`as0` + `as1` derivative) on `maspsx` output (`l* rd,sym(rs)` through `rd` when `rd != rs`, WAR-free early `mflo`/`mfhi` in expanded divides, and the two-slot `mflo`/`mfhi` hazard window before labels and `.set noreorder` branches).
- gcc in the arm64 image: [[toolchain]], "Old gcc on arm64 (T-9200)".

