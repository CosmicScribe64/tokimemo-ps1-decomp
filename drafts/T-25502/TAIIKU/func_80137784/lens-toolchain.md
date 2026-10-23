# T-25502, toolchain lens: TAIIKU func_80137784 (1216 bytes, 304 words)

Result: the natural C matches (`MATCH (linked function bytes equal target (1216 bytes))`) when one uopt allocator
rule is changed. With stock IDO 5.3 the same C is 226 rows; the guard in `src/ovl/TAIIKU/80133C80.c` stays at
4 rows. The rule is a candidate toolchain emulation pass (CODING_STANDARDS 7a), prototyped here as
`tools/uopt_model.py`. It is not in the build. It needs a patched uopt in the Docker image, which is a Dockerfile
change for the user to approve (docker.sh rebuilds the shared image when the Dockerfile changes). One
FAKE-matched function also needs a different FAKE spelling (BUNKAKEN func_8014D65C, see "Cost"). The committed
`src/` and `tools/cc.py` are unchanged, so this branch still builds with today's image.

Measured with `tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --built OBJ`, OBJ a whole-unit object
compiled by `tools/uopt_eval.py compile` (the full cc.py pipeline: cvt_pass, frame_pass, ugen_pass,
asm-processor, padding) with only the uopt binary replaced. Scratch: `$TOKIMEMO_SCRATCH/w12/`.

## Files

- `natural-model-0rows.c`: the body with natural compares (`D_8014984C == &D_8014984C[arg0]`,
  `D_8014981C == &D_8014981C[arg0]`) and without the two D_8014985C FAKEs. MATCH with the modelled uopt, 226 rows
  with stock. The other FAKEs of the guard (RUN, the `(s32)` index, the D_801491D8 record) stay; they are unrelated.
- `tools/uopt_model.py` and `tools/test_uopt_model.py` (repository root) (8 tests on synthetic text): the patch
  generator. Its output equals the tested scratch variant `cmplda_loop` up to one pair of parentheses.
- `build-integration.patch` (not applied): Dockerfile step that builds `/opt/ido-model/5.3/uopt` from the pinned
  recomp with `uopt_model.py`, the cc.py hook that runs it in the uopt shim (fails if missing), and the
  `uopt_eval.py` shim target. The Docker step itself is untested (no image rebuild); the binary used below was
  built in scratch with the same recomp commands.
- `src-taiiku.patch`, `src-bunkaken.patch`: the source changes that go with the pass (plain C for the target; the
  BUNKAKEN FAKE respelled).
- `lda_compare_scan.py`: original-code scan (section 2). `mkvar.py`: the scratch uopt variants (`cmplda`,
  `cmplda_loop`, `log`, `loop_log`, `same`). `try.sh`, `which.py`: measurement helpers.

## 1. The intermediate change that names the behaviour

T-25201/T-25309 showed that the compare's address constant must not be the same uopt candidate as the store and
`++` address base: zero-offset alias symbols match. This pass looked for the allocator step that joins them.

uopt's `makelivranges` walks every expression of a block (`func_0045E5C4` in n64decomp/ido `uoptreg1.c`,
`func_464e50` in the 5.3 recomp). For an address constant (`islda`) it asks `ldainreg(memtype, offset, class)`. In
5.3 that function ignores memtype and offset and accepts every operand class except 4; `==`/`!=` operands have
class 3. So the `lda D_8014984C` of the compare gets a live unit in its block and joins the candidate of the
array base: units bb9 (store base, reload), bb11 (the compare, no reload), bb24 (`+=` base, reload), priority
0.5, ahead of the 0x44 constant (0.4). The base takes `t5`, and every later colour moves (226 rows). D_8014981C
is the same case in block 3 (base unit, then the compare one block later).

Scratch variants (equ/neq get a class of their own, 6, which the integer-constant case maps back to 3, so only
the `lda` case can change):

