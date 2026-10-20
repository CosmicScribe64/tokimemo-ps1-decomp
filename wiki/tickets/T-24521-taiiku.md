---
id: T-24521
title: Match remaining TAIIKU functions
status: In Review
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
