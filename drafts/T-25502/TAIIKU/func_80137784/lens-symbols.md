# T-25502, lens "symbols": TAIIKU func_80137784 (1216 bytes, 304 words)

Result: **matched as plain C, no FAKE added, no config change.** The two player tests are `arg0 == 0`.
The second symbol names of the T-25309 `alias-match.patch` are not needed, and the evidence says they did
not exist in the original. `src/ovl/TAIIKU/80133C80.c` now has the function as plain C (no guard).
Copy of the body: `match-0rows.c`.

Measured with `tools/docker.sh python3 tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version c`
(whole unit) and `--version default` after the guard was removed. Base: w2-good at 7bab0f99c.

## Measurements

| body | rows |
|---|---|
| guard at start (`&D_8014984C[arg0] == (Fix32 *)&D_8014985C - 4`, the same for 981C) | 4 |
| natural pointer compares (`D_8014984C == &D_8014984C[arg0]`), `variants/nat.py` | 226 |
| natural compares + the four slots defined in C (`s32 D_8014981C[4] = { 0 };` ...), `variants/def.py` | 226 |
| natural compares + file-scope `static` definitions, `variants/sdef.py` | 248 |
| natural compares + function-scope `static` definitions, `variants/fstatic.py` | 248 |
| **`if (arg0 == 0)` and `if (arg0 == 0 && D_80122E94 == 1)`, `variants/zero.py`** | **0 (MATCH)** |

The `static` forms move the four objects to the end of the unit's `.data` (`.data+0x750..0x770`) and keep
the same colouring (base in t5, 0x44 in ra), so a static object is the same uopt candidate as an extern one.

After the guard was removed: `funcdiff --version default` MATCH (linked bytes equal, 1216), `build/ovl/TAIIKU.bin: OK`,
`globals OK`, `check_headers.py`: headers OK, `migrate_globals.py --check`: globals OK,
`sync_protos.py --check-branch`: branch check OK (0 new disagreements). Clean rebuild (`rm -rf build`,
`configure.py`, `ninja`): exit 0, sha1 OK 27/27 (each built `.bin` checked against its yaml `sha1`).

## Mechanism

The original does not compare pointers in the source. It tests the index: `if (arg0 == 0)`. uopt rewrites
the test of `arg0` into a test of an address that is linear in `arg0` and is already in a register: the slot
address `&D_8014984C[arg0]` in a0 (block 1) and `&D_8014981C[arg0]` in a1 (block 3). The constant 0 becomes
the slot base, which ugen builds with `lui/addiu` at the compare: `bne a0,t8` and `bne a1,t9`, pointer first.
The base constant at the compare comes from that rewrite. It is not a source use of the slot base, so
the slot base gets no register ahead of the 0x44 stride (the "priority <= 0" that T-25309 inferred). The C
at the entry test (`arg0 != 0 || D_80122E94 == 0xFF`) has no slot address live yet and stays `bnez a0`.

T-25106 measured `if (arg0 == 0)` as a colouring probe ("0x44 -> t5, D_80122E94 -> a2, as original") but
did not report its compare instructions; that probe already matched the original colouring. With today's
body (all T-25106 and T-25201 levers in place) it is a full match. The premise of T-25106 to T-25309 (that the
source has a pointer compare and needs a second name for the slot base) was wrong.

The same rewrite exists in matched code of this overlay. Siblings `func_80147A6C` (`index != 0` with
`slot = &D_8014A454[index]` compiles to `bne v0,t9` against a fresh `lui/addiu D_8014A454`),
`func_801482BC` (`index == 0`, the same) and `func_8013F3A4` (`arg0 == 0` against `D_8011ECD0`).

## Lens question: did the two extra symbols exist? No.

- References: in the whole TAIIKU overlay only func_80137784 references any address in
  0x8014981C..0x8014985B, and only the four bases 981C, 982C, 983C, 984C (`%hi/%lo`). There is no data word
  that points there, no reference to an interior address (0x80149820 and so on), and no reference from
  main. Other overlays use the same VRAM for their own data.
- Layout: four 16-byte, zero-initialised `.data` objects, 4-aligned, back to back after D_80149814 (an
  8-byte initialiser of func_80137090). From D_8014933C on, every `.data` piece of 80133C80 is used by exactly
  one function, and the pieces are in the address order of those functions. So the four slots are defined in
  this C file next to (or inside) func_80137784. One C definition gives one name. A second zero-offset name at
  the same address cannot come from C, only from a linker script or assembler label, and nothing
  shows either.
- O.BIN (wiki/obin.md): its symbols are the main program (Global/Abs) and the OLH overlay. Nothing for
  TAIIKU. wiki/ref-english-patch.md has no TAIIKU entries.
- Siblings: no other function uses these tables. The matched functions with the same asm shape (fresh base
  constant compared with an element pointer of the same array) all match from an index test (above).
  `census.py` lists the original functions that compare against a fresh `lui/addiu` constant that is also an
  indexed base in the same function: 12 matched, `census.txt`. The precedents with separate names (TACO
  D_801274D0, ETC D_8014FB1C) are compares with an element at a different address, which splat names on its
  own. None of them needs a duplicate name.

What would change this verdict: a linker map or symbol file of the TAIIKU build with two names at 0x8014984C
or 0x8014981C. Without one, a second name is a fakematch under CODING_STANDARDS 7, and it is no longer needed.

## Proposal

1. Take this body (committed in this worktree): plain C, no new FAKE. The function's comment names T-25502.
   Delete the D_8014985C compare FAKEs (done here) and drop `alias-match.patch` (T-25309).
2. Add a cheatsheet row (wiki/register-cheatsheet.md): "the original compares an element pointer that is
   already in a register with a fresh `lui/addiu` of the same array's base (`bne a0,t8`) -> the source tests
   the index, `if (i == 0)`; uopt rewrites the test. Do not write the pointer compare or add a second name."
   Cite TAIIKU func_80137784, func_80147A6C, func_801482BC.
3. No tooling or config change.

## Files

- `match-0rows.c`: the matched function block (macros, body, `#undef`s).
- `ed.py`, `run.sh`, `variants/*.py`: the measurement harness. The variants edit the guarded file as it
  was at 7bab0f99c (they look for the old guard text). Against the committed match they abort with
  "found 0 times".
- `census.py`, `census.txt`: the asm census above (run with `tools/docker.sh python3`).
- `alias-match-T25309.patch`: the T-25309 alias patch, for reference only; superseded.
