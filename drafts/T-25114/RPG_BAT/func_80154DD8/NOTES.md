# T-25114: RPG_BAT func_80154DD8 notes

Status: **not matched**. Best body: `body.c` here, also in `src/ovl/RPG_BAT/80154430.c` under
`#ifdef NON_MATCHING`. It has **34 funcdiff rows** (`--resolve --version c`, whole unit), all aligned,
same 301 words. The default build is `INCLUDE_ASM` and still matches.

Start state: plain `INCLUDE_ASM`. The best earlier body was T-24910's at 102 rows
(`drafts/T-24910/RPG_BAT/func_80154DD8/body.c`). Its remaining classes were the frame, the inner
selector load, the quotient coalescing (`mflo a1; negu a1,a1` against `mflo v0; negu a1,v0`) and the
counters in `$v0` against `$a0`.

Fast loop: `drafts/T-25114/score.py`, run in Docker. It compiles the guarded unit and diffs the
function against its `.s`, resolving `D_XXXXXXXX+off` relocations to bytes, in about 1.5 s against
54 s for funcdiff. `drafts/T-25114/splice.py body.c` puts a body into the guard. `--list A-B` prints
both sides; `--proto` works as in funcdiff.

## What closed (102 to 34 rows)

1. **Counters are the promoted global.** `D_8015EC9C++; if (D_8015EC68 < D_8015EC9C)` (also in
   cases 2 and 3) replaces the `temp = D_8015EC9C + 1` locals. uopt then colours the global
   `D_8015EC9C` itself (regdiff: `global D_8015EC9C lr ... uses 8 defs 3 -> $a0`) instead of a CSE
   of `D_8015EC9C + 1` in `$v0`. All three counters land in `$a0`. On the old body this alone
   moves `D_8015EC68` to `$v0` and frees `t0` for ugen (every ring temporary shifts). Fix 2 is needed
   with it.
2. **Quotients and sums are their own webs.** The original computes x and y into `$v0`/`$v1` and
   negates them into `$a1`/`$a2`, which are stored to 0x48/0x44 and reloaded for the second call.
   Source shape: `x = ...; y = ...; nx = -x; ny = -y; f(.., nx, ny, ..); f(.., nx, ny, ..)`. With
   plain reads, uopt folds a single-use x into nx (`mflo a1; negu a1,a1`). Reading one operand
   through an address view, `*(s32 *)&D_8015EC58.unk_04` (case 1 also `unk_0C`, case 3
   `*(s32 *)&D_8015EC9C`), keeps x and y as their own webs, `$v0`/`$v1` (FAKE). Declaring
   `D_8015EC64` volatile gives the same rows for y. A pointer local (`s32 *p = &D_8015EC5C`) also
   works but costs 8 frame bytes per pointer. The counter view in case 3 also puts EC9C first in
   `addu v1,t7,t8`, as the original has it.
   In uopt, an indirect read comes first in a commutative op: `*q + G` and `G + *q` both put `*q`
   first. Two reads off one struct base keep source order.
3. `D_8015EC5C`/`D_8015EC64`/`D_8015EC68` are written as `D_8015EC58.unk_04/0C/10` (RpgRec18,
   section 106 of CODING_STANDARDS). Same bytes, no body externs for them.

## What is left (34 rows)

- **Frame 0x60 against 0x58** (rows 0 and 298). nx/ny are already at 0x48/0x44. Each x/y web that
  is kept by an unmovable read (address view, pointer or volatile) costs extra frame words: 4 declared
  locals take 4 words with nothing kept, 6 with x kept, 8 with x and y kept. An empty `if (x) {}` keeps
  x at no frame cost (frame 0x50 for 4 locals). That shows the original keeps its quotients by a
  second use, not by an unmovable read. But the empty test splits blocks and moves the EC9C promotion
  to the function top (392 rows). The original's 6 words are 4 locals + 2, or 5 + alignment.
- **Case 0 selector load** (rows 37-46, order only). The original loads `D_8015EBAC` into `$v1` first
  in case 0, before the five stores. Here it comes in just before the switch. A named `sel` local, with or
  without the view, is substituted at its use. Putting the statements on one line changes nothing.
- **Case 3 counter store** (rows 283-289, order only). The original has `lui at; addiu a0; sw a0`
  right after the counter load and then the test loads. Here the store-back of the promoted counter
  comes after the `EC64`/`EC5C` test loads. `++` in the test, `x = ++G`, `G = G + 1`, one line,
  views or volatile on the test reads: no change.

## Negatives added (all measured with score.py on this body)

- Toy probe (`build/T-25114/toy`, not kept) of what blocks folding a single-use quotient into
  `nx = -x`. No effect: `short`/`long`/`unsigned` x, `register`, `x *= -1`, `0 - x`, embedded
  assignment, a separate statement, self-assignment, `(void)x`, dead stores of x, `(x - x)` terms,
  u32 temps, a local array or struct holding the quotient (the array keeps the web but leaves dead
  stores). Block-scoped x/y grow the frame.
- `-x`/`-y` written in both calls (no nx/ny): uopt spills x/y and negates again after the reload. It
  does not CSE `-x`, `x*3` or `x+16` across a call.
- Pointer locals for all of EC5C/EC64/EC9C: frame +8 per pointer, homes move. One `s32 *p` for EC9C as
  well (`p[16]`) breaks the EC9C promotion (336 rows).
- Volatile `D_8015EC5C` keeps x too, but as1 then cannot move its `lui` into the dispatch delay
  slots (rows 28/34). The original has it there, so EC5C is not volatile.
- Counter forms with the old temps, `func_8014EBD0(counter)`, a reused dead parameter `arg0` as the
  counter (adds `sw a0` at entry), void return: no gain.
- `autotweak` still SKIPs at setup (cc.py stage 55), as in T-24910.

## Next hypothesis

Find the frame-free second use of x and y that the original had, a statement that uopt counts as a
use but that leaves no code and no extra block. Start from the empty-test variant `m_g` (case 1 and
case 3 `if (x) {} if (y) {}` after the negations). Keep the block structure the same, for example
with a use inside an existing statement or a compare that uopt folds late. Then add one declared
local to reach 6 frame words, and check the counter promotion stays in `$a0`. If the frame closes
this way, the selector and case 3 store order may move with it, because both are scheduling of
promoted globals.
