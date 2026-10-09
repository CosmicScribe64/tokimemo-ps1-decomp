---
type: concept
updated: 2026-10-09
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
---

# Matching notes

Workflow: [[decompile-workflow]].

## Compiler verdict (T-0013): MIPS ucode compiler, IDO-family; IDO 5.3 is the closest available
Findings from [[tickets/T-0011-game-code-file-boundaries-and-compiler]] and [[tickets/T-0013-identify-original-compiler-pipeline]]. Toolchain details: [[toolchain]].

The game code was compiled by a MIPS/SGI ucode compiler (cfe/uopt/ugen/as1, the IDO family), not by gcc. SGI IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared -Wo,-no_const_in_reg`, via asm-processor) is now the build compiler for `src/game.c`; the SDK libs region stays gcc/ASPSX territory ([[psyq-sdk]]).

Evidence (IDO reproduces each original signature that no gcc 2.6 to 2.95 + maspsx could):

| original | IDO 5.3 | gcc + maspsx |
|---|---|---|
| `li t6,4; lui at,%hi(X); jr ra; sb t6,%lo(X)(at)` | identical (as1 splits the `sb t6,X` macro and moves the store into the `jr` slot) | `jr ra; nop` or explicit `lui v1` |
| `$t6`,`$t7`,`$t8`,`$t9`,`$t0`.. as expression temps | identical (ugen temps start at `$14`) | `$v0`/`$v1` first |
| `lh t6,X; lui at; jr ra; sh t6,Y(at)` | identical | `lhu` |
| `or rd,rs,zero` for move, `addiu rt,zero,imm` for li | identical (as1 encodings) | `addu` |
| loads never in the `jr` slot, `jr ra; nop` after a getter | identical (as1 respects the MIPS I load delay across the return) | same |
| prologue save order `sw s2; sw ra; sw s1; sw s0` (`func_80042134`) | IDO's order | not compared |

Matched with IDO 5.3 in the build (23): the 10 getters and 3 empty functions from T-0011, plus `func_800438DC`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `func_8004E99C`, `func_8004EA98` (7 of the 8 functions gcc could not match), `func_8004E750`, `func_8004E93C` and `func_80042400` (T-0014). IDO 7.1 matches the same set except `func_8004E750` (it re-uses `$a0` for the `andi` instead of `$t6`), so the original is 5.3-like or older.

### Global-address CSE: solved with `-Wo,-no_const_in_reg` (T-0014)
`func_80042400` (`D += 0x377`, value returned) is `lui v1; lw v1,%lo(D)(v1); lui at; addiu v0,v1,0x377; jr ra; sw v0,%lo(D)(at)` in the original; stock IDO 5.3/7.1 keep `&D` in a register (`addiu a0,a0,%lo(D)`, `lw v1,0(a0)`). uopt's undocumented option `-no_const_in_reg` (found in the uopt option table, passed as `-Wo,-no_const_in_reg`) stops uopt from keeping constants, including global addresses, in registers; with it the function byte-matches, and all 22 earlier matches still match. The flag is now in `IDO_CFLAGS` (`tools/cc.py`). Reading: the original uopt did not do this optimization (an older uopt, or one built/configured without it). `-Wo,-nokpicopt` has a similar but not identical effect (wrong register).

### Frame size: not reproduced (T-0014)
Every function with a stack frame is 16 bytes larger in the original than in IDO 5.3 (147 non-leaf functions start `addiu sp,-0x28; sw ra,0x14(sp)` where IDO gives `-0x18`). Where the 16 bytes go, measured against IDO 5.3 output for the same or equivalent C:

| kind | IDO 5.3 layout (from sp up) | original | example |
|---|---|---|---|
| non-leaf | args 16, saves, locals/temps | args 16, saves, **16**, locals/temps | `func_80041584` 0x18 -> 0x28; `func_80042134` saves at 0x18-0x24, frame 0x28 -> 0x38; `func_8004111C` RECT at 0x18 -> 0x28 |
| leaf with saves | saves, locals | **16**, saves, locals | `func_8007C310` s0/s1 at 0x8/0xC in 0x10 -> 0x18/0x1C in 0x20; `func_80052648` s0 at 0x14 (IDO-equivalent: 0x4) |
| leaf, locals only | locals | **16**, locals | `func_80056AA8` volatile local at 0x4 in 0x8 -> 0x14 in 0x18 |
| leaf, no locals/saves | no frame | no frame | the getters |

The 16 bytes are never accessed (checked over all 636 functions with a frame; the only hits are incoming-argument homes above the frame and one outlier, `func_8004BBA8`, which saves s0-s7/fp/ra at non-IDO offsets and may be hand-written). So leaf functions behave as if they always had the 16-byte argument build area that IDO omits in leaves, and non-leaf functions have one extra 16-byte block between the register saves and the locals/temps.

Experiments (all IDO 5.3 and 7.1, `-O2` unless noted; none changes the frame): `-g`, `-g1`, `-g3`, `-O0/-O1/-O3`, `-p`, `-32`, `-mips2`, `-KPIC`/`-call_shared`, `-xansi`, `-cckr`, `-ansi`, `-prototypes`, `-Olimit`, `-framepointer` (adds an s8 save, +24), every cfe option in its option table (`-Wf,-saveargs,-checkstack,-volatile,-Xvolatile,-check_bounds,-std0,-std1,-oldcomment,-trapuv,-use_readonly_const,-d1,-msft,-cplus,-filter,-inf,-Xfloat`), every uopt option in its table (`-Wo,-no_const_in_reg,-do_opt_saved_regs,-noheurAB,-norlodrstropt,-noprecolor,-doassoc,-docopy,-nogenvreg,-norecur,-docodehoist,-notail,-nordstore,-createbb,-moremotion,-noPalias,-static,-varref,-nokpicopt,-kpicopt,-loopunroll`), every ugen option in its table (`-Wc,-notailopt,-align8/16/32/64,-nooffsetopt,-nocpalias,-cpalias,-trapuv,-nounsignedconv,-domtag,-mips2`; `-checkstack` adds 4 and uses s7). Option tables were read from the recomp binaries' data (words byte-swapped).

Diagnostic (not a fix): in ugen (read from the matching decomp of IDO 7.1 ugen, LLONSIT/ido-decomp, `src/ugen/eval.p`) the frame is `saved_regs + arg_build + local_var_size (DEF Mmt length from uopt) + temps`, and leaves get no arg build area. Running cfe+uopt, adding 16 to the `DEF Mmt` length in the binary ucode, then ugen+as1 reproduces the non-leaf layout exactly: `func_80041584` and `func_8004111C` then byte-match (the rest of the code is plain IDO 5.3). It does not reproduce leaf functions (the extra block lands above the saves instead of below, and getters gain a frame). A C-level "unused `char pad[16]` declared last" also fixes `func_8004111C`, but only works when the function already has an address-taken local (uopt drops unused locals otherwise). Both are fakematches and are not used.

Ranked hypotheses for the exact compiler:
1. An older MIPS ucode compiler (MIPS RISCompiler 2.x/3.x as in Ultrix 4.x or Sony NEWS-OS `cc`, or IRIX 4/early-5 IDO) built or run for little-endian R3000. Fits: same cfe/uopt/ugen/as1 family; the leaf pattern is exactly "no pseudo-leaf frame optimization" (an older ugen feature level); `const_in_reg` absent in its uopt; 1995 date. Untested: no such compiler is available as a static recompilation (decompals/ido-static-recomp ships only 5.3 and 7.1; nothing else found on GitHub); running one needs an OS image (Ultrix under gxemul, NEWS-OS, IRIX 4/5 under an emulator).
2. IDO 5.x (5.0-5.2) rather than 5.3: possible, same test blocker (IRIX 5.x binaries would run under qemu-irix).
3. A hidden option or a vendor patch of IDO 5.3: unlikely; every option in all three pass tables was tried.
4. gcc of any version: ruled out (T-0013).

Next experiments (need a decision on obtaining OS images, see [[tickets/T-0100-older-mips-compiler-emulation]]): get an Ultrix 4.x or IRIX 5.2 compiler running (gxemul / qemu-irix in Docker) and compile `func_80041584`, `func_8007C310`, `func_80056AA8`, `func_80042400` with `-EL -O2 -G 0`. Until then, decompile leaf functions without stack frames only; they match.

Fallbacks (documented, NOT adopted; would be fakematches by CODING_STANDARDS section 7): (a) a cc.py stage that adds 16 to the ucode `DEF Mmt` length between uopt and ugen (non-leaf only; leaves need a second rule); (b) post-processing the object to grow the frame and shift sp offsets.

## Idioms
- Read-modify-write of a global that returns the new value (`func_80042400`): write it with a named temp (`s32 t = D + k; D = t; return t;`) to get `$v1`/`$v0`; `D += k; return D;` gives `$t6`. Needs `-Wo,-no_const_in_reg` (default build flag) for the per-access `%hi/%lo`.
- A getter `T f(void) { return D; }` with `u8`/`s32` global matches; the global must be declared `extern` with its access width in `include/game.h` (types inferred from the load/store only).
- Several `D_` symbols are accessed with different widths in different functions (e.g. `D_800B3F60` as `lbu` in `func_8004ECE0`, as halfword elsewhere). Do not declare a single type blindly; decide per symbol when more users are decompiled.
- The ninja depfile only lists `INCLUDE_ASM` files; project headers are listed in `configure.py` as implicit inputs (add new headers there).

## Leaf batch 1 (T-0400): 40 more leaf functions matched
Tool: `tools/list_leaves.py` lists the remaining `INCLUDE_ASM` functions without `jal`/`jalr`, smallest first (177 outside 0x80080000-0x80086810 at the start; `[[tickets/T-0400-leaf-function-batch-1]]`). `ninja progress` after the batch: 40/812 functions, 1404/284028 bytes (these 40 are the whole count; the 22 from T-0011/T-0013 are not in that total).

Idioms that matched with IDO 5.3:
- Straight-line stores to several different globals: one `lui $at` per store, nothing else, matches plain C. The same global accessed again in a later basic block (branches) does NOT: IDO keeps `&D` in a register (see failures).
- Compare of two globals: the operand written second in C is loaded first. `D_A == D_B` in the original (`lui t6,B; lui t7,A; lh t7,A; lh t6,B`) is written `if (D_A == D_B)` with the symbols swapped relative to the asm order (func_8004ECB4, func_80045288).
- `u8` indexed arrays and `x * 12` strides: declare `extern u8 D_xxx[]` and write `p = D + i * 12; *(s32 *)(p + off)` (func_800490C0, func_800625C0).
- 8-byte struct passed by value in `$a1/$a2` and copied field by field into an array element (func_8004E9A8, `Entry8` in `include/game.h`); copying the whole struct gives `swl/swr`.
- `if (x) D = 1; else D = 0;` and `if (D == c) return 1; return 0;` match as written. `return` of `u32 sum >> 1` cast to `s16` needs the `(u32)` cast for `srl` (func_8007C51C).
- Arg spill `jr ra; sw a0,0(sp)` (func_8004DAC4) is matched with `s32 *p = &arg0;`, marked FAKE.

Failures left as `INCLUDE_ASM` (all but the first two are the T-0014 compiler gap):
- func_80043504, func_8006CAE0: asm ends with alignment nops after the last `jr` (source-file boundary padding); plain C cannot emit them
- func_800597A0: original shares one lui at between D_800E36C0 and +4 stores; IDO gives addiu base (struct/array) or two lui (separate symbols)
- func_80042940: original puts move v0,zero before the last sb (sb in jr slot); IDO puts move in slot; tried ret var, return a=0
- func_80053CC0: D++ on u8 global: original keeps lui/lbu without address CSE then second lui at (same family as func_80042400, T-0014)
- func_800634FC func_800638C4 func_8004ADAC func_8004EBEC func_80067DD4 func_80067DFC: original repeats lui at for each access to the same global (no address CSE; IDO -O2 keeps &D in a register) and uses sltiu where IDO emits slti for u8 compares; -O1 removes the CSE but spills u8 args
- func_80046290 (and similar multi-store clusters): original shares one lui at over several stores into a global cluster (sym+off); IDO -O2 uses addiu base, even for struct/volatile; -O1 no match (T-0014 address CSE)
- func_80042908: same as func_80042940 (move v0 ordering before last sb)
- func_80059048: pointer-compare store loop in gcc-style regs (v1/a0/v0); IDO unrolls it
- func_80041840: original recomputes constant 1 in fresh temps (li t6,1 ... li t8,1), IDO CSEs it into one register
- func_8005352C: u16>>12 compare in v0/v1 with srl; IDO emits sra into t6
- func_80064E48 func_8006BD6C: global read-modify-write in two branches: original lui at per access, IDO keeps &D in a register
- Not tried: `func_80066A78`, `func_80066AC0` (`return 3`/`return 1` with a `nop` after `jr`, jump-target labels), `func_80066A2C`/`func_80066A84` (jump-table switches), `func_8004E9A8`-style larger leaves, and the other 100+ leaves; most of those contain the same-global-twice pattern above.
