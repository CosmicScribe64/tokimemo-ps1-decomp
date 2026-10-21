Trace evidence for T-24927 (as1 per-block scheduler, BUNKA_SD:func_80139290).

Captured with the T-24900 recipe (drafts/T-24900-as1/ scripts copied to
build/as1tr/, traced as1-x2 from the T-24047 scratchpad):
  sh /work/build/as1tr/xtr.sh src/ovl/BUNKA_SD/801388D0.c /work/build/as1tr/cap24927
  python3 /work/build/as1tr/fn.py .../unit.c .../xtr.log func_80139290
Full logs under build/as1tr/cap24927/ (gitignored): xtr.log (raw trace),
unit.c (preprocessed, NON_MATCHING on), in.G/in.T (as1 input).

trace-arm-0x3B-builtin.txt: the case 0x3B arm block's node list and the
per-block scheduler's picks for the current (built) source. Line numbers
refer to build/as1tr/cap24927/unit.c:
  3381 case 0x3B:
  3382     D_801206AC = 2;
  3383     break;
Nodes (INST order = ugen emission order = source order):
  INST 2 li t7,2   (line 3382, aftercycles 2)
  INST 3 lui at    (line 3382, aftercycles 2)
  INST 4 sh t7     (line 3382)
  INST 5 li t2,0x60 (line 3383, uopt-duplicated join preamble)
  INST 6 lui t0    (line 3383, aftercycles 3)
  INST 7 lbu t0    (line 3383, aftercycles 2, maxhazard 2)
  INST 8 b         (line 3383)
Pick order: aftercycles DESC, ties by INST ASC:
  lui t0 (3) -> li t7,2 (2, INST 2) -> lui at (2, INST 3) -> lbu t0
  (2, INST 7) -> sh -> b -> li t2 (branch delay slot).
The original needs the lbu picked before li t7/lui at, i.e. the preamble
emitted before the arm's store in source order.
