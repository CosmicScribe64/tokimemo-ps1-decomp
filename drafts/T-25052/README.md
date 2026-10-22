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
- Empty default-only entry switch is neutral. Actual input ucode has
  `OPTN 405 1` (zcopy on); cfe drops the selector before creating any temporary.
  A single empty case still has no temporary. Two cases or an existing local
  declared `register` and assigned an unsigned global before the first call
  trigger `OPTN 405 0` through the existing uniform build rule. The latter
  assignment emits no runtime work. Empty count and discarded byte-view tests
  are neutral. The earlier zcopy-off claim was incorrect.
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

## Compiler-option verification

`capture.py` retains the entire input stream in `trace/draft.B`; the OPTN record
immediately before the relevant ENT is authoritative. Do not infer it from
an empty switch or a neutral output. Accepted D138 also has zcopy on; its
README explanation is corrected, with historical logs left untouched.

Whole-GameState aliases, address-only volatile qualification, integer record
arithmetic, direct and local-carried unreachable-call controls did not improve
the verified prefix candidate. Zero-based pointer identities can add a spilled
zero or change alias induction, so word count alone is not evidence of progress.

## Loop bounds and tail switch

`first-exact-do-live-late-bound` preserves the first101 instructions and removes
all three promoted bound11 ranges in loops whose index remains live. The index
increment is a separate full expression, followed by
`while (i != 11 + ((u32)i < 0))`; the unsigned comparison is defined zero.
This is984/998 words, cost854. Applying the identity to every loop is the older
negative above; only the three index-carrying loops benefit. There is no extra
runtime work, and the target immediate11 branch/delay-slot pointer increment
returns in each loop.

The tail needs an unsigned switch and an explicit `case 0xFF` joining default.
`late-bounds-unsigned-switch-ff` restores seven missing branch-tree words:
991/998, cost847. Adding the defined zero identity to the selector recovers the
target's branch-delay selector copy, but colors remain wrong (992/998,cost852).
Reloading the selector through `p` adds an unwanted load. Making `op` a byte
adds unwanted narrowing to the first count. A word-unsigned `op` is identical
to casting the switch selector. Separate block-local counts produce an88-byte
frame instead of72 and are rejected (991words,cost869 with repaired bounds).
