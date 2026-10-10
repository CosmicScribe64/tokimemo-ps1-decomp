---
id: T-9030
title: Wave-5 tool fixes
status: Done
assignee: r5-fixes
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-7030-tooling-wave-4-bug-fixes]]"]
---

## Goal
Fix the tool bugs found in wave 5: dupes/neardupes editing every unit, sync_protos --fix duplicating and reordering, check_headers missing a duplicate and an alias, funcdiff MATCH on failing headers, queue.py leading pad, permute.py jump-table verification.

## Acceptance criteria
- [x] Each fix has a unit test; all tools/test_*.py pass
- [x] Clean build 27/27 OK, headers OK, progress 4227/6958 (unchanged), `sync_protos.py --check-branch` OK

## Notes
Worktree r5-fixes (branch r5-fixes), not merged.

## Comments

### Result (2026-10-10)
1. `dupes.py` / `neardupes.py --files LIST` (queue.py syntax: address stems / overlay names, prefix, case-insensitive) restrict the plans, and so the edited C files, to the listed ones. Default stays the whole tree.
2. `sync_protos.py --write/--fix`: a declaration found among the typedefs or in a foreign block of `main_api.h` is moved into its section, never repeated (`drop_declared`); existing declarations keep their order, new ones go in by address (`stable_order`); `fix_header` no longer collapses blank lines in headers it did not change; `--only HEADERS` limits `--fix` to the named headers. `--fix` on the current tree changes nothing.
3. `check_headers.py` (via `sync_protos.check_api`): "duplicate X: include/main_api.h declares it N times" with the fix, and the alias conflict (strcat vs func_800AE100) unless listed in `config/proto_known.txt`; `--check-branch` no longer prints it twice.
4. `funcdiff.py` runs `ninja build/headers.ok` (or `check_headers` without a build) and prints ERROR instead of MATCH when it fails. lb against lbu: could not reproduce a hidden difference (the instruction word is in every compared row, with and without `--resolve`); a test now pins that.
5. `queue.py`: a function whose first instruction is a nop gets `P` (blocked, P(match) 0): DATE `func_80132000` shows `SP`.
6. `permute.py` verification: INCLUDE_RODATA lines inside a replaced NON_MATCHING block stay; a funcdiff DIFF that only involves `jtbl_`/`.rodata` is judged by the unit sha1 instead of being rejected or trusted.
