# T-25044 parameter_up_down

MAIN `src/main/800737A0.c`, address 0x800747DC, 1228 bytes (307 words).
Default C matches every literal linked byte. Docker `ninja -j2
build/SLPM_86.053.ok` verifies MAIN SHA1
`e823bd844a8f8fa4d05483b59c66bc54b8393b26`. Header/global checks pass;
prototype branch check reports zero new and 15 known disagreements.

The final source uses six locals. A marked empty test of the raw mode byte
keeps that byte in v1; assigning its nibble to h and testing h directly avoids
the old temporary plus two copies. The same u32 h then captures the club index.
The loop reads signed table elements directly, so it does not overwrite the
index value and does not need another scalar slot. The row address stays in
its original stack home at sp+0x4C. No padding, volatile, union, prototype,
compiler-option, or build-rule changes remain.

All controls compile the complete translation unit:

| Control | Result |
| --- | --- |
| Earlier in-tree guard | 309 words versus 307; extra head copies, row spill0x58 |
| Cast the byte to signed before masking | Same head/pointer differences |
| Unsigned element carrier | Same head/pointer differences plus four reversed multiply operands |
| Empty test of the masked local | No improvement |
| Empty raw-byte test, direct nibble test | Head exact; first difference is promoted row-expression result |
| Distinct scalar index and element local (`four-row.c`) | All instructions exact except four row-home accesses0x48 instead of0x4C |
| Additional scalar or four-byte array | Row homes exact; frame0x70 instead of0x68 |
| Index array | Adds index store and changes homes |
| Pointer array / volatile chained pointer / index-pointer union | Changes promotion, homes, or frame; rejected |
| Reuse selector parameter as walking pointer | Changes selector precoloring; rejected |
| Distinct index, direct signed element expressions | Literal default-C MATCH1228 bytes; MAIN checksum matches |
| Remove raw-byte test from final C | 245 diagnostic diff lines; first load moves from v1 to v0 |

`replay.py` preserves the useful causal controls without target assembly or
binaries. In Docker:

```
python3 drafts/T-25044/replay.py current
python3 drafts/T-25044/replay.py no-byte-use
python3 drafts/T-25044/replay.py four-row
python3 drafts/T-25044/replay.py padded
```

Replay output is diagnostic, not a literal match assertion. Definitive proof:

```
ninja -j2 build/SLPM_86.053.ok
python3 tools/funcdiff.py --version default --resolve main:parameter_up_down
python3 tools/check_headers.py
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

Coordinator semantic review checked signed s8 promotion, the u32 index shift,
and repeated element reads with no intervening call. Its stale-comment finding
was resolved. Root owns independent integration and full27 verification. The
ticket remains In Review until that review completes.
