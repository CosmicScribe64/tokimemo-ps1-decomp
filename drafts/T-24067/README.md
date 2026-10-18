# ETC album-copy checkpoints

T-24067 owns only func_80141ABC in src/ovl/ETC/80140F50.c. Production retains assembly. These are source-only diagnostics, not default C matches.

| Source | Cost | Added/removed rows | Remaining differences |
| --- | ---: | ---: | --- |
| body.c, indexed loop with early continue | 8 | 8 | Four second-copy t5/t6 words |
| indexed-body.c, earlier indexed loop | 16 | 16 | Four end/save assignment words, four second-copy t5/t6 words |
| cursor-body.c, earlier cursor loop | 16 | 20 | Two preheader HI/LO swaps, two stack stores, four copy words |

All have frame 0x48 and 75 instruction words. Safe replay uses the existing Docker image, serially:

```
python3 drafts/T-24067/replay.py
python3 drafts/T-24067/replay.py --variant indexed
python3 drafts/T-24067/replay.py --variant cursor
```

The indexed body walks all 256 GsRec75E records through the actual GameState array. Induction elimination generates the original byte cursor and offsets. Existing Work800E7D10.unk_058 supplies the 0x800-byte album; count/mode are unk_027[1]/[0]. Byte-only aggregate assignment gives the unaligned word copies. Reading the stored mode byte for the final printf restores the tail schedule.

Only the count-initializer/for source-line group is FAKE in the indexed bodies. Removing that grouping changes the preheader schedule. The format identity used by the earlier cursor body is unnecessary here and has been removed. Canonical fields cover all addresses, with no alias, header or symbol export changes. Safe logs are build/etc24067/continue-safe.log, indexed-safe.log and cursor-safe.log.

The early-continue form rejects a record when any flag is zero or the masked selector differs. It preserves the same reads, order and copies, and corrects all saved end/save allocation words. Exact tagged full-TU replay under `--no-build --resolve --version default --unit ETC` reports only four differing instructions: rows51/52/54/55 use t5 for the second unaligned copy word where the original uses t6. This is an explicit-object diagnostic; production remains its assembly fallback. Current eight-row source is a further gain, not a claim that the known B/F compiler analogy has been verified with a private4.1 target compile.

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

Prior destination controls already cover base/index operand order, byte/integer pointer additions, root-array offset11, count postincrement and computing destination before increment. They either normalize to the same constant use, reverse an ADDU operand, or add live addresses/instructions. No evidence yet shows a destination spelling that places end ahead of save while retaining the indexed shape. The former gap about constant bit creation is answered below by an approved private logging diagnostic.

Parent approved a separate creation-event logger, with no changes to the shared read-only tracer or production tools. `trace_creation.py` requires the existing generated base source SHA256 `59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c`. It adds only logging and native logging-context variables. The generated diagnostic source hash is `ef023e424f030f4b96648257e57dac4c67f6372a124785d2c1f2ed0a6f5fe78f`; measured private binary hash is `b55d0e3d5c2702ab1abc35eda0ec21c16e396c93f9725ebba4515769e00971d7`. Both stock and private full objects have SHA256 `6ad70e1533db02398f0a136b6048880fdf78aa37942d55c96815df21eac6cc99`, including equal code/data/relocations. Evidence is in ignored build/etc24067/creation-v3/indexed.log, audit.log and provenance.json.

New address bits are assigned on first eligible encounter during `makelivranges`, after induction elimination. The `func_464e50` address arm calls `newbit` only when the IChain has no bit yet; generated address ranges can also use another path into `newbit`. The logger at `newbit` with the current range-preparation block captures all five events:

| Bit | Block | Global identity | Addend |
| --- | ---: | --- | ---: |
| 53 | 0 | Work800E7D10, symbol99 | 0x58 |
| 54 | 2 | first format, symbol100 | 0 |
| 55 | 7 | Work800E7D10, symbol99 | 0 |
| 56 | 8 | GameState, symbol7 | 0x800 |
| 57 | 10 | final format, symbol101 | 0 |

Thus save's bit precedes end because the copy block is traversed before the loop test. Endpoint introduction during induction elimination alone does not establish its final numbering. One source hypothesis derived from this evidence uses the documented T-16552 `goto first; for (; i < 256; i++) { first: ... }` form, with count/i initialized before the jump. It bypasses the entry test and puts the lexical test before the body. The compiler normalizes it to the same16/16 result, so that single control is rejected; no goto/layout sweep was run.

