---
id: T-24521
title: Match remaining TAIIKU functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[families/taiiku-family-results]]"]
---

## Goal

Find verified literal-C matches among the eleven remaining TAIIKU functions in the isolated T-24521 tree. Coordinator T-24520 integrates pinned commits.

## Acceptance criteria

- Literal default C compiles and links byte-identically; all 27 binary checks and declaration guards pass.
- Standards review, FAKE explanations, and attribution recorded before completion.

## Notes

Baseline dba33c4f5. Docker tools only, worker compilers -j1. Prior T-11640/T-16551/T-16552/T-19004 negative evidence read before selecting hypotheses. No broad exhausted grids, toolchain patches, or coverage from source-only drafts.

## Comments

- 2026-10-20: Claimed isolated branch codex/t24521-taiiku under /private/tmp/tokimemo-t24521.

- Fresh typed loader `func_80134060` now compiles: 379 diagnostic rows initially; page construction after each handle lookup aligns the original a2 pointer spill and s0/s1 ranges; eliminating redundant page local and reusing the lookup/modulo local yields 146 rows, then a missing upper scalar slot plus separated first lookup yields 140. Equal instruction count, frame and table offsets now match. Remaining differences start at the repeated 0x80000206 constant's v1 allocation instead of t1, then the ugen ring. A chain assignment is being tested; no broad search or toolchain patch.

### Verified match: func_80134060 (1,348 bytes)

- Literal default C replaces its INCLUDE_ASM. Existing `TaiShorts12` tables, shared index local, lookup-before-handle calculation, and the 10-trip `!=` loop reproduce the structure.
- Two FAKE constructs are documented at the source: unused upper word fixes table homes; unsigned first page-table store keeps the repeated handle constant in a ugen temporary and aligns the entire ring.
- Docker `ninja -j1 build/ovl/TAIIKU.ok`: TAIIKU SHA1 `306d1013b9be465819525d1fb4dc0e86b422a846` OK. `funcdiff.py --resolve --version default TAIIKU:func_80134060`: MATCH, privately linked 1,348 bytes equal target.
- 27/27 binary SHA1 checks: private TAIIKU output and 26 unchanged root outputs mounted read-only at `/baseline-build`; root independently verifies integration. No claim that this lane rebuilt those other 26 outputs.
- `check_headers.py`: OK. `sync_protos.py --check-branch`: OK, 0 new disagreements, 12 known. Initial `migrate_globals.py --check` required DATE/TACO original assembly not generated in this focused tree; generated only their split stamps and reran the guard.
- 2026-10-20: Standards self-review: C89, fixed-width types, existing aligned aggregates, canonical declarations, FAKE reasons, no raw data or generated assembly edits; no findings. Parent independently inspected the final source and found no blocking standards issue; private whole-overlay SHA1 confirms new void declarations preserve all callers. Lane remains In Progress for the other ten functions.

### Verified match: func_80134F98 (1,256 bytes)

- Literal default C, existing aggregates and natural index loops. Corrected D_801494C4 to `TaiWords11` from the original 44-byte copy. Added canonical void declarations for func_80138800/func_80138950.
- One FAKE upper unused scalar positions the counter spill; direct table expression avoids a redundant promoted range.
- Docker private `ninja -j1 build/ovl/TAIIKU.ok`: SHA1 `306d1013b9be465819525d1fb4dc0e86b422a846` OK. `funcdiff.py --resolve --version default TAIIKU:func_80134F98`: MATCH, privately linked 1,256 bytes equal target. Private TAIIKU plus 26 unchanged read-only root binary outputs: 27/27 hashes OK.
- Header guard OK, globals guard OK, branch prototype guard OK (0 new, 12 known). Standards self-review and independent parent final source review: no blocking finding. Header changes preserve all callers according to the private whole-overlay checksum.
- Lane cumulative: two functions, 2,604 matching bytes; nine remaining TAIIKU functions. No source-only draft earns coverage.

