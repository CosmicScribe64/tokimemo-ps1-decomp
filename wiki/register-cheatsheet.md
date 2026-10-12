---
type: guide
updated: 2026-10-12
sources: ["wiki/uopt-register-allocation.md", "wiki/register-allocation-wave7.md", "wiki/decompile-workflow.md", "tools/regdiff.py"]
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
| one extra ugen temporary after `x |= bit` on a flag byte | Use a bit-field store: `((Bits *)&x)->f = 1;`. |
| GameState selector read after a call lands in the wrong register | `u8 state; state = D_800E6280.unk_X;` right after the call, then compare `state`. `u32` also works; signed doesn't. |
| `bne` operands reversed but the registers are right | A narrow local for the shared constant (`s16 one; one = 1;`, mark FAKE), or a `u8 sel` copy of the selector. |
| ugen temporaries shifted by one (`t7` vs `t8`) | Change the parameter's width: `u16` vs `s16` vs `s32` starts the temporaries at different registers. Also check statement order and same-line grouping (as1 schedules by source line). |
| `li a0,k; multu` with a0 reused afterwards | The index is a struct member, not a local (`D_800E6280.unk_F5F`). Also check that the callee really takes no argument. |
| stack argument stored in the `jal` delay slot | Index the array (`p[i]`, `i++`) instead of walking a pointer. |
| narrow parameter kept in its home stack slot | Use an old-style K&R definition, and declare the promoted locals before any buffer. |
| unsigned `% 240`-style chunk with one temporary too many (ENDING) | A dead test `if (x) {}` right before `r = x % 240;`. It's FAKE: mark it T-9410. |
| loads in a different order or place than the original, with several `D_` names close together | They are probably one array or struct. Write the accesses through one base (`D_X[i]`, `(&D_X)[k]`). That fixed load order in SHOUGATU `func_8013507C` and EVENT `func_80104D48` (T-10610). |
| constant or pointer in a1/a2/v1 in a small K&R function (TT, GEKO, TACO) | Known unsolved. Spend 10 minutes at most, then guard it and log the row. |

## Do not

- Don't assume a value in `v0`/`v1` means a promoted global. Check which value regdiff names.
- Don't try allocator options or compiler flags. They were exhausted in T-0014/T-0017/T-9800.
- Don't keep m2c's `temp_*` locals by default. They add values the original doesn't have.
