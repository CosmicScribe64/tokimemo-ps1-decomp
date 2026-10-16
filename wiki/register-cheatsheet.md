---
type: guide
updated: 2026-10-15
sources: ["wiki/matching-notes.md", "wiki/uopt-register-allocation.md", "wiki/register-allocation-wave7.md", "wiki/decompile-workflow.md", "tools/regdiff.py"]
---

# Register cheat sheet

One page for a register-only miss: find the symptom, try the C changes in order, rebuild and run `regdiff` again. The evidence for every rule is in [[uopt-register-allocation]], [[register-allocation-wave7]] and [[decompile-workflow]].

## The loop

1. Fix every non-register difference first (`funcdiff --resolve`). The register analysis only covers instructions that already agree.
2. Run `TOKIMEMO_SCRATCH=<scratch> tools/docker.sh python3 tools/regdiff.py [UNIT:]func`. Read only the `FIRST DIVERGENCE` block.
3. Make one change from the table below, then rebuild and run `regdiff` again. Repeat until it says `MATCH`, or you have tried every row that fits.
4. Near miss under about 20 lines: run `tools/permute.py --time 120`.
5. Still blocked: keep the draft under `#ifdef NON_MATCHING` and add a T-0018 row naming the regdiff hint.

## How uopt picks a register (the four facts that matter)

- **Priority** of a value is roughly (loads + stores) times loop weight, divided by how many blocks it spans. Loop weight is 10 per loop level. A value that is only stored once, or loaded once and reloaded, has priority 0 and gets no register at all.
- **Order:** in a function with calls, every value competes by priority, and the highest goes first. Ties go to the value the compiler numbered first. In a leaf function, values with few conflicts are coloured in that numbering order, not by priority.
- **Registers are tried in this order:** v0, v1, a0..a3, t0..t5, then s0..s7. The first free one wins.
- **t6..t9 and `at` belong to ugen, not uopt.** Their numbering follows expression shape and statement order. Allocator tricks don't move them.

## Symptom -> what to try

