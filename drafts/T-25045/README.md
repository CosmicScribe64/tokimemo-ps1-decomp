# T-25045 ETC grade tail matched

Function: ETC func_80146D4C in src/ovl/ETC/80146400.c. Actual span is
264 instructions / 1056 bytes; the historical 1076-byte estimate was stale.

The earlier guarded C was six instruction positions (12 printed lines) away.
Only the final index shift and counter temporaries differed. A logging-only
ugen replay, checked byte-for-byte against the ordinary captured binasm,
showed that one additional temporary allocation before the final index is
sufficient. The apparently separate skipped t5 in the counter is already
absent from ugen's free-list order; it does not need a second operation.

## Final source mechanism

The T-25047 alternating-identity mechanism transfers to the preceding total
printf argument. Reuse the old padding word as `s32 zero`, then write:

```c
zero = g->unk_041 * 0;
func_800AE0B0("合計点 %d\n", (score + zero) | zero);
```

The multiply-by-zero survives uopt. One intermediate identity spends the
missing ugen slot, and as1 deletes the zero/identity operations. The zero's
lifetime ends before the call, so final address CSE is unchanged. This is
marked FAKE, not a claim about the original source.

The byte multiply is defined and yields zero for every input. Adding zero
and ORing zero preserve score without new overflow or side effects. The
existing byte read is nonvolatile, and no calls or writes are added. The
old frame slot is reused; all existing grade logic, stores and calls remain.

## Evidence and negative controls

Full-TU counts/cost use the existing Docker toolchain. Production source
stayed unchanged until a zero-cost candidate was found.

- Baseline: 264 words / cost12.
- Index/counter late-zero identities, unsigned widening, discarded narrow
  carriers and empty branches either retain baseline or add code/change
  allocation. Signed narrowing breaks final address sharing.
- Byte self-assignment reaches index t3, counter t4/t6 but adds stores and
  a reload copy. Diagnostic only; never promoted.
- Local selector/shared-pointer versions remove global promotion or change
  address allocation. Late masks can yield the desired first shift but
  duplicate the final address. Always-true byte-range wrappers retain an
  unnecessary self-compare branch and spills.
- Applying T-25047 identities inside the final address or to its base changes
  CSE/allocation. Applying them to the total sum itself remains cost12.
- Total argument `(score + zero) | zero`, or reassignment of score before
  the call: 264 words / cost0. Argument form retained as the smaller edit.
- Either identity alone: 264/cost12. Literal zero initializer: 264/cost12.
- Unsigned-negative initializer and byte multiply-zero initializer both
  reach 264/cost0. Simpler multiply form retained.

## Verification and review

- Default `python3 tools/funcdiff.py --resolve ETC:func_80146D4C`: MATCH,
  all 1056 linked bytes equal the target; assembly fallback removed.
- `ninja -j2 build/ovl/ETC.ok`: ETC binary checksum OK.
- `check_headers.py`: headers OK.
- `migrate_globals.py --check`: globals OK.
- `sync_protos.py --check-branch`: 0 new disagreements / 15 known; pass.
- Root independent standards/spec source review: no findings. C89,
  fixed-width types, defined zero/identities, reused slot and FAKE annotations.

Root owns the full 27-binary integration gate. No new project-wide census is
claimed here. Private evidence remains in /private/tmp/tokimemo-t25035/build/:
t25045-*.py orchestration, t25045/* complete candidate translation units,
measurements and diffs, captured ugen phases, and the logging-only ugen
reference/tracer. Compiler/game dumps and binaries are not committed.
