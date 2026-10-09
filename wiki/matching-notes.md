---
type: concept
updated: 2026-10-09
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
---

# Matching notes

Workflow: [[decompile-workflow]].

## Compiler verdict (T-0013): MIPS ucode compiler, IDO-family; IDO 5.3 is the closest available
Findings from [[tickets/T-0011-game-code-file-boundaries-and-compiler]] and [[tickets/T-0013-identify-original-compiler-pipeline]]. Toolchain details: [[toolchain]].

The game code was compiled by a MIPS/SGI ucode compiler (cfe/uopt/ugen/as1, the IDO family), not by gcc. SGI IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared -Wo,-no_const_in_reg`, via asm-processor) is now the build compiler for the game code (`src/main/*.c`); the SDK libs region stays gcc/ASPSX territory ([[psyq-sdk]]).

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

### Global-address CSE: solved with `-Wo,-no_const_in_reg` (T-0014)
`func_80042400` (`D += 0x377`, value returned) is `lui v1; lw v1,%lo(D)(v1); lui at; addiu v0,v1,0x377; jr ra; sw v0,%lo(D)(at)` in the original; stock IDO 5.3/7.1 keep `&D` in a register (`addiu a0,a0,%lo(D)`, `lw v1,0(a0)`). uopt's undocumented option `-no_const_in_reg` (found in the uopt option table, passed as `-Wo,-no_const_in_reg`) stops uopt from keeping constants, including global addresses, in registers; with it the function byte-matches, and all 22 earlier matches still match. The flag is now in `IDO_CFLAGS` (`tools/cc.py`). Reading: the original uopt did not do this optimization (an older uopt, or one built/configured without it). `-Wo,-nokpicopt` has a similar but not identical effect (wrong register).

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
1. `-Wo,-no_const_in_reg` is too blunt. The original hoists loop-invariant integer constants and global addresses out of loops into registers (`func_8013815C`, `SD_DetectCDPeak`, `strSync`, `func_801464D4`: `li v1,1`, `lui/addiu` before the loop, `multu` by a register holding 0x44), but does not keep a global address in a register across straight-line code (`func_80042400`). Without the flag all C functions in the project still match except `func_80042400`; with it, loops do not. No single uopt option gives both (all 19 tried, one at a time). Needs a decision: drop the flag and give up `func_80042400`, or keep it.
2. `LoadSquare`, `StoreSquare`, `MoveSquare`: the original loads the u16 parameter homes into `t8,t9` / `t7,t8,t9`, IDO into `t0,t1` / `t8,t9,t0` (stock IDO without the pass gives the same). Register allocation, kept as `NON_MATCHING`.
3. `func_8004111C`: the original stores the RECT's h and w before x and y (`NON_MATCHING`). `func_80044700`: the original frame has 8 more bytes of locals than the body needs.
4. `strSync`: besides the constant hoisting, the original does not reload the decremented counter after storing it, which IDO's `volatile` handling does.

## Object padding (T-0012)
The alignment nops after the last function of a source file (the reason `func_80043504` and `func_8006CAE0` once failed) are now produced by the uniform padding pass in `tools/cc.py`; rule, evidence and verification are in [[toolchain]] (section Object padding pass) and [[source-files]].

## Idioms
- Read-modify-write of a global that returns the new value (`func_80042400`): write it with a named temp (`s32 t = D + k; D = t; return t;`) to get `$v1`/`$v0`; `D += k; return D;` gives `$t6`. Needs `-Wo,-no_const_in_reg` (default build flag) for the per-access `%hi/%lo`.
- A getter `T f(void) { return D; }` with `u8`/`s32` global matches; the global must be declared `extern` with its access width in `include/game.h` (types inferred from the load/store only).
- Several `D_` symbols are accessed with different widths in different functions (e.g. `D_800B3F60` as `lbu` in `check_set_k`, as halfword elsewhere). Do not declare a single type blindly; decide per symbol when more users are decompiled.
- The ninja depfile only lists `INCLUDE_ASM` files; `configure.py` lists every `include/*.h` and `include/*.inc` (by glob) as an implicit input of the C compiles, so new headers need no `configure.py` edit.

## Leaf batch 1 (T-0400): 40 more leaf functions matched
Tool: `tools/list_leaves.py` (now over all `src/main` files; prints the file as a third column) lists the remaining `INCLUDE_ASM` functions without `jal`/`jalr`, smallest first (177 outside 0x80080000-0x80086810 at the start; `[[tickets/T-0400-leaf-function-batch-1]]`). `ninja progress` after the batch: 40/812 functions, 1404/284028 bytes (these 40 are the whole count; the 22 from T-0011/T-0013 are not in that total).

Idioms that matched with IDO 5.3:
- Straight-line stores to several different globals: one `lui $at` per store, nothing else, matches plain C. The same global accessed again in a later basic block (branches) does NOT: IDO keeps `&D` in a register (see failures).
- Compare of two globals: the operand written second in C is loaded first. `D_A == D_B` in the original (`lui t6,B; lui t7,A; lh t7,A; lh t6,B`) is written `if (D_A == D_B)` with the symbols swapped relative to the asm order (check_end_k, func_80045288).
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
