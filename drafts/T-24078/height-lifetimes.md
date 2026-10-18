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


A second bounded expression/lifetime set follows, no random search or type grid.
Staged-H sets the existing H local160 then compound-subtracts A at its original
post-x2 location. This plausible marked two-stage assignment tests whether a
local initial-value lifetime survives copy propagation. Actual A0..80 semantics
are identical; result TARGET IDENTICAL618/592words/frame90, no new evidence.

Negated-H changes only four H expressions to -(A-160). Actual A0..80 gives
intermediate-160..-80 then result80..160 without overflow. MIPS can encode
subtraction of160 as immediate arithmetic, so prediction was canonicalization
or a different immediate/negation web without saved160. Measured616cost/846rows/
588words/frame90. All saved roles now agree with original: mode6,136s7,pagefp,
no page memory spill; UV50/54 and poly84/88 retained. But final H is early NEGU
v0,A; optimizer distributes160 into Y's final offset, yielding Y-A+59 instead
of original standalone160-A followed byY+H-101. Two lower-right formulas still
fold to ADDIU160. Original word count is not a match: current counts differ
LI-6, ADDIU+2, ADDU-2, NEGU+4, SUBU-4, NOP+6, all other counts equal. Preserve
616 as a separate exact-count/saved-pool diagnostic, not a recovered spelling.

One further typed-boundary control against that NEW616 representation adds only
(s16) around the four negated H definitions, leaving the shared H/E local s32.
This is an exact value identity for H80..160; its explicit FAKE purpose tests
stopping the observed affine distribution, not recovering original type. The
prediction was a boundary might retain H while SDK low16 consumers drop narrow
operations, or leave extra operations. It DOES retain four local LI160/SUBH and
keeps original saved roles, but four SLL/four SRA/four MOVE survive; NOP80-to75.
Measured635cost/769 diagnostic rows/599words/frame90, reject extra instructions.
Rows769 lower than846 is not improvement. No alternate widths/masks/suffix
permutation followed. Original low16 field stores alone do not prove H width.

This isolates a remaining source question: retain a real word-valued H boundary
without either global160 pooling or distributing160 into Y+59, and without
non-original narrow/copy instructions. No faithful solution is established.
Current source forms all remain archived separately;618/606/604 frontiers are
not overwritten, production remains ASM and same function remains active.
