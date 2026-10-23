# T-25502: TAIIKU func_80137784, plain-C lens (wave 12)

Result: **MATCH with plain C, no new symbols, no config change.** The two pointer compares of the
original are not in the source. The source tests `arg0 == 0`, and uopt's test replacement turns each
test into a compare of the slot address (already in a register) with the array base. `match.c` is the
body now in `src/ovl/TAIIKU/80133C80.c` (no `NON_MATCHING` guard; the three T-25106 FAKEs for the
entry stay, the two D_8014985C compare FAKEs are gone).

Measured with `tools/docker.sh python3 tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version c`
(whole unit), then `--version default` and a clean rebuild (see the end of this file).

## The change

Start: the guard (`guard.c`, 4 rows): compares against the D_8014985C alias, constant-first order.

```c
        if (arg0 == 0) {                         /* was &D_8014984C[arg0] == (Fix32 *)&D_8014985C - 4 */
            D_80149208.player.unk_20.halves.high = -0xA0;
            D_8014929C = 0;
        }
...
        if (arg0 == 0 && D_80122E94 == 1) {      /* was &D_8014981C[arg0] == (s32 *)&D_8014985C - 16 && ... */
```

`t1-arg0-test.c` (the guard with these two lines changed): `MATCH (linked function bytes equal target (1216 bytes))`.

## Mechanism

- In both places `arg0` is dead after the test (block 1 returns; in block 3 the last use,
  `arg0 + 0x90`, is before the test). The slot address `&D_8014984C[arg0]` (block 1) and
  `&D_8014981C[arg0]` (block 3) is a CSE held in a register (a0, a1). uopt's linear-function test
  replacement (`uopttrep`) rewrites `arg0 == 0` as `addr == base + 0 * 4`, with the register first and a
  newly made base address that ugen materialises with `lui/addiu` at the compare. That is the
  original's `bne a0,t8` / `bne a1,t9`.
- The base made by the rewrite is not the same register candidate as the base of the address
  computation, so the bases of D_8014984C and D_8014981C no longer get a register (the priorities 0.5 of
  T-25106/T-25201 came only from the source-level compares), 0x44 takes t5 and D_80122E94 takes a2,
  as the earlier probe `if (arg0 == 0)` already showed for the colouring.
- Corpus precedent, found with `scan_base_compare.py` (original asm, functions where one array base
  is used as an `addu` operand and in a `bne/beq`): TAIIKU `func_80147A6C` (matched) has the same shape,
  `addu v0,t6,t7` (`&D_8014A454[index]`), fresh `lui/addiu t9`, `bne v0,t9`, and its C says
  `index != 0`. The scan finds 8 (function, symbol) pairs: 7 materialise the base again for the
  compare (the test-replacement shape; two are this function, the others are `func_80147A6C`,
  `func_80147B14`, `func_801482BC`, `func_8006F218` and, mixed, `func_8013F3A4`); one
  (`func_8013A9C8`, D_8011EFBC) compares against the register that holds the base.
- Why the earlier passes missed it: T-25106 measured `if (arg0 == 0)` only as a colouring probe ("0x44
  -> t5, D_80122E94 -> a2, as original") and read the compares as source-level pointer compares, so the
  remaining work went into the compare operand order.

## Measured negatives (this pass, all against the guard body)

| draft | rows | what happens |
|---|---|---|
| `p1-params.c` (K&R `func_80137784(arg0, pos, cnt)`, header `()`) | 228 | home stores `sw a1,60(sp); sw a2,64(sp)`; the compares are pointer-first, but both bases still take registers (t5, a2), 0x44 falls to ra |
| `p2-ansi-params.c` (ANSI 3 parameters, header `()`) | 228 | same as p1: IDO stores a dead parameter to its home |
| `p3-reg-params.c` (`register` parameters) | 228 | same |
| `natural.c` (`D_8014984C == &D_8014984C[arg0]`) | 226 | right order, bases allocated (wave 10 result, re-measured) |
| `r1-blockdecl-size1.c` (block-scope `extern Fix32 D_8014984C[1];`) | - | cfe error: redeclaration |
| `data.py static` + `guard.c` | 60 | data defined in C as statics: same code as the guard, rows are the unresolved `.data` relocations |
| `data.py static` + `guard-flip.c` (alias on the left) | 272 | uopt still moves the -16: `addiu t8,a0,16; bne t8,t9` |

`lda_dump.py` on `cc -K` output: every C spelling of a symbol gives the same `lda` key
(block, length = object size, offset 0); uopt's expression hash compares exactly those fields
(`uoptinput.c` Ulda case, `uoptcopy.c` ichain search). So no source-level compare against the same
symbol can be a separate candidate; the separate candidate in the original comes from test replacement.

## Proposal

- Keep the match (committed in this worktree). The T-25309 `alias-match.patch` (two config symbols)
  is no longer needed and should not be applied.
- Wiki (for the synthesis/merge step): add a cheatsheet row "pointer compare `bne aN,tM` of a CSE'd
  `&arr[x]` against a fresh `lui/addiu arr` where x is dead afterwards: write `x == 0` (straight-line
  test replacement, TAIIKU func_80147A6C, func_80137784)". Close the T-25201/T-25309 "second names"
  hypothesis in matching-notes.
- No tooling or config change.

## Verification (2026-10-23)

- `funcdiff --version default`: `MATCH (linked function bytes equal target (1216 bytes))`.
- `tools/docker.sh sh -c 'rm -rf build && python3 configure.py && ninja'`: 27 of 27 sha1 OK
  (main exe and 26 overlays), headers/globals/symbols stamps built.
- `check_headers.py`: headers OK; `migrate_globals.py --check`: globals OK;
  `sync_protos.py --check-branch`: branch check OK (0 new disagreements).
- `regdiff -v` on the match: identical; no hoisted slot-base range is coloured (all priority 0),
  0x44 is the 0.4 range in t5.
- Wiki: one row in `wiki/register-cheatsheet.md`, a comment on the ticket. kanban, log and index
  are left to the merge step.

Helpers in this directory: `swap.py` / `try.sh` (put a variant into the unit and measure it),
`data.py` (define the slot objects in C, static or global), `lda_dump.py` (print `lda` fields of a
`cc -K` `.B` file), `scan_base_compare.py` (corpus scan of base-and-compare materialisations).
Time: 16:21 to 17:23 UTC (62 minutes).
