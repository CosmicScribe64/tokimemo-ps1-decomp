# T-25026 MAIN func_8006D7EC near-candidate decision

Only `src/main/8006CB30.c`, function `func_8006D7EC`, was investigated.
Starting body is current source at 883760224, not T-23003's older archive.
Source unchanged, SHA256:
`69adbfc29df2f7678b79f45408c8fa9cd8f8f20559ed92465eb18cf6328dea5f`.
All existing source/ABI FAKE annotations retained. No source experiments,
shared declaration/build changes, other functions or Astra consultation.

## Reliable baseline

Docker image tokimemo-decomp-arm64, normal whole-TU guarded C:
`python3 tools/funcdiff.py --resolve main:func_8006D7EC`.
1173 target / 1173 candidate instructions, one changed zero-based position
94, two printed +/- lines, aligned cost 2. The only difference is at
0x8006D964: target `addiu v1,v1,1`, candidate `li v1,1`.
Target size 4692 bytes; this is not a literal C match. No coverage added.

Thus T-23003's one-position claim remains valid in current whole-TU context.
Its earlier 13-row state and the archived hand body are not the baseline.
Do not reinterpret a two-line substitution as two differing positions.
Assembly-fallback MATCH is not evidence of a C match.

## Exact current phase evidence

Preprocessed lines 3223-3226 map to `idx = 0`, day/monthDays test, the
0xFE store, then `idx++` (source lines 283-286).

- Input ucode has idx home M/block13/offset-12. In function-relative
  records 244-246 it loads that home, emits INC 1, and stores it back.
  This confirms the source increment reaches uopt intact.
- Optimized ucode records 182-183 materialize zero into physical R3
  (v1) before the test. At records 193-195, line 3226 contains LDC 1
  followed by STR R3, with no load of R3 or increment. This is the
  first-divergence mechanism already present in uopt output.
- Stock as1 initial-node trace has machine instruction 24030001 at
  line 3226 before local/global scheduling. Changing an as1 filler or
  store offset cannot recover a discarded arithmetic dependency.
- Existing read-only instrumented uopt and stock full-TU compilation
  agree byte-for-byte on .text/.rodata/.rel.text/.rel.rodata. The fresh
  traced whole-TU resolved result is also the one-position baseline.

No claim is made that the precise internal uopt folding subphase was traced.
The phase boundary is sufficient to reject a scheduling-only next test.

## Reconciled ledger

- T-23003: typed reconstruction and u8 callee return view closed all but
  this fold; original frame and all 1173 instruction positions align.
- T-24001: 20 direct/control/data forms, then 19 bounded Astra forms
  (18 compiled). Genuine backedges retain the increment, but their branch
  and delay slot survive; other forms fold or disrupt allocation. No match.
- T-24204: function and miniature probes cover equal-constant merges,
  algebraic zeros, aggregates, narrow aliases and dead backward paths.
  Dead paths disappear before the fold. This bounds those tested forms,
  not every possible C source.
- T-24710 and the current guard record subsequent live/dead loop/goto,
  static/array/pointer, empty-test, cast, pointer arithmetic, copied-zero
  and reordered-store negatives. No repeat performed here.
- T-24600 is completion coordination, not a newer D7EC matching claim.
- T-25013's global zcopy-off experiment regressed this function from two
  printed lines to 1630 and changed other matched/guarded outputs; the
  rule was rejected and reverted. It supplies no uniform repair here.
- T-25016 provides no newer whole-TU D7EC result or source lever. Its
  packet/canonical bulk and other near-set figures do not supersede the
  current literal resolved baseline.

## Decision and revisit condition

Keep this as an accurately measured high-byte near candidate, parked on
one uopt constant-propagation dependency. No new source mechanism was
established beyond the rejected ledger. Do not repeat counter/syntax grids,
live-backedge tests or the global zcopy-off rule. The prior tests close
those specific approaches; they do not prove matching C impossible.

Resume only with evidence for a value reaching the increment that uopt
cannot prove constant while its producer still becomes the required
zero-in-register and adds no instructions/CFG edges, or independently
authorized compiler research with a uniform rule and corpus validation.
No such producer or rule was identified in this bounded check.

## Private evidence

`/private/tmp/tokimemo-t25023/build/t25026/` contains full preprocessed TU,
input draft.B, optimized draft.B.after, decoded function excerpts,
native draft.o.trace, allocator draft.log, stock as1 G/T and initial-node
log under as1-stock/, measurement.json and fidelity.json. Existing
regdiff/uopt_eval capture uses read-only trace binary
`/private/tmp/t24047-uopt/variants/trace`. All analysis and compiler work
ran in Docker; no raw listings or binaries are committed.

## Duplicate-consultation check

Root requested an Astra consultation only if no identical exact consultation
already existed. Reused T-24001's recorded Astra follow-up rather than
launching a duplicate. It targets this exact zero-initialized counter,
required addiu and folded li, with a fresh full-TU one-position result and
19 causal representation/control-flow tests. `git diff d57c78028 HEAD --
src/main/8006CB30.c` contains only the guard-comment update: the actual body,
ABI view and FAKE controls are unchanged from that consulted checkpoint.
Current uopt phase evidence and the fresh same single-word result preserve
the consultation's premise. Its unresolved barrier is retaining the counter
increment while eliminating the artificial predecessor/backedge that prevents
constant propagation. No new hypothesis supported by the later ledger or
current trace changes that premise. No new Astra spawned and no tests rerun.
