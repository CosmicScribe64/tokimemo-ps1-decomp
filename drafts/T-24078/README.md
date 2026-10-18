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

| Current full-TU source | Aligned cost | Printed +/- rows | Words | Frame |
| --- | ---: | ---: | ---: | ---: |
| baseline.c |638|800|594|0xC0|
| shared-temps.c |638|800|594|0x98|
| sdk-uv.c |678|790|582|0x98|
| uv-pair.c |638|800|596|0x98|
| case-height.c |638|800|596|0xA8|
| inline-height.c |678|790|582|0x98|
| tile-width.c |648|806|594|0xA0|
| indexed-page.c |638|800|594|0x98|
| body.c / word-coordinates.c |604|670|592|0xA0|

Original588 words/frame0x90. Rank with tools/funcdiff.py diff_score on resolved
instruction keys (insertion/deletion1, substitution2). Printed rows use different
alignment rules for equal/unequal lengths and are diagnostic ONLY. The earlier
790-row wording did not establish improvement: sdk-uv has WORSE aligned cost.
The retained body is now the604-cost word-coordinate form; it has fewer
words but a larger frame than the638-cost joined form. None is a match.

The shared-temp control joins12 same-type aliases whose lifetimes occupy
mutually exclusive switch arms, preserving expressions and store order. It
removes40 frame bytes, not the hypothesized48. SDK UV assignments remove
inherited signed-byte casts but promote scalar coordinates to saved registers,
spill mode/tpage and replace original MULTU136 with shifts. The FAKE-tagged UV
word-pair view restores LW and saved136/MULTU, but adds two words and retains
early160-arg0 plus spilled320. Splitting height temporaries by case only enlarges
the frame; inlining height is no gain. A named136 tile-width local costs648 and
adds a home, so is rejected. No source form tested simultaneously restores the
original stackUV50/54, saved136/320 and DIV-before-MULTU schedule. This is a
concrete unresolved source/allocation gap, not an impossibility claim.

Initial malformed parentheses during field substitution failed compilation and
were repaired before any score; that failure is not counted as a baseline.

Replay in existing Docker, one compiler at a time:

```sh
python3 drafts/T-24078/replay.py --variant shared-temps
python3 drafts/T-24078/replay.py --variant baseline
python3 drafts/T-24078/replay.py --variant uv-pair
```

The helper compiles only an ignored full-TU copy, invokes explicit default
resolved funcdiff and validates its diagnostic status. It accepts exactly the
ASM fallback or exact pinned body after any promotion, and checks production
text remains unchanged. Output: build/etc24078/replay/<variant>/diagnostic.txt.
The retained s32 fallthrough is conservatively FAKE-tagged: current implicit-int
callers ignore its result, but original return type is unproven.

The remaining eight-byte frame excess and expression scheduling require a new
source mechanism before further probes. No allocation/type sweep or compiler
instrumentation; ownership remains active pending focused direction.

The parent-authorized bounded Astra read-only review supplied one untried form:
matched D32C derives pageX from its loop index. Against shared-temps only,
indexed-page removes var_fp declaration/init/increment and passes
0x140 + var_s4 * 0x80 to ECB0. Original two iterations make this equivalent.
Measured neutral638cost/800rows/594words/frame98; compiler recreates320 induction
but spills it atsp50 instead ofsp60. Mode remains s7, constant160 s6,136 fp.
This does not restore original fp320 or frame90. Source and diagnostic preserved;
no other review hypothesis or new control was tested. Astra idle; no further
unsupported probes or instrumentation. Ticket remains active pending direction.

Read-only arithmetic/lifetime audit: relationships.md maps all four original
arms and shared tail, with exact full VAs. No missing operation, wrong formula
or address was found. The three inherited s16 coordinate temporaries narrow
earlier than original register values; later low16-only uses mean this is NOT a
proven behavior bug, nor proof of original C declarations. Parent approved one
structural control changing ONLY temp_a1/temp_a2/temp_v1 to s32 against the
joined638 baseline. It reaches604cost/670rows/592words/frameA0 (original588/90).
Whole object SHA25667fbefa6e2af48b3ac7a8e508b774cea8c8efd2c4f824bce6269755d29163a00.
The first divergent arithmetic still places MULTU i*136 before the first signed
Q11 DIV, whereas original executes DIV before MULTU. UV signed-byte loads and
early160-A remain. No additional width, UV combination or ordering probe begun.
