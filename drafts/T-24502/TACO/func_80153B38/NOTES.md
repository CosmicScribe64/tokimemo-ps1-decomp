# T-24502 fan color-copy checkpoint

Full translation-unit NON_MATCHING C has26 aligned diff lines:12 target instructions versus14 built instructions in the color-decode block (net two extra real instructions,840 versus832 bytes). All geometry and polygon-loop words match, including frame0x1C8, SVECTOR[42] at0x78, col retained in s6, and buf homed then reloaded into s1. Default original asm and all27 binary hashes pass; prototype branch check has0 new disagreements (15 known). The guard has a source-local FAKE explanation for implicit-int fallthrough.

Target decodes high/middle through t7/t9, copies col to v0, masks into s7/s8/t1, remasks into faded s3/s4/s5, subtracts37, and spills t1 at0x5C. Draft initially shifts into s7/s8 then coalesces the byte masks with the faded values and inserts three moves to preserve the bright bytes; target needs one such move. Geometry and all9 final color fields are the same. Full guard uses canonical GetWorkBase/SetSemiTrans/SetShadeTex/AddPrim names; these keep the same26-line delta.

Applying documented in-loop decode and moving the SDK point array before scalar locals improves the older175/211-row drafts to this isolated block. Bounded alternate types: allwide or mixed wide bytes grow the frame and/or move the decode; signed byte combinations add conversions; low-to-high statement order worsens28cost to48; no-mask assignments and direct inline color fields are inferior. Moving color declarations into the loop has no effect; the newly proven |0 spill-web form also has no effect here. Best scratch cost28 includes two alignment nops in addition to two extra real instructions. No further grid attempted.

Primary exact replay from repository root in Docker:

```
python3 tools/funcdiff.py --version c --resolve TACO:func_80153B38
```

Standalone packet replay:

```
python3 tools/cc.py drafts/T-24502/TACO/func_80153B38/context.c build/T24502/fan-replay.o ido 5.3
python3 tools/funcdiff.py --built build/T24502/fan-replay.o --resolve TACO:func_80153B38
```

Private detailed output: build/T24502/fan-final-guard.diff; no raw instruction dump is committed. A narrow consult can inspect the byte-conversion/CSE cause without revisiting point generation or saved parameter allocation.


Independent narrow consult was negative: split extraction/masks costs153, signed faded bytes preserve the26-line gap, explicit byte casts cost170. Detailed private evidence: /private/tmp/tokimemo-fan-consult/RESULT.md. Do not repeat these variants.

Review identified undeclared func_8009F004: existing asm/libgpu.s at its label writes length6 and command0x30, the PsyQ3.x Gouraud triangle initializer (SetPolyG3). No callable canonical-name alias is present. A single documented `void func_8009F004();` declaration is added to include/libgpu.h under the preserved SDK symbol/K&R view. TACO includes it through psx_types.h and game.h; no local prototype.