| uopt | natural body |
| --- | --- |
| stock `/opt/ido/5.3/uopt` | 226 rows |
| `same` (6 mapped back to 3 for `lda` too; control) | 226 rows (object identical to stock) |
| `cmplda`: an `lda` operand of `equ`/`neq` gets no live unit | MATCH |
| `cmplda_loop`: the same, only in blocks of estimated frequency 1 | MATCH |

The minimal change: outside loops, the address in a pointer compare is not a register candidate; ugen then
materialises it at the compare (`lui; addiu; bne a0,t8`). All 304 words match, both operand orders included,
with the natural spelling (address constant on the left) and no FAKE for the compares.

## 2. The same situation across the original code

`lda_compare_scan.py` reads every splat function file (6938 matched and 14 non-matching; IDO game code only) and
classifies each `beq`/`bne` whose operand register holds `lui/addiu X`:

| site | count |
| --- | ---: |
| fresh: `lui/addiu X` in the compare's own block, outside loops | 65 |
| fresh-delay: the same with the `addiu` in the previous branch's delay slot (as1) | 4 |
| fresh inside a loop | 29 |
| loop: X set before the loop, tested at the back edge (hoisted bound, mostly strength-reduced) | 274 |
| unsure: a label in between; 8 of 10 read by hand (strength-reduced loop tests, argument registers), no held X | 10 |
| X held from an earlier block, outside loops | 0 |

