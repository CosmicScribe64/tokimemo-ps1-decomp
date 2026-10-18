# T-24039 TEL date menu reconstruction

Source-only checkpoint from 8e742c12d. Target: TEL func_8013D88C in
src/ovl/TEL/8013CCD0.c. No production C or header change; zero new verified
C bytes. Production still uses its original INCLUDE_ASM.

The target has 743 instruction words plus eight padding bytes; the resolved
comparison covers 745 words (2980 bytes). Its frame is 224 bytes. The original
T-15900 repair reports score 1045, but needs current aggregate declarations
before it can be compared on this tree. Do not interpret that legacy number
as a fresh proof or compare it as a coverage result.

## Recovered source evidence

The menu uses four arrays spaced 24 bytes apart, each holding halfwords.
Eleven entries are passed to menu_set. Slot ten is the page-control entry;
slots zero through nine are date locations. The initial zeroing loop clears
ten halfwords. The old draft advances s16 pointers by two elements where the
target advances two bytes. It also increments a GameState pointer by one
structure where the target advances one byte through flags at +0x120/+0x12A.
These are real reconstruction defects, not register-allocation details.

D_800B342C is the existing pointer table. Its second-page base is entry ten;
the old draft's scalar D_800B3454 indexing multiplied a four-byte step twice.
Entries 16..20 name the special locations. Existing GameState fields cover
the date and F64 word/byte reads. DisplayActor44 REC(96) covers the final
cursor record. No new declarations or headers are required for these views.

menu-reconstruction.c fixes the strides, table indexing and switch extents,
and expands the shared-store gotos into direct stores. The first page needs
explicit default boundary cases zero/nine to retain its ten-way dispatch;
the second page has six entries, not the mechanical unreachable case 14.
Its GameState-typed byte induction is a diagnostic access view, not a claim
that the final original source used unaligned GameState objects.

menu-lower-row-diagnostic.c preserves the lower-row alternative with the
mechanical shared-store gotos and short coordinate intermediate. Its
unreachable case 14 is incompatible with the target dispatch bound. This
source shape is explicitly marked FAKE, is not promoted and is not a lead
suitable for register-only closing.

## Serial default compiler probes

All probes used the current complete translation unit, IDO5.3 default flags,
resolved target words and one Docker compiler. Counts are printable diff rows,
not instruction-error counts or coverage. No literal linked C match was obtained.

| Source hypothesis | Candidate words | Rows |
| --- | ---: | ---: |
| Corrected byte/halfword strides and current aggregate views | 743 | 1046 |
| Shared local lifetimes, byte-state pointer, switch boundary nine | 755 | 1056 |
| Same with volatile y/step pointers | 762 | 1089 |
| GameState byte view and two short coordinate assignments | 761 | 850 |
| Same with volatile y/step pointers | 768 | 951 |
| Correct boundary cases and s32 coordinate intermediates | 755 | 1050 |
| Direct case stores, s32 coordinates | 782 | 885 |
| Direct case stores, s16 coordinates (archived reconstruction) | 782 | 885 |
| Explicit shared GameState base | 780 | 895 |
| Same with volatile y/step pointers | 781 | 964 |
| Indexed state flags instead of induction pointer | 784 | 887 |
| Eleven-element arrays instead of twelve | 784 | 887 |

The archived reconstruction has a 232-byte frame; the lower-row diagnostic
also has a 232-byte frame. Neither recovers the target allocation: the target
keeps x/y and the x-array pointer in saved registers, spills y/step pointers
in its lower local area, and rematerializes its fixed game-state base. The
candidates retain a fixed base in a saved register and spill different menu
pointers above the arrays. Explicit volatile reloads do not solve this and
increase the differences. No declaration-order sweep was attempted.

## Reproduction

Run from the repository root; both commands compile actual scratch C and
print diagnostics without substituting ASM for the target body:

```
tools/docker.sh python3 drafts/T-24028/probe.py TEL func_8013D88C drafts/T-24039/menu-reconstruction.c
tools/docker.sh python3 drafts/T-24028/probe.py TEL func_8013D88C drafts/T-24039/menu-lower-row-diagnostic.c
```

Final annotated files reproduce 885 and 850 rows respectively. Scratch
variants, objects and target comparison listings remain ignored under
build/T-24039 and build/T-24028/reproduce/func_8013D88C. No assembly listing,
object or game data is committed.

Default control: Docker ninja -j1 build/ovl/TEL.ok had no work; an independent
SHA1 check of build/ovl/TEL.bin against config/overlays/TEL.sha1 passed
(d37c3ae6beb7ab361f39366b8274cde0f7c96b5c). This confirms the existing ASM
fallback build only. Ticket remains In Review pending root review.