### Source evidence for func_80137C44

Three fresh typed runner/actor variants compile but do not match (241, 229, 223 diagnostic rows). Snapshots of four originally hoisted globals are sunk back by uopt; frame and saved-register allocation remain different. This does not improve T-19004's roughly100-row result. Preserved source bodies in `drafts/T-24521/TAIIKU/func_80137C44/`; production remains INCLUDE_ASM and coverage stays at two verified matches. No exhausted allocation grids rerun.

### Source evidence for func_80135F3C

Fresh four-aggregate and nested dedup-loop body compiles with 543 diagnostic rows. Frame288 vs target296 and one fewer saved register; tables16 bytes low. Also identified a missing rejected-player19 store in this draft; no semantic-equivalence claim. Preserved under `drafts/T-24521/TAIIKU/func_80135F3C/`, restored production and header. No coverage gained.

### Source evidence for func_80139AB0

Explicit two-column source loop avoids the old full-unrolling problem but introduces induction strength reduction (frame224 vs target96,228 diagnostic rows). Narrow counters block that transform (frame88,194 rows) at the cost of real sign extensions and different allocations. Both compile; neither matches. Preserved under `drafts/T-24521/TAIIKU/func_80139AB0/`; production stays INCLUDE_ASM. Prefix scalar views are inherited experimental evidence, not suitable for production.

### Original bounds and ownership of D_801494C4

- Original `func_80134F98` at 0x80134FA8 computes source end = 0x801494C4 + 0x24. The copy loop at 0x80134FB0..0x80134FD0 copies three words per trip until that end, covering offsets 0x00..0x20 (nine words). Loads at 0x80134FD4 and 0x80134FD8 then read end+0 and end+4, offsets 0x24 and 0x28; stores at 0x80134FE0/0x80134FE4 complete the 44-byte copy.
- The original data piece owns eleven words at 0x801494C4..0x801494EC, with `enddlabel D_801494C4` after the word at +0x28. The next label is D_801494F0 = start+0x2C. The corrected type ends before the next symbol.
- `rg -n D_801494C4 src include` finds only its owning INCLUDE_RODATA line, this function's aggregate initializer, and the canonical header declaration. No other source function uses this table. Raw sources and generated assembly were read only.

### Checkpoint review

The final production source contains precisely the two verified replacements (2,604 bytes). The remaining nine functions retain INCLUDE_ASM. A final private overlay build and literal default linked proof of both functions pass after restoring every experiment. Source-only bodies and their reproduction limits are saved separately. Self-review found no staged raw data, generated assembly, compiler edits or unmarked production FAKEs; new README prose passed the repository deslop check. Parent review of the two production changes has no blockers; source-only archive review remains before ticket closure.

- Parent archive review: marked the inherited scalar/offset views in both `func_80139AB0` bodies with leading FAKE comments naming T-24521 and their experimental status. These bodies remain source-only evidence with unresolved aggregate fields. No production changes or compiler search. The remaining archive review found no blockers; exact 44-byte table ownership evidence confirmed independently.

### Focused comparison using the two new donors

Compared only original TAIIKU instruction streams for new donors `func_80134060` and `func_80134F98` against the nine remaining functions. Method: `tools/siblings.py` `read_words` and `op_token`, then `difflib.SequenceMatcher(None, donor_tokens, target_tokens, autojunk=False).ratio()`. This preserves the sibling tool's directional score and removes registers, constants and targets. All 18 results, instruction counts and longest block indices are saved in `drafts/T-24521/new-donor-rankings.json`.

