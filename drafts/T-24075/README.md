# T-24075 ETC random selection checkpoint

Only func_8013C8EC (904 bytes), src/ovl/ETC/8013BEF0.c. Production remains
assembly and neighbors/interfaces are unchanged. These private full-TU default
resolved diagnostics are not actual linked C match proofs.

| Source form | Differing rows | Result |
| --- | ---: | --- |
| Corrected current-type baseline |92| fresh valid baseline |
| Joined six remainder locals |116| rejected |
| Global short-circuit rejection loops |12| best |
| Real word pointer and pair loads |12| neutral |
| Word-pair address operand reversal |12| neutral |

The baseline corrects the old s32-pointer scaling: D_801500C4 plus index*2
addresses one eight-byte pair; old pointer plus index*8 addresses32 bytes.
GameState fields replace absorbed scalar names. All six remainder operations
already compile as DIVU: IDO -cckr promotes u8 to unsigned int. No explicit
unsigned-cast experiment or gain is claimed. Historical92 is context, not the
fresh measurement.

Replacing duplicated m2c local/goto/store chains with ordinary do loops that
assign D_801500C0 and reject previous/per-girl excluded values restores all six
original RNG loops, including repeated stores and immediate comparison values.
The frame0x30 and226 instruction words agree. Twelve differing diagnostic rows
represent six real words, all in the final table-pair address calculation:
index/base/shift registers rotate among t5/t6/t7 and ADDU operands differ.
The word-pair and reversed operands leave that gap unchanged. Required RNG
calls, rejection sets, mutation timing and final helper calls are preserved.

The inherited s32 fallthrough return view is unproven; no return-type experiment
was performed. No instrumentation, forced store, alias/header/config change or
compiler-impossibility claim follows from this checkpoint. Ticket remains
In Progress for concrete source investigation.

Replay serially in existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24075/replay.py
python3 drafts/T-24075/replay.py --variant baseline
```

The helper replaces only the assigned fallback in a private full TU, strips the
archive banner to preserve measured source placement, compiles with stock
project flags, and checks funcdiff --built --no-build --resolve --version default
--unit ETC func_8013C8EC. It validates the process result before counting rows
and asserts production source unchanged. Objects/assembly/dumps stay ignored.
