# T-25034 quadrant renderer checkpoint

These are unfinished research drafts, not byte matches or production code.
The default function still uses INCLUDE_ASM; no coverage increase is claimed.
The user requested wrap-up, committing and pushing on 2026-10-22, superseding
prior sustained matching instructions. All experiments have stopped.

`main/func_80046AC8/baseline.c` preserves the inherited draft with an incorrect
s16 extent test. `extent-corrected.c` changes that variable to s32: the target
branches on the full signed word and narrows only when storing TILE fields.
For radius zero and arg4=0x8000, case 1 must test positive 0x80A0; the inherited
draft instead tests a negative halfword. All accepted candidates retain this
repair on all four paths.

`body.c` is the conservative entry-order candidate: target674/candidate671
padded words, aligned361/printed381. `forward-join.c` moves the shared UV label
from case0 to case3 without changing its C semantics: target674/candidate670,
aligned360/printed380. Both emit the target0xA8 frame. Remaining differences
include color conversions, UV tail sharing and local stack homes. Details and
the completed experiment ledger are in astra-findings.md.

The active target body is2684bytes/671words. Three trailing padding nops add
12bytes, giving the diagnostic2696bytes/674words. The conservative candidate
body is2672bytes/668words; with padding,2684bytes/671words. The forward-join
body is2668bytes/667words; with padding,2680bytes/670words. No adjacent function
is included. The candidates remain12bytes and16bytes short, respectively.

Reproduce with the existing private inputs and Docker image:

```sh
tools/docker.sh python3 drafts/T-25034/reproduce.py
tools/docker.sh python3 drafts/T-25034/reproduce.py forward-join.c
```

Do not promote either candidate until literal C verification and review pass.
