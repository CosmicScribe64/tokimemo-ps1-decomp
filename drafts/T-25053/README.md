# Final BUNKA_SD function research

Production remains unchanged. Start with T-24516 semantic repairs, not the
older guard's wrong selector polarity, coordinates and argument ordering.

Docker replay:

    tools/docker.sh python3 drafts/T-24516/replay.py BUNKA_SD func_80134D94 drafts/T-25053/counter-use.c

Current diagnostic candidate has243/243 words and142 aligned cost, versus144
for repaired baseline. Neither is a literal match. Actual function scope is960
bytes; comparison span includes padding.

Instrumented uopt on baseline identifies first register divergence at the
byte-counter load: local b (bit39,priority0.6) is constrained and allocated
before the global counter/address range (bit41,priority0.5). The extra global
use in counter-use.c swaps the local value into target a1, but leaves its
address in a0 and quotient/selector allocation wrong. Therefore this is a
causal checkpoint, not a solution. Trace captured privately with existing
/scratch/variants/trace, using TOKIMEMO_SCRATCH=/private/tmp/t24047-uopt.

Negative controls: u8 counter, empty quotient use, distinct s32/u32 index,
late-zero index plus later empty quotient use, and signed quotient storage
all remain144. Distinct u16 index worsens200. Signed quotient plus global use
remains142. First switch still lacks target a0-to-v0 quotient copy.
