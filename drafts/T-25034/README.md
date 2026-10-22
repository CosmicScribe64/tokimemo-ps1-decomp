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

Matching resumed later on 2026-10-22 after the coordinator relayed a newer
human instruction to continue until resolution. The wrap-up above describes
checkpoint d8e482c4a, not the current worker state. The same Astra worker
continues this function; these archived candidates remain unchanged.

The resumed best is now `main/func_80046AC8/signed-mask.c`:674target/666built
padded words,212aligned/212printed. With s32 red, `(s8)(arg6 >>16)&255`
introduces an inner signed CVTL boundary; red/green emit a temporary shift
followed by ANDI directly into fp/s7. Keeping blue as a word extraction avoids
an unwanted stack LB. Frame0xA8 and all full-s32 extent tests are retained.
This remains nonmatching research. Replay with argument `signed-mask.c`.

The current `signed-mask.c` supersedes the212-difference version preserved in
commit b25963c45. Grouping RGB stores as one comma expression per vertex now
measures674target/666built words,198aligned/198printed. Colored-quad channel
extraction, its temporary registers and all twelve color stores match. The
block's branch delay slot still hoists a1=0 instead of the target red shift.
The rest of the function remains under active reconstruction.

The latest `signed-mask.c` adds only v3-after-y3 ordering in each UV case to
the198 checkpoint. Existing scheduling puts each UV store in the target y3
load delay slot:674target/670built words,186aligned/186printed. Four missing
NOP/UV positions are restored; the candidate remains four words short.

The current `signed-mask.c` now measures674target/673built words,159/159.
Case0 width-after-y0 and case1 width-before-height restore TILE delay slots.
Each UV case stores u0 before its goto; placing the shared label after case3's
u0 store reproduces all target UV exit branches/delay stores without rotating
saved registers. One instruction remains missing overall (TILE case2 radius
MOVE); UV endpoint still uses v1 rather than a1. Quad-only half-radius
narrowing was rejected; full-s32 TILE extent/radius semantics remain.

`integer-return.c` preserves the newer s32-return/no-explicit-return control:
674target/674built words,144aligned cost. The normal pipeline retains exactly
the previously missing TILE case2 radius move and updates branch distances.
The equal-length diagnostic printer reports902lines using positional rather
than aligned comparison; use aligned cost144 for this checkpoint. The void
159 candidate remains in signed-mask.c. Both known callers discard the result,
and no existing header declaration specifies a return type. This supports an
integer/implicit-int contract but does not uniquely prove the original source
signature. Eventual promotion requires canonical declaration/ABI checks.

Latest wrap-up checkpoint (2026-10-22): `uv-standard.c` preserves the standard
forward UV assignment order (u0,v0,u1,v1,u2,v2,u3,v3) in one comma expression
per quad. Target and candidate both have674 padded words; aligned cost116,
printed156. This exchanges the overlapping base-row and endpoint range
priorities, puts the endpoint in a1, reproduces reversed final store scheduling
and removes the spurious a1=0 hoist. Native trace identifies base-row CVT at
priority40/v1, reverse radius36.67/a0 and endpoint ADD30/a1. The diagnostic
regdiff label for the endpoint is misleading; the native expression is the
row-plus127 addition. The source remains a nonmatching research draft.

The user requested wrap-up, commit and push; source experiments are stopped.
Replay with `tools/docker.sh python3 drafts/T-25034/reproduce.py uv-standard.c`.
No production source, shared header or compiler changes accompany this checkpoint.
