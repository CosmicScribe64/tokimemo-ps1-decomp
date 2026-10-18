---
id: T-24047
title: Recover ENDING sprite scene initializer
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[ending-family-results]]", "[[completion-run-2026-10-17]]"]
---

## Goal

Recover func_80135F4C in src/ovl/ENDING/80134D20.c as byte-identical default C.
Own only that TU. Do not edit the parked 8013A2E8 function or other assigned files.

## Acceptance criteria

- Actual unguarded default C, literal linked proof, affected binary SHA1.
- Bounded novel experiments based on recovered types/control flow.
- Review against CODING_STANDARDS.md; justified FAKE forms marked.
- Useful negative evidence and drafts retained without coverage claims.

## Notes

Pin 4496b0dc785dd6d109a39c4f3408e70d17079232, branch codex/complete-ending-large,
worktree /private/tmp/tokimemo-ending-large. Docker only, one compiler total.
Header changes require coordination. Earlier census has 795 residual rows and
known menu-byte GameState pointer issues; read T-18305 and T-20930 before probes.

## Comments

2026-10-18: Read current wiki/index, completion run, coding standards and ENDING
family findings. Original checkout remains read-only. Old MAIN/EVENT/TEL ownership revoked.

2026-10-18: Archived full-TU replay has208-byte frame (target160) and broad residual. Fresh hand source reuses one signed-halfword index, typed Rec24/DisplayActor44 fields, direct menu-byte array accesses and direct random-table assignment. It restores805/805 instructions with176-byte frame. New evidence motivates a shared size128 local across transfer calls and variable grid divisors; type/lifetime probes follow.

2026-10-18: Bitfield b14 (rather than rawword&0x4000) restores original sign-bit branch and switch register cycle; aligned full-TU cost340 to114. Direct record indexing removes two artificial pointer locals. With scalar i/size before stack tables, original160-byte frame and both table homes0x7C/0x60 match exactly; cost104,803/805 instructions. Divisor locals were unnecessary: literals are naturally hoisted, so removed. Register qualifiers and coordinate-line grouping do not improve output. No production header/toolchain changes.

2026-10-18: Unsigned16 width lvalue view with u16 shared size removes duplicate128 register; packed i/size/row homes recover exact frame/table positions. Current803/805 instructions, aligned cost64. Row quotient materialized immediately after the third-loop byte clear restores the ten-store block. Local byte-view, register hints and whole first-loop grouping have no effect. Native AS1 diagnostic attempt was rejected: -xbbdbg,8 is not accepted by existing frame shim argument parser; -R-only diagnostic object lacks .data and fails trailing-pad validation. No diagnostic output interpreted, no toolchain changes retained.

2026-10-18 correction to diagnostic note: the -R-only compile itself succeeded; the fidelity script failed when it tried to look up an absent optional .data section. Both stock and traced objects legitimately omit .data. A scratch-only wrapper now appends native -R/-xbbdbg8 after the unchanged frame transformation, and verifies .text/.rel.text/.data/.rodata inventories and bytes equal stock. Native stdout capture yields26784 DAG lines. First-loop DAG has no dependency from menu sb to sprite stores, explaining its early issue. No production compiler flags or scripts changed. Moving actor Y assignment before fade byte restores entire second loop, cost64 to49,804/805 instructions.

2026-10-18: First-loop menu-address roundtrip `(u8 *)(u32)&member` restores exact store ordering without disturbing frame/base registers when restricted to that loop. This is an allocation/alias-metadata FAKE candidate, not recovered source. Current804/805 instructions, aligned cost43. Most residual rows are branch offsets cascading from two tail LUIs speculated across the switch. Third-loop128 and499 occupy exchanged a3/t0 colours. Later byte-view reads/stores, explicit common-tail label, implicit-int diagnostic and unsigned page view do not improve. Useful preferred full-TU and normalized diff retained under build/t24047/roundtrip_first_menu*.

## Checkpoint

Preferred guarded C is preserved in src/ovl/ENDING/80134D20.c and
`drafts/T-24047/body.c`, with a full-TU replay script and bounded probe ledger.
It remains804/805 instructions, aligned cost43; original160-byte frame and
both stack table homes agree. No new verified C bytes.

