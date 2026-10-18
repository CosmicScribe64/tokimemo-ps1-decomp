# T-24078 two-polygon renderer checkpoint

Active recovery of only ETC func_8013D570 (2352 bytes), src/ovl/ETC/8013BEF0.c.
Production remains ASM; no helper, header or config changed. These are private
full-TU default resolved diagnostics, not linked C matches.

Original frame0x90; VRAM8013D5BC sets cursor tosp84, 8013DE64 advances four
bytes and8013DE4C/DE68 enforce two iterations. This proves POLY_FT4*poly[2]
atsp84/88. Baseline fixes old singleton storage, all16 malformed aggregate
frame-index accesses to real GameState.unk_018 and two-argument AddPrim's byte
order-table address. Matched same-TU D32C supplies the SDK field/array idiom.
Historical T17300 plain792/typed957 are different contexts, not fresh results.

| Current full-TU source | Resolved +/- rows | Frame |
| --- | ---: | ---: |
| baseline.c |800|0xC0|
| shared-temps.c |800|0x98|
| body.c: direct SDK UV assignments |790|0x98|

The shared-temp control joins12 same-type aliases whose lifetimes occupy
mutually exclusive switch arms, preserving expressions and store order. It
removes40 frame bytes, not the hypothesized48. The UV control removes inherited
signed-byte casts before actual SDK u8 stores; this restores word loads of the
persistent texture-coordinate variables. It changes no stored low byte.
Initial malformed parentheses during field substitution failed compilation and
were repaired before any score; that failure is not counted as a baseline.

Replay in existing Docker, one compiler at a time:

```sh
python3 drafts/T-24078/replay.py --variant sdk-uv
python3 drafts/T-24078/replay.py --variant baseline
python3 drafts/T-24078/replay.py --variant shared-temps
```

The helper compiles only an ignored full-TU copy, invokes explicit default
resolved funcdiff and validates its diagnostic status. It accepts exactly the
ASM fallback or exact pinned body after any promotion, and checks production
text remains unchanged. Output: build/etc24078/replay/<variant>/diagnostic.txt.
The retained s32 fallthrough is conservatively FAKE-tagged: current implicit-int
callers ignore its result, but original return type is unproven.

Next evidence to inspect is the remaining eight-byte frame excess and original
versus current expression scheduling. No allocation/type sweep or compiler
instrumentation is authorized by this checkpoint; ownership remains active.
