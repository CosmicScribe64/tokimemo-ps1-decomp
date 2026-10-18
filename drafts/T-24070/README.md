# T-24070 save-card state checkpoint

Only ETC func_80144F74 (608 original bytes), src/ovl/ETC/80143240.c, is
assigned. Production remains INCLUDE_ASM. The current source checkpoint has
39 resolved +/- rows, not a C match or verified-byte gain. Every neighbor,
including the archived T-24055 helpers, remains untouched. All work uses
the existing Docker image and one compiler; no header/config/compiler edits.

Matched NAME_ENT func_8013F758 supplies the save-card status family as a
read-only reference. No NAME_ENT source was edited. The target has its own
two capped byte-array updates and unsigned load of D_800E8BEE in case2.
Current GameState field accesses replace old scalar aliases faithfully.

Historical T-20900 plain46 was measured with scalar aliases and the unified
work struct. It is not the fresh current-type score. Baseline-body.c here
gives117; a consistent GameState pointer gives47, making cases1–3 exact.
Direct cap/increment expressions remove redundant byte-capture locals and
give39. The second cap now has expected a0/call-argument scheduling. First
cap index/address/counter registers and the initial masked-index copy remain.

| Control | Rows | Outcome |
| --- | ---: | --- |
| current-type baseline |117| baseline-body.c |
| consistent GameState pointer |47| state-pointer.c |
| shared byte counter |49| rejected |
| same shared counter as word |49| neutral, no width sweep |
| explicit first element pointer |47| neutral |
| bounded byte-sized index |111| rejected, added narrowing operation |
| raw selector captures in existing index |49| raw-index-capture.c |
| reused element pointer after raw capture |103| rejected |
| required signed call-argument capture |49| normalized/rejected |
| direct array cap/increment |39| body.c, best |

Shared-byte control merges the two existing byte locals; word control changes
only that merged local's width. The pointer controls use actual array element
addresses, with no guessed neighboring aggregate. Index narrowing preserves
the 0–15 domain but adds an unwanted AND. FAKE raw captures assign both required
F5F loads to the existing index before masking, preserving conditional reload.
The call-argument control captures the existing -128 argument in a signed
counter word; emitted allocation stays unchanged. These results do not justify
further width, pointer, or argument-capture sweeps.

Replay retained forms serially inside existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24070/replay.py
python3 drafts/T-24070/replay.py --variant baseline
python3 drafts/T-24070/replay.py --variant state-pointer
python3 drafts/T-24070/replay.py --variant raw-index-capture
```

The helper replaces only the assigned fallback in a private full TU, strips
the archive-status comment to preserve measured source layout, and checks
production unchanged. It uses explicit-object --resolve --version default
--no-build, separate from actual default C proof. Logs stay under
build/etc24070. Sources only are retained; no object, original assembly or
generated compiler source is committed.

The next precise question is why the first index range cannot choose v0.
Existing unchanged allocator trace plus same-input whole-object fidelity may
distinguish interference from priority before any new source hypothesis.
The proposal uses the existing tracer without rebuilding or changing allocation.

Parent approved the existing unchanged tracer for the 39-row input. Whole-object
fidelity passes before interpreting the log, including .text12736,
.rel.text8552, .rodata736 and .rel.rodata248. Input SHA256 is
`73c73dcbc9e8f137a3caaff17210e08cb79005644c46e3a60058be18f0ad0f44`;
whole object is `d597f00bf1b5f61de61d0245f98e27a7c928b75891768e4cd3be052f65eae61e`.
Existing tracer source is pinned59767ece... as in T-24068; no binary is built.
Mount /private/tmp/t24047-uopt read-only at /scratch in Docker:

```
python3 tools/uopt_eval.py compile build/etc24070/direct-caps/source.c build/etc24070/trace39/baseline.o --uopt /scratch/variants/trace --trace --jobs 1
python3 drafts/T-24070/trace_fidelity.py
```

The synthetic first-mask local is bit16, priority3, chosen color1(v0).
Retained index bit22 has priority1.5 and gets color2(v1). First address bit76
and counter bit77 also have priority1.5, chosen colors3(a0)/4(a1).
This matches the observed register rotation. The existing tracer reports best
costs, not per-range forbidden masks, so it does not prove the complete reason
the index cannot choose v0.

One derived ordinary source form removes the synthetic first-mask local,
loads the first byte through the masked expression, then captures the retained
index. It gives105 rows. The expected mask scratch/copy returns, but index
still gets v1 and a duplicate first-array address appears. Retaining an actual
byte-element pointer gives110; it also changes the required +0x53C access into
offset0 plus an extra addiu. Both are rejected and retained as
inline-first-mask.c/inline-retained-address.c with replay variants. These
results limit the priority explanation; removal of the named local alone does
not establish v0 availability. Best remains39 with production unchanged.

Parent next approved only an unchanged-tracer comparison on the already-tested
inline105 source. Same-input whole-object and all four sections pass first.
Input SHA256 is `7c5a2f24a580ebffc5b5f7098b7ed12923697e0ec79b8e17e474bd062f5612aa`;
object is `446a2ed155f3a7ab8b0615bcf361179f585526d14ce85c07ccacf49168eaa0ff`.

```
python3 tools/uopt_eval.py compile build/etc24070/inline-first-mask/source.c build/etc24070/trace105/baseline.o --uopt /scratch/variants/trace --trace --jobs 1
python3 drafts/T-24070/trace_fidelity.py --variant 105
```

The common global mask expression becomes bit80, priority3, and still takes
v0 first. Retained index bit23 has priority2 and takes v1. Removing the named
local changes its representation but does not remove the competing expression
range. This is observed priority/choice, not a per-color forbidden-mask result.
The next source proposal changes only the conditional reload from &15 to
unsigned modulo16, preserving the byte's0–255 domain and0–15 index. It tests
whether the front end retains separate expression ranges before a late ANDI.
Parent subsequently approved that one conditional reload control, after this
fidelity checkpoint is pinned. No result is claimed here; other choices stay fixed.

The approved conditional unsigned-modulo reload is neutral105 and its whole
TU object equals the inline105 stock object446a2ed1... exactly. The first
mask and all other source choices stayed fixed; the rejected diagnostic has
an explicit FAKE annotation for the expression-web purpose. Replay with
`--variant conditional-modulo`. Best remains39. Current observations support
no further legal source form. The unresolved question is the
common mask/index interference or cost beyond the existing best-color output;
that output does not expose per-range forbidden masks. No new instrumentation,
source sweep, production change or compiler-impossibility claim follows.

Parent stops new probes/instrumentation while an existing Astra chat analyzes
only this source read-only, without compiling, editing or spawning agents.
The Sol scope stays reserved and the draft remains intact pending at most
two concrete hypotheses. No new target is assigned.

Astra's read-only analysis has completed without a supported untried source
lead. The matched NAME_ENT case0 lacks both caps and cannot establish their
allocation. Original t6 mask/indexv0 differs from the draft's common-maskv0;
existing trace does not fully separate priority from interference. T-24070
is unresolved/In Review, not Done or a compiler-impossibility claim. The39
draft and negative controls are preserved; all44F74 tracing/probes stop.
