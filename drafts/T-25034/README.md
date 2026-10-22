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
