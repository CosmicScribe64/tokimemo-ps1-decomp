# T-25105: main func_80054FC0 (src/main/80053650.c)

Result: MATCH, 796 bytes (199 words), literal default C. One FAKE (an empty signed test).

## Start state

`INCLUDE_ASM`, best body the T-24501 draft (`drafts/T-24501/main/func_80054FC0.c`), adapted to the
T-25105 header split (`unk_CC8[16][16]` + `unk_DC8`): 113 resolved diff lines. The first 45 rows
agreed. Two problems were left:

1. The 20-byte and 24-byte record copies (`config0`, `config1`) took their transfer register one
   ugen slot late (t5 for t4 in the first, t0 for t8 in the second), and every later temporary followed.
2. The epilogue: the target is `lw ra; sw v1,0x200(a2); jr ra; addiu sp`, the draft
   `sw v1; lw ra; addiu sp; jr ra; nop` (one word more).

## What closed it

### Copy temporaries: block-copy state F

`tools/ugen_pass.py` puts a B or F sentinel store before every `Umov`, the small aligned copies
included. Between the address registers and the transfer register, ugen skips two list entries
after the B sentinel (a direct store) and one after the F sentinel (an indirect store). The
original skips one, so it is in state F at both copies. The pass's model gives B here: the last
state op before the first copy is the direct store `bank->selected_slot = sel % 15U`.

Diagnostic (scratch copy of the pass with `want_f = True`, not committed): with the draft's C, only
the 5 epilogue rows are left. So the copies need state F and nothing else.

The pass gives F when the last state op is an `istr`, or after an `istr` followed by a jump whose
test is a signed `<` with a positive bound (`_is_signed_blt_pos_imm_jump`). The indexed store
`slot_status[sel] = 1` is that `istr`. The FAKE test `if ((s32)D_800E7D10.selected_slot > 0) {}`,
placed just before the real `if`, survives uopt as `grt`/`fjp` in the ucode, emits no code, and
sets state F for both copies.

Measured negatives (full-TU funcdiff):
- `SaveImageView *bank` local for the selected-slot store: no change (uopt makes it a `str`).
- index `[(u32)sel < 0]` on the bank: `istr`, F, but ugen loads the base into a register
  (`lui; addiu; sb 0x26(t1)`, one more word): 142 lines.
- `*(volatile u8 *)&bank->selected_slot`: same as above (uopt turns it into `istr`; base in a
  register): 142 lines.
- `sel >= 1`, `sel > 0` on the u8: cfe/uopt keep the unsigned `neq`: no change (113).
- `(s32)sel >= 1` or `(s32)sel > 0` as the real test: F, but `blez` for `beqz`: 7 lines.
- `0 < (s32)sel` as the real test: 65 lines.

### Epilogue: store address that names the global

as1 moves `lw ra` above the last store only when the store's address is known not to be the stack.
A store through a pointer variable (`((SaveImageView *)(image + 0x200))->checksum`) stays in front
of the epilogue. A store whose address expression names the global,
`((SaveBank *)&D_80123120[arg0 * 0xF00 + 0x200])->checksum = sum;`, is scheduled with it:
`lw ra; sw; jr ra; addiu sp`. uopt still shares `arg0 * 0xF00 + D_80123120` (a2) with the loop
pointer `p = &D_80123120[arg0 * 0xF00 + 0x204]`, so no `image` local is needed.

Mini reproducer (non-leaf, call, byte-sum loop, then the store): a store through a pointer
parameter or local gives `sw; lw ra; addiu sp; jr ra; nop`; `*(s32 *)(arg0 * 0xF00 + G + 0x200)`
and `((s32 *)&G[arg0 * 0xF00])[0x80]` give the target shape; `*(s32 *)&G[arg0 * 0xF00 + 0x200]`
with `p = arg0 * 0xF00 + G + 0x204` loses the shared base.

Other measured spellings of the tail:
- `D_80123320[arg0].checksum` with `p = D_80123320[arg0].data`: base symbol +0x200 (6 lines).
- `D_80123320[arg0].checksum` with the draft's `image`: the store becomes `lui at; addu; sw` (18).
- index loop `sum += D_80123120[arg0 * 0xF00 + 0x200 + offset]`: 20 lines.
- the store and `}` on one source line: no change (ugen gives the epilogue its own `.loc`).

## Notes

- `s32` return with no `return`: the two callers in this file call it without a prototype
  (implicit int). No header change.
- `func_8005557C` (the other serializer in this file) has the same epilogue shape; the same
  store-address spelling should apply there.
- Scratch harness used (gitignored, not committed): `build/scratch/t25105/` held a copy of
  `tools/cc.py` whose ugen shim saved the ucode that enters `ugen_pass.py`, plus a parser that
  prints the records of the function. Reading that ucode showed the `str` before the copies.
