# T-25054: ETC results scene

func_8014790C in src/ovl/ETC/80146400.c matches all 282 instructions / 1128 bytes in the default C build. It draws the exam results and fourteen ranking rows. The saved T-24909 body was the starting point; its reported 44-row difference and claim of clear loops were stale.

## Source mechanisms

- Remove the separate n counter. The late-folded zero in `(i + 0x61) | ((u32)i < 0)` keeps the addition before multiplication, preserving the target address calculation.
- Initialize the walked record base in a one-iteration do loop with `(u32)D_800E6280.unk_041 < 0` as its condition. This comparison is false for every input but survives uopt, preventing it from sharing the walked and indexed bases. The byte is nonvolatile and the read emits no final instruction.
- Index the three result arrays by i. Their strength-reduced pointers carry noalias metadata against the stack, allowing the last call's stack argument to occupy the target delay slot. Put the record-walk increment before the counter increment to retain the target update order.
- Keep the counter initialization and do on one source line for address scheduling. Use the existing DisplayActor44 fields for every pool store.

The initializer, identity, source-line grouping, oversized buffer and legacy non-void fallthrough are marked FAKE. The only caller ignores the result. A void definition removes a required switch copy. No new calls, writes or input-dependent behavior are introduced.

## Evidence and controls

All counts and aligned costs come from full translation units compiled in the existing Docker toolchain.

| Candidate | Words | Cost | Finding |
| --- | ---: | ---: | --- |
| T-24909 archive | 282 | 66 | Current measured baseline |
| Remove n; plain i+97 | 279 | 79 | Constant distributed into displacement |
| Late-zero index OR | 281 | 21 | Loop arithmetic and registers match |
| Pointer casts and array views | 281 | 21 | Base still shared |
| Biased pointer with late identity | 282 | 12 | Retains unwanted add-zero |
| One-iteration initializer using switch byte | 282 | 26 | Separate bases; earlier switch live range changes |
| Same initializer using unrelated byte | 282 | 6 | Correct bases and registers |
| Same-line loop preheader | 282 | 4 | Only call argument schedule remains |
| Both coordinate arrays indexed | 282 | 12 | Delay slot fixed; induction setup differs |
| Remove old pointer locals; reorder increment | 282 | 4 | Only array-base setup differs |
| Index all three arrays | 282 | 0 | Literal match |
| Canonical record fields | 282 | 0 | Retained in production |
| Void return type | 281 | 3 | Removes the target switch copy |

Simple late-zero call arguments, source-line changes around calls and pointer biases did not fix the last delay slot. The array-index noalias mechanism did. An early loop-base batch had a whitespace mismatch and made no source change; corrected controls were rerun and rejected. Private source probes assert unchanged production and preserve full units, phase captures, measurements and diffs under build/t25054/.

## Verification

- Default `python3 tools/funcdiff.py --resolve ETC:func_8014790C`: MATCH, 1128 linked bytes equal target; fallback removed.
- `ninja -j2 build/ovl/ETC.ok`: checksum OK, SHA1 681c93b6e57c885f86e074c885a8bf6c3cca2319.
- Headers and globals checks pass; prototype branch check reports 0 new disagreements and 15 known.
- Root independent standards/spec review passed with no findings; full 27-binary integration remains pending. Ticket remains In Review until that gate passes.

No game binaries, assembly listings or private compiler artifacts are committed.
