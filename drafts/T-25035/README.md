# T-25035 GYOZI zero-carrier match

Function: GYOZI func_80140E0C, 388 bytes in src/ovl/GYOZI/80140C90.c.
Parent: 242c0c3a3. Donor: the concurrent T-25033 SHOUGATU reconstruction.

The old guarded draft had 97 target / 96 built words, aligned cost 5: a
missing entry a1-zero, followed by hardware-zero store and argument uses.
A late-folded expression supplies the missing zero carrier. Assigning it
to the global before the flag tests lets allocation and store sinking keep
that carrier and the later reload base in a1. The final source uses:

```c
girl = D_800F62CF * 0;
D_800F62CF = girl;
```

Both the multiplication and early assignment are marked FAKE. This has
no volatile read, extra observable store or call, or changed argument list.
The flag conditions read disjoint globals and do not call anything. Moving
the index store before them preserves behavior. The compiler emits the
store after the conditions, exactly as the original. The existing byte
argument view and result narrowing remain marked FAKE. Shared declarations
and compiler configuration are unchanged.

## Docker evidence

- Baseline full TU: 97/96 words, five printed lines, aligned cost 5.
- Late unsigned-negative local and ignored first-call second argument:
  97/97 words, cost 4; exact entry/store/call, later a0 base instead of a1.
- Added second-call unsigned-negative expression: still cost 4, v1 base.
- Carrier widths, byte store, argument views and high-address argument
  controls did not close this residue; the high-address forms added code.
- Early global assignment plus ordinary byte-view call: 97/97, cost 0.
- Simpler multiply-by-zero expression with that placement: 97/97, cost 0.
- Promoted production body, no NON_MATCHING or INCLUDE_ASM for this function:
  `funcdiff --resolve GYOZI:func_80140E0C` reports compared default build,
  MATCH, linked function bytes equal target (388 bytes).
- `ninja -j2 build/ovl/GYOZI.ok`: binary checksum passes,
  SHA1 43f92523116e7b6cd772d4397712a52eeda89703.
- `check_headers.py`, `migrate_globals.py --check` and
  `sync_protos.py --check-branch` pass, zero new prototype disagreements.

The first migration guard run lacked DATE/TACO reference assembly in this
fresh checkout; generating those split outputs resolved it. Root owns the
full 27-binary integration gate and independent review. No full-build or
coverage-total claim is made here.

Private replay evidence remains under /private/tmp/tokimemo-t25035/build/:
t25035-probe.py, t25035-refine.py, t25035-coalesce.py, t25035-early.py,
t25035-finalize.py and t25035/ candidate/measurement directories. The
trace-near directory records the intermediate late-twoarg allocator result.
Compiler output, binaries and raw instruction listings are not committed.
