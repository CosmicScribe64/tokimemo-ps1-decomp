# func_8008647C (T-17022)

Best draft: body.c, 24 differing lines (12 pairs) from the m2c start of 128 rows. What is left: the two record sums
(`unk_06 + unk_02` of `best` and of the list entry) sit in $a0/$a1 and the entry in $a2 in the original; the draft keeps
the sums in ugen temporaries ($t6/$t9) and the entry in $a0.
Naming the sums as locals `x`, `y` gives all three registers right (draft e/g) but the frame grows by 8 bytes. The
original has array at 0x6C, frame 0xB0 = six scalars below the list and one (`cur`) above it.
