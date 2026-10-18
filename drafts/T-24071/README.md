# T-24071 menu selection source checkpoint

Only ETC func_80144DF0 (388 original bytes) in src/ovl/ETC/80143240.c
is assigned. Production remains INCLUDE_ASM. The current private draft has
57 resolved +/- rows, not a C match or verified-byte gain. No neighbor,
header, configuration or compiler was changed.

Current fields replace stale scalar aliases. The slot-status index uses the
selection captured before k_disp_inc, as in the original assembly; the old
plain draft incorrectly reloaded the global after that call. The signed
selection and unsigned ready-byte load are preserved. Historical plain67 and
typed130 are different contexts, not new measurements.

| Source control | Rows | Frame / selection spill |
| --- | ---: | --- |
| valid current-type baseline |61|30 / 2C|
| consistent GameState pointer |65|30 / 28|
| selected-slot byte comparison capture |61|30 / 2C|
| real Work.slot_status / selected_slot members |61|30 / 2C|
| actual button-word snapshot |57|30 / 28|
| stored selected-field readback as required argument |57|30 / 28|
| named required status result, first declaration |57|38 / 2C|
| same status declaration after existing locals |57|30 / 28|

The original has frame38/spill28. body.c retains the button snapshot. Both
0x20/0x40 tests share its one required F88 word load, and the selection spill
moves to the original offset. The status controls trade frame and spill
rather than improving the draft; no status local, guessed buffer or return
override is retained. Stored-field readback adds no load and gives no gain.

Matched MAIN func_8005D448 is a read-only reference with different call order
and arms. Its FAKE stack reserve and s32 return view are not transferred.
MAIN func_8005CA94 records 24 failed per-arm byte/word, mask and argument
assignment spellings (matching-notes around6928); those were not repeated.

Remaining differences include frame30 versus38, per-arm byte-mask value ranges,
selected-slot load register and later scratch rotation. Original uses
andi v0,v1,255 and shares v0 across stores/call argument. The draft eliminates
masks used only by byte stores and narrows the call argument in a0. This does
not prove that no legal source can match it.

Replay one retained source serially inside existing tokimemo-decomp-arm64:

```
python3 drafts/T-24071/replay.py
python3 drafts/T-24071/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper replaces only the assigned
fallback in a private full TU, strips the archive-status comment to restore
measured layout, validates explicit-object --resolve --version default
--no-build output and checks production unchanged. This is separate from
actual default C proof. Outputs stay in ignored build/etc24071/replay.
Only source and prose are retained; no original assembly or compiler dump.
