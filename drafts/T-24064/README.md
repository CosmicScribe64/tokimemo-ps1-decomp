# ETC model initialization loop

T-24064's resumed scope is only func_8013F980 (252 bytes) in
src/ovl/ETC/8013F980.c. Former target80140AE4 remains deferred;
80141ABC is unresolved separately in T-24067. No additional functions were
probed. Source win is frozen at0f34968776ed3fc3254f5ac4f83ae6862f4932b1,
from setup433b07e4c, branch codex/complete-etc-model-loop.

The older T-17300 draft has invalid scaled pointer steps: TaiikuBig+0x50
advances0x1900 bytes, and u32+0x10 advances0x40. The recovered loop instead
advances one80-byte TaiikuBig and four words per16-byte object slot, stopping
at the existing end symbol after16 records. The original constant header
cursor is retained in s2 across printf. Matched MAIN func_8006190C
(src/main/80061710.c, T-17010) provides the integer cursor idiom: initialize
the model address, map it, advance4 to read the object count, advance4 to
link the objects. This prevents the old pointer form's absolute-load folding.

| Private full-TU control | Resolved +/- rows |
| --- | ---: |
| Integer cursor, corrected typed do loop | 10 |
| Object initializer before record initializer | 4 |
| Same physical initializer/do line | 0 |
| Plain shared declaration, without local override | 0 |

The remaining four rows were two setup-order instructions. The initializer/do
line group is explicitly FAKE in production: it reproduces the observed
schedule, while original physical source layout is unknown. Keep that group
on one line. No dummy local, altered memory access, assembly or compiler
exception was added.

Parent approved a possible local three-argument B430 declaration, following
the existing DATE view and original a0/a1/a2 calls. Standards8a's plain-header
control also matches, so the override/declaration was omitted. No shared
header, config or interface change remains.

Actual production has unconditional C and no target fallback/guard. Run in
the existing Docker image, serially:

```
ninja -j1 build/ovl/ETC.ok
python3 tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8013F980
python3 tools/check_headers.py include src
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

Measured actual-default proof is MATCH252 linked bytes; ETC SHA1 passes.
Header/global guards pass and prototypes report0 new/15 known disagreements,
branch check OK. Exact fallback replacement audit preserves every other TU
byte, including matched func_8013FC64. Evidence is in ignored
build/etc24064/default-build.log, default-proof.log, headers.log, globals.log,
prototypes.log and body-audit.log. Private control sources/diagnostics remain
under build/etc24064/integer-cursor, object-first, grouped-init and plain-header.

Ticket remains In Review until the parent's independent review completes.
The parent owns all27 checksum validation, integration and push.
