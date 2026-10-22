# T-25033 SHOUGATU E50C verified match

Source: src/ovl/SHOUGATU/8013E330.c, func_8013E50C, 472 bytes.
Default C: 118/118 words, zero resolved differences; private and default linked
proofs both match all 472 bytes. Default SHOUGATU checksum and declaration/global
guards pass (0 new / 15 known prototype disagreements). Guard/fallback removed.
Coordinator owns formal review and full 27-binary integration.

## Closing mechanism

Before the opening conditional, initialize:

```c
girl = D_800E6280.unk_F5F * 0;
D_800E6280.unk_F5F = girl;
```

Preserve the exact first status argument `*(u8 *)&D_800E6280.unk_F5F` and the
existing u8 result cast. The nonconstant zero producer survives uopt, then ugen
emits the zero without loading the input. The early byte assignment is promoted;
its store is deferred to the first call, exactly as in the target. The unsigned
byte argument gives the required a2 allocation and later byte reload base.
No extra ignored arguments or identity expressions remain.

This resolves the prior missing move a2,zero / sb a2 / move a0,a2 residue.
The field correction unk_10F8 and canonical check_para_limit() remain. The existing
integer func_800847B8 return view is retained; its result is ignored.

## Source minimization and proof

- Literal zero in the same early-global form: 117 words / 15 cost.
- Removing status result narrowing: 118 words / 78 cost.
- Removing integer847B8 return view: 117 words / 25 cost.
- Removing girl shifts the table homes; keep i/girl before the 14-byte tables.
- A direct `D_F5F *= 0` also matches but leaves girl unused; retained source uses
  the local meaningfully instead of introducing a padding-only FAKE.
- Earlier direct unsigned-negative comparison also matches. Multiplication by
  zero is the smaller retained workaround and has an explicit FAKE comment.

The early store crosses no call and only disjoint nonvolatile state accesses;
all values and effects are preserved. Exact machine proof confirms original
store timing, frame 0x58, tables 0x40/0x30, and all registers/instructions.

## Private diagnostic trail

`build/t25033/astra/match-multiplyzero` is the matching full-TU private candidate
and linked proof. `early-direct-byte` is the original matching comparison form.
`single-identity-or` reproduced the carrier with a four-cost later reload residue;
`near-secondzerouse` reduced that to two wrong register operands. `trace-near`,
`trace-second` and `trace-early` retain ucode/allocator evidence. These intermediate
forms are diagnostic, not final source. No raw binary or compiler dump is tracked.

The historical five-line guard is pinned at 7a9ed5a81c5ad04f5349d1b36c98d014752dd24f.
The human's sustained-work instruction superseded old stopping limits; literal
resolution is now achieved and verified rather than inferred from a bounded search.
