# T-25048 MAIN func_80076FDC

Source: `src/main/80075320.c`, address0x80076FDC,852 bytes/213 words.
Default C matches all literal linked bytes. MAIN SHA1 passes:
`e823bd844a8f8fa4d05483b59c66bc54b8393b26`. Header/global checks pass;
prototype check reports0 new/15 known disagreements. No FAKE, padding,
volatile, declaration override, shared-header change, or build-rule change.

The reconstruction uses two real locals: activity first, then col. The
existing D_801230D0 global carries every background selection. This lets
uopt keep all selected backgrounds in a0, including the initial0xFF store;
the older local-background drafts assigned different colors and spent an
extra temporary. Activity is v0, column is v1 and saves at sp+0x28.
Both switch arms read the current packed club field. Empty tests are
unnecessary and were removed before promotion.

Exact address audit against immutable target:

- GameState index byte: D_800E6280.unk_71C, address0x800E699C.
- Activity byte: unk_69C[index].unk_01, address0x800E691D+4*index.
- Initial background tables:0x800B69D0 and0x800B69D8.
- Both club selectors: unk_0F4.h at0x800E6374, unsigned shift12.
- Fallback tables: D_800B6890[12][2] and D_800B68A8[12][2].
- Destination global:0x801230D0.
- Switch tables:0x800B18AC and0x800B18D8. Full MAIN checksum verifies
  their contents as well as the function's literal table-reference words.

The T-17000/T-20940 M2C archives contain stale scalar aliases and one
incorrect field mapping. The new source uses existing canonical GameState
fields/table declarations and preserves the target's control flow.
func_80073CB0 was inspected first but required missing canonical tables;
root separately owns that work, while this ticket owns only80075320.c.

Private complete-TU controls:

| Source | Diagnostic diff lines |
| --- | ---: |
| Local background, direct final stores |252|
| Same local reused in all final arms |264|
| Global background, col before activity |4 (two stack-home substitutions)|
| Activity before col, empty global use |0|
| Activity before col, no empty use |0; literal default-C MATCH852 bytes|

Replay any control in Docker:

```
python3 drafts/T-25048/replay.py current
python3 drafts/T-25048/replay.py reversed-locals
python3 drafts/T-25048/replay.py empty-use
python3 drafts/T-25048/replay.py local-background
python3 drafts/T-25048/replay.py reused-background
```

Replay is diagnostic; it does not call a normalized match a literal proof.
Definitive commands, all passed:

```
ninja -j2 build/SLPM_86.053.ok
python3 tools/funcdiff.py --version default --resolve main:func_80076FDC
python3 tools/check_headers.py
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

No game binaries, assembly listings, or private compiler traces are committed.
Root owns independent review and full27 integration.
