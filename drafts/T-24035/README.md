# T-24035 small sibling transfers

Two actual default C matches: GEKO func_80134228 (244 literal bytes) and
BUNKA_SD func_801344A8 (152 literal bytes). Total 396 bytes. No header,
compiler, flags, generated assembly, or raw-data changes.

## Production proof

```
tools/docker.sh ninja -j1 build/ovl/BUNKA_SD.ok build/ovl/GEKO.ok
tools/docker.sh python3 tools/funcdiff.py func_801344A8 --unit BUNKA_SD --resolve --no-build --version default
tools/docker.sh python3 tools/funcdiff.py func_80134228 --unit GEKO --resolve --no-build --version default
```

Both targets have their guard and INCLUDE_ASM fallback removed. Normal
builds/checksums and literal default linked comparisons pass. The GEKO
compiler emits existing incompatible-pointer-assignment warnings in the
unchanged func_8013478C; header checks pass.

GEKO copies the matched func_80135AC8 source, changes the leading flag mask
from 0x20 to 0x40, and omits that sibling's D_80122D4C clear. The u8 load
view and dead-result test retain explicit FAKE reasons, now under T-24035.

The fade-out copies matched func_8013987C, changing decrement 1 to 2.
Promoted u8 counter views and the *= 0 clear retain the sibling's disclosed
FAKE reason. The zero must remain a register for the unrolled clear loop;
a plain zero assignment folds it to sb zero.

## Fade-in near miss

func_80134418 remains behind its existing NON_MATCHING guard and fallback.
Its four printable diff rows are a changed branch offset and two extra
narrowing instructions, andi plus move, after loading cap 0x80. Target has
36 instructions, candidate has 38. No matching C gain is claimed for it.

```
tools/docker.sh python3 drafts/T-24028/probe.py BUNKA_SD func_80134418 drafts/T-24035/bunka-sd-80134418-near.c
```

This reproduced four rows after adding the FAKE comments and updating the
production guard. Normal BUNKA_SD checksum still passes with that fallback.
Private exploration and diagnostic comparisons remain in ignored
build/T-24035; no objects/listings committed.

Measured source forms (printable rows): promoted counter with plain cap
assignment 32; constant after loop 23; named s32/u8 caps 23; clear/add cap
4; clear/OR cap 5; signed cap store 32; clear plus signed add 4 (sll/sra
instead of the unsigned mask); OR/AND cap 5; algebraic cap 7. The clear/add
near form retains all required FAKE comments. A direct unsigned cap or
local cap splits its constant from the promoted byte and changes the
first loop's counter/end-pointer allocation; arithmetic cap forms preserve
the desired allocation but retain conversion instructions.