To reproduce the logger, mount the existing /private/tmp/t24047-uopt read-only at /scratch in the existing image, then run serially from /work:

```
python3 drafts/T-24067/replay.py --variant indexed
python3 drafts/T-24067/trace_creation.py
cc -std=c11 -Os -fno-strict-aliasing -I/scratch/recomp -o build/etc24067/creation-v3/trace-create build/etc24067/creation-v3/trace-create.c /scratch/recomp/build/5.3/libc_impl.o /scratch/recomp/build/5.3/version_info.o -lm
python3 tools/uopt_eval.py compile build/etc24067/archive/indexed/source.c build/etc24067/creation-v3/indexed.o --uopt /work/build/etc24067/creation-v3/trace-create --trace --jobs 1
python3 drafts/T-24067/trace_creation.py --audit
```

The audit must pass before using event evidence. This is diagnostic logging; no allocator decision, compiler option, generated instruction, production tool or source exception is changed.

The current 5.3 copy-state diagnostic is preserved separately in store-control.c. A FAKE volatile stack-word store of count immediately before the required copy changes its second-word scratch from t5 to the expected t6. All four former copy-register differences disappear. This is rejected for promotion: it adds `SW s1,0x48(sp)`, enlarges frame0x48 to0x58 and changes five branch offsets. Exact tagged full-TU replay reports15 differing rows, not a match:

```
python3 drafts/T-24067/replay.py --variant store-control
```

Logs are build/etc24067/store-control-safe.log and archive/store-control/diagnostic.txt. The source is a counterfactual state probe, not a reconstruction of an original required store. Volatile qualifiers on the aggregate copy itself are neutral8/8 and rejected. No 4.1 target comparison or old-compiler necessity is inferred.

Faithful store sequencing offers no immediate substitute for the extra store. Required printf argument stores must precede the call that consumes them. Count/mode stores belong after the loop; moving them into the copy path changes global mutation across subsequent calls. The required first-word copy store uses the dynamic destination register, which the existing store-kind evidence leaves in state F. Splitting into two four-byte aggregate assignments was already rejected (31 rows, both word copies using AT). Further analysis must examine the actual backend address/state classification; it cannot assume that adding a source store is a valid solution.

Parent authorized a read-only backend reference from existing local inputs. It was generated in Docker, never compiled or executed as a replacement compiler:

```
/scratch/recomp/build/recomp.elf --conservative /scratch/recomp/ido/5.3/usr/lib/ugen > build/etc24067/ugen-reference/ugen.c
```

The existing /private/tmp/t24047-uopt mount was read-only at /scratch. SHA256 hashes are ugen `4d6ae5a5ea8a6ee77bfcc4c95195800823d4b1217e757cc516e219d05bbf5345`, recomp.elf `ac03f6a0b8c7d917a4ff4da5067d32fb52739077ed73ec98d4f214e1f63fc483`, and generated reference `4079660f9ebb068a791c6f54abb67cf7c3bb792207c89a6eb9ac2d9e146341ec`. The large reference remains ignored, rather than being committed with the source packet.

In f_eval_mov (0x42A078; reference line50898), L42aa44 allocates/frees one scratch, then L42ab84 selects AT plus the second scratch for this unaligned tiny-copy path. Word stores go directly through emit_rob; they do not revisit the scalar store/address classifier. Thus the first aggregate word store cannot stand in for the rejected extra direct stack store. The alternative paths L42a9e0/L42ab18 use additional scratch allocations for large offsets; the actual record offsets and eight-byte length do not satisfy those conditions. Forcing those paths is not established as a faithful source change.

get_free_reg (0x43E124; line82429) uses its dtype bitset0x5010000 to select paired registers for wide integer nodes. The narrowing helper at0x4365B0 selects the low word on MIPS1. This suggested unsigned 64-bit destination arithmetic narrowed to u32 as a single private control, conditional on an existing project C89 type. That condition fails: common.h defines only s8/u8/s16/u16/s32/u32, and include has no 64-bit integer typedef. Parent agreed to reject the non-C89 extension before compilation. No control was run and no allocation result is claimed. The four-word best, production source, interfaces and toolchain remain unchanged.
