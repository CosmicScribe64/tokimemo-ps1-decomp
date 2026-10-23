# T-25502: TAIIKU func_80137784, synthesis (1216 bytes, 304 words)

Result: **MATCH in plain C with the stock toolchain.** No new FAKE, no symbols file, no config or tool change.
`src/ovl/TAIIKU/80133C80.c` has the function without a guard or INCLUDE_ASM. The two D_8014985C alias FAKEs of
T-25106 are gone. The two player tests are the plain index tests `if (arg0 == 0)` (end of block 1) and
`if (arg0 == 0 && D_80122E94 == 1)` (block 3).

Base: w2-good at 8bc87088e. The source change is the plain-C lens commit (82f569f9e), cherry-picked. The conflict
with the T-25309 guard comment was resolved by rewriting the function comment.

## Verification (this worktree)

- `tools/docker.sh python3 tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version default`:
  `MATCH (linked function bytes equal target (1216 bytes))`.
- Clean build (`rm -rf build`, `configure.py`, `ninja -j2`): exit 0, 27/27 sha1 OK. The 26 splat messages
  "Unable to determine a segment for the following user-declared symbols" come from the existing split
  configuration. This change does not touch config, so they are not new.
- `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK.
- No tool changed, so the tool unit tests were not needed.

## Mechanism

The original's `bne a0,t8` / `bne a1,t9` compare a slot address that is already in a register
(`&D_8014984C[arg0]` in a0, `&D_8014981C[arg0]` in a1) with the slot base, which ugen builds fresh with lui/addiu
at the compare. That compare is not in the source. uopt's test replacement (LFTR in this wiki) rewrites the index
test `arg0 == 0` into `&arr[arg0] == &arr[0]` when two things hold: the address `&arr[arg0]` was formed in an
earlier basic block and is live into the test's block, and the raw index is not used after the test (scaled
products such as arg0*4 or arg0*0x44 may stay live). The base constant comes from the rewrite, not from a source
use, so it never becomes a register candidate ahead of the 0x44 stride. That is the "priority <= 0" T-25309
inferred. The other arg0 tests keep `bnez` (the entry test `arg0 != 0 || ...` and the first `if (arg0 == 0)` of
block 1, `bnez s0`).

Review correction (T-25502): an earlier version of this note said arg0 stays live after the block-3 test
(`addiu t8,s0,0x90`). That instruction computes `arg0 + 0x90` for the statement before the test and is only
scheduled next to it. In the target, no path reads s0 after either rewritten compare: after `bne a0,t8`
(0x80137960) both paths go to the exit, and the `bnez s0` at 0x801379B4 is on another branch, reached before it.
IDO 5.3 probes by the standards reviewer agree: `x == 0` stays `bne x,0` when x is used after the test, and when the
address is formed in the same block as the test. The plain-C lens's condition (raw index dead after each test)
was right.

`&D_8014984C[arg0] == D_8014984C` is true exactly when `arg0 == 0`, so the index test means the same thing.

## The lenses

| lens | result | what it contributed |
| --- | --- | --- |
| symbols (`lens-symbols.md`) | MATCH, plain C | Showed that the second names of the T-25309 `alias-match.patch` did not exist: only this function references 0x8014981C..0x8014985B, only through the four bases, no data word points there, and the four 16-byte zeroed `.data` pieces sit in function order like other C definitions of the unit. Measured: natural compares 226, slots defined in C 226, `static` 248, `arg0 == 0` MATCH. census.py: 12 matched functions compare a fresh base with an element pointer of the same array, none with duplicate names. |
| plain C (`lens-plainc.md`) | MATCH, plain C | Same source change, found independently. Measured negatives: pointer parameters, K&R/ANSI/`register` parameters, `()` prototype (228), block-scope `extern Fix32 D_8014984C[1]` (cfe error), C-defined slots with the guard. An lda dump of cfe output showed that every spelling of one symbol gives the same lda key, so no spelling of the base can make it a separate candidate. `scan_base_compare.py` (kept in `plainc/`) finds the same compare shape in matched TAIIKU `func_80147A6C`, `func_801482BC`, `func_80147B14`, `func_8013F3A4` and main `func_8006F218`. |
| toolchain (`lens-toolchain.md`) | MATCH only with a modified uopt | Found the uopt step that makes a source-level pointer compare join the base's candidate (`makelivranges` gives an `lda` operand of `equ`/`neq` a live unit). A modelled rule (no unit outside loops) matches the natural pointer compares. Not adopted: see below. Its files (`tools/uopt_model.py`, `build-integration.patch`, `src-bunkaken.patch`) stay on the w12-taiiku-toolchain branch (0300bc666, 5763bb1c2). |

## What was combined, and why the toolchain route is rejected

The symbols and plain-C lenses produced the same source change, so there was nothing to compose. The synthesis
verified it from a clean build and corrected the stated condition (arg0 may stay live).

The toolchain rule fails CODING_STANDARDS 7a. It is not needed, because stock IDO 5.3 gives the original bytes
from `arg0 == 0`, and so does every other function, with no rule. It is also not uniform: under the rule, the matched
BUNKAKEN `func_8014D65C` changes (13 edits) and needs a respelled FAKE. The rule explained how to make the wrong
source (a pointer compare) match. It did not show that the original compiler differed. The lens's census remains
useful: outside loops, the original never compares against an address it keeps from an earlier block.

## Remaining FAKEs (unchanged, from T-25106)

- `RUN(n)`: `(&D_80149208.player)[n]`, the shared player/cpu[3] index, as in matched func_80138680.
- `(s32)` on `RUN(arg0).unk_02`: keeps the record index signed so it shares `li t5,68` with the arg0 products.
- `(&DISP(46))[D_801491D8]`: keeps the u8 global's record product unsigned and strength-reduced, as in
  func_80135F3C.

T-25106 measured each one. They come from how TaiRunnerA and D_801491D8 are typed across TAIIKU, not from this
function, so dropping them is a type-level cleanup for the whole overlay and does not block this match.

## Next step

Code review against CODING_STANDARDS 7 and 7a, then In Review / Done for T-25502. Do not apply the T-25309
`alias-match.patch`; drop the second-names plan. The base-compare scan finds no other non-matching function with
this shape (the toolchain census agrees: among the remaining functions only this one compares a fresh base outside a
loop), so the lever does not carry over to SHOUGATU func_8013EA98.