Outside loops the original never compares against an address it keeps from an earlier block. That agrees with
the rule. It does not separate the rule from stock 5.3 by itself, because the matched C compiled by stock gives
the same 0; it says only that no original function contradicts the rule. Among the 14 remaining functions only
TAIIKU func_80137784 has such a compare outside a loop (func_8006B0C8 has one inside a loop, where the rule keeps
IDO's behaviour); so no other remaining function can improve through this rule.

How often IDO itself meets the situation: the logging variant `loop_log` prints every `lda` operand of `equ`/`neq`
that reaches `ldainreg`. Over the full build (every C object; list in `census-fullbuild.txt`) there are 28 such
sites: 17 in loop blocks (the rule keeps them) and 11 at frequency 1 in 7 functions: the target (2), TACO
func_801497EC (3, the compares against the separately named `D_801274D0`-style objects), GYOZI func_8014A480 (2),
ETC func_8013B98C, NAME_ENT func_80140D6C, TT func_80142D40 and EVENT func_8010F74C (a stack address). With the
rule all of these except the target compile to the same bytes as with stock IDO (full build below), so for them the
compare unit never decided anything. BUNKAKEN func_8014D65C is not in the list because the census ran with its
respelled FAKE.

IDO 7.1's uopt (in the image) gives an object identical to 5.3's for the natural body (226 rows), so neither
in-image compiler has the rule.

## 3. Uniformity: every matched function

- `cmplda` (no loop condition; `uopt_eval.py regress`, stopped after 170 of 437 objects): changes `initCoordinate`
  and `func_800618B0` (main). Both are `do { ... } while (p != D_80122CA0);` pointer loops in which the original keeps
  the end address in `s2`. So the rule has to keep compare units in loops. It also changes BUNKAKEN
  `func_8014D65C` (below). The other flags were knock-on offsets of those functions or harness artifacts (main
  `sndisp` is 34/35 with the stock uopt too: the harness does not name `.data` pieces like the build).
- `cmplda_loop` (`regress`, stopped after 147 objects, 3307 of 3311 functions identical): the only real change is
  BUNKAKEN `func_8014D65C`. `sndisp` and the last functions of BUNKASAI 801511E0/80154018 (two trailing `nop`s)
  differ only inside the harness: the full build below gives byte-identical main and BUNKASAI images.
- Full clean build (all 520 edges; `tools/cc.py` temporarily pointed at the `loop_log` binary, `src-taiiku.patch`
  and `src-bunkaken.patch` applied, `ninja -j2`): **27/27 sha1 OK**, and
  `funcdiff func_80137784 --unit TAIIKU --resolve --version default` gives
  `MATCH (linked function bytes equal target (1216 bytes))` with the plain C (no NON_MATCHING, no INCLUDE_ASM).
  `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass on that tree. So no matched
  object changes, given the BUNKAKEN respelling. Afterwards cc.py and src were restored and the tree rebuilt with
  the stock toolchain.
- The binary built from `tools/uopt_model.py`'s output (not the logging variant) also gives MATCH for the natural
  body.

## Cost

BUNKAKEN `func_8014D65C` is matched with `if (!g) {}` (FAKE, T-10630: the empty pointer test keeps the GameState
base in `v1` in later cases; the test itself is eliminated). That FAKE works by giving `lda D_800E6280` the very
compare unit the rule removes; in the regress runs it is the only frequency-1 site in matched code where the rule
changes bytes. Under the rule the test is inert and the base moves to `v0` (13 edits). Measured respellings under the
model: `if ((s32)g < 0) {}` (an ordered test keeps its unit) is identical to the build; `if (g->unk_110A) {}` and no
test are not (28 differing lines each). `src-bunkaken.patch` uses the ordered test, still marked FAKE. It is a
FAKE tuned to the rule's exact scope (ordered compares keep their unit), which a reviewer should weigh: it shows
the function stays matched, not what the original source was.

## Mechanism

IDO 5.3's uopt counts the address constant of an `==`/`!=` as a register-candidate use (a live unit) in every
block. The original compiler did not, outside loops. In this function that one unit is what lifts the
D_8014984C and D_8014981C bases above the 0x44 stride constant; without it the allocation, the operand order and
every instruction equal the original. Why the original behaved so is not documented: `ldainreg` takes a memtype
and an offset that 5.3 ignores, which suggests the predicate was once more selective, but that is an inference.
IDO 7.1's uopt has the 5.3 behaviour (identical object). IDO 5.2 and 4.1 uopts matched 5.3 on all 2564 functions
of T-3110; the census here finds frequency-1 compare
units in so few matched functions (and no function where they change the code except one FAKE) that this does
not tell whether either version has the rule (UNVERIFIED).

## Proposal

1. The evidence is one positive function (two compares), zero contradictions among the matched functions and
   in the original-code scan, and one FAKE that has to be respelled. That is enough for a deterministic rule but a
   thin base for "broad sample". Two options for the user, both 0 rows:
   a. adopt the pass: apply `build-integration.patch` (rebuilds the shared image), `src-bunkaken.patch` and
      `src-taiiku.patch`, add the rule to `wiki/toolchain.md` and `wiki/matching-notes.md`, rerun the full build;
   b. the T-25309 config route (two zero-offset alias names in `config/symbols_TAIIKU.txt`), which changes no
      toolchain and no other function but invents two symbol names.
2. Decisive check before (a), if the user allows it under the licensing rule: run the natural body through the
   IDO 5.2 and 4.1 uopts with the T-3110 harness (`tools/ido_eval.py`). If either matches, the rule is a real
   compiler behaviour and (a) is the honest model.
3. Open: whether the rule also holds for ordered compares (`<`, `<=`) on addresses and for integer constants in
   `==`/`!=` outside loops; neither was needed here or measured.

## Log of this pass (2026-10-23, 16:21 to about 17:45 UTC)

1. Read the T-25106/T-25201/T-25309 notes; baseline guard 4 rows (insns 119, 263).
2. Read `makelivranges` / `ldainreg` / `constinreg` in the reconstruction and the 5.3 recomp; built `same`,
   `cmplda`: natural body MATCH with `cmplda`.
3. Original-code scan (`lda_compare_scan.py`), three refinements (labels, delay slots, loops).
4. `cmplda` regress (partial): pointer-loop counterexamples, BUNKAKEN FAKE. Built `cmplda_loop`, `log`,
   `loop_log`; natural body MATCH with the loop condition too; ETC func_8013B98C natural spelling
   (`&D_8014FAFC[8]`) still differs under the rule (ugen rebuilds the address from the base register in the
   fall-through block), so its separate symbol stays.
5. `cmplda_loop` regress (partial, 147 objects), BUNKAKEN respellings, full build with the rule: 27/27.
6. `tools/uopt_model.py` + tests; its binary MATCH; IDO 7.1 uopt check; restored the tree.
