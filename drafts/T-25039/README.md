# T-25039 MAIN club selector matched

Function: MAIN func_8005B43C in src/main/8005A0B0.c, 860 bytes.
Base: 77eae26cf. Historical T-25004/T-25008 reported 920 bytes; the actual
original span is 215 instructions / 860 bytes.

The final change moves the club local assignment into its consuming else
branch, before the existing empty halfword test. The branch condition now
compares the raw halfword shift. No new arithmetic identity, ignored
argument, type override or compiler change remains in the final source.
The earlier pad and empty read remain annotated FAKE.

The nonvolatile halfword reread crosses no store or call before that else
assignment. It has the same value as the compared shift. Source behavior
therefore stays the same. Defining the local only on the call path keeps
its argument-register preference from replacing the earlier shared shift.

## Measured path

All compiles use complete translation units and the existing Docker image.
Metrics below are target/built instruction counts and aligned diff cost.

- Baseline: 215/215, cost 6 (three changed instruction positions, six
  printed +/- lines). Shift/compare use a0 rather than v1; call slot is nop
  instead of move a0,v1.
- Baseline traced uopt: the shift-expression range is already v1, while
  the separate club local is a0 because it is a call argument. Subsequent
  copy coalescing selects a0 for the earlier computation.
- A late-folded zero term in the call argument separates those ranges but
  introduces a different carrier allocation, cost 10 or 12.
- Repeating the raw shift expression with a late self-zero term restores
  the target v1 and call move, cost 6; halfword becomes a0 instead of a1.
- Definition-side late identities, narrow views, dead reaching-definition
  edges, bitfield views, changed empty reads and selector-local reuse do
  not close both carriers. No such control was kept.
- Club local assigned in else BEFORE the empty halfword read: 215/215,
  cost 0. After the empty read or within the call argument: cost 10.
  Adding a late-zero term to this otherwise matching form regresses it.

## Verification

Promoted default C has no guard or INCLUDE_ASM for this function.

- `python3 tools/funcdiff.py --resolve main:func_8005B43C`: compared default
  build; MATCH, linked function bytes equal target (860 bytes).
- `ninja -j2 build/SLPM_86.053.ok`: MAIN binary checksum passes,
  SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26.
- `check_headers.py`: headers OK.
- `migrate_globals.py --check`: globals OK.
- `sync_protos.py --check-branch`: zero new disagreements, 15 known; pass.
- Root independent standards/spec source review: no findings. The local
  lifetime change preserves behavior, and prior FAKE annotations remain.

Root owns the full 27-binary integration gate. No new project-wide total is
claimed here. The private scratch tree /private/tmp/tokimemo-t25035/build/
retains t25039-*.py, full-TU candidate directories under t25039/, individual
measurements and diffs, and traced baseline/half-add/inline-selfzero/
def-u8-rawcompare compiler phases. Raw listings and binaries are not committed.
