# T-24513 ETC source reconstruction checkpoint

Base 23e1a20fb6efe81f446df28433c96c36330309c6, branch codex/t24513-etc,
worktree /private/tmp/tokimemo-t24513. No production C, header, config or
compiler change. Zero new verified C bytes. All target functions still use
INCLUDE_ASM. These diagnostic sources are not matching claims.

## Scene initializer func_8013FFAC

The original 2116-byte function supplies two six-entry address arrays to
func_8004A1A0. The first spans 0x80198600 through 0x801AC600 in steps 0x4000;
the stop address is 0x801B0600. Original pointer updates are four bytes.
T-16100's 16-element array and four-element pointer update were reconstruction
errors. The corrected source uses six elements and a one-element update.
Its second array uses the same natural six-entry loop as the nearby matched
ETC table constructor. Real GameState fields and existing two-word and pointer
tables replace obsolete scalars and byte-offset arithmetic. A private void
forward declaration reflects the actual later same-TU func_80140AE4 definition.

Removing the copied table pointer and byte-shade temporary restores original
frame 0x70 and initial selector register a1; it does not restore the original
buffer offset 0x54 (candidate 0x58). The signed first-loop address comparison is
explicit, matching original SLT. Six-element array bounds do not justify the
old 16-element buffer, nor guessed frame padding.

The retained scene.c is the natural second-table loop from 0. It is not a full
semantic-equivalence proof. The compiler expands the first constant address
loop into straight stores; the original retains that loop. It also expands the
second table differently, and later global promotion/register choices differ.
No source or compiler impossibility claim is made.

Fresh default full-TU metrics (rows are printable diagnostics, cost is resolved
instruction-key edit distance; neither measures byte coverage):

| Control | Words | Rows | Cost |
| --- | ---: | ---: | ---: |
| Original target |529|0|0|
| Corrected six-entry/stride draft |533|486|484|
| Consistent GameState pointer |533|476|476|
| Coherent int return view |536|489|487|
| Remove copied pointer/byte locals |533|492|456|
| Restore signed address-bound comparison |533|492|456|
| Natural second array loop from 0, retained |538|455|453|

The earlier variants were rejected or superseded. Only the retained semantic
source and new rejected ranking-pointer source are archived, avoiding copies
of every local control. Original assembly and compiler dumps stay ignored.

## Other bounded controls

Ranking func_801480D0 is 2808 bytes/702 words. Existing T-24049 baseline
reproduces 709 candidate words, 885 rows, 859 cost. Explicit generation pointer
walks in walk-generation.c worsen to 708/1030/994. Its pointers reflect original
cursor accesses but do not recover the source's register/spill shape. A coherent
int return gives 710/888/862; debug-int gives 709/885/859. Stopped after these
causally distinct negative controls; no declaration grid.

Grade func_80146D4C is 1056 bytes/264 original words. T-24303 baseline
reproduces 264 candidate words/104 rows/102 cost. Removing the copied score and
reading the work array directly in each grade comparison yields 264/138/136.
A byte view of the selector is neutral 264/138/136. Both rejected. The existing
promoted-selector and case2 arithmetic allocation gaps remain. No shared
main_api.h return override is introduced.

T-24078 renderer negative history was read; its archived controls were not
repeated. No production compiler or inline assembly experiment was performed.

## Reproduction

Run inside existing tokimemo-decomp-arm64 Docker, with this checkout mounted
at /work and the original root disc mounted read-only at /work/disc:

```
python3 drafts/T-24513/replay.py
python3 drafts/T-24513/replay.py --case ranking-walk
python3 drafts/T-24513/replay.py --case ranking
python3 drafts/T-24513/replay.py --case ranking-return
python3 drafts/T-24513/replay.py --case ranking-debug
python3 drafts/T-24513/replay.py --case grade
python3 drafts/T-24513/replay.py --case grade-repeat
python3 drafts/T-24513/replay.py --case grade-view
```

Replay substitutes actual C only into an ignored complete translation unit,
uses default compiler settings, validates explicit-object --resolve --version
default --no-build verdicts and asserts production source unchanged. A compiler
failure is a failure; no absent diagnostics count as a match. Scratch outputs
remain under ignored build/T-24513. Existing source archives supply unchanged
baseline bodies rather than duplicate copies here.

## Verification and review

All eight replay cases reproduce the metrics above and explicit-object DIFF
verdicts; production stays unchanged. Fresh Docker ninja -j2 builds all27
binaries. Independent SHA1 over build/SLPM_86.053.bin and build/ovl/*.bin,
compared with the27 configured digests, passes27/27. ETC SHA1 is
681c93b6e57c885f86e074c885a8bf6c3cca2319. These hashes verify existing ASM
fallbacks, not new C progress.

Final guards: headers OK; globals OK; prototype branch check OK with0 new
and15 known disagreements. An early global guard lacked TACO-generated
assembly while the clean build was running; the completed-build rerun passes.
A direct sha1sum -c attempt used overlay config filenames relative to the wrong
working directory; independent hashing of the actual built paths above corrects
that invocation. Replay also now accepts the expected DIFF exit status1 while
requiring an actual function verdict; compiler failures still abort.

Standards and scope self-review found no issues. No source/header/config/tool
production diff exists against the base. Ticket remains In Review for the
coordinator. No push, merge or integration/shared-root writes were performed.
