# T-24045 TEL profile-screen match

Start 68126eccd. Production func_8013D03C is now unguarded C with no target
INCLUDE_ASM. New literal verified C: 2128 bytes. The archived matched-body.c
is the installed function body; shared type is in include/ovl/TEL.h.

## Recovery and negative evidence

T-19027's retained source needs its TelSignDays declaration and current
D_800B3778[n][j] accesses: that table is now u8[13][7], not the old flat view.
The corrected baseline gives 97 printable rows, 531 candidate words against
532 target words. Shared-line SET_REC forms leave the score unchanged.
Moving q's assignment after the four image-address stores fixes its two
scheduling rows. An explicit if/else-if/else age selection restores the
missing branch around the final arm.

The birthday calculation must read its packed word once, then extract month
and day. Reusing day adds broad ugen temporary changes (439 rows). Reusing i
gets 51 rows, then 44 after the pointer-store and age fixes, but the word stays
in s1 and shifts the age index/constant ten. Reusing n gives 172 rows. A
new scalar within the old padding gives 404 rows; a union aggregate copy gives
93 rows with an unwanted stack store, and a scalar union-member assignment
again gives 404. Reusing the record pointer gets 84 rows; removing its +0x1BC
address calculation gets 82, still moving the table base from a0 to a1.

The successful form reads q through a nonvolatile GameState** lvalue only for
the month/day pair. This lets the current compiler share one packed-word load
while preserving the surrounding reloads. The view is a compiler-specific
FAKE, not evidence of original source or a portable volatile-access pattern.
It is explicitly marked for standards review. The inherited unused pad[32]
and volatile q are also marked FAKE with T-24045. No custom flags, compiler
patch, inline asm or generated assembly change is involved.

## Proof

Actual installed production source was freshly compiled by:

```
tools/docker.sh ninja -j1 build/ovl/TEL.ok
tools/docker.sh python3 tools/funcdiff.py func_8013D03C --unit TEL --resolve --no-build --version default
```

The build ran CC for the changed TU, LD, OBJCOPY and SHA1 CHECK. TEL checksum
passed; headers passed. Literal resolved default proof: MATCH, linked
function bytes equal target (2128 bytes). This verifies actual C, not an ASM
fallback. Other TEL source files rebuilt due to the approved header addition;
pre-existing array-address warnings in TEL/80132000.c are outside this change.

Scratch source variants and instruction comparisons remain ignored under
build/T-24045 and build/T-24028/reproduce/func_8013D03C. No instruction listing,
object or game data is committed. Ticket is In Review pending root standards
review and integration. No push/merge.
