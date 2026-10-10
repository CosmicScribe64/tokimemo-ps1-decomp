---
type: concept
updated: 2026-10-09
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
---

# Matching notes

Workflow: [[decompile-workflow]].

## Compiler verdict (T-0013): MIPS ucode compiler, IDO-family; IDO 5.3 is the closest available
Findings from [[tickets/T-0011-game-code-file-boundaries-and-compiler]] and [[tickets/T-0013-identify-original-compiler-pipeline]]. Toolchain details: [[toolchain]].

The game code was compiled by a MIPS/SGI ucode compiler (cfe/uopt/ugen/as1, the IDO family), not by gcc. SGI IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared -Wo,-nokpicopt`, via asm-processor) is now the build compiler for the game code (`src/main/*.c`); the SDK libs region stays gcc/ASPSX territory ([[psyq-sdk]]).

Evidence (IDO reproduces each original signature that no gcc 2.6 to 2.95 + maspsx could):

| original | IDO 5.3 | gcc + maspsx |
|---|---|---|
| `li t6,4; lui at,%hi(X); jr ra; sb t6,%lo(X)(at)` | identical (as1 splits the `sb t6,X` macro and moves the store into the `jr` slot) | `jr ra; nop` or explicit `lui v1` |
| `$t6`,`$t7`,`$t8`,`$t9`,`$t0`.. as expression temps | identical (ugen temps start at `$14`) | `$v0`/`$v1` first |
| `lh t6,X; lui at; jr ra; sh t6,Y(at)` | identical | `lhu` |
| `or rd,rs,zero` for move, `addiu rt,zero,imm` for li | identical (as1 encodings) | `addu` |
| loads never in the `jr` slot, `jr ra; nop` after a getter | identical (as1 respects the MIPS I load delay across the return) | same |
| prologue save order `sw s2; sw ra; sw s1; sw s0` (`func_80042134`) | IDO's order | not compared |

Matched with IDO 5.3 in the build (23): the 10 getters and 3 empty functions from T-0011, plus `draw2d3d`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `get_ksys`, `k_sub_reset_point_set` (7 of the 8 functions gcc could not match), `func_8004E750`, `k_disp_switch` and `func_80042400` (T-0014). IDO 7.1 matches the same set except `func_8004E750` (it re-uses `$a0` for the `andi` instead of `$t6`), so the original is 5.3-like or older.

### Global-address CSE: solved with `-Wo,-nokpicopt` (T-0014, T-0017)
`func_80042400` (`D += 0x377`, value returned) is `lui v1; lw v1,%lo(D)(v1); lui at; addiu v0,v1,0x377; jr ra; sw v0,%lo(D)(at)` in the original; stock IDO 5.3/7.1 keep `&D` in a register (`addiu a0,a0,%lo(D)`, `lw v1,0(a0)`). uopt's undocumented option `-no_const_in_reg` (found in the uopt option table, passed as `-Wo,-no_const_in_reg`) stops uopt from keeping constants, including global addresses, in registers; with it the function byte-matches, and all 22 earlier matches still match. Reading: the original uopt did not do this optimization (an older uopt, or one built/configured without it).

Update (T-0017): `-no_const_in_reg` was too broad: it also stops uopt keeping integer constants and array base addresses in registers, which the original does (loop bounds, multipliers). `-Wo,-nokpicopt` stops only the address of a directly accessed scalar global from being kept in a register, so it gives both; `IDO_CFLAGS` now uses it instead. Under it `func_80042400` is plain `D += 0x377; return D;` (the named-temp form gives `$a0`). Evidence and the regression check are in the section "Constants in registers" below.

### Frame size: measured (T-0014); reproduced by the T-0016 pass (next section)
Every function with a stack frame is 16 bytes larger in the original than in IDO 5.3 (147 non-leaf functions start `addiu sp,-0x28; sw ra,0x14(sp)` where IDO gives `-0x18`). Where the 16 bytes go, measured against IDO 5.3 output for the same or equivalent C:

| kind | IDO 5.3 layout (from sp up) | original | example |
|---|---|---|---|
| non-leaf | args 16, saves, locals/temps | args 16, saves, **16**, locals/temps | `func_80041584` 0x18 -> 0x28; `func_80042134` saves at 0x18-0x24, frame 0x28 -> 0x38; `func_8004111C` RECT at 0x18 -> 0x28 |
| leaf with saves | saves, locals | **16**, saves, locals | `SD_DetectCDPeak` s0/s1 at 0x8/0xC in 0x10 -> 0x18/0x1C in 0x20; `get_h_tokimeki` s0 at 0x14 (IDO-equivalent: 0x4) |
| leaf, locals only | locals | **16**, locals | `strSync` volatile local at 0x4 in 0x8 -> 0x14 in 0x18 |
| leaf, no locals/saves | no frame | no frame | the getters |

The 16 bytes are never accessed (checked over all 636 functions with a frame; the only hits are incoming-argument homes above the frame and one outlier, `make_color_bar16`, which saves s0-s7/fp/ra at non-IDO offsets and may be hand-written). So leaf functions behave as if they always had the 16-byte argument build area that IDO omits in leaves, and non-leaf functions have one extra 16-byte block between the register saves and the locals/temps.

Experiments (all IDO 5.3 and 7.1, `-O2` unless noted; none changes the frame): `-g`, `-g1`, `-g3`, `-O0/-O1/-O3`, `-p`, `-32`, `-mips2`, `-KPIC`/`-call_shared`, `-xansi`, `-cckr`, `-ansi`, `-prototypes`, `-Olimit`, `-framepointer` (adds an s8 save, +24), every cfe option in its option table (`-Wf,-saveargs,-checkstack,-volatile,-Xvolatile,-check_bounds,-std0,-std1,-oldcomment,-trapuv,-use_readonly_const,-d1,-msft,-cplus,-filter,-inf,-Xfloat`), every uopt option in its table (`-Wo,-no_const_in_reg,-do_opt_saved_regs,-noheurAB,-norlodrstropt,-noprecolor,-doassoc,-docopy,-nogenvreg,-norecur,-docodehoist,-notail,-nordstore,-createbb,-moremotion,-noPalias,-static,-varref,-nokpicopt,-kpicopt,-loopunroll`), every ugen option in its table (`-Wc,-notailopt,-align8/16/32/64,-nooffsetopt,-nocpalias,-cpalias,-trapuv,-nounsignedconv,-domtag,-mips2`; `-checkstack` adds 4 and uses s7). Option tables were read from the recomp binaries' data (words byte-swapped).

Diagnostic (not a fix): in ugen (read from the matching decomp of IDO 7.1 ugen, LLONSIT/ido-decomp, `src/ugen/eval.p`) the frame is `saved_regs + arg_build + local_var_size (DEF Mmt length from uopt) + temps`, and leaves get no arg build area. Running cfe+uopt, adding 16 to the `DEF Mmt` length in the binary ucode, then ugen+as1 reproduces the non-leaf layout exactly: `func_80041584` and `func_8004111C` then byte-match (the rest of the code is plain IDO 5.3). It does not reproduce leaf functions (the extra block lands above the saves instead of below, and getters gain a frame). A C-level "unused `char pad[16]` declared last" also fixes `func_8004111C`, but only works when the function already has an address-taken local (uopt drops unused locals otherwise). Both are fakematches and are not used.

Ranked hypotheses for the exact compiler:
1. An older MIPS ucode compiler (MIPS RISCompiler 2.x/3.x as in Ultrix 4.x or Sony NEWS-OS `cc`, or IRIX 4/early-5 IDO) built or run for little-endian R3000. Fits: same cfe/uopt/ugen/as1 family; the leaf pattern is exactly "no pseudo-leaf frame optimization" (an older ugen feature level); `const_in_reg` absent in its uopt; 1995 date. Untested: no such compiler is available as a static recompilation (decompals/ido-static-recomp ships only 5.3 and 7.1; nothing else found on GitHub); running one needs an OS image (Ultrix under gxemul, NEWS-OS, IRIX 4/5 under an emulator).
2. IDO 5.x (5.0-5.2) rather than 5.3: possible, same test blocker (IRIX 5.x binaries would run under qemu-irix).
3. A hidden option or a vendor patch of IDO 5.3: unlikely; every option in all three pass tables was tried.
4. gcc of any version: ruled out (T-0013).

Next experiments (need a decision on obtaining OS images, see [[tickets/T-0100-older-mips-compiler-emulation]]): get an Ultrix 4.x or IRIX 5.2 compiler running (gxemul / qemu-irix in Docker) and compile `func_80041584`, `SD_DetectCDPeak`, `strSync`, `func_80042400` with `-EL -O2 -G 0`.

Update (T-0016): framed functions are no longer parked. The uniform frame pass below is part of every IDO compile, so non-leaf functions and leaf functions with frames are decompiled like any other. The `DEF Mmt +16` diagnostic above was replaced by a pass on ugen's output, because the ucode route cannot express the leaf layout and puts the hole in the wrong place when spill temporaries exist.

## Frame layout emulation (T-0016)

Update (T-3110): IDO 4.1's ugen produces this layout natively. With IDO 5.2's front end and uopt and 4.1's ugen, all 2564 matched C functions have exactly the frame pass's frame sizes and `$sp` offsets; 80 differ only in register choice ([[ido-52-evaluation]]).

Pass: `tools/frame_pass.py`, run by `tools/cc.py` on every IDO compile ([[toolchain]]). Ticket: [[tickets/T-0016-frame-layout-emulation-pass]]. This is a toolchain emulation pass, not a fakematch (CODING_STANDARDS section 7a): one rule, no function lists, ordinary C.

### The rule
Every function that has a stack frame gets F' = F + 16, and a 16-byte hole is inserted at offset H; every `$sp`-relative offset >= H moves up by 16 (locals, spill temporaries, address-of-local computations, and the incoming-argument homes above the frame).
- Function saves `$ra` (every non-leaf function, plus leaf functions that use `$ra` as a register): H = end of the register-save block. Outgoing-argument area and saves keep IDO's offsets; the hole sits between the saves and the locals/temporaries.
- Function does not save `$ra` (leaf): H = 0. The whole frame, saves included, moves up 16; the hole is at the bottom.
- Frameless functions are untouched.

Interpretation (not checked against the original compiler's source): IDO's ugen anchors locals and temporaries at the top of the frame and the saves above the argument-build area; the original compiler adds one 16-byte pad that, in functions with an argument-build area, sits above the saves, and in pseudo-leaf functions below everything. Which of the two positions applies is decided by `$ra` being saved, which is a property of the function body, not a choice.

### Evidence from the original code
Corpus: every function of the main game segment and the 26 overlays with a frame (first instruction `addiu $sp,$sp,-F`): 3312 of 6891 functions, split by whether `$ra` is saved and whether a call exists.

| class | functions | invariant checked | result |
|---|---|---|---|
| N: non-leaf | 3286 (198 with `$fp`, 634 with s-registers, 914 with locals, 35 with all four argument homes written, 253 with incoming-argument homes) | no stack access inside [T, T+16), T = highest save slot + 4 | 0 violations |
| N, no locals | 2372 | F = T + 16 exactly | 2371; 1 exception (below) |
| N with locals | 914 | lowest local/temp offset >= T + 16 | 914 of 914 (301 start exactly at T+16) |
| L: leaf, no `$ra` save | 24 (12 with s-registers) | no access in [0, 16) | 24 of 24; lowest access 0x10 to 0x2c |
| R: leaf, `$ra` saved | 2 (`make_color_bar16`, `func_80135600`: IDO's high-register-pressure pattern, saves from offset 8 like IDO leaf code) | saves at IDO offsets, hole after them | `make_color_bar16` first local at T+16; `func_80135600` has no access near the hole (weak) |

The single exception, `func_801488F0` (TAIIKU, F = T, no hole): the prologue stores `s0` before the `move s0,$a0` and the epilogue fills the `jr` slot with the stack release, which is gcc scheduling, not IDO. It is a gcc-compiled function inside an overlay and cannot be produced by IDO anyway, so it is outside the pass's domain. A search for others (functions with locals but no gap, hidden in the 914) found none.

Sample of 33 functions (the rest are in the same pattern; `s-reg` counts include `$fp`; "IDO F" is the frame IDO produces for the same body, original minus 16):

| function | class | original F | save slots | save block end T | hole | first use above hole | IDO F |
|---|---|---|---|---|---|---|---|
| func_8013E464 (SHOUGATU) | N | 0x28 | ra | 0x18 | 0x18-0x27 | none (F = T+16) | 0x18 |
| func_8013A564 (GEKO) | N | 0x28 | ra | 0x18 | 0x18-0x27 | none | 0x18 |
| func_801351B8 (KANGEI) | N | 0x28 | ra | 0x18 | 0x18-0x27 | none | 0x18 |
| func_800853FC | N | 0x30 | ra+1 s-reg | 0x20 | 0x20-0x2f | none | 0x20 |
| get_g_zyotai_s | N | 0x30 | ra+1 s-reg | 0x20 | 0x20-0x2f | none | 0x20 |
| func_8013DEA0 (ETC) | N | 0x38 | ra+2 s-reg | 0x28 | 0x28-0x37 | none | 0x28 |
| func_8013E1DC (ETC) | N | 0x38 | ra+2 s-reg | 0x28 | 0x28-0x37 | none | 0x28 |
| func_801493D4 (TACO) | N | 0x48 | ra+6 s-reg | 0x38 | 0x38-0x47 | none | 0x38 |
| func_80148B58 (TACO) | N | 0x48 | ra+6 s-reg | 0x38 | 0x38-0x47 | none | 0x38 |
| func_80136F3C (BUNKA_SD) | N | 0x58 | ra+9 s-reg incl. fp | 0x48 | 0x48-0x57 | none | 0x48 |
| func_80135E50 (DATE2) | N | 0x60 | ra+9 s-reg incl. fp | 0x50 | 0x50-0x5f | none | 0x50 |
| func_8006B900 | N | 0x50 | ra+4 s-reg | 0x40 | 0x40-0x4f | none | 0x40 |
| func_80082764 | N | 0x38 | ra | 0x28 | 0x28-0x37 | none | 0x28 |
| func_8013E934 (RPG_BAT) | N | 0x30 | ra | 0x20 | 0x20-0x2f | none | 0x20 |
| func_8013F0DC (NAME_ENT) | N | 0x38 | ra | 0x18 | 0x18-0x27 | 0x2c | 0x28 |
| func_800F86C4 (EVENT) | N | 0xb8 | ra | 0x18 | 0x18-0x27 | 0x28 | 0xa8 |
| func_80134824 (SHUGAKU) | N | 0x30 | ra | 0x18 | 0x18-0x27 | 0x2b | 0x20 |
| func_801400B0 (TT) | N | 0x88 | ra+3 s-reg | 0x30 | 0x30-0x3f | 0x5c | 0x78 |
| func_801345CC (OLH) | N | 0xc0 | ra+9 s-reg incl. fp | 0x48 | 0x48-0x57 | 0x74 | 0xb0 |
| func_80054AF4 | N | 0x60 | ra+4 s-reg | 0x28 | 0x28-0x37 | 0x44 | 0x50 |
| x_taku_menu_set | N | 0x70 | ra+1 s-reg | 0x28 | 0x28-0x37 | 0x50 | 0x60 |
| func_80140624 (TT) | N | 0x80 | ra+3 s-reg | 0x30 | 0x30-0x3f | 0x4c | 0x70 |
| func_80136810 (KANGEI) | N, argument homes | 0x30 | ra | 0x18 | 0x18-0x27 | 0x28 | 0x20 |
| func_8004A414 | N, argument homes, `$fp` | 0x98 | ra+9 s-reg incl. fp | 0x40 | 0x40-0x4f | 0x50 | 0x88 |
| strSync | L | 0x18 | none | - | 0x00-0x0f | 0x14 | 0x08 |
| func_8013815C (TAIIKU) | L | 0x28 | none | - | 0x00-0x0f | 0x1c | 0x18 |
| func_80146FA0 (TAIIKU) | L | 0x28 | none | - | 0x00-0x0f | 0x1c | 0x18 |
| func_801464D4 (ETC) | L | 0x18 | 1 s-reg | - | 0x00-0x0f | 0x14 | 0x08 |
| func_80138B90 (TT) | L | 0x28 | 4 s-reg | - | 0x00-0x0f | 0x18 | 0x18 |
| func_80137C44 (TAIIKU) | L | 0x20 | 3 s-reg | - | 0x00-0x0f | 0x14 | 0x10 |
| func_80144B40 (TAIIKU) | L | 0x40 | 2 s-reg | - | 0x00-0x0f | 0x18 | 0x30 |
| make_color_bar16 | R | 0xb8 | ra+9 s-reg incl. fp | 0x30 | 0x30-0x3f | 0x40 | 0xa8 |
| func_80135600 (EN_NICHI) | R | 0x190 | ra+9 s-reg incl. fp | 0x30 | 0x30-0x3f | 0x84 | 0x180 |

### Proof by building (IDO 5.3 + pass, byte-compared to the original)
- 12 non-leaf functions now match in the game code (`src/main/*.c`): `func_80041584`, `func_80041878`, `func_80042458`, `func_8004B338`, `birth_day_check`, `restore_bgm`, `magazine_exit`, `func_8007B5CC`, `change_dec_bg`, `func_80083378` (no locals) and `func_800462C8`, `func_80046318` (a 0x20-byte local buffer plus argument homes). All 63 earlier matches still match; `ninja progress` 75/834.
- 3 leaf functions with frames, `func_8013815C`, `func_801446A0`, `func_80146FA0` (TAIIKU), match byte for byte (prologue, local at 0x1c, restore) but only with uopt allowed to keep constants in registers, which the project flag forbids: see "Findings that are not the frame". They are in `src/ovl/TAIIKU.c` under `NON_MATCHING` (T-0016). Reproduce: compile `src/ovl/TAIIKU.c` with `-DNON_MATCHING` and without `-Wo,-no_const_in_reg` and compare with `tools/funcdiff.py --expected <original TAIIKU.o>`.
- `strSync` and `SD_DetectCDPeak` (main) show the leaf layout in their first instructions (`sw s1,0x1c(sp); sw s0,0x18(sp)` in a 0x20 frame; local at 0x14 in 0x18) but differ for the reasons below.
- Pass unit tests: `tools/docker.sh python3 tools/test_frame_pass.py` (19 tests: synthetic binasm records plus snippets compiled by the real IDO).

### Limits
- The pass changes layout only. Register allocation, scheduling and uopt heuristics are separate.
- An `addu rd,$sp,rs` index register is followed within its basic block; use of such a register after a label, any other consumer of an `$sp`-derived register, a frame-pointer frame (`alloca`), saved float registers, and unknown binasm records make the build fail with `frame_pass.py: ...` (in the original, 3 of the 167 uses of such registers are after a label, so functions written like that would not build until the pass learns them).
- Local array sizes: the original's local offsets sometimes imply a larger Mmt block than the obvious C (`func_8013815C`: `s16 v[6]` for three used elements, extra 8 bytes in `func_8015B828` and `func_80044700`); these are source-level unknowns, not pass errors. The oversized array in the TAIIKU leaves is marked `FAKE` in `src/ovl/TAIIKU.c`.
- The rule is inferred from one compiler's output; a different class of function (alloca, float code) is absent from the corpus.

### Findings that are not the frame (separate tickets: [[tickets/T-0017-const-in-reg-loop-hoisting]], [[tickets/T-0018-ugen-temp-register-order]])
1. Solved by T-0017 (`-Wo,-nokpicopt`, see "Constants in registers"); the TAIIKU functions match. Original note: `-Wo,-no_const_in_reg` is too blunt. The original hoists loop-invariant integer constants and global addresses out of loops into registers (`func_8013815C`, `SD_DetectCDPeak`, `strSync`, `func_801464D4`: `li v1,1`, `lui/addiu` before the loop, `multu` by a register holding 0x44), but does not keep a global address in a register across straight-line code (`func_80042400`). Without the flag all C functions in the project still match except `func_80042400`; with it, loops do not. No single uopt option gives both (all 19 tried, one at a time). Needs a decision: drop the flag and give up `func_80042400`, or keep it.
2. `LoadSquare`, `StoreSquare`, `MoveSquare`: the original loads the u16 parameter homes into `t8,t9` / `t7,t8,t9`, IDO into `t0,t1` / `t8,t9,t0` (stock IDO without the pass gives the same). Register allocation, kept as `NON_MATCHING`.
3. `func_8004111C`: the original stores the RECT's h and w before x and y (`NON_MATCHING`). `func_80044700`: the original frame has 8 more bytes of locals than the body needs.
4. `strSync`: with `-Wo,-nokpicopt` only one delay slot differs: IDO's as1 fills the loop's back branch with the volatile load at the loop head, the original leaves a `nop` (T-0017).

## Object padding (T-0012)
The alignment nops after the last function of a source file (the reason `func_80043504` and `func_8006CAE0` once failed) are now produced by the uniform padding pass in `tools/cc.py`; rule, evidence and verification are in [[toolchain]] (section Object padding pass) and [[source-files]].

## Idioms
- Read-modify-write of a global that returns the new value (`func_80042400`): `D += k; return D;` (with `-Wo,-nokpicopt`, T-0017; the T-0014 named-temp form now gives `$a0`).
- A getter `T f(void) { return D; }` with `u8`/`s32` global matches; the global must be declared `extern` with its access width in `include/game.h` (types inferred from the load/store only).
- Several `D_` symbols are accessed with different widths in different functions (e.g. `D_800B3F60` as `lbu` in `check_set_k`, as halfword elsewhere). Do not declare a single type blindly; decide per symbol when more users are decompiled.
- The ninja depfile only lists `INCLUDE_ASM` files; `configure.py` lists every `include/*.h` and `include/*.inc` (by glob) as an implicit input of the C compiles, so new headers need no `configure.py` edit.

## Leaf batch 1 (T-0400): 40 more leaf functions matched
Tool: `tools/list_leaves.py` (now over all `src/main` files; prints the file as a third column) lists the remaining `INCLUDE_ASM` functions without `jal`/`jalr`, smallest first (177 outside 0x80080000-0x80086810 at the start; `[[tickets/T-0400-leaf-function-batch-1]]`). `ninja progress` after the batch: 40/812 functions, 1404/284028 bytes (these 40 are the whole count; the 22 from T-0011/T-0013 are not in that total).

Idioms that matched with IDO 5.3:
- Straight-line stores to several different globals: one `lui $at` per store, nothing else, matches plain C. The same global accessed again in a later basic block (branches) does NOT: IDO keeps `&D` in a register (see failures).
- Compare of two globals (rule from the `-no_const_in_reg` era; since T-0017 try both orders: `check_end_k` now needs `D_800B3F68 == D_800B3F62`, the source order of the loads, while `func_80045288` keeps its form): the operand written second in C is loaded first. `D_A == D_B` in the original (`lui t6,B; lui t7,A; lh t7,A; lh t6,B`) is written `if (D_A == D_B)` with the symbols swapped relative to the asm order (check_end_k, func_80045288).
- `u8` indexed arrays and `x * 12` strides: declare `extern u8 D_xxx[]` and write `p = D + i * 12; *(s32 *)(p + off)` (SetWorkBase, func_800625C0).
- 8-byte struct passed by value in `$a1/$a2` and copied field by field into an array element (set_k_work, `Entry8` in `include/game.h`); copying the whole struct gives `swl/swr`.
- `if (x) D = 1; else D = 0;` and `if (D == c) return 1; return 0;` match as written. `return` of `u32 sum >> 1` cast to `s16` needs the `(u32)` cast for `srl` (getCDlevel).
- Arg spill `jr ra; sw a0,0(sp)` (func_8004DAC4) is matched with `s32 *p = &arg0;`, marked FAKE.

Failures left as `INCLUDE_ASM` (all but the first two are the T-0014 compiler gap):
- func_80043504, func_8006CAE0: asm ends with alignment nops after the last `jr` (source-file boundary padding); plain C could not emit them. Solved by the file split (T-0012): `tools/cc.py` pads each object to 16 ([[source-files]]); `func_80043504` as an empty function was verified to match
- InitMouse: original shares one lui at between D_800E36C0 and +4 stores; IDO gives addiu base (struct/array) or two lui (separate symbols)
- func_80042940: original puts move v0,zero before the last sb (sb in jr slot); IDO puts move in slot; tried ret var, return a=0
- func_80053CC0: D++ on u8 global: original keeps lui/lbu without address CSE then second lui at (same family as func_80042400, T-0014)
- func_800634FC bustup_wink func_8004ADAC k_speed_set func_80067DD4 func_80067DFC: original repeats lui at for each access to the same global (no address CSE; IDO -O2 keeps &D in a register) and uses sltiu where IDO emits slti for u8 compares; -O1 removes the CSE but spills u8 args
- func_80046290 (and similar multi-store clusters): original shares one lui at over several stores into a global cluster (sym+off); IDO -O2 uses addiu base, even for struct/volatile; -O1 no match (T-0014 address CSE)
- func_80042908: same as func_80042940 (move v0 ordering before last sb)
- func_80059048: pointer-compare store loop in gcc-style regs (v1/a0/v0); IDO unrolls it
- func_80041840: original recomputes constant 1 in fresh temps (li t6,1 ... li t8,1), IDO CSEs it into one register
- func_8005352C: u16>>12 compare in v0/v1 with srl; IDO emits sra into t6
- message_disp_switch func_8006BD6C: global read-modify-write in two branches: original lui at per access, IDO keeps &D in a register
- Not tried: `func_80066A78`, `func_80066AC0` (`return 3`/`return 1` with a `nop` after `jr`, jump-target labels), `func_80066A2C`/`func_80066A84` (jump-table switches), `set_k_work`-style larger leaves, and the other 100+ leaves; most of those contain the same-global-twice pattern above.

## Overlay batch A (T-0700): RENSYU, OMIMAI, VALEN, MASTER
56 functions matched (MASTER 22, VALEN 15, OMIMAI 13, RENSYU 6), mostly small ones: register-table setters, state-step stubs and call sequences. m2c output (with `--target mipsel-gcc-c`) is close to the final C for these.

New patterns and limits:
- **Object-boundary padding inside an overlay.** One overlay `.c` holds several original objects; the zero words after a function that ends an object (end address not 16-aligned) live in that function's `.s`. When the function becomes C they vanish and shift everything after, so the C function is followed by `INCLUDE_ASM("src/ovl/pad", pad_<NAME>_<addr>)`, a tiny committed stub (`glabel`, `nop` x k, `endlabel`; no game data). asm-processor rejects a block of one instruction ("too short .text block"), so a function followed by exactly one `nop` (end address = 12 mod 16) stays `INCLUDE_ASM` (VALEN `func_80133C44`).
- **`include/ovl/*.h` is not an implicit ninja input** (`configure.py` globs only `include/*.h`). After editing an overlay header, `touch src/ovl/<NAME>.c` or the object is not rebuilt.
- **Externs outside the overlay and the main exe** (addresses at or above 0x80162000, e.g. 0x8019DDF8): write `*(s16 *)0x8019DDF8`, not an `extern` symbol. With a symbol IDO allocates `lui t9; lh t9` (same register), the original has `lui t9; lh t0`. Only the relocation differs (`funcdiff.py` shows DIFF; compare the linked bytes).
- **Compare operand order**: `D_A == expr` and `expr == D_B` forms differ in which side IDO loads first; try the swap when the first load lands in the wrong temp (OMIMAI `func_80132398`, MASTER `func_80135040`).
- **`u8` vs `s8` constants**: `D = 0x80` on an `s8` gives `li -128`, the original `li 128` (use `u8`).
- Functions with a `jr` jump table or a string literal cannot be built: the C object's own `.rodata` is not part of the link (overlay rodata is one asm blob until T-0500). They stay `INCLUDE_ASM` (e.g. RENSYU `func_801320E0`, `func_80133E5C`).
- Left `INCLUDE_ASM`, new gaps: (a) a local table of function pointers copied from the overlay rodata and indexed by `D_800E7389` (RENSYU `func_80132040`, MASTER `func_80132140`, `func_80133A2C`): the original block-copies through `at`/`t9` with `v0` holding the local's address and a frame 8 bytes bigger than IDO's (update T-3330: one more local declared before the table, see the section at the end); (b) a global timeout counter `if (D++ >= 0x400)` (OMIMAI `func_80132544`, `func_8013364C`, VALEN `func_801324B4`): the original keeps the old value in `v1` and increments in place, IDO uses a fresh temp (structure and `xori` match, registers do not); (c) `D |= 2` on a `u8` global (VALEN `func_80133B6C`): the original keeps `&D` in its own register (`lui t7; lbu t8,lo(t7)`), same family as the T-0014 address CSE gap; (d) state-machine functions that m2c renders with `goto` (OMIMAI `func_801323F8`, `func_80133500`).
## Overlay batch B (T-0750): TEL, OLH, EN_NICHI
12 functions matched (EN_NICHI 9, TEL 2, OLH 1; call sequences, one-global conditionals, a compare chain, a pure getter-style call). The rest of the three overlays is dominated by four patterns that IDO 5.3 plus the frame pass does not reproduce; the functions stay `INCLUDE_ASM`. Draft C came from `m2c`; verification by `funcdiff.py --expected expected/ovl/<NAME>.o`.

New patterns (not in the notes above):
- Compare-chain `switch` on a `u8` global (analysis in "Register promotion of globals" below, T-0018) (`D_800E738A`, `D_800E738D`, `D_800E7389`, about 60 functions: the OLH menu handlers, TEL and EN_NICHI state machines). The original loads the value into `$v1` (`lui v1; lbu v1`), IDO into `$v0`. In functions that return a value the original also has `or v0,v1,zero` in the delay slot of the first `beqz` (the value is `D` on the default path); IDO places that move in the last compare's slot or after the calls. Tried: `switch (D)`, `s32`/`u8` locals, `if` chains, `default:` first or last, `return D` at the end, `ret = D; switch (D)` (gives `ret` in `$v1` plus `move v1,v0` after each call). Examples: TEL `func_8013A40C` family, OLH `func_80136210` family, EN_NICHI `func_8013216C`.
- `switch` with 4 or more dense cases uses a jump table (`jtbl_*` in the overlay's rodata); a C switch would emit its own table in the C object's `.rodata`, which the overlay link does not place like the original. Not attempted (OLH `func_80132420` family, TEL `func_80132060`).
- Multiplication by a constant that needs three shifts or adds (`* 0x44`, EN_NICHI `func_801383A0` family, six hit-box tests): the original is `li t1,68; multu a1,t1; mflo`, IDO strength-reduces to `sll/addu/sll` (also for `u32`, for a struct array index and for `(a << 2) * 17`). Solved by T-0017: the multiplier is a constant kept in a register by uopt, so ugen sees a register multiply; with `-Wo,-nokpicopt` IDO 5.3 emits the same `li; multu` (see "Constants in registers"). `func_801383A0` itself still differs in which of `$v1`/`$a2` holds a value (T-0018).
- Alignment padding of one `nop` after the last `jr` (OLH `func_801323CC`, 0x50 bytes with one trailing nop): `INCLUDE_ASM` of a one-instruction pad fails in asm-processor ("too short .text block", minimum 2 instructions), so such functions cannot be ported until the pad is solved in the toolchain. Pads of 2 or more nops use `INCLUDE_ASM("src/ovl/pad", pad_<NAME>_<addr>)` (convention of T-0700).
- Scheduling and register differences on otherwise plain C, left as asm: EN_NICHI `func_80132278` (original keeps `sb` for the call's delay slot after the argument `lbu`), `func_8013556C` (`sll` after `lui`), `func_80132A74` (`abs` of a difference: the second `move` goes in the `bgez` delay slot), `func_80132ADC` (branch layout and `$t6/$t7` order of the two constants), `func_80132DC4` (read-modify-write of a global: the original reloads into `$t6` each time, IDO promotes it to `$v0`), `func_80136B10` (12-byte local table copy from rodata: the original local is at 0x28, IDO at 0x2c, so the local is probably declared larger), `func_80132040` (a short-lived `lh` temp gets its own register in the original), TEL `func_80139E3C` (index variable `srl` writes back into `$v0` in the original, IDO uses a new temp).
- Loops with a constant bound (`EN_NICHI func_80135F54`, TEL `func_8013BDDC`) are the known T-0017 case (`li a0,0xB` hoisted by the original).
## Main exe batch C (T-0800): 53 functions in src/main/80041000.c to 80059A20.c
Ticket [[tickets/T-0800-main-exe-batch-c]]. `ninja progress` main game 75/834 -> 128/834 (8136 bytes). About 200 functions of the range stay `INCLUDE_ASM` (loops, jump tables, large bodies, and the failure patterns below).

Idioms that matched with IDO 5.3 plus the frame pass:
- A `u8` parameter makes IDO emit `andi t6,a0,0xFF` at entry, and an unsigned compare of it needs a `(u32)` cast to give `sltiu` (not `slti`): `void k_speed_set(u8 a) { D = a; if ((u32)a >= 0x81) D = 0x7F; }`. This solved two T-0400 failures (`k_speed_set`, `func_8004ADAC`; the "sltiu where IDO emits slti" note above). Same cast for `u8` globals compared in `func_80053D24`, `load_palette` attempts.
- An 8-byte struct by value (`RECT rect` as parameter 2) arrives in `a1/a2` and its fields are read back with `lh` from the arg homes (`func_8004A14C`).
- A stack struct whose size is only known from the frame: `FileReq` (`u8 name[32]; s32 retry;`, size 0x24, in `include/game.h`) reproduces both the local address (0x2C) and the frame of `func_80054694`. A plain `u8 buf[32]` is 8-aligned by IDO and lands at 0x30 instead (arrays of 33..36 bytes land at 0x2C).
- `if (x) return a; return b;` chains instead of a `ret` variable: a `ret` local costs a `move` and a different frame (`get_g_zyotai_h`, `get_g_zyotai_s`).
- `*(s32 *)((u8 *)&D + -(idx * 4)) = v` gives `sll; negu; addu` (`dec_bg_cd_read`); `(&D)[-idx]` gives `negu; sll`.
- Calls with more arguments than the callee's visible prototype: declare the callee unprototyped in `include/game.h` (`func_8004500C`, `func_80044750`, `set_kanji_string`); passing the caller's own `a0..a2` through (`func_8004AE28`) reproduces a call that leaves `a0..a2` untouched.
- Reading a global into a local before a later store fixes the order of the `lui` instructions (`func_80042878`).
- `tools/funcdiff.py` finds the object by the first `src/main/*.c` that mentions the name; for a function that is also called from an earlier file use `--built build/src/main/<file>.o`.

Failure patterns (left as `INCLUDE_ASM`; T-0017 and T-0018 are the owners):
1. Stack-pointer adjust sunk into leading straight-line code: functions that start with global stores or a `D += 1` before the first call get `addiu sp` after the first few stores (`initLight`, `func_80054284`, `func_80054388`, `func_8005448C`, `func_80054590`). IDO puts it first.
2. Constants and addresses kept in registers across a branch or loop (T-0017): `get_h_tokimeki_table`/`get_h_yuukou_table` (`li s2,11`), `initCoordinate`, `func_80054AF4`, `func_80048E78`, `func_80059688`, `func_80048F64`, `func_800590CC`/`func_800591D8` (`li a0/a1` before the `beqz`), `func_8004E44C` (`lui v1,0xFF00` reused by two `and`), `func_800463E8`, `tpage_buf_clear`.
3. Spill slot of a temporary at F-8 instead of F-4: `func_8004AC18` and `func_800570B8` store the spilled value at 0x38 in a 0x40 frame, IDO + pass put it at 0x3C. Looks like a uniform layout rule (candidate for the frame pass), not a source difference.
4. Register choice for a long-lived variable or return value (T-0018): `GetWorkBase`, `MouseState`, `func_8004AD60` (`li v0,8` vs `li t8,8`), `func_800450F4`/`func_8004500C` (result kept in `v1`, original keeps `v0`), `func_80044C98`/`func_80044E8C`, `func_800422C8`.
5. RECT locals: the original emits the four halfword stores in the order `h, w, x, y` for the source order `x, y, w, h` (`func_80059308`); IDO keeps source order.
6. Already known: pairs of globals sharing one `lui $at` (`func_8004482C`, `menu_bar_color`, `dec_bg_reset`), `move v0,zero` before the last `sb` (`func_8004284C`, `func_80042808`), `u16 * s32` operand order of `multu` (`RangeMouse`), `u16 >> 12` temp in `v1` (`func_8005352C`), first-parameter spill (`load_palette`: `sw a0,0(sp)` that the original does not have), `D++` after a store scheduled above it (`func_80053CC0`).

## Overlay batch H (T-1040): OPTION, ENDING
Ticket [[tickets/T-1040-overlay-batch-h-option-ending]]. 41 functions matched (ENDING 29, OPTION 12 with the 2 earlier empty ones excluded); `ninja progress` grand total 245/6962 functions. Most are call sequences ending in `func_8004284C`, one-global conditionals and `func_80046318` loaders. Call targets must use the name that appears in the overlay asm (`func_8007ED84`, `func_80043914`, `func_800AE0F0`), not the O.BIN or SDK alias, or `funcdiff.py` shows a relocation-name DIFF.

New or reconfirmed patterns:
- `funcdiff.py` prints `-...` after a function that is followed by an `INCLUDE_ASM("src/ovl/pad", ...)` stub; that is the pad, not a diff. The sha1 check is the real test. A function with exactly one trailing nop (OPTION `func_8013A4FC`) cannot be ported (asm-processor minimum of 2).
- `u8` parameter: `void f(s16 a, u8 b) { if (b != 0xFF) D = b; }` gives `andi` (ENDING `func_801396A4`); an `s32` parameter masked into an `s8` local gives `sll/sra`.
- RECT stack local with all four fields written (ENDING `func_801337A4`) matches with `RECT rect;` in source order.
- Left as asm, same families as above: compare-chain `switch (D_800E738A)` (OPTION `func_8013447C`, `func_80135DE0`, `func_80136BC8` and the value-returning `func_8013B3BC` family; original loads into `v1`, IDO into `v0`); `D * 0x2800` copy helpers (ENDING `func_8013363C`, `func_80133738`: the original keeps the product in `v0`, IDO in `t6`); `D++ < 0x80` counter (ENDING `func_8013B5E4`: `v1`/`v0` swapped); `D & 2` tested as `sll 30; bgez` (ENDING `func_801334E4`); `(D | 3) & 0xFF` stored to a `u8` global keeps the `andi` in the original (ENDING `func_80137574`); first-use register of a byte global (ENDING `func_80139EC4` `a1`, OPTION `func_80132AB8`, `func_801387E4`).
## Overlay batch G (T-1030): BUNKA_SD, DATE2
63 functions matched (BUNKA_SD 28 of 98, DATE2 35 of 102), almost all call sequences, global setters, table fills and short conditionals. Details in [[tickets/T-1030-overlay-batch-g-bunka-sd-date2]].

Idioms that worked:
- Calls to main-exe functions that `build/main_names.ld` renames (`bg_read_sub2`, `dec_bg_show_switch`, `dec_bg_cd_read`, `get_g_zyotai_s`, `memcpy`, `k_reset`, ...): the overlay asm still says `func_XXXXXXXX`, so `funcdiff.py` prints a relocation-name DIFF on those lines only; the linked bytes match (ninja sha1 OK decides). Use the applied name in the C.
- `D += 1; if (D >= k)` on a global (BUNKA_SD `func_801364F0`) matches when the compare reads `D` again; a temp `t = D + 1` puts the value in a different register. Opposite case: `func_80135440` needs the explicit `t = D; if (t == 0) ...; t = t + 1;` (one load kept in `v0`).
- `u8` compares as `(u32)D >= 3 && (u32)D < 6` give `sltiu` (as in batch C); a `u8` constant store of 0x80 needs the global declared `u8`, not `s8`.
- A trailing single `nop` (end address 12 mod 16) cannot be ported (asm-processor 2-instruction minimum): BUNKA_SD `func_801369AC`, `func_8013785C`, `func_801344A8`, DATE2 `func_80136D14`, `func_80132670`.
- Pads of 2 or more nops are `src/ovl/pad/pad_<NAME>_<addr>.s` stubs; `funcdiff.py` shows a trailing `...` DIFF for such a function (the pad is a separate symbol in the built object); the sha1 check is the real test.

Failures left as `INCLUDE_ASM` (all T-0017/T-0018 families):
- `x * 0x44` struct-array index (BUNKA_SD `func_801341A0`, `func_801377D4`, `func_80138810`): `multu` vs shifts.
- Post-increment compare of a global (`if (D++ == 0)`, `if (D++ > 0)`, `D += 1; if (D >= 0x400)`): original keeps the old value in `v1` and the compare result in `v0`, IDO swaps them (BUNKA_SD `func_80138C34`, DATE2 `func_801329F0`, `func_80132A48`, `func_80132B08`).
- `u8` global compare-chain with a returned value (BUNKA_SD `func_80134540`, `func_80135160`, `func_801359B0`, `func_80136270`): `$v1` vs `$v0`, same as batch B.
- `memcpy(t + A, t + B, 0x1400)` with `t = D * 0x1400`: the original keeps `t` in `v0`, IDO in `t6` (DATE2 `func_80132E5C`, `func_80132FB0`, `func_8013301C`, `func_80132EC8`).
- Argument `D_dst = D_src` passed on: original loads the source straight into `a0` (DATE2 `func_801335E4`, `func_80132F48`); local function-pointer table copy (`func_80132DE0`, `func_80136660`, `func_801375D4`, `func_80138048`); jump-table switches and `printf` string literals (`func_80133A6C`, `func_801365DC`).
## Main exe batch F (T-1020): 58 functions in 80075320, 800789E0, 80079B10, 80085E30
Ticket [[tickets/T-1020-main-batch-f]]. `ninja progress` grand total 204 -> 262 functions (17108 bytes). Mostly call-chain and global-store functions; they match as plain C.

Idioms that matched:
- A callee that takes a `u16` or `u8` argument needs a prototype with that type, and the caller's own `u16` parameter, to get `andi t6,a0,0xFFFF; move a0,t6` (`func_80079B10`, `func_80083440`). With an unprototyped callee IDO picks `a1` for the copy.
- A shift by a `u16` global needs the `(u16)` cast to get `lhu` when the global is declared `s16` elsewhere (`func_80079E00`); `return (a & x) == 0 && (b & y) == 0` is written as nested `if (...== 0) { if (...== 0) return 1; } return 0;` for the original branch layout.
- `D = D + 1; if (... >= ...)` on a counter: write the sum into a named temp (`s32 t = D + 1; D = t; if ((u32)t >= (u32)n)`) to reuse `v0` (`wait_sub_sub`).
- A function that returns the global it just stored (`Vblnk_Timer`) is `s32 f(void) { D = expr; return D; }`; the reload is folded.
- Array of structs with stride 0x44 or 36 as `u8 *p = D + idx * N;` with `*(s16 *)(p + off)` (`Yubi`, `sprite_brightness`).
- A `switch`-less 0/1/default state dispatch compiles with `v0` for the selector in IDO, `v1` in the original (T-0018; `normal_date_move_place` and the four sibling `*_select` functions).

Failure patterns (left as `INCLUDE_ASM`):
1. Loop-invariant constants hoisted (T-0017): `Fade_Out_Main`/`Fade_In_Main` (`li v0,1` before the compare), `func_8007BFB8` (`lui v1,0x1000` before the `bgez`), `vram_bustup_clear`, `addr_init_*_sd` (`lui v1,0x801E` shared by three adds), `normal_date_girl_suddenin` (`li v1,0x80` reused).
2. Register order of two live temporaries (T-0018): `func_80086640` (swap of two globals, `v1` first), `func_80075FA0`, `func_8007B358`/`func_8007B3DC` (`t6/t7` vs `t7/t6`), `don_wait` and `k_disp_inc2` (post-increment or decrement counter with the compare kept in `v0`, the loaded value in `v1`), `normal_date_move_place`.
3. `func_8008667C`/`func_80083338`: `s32 t = f(); return t & 0x7F;` gives `andi t6,v0; move v0,t6`, the original has `move t6,v0; andi v0,t6`.
4. `normal_date_girl_in_init`: `move v0,zero` before the last `sb` (known pattern 6 of batch C).
5. `func_8007B5EC`: the original reloads the counter after the early return, IDO keeps it in a register.
## Overlay batch I (T-1050): KANGEI, SHUGAKU
105 functions matched (KANGEI 49, SHUGAKU 56) out of 334; `ninja progress` grand total 309/6962 functions. All are small (4 to 0x70 bytes): call sequences, constant stores, one-global conditionals, RECT locals. Function-name differences in `funcdiff.py` output (`func_8007ED84` expected vs `bg_read_sub2` built) are not code diffs: the overlay asm refers to the old names, the C to the applied names; the sha1 check is the real proof.

Left `INCLUDE_ASM` from this batch (new instances of known gaps):
- Load hoisted above an earlier store: IDO moves `lui/lbu` of the second global above the store to the first (`D |= 0x80; E = 3; F |= 0x80`, KANGEI `func_80135C18`, SHUGAKU `func_80135BB8`, `func_80135430`, KANGEI `func_801355F0`/`func_80135634`), the original keeps source order (and `li v0` before the delay-slot store in `func_80135BB8`). Same family as the `$at` sharing notes in batch C.
- `D++` compared or tested in the same expression (`if (D++ == 1 && E == 2)`, SHUGAKU `func_80134024`, `func_80134D8C`, KANGEI `func_80133A14`): the original keeps the old value in `$v1` and the `sp` adjust comes after the first load (T-0018, pattern (b) above).
- ugen temp skips `$t6` after a call (`get_g_zyotai_s(0) & 0x7F` lands in `$t7` in the original: KANGEI `func_80138320`, `func_801386C4`): T-0018. Same for `get_g_zyotai_h` result clamp to 3 (KANGEI `func_80132354`: the original fills the `bnez` slot with `andi a0,v0,0xFF`).
- A parameter-register load before the `sp` adjust (`lui a0; lbu a0` first, KANGEI `func_801325D0`, SHUGAKU `func_80132BCC`) is not reproduced; IDO uses `v0`/a copy through `a1`.
- `set_kanji_string` is unprototyped `void` in `include/game.h`, so its return value cannot be passed on (SHUGAKU `func_80138ADC`); needs a `game.h` change (not allowed in this batch).
- Idioms that matched: `*(s16 *)((u8 *)&D + idx * 0x38) = v` for a struct-array field with an unknown struct; `func_80046318(n, 0x801B0000, 0xAF0D)` style calls with constants as is; `RECT` locals in x, y, w, h order (KANGEI `func_801350E0`); a pointer-to-struct global (`D_80139AD0->unk_34->unk_08`) as two nested `KSub`/`KObj` pointers; `(a >= k1) + (a >= k2)` for a bool sum.
## Overlay batch K (T-1070): NAME_ENT, TT
76 functions matched (NAME_ENT 53, TT 23): state-gated call sequences (`if (D_800E738A == 0) f();` family, pad-button masks on `D_800E7208`), argument-heavy call sequences (`func_8004E788`, `func_80044750`), slot finders, small struct setters. Verification with `funcdiff.py --expected expected/ovl/<NAME>.o`.

New patterns:
- **Register numbers follow source order of the temporaries.** `func_8013BF68` (TT) matched only after the stores of constants were written before the `lh`/`sh` copies (`0xC00` got `t7`, loaded halfwords `t8`/`t9`). To fix a `t7`/`t8` swap, reorder statements before trying anything else.
- **Compare-chain `switch` on a `u16`/`u8` argument matches when `default:` shares the first case label**: `case 8: default: A; break; case 9: B; case 10: C;` gives `beq 8; beq 9; beq 10; fall into A` (TT `func_8014A8F8`). A `switch` on a `u16` field behind a global pointer (TT `func_8013BAD0`) also matches; only the `u8` global `D_800E738A`/`D_800E7389` switches keep the `v1`-vs-`v0` gap (NAME_ENT `func_80145A74`, `func_80132134`, `func_80144AD0`, `func_8013C4A8`, `func_80140CCC`).
- `u16` counters compared with `sltiu` need the `(u32)(u16)` cast on the pre-incremented value: `if ((u32)(u16)++*(u16 *)(p + 0x46) >= 0x79)` (TT `func_80149098`, `func_80146DC8`).
- A global pointer used for one or two accesses is written `D[off]` or `*(T *)(D + off)` and IDO loads it once into a `t` register. For many accesses through the same global pointer IDO reloads it each time, while the original keeps it in `t6` (TT `func_8013ABB0`, `func_8013AB50`, `func_80132130`: a local `u8 *p` lands in `v0` instead). Left as `INCLUDE_ASM`.
- Slot-count loops that clear one byte per element in unrolled groups of 4 (TT `func_8014AF3C`, `func_80147CE0`, about 12 more): structure matches, the bound is `li at,N` in the loop where the original hoists `li a0,N` (T-0017).
- Pad stubs: `funcdiff.py` reports `-...` for a function followed by a pad stub (objdump folds the zero words); the link check (`ninja build/ovl/<NAME>.ok`) is the verification.
- Left `INCLUDE_ASM` for other reasons: one-nop pads (TT `func_801320F0`, `func_80148764`), RECT local with `h,w,x,y` store order (TT `func_80142DA8`), `D = x; return` style `t7`/`t8` swap (TT `func_8014742C`), pointer-struct clears with an extra `addiu` base (TT `func_801473BC`, `func_801473E8`), a second load that is not CSE'd (TT `func_8013EC1C`), `divu` by a hoisted constant 10 with `break 7` (TT `func_80148714`), spill into `t1` instead of `a0` (NAME_ENT `func_80142560`).
## Main exe batch D (T-1000): src/main/8005A0B0.c and 80061710.c
Ticket [[tickets/T-1000-main-exe-batch-d]]. 39 of 129 functions matched (90 stay `INCLUDE_ASM`); `ninja progress` in this worktree: main game 128/834 -> 167/834 functions (grand total 204 -> 243 of 6962). Most matches are call sequences, guard-plus-call functions (`k_disp_inc(); if (D & 0x20) f(n);`), store sequences and hit-box tests.

Failure patterns seen here (all variants of T-0017 / T-0018, none new):
1. First global load goes to `$v1` in the original, `$v0` in IDO: every dispatcher that tests a `u8`/`s32` global (`switch (D_800E738A)` with 2 to 4 cases, `if (D == 0) .. else if (D == 1)` chains) differs only by `v1`/`v0` (about 20 functions: `pre_syogatu_init`, `uwasa_main`, `func_80061688`, `func_8005B798`, `func_8005A560`, `func_8005AE60`, `func_800626B0`, `func_80060B78`, `func_80062634`, ...). `ret = D; switch (D)` and if-chains were tried; the same register choice shows up in `func_80061EFC` (`D_800E62BA` read into `v1`, constant `li v0,255` shared by three stores).
2. Table-address arithmetic with a parameter-free index: `p = tbl[D & 7]` (`func_8005B040`, `func_8005B0F4`) and `tbl[idx]` (`func_8005B2BC`) get the `lui/addiu` and `sll` in a different order and temp registers.
3. Constant hoisting (T-0017): `func_8005AC70` (`li s1,8` loop bound), `func_8005C7FC`, `func_800618B0`, `func_8005E7F0`; stack frames of functions with a value live across calls (`func_8005D448`, `func_8005B1A8`: the original spills to the frame instead of using an `s` register, 8 bytes more than IDO).
4. Repeated byte constants: three stores of `0xFF` reuse `li v0,255` in the original, IDO loads a fresh temp per store (`func_80061FC4`); chained assignment and a named local did not help.
5. `move a2,a1` as `or` (original) vs `addu` (IDO) when the same constant is passed as two arguments (`func_80062C28`).
6. Callee result used though the callee is `void` in its own file (`func_8005BAE0` family): the match needs `u8 k_sub_disp_start()`, but the defining file declares it `void`; kept as asm.

Idioms: when a global is read as `lbu` in one function and stored as a word in another, a cast at the access (`*(s32 *)&D_800E7944 = x;` in `func_8005C4CC`) is enough. `u8` colour bytes must be declared `u8` (`li 128`/`li 255`, not `-128`). Calls to functions defined later in the same file need a prototype in `include/game.h` (IDO treats an implicit declaration as `int` and rejects the later `void` definition).
## Trailing padding of C functions (T-1310)
Supersedes the pad-stub notes of batches A, B, I, J, K below (`src/ovl/pad/*.s`, "1-nop pad cannot be ported", `funcdiff.py` `-...` after pad stubs). Port a function with trailing nops like any other: just write the C. `tools/trailing_pad.py` adds the nops the `.s` shows after `endlabel`; rule, scan and limits in [[toolchain]]. Unblocked and matched: BUNKAKEN `func_8013EAC8`, `func_80143D68`, `func_80148E68`, `func_80150CC8`, `func_80152618`; BUNKASAI `func_80135448`, `func_80146F38`, `func_8014DD28` (scene-start template, the three source globals differ in BUNKASAI: read them from the `lw` lines); OLH `func_801323CC` and OPTION `func_8013A4FC` (`func_80049A40(...); dtd_on(n);`), VALEN `func_80133C44` (`bg_read_sub2(0x414C); func_8004284C();`), TT `func_801320F0` (same shape as `func_80133BE0`, offsets 0xC/0x10). TT `func_80148764` stays asm for another reason: the original reloads `p[5]` in the second test, uopt CSEs it. TT `func_80148714` and the other batch-K skips are unrelated to padding. Detail: the compiled function must end exactly where the original function ends (symbol size), so a size mismatch shows as a failing sha1, not a silent pad.

## Overlay batch J (T-1060): BUNKAKEN, BUNKASAI
77 functions matched (39 + 38 incl. one earlier empty stub); everything is one of two templates, so it went fast with a generator script over the asm (not committed).
- 0x34-byte setters (`D_x0 = const; D_x4 = const; D_x8 = const;`, three `lui/ori` + `lui at/sw`): 27 + 22 clones, plain C with `u32` globals. Also the copy variant `D_800CA160 = D_80155E90; ...` (`func_8013BFA0`).
- 0x64-byte scene-start stubs: `func_800847B8(n); handler(); D_800CA14C = 0; <3-word copy>; func_8004284C();` (26 clones). `n` is 0 when the original has `or a0,zero,zero`.
- Pads after the last function of an object: 2 or more `nop` use `INCLUDE_ASM("src/ovl/pad", pad_<NAME>_<end addr>)`; the 1-nop case still cannot be ported (asm-processor minimum), so `func_8013EAC8`, `func_80143D68`, `func_80148E68`, `func_80150CC8`, `func_80152618` (BUNKAKEN) and `func_80135448`, `func_80146F38`, `func_8014DD28` (BUNKASAI) stay `INCLUDE_ASM`.
- `funcdiff.py` reports DIFF for these pad cases (the nops live in the original function) and for calls to renamed main-exe functions (`k_reset`/`k_disp_start` vs the auto name in `expected/`); only the linked sha1 (`ninja build/ovl/<NAME>.ok`) is authoritative there.
- Left: `func_80150930`-style local function-pointer table copied from rodata and indexed by `D_800E7389` (known gap (a) of batch A), and every function above 0x7C bytes (not attempted, time-box).

## Conflicting extern declarations (T-1200)

After union merges, one global or function was declared with different types in `include/game.h` and the overlay headers, which IDO rejects ("redeclaration"). Decisions, by access widths over all users:
- `D_801217D0`: s32 (sw/lw users everywhere); the table of 36-byte entries is reached with `(u8 *)&D_801217D0 + idx * 36` in `src/main/80079B10.c`.
- `D_800CA148`, `D_800CA14C`: s16 (sh/lh in every overlay); main code that passed the array stores `&D_800CA148`.
- `D_800CA160/164/168`: s32 (word stores and loads only; the overlay u32 declarations were removed).
- `D_80120652`: u8 (sb stores, lbu loads; overlay s8 removed). `D_800CA19C`, `D_800CA1DC`: u8 arrays, only ever address-taken; overlay callers pass the array.
- `func_80046318`, `func_80083440`: main defines them with a `u8` first parameter, overlay callers were matched with `s32` (no `andi`). A `u8` prototype visible to the overlay adds an `andi 0xff` (KANGEI grew 16 bytes), so game.h no longer declares them: overlays declare the `s32` view, `include/main_only.h` the `u8` view for `src/main/80079B10.c`.
- Prototype repeats (`get_g_zyotai_s`, `func_80046500`, `draw2d3d`, `set_dec_bri`, `func_8006612C`, K&R `()` duplicates) were dropped in favour of the `game.h` one; 60-odd exact duplicates removed.
A symbol that a header does not declare at all (implicit `int`) can also change call code when a header later declares it with narrow parameters; only the sha1 check sees that case.
## Main exe batch E (T-1010): 40 functions in src/main/80062CD0.c and 8006CB30.c
Ticket [[tickets/T-1010-main-exe-batch-e]]. 40 of 153 functions matched (all 27 sha1 OK after every group). Most of what is left hits the three known blockers (T-0017 loop constants, T-0018 register order, `addiu sp` placement), listed below with the new variants.

Idioms that matched with IDO 5.3 plus the frame pass:
- Init functions that set a block of globals (`message_window_init`, `data_save_load_class_init`, `func_8006C760`, `func_8006C848`, `func_80067E34`): plain C, one store per line in asm order; pointers to tables are `u8 *` globals assigned from `u8 D_xxx[]` arrays, the `lh`-then-`sh` copy is `D_dst = D_src` with `s16` types. The `lui $at` per store comes out right.
- RMW of one global in two branches (`message_disp_switch`, `func_8006BD6C`: `if (a == 1) D |= 0x80; else D &= 0x7F;`) now matches; the T-0400 failure note predates `-Wo,-no_const_in_reg`.
- A dispatch on a local that is assigned in two places (`func_800674B0`: `u8 v = D; if (...) v = 1; switch (v)`) gives the expected compare chain and `$v1`. Dense `switch` of 4 cases is a compare chain here, not a jump table.
- `% 7` and `/ 7` on an s32 expression (`func_80068898`, `func_800688F0`) match as plain `/` and `%` with `(s16)` return.
- A function that ends without `return` on one path, `s32 f(void) { if (c) { g(); return 0; } ... h(); }`, reproduces `or $v0,$zero,$zero` on the early path only (`func_80072B5C`).
- Two globals 2 or 0x44 bytes apart that the original touches through one `lui`-less sequence with the second access scheduled after the first store (`func_8006764C`, `func_80065900`): declare the first as an array and index it (`D_8012059A[1]` for `D_8012059C`). IDO then cannot hoist the second load above the first store. The linked bytes match (sha1), but `tools/funcdiff.py` shows relocation names that differ from the splat symbol; the object compare stays DIFF while the executable matches.
- `D = (D & 0xFF) | 0x11` instead of `D |= 0x11` on a `u8` global reproduces the `$t8`/`$t9` destination of the `ori` (`bustup_wink`, `bustup_speech`); marked `FAKE` in the source. The no-op mask consumes one temp number. It does not help `func_80067DD4`/`func_80067DFC` (original has `andi $v0,$t7,0xFF` and keeps the value in `$v0`).
- Passing more arguments than the callee declares: `func_80066104` is `void (void)` in its definition (an argument home `sw a0` would appear otherwise) and declared unprototyped `void func_80066104();` in `include/game.h` for its callers.

Failure patterns left as `INCLUDE_ASM` (the owner ticket in brackets):
1. `switch`/state-machine functions on a global `u8` (`func_80072BC0`, `func_800728A4`, `func_800725B0`, `func_800732F8`, `func_800733D8`, `func_80070F80`, `func_80072944`, `func_8006CDD4`, `week_day_init`, `vacation_day_init`): the original loads the value into `$v1` and in value-returning variants copies it to `$v0` in the first branch delay slot; IDO uses `$v0`. Tried `switch (D)`, `u8`/`s32`/`register` locals, casts, `& 0xFF`, if-chains, `return` forms. [T-0018]
2. Loops with a constant bound or end pointer (`hizuke_disp_switch`, `parameter_disp_switch`, `icon_disp_switch`, `func_8006D038`, `last_date_spot_timer_dec`, `func_8006D4A0`, `func_8006CF80`, `syoushin_up` with `li s2,11`, `func_80062D0C` with `lui v0,0x8000`): IDO unrolls them exactly as the original does (4 per iteration) but computes the end pointer inside the loop; the original hoists it. [T-0017]
3. `addiu $sp` sunk after leading global stores: `week_day_exit`, `week_day_main`. [pattern 1 of batch C]
4. Register choice in leaf code: `func_8006C700` (bit-field extract, original keeps values in `$v0,$a0,$a1,$v1`), `get_weekly_bg_sector` (`$a0` for the loaded byte), `func_80066ACC`, `func_80074F24` (`li $v0,1` where IDO uses `$t2`), `func_80074D28` (IDO hoists `move a0,v0`). [T-0018]
5. `func_800676AC`: IDO keeps the parameter in `$a1` across the call (`move a1,a0`) and does not reload the stored `s16`; the original reloads. `func_8006C934`: store in the delay slot of an early `jr ra` and different branch layout.
6. `parameter_change`: the `sh 999` sits in the delay slot of the compare branch and runs on both paths; no C shape found.
7. Jump-table functions (`func_80063520`, `func_80066A2C`, `func_80066A84`, `func_80072338`, `func_80072CA0`, `_schedule_init`, `func_800722C4`) could not be built before T-1340; they can now through a rodata island (see "Jump tables (T-1340)" below). The listed ones were not attempted again (`func_80066A2C`/`func_80066A84` have case labels in the neighbouring tiny functions; `func_800722C4` has bit-field code).

- Loops with a constant bound (`EN_NICHI func_80135F54`, TEL `func_8013BDDC`) are the known T-0017 case (`li a0,0xB` hoisted by the original). Solved: `func_80135F54` matches; `func_8013BDDC` now differs only in the offset of its one local (0x10 vs 0x14).

## Constants in registers (T-0017)
Ticket: [[tickets/T-0017-const-in-reg-loop-hoisting]]. Build flag `-Wo,-nokpicopt` replaces `-Wo,-no_const_in_reg` (`tools/cc.py`, tests `tools/test_cc.py` class `IdoFlags`).

What the original does, measured over every function of the main exe and the 26 overlays (scratch scans, not committed):
- Integer constants live in registers: 1020 functions compare a loop counter against a register loaded by `li` before the loop, 241 multiply by a register loaded by `li` (`li a1,0x44; multu v0,a1`). IDO 5.3 does the same only when uopt may keep constants in registers; with `-no_const_in_reg` it re-materialises the bound and strength-reduces the multiply into shifts. So the `* 0x44` mismatch was never an older ugen without multiply strength reduction: ugen reduces a multiply by an immediate, and here it never sees an immediate.
- Array and struct base addresses live in registers (1123 functions use a `lui; addiu %lo` base two or more times), while a directly accessed scalar global is re-addressed with `%hi/%lo` per access (3126 functions access one symbol through `%lo(sym)` twice or more).
- `-Wo,-nokpicopt` reproduces exactly this split; `-no_const_in_reg` suppresses both kinds, stock IDO keeps both kinds.

Options and pass mixes tried for the T-0017 cases and the T-0750 multiply: every uopt option one at a time (list in the frame section above), `-Wc` ugen options, `-Wb,-O0/-O1/-O3/-noreorder`, `-O1/-O3`, `-g/-g3`, and all 16 combinations of IDO 5.3 and 7.1 `cfe`, `uopt`, `ugen`, `as1` (the passes are separate binaries; `USR_LIB` points `cc` at a mixed directory). The 7.1 passes give the same code as 5.3 for these functions.

Regression check (scratch harness compiling each `src/**/*.c` and comparing every C function with the original `.s`, symbols included): with `-Wo,-nokpicopt` every function that matched before still matches except `check_end_k` (load order of the two compared globals; fixed by writing the operands in load order) and `func_80042400` (rewritten as `D += 0x377; return D;`). Clean rebuild: 27 of 27 sha1 OK.

Matched with the new flag (were blocked): `func_80048E78`, `tpage_buf_clear`, `get_h_tokimeki_table`, `get_h_yuukou_table` (main), `func_80135F54` (EN_NICHI), `func_8013815C`, `func_801446A0`, `func_80146FA0` (TAIIKU, out of `NON_MATCHING`), and the `multu` loops `func_80140788` (DATE), `func_8011C11C`, `func_80103B60` (EVENT), `func_8014261C` (GYOZI), `func_8013FFD0` (SHOUGATU). `ninja progress` 151 -> 164 functions, 8132 -> 9580 bytes. Side effect on the frame pass: a local array indexed in a loop now gets its address hoisted (`addiu s1,sp,N`), which the pass already adjusts like any `$sp` immediate; the IDO snippets in `tools/test_frame_pass.py` cover both forms. The remaining ~1200 functions with hoisted constants are now ordinary matching work ([[tickets/T-0950-match-nokpicopt-unblocked-functions]]).

## Register promotion of globals (T-0018)
Ticket: [[tickets/T-0018-ugen-temp-register-order]]. Not solved; this is a compiler difference. Update (T-1321): the cause is IDO's `CVT` on unsigned loads and its copy propagation of switch temporaries, not a loop-only allocator; `tools/cvt_pass.py` models both but is not in the build (section "Unsigned-load conversion pass (T-1321)" below).

The T-0750 "compare chain on a `u8` global gets `$v1`" is one symptom. When a function reads a scalar global in several basic blocks (switch or `if` chain, then `D++` after a call), the original keeps every load of that global in one register, re-loading it into the same register after each call (`lui v1; lbu v1,%lo(D)(v1)` before the chain and again after `jal`, then `addiu t6,v1,1; sb t6,%lo(D)(at)`). IDO 5.3 and 7.1 do this only inside loops: in straight-line code the compare chain gets a CSE temporary (`$v0`) and each later load gets a fresh ugen temporary (`$t6`, `$t8`, ...), which also shifts every later temporary.

Evidence (scan of all functions whose first two instructions load a byte global):
| first load | later loads of the same global | functions |
|---|---|---|
| `$v1` | all into `$v1` | 143 |
| `$v0` | all into `$v0` | 46 |
| `$v1` / `$v0` | none | 135 / 234 |
| any | into other registers | 6 |

So in 189 of 195 functions that re-load the global, every load uses one register: the original uopt register-promotes global scalars across straight-line code, with reloads after calls. Which of `$v0`/`$v1` it gets is not explained (hypothesis, untested: a call result or return value in `$v0` inside the promoted range pushes it to `$v1`); IDO cannot be steered there because it does not promote. Same family: `func_80042400`-style `$v1` vs `$a0` choices in `func_801383A0` (EN_NICHI) and the `$v1`/`$a2` swap there.

Tried without effect (on `func_8005A560`, `func_8005B798`, `func_8013A40C`): every uopt option, `-Wc`/`-Wb` options, `-O1/-O3`, `-g`, `-cckr/-xansi/-ansi/-signed`, `-Wf,-Xvolatile/-volatile/-d1/-saveargs`, all 16 IDO 5.3/7.1 pass mixes, with `-no_const_in_reg`, with `-nokpicopt` and stock; C variants: `switch` vs `if` chain, `u8`/`s32`/`u32`/`register` local copy, `(s32)` cast, `default:`, `goto`, struct member, array element, absolute address, `int` return, `return D`. IDO does promote the global in a loop (`while (D != 5) { D++; g(); D += D; }` gives `lbu v1` reloads), so the machinery exists; the original's priority function differs (it promotes with no loop weight).

Not modelled as a pass: a binasm rewrite would have to redo uopt's register choice (`$v0`/`$v1` depends on interference the pass cannot see), which fails the uniform-rule test of CODING_STANDARDS 7a. Next experiments: (1) read uopt's register-promotion priority in the ido-decomp sources (live-range benefit and the loop-depth weight) and check whether a single weight change explains all 189 cases; (2) an older MIPS uopt (Ultrix/NEWS-OS/IRIX 4, [[tickets/T-0100-older-mips-compiler-emulation]]); (3) if (1) gives a one-line rule, consider patching that weight in a copy of uopt (a toolchain change, not a pass), gated on all matches staying green.

Update (T-3100, [[original-compiler]]): "IDO does this only inside loops" is too strong. IDO 5.3 promotes a global in straight-line code once it has enough references. For example `if (D==1) { D++; g(); D += D; }` gives `lbu v1` with a reload into `$v1` after the call, and a `switch` with those cases gives `lbu v1; beqz v1; move v0,v1`, which is the original's shape. The original promotes at lower counts: one switch selector, or one compare plus a `D++` after a call. The register choice then follows from uopt's coloring order (switch temporary `$v0`, global `$v1`; if-chains `$v0`). So the gap is the promotion threshold, not the register order. `-Wo,-regr,N` changes the selector register but promotes nothing and regresses 194 matched functions.

`LoadSquare`, `StoreSquare`, `MoveSquare` (original `lhu` into `t8,t9`, IDO `t0,t1`) are unchanged by every option, flag and pass mix above, and by K&R parameter declarations; still `NON_MATCHING`.

## Unsigned-load conversion pass (T-1321)
**Status: not in the build.** After the merge with main (per-object layout, 2734 matched functions) the pass changes 26 functions that match without it: 20 switches on unsigned globals that the original keeps in `$v0` (all of GYOZI's and SHOUGATU's, DATE2 `func_80133A6C`, SHUGAKU `func_80132760`/`func_80137C3C`/`func_80138154`, ETC `func_80145960`/`func_80145FF0`, DATE `func_801448D4`, GEKO `func_80132244`, main `func_80078A0C`), 4 promotions the original does not make (ETC `func_80145FF0`, DATE `func_801448D4`, GEKO `func_80141D74`, `func_80143EA4`; even without the switch rule), and `bustup_speech`/`bustup_wink`, whose FAKE masks it makes unnecessary. The C of the `$v0` and `$v1` switches is the same (`switch (D)` on an extern `u8`), so no rule on the C or the ucode separates them; the original's discriminator is a property of the variable that the C does not encode (T-3100's model: a promotable scalar vs struct or array data, see [[original-compiler]]). A third rule found during the merge is kept in the tool: IDO puts the constant first in `cvt(D) == k`, so after uopt the pass swaps `D == k` back (DATE `func_8013E184`, GEKO `func_8013F02C`). Open in [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]].

Ticket [[tickets/T-1321-register-promotion-build-step]]; tool `tools/cvt_pass.py` ([[toolchain]]). Supersedes the "Not solved" verdict of the T-0018 section above: IDO 5.3 does keep globals in registers across blocks and calls in straight-line code (a `s32` global compared and reloaded after a call, GEKO `func_8013A5E8`, comes out exactly like the original); what stops it for the recorded cases is two IDO details, not the allocator.

How it was found (scratch tools, not committed: a ucode reader, a pass-by-pass IDO driver, uopt's `-Wo,-zdbug:6` coloring trace written to a list file):
- uopt has undocumented numeric options parsed as `-name:N` (`zmark zvref zdbug zmovc zcopy zcomo zstor zscm zaloc`, n64decomp/ido `uoptinit.c` and `uoptutil.c` `getoption`); the earlier option sweeps (T-0014, T-0017, T-0018) covered only the named flags. `-zdbug:6 -l FILE` prints the coloring decisions (the list file is only flushed in 4 KB blocks and uopt aborts on its final float statistics, so append a large dummy function to the file to get the tail out).
- `func_8005A560` (8005A0B0) traced: the variable `D_800E738D` is "not colored (-ve save)"; the colored live range is `cvt(D_800E738D)`. cfe writes `LOD L; CVT J<-L` for every load of an unsigned 8/16-bit variable, and uopt treats the widened value as its own expression: the compare chain uses the expression (in `$v0`) and the variable keeps only its `D++` uses after the calls, which are not worth a register. Signed globals (`lb`/`lh`/`lw`) have no `CVT`. Deleting the `CVT` in the ucode makes IDO produce the original byte for byte (chain and reloads in `$v1`; `$v0` is taken by the callees' return values inside the global's live range).
- Switches: cfe compares a compiler temporary (`VREG`) that holds the selector. In 282 of 338 switch compare chains at a function start the original has the selector in `$v1`, often with a dead-looking `or v0,v1,zero` in the first delay slot (155 of them); IDO merges the temporary into the global by global copy propagation and uses `$v0`. With copy propagation off (`-Wo,-zcopy:0`) IDO reproduces both forms: the global in `$v1`, the temporary in `$v0`, and the copy survives only where `$v0` is live at the return (an `int` function that falls off its end, i.e. K&R implicit int). The 56 `$v0` exceptions cluster by source file (all switches of GYOZI, SHOUGATU, BUNKA_SD, most of SHUGAKU, one run of 8005A0B0, a run in ETC around 0x80145960), which fits a coding style: switching on a local copy (`u8 mode = D; switch (mode)`), which keeps `$v0` under the pass and matched (`func_80145960`, GYOZI `func_8013B920`).
- Copy propagation off for the whole file breaks 106 matched functions; off only for procedures with an unsigned-global switch temporary breaks one (`func_80145960`, rewritten with a local copy). Signed globals keep IDO's behaviour (the `s8` switches of ETC `func_8014979C`, `func_8014B8CC` and `s32` ones like TACO `func_80135F6C` need it). A `lw` global switched in `$v1` (`normal_date_move_place` on `D_800E7384`) matches when declared `u32`.

Rule, layer, limits: see [[toolchain]] section "Unsigned-load conversion pass". The `==`/`!=` operand order is IDO's with the `CVT` (uoptinput swaps the operands when the left one is not a plain variable, and cfe puts the widened load first); the pass keeps that order, so the existing C (operand orders chosen for IDO, e.g. `func_80045288`, OMIMAI `func_80132398`, DATE2 `func_8013279C`) still matches.

Rejected or not needed:
- `-Wo,-zmovc:0` (reload cost 0): promotes the globals but also every constant (`li v1,1; bne v1,v0`) and single-use load.
- Patching uopt's thresholds (prototype on a scratch copy of the recompiled uopt, never committed): `adjsave > 0` to `>= 0` in `compute_save` together with the two split/color comparisons of `globalcolor` promotes `D_800E738D` but also puts compare constants and `lui` addresses into registers; `func_80145960` and DATE2 `func_8013279C` stop matching and `func_8005A560` still differs. The difference is the conversion, not the threshold.
- Dropping `VREG` records, `-nordstore`/`-zstor:0`, mapping the temporary to a fixed register: each fixes at most part of the switch cases.

Matched corpus before the merge (1168 functions): with the pass every function that matched before still matched except `bustup_speech`, `bustup_wink` (their FAKE `& 0xFF` masks are no longer needed: plain `D |= 0x11` matches), ETC `func_80145FF0` and `func_80145960` (switch on a local copy now). Clean build 27 of 27 sha1 OK.

Measured on a sample (C written by agents for 148 R/V-flagged functions of at most 400 bytes plus the recorded `promo` rows, compared with and without the pass; scratch, not committed):
| | functions |
|---|---|
| match only with the pass | 40 (ETC 23, TACO 3, main/small overlays 14) |
| match with and without (detector false positive) | 19 |
| differ in both | 89 (switch placement in implicit-int functions, the shapes of [[tickets/T-3002-remaining-promotion-shapes]], frames, header prototypes) |
| match without, not with | 0 |
So the R/V detector's precision for "blocked by this gap" is 40 of 59 resolved functions (68%); 13 larger R-flagged functions (540-804 bytes, RPG_BAT and TACO) matched with plain C and did not need the pass either, so the R rule over-reports (it fires on `s32` globals, which IDO already keeps in registers). After the merge only the 13 larger plain-C ones are in the build; the 40 pass-only matches stay scratch C.

Not reproduced by the pass (open): TACO `func_8013760C` (two switches and a loop; the copy `or v0,v1,zero` and `$v1` do not appear even with copy propagation off), ETC `func_8013E89C` (selector in `$a1`), RPG_BAT `func_8013B2F0`/`func_8013CC90` (two globals, `$v1` vs `$v0` swapped).

Batch-agent guidance for when the pass is adopted (T-3000); with the current build these functions stay `INCLUDE_ASM`:
- A compare chain or switch on an unsigned global with `$v1` and reloads into `$v1` after calls: write the natural C, `switch (D)` directly on the global.
- The same with `$v0`: switch on a local copy (`u8 mode = D; switch (mode)`).
- `or v0,v1,zero` in the first delay slot of the chain: the function returns `int` and has no `return` (implicit int); declare it `s32 f(void)` without a return statement (DATE2-era K&R style). Example: `func_80072944` matches as `s32 func_80072944(void) { switch (D_800E738A) { case 0: f0(); break; case 1: f1(); break; } }`.
- A `lw` global switched in `$v1`: declare it `u32`.

## Work queue and the T-0018 detector (T-1320)
Ticket [[tickets/T-1320-tooling-work-queue-and-blocker-detector]]. `tools/docker.sh python3 tools/queue.py` (tests `tools/test_queue.py`) reads the generated `asm/` and the `INCLUDE_ASM` lines of `src/main/*.c` and `src/ovl/*.c` and ranks the 6248 remaining functions (main exe and 26 overlays): unblocked first, leaf before non-leaf, then size. Columns: size, leaf or call, call count, flags. `--files` takes address stems or overlay names, `--next N` the N best unblocked, `--blocked`, `--summary`, `--calibrate`.

Flags (J, S, P are exact; R and V are heuristics):
- `L` a backward branch (information only).
- `J` jump table (`jtbl_*`): 902 functions; a C `switch` table does not land where the original's does (batches A, E).
- `S` reference to a string in `.rodata` (symbol followed by `.asciz`): 1149 functions; same reason.
- `P` one extra `nop` after the last `jr` and its delay slot (asm-processor cannot port a 1-nop pad): 131 functions.
- `R` one global scalar is loaded in two or more basic blocks outside loops into the same register (not `$a0-$a3`, base from the `lui` of the same symbol): the original promotes the global, IDO does that only in loops. 953 functions.
- `V` a global loaded into `$v1` while `$v0` is dead and the value is read two or more times (compare chain, old value of `D++`): the original's promoted register choice; IDO takes `$v0` first. 739 functions (387 also R). R or V: 1305 functions, 853844 of 2231464 bytes.
Detected from the original asm only. Not detected: `addiu sp` placement, RECT store order, local function-pointer table copies, hoisted `lui` sharing, the T-0018 register-order cases that are not a global load (see the `regorder` rows of [[data/t0018-cases]]).

Calibration (`queue.py --calibrate`), positives = the 42 `promo` rows of [[data/t0018-cases]] (functions named in the batch sections above as T-0018 failures that reload or dispatch on a global), negatives = the 714 matched functions (`asm/matchings`):
| | result |
|---|---|
| recall on the promo rows | 41 of 42 (97.6%); only `func_80086640` (swap of two globals, first load `$v1`, second `$v0`) is missed |
| matched functions flagged | 0 of 714 |
| precision against matched functions | 41 of 41 (100%) |
| other T-0018 rows (`regorder`, `reverse`) flagged | 6 of 32 (not expected to fire) |
Caveats: the rules were tuned on these same rows (recall is in-sample), and the matched set is biased (agents picked what compiles, so a function that has the pattern is rarely in it, which is also why zero matched hits is the expected result). The tuning removed three false-positive shapes seen in matched code: array-index loads (`lui; addu; lbu` through the same register), argument registers `$a0-$a3` reloaded for calls, and a single-use `$v1` temp whose result goes to `$v0` (`func_80042400`). Two recorded T-0018 shapes go the other way (the original loads the global again into fresh temporaries where IDO keeps one register: `func_80132DC4`, `func_8007B5EC`, category `reverse`); they are not detectable by the same-register rule. R and V together flag 21% of the remaining functions (T-1320 rule; replaced by the T-1321-based rules in "Detector retune and byte-weighted queue (T-3340)" below), so treat them as "probably blocked": a flagged function that matches is a finding to write down.

## Detector retune and byte-weighted queue (T-3340, T-3000 part)
Ticket [[tickets/T-3340-shared-main-prototypes-and-byte-queue]]. `tools/queue.py` (tests `tools/test_queue.py`).

Why the R/V rules changed. T-1321 showed that the gap is the `CVT` cfe puts on unsigned narrow loads (`lbu`/`lhu`), not register allocation in general. Signed and word globals (`lb`/`lh`/`lw`) are promoted by IDO already, and the C of a switch whose selector the original keeps in `$v0` is the same as the C of one whose selector sits in `$v1`. The old rule (every global load counts) therefore over-reported on word globals and said nothing about the selector.

New flags:
- `R` an unsigned narrow global (`lbu`/`lhu`) loaded in two or more blocks outside loops into the same non-argument register, not a switch. `V` the same family for the register choice: such a load goes to `$v1` while `$v0` is dead and the value is read twice (`D++` and a compare). Both are blocked (cvt gap).
- `U` blocked-unknown: a switch or compare chain at the start of the function on an unsigned narrow global (first load within 12 instructions, selector read by two compares, or one `sltiu` before a jump table). Shown as `U1` (selector in `$v1`) or `U0` (`$v0`). Nothing in the C separates the two, so the queue gives no cause or fix and keeps both out of `--next` and `--plan` (`--include-unknown u0|all` lets them in). The asm does separate them in practice, see the table: no matched function has a `U1` selector, 48 have a `U0` one.
- `T` not a blocker, a hint: the V shape on a `lw`/`lh`/`lb` global. T-1321: a `lw` global switched in `$v1` matches when declared `u32` (`u16` for `lh`); try the unsigned type first.

Calibration (`queue.py --calibrate`, current tree: 4153 functions in assembly, 2803 matched functions outside the table as negatives). Positives are the 54 `promo` rows of [[data/t0018-cases]] found in the asm (a row counts whether the function was matched later with a trick or is still open; 39 of them are the global-in-register family, the others are `D = N; f(N)` constants and hoisted loads, which no register rule can see). Earlier calibrations looked rows up by the overlay name only, which no longer matches the per-object labels (`TT/80147380`), so 38 rows were silently dropped; fixed.
| flags | rows hit | family hit | matched functions flagged | precision |
|---|---|---|---|---|
| old rule (R or V, any load) | 36/54 | 36/39 | 18 | 66.7% |
| **R, V or U1 (blocked)** | 29/54 | 29/39 | 1 | **96.7%** |
| R | 4 | 4 | 0 | 100% |
| V | 2 | 2 | 1 | 66.7% |
| U1 (selector in `$v1`) | 23 | 23 | 0 | 100% |
| U0 (selector in `$v0`) | 1 | 1 | 48 | 2.0% |
| T (hint) | 7 | 7 | 9 | 43.8% |
| R, V, U or T (everything shown) | 37/54 | 37/39 | 72 | 33.9% |
So the blocked set (R, V, U1) trades 7 of 36 hits for 17 fewer false alarms; the old rule's false positives were the word-global reloads (13 R, 10 V). `U0` is the one that is not a blocker in practice (1 recorded failure against 48 matched), which is why `--include-unknown u0` is the cheap way to use it. The recall figure is in-sample (the rules were tuned on these rows), and the negatives are biased towards what compiles. Remaining: 304 `U1` and 217 `U0` functions in assembly, 1154 with R, V or U in all (old rule: 1287 with R or V).

Byte-weighted ranking (`--by bytes`). Score = expected bytes / effort. Expected bytes = size x P(match) x (1 + credit); P(match) = 0.85 - 0.04 per call (max 6) - 0.05 with a loop, times 0.8 for a jump table and 0.85 for strings (with a rodata island, else 0), 0.25 for R or V, 0.7 for U0, 0.1 for U1, 0 for the `P` pad; effort = 12 + (size / 4)^1.25. These constants are estimates (`GAIN` in the tool), not measurements: they put the best bytes per effort at 100 to 300 bytes instead of the smallest leaf. A function in a `dupes.py`/`neardupes.py` group (computed in-process, or read from `--groups FILE`) with no matched member is listed once, as the cheapest member (`rep+N`), and credited 0.9 per byte-identical twin and 0.6 per near duplicate; with a matched member it is not work (`apply:<name>`, run the copy tools). `--plan N --agents K` takes the best N, groups them by C file (`--exclusive unit`: by overlay, since an overlay shares its header) and deals the files to K lists by longest-processing-time first on the summed effort, so no file is in two lists; a 40-function plan for 4 agents on the current tree came out with effort per list within 2% (most and least loaded list).

## Merge of -Wo,-nokpicopt over 703 matched functions (T-1200 follow-up)
Clean build under the new flag, every function compared with the old-flag build: three functions with unchanged source stopped matching. `func_80042878` (main): local `t` changed from `u8` to `u32`, which puts the load in `$v0` again. `normal_date_bg_fadeout` (main) and `func_80135440` (BUNKA_SD) get `$v1` where the original has `$v0` (the T-0018 register family); about 15 source shapes (types, casts, temporaries, ternary, order) did not help, so both are back to `INCLUDE_ASM`. `check_end_k` and `func_80042400` already carry the branch's fixes. `get_h_tokimeki`/`get_h_yuukou` keep the `u32` prototypes from the branch. Result: 703 + 13 (branch) - 2 reverted = 714 functions of 6962, 27 of 27 sha1 OK.

## m2c wrapper vs plain m2c, and decomp-permuter results (T-1330)

m2c comparison. 16 matched functions of 96 to 360 bytes (5 main exe, 11 overlay; no matched function has a `switch`), draft token similarity to the final C in `src/` (identifiers other than globals/functions folded to one token, comments dropped, `difflib` ratio): plain `mipsel-gcc-c` 0.926, plain `mipsel-ido-c` 0.926, `tools/m2c.py` 0.927 (also with the function's own prototype hidden). On these branch-free or simple-loop functions the target flag changes nothing and context only changes casts and which `extern` lines m2c prints; the wrapper is worse where the context makes m2c add a cast between `u32` and `s32` globals (`func_8013C3C4`: `D = (s32) D2;`) and better where the headers hold the pointer types of globals (`func_8013B354`, `func_800674B0`, `func_8013731C`: 0.87-0.93 -> 0.94-1.0). The real gain is `switch`: 902 of the 6248 remaining functions use a jump table, and plain m2c gives up on them ("the corresponding jump table is not provided"); with the table from the segment rodata the wrapper prints `switch`/`case` (`func_80045414`, `func_80132C24`). Not solved: m2c prints `?` prototypes for callees missing from the headers, struct-copy temporaries as `M2C_MEMCPY_ALIGNED`, and `loop_N`/`goto` for IDO loops that the C wants as `while`. Rodata data variables must not be given to m2c (they come out as folded constants); the wrapper passes only `jtbl_` blocks and strings.

decomp-permuter results (commit 8556c81, `--stack-diffs`, each run under 3 minutes on the emulated amd64 image; permuter output verified by a real `ninja` build, the permuter score alone is not accepted):

| function | base score | permuter found | real build | verdict |
|---|---|---|---|---|
| `strSync` (800563F0, delay-slot nop vs volatile load, T-0016/T-0017 note) | 200 | score 0 in under 90 s: return type `unsigned int` with no `return` (an implicit-int old-style function, plausible original source) | `ninja`: main exe sha1 OK | match, not applied |
| `func_80044700` (80043510, "8 more bytes of local") | 63 | score 0: `s32 t = arg1; rect.x = t << 4;` (a named copy of the parameter) | `ninja`: sha1 OK | match, not applied |
| `func_8004111C` (80041000, store order h,w before x,y) | 20 | score 0 only as `do { x,y,w,h } while (0)` and `func_8009C7F8(&rect, 0, 0 * 0, 0)`; an earlier run without stack diffs found `int pad;` and failed the real build | not built | rejected: the trick would be a fakematch (7); stays NON_MATCHING |
| `LoadSquare` (ugen temporaries t0/t1 vs t8/t9) | 20 | nothing below 20 in 90 s | n/a | register-allocation gap (T-0018 family), as expected |

Lessons: (1) always run with `--stack-diffs` (default in `tools/permute.py`); without it the score ignores the frame and `func_80044700` already scored 0. (2) The permuter's output reformats the whole file; take only the changed expression. (3) The two verified matches are not applied in `src/` by T-1330 (tooling ticket); they are listed here for the next batch ticket (T-0950): `strSync`, `func_80044700`.

## Jump tables (T-1340)
Mechanism and limits: [[build-system]] (section "Jump tables: rodata islands"), how-to: [[decompile-workflow]]. Older notes above that say jump-table functions "cannot be built" are superseded.
- Compiler facts measured: IDO 5.3 emits a `switch` with 5 or more dense cases as a jump table (`sltiu`, `sll 2`, `lw`, `jr`) and writes all tables of an object after all of its strings and constants, in function order (checked with a scratch file: `INCLUDE_RODATA` block, switch, block, string, switch gives block, block, string, table, table). The original compiler does the same (strings first, tables last, per object; objects end with zero padding to 16).
- Matched (8): main `func_80053DDC` (card status loop; one separate block per `case`, return value kept in `$s0`), `func_8007A254` (`u16` argument masked with 0xFF and written back); overlays RENSYU `func_80133E5C` (nested switch, `(u32)D >> 4` gives `srl` where `u8 >> 4` gives `sra`), ETC `func_8014A2C4` (`case 1: case 2: default:` share the default block; without them IDO narrows the table to 3..10), DATE `func_8013EB1C` and EVENT `func_8011A874` (`if (g == 1) { switch ... }` then `return f()` at the end), DATE2 `func_80133620`, GYOZI `func_8013AE80`. None is a T-0018 compare-chain case.
- Tables of dense switches are `jtbl_*` blocks of 5 or more words; dispatch with fewer cases is a compare chain and needs no island.
- A string passed through an extern symbol (`format(D_800AFBF0)`) must become the literal (`format((u8 *)"bu00:")`) when the file gets an island, otherwise the symbol has no owner.

## Wave 2 batch TACO (T-2030): 70 functions in src/ovl/TACO.c
Matched: tiny global writers, call wrappers, the `D_8015EDB4` actor array (`TcActor`, 0x88 bytes; elements 16, 17, 21, 23, 24, 25 hold per-slot bytes at +2, +3 and +0x84..), light set-up blocks (`func_8014394C` family), `RECT` helpers. New patterns:
- Several overlays share addresses: `tools/m2c.py` picks the alphabetically first overlay that has the function name, so for TACO it can print another overlay's body (seen with `func_8013D1E0`). Compare the draft's globals with the `.s` before trusting it (tooling bug, reported).
- A function with unused leading register parameters that forwards `$a0-$a3` to a callee with stack arguments (`func_8014559C`) needs the full parameter list, 4 dummy params plus the 2 used ones, and the callee prototype with all 7 parameters.
- `(arg + D_8015EDB4)->field` (index first) gives `addu v1,v0,t6` (pointer first) in `func_8015AB18` / `func_8015AB80`; `D_8015EDB4[arg].field` gives the other order.
- A `u8` field compared with `>= 0x7C` is `slti`; the original has `sltiu`: write `0x7CU` (`func_80151264`).
- Left as `INCLUDE_ASM`, not T-0018: one `lui at` shared over adjacent s16 stores (`func_80143E80`, same as `func_80046290`); IDO unrolls a counted copy loop that the original does not (`func_80147400`); `RECT` locals where the original frame is 8 bytes larger (`func_8013B404`, `func_801436D4`, `func_801438F0`) or smaller (`func_80144D90`); a pointer local that the original spills across calls (`func_80143574`, `func_801438F0`); by-value struct parameter copied with `swl/swr` (`func_8013D1E0`); a `RECT` argument copied by value into a 10-argument call (`func_80156178`, `func_80158AB0`, `func_80158CDC`, `func_801571E8`); a ring-index global that lives inside the table it indexes (`func_8013D2A0`; the original reloads it after the table stores, declaring it as `D_8015E8B0[0].unk6` did not reproduce the order).
## Wave 2: ETC (T-2040)
Ticket [[tickets/T-2040-wave-2-etc]]. 190 new functions of `src/ovl/ETC.c` matched (197 of 387 now C) (mostly three families: pointer-table loaders, the 0x44-entry screen setup functions, call wrappers). Patterns:
- Loads from a constant address in a loader (`lui t9,hi; lh t0,lo(t9)`, new destination register) are `*(s16 *)0x801C26A0`; reading the same address through an `extern` symbol reuses the address register (`lh t9,..(t9)`) and misses. Stores to the overlay's own globals can use the extern symbols. Pointers stored as `lui`/`ori` constants are `(u8 *)0x801B0000` assigned to a `u8 *` global; a table of equal `0x80000000` stores needs one type for all targets (an `s32` and `u8 *` mix gets a second `lui`).
- Calling an unprototyped (K&R `void f();`) function with a value loaded from a table keeps that value in `$a1` (`lw a1,..; move a0,a1`) as in the original; a prototype with arguments gives `$a2` and a different call setup.
- A function that is non-void but has no `return` (implicit `int`, `s32 f(void) { ...; }` falling off the end) keeps `$v0` live at `jr ra`, and as1 then does not hoist a `lui v0` of a later switch into the load-delay slot (`func_8014B8CC`, `func_80149A08`, `func_8014979C`, `func_8014BA8C`, `func_80143334`, `func_80145FF0`). A `void` version differs only by that `lui`.
- Several globals of one table accessed through one symbol (`D_80120650[3] |= 0x80; D_80120650[7] = 0x80; *(s16 *)&D_80120650[0x2A] = -0x65;`, entries of 0x44 bytes) keep as1 from moving a later `lbu` above an earlier `sh`; separate symbols (`D_80120653`, `D_8012067A`, `D_80120697`) get the load hoisted and miss. The base `D_80120650` is a guess (any base in the same word works for the bytes); the linker resolves it from the name. Used by 60 screen setup functions and two table initialisers.
- Frame offset gaps: functions with a local `RECT` (three: `func_8013D264`, `func_8013D2C0`, `func_8013FC08`) have the rect 4 bytes higher than a plain `RECT rect;` gives; `s32 pad;` declared before it matches (FAKE comment). `func_80136948` (a six-word array passed to a callee) needs `s32 pad[2]` after `u8 *buf[6]` (FAKE). Real source unknown.
- A `u8` parameter forwarded to a call (`func_80132048`): the original does `andi t6,a0,0xff; move a0,t6` before the branch; `u8 arg0`, K&R `u8 arg0;` and a `u8` local all give `andi a1,a0,0xff` later. Not matched.
- `func_801436E4` (shift argument kept in a stack slot across three calls): the slot is at `sp+0x2C` in the original, `sp+0x28` in the build; `s32 pad` moves it but enlarges the frame; the permuter reached score 10 only. Not matched.
- `func_80140AE4`: all temporaries after the first compare are one register higher in the original (`t7` for the first `lw`), no source change found. Not matched.
- Tooling bug: `tools/m2c.py func_8013xxxx` picks the first overlay (sorted) that has an asm file of that name, and all overlays load at 0x80132000, so names collide (`func_80132000`, `func_80136CB0`, ...) and the draft comes from another overlay. Workaround used here: a scratch root `build/etcroot` with symlinks to `asm/ovl/ETC` and `include`, passed with `--root`. A `--overlay NAME` option would fix it.
## Wave 2: DATE (T-2010)
Ticket [[tickets/T-2010-wave2-date]]. 278 functions of `src/ovl/DATE.c` matched (queue minus the R/V flags); about 140 unblocked ones are left, mostly branchy functions that fail on register order.
- Pointer-table initialisers (`D_A = 0x801B0000; ...; D_B = *(s16 *)0x801CE12C;`, 176 bytes, about 35 functions): a read of a halfword in another overlay's data must be written as a cast of the literal address, `D_x = *(s16 *)0x801CE12C`. The original has `lui t9; lh t0,%lo(sym)(t9)` (address and value in different registers) and the store of the previous constant before the `lui`; a declared `extern s16 D_801CE12C;` (also as array, struct member, pointer temporary) gives `lui t9; lh t9` and hoists the load. Same idiom as MASTER `func_80138F70`. `funcdiff.py` reports these as DIFF only because the original object has a relocation against the splat symbol where the C object has the absolute address; the linked bytes are equal (the sha1 decides).
- Calls to known callees use the O.BIN names of `config/obin_renames.txt` (`bg_read_sub2`, `get_g_zyotai_s`, `normal_date_speak`, ...), as the older DATE code does. m2c prints the old `func_XXXXXXXX` names from the asm.
- m2c prints callee prototypes as `? f(?);` with a wrong argument count when the callee has several arguments, and drops the arguments of callees declared as `void f();`; read the argument counts from the asm (`_sprite_set_light_effect1` takes nine, `func_8014EF1C` twelve).
- Not matched, new patterns: (1) constants stored to several globals (`3, 3, 4, 4, 2, 2, 3, 4, 2`): the original loads each constant once and reuses the register across stores, IDO (with `-nokpicopt`) loads it per store; chain assignments and locals do not change it (`func_8013DCC0`); with stock flags IDO shares the constants but takes `$v0/$v1`. (2) A bit test or compare, then `D = (u16)D + n`: the original's temporaries start one register later (`func_80156570`, `func_80157184` family, six functions). (3) `s16` parameter used eight times: original sign-extends once into `$v0` (`func_80148924`). (4) Two read-modify-write statements in a row (`D1 += 1; D2 += 1`): the original does not hoist the second load above the first store (`func_8013C700`). All listed in [[data/t0018-cases]] where they are register-order cases.
- Tool notes: a layout shift (one function growing) makes every later function differ in the linked bytes, so a per-function check should compare relocation-resolved words of the objects (`objdump -dr` of the all-INCLUDE_ASM object and the built object, relocation symbols resolved to their addresses) instead of `funcdiff.py` text; this also accepts the `*(T *)0xADDR` idiom.
## Wave 2: SHOUGATU (T-2060)
152 functions matched in `src/ovl/SHOUGATU.c` (from m2c drafts of the whole queue, then a funcdiff pass; 30 of 400 were matched before).
- Tooling bug: `tools/m2c.py` `locate()` takes the first `asm/ovl/*/nonmatchings/*/<func>.s` it finds, but overlays share addresses (`func_80140780` exists in several), so for an overlay function it can draft another overlay's asm. Workaround: call `m2c.main` with `locate` patched to the overlay path. `tools/permute.py` likely has the same lookup problem.
- Types come from the asm, not from the first draft: m2c declared `u8` globals as `s8` when the first user used `lb`; check `lbu`/`lb` over all users (a `lb` instead of `lbu` shows as a one-instruction diff).
- Shifts: `(u8) D >> 4` and `(u16) D >> 12` give `sra`, the original has `srl`: write `(u32) D >> 4`. Operand order of a compare of two shifted globals follows the original load order (swap the operands until the loads line up).
- `s32` return that is always `0` in some paths (`or v0,zero,zero` in a branch delay slot) means the function returns `s32` with `return 0;` in those paths (`func_80140030`).
- Byte tables and records: `*(s16 *)((u8 *)&D_800E6442 + D_800E71DF * 0x38) = 0x28;`, `D_80146168[D_80146158]` (`u8` array), `D_800E6280 + idx * 0x38 + off` all match when the multiply happens once; with two straight-line `idx * 0x38` the original keeps 0x38 in a register (`li; multu`) and IDO uses shifts (T-0018 row).
- Left `INCLUDE_ASM`: 44 functions that copy a function-pointer table (0x60 to 0xF0 bytes) from `.data` into a local and call through it (e.g. `func_80137C28`, `func_801330C0`). C like `FnTbl tbl = D_xxx; tbl.f[i]();` gives the same code as the original but the local sits at `sp+0x28` and the frame is 0x88, the original has `sp+0x2C` and 0x90. Same as batch A gap (a); nothing in the C reaches the extra 8 bytes without a `FAKE` local. Update (T-3330): a source difference, solved by declaring the index local before the table; see "Local function-pointer tables: one more local declared first".
- T-0018 rows added: 22 (`regorder`/`promo`), see [[data/t0018-cases]]: `D |= 4` fresh temp, `func_80051A68(..) & 0x7F` result one temp later, `D = N; f(N)` keeping N in `a0`, `beq v0,v1` operand order with constants in registers.
## Wave 2, TT (T-2070)
Ticket [[tickets/T-2070-wave-2-tt]]. 49 of 277 `INCLUDE_ASM` functions of `src/ovl/TT.c` matched (228 left; `queue.py` hid about 70 of them as R/V). Verification: `funcdiff.py --expected expected/ovl/TT.o` per function, `ninja` for the overlay sha1.

Tooling bug: `tools/m2c.py` takes the first match of `sorted(glob(...))` when a name exists in several overlays (every overlay has `func_80132000`, `func_80133B3C` and so on), so the draft can come from another overlay. Scratch fix used here (not committed): import `m2c`, replace `m2c.locate` with a lookup in `asm/ovl/TT/nonmatchings/TT` first. `funcdiff.py` is not affected when `--expected/--built` are given.

New patterns:
- Unrolled slot-clear loops are plain index loops. IDO unrolls them by 4 itself: `for (i = 0; i < 0x40; i++) p[0x56 + i] = 0;` gives the `addiu v1,4` loop with `sb 0x57..0x59` and `sb 0x52` after the pointer add (TT `func_80133B3C`, `func_80142740`; word stores `func_80142774`). Do not write the unrolled body by hand.
- A pointer loop `p += 0xA4; p[-0xA4] = 0;` with the counter declared before the pointer matches `func_80147CE0` (declare order decides `$v0`/`$v1`).
- Slot-spawn loops (`p = func_8014AF00(p, p + 0x1E00); ... p += 0x78;` then nine reset stores): the original stores the reset block relative to the already advanced pointer. Writing `i += 1; p += 0x78; *(s16 *)(p - 0x1A) = 0; ...` matches (`func_8014BA54`, `func_8014BC74`, `func_8014BE60`, `func_8014B4A4`, `func_8014C0B0`, `func_8014B67C`); marked `FAKE`. The flag parameter must be `u8 arg0` (the `andi` is at function entry); a `u8`/`s32` local copy does not give that.
- Register numbers follow the source order of the temporaries: when two loads swap registers (`lh t0`/`lh t9`, `lbu`/`addu` before a `multu`), swap the two statements or the two operands (`func_8014AAE8`, `func_80134924`, `func_8014BD68`).
- A store of the same value that was loaded: `*(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);` placed first in the then-block is CSE'd away; the original reloads it. Write it after the `p + 0x24` store (`func_80135A54`, `func_80138178`, `func_8013F588`).
- `u16` field compared with `sltiu`: `(u32)*(u16 *)(p + 0x5E) >= 0x19`; a `u16` local compared with `0xC81U` also works. `do { ... } while (i++ < 0x10000 && f() == 0);` gives `slt` with the increment in the branch delay slot (`func_80133A60`).
- Pointer kept across a call: `u8 *p = D_80158A74;` declared before the call is spilled at `sp+0x2C` like the original (`func_80149E44`, `func_801381E8` family). Nested `switch` on two `u16` fields of one global matches as written by m2c (`func_8013F7B0`, `func_80140CBC`).
- `-f() * 0x600` (negate first) and `if (cond) a = -d; else a = d;` (two arms) reproduce the `negu`/`sll` order of `func_8014B5A4` and `func_80145508`.
- The earlier T-1070 note on `func_8014742C` (`D = x; return`) is obsolete: `arg0[0] = 1; *(u16 *)(arg0 + 2) = arg1;` matches. Its row in [[data/t0018-cases]] stays (rows are never edited).

Left `INCLUDE_ASM` (tried, new blockers; six are rows in [[data/t0018-cases]]):
- A constant used as a `case` label and stored again to a halfword shares one register in the original (`li s2,0x40` for both); IDO keeps two registers or no register: `func_80149108`, `func_80134ADC`; `func_8014B088` passes the shared constant through a `u16` prototype (`andi a1,s5,0xffff`).
- Frame layout: oversized local at `sp+0x30` before a spill slot (`func_8013A810`, `func_8013A8B8`), spill slots 4 bytes off (`func_80133D14`, `func_80134874`, `func_8013EFAC`, `func_80133AD0`).
- Unused constant hoisted into a saved register (`li s3,0x40` never read): `func_80134BBC`.
- Two reads of one global after a branch that the original does not CSE (`D_80158A60` pad word): `func_80149400`, `func_8013EC64`.
- `negu` of a division result into the same register (`func_8014B198`), multiply operand order that the scheduler decides (`func_8014BB30`, `func_8014BF54`), ugen temporary numbering off by one (`func_8014A198`, `func_80148974`).
- Packet-building functions (`func_8013D2E0` and kin) need the libgpu `P_TAG` bit-field insert (`addr:24`); a local bit-field struct builds the same code but the register choice is off.
- `func_801473BC`, `func_801473E8`: stores in element order 0,1,3,4,5,2 with a `+0x208` pointer; a plain loop gives another order.
- `func_80148714`: `divu` by 10 with `break 7` means the divisor is a register variable; no plain C found.
## Wave 2: GEKO (T-2050)
- `tools/m2c.py <func>` takes the first sorted hit of the function name over `asm/ovl/*`; overlays share address ranges, so a name that exists in two overlays (e.g. `func_80136EE4` in ENDING and GEKO) yields the other overlay's draft and header context. Workaround: `--root` pointing at a scratch tree with symlinks `include` and `asm/ovl/GEKO` only. Tooling bug, not fixed here.
- m2c prints `*(s16 *)0x801D0000` for a symbol it does not know; the real address comes from the `.s` (`%lo(D_801D63D0)`), check it.
- Addresses of other overlays (0x801C0000 and up, splat names like `D_801CE158`) must be absolute casts `*(s16 *)0x801CE158`: with a symbol the `lui` and `lh` use other temporaries, with the cast the object differs only by the missing relocation and the linked bytes match.
- m2c prints callee prototypes with the bodies (`s32 func_80051A68(u8);  /* extern */`); they belong in the overlay header once, not in the .c. A callee defined later in the file needs a prototype in the header (implicit int vs void).
- Left `INCLUDE_ASM`, new gaps: the local function-pointer table copy (`M2C_MEMCPY_ALIGNED` + `(&sp[0])[D_800E738A](0x80)`, about 25 functions: known gap (a) of batch A; solved by T-3330, see the section at the end); `func_8013BC74`/`func_8013BCBC` (`s32 t = D_800E738A; call(); if (t != D_800E738A)`: the original keeps the local at sp+0x28, IDO puts it at sp+0x2C; T-3330: `s32 cur; s32 prev;` matches); `func_80136E7C` (a bit-3 test of a word is `sll 28; bgez` in the original, `andi` from C); `func_801374A8` (two `D_800E6280 + idx * 0x38` uses: original keeps 56 in a register for `multu`, IDO shifts); `func_8013CC20` (`D = 7; f(7);` shares one register for the constant in the original).
## Wave 2: GYOZI (T-2020)
GYOZI 244 of 417 functions matched (178 new): event-script call wrappers, setters, table fills, small conditionals and compare-chain `switch`es. Ticket [[tickets/T-2020-wave-2-gyozi]].
- `tools/m2c.py` picks the first `asm/ovl/*/nonmatchings/*/<name>.s` it finds, and overlays share addresses (`func_8013461C` exists in several), so drafts of an overlay function can come from another overlay. Workaround: `--root` pointing at a directory that links only `asm/ovl/<NAME>` and `include`.
- Strings: a function that passes a string through an extern symbol (`func_800BCE10(&D_800D92A0, &D_801458A0)`) builds fine when the symbol is in an asm rodata blob. In the one rodata island of the file (GYOZI `0x11B00..0x11B80`) the symbol has an owner problem (`trailing_pad.py` aborts on the `.rodata` piece in the function's `.s`), so write the literal with octal escapes (`"\230\114\211\272"`; hex escapes can swallow following letters).
- Post-increment compare of a global: `if (D_800F6474++ == 0)` gives the `sltiu`/`addiu` order of the original; `D_800F6474 += 1; if (D_800F6474 == 0)` does not (`func_80135DE8`, `func_8013AFE8`, `func_8013B1D0`).
- `func_8005E0E0(x) & 0x7F` needs the cast `(u8)func_8005E0E0(x) & 0x7F`: it reserves one extra temp, as in the original (`andi t7` instead of `t6`). A `u8` return type in the declaration does not do it.
- `u8 >> 4` gives `sra`; the original has `srl`: cast to `(u32)` first (`(u32)D_800F563A >> 4`).
- Value passed on and stored: `func_8008F618(D_800F62CF = D_800F5ACD)` loads once into `a0` and stores from it (`func_801361D4`). The same form with a constant does not match (`func_8013B43C`: IDO emits two `li`).
- A record array reached as `base + idx*0x38 + 0x1B2` is a struct with a large leading pad (`GyoziWork { u8 pad[0x1A8]; GyoziGirl girl[11]; }`); `D_800F53A0.girl[i].unk_0A` matches where the idx is a `u8` (`func_80137AA8`). With an `s32` index, or two accesses in one function, the original uses `li 56; multu` (`func_80136124`, `func_80144A74`): not reproduced.
- Declarations with the wrong width were the most common first failure (`lh` vs `lbu`); fixing the global's type in `include/ovl/GYOZI.h` turned about 10 failures into matches. `ninja build/ovl/GYOZI.bin` (sha1) is the regression check after a header type change.
- Left `INCLUDE_ASM` (new patterns): (a) six pointer-table fills (`func_8013BEA0`, `func_8013C3B0`, `func_801418B0`, `func_80143AD0`, `func_80143BA0`, `func_80143C70`) whose last store copies an `s16` from another overlay: original `lui t9; lh t0,lo(t9)`, IDO reuses `t9` (T-0018 data rows). (b) Statement scheduling: three `|=`/`-=` updates of adjacent globals in program order in the original, IDO hoists the loads (`func_8013683C`, `func_80135C3C`). (c) Local function-pointer table copied from rodata and called by index (about 35 script-step functions, e.g. `func_80134520`; known gap (a) of batch A, solved by T-3330, see the section at the end). (d) Init sequences with `func_8008F618(D_800F62CF = const)` (`func_8013AC38`, `func_8013B43C`, `func_8013BB0C`, `func_8013C118`, `func_8013CF2C`, `func_8013DC2C`, `func_8013C91C`). (e) Bitfield-like date compares (`func_8013D9E8`, `func_8013DA58`, `func_8013DAA4`, `func_80137D70`, `func_801378D0`, `func_80139858`): `(u32)D << 0x17 >> 0x1B` order of the loads differs.
## Overlay batch EVENT (T-2000)
- Table-init functions (store `0x801xxxxx` constants and one `s16` copy into `D_8012xxxx` globals) are the bulk of EVENT. A copy from another overlay buffer must be written as a raw-address load, `D_8012069C = *(s16 *)0x801CE13C;`, not through a named extern: the named global makes IDO hoist `lui; lh` above the previous store and reuse the address register (`lh t9,0(t9)`); the raw address keeps the original order and gives `lh t0,..(t9)`. The `lui` immediate in the `.s` is the high half only: take the full address from `lui` plus the signed `lh` offset (m2c printed `0x801D0000` for `lui 0x801D; lh 0x63C8`, wrong).
- A pointer-table slot that `game.h` declares as `u8`/`s16` (`D_80120654`, `D_80120664`, `D_80120668`) is written `*(s32 *)&D_80120654 = 0x8019B034;` (CODING_STANDARDS 8a, address of the scalar). Without it IDO stores a small constant with `sb`/`sh`.
- `EVENT.h` includes `game.h`; m2c's context has it, so a symbol that m2c does not list as missing may be undeclared in the overlay otherwise.
- Unknown callee prototypes are written K&R, `void func_X();` (as the existing EVENT declarations); a narrower prototype would change call code.
- `D_80094714`/`D_80094718` are `u16` (all accesses are `lhu`).
- Not reproduced, left as `INCLUDE_ASM` (T-0018 family rows in [[data/t0018-cases]]): compare chains with `li at,1` per compare, one `li v0,N` shared by several byte stores, a constant shared across halfword stores (`li t6,3` used four times; chain assignments `a = b = 3` and a local `s32 a = 3` do not produce it), `$v0`/`$t6` choice of the first compare-chain load.
- Not T-0018, also left: a load of a global placed before preceding byte stores (`func_800F87BC`, `func_800FCF40`: the original hoists in one and does not in the other), `if (x & 0x4000)` compiled as `sll 17; bgez` (`func_80103BF8`, `func_80103C3C`), `sll/sra` of an `s32` argument held in `$v0` for eight read-modify-write halfword updates (`func_8011ED50`), off-by-one `ugen` temporary in `D |= 0x40` after an if/else (`func_8010242C`).
- m2c output for pure assignment and call-only bodies compiled unchanged for more than 400 of the 781 functions; the bodies with `unk` offsets and `&D_...` arithmetic need struct work and were not attempted.
## Wave 2 (T-2080): TAIIKU, RPG_BAT
108 functions matched (RPG_BAT 61, TAIIKU 47 on top of the 9 from T-0017; the three `NON_MATCHING` TAIIKU functions were already plain C). Verification: `funcdiff.py` with a normaliser for `sym+offset` relocations (scratch, not committed) and the overlay sha1 in `ninja`.

What worked:
- **Write read-modify-write and clamps on the global itself**, not through an m2c temporary. `D += x; if (D < -0x4000) D = -0x4000; if (L < D) D = L;` gives the original's `$v0`/`$v1` use at once (TAIIKU `func_80136AB0`, `func_8014692C`, `func_80143CC0`, `func_8013AEFC`); the m2c form with a `var_v0` local lands in `$t7`/`$t8`.
- **Fields of the 0x44-byte record array `D_8011ECD0`** (`D_80120676` is `D_8011ECD0 + 0x19A6`): spelling two accesses as `*(s16 *)(D_8011ECD0 + 0x19DC)` / `+ 0x1A20` keeps the second load after the first store, like the original. Separate extern symbols let `as1` hoist the load above the store.
- **Loops are the original's.** IDO unrolls a constant-count loop by 4 with a pre-incremented pointer: the original `func_8013EA30` is `for (i = 0; i < 96; i++) D_8011ECD0[0x1107 + i * 0x44] = a;` and `func_8013F220` is `for (i = 0; i < 3; i++)` over `s32 D_8015ED64[][4]` with four stores. Do not unroll by hand. A loop with a call inside is not unrolled (`i + 0x28` arguments, `func_8015A6D4`). Two 3-iteration loops over the same base: `for (i = 0; i < 3; i++) *(s32 *)(D_800E6280 + 0x123C + i * 4) = 8;` and the same with `0x12BC` give the `sltu` loop followed by the `bne` loop with the base rebuilt (TAIIKU `func_801356E0`, `func_80143398`, `func_80145EF4`); the m2c `do { p += 4; ... } while` form makes IDO share the base in `$a3`.
- A load that must stay after earlier stores to other globals: read the absolute address, `D_8014A0EC = *(s16 *)0x801B93F8;` (TAIIKU `func_80142180`, `func_80145210`). The bytes link identically; `funcdiff.py` shows `lui t9,0x801c` against the relocation, so the overlay sha1 is the check.
- `s8`/`s16` parameters from m2c are `s32` when the original does not sign-extend (`func_8013E7C0`, `func_80151380`). A `u8` global written with `0x80` must be `u8`, not `s8` (`li 128` against `li -128`).
- `if (c) return 1; return 0;` gives `beqz; move v0,zero; b; li v0,1` (RPG_BAT `func_80156870`, TAIIKU `func_8013CEF8`). `(u32)x >= N` gives `sltiu` (`u16` globals: `(u32)(u16)x`). `a = f(x); b = f(y); return a + b;` gives `addu v0,t6,v0`, the one-expression form swaps the operands (RPG_BAT `func_8013EA00`).
- `-D * 8` gives `negu; sll; move`, `D * -8` gives a different pair (`func_8013ACA4`).
- Functions called before their definition in the same file need a prototype in the overlay header, otherwise IDO reports a redeclaration (implicit `int`).

New blockers (not T-0018):
- **Shared `$at`.** The original shares one `lui $at` between several stores/loads of neighbouring globals (`lw/sw` to `D_8015EDB0` and `D_8015EDB8` with one `lui at`; `sw zero` three times after one `lui`). IDO re-emits `lui at` before every access with scalars, arrays, structs, same-file definitions and `-O3`. 26 queue entries are affected (RPG_BAT counters `func_8014EBD0`, `func_8014F524`, `func_8014EBA8`, `func_8014F230`, ...); left as `INCLUDE_ASM`. It looks like an `as1` difference, not a source difference.
- Record decrements `*(s16 *)(D_8011ECD0 + 0x1AA8) -= 4` ... (TAIIKU `func_80140338`, `func_801487B4`, tail of `func_80145044`): everything matches except that the original puts `lui t0` before the store of the previous field and IDO after it.
- Loop `do { *(s16 *)(p + 0x1998) += 1; p += 0x44; } while (p < D_8011EDE0)` (TAIIKU `func_80145044`): `$v0`/`$v1` for pointer and bound are swapped.
- Base of an array used twice in one expression is shared by IDO but reloaded by the original (TAIIKU `func_80147A6C`: `e != D_8014A454` after `&D_8014A454[i]`); base kept in a register per block in the original (`func_8013C5C8`).
- Epilogue scheduling: the original hoists `lw ra` above two final stores through a pointer (TAIIKU `func_80140738`). `func_8013F15C` (RPG_BAT): frame 8 bytes too big with one extra local, 4 bytes too small without (`p` at 0x2C needs a hole at 0x28).
- `func_801331D0` (TAIIKU): `for (x = 0x140; x < 0x200; x += 0x40)` with a call in the body; IDO keeps `0x200` in `$s2`, the original compares with `slti 512`.
- `func_8013C1CC` (RPG_BAT): IDO hoists the next `lui v0` into the delay slot of the first load, the original keeps a `nop`.
- `func_801388E4` (TAIIKU): an uninitialised local that lives on the stack (`sw zero,16(sp)`), IDO puts it at +20.

Tooling notes: `tools/m2c.py` takes the first overlay (alphabetical) when a function name exists in several overlays (`func_8013A464` is in DATE and RPG_BAT), so the draft can belong to another overlay's function. m2c types are unreliable: unknown struct types come out as `?` and field offsets as `unkNN`; width and signedness for `*(T *)(p + off)` can be taken from the `lb/lbu/lh/lhu/lw/sb/sh/sw` operands in the `.s` file.
## Wave 2 small overlays (T-2100)
Ticket [[tickets/T-2100-wave2-small-overlays]]. 94 functions matched in 10 overlays (ENDING 17, SHUGAKU 21, NAME_ENT 18, EN_NICHI 16, KANGEI 11, BUNKA_SD 3, DATE2 3, OMIMAI 2, VALEN 2, OPTION 1, MASTER 0), clean build 27 of 27 OK.

New patterns:
- **Hit-box tests (EN_NICHI `func_801381C8`, `func_801382B4`, `func_80138434`, `func_80138524`, `func_80138614`, `func_80138708`, `func_80138890`, `func_80138AB0`, and the four one-rectangle variants).** Two records at `D_8011ECD0 + idx * 0x44`, `s16` fields at +0x1C0E/+0x1C0A (target) and +0x1A32/+0x1A2E (probe), flag `+0x1BE7 & 0x80`. Write each range test as `hi >= p && p >= lo`, one `if` per rectangle, the flag test as the innermost `if` with `return 1`, and one shared `return 0` at the end (an inner `return 0` changes the branch layout). When the original keeps a bound in a register shared by both rectangles (`x + 8` is the low bound of one and the high bound of the other), declare it as a named `s32` temporary assigned right after the `s16 x` load and before the second record pointer is computed (`xs = x + 8;`); without it IDO puts the CSE value in `$t6` and shifts the pointer registers. Two shared bounds: two temporaries in the order the original allocates them (`$a2` then `$t6`).
- **Externs above 0x80162000** (other overlays' data, e.g. `0x801E6638`): read as `*(s16 *)0x801E6638` (batch A note). Used by 13 pointer-table setters in ENDING, KANGEI, SHUGAKU, NAME_ENT; `funcdiff.py` shows a `lui` immediate difference, the linked bytes match.
- **Load hoisted above an earlier store, solved once:** ENDING `func_80136C5C` (`D_8011F536 += a; D_8011F4F2 += a;`, 0x44 apart): writing the first as `(&D_8011F4F2)[0x22]` keeps the later load below the store. FAKE comment in the source. The same trick failed for SHUGAKU `func_80135BB8`/`func_80135B68` (different symbols 0x44 and 0x13 apart) and `func_80135994`.
- **Parameter type:** a `s16` parameter that is only stored (`*(s16 *)(p + 0x1998) = arg1`) is `s32` in the original (BUNKA_SD `func_801341A0`, `func_801377D4`, `func_80138810`); `s16` adds `sll/sra`.
- **Pointer table through a global** (`*(*(D_800CA2D4 + idx * 4) + off)`): write `u8 **p = (u8 **)D_800CA2D4; p[idx][off]`; IDO then loads the index first as the original does (SHUGAKU `func_80138CB8`, `func_80138D64`). Plain `*(u8 *)(*(s32 *)(D + idx * 4) + off)` loads the pointer first.
- **Compare operand order** decides the load order again: SHUGAKU `func_801332D8` needs `D_x == (D_y & 0xF)`, KANGEI `func_8013836C` needs `expr == D_x`; SHUGAKU `func_80138BD8`, `func_80138C48` need `((u32)D_a >> 4) == ((u32)D_b >> 0xC)` (casts give `srl`, no cast gives `sra`).
- **Tables of 4-byte values indexed by a byte** (`D_800B3688[idx]`, OMIMAI `func_801324BC`, `func_801335C4`): declare `s32 D[]`; the m2c form `*(&D + idx * 4)` is a byte offset and would be wrong with an `s32` declaration.
- **Constant folding differs for `idx * 0x22 * 4`:** EN_NICHI `func_80133F00` keeps `sll 1; (x << 4) + x; sll 2`; every spelling tried (`* 0x22 * 4`, `(s32 *)` index, two locals) is folded to `* 0x88` or shifts the temporaries. Left asm.
- Left asm, new instances of known gaps: `func_8013B51C` (NAME_ENT) and SHUGAKU `func_80135B68`, `func_80135BB8`, `func_80135994` (load hoist), ENDING `func_80137574`, SHUGAKU `func_8013A274`, `func_80138DD8` (T-0018, rows in [[data/t0018-cases]]), OPTION `func_801325A0` (the original keeps four stepping induction variables for an unrolled 2x4 fill; no loop spelling reproduces them), NAME_ENT `func_80142A3C` (store order of the unrolled clear loop), KANGEI `func_80134EA8` and SHUGAKU `func_8013A980` (frame / `lw ra` delay-slot placement).

Method notes (tooling, not committed): (1) `tools/m2c.py` picks the first overlay in alphabetical order when the address exists in several overlays (`func_80138890` exists in DATE, EN_NICHI, TACO); the draft can belong to another overlay. (2) `funcdiff.py` against `expected/ovl/<NAME>.o` compares a stale object when the overlay fails to compile, and reports MATCH for an unconverted function; look at the ninja result. (3) When bulk-converting, compare each function's linked bytes (symbol address in the overlay ELF, original size from the `.s` header, bytes from `disc/files/CDROM/EXEDIR/<NAME>.EXN`) and revert only functions whose own address is right but whose bytes differ: one wrong-size function shifts every later one.
## Main exe wave 2 (T-2090): src/main/*.c
Ticket [[tickets/T-2090-wave2-main-executable]]. 61 functions matched in 15 files (the leftovers of batches C, D, E, F); `ninja progress` grand total 901 -> 962 of 6962, 27 of 27 sha1 OK after every commit. The queue's remaining entries are mostly the T-0018 family (rows in [[data/t0018-cases]]); the list below is what was new.

Idioms that matched:
- Byte-flag loops over the 0x38-byte character entries and similar tables: write the loop over an index with the table symbol as base and the field offset folded into the index, `for (i = 0; i < 11; i++) D_800E6280[0x1C8 + i * 0x38] &= 0xFFFE;`. IDO unrolls it exactly like the original (three leading statements from the remainder, then 4 per iteration) because the trip count is a constant, and the pointer it keeps is `D_800E6280 + 4*i`. A different base symbol (`D_800E68EC[i * 4 + 3]`) changes the loop pointer's `lui/addiu` and the sha1. `funcdiff.py` shows only relocation names for these (`D_800E6448` vs `D_800E6280+0x1C8`); the executable sha1 is the test (`func_8006CF80`, `func_80071280`, `func_8006D4A0`, `func_8006D038`, `last_date_spot_timer_dec`, `func_8006CF38`, `tpage_buf_clear_all`).
- A single-bit test that compiles to `sll rX,rY,N; bgez/bltz` (not `andi`) on a word read through a pointer is a bit-field access: `typedef struct CharFlags { u32 b0 : 1; u32 b1 : 1; u32 rest : 30; }` and `((CharFlags *)(p + 0x1C8))->b1` (`syoushin_up`). `x & 0x20000000` on a local does not give this (it hoists the mask into a register); a global u32 does (`func_8007BFB8`).
- Indexing the first table symbol instead of naming the next ones stops IDO from hoisting loads over stores (marked `FAKE` in the source: `cal_sprite_disp_switch`, `func_80062D0C`, `func_8007A924`, `func_80053CC0`): `(&D_801217D0)[9] &= 0x7FFFFFFF` for the entry 36 bytes further on (`cal_sprite_disp_switch`); the plain scalar names give the hoisted-load schedule.
- `p[0xF] = word & 0xFF;` for a byte store out of a word reproduces the extra temp of the original (`func_8004E44C`, marked `FAKE`); `s32 f(void)` with `return 0;` only on early exits and a fall-off on the main path reproduces `or $v0,$zero,$zero` only on the early paths (`dec_init`).
- Chained pointer-and-offset struct code for sprite packets (`GetWorkBase` result as `u8 *`, `p[4] = ..; *(s16 *)(p + 8) = ..`, `AddPrim(base + (D_8011ECA0 << 10) + n * 4, (s32)p)`) matches as written (`set_tarao_sprt`, `set_tarao_rect`).
- Table row address as `D_xxx + (arg0 % 25) * 3` on a `u16[]` (not `D_xxx[idx][3]` or `(u8 *)D + idx * 6`) reproduces the temp numbering of the `addr_init_*_sd` functions: the multiply by 3 and the shift stay separate temps.
- Parameter types decide the entry code. `u16 arg0, u16 arg1` that the original spills and reloads (`sw a0,0x30(sp)` ... `lhu t8,0x30(sp)`) need ALL parameters declared `u16`, including the two the original writes straight into a `RECT`; `s16` for those two gives `t0/t1` instead of `t8/t9` (`LoadSquare`, `StoreSquare`, `MoveSquare`: the old "T-0016 register allocation" NON_MATCHING blocks were this). A `u8`/`u16` parameter that the original uses without `andi` is declared `s32` (`birth_day_check_days`, `func_8006D52C`).
- A `u8`/`u16` parameter that is live across calls is stored to its home slot at entry and reloaded with `lbu`/`lhu` in the ordinary prototype style, no old-style (K&R) definition needed (`func_800570B8`, `func_8007AB24`; the header prototype of `func_8007AB24` is `u16`). An old-style definition also compiles to the same code, but `tools/srcscan.py` (`DEF_RE` wants `)` and `{` together) does not see it and `ninja progress` aborts with "neither INCLUDE_ASM nor C body".
- `u16 >> 12` needs the `(u32)` cast for `srl` but its temp register still differs (`func_8005352C`, T-0018).

Tooling facts:
- `funcdiff.py <name>` takes the object of the first `src/main/*.c` that mentions the name; if the ninja build of the right file failed or was skipped (the header check `build/headers.ok` stops the build before the objects), the stale all-asm object is compared and prints MATCH. After any `game.h` edit run `ninja` and read its errors, or pass `--built build/src/main/<file>.o`.
- A scratch compile (`tools/cc.py x.c out.o ido 5.3`) of a function that calls others prints `jal <other_function>` operands from the nearest symbol of the scratch object, so every call line differs from the expected object; compare the relocations (`R_MIPS_26 name`) instead.
- `funcdiff.py` cannot compare a function whose name starts with an uppercase `L` (`LoadSquare`): `functions()` treats names matching `\.?L\w+` as internal labels and returns `None` for both objects ("missing in built"). Compare such a function by `objdump -dr` against the `.s`, or by the sha1.
- `funcdiff.py` also shows the trailing `...`/`nop` of the end-of-object padding as a one-line difference for a function that ends an object.

Functions that need other owners' files (not done):
- `func_80042940`, `func_80042908`, `func_80042808`, `func_8004284C`, `normal_date_girl_in_init`: the original returns 0 (`or $v0,$zero,$zero`), but the headers of the overlays that call them (VALEN, DATE, ENDING, OPTION, ...) declare them `void`; redefining them as `s32` is a conflicting declaration in `check_headers.py`. They also hit the delay-slot order (`or $v0` before the last `sb`) that no source form of `return 0` produced.
- Jump-table functions (`func_80066A2C`, `func_80066A84`, `func_80063520`, `func_800722C4`, `func_8007A618`, ...): need `tools/rodata_island.py`, which edits `config/SLPM_86.053.yaml` (not allowed in this wave). `func_80066A68/70/78/AC0` are case bodies of those switches; the whole function A2C..A84 is one switch.
- The `D_800E36C0` mouse pair: `InitMouse` (`sw $a0` / `sw $a1,+4` after ONE `lui $at`), `func_80046290`, `dec_bg_reset`, `k_reset`, `k_disp_start`, `func_8004482C`, `menu_bar_color`: several stores to a global cluster share one `lui $at` in the original; IDO re-emits `lui $at` for every store, for scalars, arrays, structs and static data alike.

Other near misses kept as `INCLUDE_ASM` (all differ only in scheduling or register choice, none by a missing idea): `func_800410AC` and `normal_date_three_select_init` (original frame has 8 more bytes of locals), `func_8007B734`/`func_8007A868` (loads the original hoists, IDO sinks), `hizuke_disp_switch`/`parameter_disp_switch` (order of the first load in the unrolled body), `func_80071038` (loop with a 0x38-byte entry and a large body: IDO unrolls the pointer form with a run-time remainder; the indexed form peels the first iteration), `func_8004636C`, `SD_CalcCDAve`/`SD_DetectCDPeak` (u16 loop temporaries), the `func_8007B358` family (four copies, `lhu` temp number).

## Local function-pointer tables: one more local declared first (T-3330)
Ticket [[tickets/T-3330-local-fptab-frame-layout]]. The "local function-pointer table" gap (batch A gap (a), SHOUGATU, GEKO and GYOZI sections above, DATE2 and BUNKASAI notes) is a source difference, not a toolchain one. No pass is needed, and `tools/cc.py` and `tools/frame_pass.py` stay unchanged. IDO 5.2 and 4.1 lay these functions out exactly like 5.3 ([[ido-52-evaluation]]).

### The IDO rule behind it
If a function keeps any local in memory (an address-taken array or struct, a struct copy, or a scalar spilled across a call), IDO gives every local declared in the function its own stack slot. That includes unused locals and locals that live in a register. Measured with IDO 5.3 and the frame pass on scratch C:
- The slots fill the locals area from the top of the frame downwards, in declaration order: the first declared local has the highest address.
- The area is rounded up to 8 bytes, and the rounding gap sits below the last local (at T+16 in a frame-pass layout, T = end of the register saves).

| C (SHOUGATU `func_80137C28`, 0x60-byte table) | table at | frame |
|---|---|---|
| `FnTbl24 tbl; tbl = D_80144E20; tbl.f[D_800E738A]();` | sp+0x28 | 0x88 |
| `s32 idx; FnTbl24 tbl; tbl = D_80144E20; idx = D_800E738A; tbl.f[idx]();` | **sp+0x2C** | **0x90** (original) |
| `FnTbl24 tbl; s32 idx; ...` (index declared after the table) | sp+0x30 | 0x90 |
| `int x; FnTbl24 tbl; ...` (`x` never used) | sp+0x2C | 0x90 |

GEKO `func_8013BC74` (`prev = D_800E738A; func_8007C8A4(); cur = D_800E738A; if (prev != cur) ...`) is the same rule. With only `prev` declared, the spill lands at sp+0x2C. With `s32 cur; s32 prev;`, `cur`'s slot is at 0x2C and `prev` at 0x28, as in the original.

### Evidence from the original
A scan of every splat function file (`asm/**/{matchings,nonmatchings}`) looked for a block copy (`lw/sw $at` loop or unrolled copy from `%lo(table)`) followed by a `jalr`. It found 236 functions, all `INCLUDE_ASM`, in 15 overlays: GYOZI 56, SHOUGATU 50, DATE 34, GEKO 30, EVENT 25, SHUGAKU 11, KANGEI 9, DATE2 5, VALEN 4, ENDING 3, OMIMAI 3, TAIIKU 3, MASTER 1, BUNKAKEN 1, BUNKASAI 1. In the table below, `below` is the table address minus T+16 and `above` is the frame size minus the table's end.

| below | above | table size mod 8 | functions | reading |
|---|---|---|---|---|
| 4 | 4 | 0 | 114 | one 4-byte local declared before the table and a rounding gap below it (a second 4-byte local after the table would give the same offsets) |
| 0 | 4 | 4 | 99 | one 4-byte local before the table, no gap |
| 8 | 4 | 4 | 10 | one local before, 4 or 8 bytes of locals after (the `p = &tbl.f[i]; if (*p == A \|\| *p == B) *p = C;` shape, e.g. SHOUGATU `func_80134120`) |
| 0 / 4 | 8 | 0 / 4 | 5 | two words before (TAIIKU `func_80133C80`, `func_80138A34`, DATE `func_80155004`, EVENT `func_800FC040`, GEKO `func_8013B10C`) |
| 12 | 4 | 0 / 4 | 2 | one before, 8 or 12 bytes after (GYOZI `func_80137798`, SHOUGATU `func_80136078`) |
| other | | | 6 | large functions with other locals |

All 236 have at least 4 bytes above the table. None has the bare-table layout that IDO gives for a table with no other local. The original source declared a scalar (almost always exactly one) before the table, and that scalar is the 4 bytes.

### Idiom
Declare the index local first, then the table, and assign the index after the copy:
```c
typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_80144E20;

void func_80137C28(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80144E20;
    idx = D_800E738A;
    tbl.f[idx]();
}
```
It is not a fakematch: `idx` is an ordinary used variable, and its slot comes from IDO's own rule above. A local that exists only to occupy a slot (unused, as in the `int x` row) would be a fakematch and must be marked `FAKE` (CODING_STANDARDS 7a). The two-word cases (TAIIKU) have no matched C yet for that reason.

### Line breaks change as1's schedule
Scratch tests must use normal multi-line C. If the same function is written on one line, IDO's as1 schedules the prologue as `lui t6; addiu t6; addiu v0,sp; move t0; addiu t9; sw ra`. The original, and the same C with one statement per line, give `lui t6; addiu v0,sp; addiu t6; sw ra; addiu t9; or t0`. Reordering the binasm instruction records by hand does not change as1's schedule, so the line records (`.loc`) are the likely input (inferred, not traced in as1). Before this was found, the one-line tests looked like a second toolchain difference.

### Proof
These 18 functions were matched in an uncommitted tree with the idiom: 16 table copies and the 2 GEKO spills.
- SHOUGATU `func_80137C28`, `func_801330C0`
- GEKO `func_8013BC74`, `func_8013BCBC`, `func_801323E0`
- GYOZI `func_80134520`
- RENSYU `func_80132040` (8-entry table, copy unrolled)
- MASTER `func_80133A2C`
- DATE `func_801585F0`, `func_8014FAE0`
- EVENT `func_800F93FC`
- KANGEI `func_80133C10`
- SHUGAKU `func_801336A4`
- VALEN `func_801323D0`
- OMIMAI `func_80132D44`
- DATE2 `func_801375D4`
- ENDING `func_80133F1C`
- BUNKASAI `func_80150930`

With them, `ninja` gives 27 of 27 sha1 OK and the grand total goes from 2734 to 2752 of 6958. The C is in [[data/t3330-fptab-proof.patch]], to apply after T-1321; it also adds `extern u8 D_800E738A;` to `include/ovl/ENDING.h`. No tool changed, so no matched function can change.

### Left
- SHOUGATU `func_80134120` and GYOZI `func_80137798` get the frame and table offset right with `s32 idx; FnTbl tbl; void (**p)();`. The registers still differ: the original keeps `D_800E738A` in `$a1` across both reads and the table address in `$a2`, which is the register-promotion gap of [[tickets/T-1321-register-promotion-build-step]].
- The remaining 218 table functions: apply the idiom per function (the queue can treat them as ordinary work now).
## Tooling fixes T-3300 (bugs reported by wave 2)
Ticket [[tickets/T-3300-tooling-fix-wave-2-bugs]].
- `m2c.py` picked the first overlay (sorted) with an asm file of that name. Fixed: the unit comes from `--unit`, a `UNIT:` prefix, an asm path, or the C file that holds the function's `INCLUDE_ASM`; an ambiguous name is refused with the list of units. `permute.py` does not use `locate()` (checked), so it was not affected.
- m2c printed `*(s16 *)0x801D0000` for `lui t9,(0x801D2000 >> 16); lh t0,%lo(D_801D63C8)(t9)` (EVENT `func_800F9900`). Cause: upstream m2c. It pairs a `%lo` only with a `%hi` of the same symbol and turns an unpaired `%lo(sym)` into 0 (`evaluate.py add_imm`: "replacing %lo(...) by 0"; `arch_mips.py`: "We also don't handle %lo macros at all right now"). The signedness of `lh`/`lb` plays no part; the low half is lost for every access size. Our pinned commit 708d2d2 is upstream's newest commit (checked 2026-10-09, no fix to pin), so the wrapper works around it: `m2c.py fix_lo_literals` rewrites such a `%lo(D_<8 hex>)` to the signed low half when the base register came from a `lui` with a literal. Symbols without the address in their name are not resolved. Re-check the workaround when m2c is repinned: `test_m2c.py` asserts the bare m2c still shows the bug.
- `funcdiff.py`: names starting with `L` (`LoadImage`) were treated as labels (only `L<8 hex>`, `.L*` and `jtbl_*` are now); a failed build gave a stale MATCH (it now runs `ninja <object>` and fails on a failed build or an object older than its source; `--no-build` skips ninja); `NameError: SRC_GLOB` on the host is gone, and the host gets a "use Docker" message; `--resolve` compares relocation-resolved words (the `*(T *)0xADDR` idiom). Not resolved: relocations against renamed symbols (`bg_read_sub2` for `func_8007ED84`) and against section symbols.
- `srcscan.py`: K&R definitions (`void f(a)` + parameter declarations + `{`) are definitions now; `ninja progress` no longer stops with "neither INCLUDE_ASM nor C body".
- `identify_version.py` reads `.zip` (in place), `.7z` and `.chd` (unpacked to a temporary directory with p7zip / chdman, now in the Docker image). Checked on the five archives of `versions/` in the main checkout (mounted read-only): Rev 1, Rev 2, Rev 4, Shokai Genteiban zips and the Best 7z all identify with 27 of 27 overlays. The CHD path was run on a CHD that `chdman createcd` made from the extracted Best disc (all 27 overlays identified) and on a synthetic image in the unit tests.

## Wave 3, list 9 (T-4090): 27 files, main exe and 16 overlays
Ticket [[tickets/T-4090-wave3-list-9]]. 59 of the 136 functions (8984 of 26284 bytes) matched; clean build 27 of 27 sha1 OK, `ninja progress` grand total 2805 -> 2864 of 6958 functions. Matched from the list: table-copy functions (EVENT, GYOZI, DATE, SHOUGATU, GEKO, MASTER, DATE2, OMIMAI, TAIIKU), string setters, the DATE screen set-up family, three jump-table switches, ETC bit-field loop, EN_NICHI record loops. Failed attempts that fit the T-0018 shapes are rows in [[data/t0018-cases]]. New or confirmed patterns:
- **Table copy, which scalar first.** A function-pointer table whose size is a multiple of 8 (GEKO `func_8013BBE0` 0xE8, MASTER `func_80138E58` a 12-byte pointer table, DATE2 `func_80136FC4`) needs one 4-byte scalar declared before it, as in T-3330; if the index global is loaded twice in the original (once for a compare, once for the call, GEKO `func_8013BBE0`, DATE `func_8014FBA0` stays open) the scalar is an unused local, if the index is loaded once it is `idx = D_xxx; tbl.f[idx]()`. A real counter that is declared first does the same job (TAIIKU `func_80144834`, which has a local `s32 tbl[3]` copy and a three-record loop). Tables whose size is 4 mod 8 and whose index is read twice (DATE `func_8014FBA0`, 0xAC bytes) land at sp+0x2C or sp+0x30; the original has sp+0x28: not reproduced.
- **Implicit int is the first thing to try on a near miss.** `s32 f(void)` without a `return` value (or with a bare `return;`) stops as1 from filling a branch delay slot with the next block's `lui`/`addiu` and changes how uopt compiles a `D++ >= N` / `D++ == N` test at the end of a conditional tail: DATE `func_8014FDA0` (`lw ra` stays in the slot), EN_NICHI `func_80132B40` (permuter, score 60 to 0), DATE `func_8014FD30` and ETC `func_8014A180` (the original's `sltiu`/`xori` + `beqz` instead of IDO's `bnez`; the row for `func_8014FD30` in [[data/t0018-cases]] was written before this and stays). The permuter finds it from a `void` base because it tries return types; try it by hand before the permuter. GEKO `func_8013BEE0` needed it too (with the dead second `D != 0` test of `D == 0 || (D != 0 && f() == 9)`). It did not help EN_NICHI `func_80134784`, the `u8`/`s16` clamp and sign-extension cases (EVENT `func_8010A790`, GYOZI `func_801427E4`) or DATE `func_8014F370`.
- **Load kept after a store (FAKE, same family as ENDING `func_80136C5C`).** Address the *later* statement through the earlier symbol: `(&D_800E643E)[4] -= 0x14;` for the second and third read-modify-write of adjacent halfwords (DATE `func_8014F404`, `func_8014F500`, `func_8014F610`, `func_8014F7F4`), `(&D_800E62B6)[1] += 2;` (EN_NICHI `func_80132B40`, `func_80132C4C`). When the earlier statement is a constant store and the later one a load of another symbol (SHUGAKU `func_801351CC`), store *through the symbol of the later load* with a negative index, `(&D_800E71DF)[-0x83D] = 0;`; indexing from the stored symbol does nothing.
- **Strings.** In an island object (`config/objects/*.txt` `yes`) the literal is written inline and `funcdiff.py` only shows the `.rodata` relocation pair; `build/ovl/<NAME>.bin` sha1 is the check. `printf`-style callees need a K&R declaration (`func_800AE0B0()` was `(void *)`; the call passes `D_800E6377` as second argument).
- **Jump tables in a plain function.** ETC `func_8013BD40` (cases 1-5, 10-12 and 6-9 as two groups), main `func_8007A618` (explicit empty `case 0:` to `case 5:` labels give the original's table of 0..15 and its `sltiu 16`), `func_8007B99C` all match as soon as the labels are written out; the funcdiff text only differs by the table relocation.
- **Renamed callees.** m2c prints the old `func_XXXXXXXX` names; the symbol file renames them (`config/obin_renames.txt`: `func_80043914` is `load_palette`, `func_80084D3C` is `check_para_limit`, `func_8004E788` is `set_kanji_string`, `func_8004E884` is `k_disp_start`, `func_8007EAAC` is `normal_date_move_place`). Use the new name or the link fails; do not declare the old name in `main_api.h`.
- **Result of a `void` callee passed on.** ENDING `func_8013BDD8` passes the return value of `set_kanji_string` to `k_disp_start`; matched with `#define MAIN_API_OVERRIDE_set_kanji_string` in `ENDING.h` and `s32 set_kanji_string(s16, s32, s32, void *, s32)` (the `s16` gives the `sll/sra` of the first argument).
- **Prototype width decides the call code.** `func_80044750` returns 1 and calls `func_80079B10(u16)` with the prototype (`andi t6,a0,0xffff; move a0,t6`); the same body with a K&R callee gives `andi a0,a0,0xffff`. `main_api.h` now has `s32 func_80044750(s32)` and `void func_80079B10(u16)`.
- **Bit-field tests.** ETC `func_80149468` (four-word flag array, eleven entries): a `typedef struct { u32 pad0:25; u32 grade:2; u32 pad1:3; u32 flag:1; u32 pad2:1; }` read as `((EtcSlot *)(D_800E6280 + 0x66C + i * 4))->grade >= 3 && ->flag` gives `sll 5; srl 30` and `sll 1; bgez`; the base must be `D_800E6280 + 0x66C + i * 4` (the loop pointer then starts at D_800E6280+0xC as in the original, three peeled iterations first).
- **Record loops over `D_8011ECD0` (0x44-byte records).** EN_NICHI `func_80132C4C`/`func_80132B40`: `for (i = 0; i < 64; i++) D_8011ECD0[0x1987 + i * 0x44] -= 4;` is unrolled by IDO exactly like the original (four stores after one `addiu 0x110`); the field offsets in m2c's draft are relative to the already advanced pointer, add 0x110.
- **Local struct copies are not always T-3330.** `Tri3 tbl` (12 bytes) copies in a three-record loop: the frame of the original has no stack slot for the other scalars, so the loop variable has to be the only other local (extra pointer or temp locals move the table up by 4 per local). TAIIKU `func_80144B40` (three constants in $s0/$s1/$t regs) stays open for that reason, `func_801448D4` matches except for temp numbers.

- **decomp-permuter paid off on small near misses** (`permute.py <func> --time 200`, three at a time in one container each, about 4 of 15 reached score 0): ETC `func_8014A180`, DATE `func_8014FD30` (return type, above), GYOZI `func_80135CA0` (an empty `if (!D && !D) {}` after the table-slot store; FAKE), GYOZI `func_80135D64` (`D == (D_800F563A * 0)`; FAKE), GYOZI `func_801361D4` (`u32 v = (D_800F62CF = D_800F5ACD); f(v);` is plain), GYOZI `func_801427E4` (the global is `volatile s16`: declared so in `GYOZI.h`), main `func_8007BE94` (`(arg0 ^ 0) == (u16) D`; FAKE: the left operand becomes an expression and uopt swaps the compare), main `func_80057640` (`dec_bg_show_set(1, (u32) (1 - D_800B5939))` recomputes the value for the argument instead of passing the local; FAKE). A base score of 0 in the permuter dir means the C already matches; scores that stay above 200 after 300 s were never worth more time (EN_NICHI `func_80135A48`, `func_80133924`, TT `func_8013ADC0`, main `func_80079C70`, `func_8007A50C`, `func_8007B64C`, `load_palette` best 60).

Left as `INCLUDE_ASM`, not T-0018 (the others are rows in [[data/t0018-cases]]):
- Shared constants: `D = 4; f(4)` style (EVENT `func_8010B614`), one `li` for several stores (GYOZI `func_8014210C`, four equal `sh` groups), a mask constant shared by `&= 0x7FFFFFFF` and `|= 0x80000000` (EVENT `func_8010B570`, `func_8010B6E4`, `func_8010B8A4`; T-3001).
- Four read-modify-write halfwords in a row (EVENT `func_8010B678`): the loads of the third and fourth are hoisted in the original; the index trick does not reproduce that pattern.
- TACO `func_8015D270` is SDK-style code (`multu` for signed products, `nop` after `mflo`): gcc, not IDO.
- Frames that need a spill of a computed pointer across a call without an `s` register (EN_NICHI `func_80134F2C`, TT `func_8013A610`/`func_8013A710` with locals at sp+0x30/0x5C).
- A base address that stays in a register for `D_8014A13C.field` accesses (TAIIKU `func_80143D24`) cannot be forced with a cast or a local pointer; constant offsets fold into `%lo`.

Tooling: `tools/permute.py all <func>` does not accept the documented positional form (`permute.py: error: unrecognized arguments`); `permute.py <func> --time N` works. `tools/sync_protos.py --fix` took more than ten minutes with other agents building; adding the declarations by hand to `main_api.h` (sorted by address) and running `tools/check_headers.py include src` is equivalent and instant. `git checkout include/main_api.h` throws away every other agent-visible addition of the session: never use it to undo one declaration.