| regdiff says / asm shows | Try, in this order |
|---|---|
| **tie**: two equal-priority values swapped (constants, addresses) | The compiler numbers them by first use. Reorder the statements so the original's value is used first. In `a[i] = b[i]` the left address comes first; dropping a temporary (`v = b[i]; a[i] = v;` -> `a[i] = b[i];`) swaps the two bases. |
| **priority order**: the wrong value wins a register | Give the loser one more use, or the winner one fewer. Repeat an expression instead of keeping a temporary, or the reverse. Move a use into or out of a loop, since loop uses count 10x. |
| **extra or missing range**: the original keeps a value in a register that the draft recomputes, or the reverse | Add or remove a local for that value. A raw load in one register and a derived value in another means: declare the raw value as a local and repeat the derived expression. |
| one `li k` stored to several places in the original, separate `li`s in the draft | Chain assignment through elements: `a[0] = b[0] = k;`, or `(&A)[0] = (&B)[0] = k;` for two scalars. Separate plain scalars cannot share it. |
| `xori; sltiu; beqz`, or a switch-temporary copy `or v0,v1,zero` | Make the function `s32` with no `return` (K&R). Compare the old value: `if (D++ == k)`. |
| a `move`/`andi`/`li` into v0 near the end that nothing reads, or the outer targets of a chain stored from a v0 copy of the value | Make the function `s32` with no `return`: v0 is then live at the exit and uopt keeps the chain assignment's hidden temporary (or a promoted value) in it. Matched 5 functions in main 8007C030 (T-11590). |
| one extra ugen temporary after `x |= bit` on a flag byte | Use a bit-field store: `((Bits *)&x)->f = 1;`. |
| GameState selector read after a call lands in the wrong register | `u8 state; state = D_800E6280.unk_X;` right after the call, then compare `state`. `u32` also works; signed doesn't. |
| `bne` operands reversed but the registers are right | A narrow local for the shared constant (`s16 one; one = 1;`, mark FAKE), or a `u8 sel` copy of the selector. |
| ugen temporaries shifted by one (`t7` vs `t8`) | Change the parameter's width: `u16` vs `s16` vs `s32` starts the temporaries at different registers. Also check statement order and same-line grouping (as1 schedules by source line). |
| `li a0,k; multu` with a0 reused afterwards | The index is a struct member, not a local (`D_800E6280.unk_F5F`). Also check that the callee really takes no argument. |
| stack argument stored in the `jal` delay slot | Index the array (`p[i]`, `i++`) instead of walking a pointer. |
| narrow parameter kept in its home stack slot | Use an old-style K&R definition, and declare the promoted locals before any buffer. |
| unsigned `% 240`-style chunk with one temporary too many (ENDING) | A dead test `if (x) {}` right before `r = x % 240;`. It's FAKE: mark it T-9410. |
| loads in a different order or place than the original, with several `D_` names close together | They are probably one array or struct. Write the accesses through one base (`D_X[i]`, `(&D_X)[k]`). That fixed load order in SHOUGATU `func_8013507C` and EVENT `func_80104D48` (T-10610). |
| switch on a global byte with an extra `cvt`/copy of the selector, or the selector in the wrong register across two tables | Read the selector through a scalar byte view: `u8 sel = *(u8 *)&D_X;` or switch on `((u8 *)&D_X)[0]` (T-10860, many matches). |
| two live values share one register in the draft, the original keeps them apart (or the reverse) | An empty test `if (x) {}` or a dead compare local splits the live range (permuter finds these, T-10820/T-10850). Mark it FAKE. |
| a local holds a computed value in the wrong register (`a3` vs `v1`, a `v0`/`v1` swap), and the original stores that value to a global in every branch or right before each call | Assign the value to the global itself and read the global afterwards (`D_8011ECA0 = f(); ... D_8011ECA0 * k`, `D_800CA2A4[0] = g % 3; if (D_800CA2A4[0] == 0)`). uopt promotes the global, sinks its store into each branch and colours it like the original. Matched `func_800422C8` and `junban_init` after ten failed local-variable passes (T-16220). |
| byte stores of `x >> 16` / `x >> 8` reused twice sit in `t6..t9` in the original, the draft gives them `v0`/`v1`/`a0` | Mask them: `p->r0 = (c >> 16) & 0xFF;`. ugen drops the `and` for `sb`, so the shift stays a ugen temporary; plain shifts, chains and `u8` locals become uopt ranges (T-16220). |
| the original reloads a global after a store, the draft keeps it in a register | Store through a pointer or record view (`((Rec44 *)D_X)[96].f`, `gs->f`): uopt treats it as a possible alias and reloads. |
| the draft forwards a stored value (`$t9` temp), the original reloads | Use a compound assignment (`D_X += 0x48;`) instead of recomputing from the source. Also: `v = call(); v += k;` keeps the call result first in `addu`. |
| what stays in a register differs only around a `break`/`continue` | Join blocks decide it. Route the path straight to the shared tail (or add a redundant `break`) to match the original's block shape. |
| loop counter gone (strength-reduced) or a tail loop gets other s-registers | Write `(col << 4) - 0x80` instead of `col * 16 - 0x80` to keep the induction variable; reuse the earlier loop's `i` for a tail loop. |
| store order, delay-slot fill or `addiu` order differs, registers right | as1 schedules by source line: put the stores (or loop init and head) on one line, or permute independent store statements. |
| temporaries swapped in a sum (`u`, `v`, `clut`) | Try the operand orders of the sum; they set the ugen temp numbering. |
| frame size or local stack offsets differ | Arrays sit lowest in the locals area; scalars declared before an array go above it. Add unused locals ("homes") to reserve slots and mark them FAKE. |
| bit test compiles as `sll 30; srl 31` in the original | Bit-fields are allocated from the least significant bit: `struct { u32 low : 1; u32 f : 1; u32 rest : 30; }`. |
| reloads appear at a block start after about 20 references | uopt ends a block after 20 variable references (`varreflimit`). Split or join statements to move the boundary. |
| a struct or table field compared constant-first (`beq a1,v1`, constant in a register) and a `move` copy of it kept in another register | Read the field through a local pointer: `GameState *g = &D_800E6280; idx = g->field; if (g->field != -1)`. An indirect load is not a plain variable, so uopt puts the constant first and does not fold `idx` into the field. NAME_ENT selector-copy family, 7 functions (T-16920). |
| constant or pointer in a1/a2/v1 in a small K&R function (TT, GEKO, TACO) | Known unsolved. Spend 10 minutes at most, then guard it and log the row. |

## Do not

- Don't assume a value in `v0`/`v1` means a promoted global. Check which value regdiff names.
- Don't try allocator options or compiler flags. They were exhausted in T-0014/T-0017/T-9800.
- Don't keep m2c's `temp_*` locals by default. They add values the original doesn't have.
