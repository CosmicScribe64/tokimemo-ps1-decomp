# T-25604 round 1, lens "search on the new best": NAME_ENT func_80143580 MATCHED

Worktree `f-ne-r1-search-on-new-best` (from `w2-good` at ce2dfb9e8), 2026-10-23 21:47 to 22:50 UTC.
Started from the digest best (`../fresh-r1-best.c`, 122 rows, rev1 0 structural hunks + 70 register
substitutions). Result: **full match** as plain C in `src/ovl/NAME_ENT/80142A80.c`, clean build 27/27
sha1 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK. The same
source with the rev1 source delta (`../fresh-r1/rev1.py`) also reproduces Rev 1 exactly: 1111 of 1111
instructions, 0 structural hunks, 0 register substitutions, every unrelocated word equal
(`final_rev1.py`).

## Steps (bodies in `c/`, rows = whole-unit `funcdiff --resolve`, rev1 = hunks / register subs)

| body | rows | rev1 | change |
| --- | --- | --- | --- |
| `step0-digest-best-122.c` | 122 | 0 / 70 | digest best |
| `step1a-record-chain-90.c` | 90 | 2 / 68 | case 9: `D_801217D0[0] = [1] = [2] = 0x80000000;` (then `spr1 = 0x80000000`) |
| `step1-implicit-int-chain-30.c` | 30 | 0 / 19 | + func_8004AC18 undeclared (implicit int), `if (address) {}` FAKE dropped, compare-status FAKE dropped (no change) |
| `step2-entry-index-loop-20.c` | 20 | 0 / 12 | entry fill as `for (address = 0; address < 8; address++) tiles[address] = 0x80180000 + address * 0x4000;` |
| `step3-fills-on-i-match.c` | **0** | 0 / 0 | every fill (entry and the nested ones of cases 6/13/16) counts with `i` |

Removing either lever from the final body: func_8004AC18 void 214 rows, separate case 9 stores 216.

## Mechanism

Three facts, all read from the traced uopt (regdiff `-v`, `ranges.py --block`) and ugen's binasm
(`gc.py`/`g9.sh`, as1 shim that saves the `.G`):

1. **Store-backs leave a block in register order.** In ugen's output the store-backs of promoted
   globals at a block end are sorted by register number (seen in cases 0 and 9 of every build). The
   original's case 9 order (`unk_11`, `unk_12`, `unk_10`, then `D_801217D0[2]`, then the selector and
   `spr1.unk_10`) is therefore a statement about colours: `D_801217D0[2]` is a *promoted* global in
   case 9's else block whose register lies between `unk_10` (a1) and the selector (t0). That is the
   "range with no instruction of its own" the brief asked for: it is copied from the constant and as1
   folds the copy, so its a2 never shows.
2. **A chain reads its innermost target back.** `X = Y = Z = K` stores Z, loads Z into a cfe chain
   temporary and stores the outer targets from it; Z then has a load and a store in the block
   (saving 1, priority 1) and uopt promotes it. With `[0] = [1] = [2] = 0x80000000` the promoted range
   is `[2]`, and the 0x80000000 constant keeps only 2 uses in the case 9 block, so its priority drops
   from 1 to 1/3: it is coloured after the selector (0.5) and takes t1, the original's register, in
   all six cases. The in-place order of the other stores (`[1]`, `[0]`, spr1) is the original's.
3. **An undeclared call result blocks v0.** With func_8004AC18 called undeclared (implicit int, the
   ETC func_8013FFAC lever of T-25314), v0 is live after each of its calls. In case 9's else block
   (right after `func_8004AC18(0xFF)`) that pushes the chain temporary to v1, and `[2]` gets a2 (v0
   result, v1 temporary, a0 `unk_12`, a1 `unk_10` taken; a3 is the 0x80 chain temporary). Now a2
   and a3 are blocked in the selector's blocks, so the selector gets t0 without the T-25319
   `if (address) {}` FAKE, `unk_10` keeps a1 everywhere, and 0x80000000 gets t1.
4. **Fill counter.** The dead `address = 0` inner-loop inits of the nested fills were units of the
   entry fill's `address` range (store units at loop frequency 10, priority 35.5), in blocks where
   v0/v1/a0 are taken; that put the entry value in a1 and blocked a1 for the cases' outer end
   constants. With all fills on `i` (already the counter of the record, palette and copy loops)
   these defs join `i`'s range instead. Any other assignment of `i`/`index`/`address` to the four
   fills measured 20-48 rows (80 combinations).

## FAKEs left in the matched source (all still required, measured on the final body)

- F5F entry self-store (T-25319): without it 92 rows.
- `^ 0` on case 5's func_8004B19C argument (T-25319): 79 rows.
- Nested fill loops, outer loop runs once (T-25131/T-25319, from T-20008): single loops 306 rows.
- Two unused s32 locals `pad0`/`pad1` for the 0x90 frame (were `index`/`address`, now unused):
  24 rows without both, 28 without one; `tiles[9/10/12]` instead 20-28.

Dropped: `if (address) {}` (T-25319), the compare statuses in `index` (clean-room lens).

## Header changes

- `include/main_api.h`: `func_8004AC18` wrapped in `#ifndef MAIN_API_OVERRIDE_func_8004AC18`;
  NAME_ENT defines the override with its reason and declares `extern s32 func_8004AC18();`
  (pattern of ETC `8013FF80.c`). Other callers of the file still match (NAME_ENT.bin OK).
- `include/main_api.h`: `void col2sepia();` added (the K&R definition's type; it was in no header).
  Required: with col2sepia undeclared the function does not match.
- `func_80143498` stays `void` as on main (the digest's `s32` view is not needed).

## Search results (lens method)

- Search driver `search.py` (autotweak's edit kinds plus `emptyall` and `split`, scored whole-unit and
  on rev1): on the 122 base, 462 + 964 variants; real gains only the case 9 chain joins (106-108 rows,
  rev1 3 hunks) and `if (index) {}` in the entry fill (120). The chain gain was the lead: it moved
  0x80000000 to t1. Reading its trace and binasm gave facts 1-3.
- On the 30-row base the search found join/group variants at 26 rows (rev1 0 / 15) before I edited
  src; its later "0 rows" lines are an artifact (the driver read the edited src). Do not run the
  driver while src changes.
- Permuter (`-j 1`, nice 19) on the 122 body: nothing better than base in 28 min; restarted on the
  30-row body, stopped after the match.

## Scripts (scratch, no game data; build output under ignored `build/T-25604/`)

`score2.py` (whole-unit rows + rev1 alignment + key colours; `SCORE2_UNIT` names a guarded template
once src is plain C), `search.py`, `install.py`, `rd.sh` (regdiff -v on a body), `ranges.py` (ranges
per block from the trace), `interf.py` (from the r1-compiler branch), `gc.py`/`g9.sh`/`gdump.py`/
`as1save.py` (ugen binasm dump, adapted from T-25306), `c9.sh`, `final_rev1.py`. They read
`build/T-25604/ref/rev1.s`, a local copy of the gitignored `versions/asm/rev1/...` (never commit it).

## Proposal

Merge the match (src + the two main_api.h changes). Playbook entries worth filing:
store-backs leave a block in register order (case order is a colour statement); a chain promotes its
innermost target (the "invisible a2/a3 holder" idiom); undeclared callees as v0 blockers inside one
block. Review the four remaining FAKEs on the T-25604 ticket.
