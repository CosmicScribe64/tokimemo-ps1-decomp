# T-24078 bounded late-height lifetime set

Three sequential controls independently against signed indexed-uv-right618,
not a Cartesian combination with unsigned606. No new capture, logger, compiler
option or production edit. All s32 declarations/home order, signed DIV casts,
SDK stores and geometry formulas remain fixed. Matching-only choices have FAKE
comments; original local/expression/type spelling remains unproven.

| Source | Falsifiable prediction | Aligned cost / rows / words / frame | Actual result |
| --- | --- | --- | --- |
| height-x0 | After x2, X0 scratch is dead in all arms; reuse it for H may bind H to later X0 lifetime, or normalize/split |618/846/592/90|TARGET resolved IDENTICAL to618|
| height-capture | Capture H inside Y2 first consumer instead of standalone assignment may move load/definition ordering, or normalize/hoist |618/846/592/90|six ADDU words change, H still same early v0|
| height-first-narrow | First H use only exposes actual SDK low16 domain; a separate conversion range may survive allocation or drop/add copies |650/804/594/90|four MOVE added, two NOP removed, H still early v0|

Disjoint reuse is legal: X0's final read is the x2 SH; lower-right X3 uses other
terms or separate rightX, never X0. H is defined after x2 and feeds only y2/y3.
The existing X0 variable can receive H, then UV24 at the shared tail after all
arm values die. Existing H variable retains its earlier E and later UV160 roles,
so no declaration is removed and no source value crosses arms/calls/iterations.
The result normalizes fully; it supplies no new register lifetime evidence.

Capture placement moves only four assignments into first y2 expressions. Final
H definition offsets remain +12C/+2E4/+498/+698, same subu v0,s6,s1. Changed words
are first y2 ADDUs: cases0/1 reverse operands, cases2/3 change addition association
between fresh center, Q112 and H. This is mathematically equal but not a late-H
scheduling repair; current global score is neutral, not exact identity.

The third control casts only H at y2 to s16, leaving word H definition and y3
use unchanged. SDK y stores already retain low16. More strongly, actual normal
A0..80 gives H80..160, exactly unchanged by s16 cast before any addition. This
is an equivalent first-consumer expression, not a declaration width recovery.
Its purpose is the known one-of-two masked-use allocation idiom; FAKE original
type unproven. No sign-extension instructions remain, but four extra copies do;
650cost/+2words is worse even though printed rows804 is numerically lower.
No other H width, mask, suffix or placement permutation follows this set.

All controls preserve frame90, poly84/88 and UV50/54. Constant160 stayss6,
136fp, mode7 and page spill60; signed Q11-before-P order remains. Retain signed
618 and marked unsigned606 independently, alongside numeric604. These negatives
show the tested lifetime forms fail, not that no source solution exists.
