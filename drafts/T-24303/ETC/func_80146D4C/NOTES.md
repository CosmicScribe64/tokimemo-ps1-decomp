T-24303 draft of ETC func_80146D4C (typed from the T-17300 m2c draft, score 180 -> 104 rows; 88 with
`--proto 's32 func_800AE0B0();'`, which needs a MAIN_API_OVERRIDE wrap in main_api.h).
- `sub` is reused for the func_801464D4 result (puts it in s0 like the original).
- Declaration order grade, score, w, ret, main, sub, bio gives the original frame 0x58 and homes
  (ret 0x48, main 0x44, bio 0x3C).
- Left: every read of D_800E6280.unk_041 sits in a3 in the original (a uopt range for the promoted
  global); in the draft that range has priority 0 (one use per call-free region) and stays uncoloured,
  so the reads are ugen temporaries. Repeating the reads in the grade chain does not change it.
- Case 2 loads unk_110 straight into s0 in the original (v0 in the draft).
