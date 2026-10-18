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

Follow-up controls preserve the indexed 16/16 checkpoint. An inclusive `i <= 255` bound and indexed do-loop both reproduce 16/16. A local limit, u16 index and top-exit loop give 80, 90 and 76 rows respectively because induction elimination is lost. Two four-byte aggregate assignments give 31 rows; a byte-copy loop gives 71. Explicit record/album pointer locals give 32/33. These are full-TU private comparisons, not default C proofs.

The existing tracer on the indexed source reports format/save/end bits 54/55/56 at equal priority 3.33333325; save is numbered before end. `build/etc24067/indexed-trace.log` records the allocation. The separate copy discrepancy is consistent with the already measured B/F block-copy state distinction in wiki/ido-52-evaluation.md (T-20710): IDO 5.3 changes state at conditional block starts, while the historical 4.1 control retained B. The current native listing has two `.set noat` directives before the eight-byte copy and uses t5 for its second word. That is supporting analogy, not a new 4.1 experiment or proof that source recovery is exhausted.

A single conditional-identity bound, `(i == 0 ? i : i) < 256`, tests whether a retained control-flow boundary changes the tied endpoint range. It instead loses induction elimination (cost70/72 rows), so it is rejected; no further identity-bound variants are retained.

Stock stage capture can be reproduced with `python3 drafts/T-24067/capture.py` in Docker. It first replays the indexed source, copies uopt's actual input/output without modifying them, then requires equal full-TU code/data/relocations against the ordinary compile. Validated sizes are .text 7216, .rel.text 5632, .rodata 1488 and .rel.rodata 336 bytes. Files stay under build/etc24067/archive/ucode. No compiler binary, option or transformation changes.

The indexed frontend contains scalar `i < 256`, with no endpoint address. The post-uopt stream contains the endpoint and these materializations:

| Value | Allocator bit | Priority | Output register |
| --- | ---: | ---: | --- |
| format | 54 | 3.33333325 | 18 / s2 |
| save root | 55 | 3.33333325 | 19 / s3 |
| GameState root + 0x800 | 56 | 3.33333325 | 20 / s4 |

The exact cursor checkpoint has a different balance: format bit67 priority5, explicit end bit3 priority2.75, save bit68 priority2.5. End's entry store contributes1 and its loop use10 over four blocks; save has only10 over four. Thus cursor selects format/end/save correctly, while indexed ties save/end. The captures establish the input/output and final numbering; they do not timestamp the internal bit-creation phase.

An attempted bridge initializes the real end pointer and uses `i < ((u8 *)end - (u8 *)&D_800E6280) / 8`. It gives68 rows: uopt retains an address-typed subtraction of two loads from the same machine register plus800, division and an entry guard. It does not reduce the bound to256. Consistent unsigned integer address subtraction also fails (108 rows). An empty endpoint reference after printf creates an extra saved range (64 cost/66 rows). None of these forms is promoted.

Prior destination controls already cover base/index operand order, byte/integer pointer additions, root-array offset11, count postincrement and computing destination before increment. They either normalize to the same constant use, reverse an ADDU operand, or add live addresses/instructions. No evidence yet shows a destination spelling that places end ahead of save while retaining the indexed shape. The next diagnostic question is when the two constant bits are created: the existing trace shows final ranges but not constant identity at creation. A creation-event trace would distinguish a source-order lever from an endpoint that is invariably introduced after the save candidate; no new tracer or compiler modification has been made.
