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
