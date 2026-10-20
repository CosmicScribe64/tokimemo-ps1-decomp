T-24303: func_80075C84 against the two-variable keys ring (D_800B6990[8], D_800B6998, both defined in
80075320.c since func_800755B4 matched with them). body.c is the T-20940 hand draft moved to the two
symbols: 338 rows, because uopt no longer reloads the position after `keys[pos] = k` (it did with the
one-symbol KeyRing, 53 rows). The original keeps &D_800B6990 in t4 (lui in the switch-compare delay
slots, addiu in each arm and after each call) and reloads the position; tried: a local `u8 *keys`
(folded, no reload), `(u8 *)(s32)D_800B6990` (reload comes back, but sym(at) addressing stays and
the frame grows by 8), `(u8 *)0x800B6990` (201 rows, still sym(at) addressing). Open: a source form
that keeps the keys base in a register and makes the store alias the position.
