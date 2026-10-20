# T-24502 projection grid checkpoint

Not matched; production stays original asm. The semantic donor func_8013E778 in src/ovl/TACO/8013DB80.c supplies the SDK MATRIX/SVECTOR/point grid and polygon field stores. No matched opcode sibling reaches0.70 for this function (or39FB8/45650), including the six new campaign matches; no global sweep repeated.

Best cost70. Original real code334 instructions (0x538 bytes); draft335, plus alignment makes both standalone arrays336 words. Original frame0xD8 and major homes all match: matrixB0, inputA8, points98, copied position6C. Inner for(j=0;j<2;j++) restores original saved-register allocation: step s7, half fp, bound2 s3, projected pointer s4, depth pointer s5, flag pointer s6. All UV/color/polygon code is otherwise aligned; one extra real instruction shifts later positional rows, so this is not a <=30-row production guard.

Concrete boundary: original outer rot/count/point-offset homes are0x60/0x90/0x54 and kind copy0x64; current0x74/0x84/0x60 and kind copy0x5C. The preheader adds move v1,zero after the original move v0,zero, stores the wrong counter homes, and the outer latch updates/stores in a different order. Geometry/matrix homes and inner loop are already correct. Scratch full listing is private build/T24502/dee4-control0.diff only.

Bounded lineage: old T-19028 typed draft370rows; matched donor grid212cost; explicit outer state230; recovered local order plus implicit half/inner-offset expressions108; donor inner for-loop gives70. At108 the read-only trace had half priority55.5 ahead of bound2 55, projection pointers50 and step41.33; the inner for form fixes that allocation, so do not diagnose the current70 version using the stale108 trace. Explicit half / |0 view168, both outer/inner for130, an extra inner zero-use230, implicit point offset without count192, count-zero use238. Fresh own implicit-int font39424 view also left its513cost unchanged. Do not repeat these probes.

Replay in Docker:

```
python3 tools/cc.py drafts/T-24502/TACO/func_8013DEE4/context.c build/T24502/dee4-replay.o ido 5.3
python3 tools/funcdiff.py --built build/T24502/dee4-replay.o --resolve TACO:func_8013DEE4
```

The scratch context only shadows the legacy void declaration: target explicitly produces -1 at the distance exit, and the existing T-19028 draft therefore uses s32 with fallthrough elsewhere. Production header/callers have not changed. A later matching promotion must correct the own header declaration and prove all callers/default bytes.
