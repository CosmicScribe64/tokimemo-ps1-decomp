---
id: T-10840
title: Wave 13-15 tool fixes
status: Done
assignee: r16-fixes
created: 2026-10-12
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[build-system]]"]
---

## Goal

Fix guarded-draft comparison and false MATCH verdicts, newly defined main prototypes,
permuter seed tolerance, and regdiff file-scope data handling. Edit tools/tests/wiki only.

## Acceptance criteria

- [x] Guarded C is compiled on request/default and the compared version is identified.
- [x] MATCH requires exact compiled function bytes; stale/racing/normalized comparisons fail closed.
- [x] --write discovers main C definitions absent from headers.
- [x] Permuter accepts a configurable instruction-count tolerance and warns.
- [x] regdiff supports file-scope data or provides a precise limitation diagnostic.
- [x] Each fix has synthetic regression tests; Docker checks and standards review recorded.

## Notes

Shared brief read from scratchpad/tooling16-brief.md. Changes stay uncommitted for orchestrator review.

## Comments

2026-10-12: Read wiki index, relevant toolchain/workflow and wave notes, local agent setup,
and coding standards. Investigation started.

2026-10-12: Implementation complete, In Review. Normal builds still match all 27
unit checksums after deleting/regenerating asm and build. Header/global markers
exist, branch guard reports zero new disagreements (17 known), and progress is
5186/6958 functions, 966748/2279368 bytes. First broad Docker suite passed 743
tests (two baseline skips); final tests include additional review regressions.

Diagnosis: guarded C was deterministically bypassed by the production build.
Normalized string/table keys can agree without byte identity; default objdump
folds zero runs, and local symbol headers can truncate a function. Freshness was
cached and expected object writers shared outputs. No timeout success path exists
in funcdiff (synchronous subprocess return codes were already checked). The
historical TT loaded-run failure has insufficient retained evidence to distinguish
an actual race from fallback/normalization; no precise historical cause is claimed.

Remedy: select guarded C by default (`--version default` inspects production),
retain full-file context, freeze objects, serialize/atomically publish references,
hash source/header inputs, disassemble all zero words under ELF function bounds,
show both positional swap words, and privately link the exact snapshot against
the immutable disc's function span before MATCH. Scratch proof rejects section
placement and ELF absolute values that disagree with address-name conventions.

Review: local code-review skill's Standards axis found no breaches. Spec review
caught scratch own-data/absolute-symbol cases; both now fail closed with synthetic
regressions. Inline standards checklist confirms Python/Docker-only tooling,
ordinary source/flags unchanged, no compiler emulation pass, no raw-source edits,
no copyrighted fixture data, and edits limited to tools/tests/wiki. Final review
and test checkpoint pending before Done. Logs are in external scratchpad/r16-fixes.

2026-10-13: Final review complete and ticket Done. Both Standards and Spec axes
have zero unresolved findings; all review findings were fixed and retested.
Private-type checks include source header closures and preserve const pointer
types; the final prototype suite passes 75/75 and header suite 16/16. The full
Docker suite passed 748 tests (two baseline skips) before the last two focused
prototype regressions; those additional tests pass in the final 75-test suite.
Final Ninja and branch guard pass; clean-build proof remains 27/27 and progress
5186/6958. Literal proof passes main func_8007BBE8 (276 bytes), default TAIIKU
func_801368B8 (312 bytes) and OPTION func_8013C52C (512 bytes, jump table).
Guarded TAIIKU func_801368B8 displays both swapped lui/sw rows; func_80137228
reports its actual C DIFF. TT func_80149B20 is currently asm, explicitly reported
as default and literal-verified; the missing historical draft/race is not recreated.

Changed tools: funcdiff.py, new funcdiff_verify.py, sync_protos.py, permute.py,
regdiff.py and their tests (new test_funcdiff_verify.py). Wiki records/options
updated and linted; source, includes, config and raw inputs unchanged. No game
function gains. All edits remain uncommitted for the orchestrator. Validation
logs: scratchpad/r16-fixes/final-tests.log, protos-final-tests.log,
headers-final-tests.log, final-checks.log, guarded-final.log, literal-final.log.
