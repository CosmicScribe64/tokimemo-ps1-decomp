# T-24518 ETC initializer reconstruction checkpoint

Latest structural source: replay indexed-both-tables. Both six-entry tables
use the same natural index; strength reduction gives a separate first address
range inV0/boundA0, while second-loop index staysA0. The complete first table
region is EXACT:10 resolved words at function+0x288..0x2AC. Frame70/buffer54
and shade block order remain recovered. Candidate527 words/336 cost,
516/529 normalized positions; two missing opcode-count words and second-table
scheduling/register differences remain. The pointer local is now unused and
retains a scalar home for the diagnostic frame; its real source is unknown.

```
python3 drafts/T-24518/replay.py indexed-both-tables
python3 drafts/T-24518/probe.py build/T-24518/replay/indexed-both-tables.c
python3 drafts/T-24518/shape.py indexed-both-tables --prove-first
```

Known helper names are repaired from config/obin_renames.txt, including the
void load_tpage_buf_lock rather than undeclared old func80048514. Renaming
alone does not recover the loop; the indexed source is causal. A distinct
first scratch reused for shade/alpha still unrolls(537/398), rejecting that
model. Signed explicit second-table peel folds away the target's runtime
induction(522/373). Unsigned index preserves a different second schedule but
changes first SLT toSLTU(527/400); a single unsigned second-stride spelling
is neutral336, so no further width grid follows.

MASTER preceding-callee return-view recipe was tested specifically on case13.
Call-site int function-pointer cast emitsJALR, rejected. Direct s32 view and
definition of func80140AE4 requires `--callee-int func_80140AE4` with replay
rgb-callee-direct; it preserves the matched callee's56 words at0 cost but
does not close the case13 hoist (528/341, extra word is another load-delay
NOP). The T24207 matched do/while call-block transfer is byte-neutral352.
Natural global alpha preincrement with int/no-return replaces duplicate
ANDI by an unwantedV0 copy(529/392). No production return-type/header edit.

Private full-function source experiments only. No file here is included in the
production build. Base c768eda884eb30dd0067509f90d8892dc2563ef1. Ticket remains
In Progress; there is no match or coverage claim.

The3FFAC baseline and best signed-flag.c are stored in full. The later EN_NICHI
layout check retains en-baseline.c. Other names below
refer to exact replay controls in controls.json. Each stores compact line edits
against baseline plus its expected SHA256. Full active evidence also remains in
ignored build/T-24518/working-evidence. Recreate any control before probing it:

```
python3 drafts/T-24518/replay.py shared-signed field-counter shade-elements
python3 drafts/T-24518/probe.py build/T-24518/replay/field-counter.c
```

`replay.py --all` verifies every retained source hash without recompilation.

Run from this worktree in the existing tokimemo-decomp-arm64 Docker image,
mounting this tree at /work and original root disc at /work/disc:ro:

```
python3 drafts/T-24518/probe.py drafts/T-24518/signed-flag.c
python3 drafts/T-24518/shape.py signed-flag
```

Best resolved instruction cost: signed-flag.c, 529 target/529 candidate words,
190 cost. The initial baseline had538 candidate words/453 cost. Printed diff row
counts include disassembly text and are not the scoring metric. Probe checks
resolved words/relocations from the actual translation unit through the normal
stock pipeline; it does not edit production source or headers. Return-type
controls use a private copied header. Build outputs are ignored.

Recovered source distinctions:

- shared-signed.c: one signed induction variable reused by the two table loops
  retains the first loop and produces the original second unroll, frame0x70,
  buffer0x54.530 words/447 cost. Independent address variables, pointer bounds,
  or reusing only a copied address do not reproduce this distinction.
- field-counter.c: coherent named-field scalar byte view enables state
  promotion across unrelated GameState stores.530 words/351 cost. Raw byte
  offset byte-counter.c is equivalent and superseded by the named-field form.
- signed-flag.c: consistent signed store of01000000 to the first sprite flag
  lets it share the second slot's signed flag constant.529 words/190 cost.
  This is a measured private view; main_api.h remains untouched.
- shade-elements.c: a full element-store chain shares the128 shade constant,
  unlike scalar chains or individual element statements.527 words/352 cost,
  wrong color-store order. The real SpriteSlot24 member/pointer controls are
  recorded as sprite-colors.c/color-pointer.c; neither recovers original order.
- shade-return.c: s32/no-return liveness fixes the final default delay-slot
  hoist but adds a case9 narrowing;529 words/316 cost. Not selected as best.

Other bounded controls remain as source for reproducibility. The ticket records
their causal hypotheses and results. No grids or production compiler changes.

