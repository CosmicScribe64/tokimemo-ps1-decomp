---
type: research
updated: 2026-10-15
ticket: T-9800
sources: ["tools/ido-ecvt.patch", "tools/uopt_eval.py", "tools/regdiff.py"]
---

# uopt register allocation: trace and uniform experiments

Results for [[tickets/T-9800-uopt-register-allocation-trace-and-uniform-patch]].
Context: [[original-compiler]], [[ido-52-evaluation]],
[[register-allocation-wave7]], T-9000 in [[matching-notes]].
Tool built on this trace: `tools/regdiff.py` (T-10400, section "regdiff" at the end and [[decompile-workflow]]).

## Reproducibility and source boundary

All analysis and compiler builds run through tools/docker.sh. Set
TOKIMEMO_SCRATCH to the shared external scratchpad; it is mounted at /scratch.
This ticket writes only /scratch/r9-uopt. No compiler binary or reconstruction
source is added to the repository. Experiments use the existing pinned
ido-static-recomp revision 9c242adc890beef098020149d9554f48208f699d.

Reference read (not copied into the repository):
[n64decomp/ido uoptreg2.c](https://github.com/n64decomp/ido/blob/d068e439f52615763a3facd6944873899ebad2fd/src/uopt/uoptreg2.c),
revision d068e439f52615763a3facd6944873899ebad2fd. Its symbolic function names
can be aligned with the generated recomp C. Addresses in comments belong to
the reference build, so generated labels are checked independently.

To reproduce in an external scratch checkout, run these commands **inside**
tools/docker.sh with TOKIMEMO_SCRATCH set. Clone with submodules and check out
the pinned revision above (the recomp, not the reference reconstruction).
The generated source must hash to
`b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f`.

```sh
cd /scratch/r9-uopt/recomp
mkdir -p ../variants
patch --fuzz=0 -p1 < /work/tools/ido-ecvt.patch
make RELEASE=1 setup
make RELEASE=1 VERSION=5.3 IDO_TC=uopt build/5.3/out/uopt
python3 /work/tools/uopt_patch.py trace build/5.3/uopt.c ../variants/trace.c
gcc -std=c11 -Os -fno-strict-aliasing -I. ../variants/trace.c \
    build/5.3/libc_impl.o build/5.3/version_info.o -lm -o ../variants/trace
cd /work
python3 tools/uopt_eval.py cases /scratch/r9-uopt/trace-manifest.json \
    /scratch/r9-uopt/reproduced-traces --uopt /scratch/r9-uopt/variants/trace --trace
```

Replace `trace` with a candidate name and omit `--trace` for allocation
experiments. Run `regress /scratch/r9-uopt/reg-<candidate> --uopt <binary>`
against a freshly verified normal build, then `cases` with frozen-manifest.json.
Manifests, source drafts, generated recomp output, binaries and detailed traces
stay in scratch. The ecvt patch applies with zero fuzz. No Dockerfile or normal
compiler binary has been changed.

## Trace crash

The synthetic source `extern int d; int f(void) { return d+d; }`, compiled
with the normal IDO flags plus `-Wo,-zdbug:6`, aborts with
`libc_impl.c:1135: wrapper_ecvt: Assertion 0 failed`. The wrapper is an
unimplemented stub, not evidence of allocator corruption. tools/ido-ecvt.patch
implements the guest string and decimal-point/sign outputs. The fixture now
compiles and its listing includes `assigned (unconstrained) 2`. Diagnostics use
the existing `-zdbug:6 -l <listing>` options; ordinary compilation is unchanged.
tools/uopt_patch.py's `trace` variant additionally prints the procedure name,
pool bounds, live-unit counts/flags, computed priority and minimum colour cost.
Those extra diagnostics belong only to the experimental binary.

## Initial baseline

A fresh configure/ninja build of this checkout passes 27/27 SHA1 checks.
Detailed log: scratchpad/r9-uopt/baseline-build.log. tools/uopt_eval.py is the
separate evaluation harness; no allocator change is enabled in the build.
Baseline revision: b1e5c6df17f1bc24e3dd32b7f0acd09ce50ed524. The control sweep
reproduces 4,870/4,870 matched C functions. 4,869 use IDO; TAIIKU
`func_801488F0` uses the configured PsyQ GCC compiler. Treating it as IDO is
a harness error, not an allocator regression; the corrected sweep recompiles
it with config/toolchains.txt.

Current inventory: 1,189 historical rows, 737 distinct functions, 406 already
C and 331 open. This supersedes the wave-7 snapshot of 1,088 rows. The draft
collector found 11,659 distinct bodies for the 331 open functions in wave
scratch and the older consolidated draft collection. It selects the lowest
recorded diff scores plus alternatives from other waves for fresh compilation;
old scores and failed source drafts are not treated as compiler evidence.

## Priority formula

Cross-checked `compute_save`, `cupcosts`, `isconstrained` and `globalcolor`
against the pinned generated recomp, not just the readable reconstruction.
The following applies to the project's per-procedure O2 path without profile
feedback. Arithmetic in these routines is single-precision floating point.

For each live unit (a live range's references in one basic block), let `F` be
the estimated block frequency, `L` its load count, `S` its store count and
`m = movcostused` (default 1). Its saving starts as `(L + S) * F`.

- Subtract `m * F` when `needreglod` is set, unless the block is a loop first
  block and the reload can move to predecessors. The movability check needs
  an in-range predecessor and excludes an outside computed-jump predecessor.
- Subtract `m * F` when the range has a store, is live on exit, needs a save,
  and this unit either stores or does not need an incoming reload.
- Sum the savings. Count the live units plus the reaching blocks (blocks
  traversed without a reference). Compress that count `N` to
  `D = N` for `N <= 2`, otherwise `D = floor((N - 2)/4) + 2`.
- Priority is the total saving divided by `D`. Double-word real (`Qdt`)
  ranges double the result. `priority > 0` is the eligibility test; zero is
  classified with negative savings.

No-feedback block frequencies use powers of ten by loop depth, via
[uoptloop.c](https://github.com/n64decomp/ido/blob/d068e439f52615763a3facd6944873899ebad2fd/src/uopt/uoptloop.c).
Frequency and reload/store flags matter as much as textual reference count.

The selected register adds a separate cost: save/reload around affected calls,
weighted by block frequency, and a `m * F / 10` penalty when it disagrees with
a live unit's existing precolour. An impossible parameter/return-register
choice costs `1e20`. A previously unused callee-saved register additionally
costs `clamp(0.25 * m * number_of_basic_blocks, 4, 60)` for its first use.
Allocation requires `priority * D > minimum_register_cost`, as well as the
positive-priority eligibility flag. Thus changing only the first comparison
from `>` to `>=` cannot make a zero-profit range allocate at zero cost: both
gates must change. The zero-threshold experiment changes both.

## Range order and register tie-breaks

All ranges in a non-leaf procedure are constrained. A leaf range is constrained
when its interference count reaches the register-class size. Constrained
ranges are handled first: scan bit positions in ascending order, choose the
highest eligible priority, and retain the first bit position on equal
priorities (strict `>`). Splitting can add ranges and update interference.

Unconstrained ranges are then handled in ascending bit-position order,
without sorting by priority. This explains why a priority-weight hypothesis
and an ordering hypothesis are different experiments. Bit positions are
optimizer identities, not C declaration indices; expression formation and
copy propagation can change them before colouring.

For either group, try un-forbidden caller-saved colours in ascending order,
then callee-saved colours in ascending order. Choose the lowest cost and keep
the first register on equal cost (strict `<`). The all-caller-save path has
an additional equal-cost preference for a register already used by the
procedure. Precolouring and forbidden masks can remove the apparent first
choice before this search.

Integer colour numbers, **not hardware register numbers**:

| colours | physical registers |
|---|---|
| 1, 2 | v0, v1 |
| 3..6 | a0..a3 |
| 7..12 | t0..t5 |
| 13 | ra used as caller-saved colour |
| 14..21 | s0..s7 |
| 22, 23 | fp, ra as saved colours |

A leaf normally searches caller colours 1..12 (1..11 if fp is reserved),
and saved colours through 23. A non-leaf includes caller colour 13 and
searches saved colours through 22 (21 if fp is reserved). The actual
forbidden sets still apply. ugen allocates scratch temporaries such as t6;
it is a separate allocator, so changing these colours is not a general
temporary-numbering fix.

## Why -regr is non-monotonic

`-regr N` sets `usingregoption` **and disables precolouring**. It is not a
clean experiment that varies one pool size. In the generated `procinit`:

- Exactly 9 takes a special branch forbidding a0..a3 while leaving the other
  ordinary colours available.
- Other values first forbid caller colours 1..13. A positive value permits
  the first `min(N,6)` t0..t5 colours; 7 also permits v1; 8 permits v1 and a3.
- Values above 9 therefore return to the six t registers. Zero permits none
  of those caller colours. Saved colours have their own option and masks.

The readable reconstruction's `uoptinit.c` assignment to `dftregsused` omits
the complement/intersection present at generated labels L43cb04..L43cb34;
using it literally would give the wrong explanation. The special-case-9 path
also makes the later `N == 9` test unreachable. This matches the previously
observed t0 / v1 / v0 / t0 sequence without requiring an unstable allocator.

## Candidate definitions and results

tools/uopt_patch.py verifies the SHA256 of the pristine generated C and exact
replacement counts. Each output is a separate native binary. No variant
matches function names, data addresses in the game, source markers or flags.
Addresses in the patch refer to compiler internals of the pinned executable.

| candidate | uniform change |
|---|---|
| zero_threshold | admit zero priority and equality with the minimum register cost |
| free_entry_load | omit the live-unit reload charge at the procedure entry block |
| half_move_cost | halve live-unit reload and save charges; keep call costs unchanged |
| late_priority_tie | choose the later bit position on equal constrained priority |
| late_constant_tie | choose the later equal-priority range only when both ichains are integer constants (type 2) |
| reverse_unconstrained | visit unconstrained bit positions in descending order |
| prefer_v1 | rank v1 before v0 when colour costs tie |
| prefer_a2a3 | rank a2/a3 before a0/a1 when colour costs tie |

**Decision: adopt none.** Every candidate changes already-matched code. The
best preservation result is `late_constant_tie`, but it still changes 661
literal linked function spans. No allocator patch enters tools/Dockerfile or
the default build. The diagnostic repair and fail-closed experiment tools are
retained for further compiler research.
These results reject the tested rules with the current C corpus; they do not
prove that the original allocator was identical or that no other uniform model
can work. Source types, expression formation and existing matching idioms can
change the graph presented to the allocator.

All preservation counts below have denominator **4,870**. "Pre-link" compares
resolved instructions; "linked" requires identical bytes, sizes, addresses and
padding. New cases exclude the eight stock-uopt draft hits. The case corpus is
held fixed for every candidate.

| candidate | pre-link identical | linked identical | linked unavailable | unit hashes / 27 | new functions / historical rows |
|---|---:|---:|---:|---:|---:|
| stock control | 4,870 | 4,870 | 0 | 27 | baseline |
| zero_threshold | 954 | 11 | 2,205 | 0 | 0 / 0 |
| free_entry_load | 2,023 | 76 | 2,205 | 0 | 0 / 0 |
| half_move_cost | 953 | 11 | 2,205 | 0 | 0 / 0 |
| late_priority_tie | 4,566 | 3,146 | 0 | 4 | 1 / 3 |
| reverse_unconstrained | 4,703 | 3,623 | 0 | 13 | 0 / 0 |
| prefer_v1 | 3,371 | 2,229 | 0 | 0 | 0 / 0 |
| prefer_a2a3 | 4,570 | 3,653 | 0 | 5 | 2 / 5 |
| late_constant_tie | 4,793 | 4,209 | 0 | 9 | 1 / 3 |

The first three candidates each have 317 functions in objects rejected by
asm-processor after placeholder sizes change. Their affected units contain
2,205 matched functions, all counted as unavailable in the linked gate.
Unavailability is a failure for adoption, not a claim about those functions'
bytes. Other candidates have no compile/link failures. Code-size changes can
move later functions and change relocations, explaining the difference between
pre-link and linked counts. Candidate case sweeps have zero compile/comparison
errors. Rechecking all seven initial case sweeps with the final comparator
changes zero results.

Machine-readable counts, artifact/tool hashes, coverage and verified case hits:
[data/t9800-allocator-scores.json](data/t9800-allocator-scores.json).
Detailed logs and objects remain in scratchpad/r9-uopt. `prefer_a2a3` gains TT
`func_80134ADC` (224 bytes, three rows) and `func_80145370` (360 bytes, two rows).
Both tie variants gain TT `func_801475C8` (124 bytes, three rows). All three
distinct gains were linked at their original address with external symbols
bound to original addresses and compared literally. This is an isolated
function proof, not a passing whole-unit build with the allocator enabled.
The candidates also lose stock case hits (respectively 6, 4, 6, 2, 3, 7, 4, 1
in table order, excluding the stock row). No game C is changed or newly counted
as matching in project progress.

The broad late-priority tie change gains TT `func_801475C8` (three historical
rows). Linking the isolated function at its original address with external
symbols bound to original addresses confirms all 124 bytes. Its constants
0x40 and 6 are bits 21/22, both priority 5 over denominator 2, both saved-register
cost 4; the ordinary tie assigns s2/s3 in the wrong order. The broad change
preserves only 3,146/4,870 functions after linking. This motivates the eighth,
narrower `late_constant_tie` hypothesis: reverse only ties between integer
constant ichains, identified by the compiler's expression type, without game
names or addresses. It preserves 4,793/4,870 pre-link instruction spans and
4,209/4,870 literal linked function spans, so it also fails the adoption gate.
Matched `tpage_buf_clear` is a direct counterexample: its bits 15/16 both have
priority 5 over denominator 2, and the original matches the earlier-first order
(a0/a1, cost zero). The TT gain alone does not establish a general tie rule.

The frozen corpus is 214 best compiling wave drafts plus 121 fresh m2c drafts:
335 variants covering 227 open functions / 432 historical rows. All 331 m2c
generations succeeded; 210 outputs fail compilation (unknown/incompatible
types, unresolved fields or non-C placeholders). The wave preparation attempted
1,182 bodies, 77 declaration repairs, 145 type-context repairs, 319 additional
extern repairs and four earlier r7-regs bodies. Repairs restore recorded
declarations, never invent unknown field types. The 104 uncovered functions
have no compiling recovered draft and are not negative allocator evidence.
Source hashes in scratchpad/r9-uopt/frozen-manifest.json hold the corpus fixed
across candidates. Fresh m2c is scored even where a better wave draft exists.
None of the 121 compiling fresh m2c drafts becomes a match under any candidate.

Eight wave drafts already have identical resolved instructions with stock uopt:
TT `func_8013923C`, RPG_BAT `func_8013F350`, DATE `func_80149268`, main
`func_80079B34`, `func_80074DE0`, `func_80048DD0`, GEKO `func_80132A4C`, TEL
`func_80139E3C` (19 historical rows). They are source candidates, not allocator
gains or landed matches. Some inherit experimental/FAKE idioms. No game C is
changed; source review and full-unit checks would be required to land them.

| historical shape | compiling functions | open functions |
|---|---:|---:|
| other register/promotion | 38 | 58 |
| constant allocation/sharing | 55 | 70 |
| scheduling/order | 26 | 33 |
| temporary numbering/order | 14 | 23 |
| pointer/base/CSE | 23 | 46 |
| selector/compare chain | 52 | 58 |
| frame/spill | 10 | 19 |
| fade/girl | 3 | 3 |
| multiply/stride | 4 | 15 |
| DATE repeated products | 1 | 4 |
| 0x800/0x40 constants | 1 | 2 |

Scoring compares funcdiff's resolved instruction words and relocation identities,
without masking register bits. Full matched objects retain their normal layout.
Isolated cases use ELF function sizes plus the original trailing words, excluding
object-end alignment zeros; INCLUDE_ASM's ELF size already includes its trailing
words. This correction is tested with synthetic objects and all case results
were rescored. Jump-table references retain funcdiff's symbolic normalization;
therefore a case hit is provisional until its whole unit checksum passes. This
is a necessary gate, not a substitute for the final 27-unit adoption check.

tools/uopt_link_eval.py adds the stricter check: copy the normal linker scripts,
substitute the candidate object paths, link complete units, and compare literal
function bytes, sizes, addresses and original padding. It also compares all
whole-unit SHA1s, covering jump tables and data. If any object in a unit failed
compilation, the entire unit is unavailable; no stock object is substituted.
This distinguishes resolved pre-link equality from byte identity after linking.
The control relink reproduces 27/27 unit hashes. The reader accounts for GNU
nm sign-extending ELF32 MIPS addresses and never rewrites its input ELF files.
Synthetic tests cover both behaviors.

## Trace evidence and C-level implications

The first trace set contains nine blocked drafts and eight matched controls.
All 17 compile; the eight controls remain exact. Files are
scratchpad/r9-uopt/trace-manifest.json and traces/{index}.o.trace (native
listing) / {index}.log (extra instrumentation). These are private scratch
artifacts, not repository content. Matched controls: main `func_8005AE60`,
`func_8005E7F0`, `bustup_wink`, `func_80067DD4`; DATE `func_80154D38`;
ENDING `func_80138278`; EVENT `func_8010AC40`; GYOZI `func_8013AE80`.

- **Temporary/order, TT `func_8013923C`:** leaf ranges with bit positions
  0, 8, 10, 16 have priorities 3, 1.5, 3, 1.5 and receive colours 1, 2, 3,
  1 respectively. Priority 1.5 preceding 3 directly confirms the unsorted
  unconstrained traversal. Another range has one load and an entry reload,
  net zero, and is not allocated. Source work should inspect which expression
  becomes a range and its identity, before treating the later t-register
  rotation as a global pool permutation.
- **Pointer/base/CSE, TT `func_8014C5F8`:** the heavily used pointer range
  (bit 3) has priority 8 and wins colour 3. Bit 18 initially has priority 1
  with denominator 2, but its cheapest colour costs 2: equality fails the
  strict profitability test. Splitting produces a negative range and a
  new range with priority 0.5, denominator 2 and cost 0, assigned colour 2.
  This is a concrete call/lifetime effect. Pointer reuse across a call can
  change the whole sequence even when the loop itself has the same C.
- **Fade/sharing, main `normal_date_girl_suddenin`:** seven store-only
  ranges have one store and one required save, net zero. Bit 12 has a load
  plus store minus save, priority 1, and receives v0. The missing sharing is
  not explained merely by exchanging the first two available colours; the
  input has a different set of profitable ranges. Compare chain assignments
  and scalar versus indirect store structure before trying allocator weights.
- **Narrow flag stores, matched `bustup_wink` versus `func_80067DD4`:** both
  have an unprofitable global range (load minus reload, store minus save,
  priority zero). Their expression/local range is profitable: 1.5 in the
  former, 2.5 in the latter, both assigned v0. The successful narrow-local
  source idiom in T-9550 does not require the global itself to be promoted.
  Therefore the old inference “final andi means a promoted global” is too
  strong; trace the expression as well as the variable.
- **Selectors, matched GYOZI `func_8013AE80`:** the sole candidate has one
  load and one reload, net zero, and globalcolor assigns no register to it.
  The v0 selector can be produced by the downstream local/temporary path.
  A selector's physical register alone is not proof of global promotion.
- **Constants, matched DATE `func_80154D38`:** its store ranges also have
  zero net saving. The original already matches this unpromoted shape. A
  blanket zero-profit promotion rule cannot distinguish it from the fade
  draft above. EVENT's matched chain-assignment control likewise has many
  zero-profit globals while its source structure supplies sharing.

The second trace set adds twelve subjects (more-traces/) and four earlier
DATE/TACO variants (r7-traces/). Every subject compiles. A historical row's
cluster describes the old blocker; a recovered draft can have a different
remaining difference.

Targeted gain traces add TT `func_801475C8`, `func_80134ADC`, `func_80145370`
(gain-trace/ and a2-gain-traces/). Four matched counterexamples are traced in
counter-traces/: `search_load_tpage_buf_lock`, `search_tpage_multi`,
`tpage_buf_clear`, `func_8005E454`. They all remain exact with instrumentation.
The complete set is **40 compilations / 38 distinct functions**, with twelve
matched controls; manifests and native listings remain in scratch.

- **0x800/0x40 constants, TACO `func_8014B8B4`:** bits 87 and 88 each have
  seven loads minus one reload, priority 6, in the store block. Both cost zero;
  constrained allocation takes bit 87 first (v0), then 88 (v1). Bit 89 has six
  loads minus reload, priority 5, and receives a0. This is an actual equal-
  priority ordering case, unlike the zero-profit scalar-store examples. The
  loop counters have priority 31 and use saved registers earlier. A local
  constant's textual declaration order does not necessarily change these
  optimizer bit positions; trace them after copy propagation. The draft also
  retains a scheduling difference for the later 0x80 constant.
- **DATE repeated products, `func_801366A4`:** the `(i+i)*3` draft has two
  address ranges with priority 4, each with three loads and one store, assigned
  v0/v1. The original repeats six index multiplications; the draft combines
  shared address computations before colouring. Changing colour order does
  not identify why the original retained those expressions. Investigate CSE,
  strength reduction and the alias/type model, not just the register pool.
- **Multiply/stride, SHUGAKU `func_80134658`:** the draft already retains both
  stride multiplications. The address range has priority 6 and takes v0;
  two reused expressions each have priority 1 and take v1/a0. The two stores
  of 8 have zero-profit destination ranges and emit separate constants,
  whereas the original shares a v1 constant. This combines expression sharing,
  global colouring and downstream temporary numbering. Explicit plausible
  shared values/aggregate accesses deserve separate source probes.
- **Frame/spill, DATE `func_8013E6A4`:** the saved call result has priority 1
  over denominator 2, versus cost 2 across `normal_date_speak`; equality fails
  and splitting produces zero-profit pieces. The recovered narrow-local draft
  already has the expected frame behavior, leaving counter-expression register
  choices. Distinguish a lifetime spill from aggregate/local stack layout;
  GEKO's recovered table-copy draft matches instructions with stock uopt.
- **Scheduling/other, main `func_80042808`, `func_8004284C`, `func_80042908`:**
  the remaining actual instruction difference is the original's `move v0,zero`.
  Global loads/stores have zero net saving, and the parameter in the third
  procedure keeps a0. A pool permutation cannot supply a missing return-value
  operation. Check return type/return expression and callers, as required by
  the T-9000 controls; do not infer an allocator defect from the historical tag.
- **Selector controls:** the recovered `func_80079B34` and `func_80074DE0`
  drafts match their instruction spans after excluding object alignment. The
  former allocates the loop selector (priority 26.67) to v1 and the argument
  value (13.67) to a0; six loop constants take s0..s5 at cost 8 each. The latter
  has an expression/local range in v0 and a global range in v1. Selector
  registers depend on which range represents the selector, not one universal
  v0/v1 preference.

Nine synthetic tests pass in Docker, including the actual patched ecvt guest
ABI (negative values, decimal point, byte-swapped string, buffer reuse/growth)
and a real native trace compile whose instructions equal stock uopt. Command:
`tools/docker.sh env TMPDIR=/scratch/r9-uopt/tmp UOPT_TRACE_BINARY=/scratch/r9-uopt/variants/trace UOPT_RECOMP_SOURCE=/scratch/r9-uopt/recomp/libc_impl.c python3 tools/test_uopt_tools.py`
(set TOKIMEMO_SCRATCH first).

## Final verification and review

Final Docker configure, ninja clean and rebuild pass **27/27 SHA1 checks**.
The nine synthetic tests pass, the prototype branch guard reports zero new
and 17 known disagreements, and shell syntax/diff whitespace checks pass.
Wiki ticket/board links, all eight score records, tool hashes and isolated gain
proofs are checked. Proof logs: scratchpad/r9-uopt/final-clean-build.log and
final-checks.log. Inline standards/spec review is recorded in the ticket;
all findings are resolved. No commits, stashes or branches were created.

## regdiff: the trace as a diagnostic tool (T-10400)

`tools/regdiff.py` ([[decompile-workflow]], section "regdiff") puts the T-9800 trace to work for a single draft. It
compiles the full translation unit through `tools/cc.py` (T-10840: retain file-scope data and helpers) with the traced uopt, links each
register difference to a live range of the trace and prints the range's priority, order and colour against the
competing range, with a hint from the rules above. Nothing about the traced uopt enters the repository; the default
path is `/scratch/r9-uopt/variants/trace`, built with the recipe at the top of this page, and it runs in the normal image.

How an instruction is linked to a range: the draft's code is split into register webs (reaching definitions per
register, a call clobbers the caller-saved registers after its delay slot, `sw`/`lw` of the same stack slot join
the pieces of a range saved across a call, `lui`+`addiu` on one register is one value). Webs and trace ranges of the
same hardware register are paired by the def/use counts of the trace and by program order; the report says when
the register has only one range, when the counts agree and when it is a guess. Variable names come from the
ucode (`loc` lines of each `lod`/`str` against the identifiers on those lines of the draft); a hoisted constant is
named by the constant its web builds. Registers `$at` and t6..t9 are ugen's, not uopt colours, and are reported as such.

### Rules the tool's cases made visible

- **Hoisted temporaries are numbered in the order the code first uses them.** Bits past the expression table (hoisted
  constants and addresses, e.g. bits 32..39) follow the first use in the loop body; at equal priority the lower bit is
  coloured first and gets the lower register. DATE `func_80136F2C` (bits 32 `D_800CA22C`, 33 `D_800CA224`, 34 `1`, 35 `4`,
  36 `3`, 37 `D_800CA234`, 38 `-1`, 39 `2`) and EVENT `func_80114B0C` (68 `D_8012439C`, 69 `D_80124394`, 70 `1`, 71 `4`,
  72 `3`, 73 `D_801243A4`, 74 `-1`, 75 `2`) show the same sequence: source array, destination array, the constants in the
  order of the tests, the third array, the constants of its tests.
- **The address of the left side of an assignment is used before its right side** (`a[i] = b[i]` pushes a's address
  first). `v = b[i]; a[i] = v;` therefore numbers b first. Dropping the temporary (the later `v - 1` reads `b[i]` again, which
  uopt shares) swapped the two base registers and matched DATE `func_80136F2C`, after 28 hand variants and a 300 s
  permuter run had failed (tie of two priority 3.333 ranges, bits 32 and 33).
- **A swapped `bne` with equal registers is operand order**, not allocation. uopt puts a plain variable first, and a
  narrow global is no plain variable after the cvt pass. A local copy of the global (`u8 sel; sel = D_8012531C; if (sel == 1)`)
  puts the register first like the original and matched EVENT `func_80119518` (a `u32` copy matched too, `s32` did not).
  Writing the operands of the `==` the other way round (`1 == D`, `D == sel`) did not change the order in these tests.
- Unconstrained ranges are coloured in bit order even when a later bit has the higher priority; the tool prints the
  note when it matters.

### Validation

Draft sources are the T-9800 draft corpus and the guarded drafts of T-10330; the original side is the unit's
all-`INCLUDE_ASM` object (funcdiff). "Before" rows are the older drafts, run with `--src`.

| case | what the draft gets | what regdiff says | outcome |
|---|---|---|---|
| EVENT `func_80119518` (draft 78, T-0018 row) | `bne v1,v0` against `bne v0,v1`, nothing else | operand order only; local-copy hint | `u8 sel` copy of `D_8012531C` matches; landed |
| DATE `func_80136F2C` (draft 162) | base registers t0/t1 swapped, 4 lines | tie 3.333/3.333, bits 32/33, first-use order, hoisted sequence | no temporary `v`: matches; landed |
| EVENT `func_80114B0C` (guarded) | t1/t2 bases swapped plus a `bne` order | tie 2.5/2.5, bits 68/69 | not matched; draft cut from 10 to 5 differing lines with `*p = v = *r` |
| TT `func_801475C8` (draft 132) | s2/s3 swapped (`0x40`, `6`) | tie 5/5, bits 21/22, saved-register cost 4 | not matched (`while`, `do`, down-counting loops tried) |
| GYOZI `func_8013E638` (guarded) | i in v0, element pointer v1 | i 25.5 outranks pointer 20, 5 loop references against 4 | not matched (scalar `D_800F62CF` view changes the structure) |
| ETC `func_80143F24` (guarded) | global in v1, original a1 | global 15 outranks the loop counter expression | not matched |
| TT `func_8014A814` (draft 137) | five constants in other registers | constant table lists each value, draft register, original register | not matched |
| DATE `func_801511B4` (draft 221) | `lui v0` for `D_800B593C`, original v1 | original keeps another value in v0 across the block | not matched |
| main `MouseState` (draft 8) | locals `first`/`result` tie 1.5/1.5, unconstrained, bits 7/11 | tie, first mention order | repository version has no `first` and matches |
| control EVENT `func_8010AC40`, GYOZI `func_8013AE80`, main `MouseState` | exact | `MATCH`, no divergence | as expected |

Two more of the old drafts, DATE `func_8013E3C4` and SHUGAKU `func_801322F0`, are pure operand-order differences, and
the tool reports them as such; both are matched in the repository today with a local copy (a narrow `u16` plus cast, FAKE
T-10020, and a `GameState *` local), the same remedy.

Survey of the whole best-draft manifest of T-9800 (163 drafts): all 163 drafts ran without a failure (traced compile 2.9 s on average, analysis 0.2 s, at load average 20 to 55): 5 identical, 4 operand order only, 25 register differences only, 127 with other instruction differences as well (14 of them without any register difference), 2 with nothing found. Of the 138 first register divergences, 49 are ugen temporaries (t6..t9 and $at), 46 priority order, 19 an extra or missing live range, 14 without a matching range and 10 ties.

Historical stripped-function timing (T-10400; T-10840 now retains the full file): the traced compile takes 1.5 to 2 s on an idle machine, the whole call about
3 s inside a running container (cached original object) and 6 to 12 s from the shell at load average 20 (container start,
imports, symbol lookup). `regdiff.run` can be called repeatedly from one container to try variants.

Limits: the tool explains one decision at a time and is only as good as the link from instruction to range; it does not
know the original's C. Splitting after a failed colour check is reported as a piece of the parent range. ugen's t6..t9 and
`$at` are shown as a sequence only. Rules the tool cannot yet turn into a C change (TT `func_801475C8`, GYOZI
`func_8013E638`) stay open.

## Selector-copy rule and ugen survey (T-16900)

Two time-boxed experiments; numbers and lists in [[tickets/T-16900-compiler-experiments]].

**Signed narrow selector-copy rule.** The 7 NAME_ENT selector-copy drafts of [[tickets/T-11570-w1-ovl-name_ent-80139740]]
were frozen and compiled with a scratch variant of `tools/cvt_pass.py`. Copy preservation (zcopy off) leaves 4 lines in
three of them, all `beq v1,a1` where the original has `beq a1,v1`. A widening record does not produce that order: uopt
folds `CVT J<-J`, and `CVT J<-L` gives the load a temporary of its own and makes most drafts worse. Putting the constant
first after uopt does produce it, if a register operand is classified by its latest definition (the pass's
`_constant_first` gives up when the constant's register also passes a call argument). Copy preservation plus constant
first matches `func_8013DAC4`, `func_8013F454` and `func_8013FB8C` (NAME_ENT.bin sha1 OK with the drafts active).
Applied to every procedure with a narrow signed global copied into a local, zcopy off changes 12 matched functions;
constant first against every narrow signed global changes 5. The coupled form (both, only where the procedure copies
global G into a local and compares G with a constant) changes none: clean rebuild 27/27 sha1. It also fires in no
matched function, so that result shows the rule is harmless to the matched set, not that it is right. It meets the
experiment's bar (3 of 7, 0 changed). Before a uniform pass, the coupled shape needs a census in the original asm with
its exceptions (CODING_STANDARDS 7a).

**ugen survey.** regdiff over the 262 unmatched T-14500 drafts that are 21 to 100 lines from matching (no TT):
250 first register divergences, 59 of them in a ugen temporary on the draft side (23.6%, against 49 of 138 in the
T-10400 sample). Only 23 (9.2%) have ugen temporaries on both sides, the case where ugen's own order decides; in the
rest one side holds the value in a uopt colour, which the existing trace covers. Below the 25% bar: no 3-day ugen
trace.
