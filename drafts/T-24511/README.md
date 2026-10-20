# T-24511 TEL reconstruction checkpoint

Isolated base `23e1a20fb`, branch `codex/t24511-tel`. Production source and
headers are unchanged. **Zero new verified C bytes.** Both remaining functions
still use INCLUDE_ASM; the successful checksums verify that unchanged control.

## Date menu: func_8013D88C

Read T-24039 before this work. Its 782-word / 885 printed-row control reproduces.
The resolved target spans 745 words, including two padding words; its frame is
224 bytes. Candidate counts below include whatever padding the comparison keeps.
Rows are positional diagnostics, not coverage or independent instruction errors.

Two reconstruction corrections are visible in the immutable target:

- Page two branches out of the `i < 3` success arm before testing `i == 3`:
  use an exclusive chain. Independent consecutive `if`s add a redundant test.
- The `arg2` switch skips its ordinary condition for 14 as well as 12, 2 and 0.
  This is distinct from T-24039's erroneous `case 14` in the **i** dispatch.

The target also loads the packed F64 word once before each month comparison
chain for indices 3, 4 and 5. Explicit capture recovers that load placement and
removes thirty words versus the signed-index control. The existing coordinate
local is reused for the capture; this lifetime choice is provisional and marked
FAKE. The preserved corrected source has 755 words / 844 rows, still far from a
match. The independent-if diagnostic has 754 / 845, but retains the extra test.
Neither is a register-only near miss. Do not select source merely by row count.

| Hypothesis | Words | Printed rows |
| --- | ---: | ---: |
| T-24039 control | 782 | 885 |
| Add arg2 case 14 bypass | 785 | 862 |
| Split negative quotient and x scaling | 791 | 966 |
| Reuse arg1 as late byte offset | 796 | 993 |
| Byte flag pointer | 791 | 966 |
| Byte pointer plus arg1 reuse | 796 | 993 |
| Exclusive chain with split x | 792 | 965 |
| Same plus arg1 reuse | 797 | 992 |
| Change pointer initialization order | 785 | 862 |
| Signed loop index | 784 | 831 |
| Wide coordinate locals | 785 | 862 |
| Separate page-one pointer lifetimes | 804 | 935 |
| Declare arrays before scalars | 785 | 862 |
| Arrays first plus arg1 reuse | 790 | 907 |
| Packed-word capture / signed index | 754 | 845 |
| Same plus exclusive chain (preserved source) | 755 | 844 |

Byte flag pointers and initialization order are neutral. Arrays-first moves
spills below the arrays but retains the wrong 232-byte frame. Explicit separate
page pointer lifetimes and parameter reuse worsen output. The unresolved root
is allocation/spilling: the target gives the x-array pointer a saved register
and spills the flag induction pointer only across calls; the draft keeps the
flag pointer saved and spills menu pointers. Do not run a declaration grid
without a fresh causal hypothesis. No compiler changes or inline assembly.

## Replay

Run these **serially inside the existing project Docker image**, from the
repository root, with this worktree at /work and the original disc mounted
read-only at /work/disc. Configure and build first; limit ninja to j2.

```
python3 drafts/T-24511/probe.py
python3 drafts/T-24511/pointers.py
python3 drafts/T-24511/frame.py
python3 drafts/T-24511/word.py
python3 drafts/T-24028/probe.py TEL func_8013D88C drafts/T-24511/menu-reconstruction.c
```

The four small scripts preserve all sixteen measured transformations; generated
objects, assembly and comparison listings stay under ignored build/T-24511.
The shared probe preamble is loaded by the later scripts without executing its
compiler loop. Only source and compact diagnostic counts are committed.

## Large handler: func_80136724

Read T-19022, T-23001 and T-24022 before hypotheses. The target is 6,580 code
bytes; 7,044 includes its two jump tables. Read-only audit compared the older
custom appointment bit fields with the current TelDateEntry: field boundaries
agree. The T-23001 body already fixes current aggregate member spellings and
case 0xFF. No new concrete semantic repair was found in this limited audit.
Month normalization/promotion remains exhausted; no new large-handler compile
or source claim was made.