Remaining causes are isolated: four words exchange128/499 between a3/t0;
two default-arm script-address LUIs move into the switch dispatch, accounting
for the other differences and branch-offset cascade. Earlier references to
"tail LUIs" mean the default arm, not the common continuation. Adding explicit
case12 alongside default blocks this speculation, yielding805 instructions and
five differing words, but changes the upper bound12 to13 and adds a table entry.
That diagnostic is explicitly rejected as a matching candidate.

Halfword quotient/page locals add narrowing operations and do not close the
constant tie. Source default-first ordering worsens output; moving default body
outside the switch leaves the same residual. Probes have been bounded to source
recovery and the measured residuals. No shared headers, compiler options, tooling
rules, other TUs or parked ENDING8013A2E8 changed. The two alias/width devices are
FAKE-marked.

## Review and validation

Independent standards review found no actionable findings: the ticketed guard,
C89 source, shared types, explicit FAKE notes, replay failure handling and
negative-evidence labels comply with CODING_STANDARDS.md. Parent scope review
confirmed only the owned function changes in production; no headers, compiler
rules, toolchain flags or other TUs changed. The matching acceptance criterion
is not met, so this ticket remains In Review with zero new verified bytes.

Docker replay reproduces804/805 instructions and aligned cost43; the rejected
case12 diagnostic reproduces805/805 and cost10. Normal ENDING rebuild passes
its SHA1. Header/global checks and branch prototype check pass (zero new
disagreements,15 known). All27 built binary hashes pass after rebuilding stale
copied GEKO/GYOZI cache outputs. Those rebuilds required no source changes.
Ignored build/t24047/ retains verification logs; git diff --check is clean.

2026-10-18 continuation: reopen the same function for a small set of
evidence-driven source hypotheses. Preserve b0111332c as the reviewed checkpoint;
no new worker or other function. First examine default-arm address dependencies
and the grid-loop constant declaration lifetime.

Continuation probes: default-arm integer-address views produce807 instructions,
cost64; volatile loads803/cost52. Both are rejected. Named palette and signed
origin locals each retain804 instructions/cost55, including a frame expansion;
no allocation improvement. Moving the palette store to the start of the grid
loop is output-neutral (804/cost43); moving it after UV is much worse
(803/cost636). Production source stays at the reviewed checkpoint.

Explicit outer range control does not explain the switch: both normal-first
and default-first if/switch forms emit834 instructions (cost109 and369). The
compiler retains a second range test and changes selector lifetime across calls.
These are rejected; no control-flow changes retained.

Constant type probes (u16/u32 palette casts and unsigned palette view) are all
output-neutral804/cost43. A scratch-only default-arm declaration probe using
three distinct symbol names instead of the matrix is also804/cost43. Thus the
shared matrix's symbol metadata does not explain speculative default addresses;
no shared header change is warranted.

Register-qualified palette/origin locals also give804/cost55 with the same
frame expansion as ordinary locals. Fourteen continuation scores are retained
in drafts/T-24047/experiments.json. No closer source emerged; production C and
the preferred replay body are unchanged from b0111332c. Return to In Review
with the same two blockers and zero verified byte gain. Scope/standards review
of this follow-up finds no code changes or new findings; it records only
negative evidence. Previously passing production validation remains applicable.

2026-10-18 continued closure: inspect the existing native AS1 DAG and available
stock uopt listing support before another source probe. Native AS1 trace shows
the default LUI entering the selector's load stall during a later cross-block
scheduling pass; the first per-block schedule has the target's empty load slot.
No compiler binary or production options change.

Existing tools/uopt_eval.py stock listing diagnostic fails at the documented
libc_impl.c:1135 wrapper_ecvt stub. This is a tracing-runtime dependency, not a
source compile failure. Do not rebuild the tracer; use the existing AS1 evidence
for source control-flow probes. Diagnostic outputs stay ignored.

