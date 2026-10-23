T-25007 pass on RPG_BAT func_8014BFE4. Best draft is still drafts/T-17110/RPG_BAT/func_8014BFE4/hand.c
(128 funcdiff rows; autotweak 300 s: 128 -> 126, casts only).
- Original registers: idx in s2, idx+8 in s5 (computed before the first call of the digit arm), digit in s1
  (y spilled to its home 0x70 around the arm), div in a1 (spilled at 0x58), t = slot*12 a memory-only local
  at 0x50 that is reloaded three times after the loop; t + 0x40 after the loop is a fresh CSE in s0.
- Build: div takes s5 (idx+8 is not a range), and `t + 0x40` after the loop is the pre-loop value kept in a
  spill slot (uopt value-numbers it with the pre-loop idx init). That and the head chain (the original
  makes a fresh dead copy of a3/t7 for every read in the first blocks) are the rows.
- Tried, worse: `u = t + 0x40` and `u + i` call arguments (255), t as a named local (164), idx as the reused
  arg0 (223), `j = idx + 8` before the first call (forward-substituted, 162), post-loop `t + 0x44/0x48/0x40`
  without idx (164), `div = 1` before the switch (241), head chain as else-if/flat/`a = a + 4` (128-169).
- Permuter 600 s on the hand draft: best candidate adds `idx += 0;` (internal score 3336 -> 2751), still 128 funcdiff rows.
