# T-25303: main strCallback (second pass, 161 words, 640 bytes)

## Start state

First pass T-25118 (`drafts/T-25118/main/strCallback/NOTES.md`): no match, 75 rows, guard removed,
body in `drafts/T-25118/main/strCallback/strCallback.c`. It proved the body exact under the F
copy-state gadget (private shim copy) and concluded that only a `tools/ugen_pass.py` rule change
could fix it, because `D_80125C5C = 0` is the last state op before the RECT copy and no spelling
of that store reaches F without an extra instruction.

## What was measured

1. Re-inserted the first-pass body as a NON_MATCHING guard: `funcdiff --resolve --version c`
   75 rows, first divergence row 25 (`lui t2` vs `lui t3`) and rows 27/28 (second copy word `t1`
   vs `t2`). Same divergence as the first pass, same cause (state B before the copy).
2. Different cause sought: not the flag store itself, but any other state op before the copy.
   The shim's F rule has two arms: last state op is an `istr`, or (sticky for the rest of the
   procedure) an `istr` followed by a signed `<` jump against a positive constant.
3. `*(s32 *)0x80125C5C = 0;` (literal address, so the store stays an `istr`): ugen loads the
   upper half into a register (`lui t8, 0x8012; sw zero, 0x5c5c(t8)`), not `$at`; one extra word,
   and the ring is still one slot high. Rejected.
4. The sticky arm with the playbook idiom (T-25107 MASTER `func_801363F4`/`func_801344E0`, T-25147 main `func_8005938C`):
   at the top of the function,
   `if ((u32)D_800B0896 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_800B0896 < 1); }`.
   uopt keeps the `istr` and the signed loop test, so the shim sets its sticky F state; ugen drops
   the never-true arm and as1 deletes the dead byte load. Result: MATCH, 640 bytes, with the C
   body otherwise unchanged from the first pass.

## Result

Matched, default C: `funcdiff strCallback --unit main --resolve --version default` prints MATCH
(640 bytes); no INCLUDE_ASM line left for strCallback. The FAKE arm carries a comment naming
T-25303. `func_80056BA8`, `strNext`, `strInit` in the same file still MATCH.

## Negatives added

- Literal-address `istr` for the flag store (`*(s32 *)0x80125C5C = 0`): register base `t8`
  instead of `$at`, one extra word.

## Lesson

The first pass stopped at "no spelling of the last store gives F". The shim's sticky
`uist_rill_loop` arm does not need the last store at all: the documented never-true arm works
anywhere before the copy. Check that idiom before concluding a small-copy ring shift needs a
tooling change.