Packed halfword declaration swap is neutral804/cost43. Unreachable case -1 or
256 sharing default each emit805 instructions but disrupt whole-function frame
and allocation (cost770); they do not provide a usable default-entry barrier.
These diagnostic forms are rejected. Source-line grouping is the next measured
lever, supported by the existing assembler scheduling notes.

Correction to stock-listing blocker: the full-TU diagnostic crashes only in a
later function. Its partial listing contains complete35F4C global coloring and
reemission, including constrained ranges528/529 assigned colours6/7 (a3/t0).
Thus allocation for the two constants is observable without a tracer rebuild;
the whole diagnostic compile still fails and produces no accepted proof object.
Whole-switch and default-load source-line grouping both remain804/cost43.

UV/page-result reuse of the quotient local worsens output to802 instructions,
cost623/621. These do not change the source checkpoint. Truncating only later
functions still triggers ecvt during final statistics, after target reemission;
the earlier claim that only a later function triggers it is too narrow. Lower
native listing verbosity is being checked for an unchanged-code diagnostic.

Native listing level5 also reaches the ecvt assertion; no diagnostic object
is accepted. Narrow size declarations s16 and u8 are neutral804/cost43.
Matched neighbour func_80136EE4 has both selector slots occupied by the
prologue, so it offers no empty-stall counterexample to AS1 cross-block motion.
Private preprocessed-TU views of func_80132000 as implicit-int or unprototyped
void are also neutral804/cost43; shared declarations remain unchanged.

## Diagnostic prerequisite history

The allocator investigation initially needed the instrumented uopt from
[[uopt-register-allocation]] to expose priorities, costs and forbidden colours.
Stock listings expose assignments but not those inputs and abort at the ecvt
stub; regdiff confirms its traced binary is not available in this container.
The coordinating lane previously found no existing host trace binary and asked
that it not be rebuilt in this source lane. No compiler rebuild was attempted.
That diagnostic runtime was the dependency for further allocation work; it is
now repaired below and documented in [[ending-allocator-trace]].

Eleven additional source scores are archived, all negative. The production
source and preferred replay body remain exactly b0111332c. Scope/standards
review finds no new production changes or findings. Ticket returns to In Review
with unchanged804/805 instructions/cost43, zero new verified bytes, and all
previous normal-build validation still applicable.

2026-10-18 prerequisite repair: coordinating lane now authorizes a bounded
15-minute rebuild of only the documented instrumented uopt in external scratch.
Checkpoint4428f8789 preserves the preceding evidence. No reusable source found
in the existing research/acceleration trees or user cache. Use pinned revision,
source hash and existing ecvt/trace patches in Docker; verify stock output before
using diagnostics. No production compiler or headers will change.

Pinned reconstruction fetched successfully; generated5.3/uopt.c SHA256 exactly
matches b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f.
The existing ecvt patch applies with zero fuzz. Setup/generation uses56MB and
serial Docker build proceeds within the15-minute bound. This repairs the
diagnostic prerequisite only; all production compiler binaries remain unchanged.

Diagnostic prerequisite repaired in about3 minutes,62MB external scratch. The
patched baseline and instrumented allocator both reproduce stock .text,
.rel.text, .rodata and .rel.rodata bytes and section inventories exactly on the
complete preferred TU. Target resolved words also equal the804-instruction
checkpoint. Native trace now exits successfully. Use
/private/tmp/t24047-uopt/variants/trace mounted as /scratch/variants/trace;
no production toolchain changes. Detailed allocator diagnosis can continue.

regdiff confirms a true tie:499 range528 and128 range529 each have priority5,
cheapest cost0; ascending bit order chooses a3/t0. Separate lexical scopes do
not reuse CFE stack homes: an unused UV local alone expands frame160 to168
(804/cost55), and early computed UV worsens to802/cost639. Reusing a table
halfword produces805/cost632. A union-member table copy preserves the frame but
changes aligned stores to swl/swr (811/cost66); inspect whole-union copying
before using that alternate storage. No production changes retained.

