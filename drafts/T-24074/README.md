# T-24074 record-scene source checkpoint

Only ETC func_80144224 (796 original bytes), src/ovl/ETC/80143240.c,
is assigned. Production remains INCLUDE_ASM. Best private source is166 resolved
+/- rows, not a C match or verified-byte gain. All matched neighbors/helpers,
including4407C and441DC, remain untouched. No header/config/compiler edits.

The fresh baseline corrects semantic defects in retained T-17300 source:

- Original44344 passes frame-counter/701 to4407C. The old draft omitted it,
  losing the now-proven arg14 positioning behavior.
- Matched441DC takes exactly two arguments; extra a2/a3 values in the first
  calls were stale live registers rather than real arguments.
- D5014C is the real16-word order table; Rec24[0/1].unk00 are the initial flags.
- Original reloads the frame counter after each helper-call group. The old
  source kept its initial snapshot even across those calls. Correct reloads
  preserve mutation semantics; the final debug print reads the global again.

| Control | Resolved +/- rows | Frame |
| --- | ---: | --- |
| faithful current full-TU baseline |209|58|
| join disjoint phase/fade and spill aliases |203|48|
| direct real-table lookup/counter mutation |186|40|
| existing unsigned GsWord.u frame view |187|40|
| quotient assignment before remainder |187|40|
| consistent GameState pointer/direct frame reads |226|40, rejected|
| inline phase remainder expression |166|38, retained|
| signed quotient declaration |166|38, neutral|

Frames are hexadecimal; original38. Direct lookup removes three m2c-only
locals and omits redundant old counter writes. Baseline/join forms preserve
those observed writes as marked FAKE diagnostics. Unsigned GsWord.u matches
DIVU semantics and removes a duplicate initial frame load, but alone gives
no score gain. The quotient's maximum is6126914, so its signed-local control
preserves all possible values; it is neutral and not a type sweep. Joining
disjoint values removes copied stack aliases without changing calls or reads.

Inlining the phase expression removes its copied remainder web. The original
frame38, initial frame loada3 and order-indexv1 then agree. Remaining gaps
include quotient a0/sp34 versus original scratcht6/sp30, an extra frame-value
copy into a2, real-table5014C versus original biased50150 address setup, and
omitted redundant counter stores. Later fade/register scheduling also differs.
No exact allocator rule or faithful new source form is established. No
instrumentation, aliases, forced stores or compiler-impossibility claim follows.

The source retains the current implicit-int dispatcher call view as s32
fallthrough; no return value is consumed by the current caller. Its original
return type is unproven, and no return-type control was performed.

Replay one retained form serially in existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24074/replay.py
python3 drafts/T-24074/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper substitutes only the assigned
fallback in a private full TU, strips the archive-status comment to preserve
measured layout, validates explicit-object --resolve --version default
--no-build output and checks production unchanged. No default ASM fallback
proof counts as C progress. Outputs stay ignored under build/etc24074/replay;
only source/prose is retained, with no original assembly or compiler dump.

One parent-approved post-review control uses a real s32 pointer cursor and
selected value only in the interior table arm: selected=*order++, conditional
selected=*order++, then helper(selected). Original biased-load assembly supports
this distinct source hypothesis; prior negative-index forms did not test it.
The valid current full-TU diagnostic measures241 rows/frame0x40, versus
best166/frame0x38. Order index changes fromv1 toa1 and quotient spill fromsp34
tosp3C. Rejected; no cursor sweep follows. Replay with --variant postinc-cursor.
The166-row body and all required post-call frame reloads remain unchanged.
