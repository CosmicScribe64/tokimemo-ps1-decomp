# T-17016 drafts

`main/parameter_up_down/body.c`: the best form of `parameter_up_down` found in T-17016 (221 funcdiff lines, 307 of 307
instructions, frame size right; the T-17000 draft needed a different frame). It reuses `arg1` and `arg2` for the
`4/1` and `2/1` multipliers (the original keeps both in `s4`/`s3`), writes the `(arg0 == 4)` loop and the table loop as
an if/else with no `return`, and repeats `D_800E6280.unk_0F4.b[1] & 0xF` instead of a named selector. Open: the
original loads the mode byte into `$v1` and masks it into `$v0` (the selector range starts before the load, so the
`lui`/`lbu` pair takes `$v1`, and `$v0` is free for the later `lhu`), the spill of `row` sits at sp+0x4C (ours sp+0x58),
and every later temporary is shifted by the missing `$v0`. See wiki/matching-notes.md, section T-17016.
