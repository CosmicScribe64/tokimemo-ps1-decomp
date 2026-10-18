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


The parent-approved unchanged tracer passes same-input whole-object identity
before interpreting its output. Source SHA256 is
`18513169b0051a22a8833ec96928c814cbf9f3e1c53135757be76bb0318a99e0`;
stock/logged object is
`9eadc551036a57ba4c3329d6c40b164c8e8d0e5cce7f4a9f11d24514d5265a62`.
Equal sections: .text12736, .rel.text8552, .rodata736, .rel.rodata248.
The existing read-only tracer source/binary hashes are checked by
trace_fidelity.py; no new logging binary was built.

Mount /private/tmp/t24047-uopt read-only at /scratch in the existing Docker
image, then run these serially after replay:

```
python3 tools/uopt_eval.py compile build/etc24071/replay/buttons/source.c build/etc24071/trace57/baseline.o --uopt /scratch/variants/trace --trace --jobs 1
python3 drafts/T-24071/trace_fidelity.py
python3 drafts/T-24071/capture.py
```

capture.py copies stock uopt input, raw output and post-cvt output without
altering the stages. It requires whole-object equality and checks each of
the four code/data/relocation sections before any interpretation. Stage
files and logs remain ignored under build/etc24071.

The masked synthetic ranges64/65 have priority3 and receive color1/v0.
Selection10 has priority2/color2/v1; button global16 has priority0.5/color3/a0.
The captured final uopt output still contains selection AND255, conversion
from J to L, and a store to register2/v0. The byte store consumes that L value;
the argument separately converts through CVTL8 and back to J. Final assembly
uses a raw move into v0 and narrows a0 for the argument. Therefore this input's
missing ANDI is a later lowering effect, not an absent pre-allocation range or
proof that v0 was unavailable. The unchanged trace does not establish every
interference constraint. Previously measured word-local mask hoisting remains
a separate source obstacle; this packet does not justify a width sweep.
