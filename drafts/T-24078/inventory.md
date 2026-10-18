# T-24078 current618 instruction inventory

Read-only resolved original588words versus indexed-uv-right592words. No new
compiler run, capture or logging variant. Count mnemonic from each resolved
instruction; this is a coarse inventory, not sequence alignment or match proof.

| Mnemonic | Original | Current618 | Delta |
| --- | ---: | ---: | ---: |
| li |37|32|-5|
| lw |80|82|+2|
| sw |15|17|+2|
| move |7|6|-1|
| nop |74|80|+6|

All other mnemonic counts agree: addiu35, addu41, b4, beq3, beqz1, bne29,
bnez14, break28, div14, jal6, jr1, lh16, lui34, mflo30, multu16, sb11,
sh33, sll31, subu28. The differing counts sum to four extra words.

Two extra LW/two extra SW/one fewer MOVE correspond to explicit page320
spilling: initialization store, per-loop load instead of register move, latch
reload/store instead of register-only increment. Original page occupiesfp.
Original six per-arm LI160 instructions become one shared saved160 initializer,
saving five LI; UV160 initializer and rightX addiu160 otherwise remain. Current
has six additional NOPs, so eliminating constant loads does not shorten the
function. This is consistent with missing independent work in division stalls,
but exact per-NOP scheduling cause has not been traced. Do not claim proven
scheduler internals or that every register mismatch follows from this count.

The narrow source question is why current shared160 receives saved s6 while
original keeps mode s6,136s7,pagefp and materializes160 perarm. Existing captures
are604, not618, so exact current constant IR/priority is not observed. A possible
signed-versus-unsigned constant-expression partition is unproven; no literal
suffix/type experiment was run. Need one faithful source condition, not a
constant/type sweep or compiler exception. Original H is word-valued until
low16 SDK stores, which alone does not prove original local width.

Current618 still differs despite matching frame90, poly84/88, UV50/54, original
UV LW/reuse/order and original Q11-before-P order.604 numeric best remains
independent. Production ASM unchanged, same function active.
