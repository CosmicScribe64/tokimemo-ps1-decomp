# T-24016 BUNKASAI state machines

Final bodies are normal unguarded C in src/ovl/BUNKASAI/801354B0.c
(func_80135C4C) and src/ovl/BUNKASAI/80154018.c (func_80158764).
No alternate compiler, flags, header changes, FAKE, or generated-asm edits.

## Reproduction

```
tools/docker.sh ninja -j1 build/ovl/BUNKASAI.ok
tools/docker.sh python3 tools/funcdiff.py func_80135C4C --unit BUNKASAI --resolve --no-build --version default
tools/docker.sh python3 tools/funcdiff.py func_80158764 --unit BUNKASAI --resolve --no-build --version default
```

Results: BUNKASAI.bin OK; linked default matches for 1,508 and 1,652 bytes.

## Negative evidence

First target: T-16510 draft 264 printable diff rows; direct dialogue-field
updates instead of signed phi temporary 130; GameState pointer zero.
The assessment value is unk_66C[1].unk_00 (0x800E68F0).

Second target: T-18700 draft 332; faithful direct dialogue stores, GameState
pointer, and byte/bitfield views 239; ascending nested case arms 223
(func_80158764-near.c); repeated direct byte-field reads instead of saving
the shifted value zero (func_80158764-match.c). Source structure follows
matched sibling func_80157860. The final match changes only local names.
To reproduce a saved variant, substitute its body for the canonical definition
in a scratch full translation unit and use order_search.compile_variant with
non_matching=False, then resolved funcdiff against the expected object.
The near variant is negative evidence, never a matching-C claim.

The first production installation mistakenly used bulk_m2c.insert_draft,
which adds NON_MATCHING; its initial default proof checked fallback assembly.
Root review caught the guard; it was removed and actual normal C recompiled
and relinked before the successful proofs above. Final source contains neither
target's fallback or guard.

Private exploratory C, scripts, objects and instruction listings remain intact
under ignored build/T-24016-private/exploration. Only the useful source-only
near and matching candidates are committed here; no instruction listings or
objects are staged.