Whole-union copy restores the exact checkpoint (804/cost43), but early UV
computation still worsens it (802/cost633); byte scratch worsens to811/cost668.
Moving the UV store immediately before palette is803/cost636. The new tracer
explains the coupled regression: constant first-use order does flip, but the
quotient loses a counted use, dropping priority15 to10 and allocation v1 to a0.
That shifts all grid constants one colour higher (128 t0,499 t1) and reserves
another temporary register across the function. A direct late quotient spelling
is the next narrowly targeted use-count probe, not a compiler tie-break change.

The direct late quotient probe improves804/cost37: it fixes all four constant
words while preserving the frame and temporary pool. Remaining loop difference
is one zero-halfword store issued early; default-arm speculative LUIs and
branch-offset cascade remain. Preserve as drafts/T-24047/closer.c, replay with
--body; production guarded body remains unchanged pending closure. Tracer recipe,
fidelity verifier and all91 source measurements are checkpointed for reuse.

After checkpoint7689bcca3, placing the zero-halfword store after the UV source
statement closes its scheduling gap:804/cost35, with all initialization loops
and scene bodies exact. Only default-arm cross-block LUI motion remains. The
rejected explicitcase12 diagnostic now has805 instructions/cost2, just the
wrong bound13 versus12 (and extra table entry), confirming isolation. Keep
that form diagnostic-only; zero verified bytes until the real table matches.

Register qualifiers and range-preserving s16 selector cast are neutral35.
Final-store volatile pointer view scores23 only through offset cancellation:
both LUIs still move and its body gains address instructions (806 total), so
it is rejected. Direct volatile script-global/selector-field declaration views
and an ordinary default C label are neutral35. Private assembler-input capture
preserves normal output exactly and shows the explicitcase12 form adds three
records: bound, table entry and default-entry alias. No assembler input is edited.

Current guarded production reconstruction and drafts/T-24047/body.c now use
the35-cost source. Only four C source lines move/change; the ASM fallback stays.
All initializer loops, constants and scene bodies agree. Normal default build
and reusable tracer fidelity are revalidated before the next checkpoint.

Validation complete: default replay804/cost35, reusable stock/patched/traced
code/data/relocation equality, and freshly rebuilt ENDING SHA1 all pass. Parent
standards/scope review finds no new findings: C89, existing shared types,
ticketed ASM guard and both FAKE explanations retained; no headers or production
toolchain changed. The improvement does not count as new matching C coverage.

Message-table and EdScriptSet pointer-type views are also neutral35. The
ledger now retains100 diagnostics, including explicitly rejected wrong-table
and extra-instruction forms. The remaining blocker is assembler cross-block
motion, not a script pointer-versus-integer declaration difference.

After source checkpoint e500e504e, assigning the selector to existing row or i
locals is neutral35. Read-only AS1 reconstruction reference identifies the LUI
opcode as unconditionally eligible for speculation; ordinary value types alone
cannot stop it. The candidate must be blocked by dependencies/control flow or
an earlier instruction-sequence difference. No assembler binary was built or
changed; reference source stays external in /private/tmp/t24047-uopt/.

The full-width bit-field view is also neutral35. A cached
tokimemo-decomp-ido52 image is available; T-3110 records the user's prior private
IDO5.2 evaluation approval. No compiler archives remain in the usual caches.
Use one bounded5.2 assembler replay of the captured, unchanged input to test
whether the last motion is backend-version-specific. Production settings remain
unchanged; no per-function compiler exception is proposed.

IDO5.2 archive matches the previously recorded SHA256
870cd946d31d6abee24d47926328d15909771520240a0a4f5cb7d27aea90e1ad.
Extracted37MB only under external scratch and reused cached amd64 evaluation
image. Replay uses captured post-frame input plus copied symbol sidecar; the
project mount is read-only. No host installs or production build changes.

IDO5.2 as1 replay is identical to stock53 for the complete target:804
instructions/cost35. Thus the assembler version alone does not close this gap.
Capture post-optimizer input for one isolated5.2 ugen replay next, keeping the
production front end, uniform passes and stock assembler fixed.

