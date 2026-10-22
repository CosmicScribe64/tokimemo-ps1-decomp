# T-25055 TEL qualification dialogue

Base: `633d95356`. Target: TEL `func_80136724`, 6,580 bytes / 1,645 words, production file `src/ovl/TEL/80132000.c`. Prior donor archive `drafts/T-25024` and checkpoint `3b257980c` remain intact. All measurements use the complete translation unit and the normal IDO5.3 pipeline in existing Docker image `tokimemo-decomp-arm64`, with the original disc mounted read-only. No compiler, flags or shared declarations changed.

## Causal reconstruction

The pointer donor initially has 1,643 words, 1,403 changed ordinal positions and aligned cost970. A missing reserved register explains the apparent first temporary deficit at word85. The original reserves both t0 and t1 throughout the function. Its case0x40 selector is the global D_801405E4; the donor's ent local separates that range and leaves t1 available to ugen.

1. Use D_801405E4 directly for case0x40 selection and indexing. The writes cross no calls or intervening alias reads before sndisp. The compiler emits the target's deferred global store. The extra reserved caller register restores most of the function's temporary sequence.
2. Keep the case6 maximum and final total in the same existing total local. Both maximum branches define it before the eligibility calculation. This restores the late zero, its dependency, the NOP, and the correct instruction count.
3. Add a defined always-false unsigned test of the girl byte immediately before reading the rank array. The extra uopt use changes allocation and address sharing; ugen removes it. It repairs40 of41 remaining instruction positions. The field is ordinary nonvolatile memory and is already read immediately afterward; no new runtime load remains. Marked FAKE.
4. Use hp*1 in the second case0x40 comparison. This preserves expression-form operand order in uopt and disappears before emitted code. Marked FAKE.

Unused scalar slots and the inferred text-buffer extent are also explicitly marked FAKE. The rank and point arrays follow the matched helper's16-byte and16-halfword writes. The packed TelDateEntry has all32 bits assigned before the by-value call. Target frame0x198, text0x78, rank0x68, points0x48, packed word0x44 and total0x17C agree.

| Control | Candidate words | Changed positions | Aligned cost |
| --- | ---: | ---: | ---: |
| Pointer donor | 1643 | 1403 | 970 |
| Direct global selector | 1644 | 1352 | 127 |
| Exact byte view, local selector | 1643 | 1403 | 970 |
| Shared total alone | 1644 | 1366 | 969 |
| Direct selector + shared total | 1645 | 41 | 78 |
| Reuse random local for eligibility | 1645 | 41 | 78 |
| Reuse rank local for difference | 1645 | 41 | 78 |
| Byte view of girl call argument | 1645 | 41 | 78 |
| Additional unsigned-false girl use | 1645 | 1 | 2 |
| Neutral second comparison expression | 1645 | 0 | 0 |
| Remove third strcat argument, case0x22 | 1645 | 0 | 0 |
| Remove third strcat argument, case0x2F | 1645 | 0 | 0 |
| Remove both third strcat arguments | 1645 | 0 | 0 |
| Remove both integer text casts | 1645 | 0 | 0 |
| Final annotated and cleaned source | 1645 | 0 | 0 |

Changed positions compares resolved instruction keys at identical ordinal indices. Aligned cost counts insertions/deletions, with substitution costing two. Neither metric by itself is a literal linked-byte proof.

## Replay

Run at repository root in Docker. The probe accepts a function body and replaces only the target definition in the current full TU, preserving the other functions and production flags. It also supports the old INCLUDE_ASM body. Captures and instruction listings remain private in build/T-25055 and must not be committed.

```
python3 drafts/T-25055/probe.py drafts/T-25024/pointer-baseline.c --capture
python3 drafts/T-25055/controls.py
python3 drafts/T-25055/range_controls.py
python3 drafts/T-25055/probe.py drafts/T-25055/combined-41.c
python3 drafts/T-25055/probe.py drafts/T-25055/girl-profit-1.c
python3 drafts/T-25055/probe.py drafts/T-25055/matched.c
python3 tools/funcdiff.py --resolve --version default TEL:func_80136724
ninja -j2
python3 tools/check_headers.py
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

`capture_stage.py` forwards unchanged arguments to the normal cvt/ugen/frame passes while saving stage files before and after. Captured and uncaptured baseline metrics agree. A diagnostic attempt to request a textual ugen listing with -S was unsupported and produced binary data; no result or hypothesis relies on it.

To replay the optional allocator summary, mount the existing traced uopt at `/scratch/variants/trace`, then run:

```
python3 tools/regdiff.py TEL:func_80136724 --src build/T-25055/combined-41/guarded.c --uopt /scratch/variants/trace --scratch /work/build/T-25055/regdiff-combined -v
python3 tools/regdiff.py TEL:func_80136724 --src build/T-25055/matched/guarded.c --uopt /scratch/variants/trace --scratch /work/build/T-25055/regdiff-final -v
python3 drafts/T-25055/trace_summary.py
```

The first regdiff command exits1 because that control differs; continue with the final capture and summary.

## ABI and verification

The default-C proof reports `MATCH (linked function bytes equal target (6580 bytes))`. The full build passes all27 binary checksums; an incremental build after final neutral cleanup passes, and fresh hashes of all27 binaries match. See checksums.json. Headers and migrated-global checks pass. Prototype branch check reports0 new disagreements,15 known and branch check OK. Production has no guard or INCLUDE_ASM fallback for this target. Coordinator directly reviewed standards/spec and resolved all findings; ticket is In Review for integration.

The `func_80052060` header uses an unprototyped declaration, while its matched definition takes four s32 arguments. Its table-pointer-to-s32 casts follow that actual 32-bit ABI. In contrast, set_kanji_string has a fully prototyped u8* fourth argument; its inherited integer text casts are removed after a neutral control. The two inherited strcat month arguments are also neutral, independently and together, and are removed. The canonical two-pointer declaration is unchanged. Reproduce these controls with abi_controls.py and probe.py.

## Allocator evidence

allocator-summary.json contains selected ranges from the captured native trace. The girl range is identified by static symbol6 and byte offset3935 (0xF5F); regdiff's source-name heuristic incorrectly associates some accesses with the later record loop, so the summary labels it from the exact offset. The added false read increases its recorded uses from7 to9, its block count from4 to5, and priority from0.25 to0.4. It moves from t1 to t0; D_801405E4 moves from t0 to t1. lim changes a0 to v0 and hp changes a1 to a0, both retaining priority1.5. total remains v1 at priority2.25. Allocation-order fields in this JSON are zero-based. The traced final compile independently matches the target instruction sequence.

results.json contains candidate and full-TU source SHA256 for every measured control. context-sha256.json pins the final production source, relevant declarations, normal pass scripts, stock compiler stages and instrumented uopt. Only compact metadata and reconstructed function sources are archived. The full-TU inputs, emitted objects, assembly and ucode remain private in build.
