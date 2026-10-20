# T-24502 pickup handler checkpoint

Not matched; production remains original asm. Repaired stale draft: use signed s32 sentinels for -7/-1, separate random result from narrow fields, Tc14 pointer increments and end bound (Tc14 *)&D_8015EDB0, and typed actor8 fields. Reuse random result in the bonus local before switch to avoid an uninitialized saved register range.

Best sensible baseline cost251,348 scratch words versus347 target (one alignment nop). Original first-loop exit move v0,zero disappears with no-return view; bonus cases then use v1 instead of original v0 and propagate ring differences. Explicit return/call-result assignments worsen cost287. Replacing bonus local with global D_8015EDD4 lowers comparison cost161 but introduces per-case global stores and364 words, so it is not the preferred reconstruction. These bounded alternatives were not promoted. No compiler edits or source grids.
