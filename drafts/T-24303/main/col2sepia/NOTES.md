T-24303 col2sepia: Pix2In (u32 bit-fields) read and written through one pointer gives the original's
instruction shape (byte/halfword stores, word reloads after each store). body.c (local `u32 v`): 171
rows, dr/dg in s0/s1 as the original but `v` outranks the loaded word (regdiff: priority 45 vs 40).
Without `v` (`if (p->r0 + dr >= 0x20) ... else p->r0 = p->r0 + dr;`): 187 rows, the word and the
sums take the original's v1 but dg is spilled to its home instead of s1, and the r+g+b sum stays a ugen
temporary where the original has a uopt range in a0.