| Remaining function | Closest new donor | Ratio |
| --- | --- | ---: |
| func_8013DDB8 | func_80134F98 | 0.2896 |
| func_80135F3C | func_80134F98 | 0.2374 |
| func_8013F3A4 | func_80134F98 | 0.2281 |
| func_80137C44 | func_80134F98 | 0.2227 |
| func_8013E894 | func_80134F98 | 0.2154 |
| func_80139AB0 | func_80134060 | 0.2074 |
| func_8013EC60 | func_80134060 | 0.1862 |
| func_8013E1FC | func_80134060 | 0.1686 |
| func_80137784 | func_80134F98 | 0.1650 |

Inspected the strongest instruction blocks and the matched source templates:

- DDB8's best seven-instruction block is generic jump-table dispatch. Its five-instruction addition run computes grid coordinates, whereas the donor increments character award fields. The source operations differ.
- 35F3C shares the donor's ten-instruction 44-byte aggregate-copy prefix. The earlier saved reconstruction already uses the same `TaiWords11` assignment; remaining differences concern nested selection loops and allocation. The 34060 pairing (0.2222) also finds small aggregate copies, but its 18-byte tables differ from this target's 22-byte tables. Neither adds a new source hypothesis.
- F3A4 shares the generic nine-instruction seven-way dispatch plus a five-instruction signed threshold/add sequence. Its actor update arms do not follow the donor's award loops or loader handle loops.

Every pairing is below 0.30 and below `siblings.py`'s default 0.70 report threshold. No new structural transplant was justified; no compiler or build ran in this focused pass. Production remains the same two verified matches, with nine assembly tails. Returned to In Review for parent review of this evidence.

### Fresh geometry reconstruction of func_8013E894

Read indexed T-11010 and T-11640 history before starting; neither attempted this routine in depth. Original four `Rot_2D` outputs establish two separate coordinate arrays: x at sp+0xAC/0xB0/0xB4/0xB8 and y at sp+0x9C/0xA0/0xA4/0xA8. Scalar rotation inputs are sp+0x94 and sp+0x90. The routine rotates two radii at adjacent angles, translates all four points, scales y, warps two endpoint pairs about radial midpoints, negates/offsets coordinates, and clips/draws the two lines. Existing `TaiikuCamera0C` and `TaiikuBall` own every global access.

Fresh real-array source compiles at 350 diagnostic rows, with a 192-byte frame versus target 200. Camera snapshots, the original early pair-counter advancement, and conditional y reads reduce this to 341 rows and a matching 200-byte frame. A two-dimensional endpoint-pair view scores 337 rows with frame 192; separate row/count lifetimes retain unit increments but leave 368 rows (flat) or 364 (2D). These are typed structural probes, not scalar declaration grids.

Earliest remaining ownership difference: the target keeps x base in s3, y base in s2, plus distinct translation/scale/y-offset limits in s5/s6/fp that are all physically equal to x base. The explicit pointer source correctly spells bounds from each own array but uopt merges equal-valued ranges, leaving a register for the angle. Target angle spills at sp+0x78. Next hypothesis tests natural array-index induction through the five stages, sharing the real counter between loops, to let uopt create independent stage-limit ranges. No new coverage yet.

Natural array-index stage loops reproduce the distinct equal-valued bounds and spill the angle. Splitting real first-endpoint x/y temporaries and placing the angle below the arrays gives the exact observed 200-byte frame and every known home (x 0xAC, y 0x9C, rotation inputs 0x94/0x90, angle 0x78), with no padding. This candidate is 245 instructions versus target 243 and reports 224 diagnostic rows. Named axis unions, a direct typed camera view, initial row copy spelling, own implicit-int return and an ephemeral implicit-int `Rot_2D` view do not change its bytes; the experimental shared-header view was restored.

Reused the existing `/private/tmp/t24047-uopt/variants/trace` read-only after checking stock/traced full-TU `.text`, `.rel.text`, `.rodata`, `.rel.rodata`, `.data`, `.rel.data` inventories and bytes: all equal. First divergence at instruction34 is a ugen temporary, not a traced uopt range. First structural difference at106 is an extra `move a0,v0`. A shared `ixa(x, i*4)` range across three x stages has priority476.7 and takes v0, outranking the camera x field at100 (v1); the original has those roles reversed. Pair row is a separate priority140 v0 range and displaces the original first-y allocation. A stage-owned index hypothesis is being tested to split the shared address range naturally. No compiler patches or tracing rebuilds were made.


