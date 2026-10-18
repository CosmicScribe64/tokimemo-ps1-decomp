# T-24078 original value relationships (read-only)

Compare original func_8013D570 with pre-control638-cost shared-temporary body.
A=arg0, i=iteration0/1, P=low32(i*136), H=160-A. Qn denotes signed
DIV80 of low32(A*n); R denotes signed DIV80 of low32(low32(A*i)*68).
Y denotes a FRESH signed GameState.unk_018[D_8011ECA0] load, not a cached value.
Intermediate arithmetic is word-sized; SDK vertex stores retain low16 bits.

| Original full VRAM | Relationship | Retained source |
| --- | --- | --- |
| 8013D654..D700, D9B8..DA70 | Case0/2 X=(P-Q11)-R+24 | temp_v1 same formula |
| 8013D80C..D8C0, DBB8..DC70 | Case1/3 E=(Q141+P)-R; X=E+24 | temp_v1_2, temp_a1 |
| 8013D704..D770, DAC0..DB10 | Case0/2 upper-right=X-Q68+136 | x1 same association |
| 8013D7C4..D7E0, DB5C..DB88 | Case0/2 lower-right=((160-Q11)+P)-R-Q68 | x3 same association; do not collapse to x1 |
| 8013D8C4..D930, DCC4..DD10 | Case1/3 right=E-Q68+160 | temp_a2, x1/x3 |
| 8013D70C..D7FC, D8CC..D9A8 | Case0/1 top=Y-101; bottom=(Y+H)-101 | y0/1 and y2/3 agree |
| 8013DA74..DBAC, DC74..DD98 | Case2/3 top=(Y+Q112)-101; bottom=((Y+Q112)+H)-101 | temp_lo_8; both y associations agree |
| 8013DDA0..DDF8 | LW UV words sp54/sp50, then SB copies to u0/u2 and u1/u3 | inherited s8 casts truncate earlier; low byte agrees |
| 8013DE44..DE6C | After AddPrim, reload UV words, +8; i+1; cursor+4; page+128; stop at2 | independent induction variables agree |

Constants11,141,112 are confirmed by original shift/add/sub sequences; every
quotient uses signed DIV, not DIVU. The original does not replace R with i*Q68,
though that identity holds for i=0/1. Such simplification would erase original
repeated multiply/divide dependencies. No formula/address/missing operation
correction was found in this pass.

True cross-call/iteration values: s1=A, s6=mode, s2=GameState address (not a
cached field), s3=80, s5=68, s7=136, s4=i, s0=&poly[i], fp=320+128*i;
UV words start160/24 and advance8 after AddPrim. No calls occur inside the four
arithmetic arms. Quotients and computed vertex coordinates need survive only
within their arm; persistent constants/induction values survive initialization
calls and AddPrim. Frame index and signed center values are repeatedly reloaded
after polygon stores; do not cache these globals across stores or calls.

One supported structural question, not a proven semantic bug: original v1 is
word-sized atD6FC/D700, stored with SH atD708, then still consumed as a word by
SUB D768 before SH D770. Cases1/3 a1/a2 likewise remain full words from
D8C0/D928/D92C to SH D95C/D98C (and DC70/DD08/DD0C to DD40/DD74).
The draft declares temp_v1/temp_a1/temp_a2 as s16. All later uses are additions,
subtractions and low16 stores, so early narrowing is observationally equivalent
for these outputs; original assembly alone does not prove original C types.
A single word-until-SDK-store control changing ONLY those three locals to s32
would follow the observed register values. Parent subsequently approved exactly this control:604 alignedcost/670rows,
592words/frameA0 versus joined638/594/98. The first arithmetic divergence
remains MULTU-before-DIV, so the width correction does not close the dependency
schedule. No other source choice changed.
This is not authorization for other width/layout permutations or instrumentation.