| Control | Candidate words / resolved cost | Causal question |
| --- | --- | --- |
| baseline | 538 / 453 | Prior six-entry semantic repair |
| address-cursor, pointer-bound | 538 / 453 | Does pointer spelling retain first loop? No |
| shared-counter | 530 / 449 | Shared induction retains first loop |
| shared-signed | 530 / 447 | Signed shared induction gives buffer0x54 |
| address-word | 538 / 449 | Unsigned type alone is insufficient |
| shared-address | 538 / 447 | Reusing copied expression is insufficient |
| shared-pointer | 530 / 447 | Pointer store canonicalizes to shared-signed |
| state-flow | 531 / 428 | Explicit state copies clarify flow, add copy |
| shade-lifetime | 530 / 469 | Reusing the alpha temporary is worse |
| field-counter, byte-counter | 530 / 351 | Coherent byte view recovers promotion |
| field-girl, signed-shade, chained-values, shade-chain | 530 / 351 | No improvement |
| field-alpha | 530 / 427 | Fade scalar view worsens allocation |
| fade-mask | 530 / 439 | Explicit masks do not retain ANDI |
| return-live | 532 / 355 | Return liveness alone adds instructions |
| signed-flag, sprite-colors, shade-walk, shade-value-flow | 529 / 190 | Signed constant sharing; other controls neutral |
| shade-elements | 527 / 352 | Complete element chain shares128, wrong store order |
| shade-return | 529 / 316 | Return liveness fixes hoist, adds case9 narrowing |
| color-pointer | 527 / 360 | Real color pointer does not fix store order/frame |
| shared-value | 516 / 491 | Reusing loop counter for shade incorrectly retains second loop |
| rgb-loop | 516 / 507 | Third component loop changes second-loop structure |
| fade-copy | 529 / 302 | Matched main fade snapshot does not retain ANDI |
| final-flow, table-line, shade-byte-value | 529 / 190 | Direct final predicates, single-line table loop and isolated byte shade local are neutral |

Later scope extension checked actual closest owned guards. ETC4790C's old44-row
body now reproduces282 target/279 candidate words,85 cost. A current callee's
void forward declaration and current frame field name are needed to compile.
The MAIN767A4 separated-slot recipe (read-only T-24501 source, coordinator
reports932 literal bytes, checks pending) gives a single measured layout:
three upper words plus256-byte buffer instead of0x10C. It is byte-neutral.
Matched ETC43758's typed DisplayActor44 walk is also neutral. Replays
actor-baseline/actor-slots/actor-typed use the committed T-22029 body as their
base, so another duplicate full body is unnecessary. No slot grid follows.

EN_NICHI36DEC baseline repeats348 target/347 candidate words,317 cost. Original
three44-byte tables beginC4/98/6C, byte table54; top table endsF0 in frameF8.
Two upper FAKE scalar slots recover frameF8, improving only four diagnostics to
313. Candidate still lacks original saved kind cursor (originalS0), saves only
two registers versus three, and has broad table/loop differences. Existing
T-24516 byte-cursor control was read and not repeated. Recreate en-slots with
`--scope EN_NICHI:func_80136DEC`; actor controls use `--scope ETC:func_8014790C`.

The next structural source, replay shade-rgb-chain, recovers the entire
case5 shade block's opcode and store order: named element chain
E0=E1=E2=128 produces E1/E0/GSzero/E2 in the call slot, as predicted from
the observed innermost-store rotation. One causal order prediction, no grid.
It has527 words/352 cost and515/529 normalized positions. Keep it alongside
the190-cost signed-flag baseline: relative branch displacements penalize the
two missing words. The only remaining opcode-count gaps are case8 ANDIff
and the case13 flag-load delay NOP replaced by a hoisted D801500D8 LUI.
The latter is case13, not the final state/default as earlier descriptions
loosely called it. s32/no-return blocks that hoist but produces a duplicate
case9 alpha mask; postfix increment is neutral and a word alpha local still
duplicates the mask (rgb-return-post/word,529 words316/314 cost). Its518
normalized positions also include a substituted mask, not a match.

Exact stock/trace fidelity additionally passes signed-flag and en-slots.
EN cursor trace assigns kind induction expression bit46 to caller v1
(priority15.5), index i to saved s0 (16.33), constant1000 to saved s1.
Original has kindS0/indexS1/constantS2. This is source/range attribution,
not proof that the backend cannot produce the original. The historical
func80048F64 int-return view on en-slots worsens313 to443 cost; discarded.
Natural postincrement kind cursor gives453, confirming the old end-increment
control's negative result, so no cursor or width grid follows.

Remaining opcode/flow gaps in signed-flag.c: shared128 materialization and
store order, case8 ANDIff, final default NOP/LUI scheduling. The two table loops
have equivalent shapes but differ in materialization/store scheduling. Other
differences are live-value allocation and temporary numbering. The shape tool
masks registers and branch displacements only to localize these regions; its
512/529 aligned positions are not matching evidence.

Existing read-only trace binary /private/tmp/t24047-uopt/variants/trace was
mounted as /scratch/variants/trace. On shared-signed and field-counter:

```
python3 drafts/T-24518/fidelity.py field-counter
python3 tools/regdiff.py ETC:func_8013FFAC --src build/T-24518/field-counter/variant.c --uopt /scratch/variants/trace
```

Stock/traced text, relocation and data sections and resolved target words agree
exactly. The direct GameState state member costs32 reload/save charges for
15 uses+17 definitions, explaining its failed promotion. This does not prove a
backend blocker; source reconstruction remains active.