IDO5.2 ugen on the captured production optimizer output also reproduces stock
target words exactly (804/cost35), through the existing frame pass and stock53
as1. Older generator/assembler versions alone do not explain the missing barrier.
Check the front end next with a stock amd64/arm64 equality control first; reuse
the same approved archive and private image, no new toolchain installation.

The stock amd64/arm64 target control agrees exactly. Substituting only5.2 cfe
also gives identical804/cost35, with expected ucode3.18/3.19 warning accepted
by the stock optimizer. Only the optimizer remains to isolate before ruling
out a simple minor-version explanation; conversion/frame passes stay uniform.

Isolated5.2 uopt and the complete5.2 front-end/optimizer/generator/assembler
path, with only the existing uniform conversion/frame passes, also reproduce
stock target words exactly:804/cost35. A simple3.18 versus3.19 explanation is
ruled out for this source. Return to source-level CFG entry metadata; test a
referenced forward C default label (earlier outside-switch control fell through
without such a label). No production compiler change or exception.

Referenced forward default label is neutral35. A diagnostic empty inner switch
adds code and disrupts allocation (807/cost756), so is rejected. The source
ledger now has105 measurements. Five backend controls and exact provenance are
retained in drafts/T-24047/backend-evidence.json and [[ending-allocator-trace]].
Scope/standards review finds no production changes or new findings in this
evidence-only follow-up. Keep task In Progress on the same sole AS1 blocker.

After checkpoint d6c4f72b3, default compound-block and do-once wrappers are
both neutral35. The existing VALEN34030 note in [[matching-notes]] describes
the same unknown second-entry/bound problem; do not repeat its outlier-case
or duplicate-range experiments. Test only a call-argument sequence difference
next: equivalent ordered comma assignments can change pre-scheduling order
without new storage, unlike declaration views already shown neutral.

Comma-store sequencing inside arg0/arg3 expands the frame to168 and leaves
the same motion (804/cost47 and51); both rejected. One late false-backedge
control now tests the known unsigned-load conversion boundary: `(s32)u8 < 0`
cannot execute in valid C, but the optimizer's temporary signed load model may
retain its CFG edge until the existing post-pass restores LBU. Put it after
all real calls to avoid changing earlier temporary allocation. No claim or
production adoption without literal default proof.

Late false-backedge diagnostic is rejected at802 instructions/cost723: frame136,
large allocation changes, and the LBU/BLTZ pair survives into final output.
AS1 does not eliminate this unsigned-impossible branch, so it cannot supply a
temporary extra entry without extra instructions. The ledger now has110 source
measurements. Compound-block/do-once controls remain804/cost35; comma-store
arg0/arg3 controls are804/cost47 and51. Production stays guarded at35, zero
new verified C bytes. Evidence-only scope/standards review finds no new issue;
ticket and board remain In Progress.

Fresh native AS1 traces of captured default/case12 inputs each reproduce their
stock target words exactly. Both assign weight1 to dispatch header and4 to the
two successor blocks. Normal form numbers default37, dispatch38; explicitcase12
numbers dispatch37, default38. Native motion is two default LUIs versus the
dispatch SLL, respectively. Thus this is not a difference in these block weights.
The read-only reference identifies bit5 as a candidate skip flag and internal
label kind25 as one setter (internally created labels are26), but its involvement
in this pair is not proven. Logs remain ignored under build/t24047/switch-metadata/.
Test a default block after the returning common tail, with ordinary gotos, to
vary CFG traversal order without changing the case set or table bound.

After-tail default/back-goto is rejected at806/cost73: common tail remains before
default, adds branches, and both LUIs still move. Three single-access volatile
controls now isolate the default entry (first load, second load, first store),
since the previous all-load and final-store controls also changed later ordering.
These are diagnostic only; no production qualifier change.

Individual first-load/second-load/first-store volatile controls give805
instructions at costs54/50/54. Each still imports default address work and
changes the address temporary sequence, so all are rejected. Ledger now114;
native-switch-evidence.json retains capture hashes, equal local weights and
observed motion without claiming label-flag causality. [[ending-allocator-trace]]
documents exact debug arguments and the unproven flag boundary. Scope/standards
review finds no new issue; no production source, headers or toolchain changed.
