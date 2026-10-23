# T-25101: main get_near_menu

Result: matched as default C, 1784 bytes (446 words), commit "T-25101: match main:get_near_menu". No FAKE.

## Start state

`src/main/8004F870.c` held plain `INCLUDE_ASM`. The archived drafts (T-14500, T-16000 at score 1488, T-17000 m2c error, T-12050 at 1424) were raw m2c output with unrolled pointer walks, so the body was written again from the asm.

## What the function does

`get_near_menu(mode, dir, menu)` picks the menu entry nearest to the current one in a direction:

- `n` is the entry count of `menu`, at most 16.
- `diff[i]` is the x offset (mode 0) or the y offset (mode 1) of entry i from the current entry. IDO unrolls the loop by four.
- `dir == -1` turns negative offsets into positive distances and sets positive ones to 0. `dir == 1` sets negative offsets to 0. Other `dir` values leave `diff` as it is.
- The scan skips disabled entries (`unk_07`) and entries at distance 0. An entry on the same row (mode 0) or column (mode 1) wins with the smallest perpendicular distance. Such distances are stored as negative values below the -320 start. Otherwise the smallest distance wins, and a tie goes to the entry with the smaller cross offset.

## Measurements (sbs = structure-aligned instruction diff)

1. Natural reconstruction, tie block written as `if (mode == 0) ... else if (mode == 1)`: 352 differing instructions. `mode` was not allocated (regdiff: bit 21, priority -1.846, "-ve save"). It was spilled to its home and reloaded, which shifted every saved register by one, kept the tie's first `abs` result in s1 instead of the 0x7C/0x80 spill slots, and gave a 176-byte frame.
2. The tie block as `switch (mode) { case 0: ... case 1: ... }`: `mode` in fp and the constant 1 in a2, which matches the target's `beqz fp` / `beq fp,a2` chain. 9 rows were left, all the `diff` base (sp+0x94 against 0x88).
3. `s16 diff[16]` declared after `g, n, best_d, best` (four scalar slots above the array): MATCH.

Control: case 3 with the tie block put back as an if-chain is again "mode not allocated, -ve save". The switch is the lever, not the declaration order.

## Levers that were already right in the first draft

- The `i = 0` resets that come before the second and third loops are the `for` inits. uopt moves them into the predecessor arms, as in sndi.
- `if (best_d < -abs(...)) { best_d = -abs(...); best = i; }` repeats the call. The second result is truncated to s16, which gives the sll/sra pair.
- `if (best >= 0) return best; return -1;` gives `bltz s4` with `li v0,-1` in the delay slot.

## Negatives

- An if/else-if chain on a parameter that is live across calls in a loop can lose the parameter's saved register. A switch with the same arms keeps it.
