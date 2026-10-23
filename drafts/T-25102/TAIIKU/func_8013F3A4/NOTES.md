# T-25102: TAIIKU func_8013F3A4 (361 words, 1444 bytes)

Result: matched, default C, commit `T-25102: match TAIIKU:func_8013F3A4`. Clean rebuild 27/27 sha1 OK;
check_headers, migrate_globals --check and sync_protos --check-branch pass. No FAKE constructs.

## Start state

Plain `INCLUDE_ASM`. The only bodies were automated m2c drafts (T-14500 score 525, T-16550 score
519). They used per-case pointer temporaries and read actor 0 `unk_00` where the original reads
record 104 `unk_03` (the player flag clear). T-11640 and T-24521 named the blocker: IDO keeps the
address of `D_8011ECD0` in `$a3` for the whole switch, while the original rebuilds it with
`lui/addiu` in ugen temporaries in every arm and case.

## Structure

Records 104 + arg0 and 108 + arg0 of the `D_8011ECD0` display pool. Runner 0 takes its state from
`D_80122EC4` and its phase from `D_801499C0.halves.high`; runners 1..3 call `func_8013FB14` and
`func_8013FBA8`. Two arms set or clear bit 0x40 of both records' `unk_03`. A seven-entry jump table
(cases 0 and 6 share a body) sets `unk_26`/`unk_2A` of record 104 and copies them, with an offset, to
record 108. Cases 1, 2 and 5 store `0x90`/`0x8A` to record 104's `unk_18` for runner 0 and
`0x8E`/`0x86` to its own record otherwise.

## What closed it (rows from funcdiff --resolve)

1. **No record pointer local.** Every access is written `((DisplayActor44 *)D_8011ECD0)[arg0 + 104].f`.
   With `p = &((DisplayActor44 *)D_8011ECD0)[arg0]` per case, regdiff shows two hoisted
   "new temporary" ranges: the base address and the constant 0x44. uopt loads them at the switch
   head and in both flag arms (`li a2,68; multu` replaces the original `sll/addu/sll`), and every
   temporary after that is off (524 rows; with an `if (p) {}` to pin the pointer at the case top,
   479). The range is connected through the flag arms in the arg0 != 0 path: without those arms
   nothing is hoisted. Declaring `DisplayActor44 D_8011ECD0[]`, sized declarations, unsigned index
   or multiplier spellings, byte arithmetic and `(&rec[104].unk_03)[arg0 * 0x44]` all leave the
   hoist in place.
2. **`if (arg0 == 0)` for the player test**, not a pointer compare. uopt rewrites it into
   `bne v0,<base>` (the original's `lui t6; addiu t6; bne v0,t6`). uopt's output has a fresh `lda`
   in the compare, and the allocator does not count it as a use of the base constant (a test
   with a pointer and the compare alone hoists nothing). Written as
   `&rec[arg0] == rec`, the compare is a real use; it makes the base range profitable again and
   brings back the hoist. The control measures 482 rows. The same rewrite appears in matched
   TAIIKU `func_80147A6C` (`index != 0` becomes `bne v0,t9`).
3. **`phase` declared before `state`.** `state` is saved across `func_8013FBA8`. Declared first, it
   takes `0x2C(sp)` where the original has `0x28` (control: 4 rows).
4. **x store before the y stores in each arm** of cases 1, 2, 4 and 5:
   `rec.unk_26 = ...; rec.unk_2A = ...; rec.unk_2A = rec.unk_2A - arg0 + 3;`. as1 still emits the x
   store last, but ugen numbers the x temporaries first. In the original, the then-arm of case 1
   leaves t0/t1 for x before t2..t7 for y (control: 258 rows).

## Negatives measured (do not repeat)

- Shift spelling of the record offset, `D_8011ECD0 + (((arg0 << 4) + arg0) << 2)`: the base is still
  hoisted and ugen uses fresh temporaries for each shift (547 rows).
- `-Wo,-Olimit,20` (diagnostic only): uopt skips the procedure and keeps locals on the stack, unlike
  the original.
- A literal-address base `(DisplayActor44 *)0x8011ECD0` gives `lui at; addu` with folded offsets.

## Tools used

`regdiff -v` with the traced uopt (R9SAVE units of the hoisted ranges: loads at the switch head
and in both flag arms). A capture of uopt's input and output ucode showed the late `lda` of the
compare rewrite. The scratch scripts lived under `build/` and were removed by the clean rebuild.
Copy the traced uopt again from another tree's `build/scratch/*/r9-uopt/variants/trace` if you need it.
