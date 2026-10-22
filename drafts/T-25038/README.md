# T-25038 SHOUGATU selection loop: verified match

`src/ovl/SHOUGATU/8013E330.c`, `func_8013E6E4`: 948 bytes, 237/237 words, zero resolved differences. Default C has no assembly fallback. Private and production full-TU links both match; a separate link of the actual Ninja object also matches E6E4 and preserved E50C (472 bytes). SHOUGATU SHA1 passes. Ticket is In Review for root's combined integration gate.

## Mechanism and controls

The fresh canonical full-TU baseline was 187 printed/aligned lines, 236/237 words. Frame 0x68 and table homes 0x50/0x40 already matched. The old saved56 concern was absent. First difference was the retry flag in v0 instead of v1 after the debug call.

1. Integer debug return fixes that flag register: 177 differences. Production retains the canonical `void *arg0, ...` parameter list; only the ignored return type changes.
2. Exact unsigned byte view at the club6 selector restores its independent load: still 177 differences, now 238 words. Club5's selector remains promoted in a0 through the later status call.
3. Exact unsigned byte view at the status-call argument removes that promotion: zero differences, 237 words.

Removal controls from the match:

| Removed form | Aligned cost |
| --- | ---: |
| Club6 selector byte view | 174 |
| Status argument byte view | 177 |
| Status result u8 cast | 126 |
| Random operand multiplication by one | 2 |
| Integer debug return | 10 |

The cast control prints 128 lines but has aligned cost 126. All other listed costs equal printed lines. These are marked FAKEs in production. Byte views read the same unsigned byte; the arithmetic identity and redundant result narrowing preserve values. No extra call, store, flag, or build exception was introduced. The canonical `check_para_limit()` correction is retained.

## Ownership and verification

Root approved the narrow `MAIN_API_OVERRIDE_func_800AE0B0` guard around the existing canonical declaration, leaving every nonopting TU unchanged. The overlay forward declaration belongs to SHOUGATU.h. `D_80122D48` retains the existing main/DATE scalar-local exception documented in T-11590: EVENT has an overlapping function table, so canonical migration needs a separate coordinated change. Coordinator reviewed source and headers against CODING_STANDARDS with no findings.

All checks ran in existing `tokimemo-decomp-arm64`:

- `ninja build/ovl/SHOUGATU.ok`: SHA1 OK, covering the complete overlay and neighboring functions.
- `python3 tools/check_headers.py include src`: headers OK.
- `python3 tools/migrate_globals.py --check`: globals OK.
- `python3 tools/sync_protos.py --check-branch`: 0 new / 15 known, OK.
- Private scripts `build/t25038-proof.py` and `build/t25038-built-proof.py`: literal 948-byte proofs; built-object E50C 472-byte proof also passes.

Source SHA256: `f78b646e687d2bc32c6d5329305ffbe490fc6435258d3f9af5698e247cb74954`.
Object SHA256: `0a44b78839b38c222a60999724ee157c00ce0a842f747e5c3a3a234c4aaf065b`.
Private evidence is under `build/t25038/`; no binary or compiler dump is tracked.

Predecessor ledger: `drafts/T-24523/SHOUGATU/func_8013E6E4/`. The sustained-work instruction was satisfied by literal resolution, not a stopping limit.
