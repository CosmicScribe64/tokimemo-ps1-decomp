---
type: concept
updated: 2026-10-09
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
---

# Matching notes

Workflow: [[decompile-workflow]].

## Compiler verdict (T-0013): MIPS ucode compiler, IDO-family; IDO 5.3 is the closest available
Findings from [[tickets/T-0011-game-code-file-boundaries-and-compiler]] and [[tickets/T-0013-identify-original-compiler-pipeline]]. Toolchain details: [[toolchain]].

The game code was compiled by a MIPS/SGI ucode compiler (cfe/uopt/ugen/as1, the IDO family), not by gcc. SGI IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared`, via asm-processor) is now the build compiler for `src/game.c`; the SDK libs region stays gcc/ASPSX territory ([[psyq-sdk]]).

Evidence (IDO reproduces each original signature that no gcc 2.6 to 2.95 + maspsx could):

| original | IDO 5.3 | gcc + maspsx |
|---|---|---|
| `li t6,4; lui at,%hi(X); jr ra; sb t6,%lo(X)(at)` | identical (as1 splits the `sb t6,X` macro and moves the store into the `jr` slot) | `jr ra; nop` or explicit `lui v1` |
| `$t6`,`$t7`,`$t8`,`$t9`,`$t0`.. as expression temps | identical (ugen temps start at `$14`) | `$v0`/`$v1` first |
| `lh t6,X; lui at; jr ra; sh t6,Y(at)` | identical | `lhu` |
| `or rd,rs,zero` for move, `addiu rt,zero,imm` for li | identical (as1 encodings) | `addu` |
| loads never in the `jr` slot, `jr ra; nop` after a getter | identical (as1 respects the MIPS I load delay across the return) | same |
| prologue save order `sw s2; sw ra; sw s1; sw s0` (`func_80042134`) | IDO's order | not compared |

Matched with IDO 5.3 in the build (22): the 10 getters and 3 empty functions from T-0011, plus `func_800438DC`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `func_8004E99C`, `func_8004EA98` (7 of the 8 functions gcc could not match) and `func_8004E750`, `func_8004E93C`. IDO 7.1 matches the same set except `func_8004E750` (it re-uses `$a0` for the `andi` instead of `$t6`), so the original is 5.3-like or older.

Not reproduced (the original is not stock IDO 5.3; probably an older or Sony-configured MIPS compiler):
1. Frame size: every non-leaf frame is 16 bytes larger than IDO's (147 functions start `addiu sp,-0x28; sw ra,0x14(sp)` where IDO gives `-0x18`; `func_80042134` saves s0-s2/ra at IDO's offsets 0x18-0x24 but allocates 0x38 instead of 0x28; in `func_8004111C` a RECT local sits at 0x28 in a 0x30 frame, IDO puts it at 0x18 in 0x20). The extra 16 bytes sit between the register save area and the locals. No IDO 5.3/7.1 flag tried changes it (`-g*`, `-O1/-O3`, `-cckr`, `-ansi`, `-mips2`, `-framepointer` (+s8, not +16), unused locals, K&R calls). This blocks all non-leaf functions.
2. Global-address CSE: `func_80042400` (`D += 0x377; return`) is `lui v1; lw v1,%lo(D)(v1); lui at; addiu v0,v1,0x377; jr ra; sw v0,%lo(D)(at)` in the original; IDO 5.3 and 7.1 (`-O1/-O2/-O3`, several source shapes) keep `&D` in a register (`addiu v1,v1,%lo(D)`, `lw t6,0(v1)`). Left as `INCLUDE_ASM`.

Ranked hypotheses for the exact compiler:
1. An older MIPS Computer Systems / SGI ucode compiler (MIPS RISCompiler 2.x/3.x or IDO 3.x/4.x era), as shipped with Sony NEWS (NEWS-OS) workstations, reportedly early PlayStation development hosts, built for `-EL`. Fits: same front/back-end family, 1995 date, different frame layout and uopt heuristics. Untested: no such binaries are available as static recompilations.
2. IDO 5.3 with an ugen/uopt option we have not found (e.g. a reserved 16-byte area). Weaker: the frame difference is systematic, and the CSE difference is in uopt, a different pass.
3. Not gcc of any version: ruled out by the table above.

Next experiments: look for older IRIX/NEWS-OS/Ultrix MIPS compilers (Ultrix 4.x `cc` is native little-endian) runnable under qemu-irix or a static recompilation; diff ugen frame code between IDO 5.3 and 7.1 recomp sources for a frame-padding parameter; meanwhile decompile leaf functions only (they match IDO 5.3 exactly). Do not fake the frame size.

## Idioms
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