### Geometry checkpoint and restricted MAIN donor pass

Stage-owned indices regressed E894 to 307 diagnostic rows; post-increment endpoint induction gave 352. The best real-array reconstruction remains 224 rows with exact 200-byte frame/homes and 245 instructions versus 243. Saved as `drafts/T-24521/TAIIKU/func_8013E894/typed-arrays.c`. Its first structural divergence and faithful trace evidence are recorded above. No frame padding or artificial bound objects were added.

Reconstructed E1FC from its original two phases: 64 straight strips followed by 64 rotated strips. The straight phase translates four x values and two y values before duplicating y endpoints; both phases share the real endpoint warp/clip operation. An explicit next-row lifetime gives the best 548 diagnostic rows, frame 208 versus target 200, with correct x/y array homes but next-angle home 0x74 versus target 0x60. Replacing the explicit next angle with derived `angle + 16` induction gives 606 rows at frame 208; moving its loop bound to that expression leaves bytes unchanged. Best body saved as `drafts/T-24521/TAIIKU/func_8013E1FC/typed-arrays.c`. Neither body matches; semantic equivalence is not proved and coverage remains unchanged.

At coordinator request, compared only the newly solved MAIN donors func_80076000 (299 instructions) and func_800767A4 (233) against the nine TAIIKU tails using the same original-opcode normalization and directional SequenceMatcher method above. All 18 pairs are in `drafts/T-24521/main-donor-rankings.json`. The strongest are 767A4→35F3C 0.2699, 76000→35F3C 0.2396 and 767A4→E894 0.2143. The first pairing's longest blocks (12 and 11 instructions) are signed-division exception guards. The second shares at most five generic address/load instructions; the third shares at most four call-setup instructions. No source-template relationship or fresh compile hypothesis follows, so no compiler ran for this pass. Earlier donor pairs were not repeated.

Restored production to the two verified matches; all nine remaining functions retain their prior assembly fallbacks. The source-only geometry archive uses existing owned aggregate views and real coordinate arrays; no experimental header change remains. Parent review of this bounded checkpoint is pending; the ticket stays In Progress under the coordinator's explicit active-work instruction.

Focused inspection of the three remaining candidates identifies EC60 as a genuine structural sibling of E1FC: both have straight and rotated strip phases, x/y arrays at 0xAC/0x9C, 200-byte target frames, and the same midpoint/warp/clip body. EC60 starts straight x coordinates at +320/+240, duplicates both y endpoints before rotating all four points by camera angle, then translates/scales them. This provides an untried source transplant hypothesis from the new real-array reconstruction, although E1FC itself is still unmatched. DDB8's grid-loop unrolling and F3A4's actor-state dispatch do not share this geometry body. No new compiler ran for this inspection.

Archive verification: Docker `ninja -j1 build/ovl/TAIIKU.ok` passes the original SHA1; both literal default linked matches still pass (1,348 and 1,256 bytes). Private TAIIKU plus 26 unchanged read-only root binaries passes 27/27 SHA1 checks. Header/global guards pass; branch prototype check passes with zero new and twelve known disagreements. README deslop scan reports zero hits. Standards self-review finds C89 source-only bodies, real array bounds and owned aggregate fields, no new padding/FAKE constructs, no production/header/compiler changes, and no raw game assets. Parent archive review pending. Local proof: `/private/tmp/t24521-geometry-checkpoint-proof.log`.

Parent reviewed both geometry bodies and their README after the checkpoint: no blockers. Confirmed real arrays, canonical existing camera/ball types, no padding/new type/assembly, ordinary C89 declarations, explicit source-only and semantic limits, and measured homes/trace fidelity. Authorized one EC60 sibling transplant diagnostic compile; no broader search if residual differences are broad.


