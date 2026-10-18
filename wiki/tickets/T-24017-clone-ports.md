---
id: T-24017
title: Port three exact opcode clones
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: []
---

## Goal

Port EVENT func_80108B24 and GEKO func_8013FD88 (476 bytes each) from matched ENDING func_80139B48, and SHUGAKU func_80133038 (176 bytes) from matched DATE func_80149F48 / GEKO func_80137580. Exclusive source files: src/ovl/EVENT/80107FC0.c, src/ovl/GEKO/8013F360.c, src/ovl/SHUGAKU/80132000.c. Base 0f2b9818f8eff7c5c1ccb6a2f306a87ccb120444.

## Acceptance criteria

Unguarded C with no target INCLUDE_ASM, default linked literal proof and affected overlay checksum for every win. Docker tools, one compiler/ninja -j1, no toolchain exceptions, no push/merge. Independent standards review before Done.

## Notes

Read wiki/index, prior negative evidence and matched sibling recipes from T-20930, T-17205 and T-22010. Twenty-minute bound. Root fresh census build/complete-siblings.json.

## Comments

## GEKO checkpoint

func_8013FD88 matches 476 default linked literal bytes as unguarded C; no target INCLUDE_ASM. GEKO SHA1 check passes after ninja -j1. Exact ENDING SDK polygon sibling port with inherited FAKE masks/grouped colour trios, no new toolchain behavior. See [[geko-family-results]].

## SHUGAKU checkpoint

func_80133038 matches 176 default linked literal bytes as unguarded C, old guard/fallback removed. Exact DATE copy-chain recipe, no data-island or header changes and no FAKE. SHUGAKU SHA1 passes after ninja -j1; four pre-existing pointer warnings outside target remain. See [[shugaku-family-results]]. Total checkpoint 2 of 3 / 652 bytes.

## EVENT checkpoint

func_80108B24 matches 476 default linked literal bytes as unguarded C, no target INCLUDE_ASM. EVENT SHA1 passes after 22 header-dependent build steps. Legacy helper/global names retained, buffer stride64; shared main_api declarations and unchanged-layout EventStateView prefix fields coordinated with root. Inherited FAKE colour masks/grouped trios only. See [[event-family-results]] and [[data-types]]. All three targets now match: 1128 new verified bytes. Root will run all27 shared-header integration checks.

## Final result and verification

3 of 3 matched, **1128 new literal linked C bytes**. Each target is an unguarded C definition with no target INCLUDE_ASM fallback; the root-supplied source-only claim gate passes all three.

| Unit / target | Bytes | Win commit | Unit SHA1 |
| --- | ---: | --- | --- |
| GEKO func_8013FD88 | 476 | eed41842fd562ae60d58e73f2c1eebda273da6d8 | fa37853a9bd8df628c6313b0a4af8ec7568482cd |
| SHUGAKU func_80133038 | 176 | be6764a6ebf1480543c4157af3da7216906ad09c | e9c64e952a1ef5dfea5e9b3e6a659aa6ccb8eb8c |
| EVENT func_80108B24 | 476 | 9e32fc2daa85a8934790ae323fb1bfdedaee26e7 | ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e |

After the EVENT shared-header additions, serial Docker ninja -j1 rebuilt all current-header dependencies of GEKO and SHUGAKU (39 steps); EVENT was already rebuilt (22 steps). All three full unit SHA1 checks pass. Fresh combined funcdiff --resolve --no-build --version default verifies 476 + 476 + 176 linked bytes. check_headers and migrate_globals --check pass; sync_protos --check-branch passes with 0 new disagreements, 15 known. Root coordinates the additional all27 shared-header integration pass.

### Review handoff

Self-review against CODING_STANDARDS.md: C89 only; SDK POLY_G4 reused, no duplicate SDK structs; target placeholders retained; inherited FAKE masks documented in both polygon ports; SHUGAKU product narrowing follows the already-reviewed DATE recipe without FAKE. Shared declarations are unique, legacy names remain unknown, coordinate pairs replace padding without changing later EventStateView offsets. Only the three assigned function bodies and required includes/forward declaration changed in source; no other function body changed. No toolchain changes, ASM edits, game assets, old archive edits, pushes or merges. In Review for independent parent review.

## Independent root review

Reviewed production source/header changes against CODING_STANDARDS.md: SDK types, fixed-width fields, shared declarations and explicit FAKE scheduling notes retained. No unresolved findings. Integration979a83671 passes all27 original binary hashes, headers/globals/prototype guards and target-INCLUDE_ASM rejection followed by default linked proofs: GEKO476, SHUGAKU176, EVENT476 bytes. Ticket Done.
