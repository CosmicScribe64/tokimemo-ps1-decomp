# T-15700 TACO and TAIIKU compile-repair archive

All **28/28** reserved scopes compile as complete translation units in the existing Docker IDO 5.3 pipeline: TACO 17/17, TAIIKU 11/11. All remain **NON_MATCHING**. No diagnostic zero, literal-byte match, production source edit or matching-coverage gain is claimed. Target code extents total 29584 bytes.

| Scope | Code bytes | Diagnostic rows |
| --- | ---: | ---: |
| TACO:func_80137264 | 812 | 172 |
| TACO:func_8013A790 | 1492 | 574 |
| TACO:func_8013D528 | 784 | 259 |
| TACO:func_8013D838 | 832 | 274 |
| TACO:func_8013DEE4 | 1336 | 429 |
| TACO:func_8013E778 | 812 | 263 |
| TACO:func_8013EAA4 | 1948 | 626 |
| TACO:func_801424A8 | 880 | 260 |
| TACO:func_80145650 | 1908 | 709 |
| TACO:func_801487A4 | 948 | 366 |
| TACO:func_801497EC | 912 | 348 |
| TACO:func_8014CB24 | 1756 | 598 |
| TACO:func_80153B38 | 832 | 293 |
| TACO:func_80154960 | 1556 | 192 |
| TACO:func_801553AC | 1084 | 364 |
| TACO:func_80159260 | 1048 | 308 |
| TACO:func_8015ACCC | 1260 | 515 |
| TAIIKU:func_80132BD0 | 592 | 278 |
| TAIIKU:func_80132E20 | 692 | 525 |
| TAIIKU:func_80134060 | 1348 | 364 |
| TAIIKU:func_80135F3C | 1676 | 556 |
| TAIIKU:func_80138DF8 | 1336 | 236 |
| TAIIKU:func_8013D114 | 320 | 98 |
| TAIIKU:func_8013FC5C | 488 | 148 |
| TAIIKU:func_80143DD0 | 384 | 107 |
| TAIIKU:func_80143F50 | 956 | 241 |
| TAIIKU:func_80144464 | 572 | 185 |
| TAIIKU:func_80147B14 | 1020 | 540 |


Each scope retains real repaired C, declarations, source TU, full preprocessed TU, complete compiler stdout/stderr diagnostics (preprocessor source stdout is retained separately), comparison, hashes and provenance. Original attempts and their raw m2c/body/context remain immutable under drafts/T-14500. Every earlier alternative is referenced in provenance.json. First complete diagnostic inputs before reconstruction are preserved under each scope's attempts/first-full-diagnostics; these are historical failed inputs, not final compiling alternatives.

Source baseline is the frozen 48362009 census; unrelated prior accepted work does not alter the owned TUs. Historical taco-13800/taiiku-13600/cx2/cx9 source trees were inspected and no implemented body for these exact targets was found. The first complete-diagnostics pass repaired declaration/prototype conflicts; remaining work uses existing SDK SVECTOR and observed coordinate arrays, exact copied table extents, existing aggregate members, packed-word/byte views, and the canonical TcPos-by-value signature. No empty stubs, inline assembly or target ASM fallbacks substitute for C.

One archived-only companion type completion is required: func_80154960 consumes func_80156178's result. Its context gives that helper an s32 return and returns its existing final func_8015ACCC call result. This follows the observed consumed v0 ABI; the production matching helper is unchanged. All other production C is also untouched. Recovered source types and instruction differences still require independent semantic/matching work.

To recompile all final archived bodies, run inside Docker from this checkout with immutable census mounted read-only at /ref:

```
python3 drafts/T-15700/reproduce.py
```

The driver accepts scoped names for a subset and preserves full compiler diagnostics. reconstruct.py records the one-time derivation from the first-pass inputs; do not reapply it to already reconstructed bodies. finalize.py normalizes archive metadata and removes duplicated preprocessor source stdout from diagnostic logs without dropping compiler diagnostics. No binaries, generated assembly, disc files or game assets are archived.
