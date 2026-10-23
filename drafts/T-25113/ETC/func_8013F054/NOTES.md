# T-25113: ETC func_8013F054 (src/ovl/ETC/8013BEF0.c), 716 bytes

Result: matched as default C (funcdiff `--version default` MATCH, ETC sha1 OK). No FAKE. The body is in `src/ovl/ETC/8013BEF0.c`.

## Start state

INCLUDE_ASM. Best known body: `drafts/T-24076/body.c`. Measured again here: 71 funcdiff rows (`--version c`). It uses a pointer/cursor do/while, scalar-address views for the final `unk_F5F` reads and a `u8 v` code local.

## Steps (one change each, measured)

1. **Index do/while instead of the pointer cursor** (71 to 31 rows). `i = idx; do { v = D_800E6280.unk_75E[i - 1].unk_00; i--; if (...) { a = unk_75E[i].unk_02; ... } } while (i >= 0);`. uopt strength-reduces `i`, and linear function test replacement builds the bound `&D_800E6280` in its own register (`$a2`). The base of the first address stays a ugen temporary (`$t3`). The pointer form CSEs the base and the bound into one register, and every later temporary is then one lower. The `unk_F5F` loads stay inside the loop. T-25012 said index forms hoist them, but that does not hold for this spelling. The whole tail after the join already matched at this step.
2. **Load at entry plus copy.** With `i = idx` in the preheader, uopt copy-propagates `idx = D_80150064` and sinks the load into each arm. `i = idx;` at the top keeps one entry load and the `move`, but the two colours are swapped (11 rows). regdiff `-v`: the promoted global D_80150064 (bit 2) and local `i` (bit 5) both have priority 0.667, and the lower bit is coloured first. `i = D_80150064;` with the global used directly, or `idx = i = D_80150064;`, gives `i` the lower bit: 2 rows left. Equal-priority negatives: changing the declaration order, moving the copy below the zero inits, moving the copy into the `unk_1124 == 0` arm (16 rows), `i = idx = ...` (11 rows).
3. **Loop compare operand order** (`bne a0,v1` against `bne v1,a0`, 2 rows). A named `v` local makes uopt order the variable first. A load expression puts the constant register first (the same order as at the `unk_F5F` compares). Writing `0xC == v` changes nothing. Moving `i--` to the top of the body and reading `unk_75E[i].unk_00` in each test, with no `v`, matches: uopt CSEs the byte and the found arm copies it (`move t1,v1`). `(v | 0) == 0xC` also matches, but it is a FAKE and was not used.
4. **Cleanup, each step measured.** A `GameState *g = &D_800E6280;` pointer used for every field keeps the two final `unk_F5F` reads separate. The plain `D_800E6280.unk_F5F` form CSEs them (19 rows). This replaces T-24076's FAKE scalar-address views. `u8 *w = (u8 *)&D_800E7D10;` with `[11]` in the album arm is the same form as func_8013E6AC. `i` replaces D_80150064 in the direct arm and in the guard, and copy propagation gives the same code. A `void` return does not compile: the callers in func_8013BFFC and func_8013C330 call the function undeclared (implicit int).

## Levers worth reusing

- When the original has a separate loop-invariant bound register next to a ugen base, write an index loop and let LFTR create the bound; a hand-written pointer loop CSEs the two.
- An equal-priority colour swap between a promoted global and its copy follows bit order, which is first appearance: assign the copy straight from the global.
- Compare operand order (register constant against value): a named local goes first, a load or CSE expression goes second.
