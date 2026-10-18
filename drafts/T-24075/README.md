# T-24075 ETC random selection checkpoint

Only func_8013C8EC (904 bytes), src/ovl/ETC/8013BEF0.c. Production source pin5d083af5
passes the actual default linked904-byte proof and ETC checksum. All34 other
actual function bodies and source outside the target are unchanged. Earlier
private full-TU diagnostics below are source controls, not independent matches.

| Source form | Differing rows | Result |
| --- | ---: | --- |
| Corrected current-type baseline |92| fresh valid baseline |
| Joined six remainder locals |116| rejected |
| Global short-circuit rejection loops |12| best |
| Real word pointer and pair loads |12| neutral |
| Word-pair address operand reversal |12| neutral |
| Matched same-TU direct pair call |0| actual default904-byte proof passes |

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
compiler-impossibility claim follows from this checkpoint. Ticket is
Done after parent independent verification and publicationc7690151b.

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

Matched func_8013F320 in the same TU calls the same table through direct
D_801500C4[index*2] and [index*2+1] arguments. Porting that actual pair-call
source idiom, without altering rejection loops, closes all six tail words:
private full-TU explicit-object default proof reports904 bytes equal. The private result preceded the production proof below. The sole caller8013BF34 passes no
arguments and ignoresv0; target assembly does not establish a return. The
retained s32 fallthrough view is conservatively marked FAKE because the original
return type is unproven. Existing implicit caller view is preserved without
header/config changes. Fresh replay with the note must remain zero.

Actual production proof, frozen5d083af5:

```
ninja -j1 build/ovl/ETC.ok
python3 tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8013C8EC
python3 tools/check_headers.py include src
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

ETC SHA1 OK; default build linked-function MATCH904B; headers/globals OK;
prototype branch check0 new/15 known. Logs in ignored build/etc24075:
default-unit.log, literal.log, headers.log, globals.log, prototypes.log and
neighbor-audit.json. A fresh unchanged full-TU before.o versus production
object comparison verifies34 non-target function resolved-word bodies. Six
.L assembly labels are excluded from the function inventory and covered by
their enclosing function. Exact whole-source substitution proves all neighbor
text unchanged, with no target guard or fallback. No header/config mutation.
Parent full standards/spec review reports no findings; independent all27
checksums/interfaces/actual-C gate and literal904B pass. Published mainc7690151b.
The exact production body remains frozen5d083af5; this packet preserves source
and replay. Root Docker measurement at publishedc7690151b:6516/6958 functions,1866672/2279368 bytes (81.9%),412696 remaining; gain904 bytes.
