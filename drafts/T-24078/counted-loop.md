# T-24078 counted-loop control

Matched same-TU func_8013D32C uses `for (i=0; i<2; i++)`. Its original final
induction-eliminated backedge is BNE at8013D538. Original D570 increments its
local counter at8013DE4C and branches at8013DE68 until2. The retained historical
T14500 and T17300 plain/typed sources and all earlier T24078 controls used
`do/while (i!=2)`; no prior canonical-for control was found in these packets.
This does not prove original D570 source spelling.

One control against signed indexed-uv-right618 moves counter initialization
and increment into `for (var_s4=0; var_s4<2; var_s4+=1)`. Every body expression,
other local declaration, cursor/page update, SDK field width and memory store
order stays fixed. The counter never escapes and the body does not modify it;
values0/1 execute once each, exit value2 agrees, no overflow. The source has an
explicit FAKE note because the loop form tests compiler induction handling.
Prediction: frontend loop facts might change induction/constant allocation, or
canonicalize; no register repair or exact match was promised.

Replay `--variant counted-for` measures612 aligned cost /840 diagnostic rows /
592 words /frame90, original588words. Original-mode matching is not restored:
mode7, constant160s6,136fp and page spill60 remain. UV50/54 and poly84/88 remain.
Against the exact saved618 object, all opcode counts agree and exactly15 words
change: prologue positions048..05C reorder counter initialization around other
initializers, and tail positions8DC..90C rotate UV/page reload/update scratch
registers. Every geometry-arm word is identical. Q11-before-P remains.

Therefore this is a separate small schedule/alignment result, not evidence
that saved160 priority or interference changed. Keep source/replay, do not
combine automatically with the H/type frontiers or run loop-form permutations.
Production remains ASM and same function remains active.
