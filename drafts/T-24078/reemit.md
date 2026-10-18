# T-24078 bounded re-emission observation

One parent-approved logging-only diagnostic variant and one SAME604 full-TU
compile. No compiler option, optimization decision, emulated state or production
compiler/source change. Generated source/binary and raw logs remain ignored.
Patch replay reconstructs the exact generated source; hashes/dependencies and
whole-object/section fidelity are reemit-fidelity.json.

Base source is existing /private/tmp/t24047-uopt/recomp/build/5.3/uopt.c,
SHA256b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f.
Logging generated source SHA2563ce7370e5302c5500b17042f18a60226b1a5420cd0f28874a8ea72db30b2c7de.
Private binary SHA25668d6a8ff4e167f7ed7df4a5377582e3a4b0eec80d2865b6c77ff75212d95e5f0.
SAME604 source609467db and WHOLEOBJECT67fbefa6 identities match stock BEFORE
log interpretation; text15008/rel.text10696/rodata224/rel.rodata448 also identical.
No debug option was appended: evaluator ran without --trace.

Native helpers read only procedure func_8013D570. At reemit entry they select
first-arm label159/LOC450 and stop beforeLOC455; graph reads have depth8/node80
bounds. Statement next+8, BB+16, value+20 and operator children+36/+40 follow
existing generated-reference accesses. Native state only bounds logging; no
native variable enters emulated decisions. One variable-copy sentinel appears
as kind0 in the raw log; it is not interpreted as an arithmetic graph node.
The separate uwrite hook copies at most32 output records from the same arm.

| Entry/output observation | Proven implication |
| --- | --- |
| Internal BB11, sourceLOC450 Q11 assignment is opcode96/NOP | original assignment is already eliminated before reemit |
| LOC451 P,452 R,453 base assignments are also NOP96 | original source ordering is present only in metadata/NOP positions |
| First X0 ISTR atLOC454 has ADD(SUB(SUB(P,Q11),R),24) value tree | P is the left child of first subtraction, Q11 right; no retained preceding Q11 store |
| P tree is MPY(i,136), Q11 tree DIV(MPY(A,11),80) | roots correspond to the captured arithmetic, not label guesses |
| Observed output MPY P at9, STR5 at10, Q11DIV at15, STR4 at16 | operand-first DAG emission yields P beforeQ11 after assignments disappear |

This rules out a simple swap of surviving Q11/P assignment statements inside
reemit. Assignment elimination happened earlier; the observation does not name
which earlier transform eliminated them. It also does not establish a compiler
limitation or original source type. H's post-ugen movement remains separate.

One faithful source implication: consider origin-first X0=((24-Q11)+P)-R,
mirroring already original-backed X3=((160-Q11)+P)-R-Q68, instead of recovered
normalized X0=((P-Q11)-R)+24. This changes a genuine expression relationship
rather than inserting a barrier or artificial value. Constant80 division bounds
Q11, and actual callers use A=0..80, i=0/1; this association is equivalent there.
The prior case0-compound test preserved normalized association and did NOT test
origin-first. No new source test has been executed by this packet.

Exact Docker recipe (existing image/dependencies only, one job, pinned context):

```sh
cp /scratch/recomp/build/5.3/uopt.c build/etc24078/reemit604/uopt.c
patch --fuzz=0 -p1 -d build/etc24078/reemit604 < drafts/T-24078/reemit-observer.patch
gcc -std=c11 -Os -fno-strict-aliasing -I/scratch/recomp build/etc24078/reemit604/uopt.c /scratch/recomp/build/5.3/libc_impl.o /scratch/recomp/build/5.3/version_info.o -lm -o build/etc24078/reemit604/uopt-reemit
python3 tools/uopt_eval.py compile build/etc24078/replay/word-coordinates/source.c build/etc24078/reemit604/diagnostic.o --uopt /work/build/etc24078/reemit604/uopt-reemit --jobs 1
```

Mount existing /private/tmp/t24047-uopt read-only at/scratch. Regenerate pinned
604 source/object using replay --variant word-coordinates first if absent.
Compare WHOLE OBJECT with its saved stock object before reading diagnostic.log;
then separately compare text/rodata and relocations. Reject any fidelity failure.
Do not use --trace (adds options), other variants or this binary for production.
