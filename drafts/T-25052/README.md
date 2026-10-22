# MAIN schedule dispatcher, T-25052

This lane owns only `func_8007132C` in `src/main/8006CB30.c`, 3992 text bytes
(998 instructions). Production remains assembly. Reuse the existing Docker
image and mount the ignored root disc/asm inputs read-only. The probe compiles
copies of the complete translation unit and asserts production is unchanged.

Run `python3 drafts/T-25052/probe.py LABEL` inside Docker. Each named causal
candidate retains its body, actual TU, object, resolved diff and measurement
under ignored `build/t25052/LABEL`. `analyze.py LABEL` separates structural
regions from register differences; `capture.py LABEL` uses the existing tracer
mounted at `/tracer/trace` and saves normal/optimized ucode.

## Reproduction and first verified region

- Saved T-24024 baseline: 975/998 words, aligned cost1013. Calendar byte views:
  984/998, cost892. Exact reproduction on this worktree.
- `field-first-count-op`: 984/998, cost860. Reuse `op` for the first count;
  reuse `t` for the later count, eliminating unused `n`. The target frame72,
  spill offset0x30, and first101 instructions agree except exit-branch distances.
  The first count's lifetime must be independent from the later count's flag
  word: their common variable previously put the first count in a0 instead ofv0.
- The direct record pointer already compiles to the target +0x69C offset.
  Explicitly changing it to a shifted GameState pointer was a negative; do not
  adopt that representation based only on the disassembly's displaced load.

## Negatives and unresolved mechanisms

- Volatile calendar reads: 961 words/cost967, persistent s1 base and missing
  delay-slot address fills. No benefit.
- Explicit GameState record origin: 982/cost914; shared counts improve to894
  but induce index reloads where the target forwards narrowed increments.
- Empty entry switch is neutral: this function already activates the uniform
  zcopy-off boundary. Empty count test and discarded static byte views are
  also neutral here.
- Loop inequality spelling, unsigned index, and inclusive bound do not solve
  promotion. Late-folded loop bounds add substantial code (1062 words).
- Index byte views demote the overpromoted global base and fix the early
  appointment address sequence, but introduce unwanted pointer induction in
  the schedule opcode chain. All index views give967 words/cost765, while
  store-only views give964/cost768. These are lower scores, not structurally
  acceptable checkpoints. Prefix-only views do not demote the global base.
- Late pointer/subscript identities with `(u32)&D_800E6280 <0` prevent some
  early folding, but keep a spilled zero and expand the frame. Not acceptable.

The next work must preserve the verified first decision blocks while recovering
unpromoted address recomputation and immediate loop-bound11. Avoid repeating
these controls or optimizing by the aggregate score alone.
