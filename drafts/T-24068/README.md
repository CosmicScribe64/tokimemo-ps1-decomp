# ETC album interaction checkpoint

Exclusive scope is func_80140AE4 (224 bytes) in src/ovl/ETC/8013FF80.c,
from setup59f6194a3 on codex/complete-etc-album-interaction. Other bodies,
the published252-byte model-loop win and unresolved T-24067 remain unchanged.
Ticket is In Progress; production still INCLUDE_ASM, no new verified bytes.

T-13103's GameState pointer transfer48 rows/switch54/58 and T-9560's
selector/result/constant-width/clamp/temporary-order controls were read first.
Current sibling report has no matched sibling at similarity>=0.70.
Fresh baseline uses real GameState.unk_F80 and the historical unsigned byte
comparison view, avoiding the old byte-offset draft cast. It reproduces48
resolved +/- rows, with the correct56 words/frame0x28. Differences are the
selector in a0 rather than v0, plus a uniform one-step later scratch mismatch.

Two new bounded controls both retain48 rows and are rejected:

- The three required discarded-return calls in one C89 comma expression,
  preserving exactly their original order, are neutral.
- Selector reads through existing word D_801500D8+4=0x801500DC are explicitly
  FAKE diagnostics (unknown aggregate). Width/alignment and D8 mutations are
  preserved. This does not improve allocation and flips an additional compare
  operand orientation; no neighboring-base/type sweep follows.

Replay each privately in existing Docker, serially:

```
python3 drafts/T-24068/replay.py
python3 drafts/T-24068/replay.py --variant calls-comma
python3 drafts/T-24068/replay.py --variant neighbor-selector
```

Each uses the full TU and explicit-object --no-build --resolve --version
default --unit ETC, checks production unchanged, and makes no normal default
C proof claim. Initial logs are build/etc24068/{baseline,calls-comma,neighbor-selector}/diagnostic.txt;
The replay recreates those same variant directories and strips only its
archive-status comment before compilation, retaining the original input bytes.

Existing read-only /private/tmp/t24047-uopt/variants/trace was reused unchanged,
mounted at /scratch/variants/trace in Docker. Same-input stock/private entire
object SHA256 agrees at `e4ce1543013767591dacbbed4d00f4889d9b3595e1773b10c82a549c92aba6ee`
before interpreting the trace. The selector bit0 has priority0.5, best cost0
internal color3 (a0). Constant1 bit27 has priority0.666666687, takes color2
(v1) with cost1 first. This is not an equal-priority tie. Fixed call-result
webs and color1 (v0) cost/liveness need explanation before a new control;
the existing tracer reports best cost, not the rejected per-color costs.

Exact trace input is build/etc24068/baseline/source.c; command:

```
python3 tools/uopt_eval.py compile build/etc24068/baseline/source.c build/etc24068/trace/baseline.o --uopt /scratch/variants/trace --trace --jobs 1
```

build/etc24068/trace.log holds priority/cost observations;
trace/baseline.o.trace holds the target's bit identities/flow. No compiler,
header, config, production source or allocator decision was modified.

Parent approved exactly one target s32 fallthrough/no-return control after
checking history: T-9560 only retains generic20/16/16 variant counts, and
T-17300's void draft does not prove this exact view was exhausted. All callee
prototypes and other source choices were fixed. It reproduces the exact48-row
baseline register differences and is rejected; no width/result sweep follows.
The diagnostic is explicitly FAKE (original return type unproven) and retained
as `replay.py --variant target-return`. No production/interface change occurs.

The existing trace's best-color report cannot establish whether v0 is
forbidden by a fixed call-result live range or simply has higher call cost.
Read-only globalcolor inspection locates LR forbidden masks at+40/+44 and
candidate loops L47187c/L4719cc, with costs before choice atL47190c/L471afc.
These are the precise observation points needed for a narrow logging-only
diagnostic, subject to parent scope approval and full same-input object audit.

Parent approved exactly those bit0/colors1..3 observations in a separate
private logging-only uopt binary. trace_colors.py pins the existing source
hash, logs mask/candidate cost before existing choice branches, and writes no
emulated state or allocator decision. No shared tracer/production tool changes.
The native source hash is `cc46d1fc8698fd97e546731b5681f88e8aca3cd87b70db8e1f893cef4675bf4d`,
private binary `96d3912e115d7675b8d56217b4ee1eec631a63384b9c3e1681bddd9c9ea8b021`.
Use existing /scratch/recomp deps, serially in Docker:

```
python3 drafts/T-24068/trace_colors.py
python3 tools/uopt_eval.py compile build/etc24068/baseline/source.c build/etc24068/color-cost/baseline.o --uopt /work/build/etc24068/color-cost/trace-colors --trace --jobs 1
python3 drafts/T-24068/trace_colors.py --audit
```

Mandatory audit PASS: same-input whole object SHAe4ce1543013767591dacbbed4d00f4889d9b3595e1773b10c82a549c92aba6ee,
with equal .text4048/.rel.text3712/.rodata64/.rel.rodata120. Input source SHA
is `9fd845874a7b9de9ce82a9b6eb65413c719d4b2c0cbebbd695d32a1e7cabad88`.
The selector's forbidden mask is60000000/0: color1(v0) and2(v1) are skipped
as forbidden, so their costs are not evaluated. Color3(a0) is allowed/cost0.
Thus this is an interference restriction rather than a higher-price v0 choice.
Read-only updateforbidden unions basic-block register masks over live-range
units; selector units are bb3 (just after the final leading check call) andbb11.
The existing output does not identify the particular fixed-range contributor.

One derived source hypothesis is explicit `(void)func_80140E80();` only for
the last leading discarded-return call, preserving its true s32 prototype and
all required calls/source. This differs from comma sequencing by marking the
result category directly at the call before selector bb3. It might remove a
fixed v0 result restriction without emitted operations. This is a hypothesis,
not evidence of a match; no source control result is claimed yet.

Parent approved that one explicit-final-discard control. It is neutral48 rows
and its entire object equals baseline, so it is rejected; no instrumentation
widening or result/width sweep follows. Reproduce with `replay.py --variant
discard-final`. Callee prototypes and all other source choices stay fixed.
The exact unresolved question is which existing bb3/bb11 register-mask
contributor supplies forbidden v0; next work is existing trace/source mapping
only until any smaller missing observation is reported and authorized.
