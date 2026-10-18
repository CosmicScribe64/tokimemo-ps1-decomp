# ETC album-copy checkpoint

T-24067 owns only func_80141ABC in src/ovl/ETC/80140F50.c. This source-only checkpoint has diagnostic cost18 / 28 added-and-removed rows, exact frame0x48 and 75 instruction words. It is not a default C match. Production retains its assembly fallback.

Run in the existing Docker image, serially:

```
python3 drafts/T-24067/replay.py
```

The source walks256 eight-byte GsRec75E records using a byte-advanced GameState cursor. Existing Work800E7D10.unk_058 supplies the0x800-byte album; count/mode are unk_027[1]/[0]. Byte-only aggregate assignment gives the observed unaligned word copies. The endpoint uses the canonical GameState address +0x800, so no separate alias, header or symbol export is needed.

Compared with the previous27-row lead, reading the saved mode field for the final printf restores that tail. An eliminated format-pointer identity test is explicitly FAKE: it retains the format range and produces s2=format, s3=end, s4=save. Original source is unknown. Explicit endpoint and format locals preserve the target frame.

Remaining differences are the preheader instruction order, reversed equal-value stack-argument stores and second-word copy scratch t5 rather than t6. Grouping initializers reaches26 rows at the same cost18. Canonical field endpoints, argument widths, signed save stores, pointer variants, counter postincrement, aggregate alignment, K&R/SDK printf views and the recorded identity controls do not close these differences.

Private allocation diagnosis used the existing read-only /private/tmp/t24047-uopt/variants/trace. An identical-input stock control has identical .text, .rel.text, .rodata and .rel.rodata. Before the format identity, hoisted format/save/end bits52/53/54 all have priority3.33333325; their first-use tie explains the wrong s3/s4 allocation. No production compiler change is proposed. Logs and traces remain ignored under build/etc24067; no game assembly or binary is archived.
