# T-20810 viewsweep first use

Output of `tools/viewsweep.py` (return-type and callee-view sweep, `wiki/decompile-workflow.md`, "Step 1 for a near miss")
over the 101 guarded drafts of the tree and 31 archived draft bodies of functions that were restored to `INCLUDE_ASM`.
Source-only: function names, row counts and edit strings, no asm and no game data. Nothing here is a verified match
except what the patch says.

- `viewsweep-results.jsonl`: one line per target. `set` is `guarded` or `restores`; `status` is `match`, `improved`,
  `no-gain`, `failed` (the unedited unit does not compile for a reason no view change touches, mostly an archived draft
  that needs declarations it does not carry) or `error`; `baseline` and `best_rows` are funcdiff rows of the function
  in the full unit (`null` baseline: the unit does not compile as it is); `best` are the edits of the best variant
  (`s32 f();` style declarations, applied to the preprocessed unit in memory); `untested` lists variants whose compile
  failed twice, with the compiler's first `Error:` line; `own_edits` and `callee_edits` are the lattice that was tried.
- `viewsweep-table.txt`: the same, ranked by best rows.
- `patches/NAME_ENT_func_80134B94.patch`: the one match, as a patch against `w2-good`. Verified in this worktree with
  the patch applied: `ninja` up to date with every unit sha1 OK (NAME_ENT.bin OK), `funcdiff.py --resolve
  NAME_ENT:func_80134B94` MATCH (804 bytes, literal bytes), `check_headers`, `migrate_globals --check`,
  `sync_protos --check-branch` OK. It is not applied to `src/` because the ticket's rules forbid editing `src/`;
  apply with `git apply drafts/T-20810/patches/NAME_ENT_func_80134B94.patch` and re-run the same checks.

Reproduce (repo root, Docker): `tools/docker.sh python3 tools/viewsweep.py --guarded -j 2 --json build/vs` and
`--targets LIST -j 2` with `UNIT:func drafts/T-NNNN/UNIT/func/body.c` lines.
