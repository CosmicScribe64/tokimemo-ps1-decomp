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
archived replay logs use build/etc24068/archive/.

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
