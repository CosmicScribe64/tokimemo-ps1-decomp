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
