# MAIN counter closure

`func_8006D7EC` in `src/main/8006CB30.c` now matches all 4,692 literal
linked bytes at its original address. The full-TU guarded and promoted
default C each produce the expected MAIN executable SHA1:
`e823bd844a8f8fa4d05483b59c66bc54b8393b26`.

The baseline at 242c0c3a3 had 1,173 instructions with only position 94
different: the original increments the zero-valued counter; the compiler
materializes one. T-25026 identified the original fold in uopt.

## Mechanism

The SHOUGATU T-25033 donor supplied an unsigned-less-than-zero producer:

```c
idx = (u32)D_800E6280.unk_F5F < 0;
```

It is defined C and always zero. Uopt retains the comparison and later
addition. Ugen folds the comparison into a zero move, but emits the
counter addition intact in binasm. Stock as1 then substitutes the zero
and produces the unwanted constant one. This established a second fold
barrier that earlier direct-expression tests never reached.

The following arm, after the existing 0xFE store and before the increment,
retains the dependency across both stages:

```c
if (idx) { idx = idx + ((u32)day < 0); }
idx++;
```

The arm is unreachable because idx is zero. Its right-hand comparison is
also always zero. The arm and its branch disappear entirely; the real
increment remains. There are no extra instructions, reads, writes, calls
or branches in the final output. Both constructs have FAKE comments.

## Controls and reproduction

Docker only, existing `tokimemo-decomp-arm64` image. `late_zero.py` accepts
named candidates and writes full-TU sources, measurements and diagnostics
under ignored `build/t25036/`. It reconstructs the old zero initializer
from the promoted source to retain a reproducible baseline.

```sh
python3 drafts/T-25036/late_zero.py baseline global-byte carrier-inner-late-add
```

Expected resolved results: baseline 1,173/1,173 words, cost 2;
global-byte 1,173/1,173, cost 2; carrier-inner-late-add 1,173/1,173, cost 0.
Cost 2 means two printed substitution lines for one differing instruction.

Other tested controls: local unsigned-day and global-address producers
keep the same final fold; counter-address changes allocation. Narrowing,
mixed-view arithmetic and variable shifts add instructions. Dead arms
whose conditions are written directly as unsigned comparisons disappear
too early. A branch on the carrier with an ordinary alternate assignment
keeps the increment but leaves unreachable code and a branch. Replacing
that alternate assignment with the late-zero identity removes both.
The equivalent forward arm leaves an unconditional branch and delay slot;
placing the arm immediately before the increment is necessary in the
tested variants. No compiler options or shared tools were changed.

`capture.py LABEL` uses the read-only tracer mounted at `/tracer/trace` to
capture pre/post-uopt records; `capture_as1.py LABEL` captures unchanged
binasm and stock as1 initial-node diagnostics. These are optional phase
diagnostics, not part of the build.

`prove.py LABEL SOURCE guarded|default` compiles the whole TU and links it
privately against verified MAIN dependencies mounted read-only at
`/baseline`. The integration tree's `asm/` and `disc/` are also mounted
read-only at `/work/asm` and `/work/disc`. It checks original address,
function size, all literal bytes and the entire executable SHA1. Fresh
guarded/default proof results are under `build/t25036/*-proof/proof.json`.
No existing objects are overwritten or copied in bulk. Root owns the
normal full-project integration build.

Header/global checks pass; prototype branch check reports zero new
disagreements and 15 known. Original raw inputs and binary diagnostics
are not committed.
