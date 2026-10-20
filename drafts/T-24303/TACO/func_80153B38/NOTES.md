T-24303 TACO func_80153B38 (fan of 40 POLY_G3 from 42 SVECTOR points): hand rewrite, 211 rows (m2c
draft 293). Needs `s32` (implicit-int caller in the same file). A separate walk pointer `p = buf` for
the second loop lets `buf` die after loop 1 as in the original (the original spills it to its home and
reloads it into s1). Left: the original keeps `col` in s6 across loop 1 (the draft reads it from its home,
`lbu` of the low byte), the SVECTOR array sits at sp+0x78 (draft 0x6C: three home words more below it),
and `cb` has a home at sp+0x5C that it spills to.
