# ETC album-copy checkpoints

T-24067 owns only func_80141ABC in src/ovl/ETC/80140F50.c. Production retains assembly. These are source-only diagnostics, not default C matches.

| Source | Cost | Added/removed rows | Remaining differences |
| --- | ---: | ---: | --- |
| body.c, typed indexed loop | 16 | 16 | Four end/save assignment words, four second-copy t5/t6 words |
| cursor-body.c, earlier cursor loop | 16 | 20 | Two preheader HI/LO swaps, two stack stores, four copy words |

Both have frame 0x48 and 75 instruction words. Safe replay uses the existing Docker image, serially:

```
python3 drafts/T-24067/replay.py
python3 drafts/T-24067/replay.py --variant cursor
```

The indexed body walks all 256 GsRec75E records through the actual GameState array. Induction elimination generates the original byte cursor and offsets. Existing Work800E7D10.unk_058 supplies the 0x800-byte album; count/mode are unk_027[1]/[0]. Byte-only aggregate assignment gives the unaligned word copies. Reading the stored mode byte for the final printf restores the tail schedule.

Only the count-initializer/for source-line group is FAKE in the indexed body. Removing that grouping changes the preheader schedule. The format identity used by the earlier cursor body is unnecessary here and has been removed. Canonical fields cover all addresses, with no alias, header or symbol export changes. Safe logs are build/etc24067/indexed-safe.log and cursor-safe.log.

The earlier cursor form retains two FAKE idioms: its format-pointer identity restores saved allocation, and its initializer/do line group improves scheduling. It complements the indexed form, whose schedule agrees but whose saved end/save allocation is reversed. Earlier source checkpoints are eadad3134 (cost 18 / 28 rows) and 47c7d6151 (cursor cost 16 / 20 rows).

Private diagnosis used the existing read-only /private/tmp/t24047-uopt/variants/trace. Identical-input stock and instrumented .text, .rel.text, .rodata and .rel.rodata agree. Before the cursor format identity, hoisted format/save/end bits 52/53/54 all have priority 3.33333325; first-use order explains their saved allocation. No production compiler change is proposed.

Relevant failed controls include pointer/copy alignment views, argument conversions and captures, explicit counter-update ordering, mixed local/field arguments, constant-first copy addresses and stack-backed printf temporaries. Signed != or unsigned indexed bounds retain 16 rows; explicit record/byte address bounds create a second induction pointer and worsen. The T-17001 masked-byte-store analogy does not directly apply to word printf arguments: its mask remains an actual ANDI. TACO's store-kind rotation rule is consistent with the listing, but a real stack temporary adds frame/storage/masking instructions here.

The authorized private IDO 4.1-ugen comparison is unavailable because no existing 4.1 compiler directory is present; the image has only 5.3/7.1. No download, install, new image or expanded search was performed. This does not establish a compiler defect. Detailed logs remain ignored under build/etc24067; no game assembly or binary is archived. Ticket remains In Progress.