### Single EC60 geometry transplant result

The one authorized EC60 reconstruction compiles with 489 diagnostic rows and 458 instructions versus target 447. Frame is 216 versus 200, x/y homes are 0xBC/0xAC versus 0xAC/0x9C, and loop state spills at 0x88 versus 0x78. First loop induction already differs at the prologue (constant 8 rather than zero/current-plus-one), so this is a broad mismatch. Saved body under `drafts/T-24521/TAIIKU/func_8013EC60/typed-arrays.c`; semantic equivalence remains unproved. Restored production immediately. No second probe, declaration grid, or fresh deep pipeline. Ticket stays In Progress per explicit coordinator instruction; parent review of this additional archive remains pending.

After restoring EC60's assembly fallback, private whole-overlay SHA1, both literal default linked matches and 27/27 hashes pass again. Headers/globals pass and prototype branch check remains zero new/twelve known. No production/header diff remains. Standards self-review finds C89, real arrays and canonical actor/camera/ball views, no padding or new type; actor-102 field ownership follows 0x8012080E = 0x8011ECD0 + 102*0x44 + 0x26. README deslop's single weak colon hit introduces an explicit list of source changes and was retained. Proof: `/private/tmp/t24521-EC60-restored-proof.log`.

Parent independently reviewed the EC60 source-only body and README: no blockers. Confirmed C89 real arrays/locals, existing camera and actor-102 field ownership, no dummy homes/new header/assembly, explicit unproved semantic limits, and the documented 489-row/216-byte-frame residual. No more EC60 compiles; ticket remains In Progress for coordinator reassignment.


### Owned runner transition func_80137784

Read the draft catalog before reconstructing this 1,216-byte routine. T-14500 has one autogenerated 493-row draft; T-16550 fold/fold+fixes has 448 rows, raw actor offsets and a wrong clear-loop endpoint (+0xCC actor records instead of three records). No earlier owned reconstruction was found. Same-unit matched func_80137EA8 supplies canonical actor fields; func_80138680 supplies the explicitly FAKE shared player/cpu view, and caller func_80136EC0 passes CPU index+1.

Fresh source uses existing TaiRunnerA, DisplayFixActor44 and Fix32 types, removes scalar-offset walks, and fixes the clear loop to three records (actors 104..106). Four original data pieces D_8014981C/2C/3C/4C each own exactly 16 bytes. Original indexing shifts slot by two, establishing four word slots; 4984C additionally reads signed halfword +2 of a selected word, represented by existing Fix32. No other source user appears in `rg D_801498[1234]C src include` apart from owning data lines and this probe's declarations. Replaced inherited D_8014929A alias with canonical D_80149298.halves.high before the first real diagnostic.

The single corrected typed foundation compiles with 396 diagnostic rows, 296 instructions versus 304, and a 120-byte frame versus 56. Original selection/slot indexing shares a 68 multiplier via multu; canonical pointer indexing produces shift/add and changes the prologue before the first conditional. The corrected clear loop rolls, but register/call-spill structure remains broad. This is no exact match or proved semantic equivalence; old raw-draft scores are not a valid semantic baseline. Saved corrected body and required header declarations in `drafts/T-24521/TAIIKU/func_80137784/`. Restored production and header; no declaration/view grid or further compiler probe justified. Ticket remains In Progress per coordinator instruction; parent archive review pending.

Restoration proof `/private/tmp/t24521-37784-restored-proof.log`: private TAIIKU SHA1, both literal default linked matches and 27/27 hashes pass. Headers/globals pass; prototype branch check remains zero new/twelve known. New README deslop scan has zero hits. No production/header diff remains. Standards self-review finds C89, existing typed fields and a documented inherited FAKE runner span; no new raw scalar views, padding, compiler edits or raw assets.
