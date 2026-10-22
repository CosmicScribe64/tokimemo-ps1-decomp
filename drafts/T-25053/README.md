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

Further allocator evidence: first constant-address range bit46 has priority3
and getsv0; quotient bit44 has priority1.4 and getsv1; selector bit53 has
priority1/3 and reusesv0. Thus fixing the counter alone cannot recover the
quotient copy and selectora2. Typed index locals coalesce; narrowing s8/u8
adds instructions (253/cost192,248/cost211), while s16 stays243 but cost172.
Reusing q/b as later rectangle arguments after definite constant assignments
is neutral144 (142 with the extra globaluse), so it does not precolor them.
These results exclude narrow conversions and simple later argument reuse as
the missing copy mechanism. Traces remain private; no binary dumps committed.

New best: `signed-selector.c` has243/243 words, aligned cost104. Replay with
the same T-24516 driver above, replacing the final candidate path. A signed
counter local with explicit unsigned division recovers target countera1 and
quotienta0 (cost138 alone). Empty selector use before counter initialization
recovers selectora2 (cost112 alone); combined cost104. The loaded byte is0..255,
so signed storage and unsigned division preserve the computation. Selector use
is diagnostic FAKE and changes its load scheduling; it remains unmatched.
Signed counter plus reversed zero assignments scores136; plus empty counter
use scores138. Missing quotient copy persists; selector load is too early.

Follow-up: reversing the two zero resets (b before q, matching target order)
improves this same archived candidate to102 aligned cost, still243/243 words.
Making q signed or using a separate s32 index is neutral104 before that reset
change. Narrow distinct index after combined fixes worsens: s16 cost132,
u16 cost166 (both243 words), u8 cost177 at248 words. The quotient copy is not
explained by these narrowing controls.
