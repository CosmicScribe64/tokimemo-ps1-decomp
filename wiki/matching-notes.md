---
type: concept
updated: 2026-10-09
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h"]
---

# Matching notes

Findings from the first decompiled `game` functions ([[tickets/T-0011-game-code-file-boundaries-and-compiler]]). Workflow: [[decompile-workflow]]. Toolchain: [[toolchain]].

## Compiler verdict (provisional, with evidence)
Current setup: `gcc 2.7.2-psx` (Sony, "GNU C 2.7.2 [AL 1.1, MM 40]") `-O2 -G0 -mcpu=3000` + maspsx `--aspsx-version=2.79` + GNU as. It byte-matches only the simplest shape:

- Matched (10): global getters of the form `lui v0,%hi(X); lbu/lw v0,%lo(X)(v0); jr ra; nop` (`return D_xxx;`): `func_8004480C`, `func_8004481C`, `func_800451D0`, `func_800460CC`, `func_800460DC`, `func_800460EC`, `func_80046274`, `func_8004901C`, `func_8004ADD4`, `func_8004EC14`.

Everything else that was tried (setters, `&sym` returns, `x = y`, `x += c`; 10 more functions) does NOT match with any compiler in `/opt/gcc` (2.6.3-psx, 2.7.2-psx, 2.7.2-cdk, 2.8.0-psx, 2.8.1-psx, 2.91.66-psx; best was 4/10 with 2.7.2-cdk and 2.8.x). Original code shape that gcc could not produce:

| original | gcc + maspsx |
|---|---|
| store to a global: `li t6,4; lui at,%hi(X); jr ra; sb t6,%lo(X)(at)` (macro `sb t6,X` expanded via `$at`, delay slot filled with the store) | 2.7.2-psx / 2.6.3-psx: cc1 leaves all scheduling to the assembler and emits `jr ra; nop` (gas cannot move a relocated insn into the slot). 2.7.2-cdk / 2.8.x: cc1 fills the slot itself but emits explicit `lui v1,%hi; sb v0,%lo(v1)`, never `$at` |
| first temp register is `$t6` (`$14`), then `t7,t8,t9,t0,t1` (also seen in `func_800410AC`: `t6,t7,t8,t9,t0,t1`) | gcc 2.6 to 2.95 allocate `$v0`/`$v1` first in leaf code |
| signed halfword copy `lh t6,X; sh t6,Y` | gcc emits `lhu` then `sh` |
| `lw v1,X; lui at; addiu v0,v1,0x377` (load delay slot filled with the `lui at` of the following store) | gcc: `lw v0,X; nop; addiu v0,v0,0x377` |
| `move` encoded as `or rd,rs,zero` (`or a2,zero,zero` before calls) | gas encodes `move` as `addu` |

Hypothesis (unproven): the game was not built by Sony's gcc-based ccpsx plus ASPSX as modelled by maspsx; the register order (`t6` first), `$at` store expansion with delay-slot filling, `lh`/`or` idioms resemble SGI/MIPS `cc` (IDO-style) output, or an assembler that performs real reordering/register-agnostic delay-slot filling. Next steps (not done): check whether the toolchain can reproduce `or` for `move` and slot filling (e.g. a maspsx or custom post-pass), compare against known IDO output on the same C, and examine more complex functions (loops, switch, s0-s7 usage) before concluding. Keep the gcc setup meanwhile: it is harmless (all other functions stay `INCLUDE_ASM`).

Unmatched examples left as `INCLUDE_ASM`: `func_80042400`, `func_800438DC`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `func_8004E99C`, `func_8004EA98`.

## Idioms
- A getter `T f(void) { return D; }` with `u8`/`s32` global matches; the global must be declared `extern` with its access width in `include/game.h` (types inferred from the load/store only).
- Several `D_` symbols are accessed with different widths in different functions (e.g. `D_800B3F60` as `lbu` in `func_8004ECE0`, as halfword elsewhere). Do not declare a single type blindly; decide per symbol when more users are decompiled.
- The ninja depfile only lists `INCLUDE_ASM` files; project headers are listed in `configure.py` as implicit inputs (add new headers there).
