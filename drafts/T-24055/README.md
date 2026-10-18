# T-24055 ETC short-helper source pass

Zero of three matched, zero new C functions/bytes. Production src/ovl/ETC/80143240.c, headers, config and compiler are untouched. All three targets remain plain INCLUDE_ASM. Baseline one-compiler `ninja -j1 build/ovl/ETC.ok` passes, SHA1 681c93b6e57c885f86e074c885a8bf6c3cca2319. No default fallback match is counted as C progress.

Read ETC family, T-17300, T-9720/T-10030, T0018 records and prior setup/stride evidence before probes. Original model coordinates stride0x50 through TaiikuBig, object words stride0x10. Coordinate table D_801503C8 holds unsigned byte pairs: both old fill and scroll bodies incorrectly read their first byte as a word. Corrected before scoring. Source-only selected files are function bodies for full-unit insertion, not standalone C files. Old drafts remain unchanged.

func_8014354C (248 bytes): model.c uses the matched TAIIKU801406A8 model pointer sequence and current shared types. Original splits address0x801F9008 plus4; pointer set before mapping and advanced afterward reproduces that. The loop agrees; prologue/address construction uses s2 as the common high base where original separately constructs a0, and SDK SVECTOR tail stores still group differently (original vz,vx,vy). No novel win: selected13/13 freshly reproduces a historical result. Literal map argument form108 raw/12 aligned, current archived control112/16, byte pointer13/13, vector chained zero15/15 and vector-array alias13/13. The rejected array alias is FAKE marked in ignored probes; selected source retains real SVECTOR members.

func_80143758 (292 bytes): fill.c uses existing DisplayFixActor44 fields, correct unsigned table-byte reads, and the original separate record/flag alias walk. The halfword clear of the word view at+0x2C is explicitly documented. Current111 rows/109 aligned; simple index and single actor-base variants193/191 and194/192. Original hoists its many constants into a different set of registers and uses a seven-record end pointer. No padding or header declaration was added to force it.

func_80143968 (288 bytes): scroll.c corrects table-byte reads and the current actor's+0x2A position field. Current70/70; declaring the record index before the helper call preserves a meaningful across-call index but worsens to109/109 and adds a saved register/frame slot. Direct shared counter and precomputed index give74/72 (the expected constant68 multu appears); computing the index later gives89/85. Word-lvalue promotion remains74/72, implicit-int return liveness70/70. Original keeps the counter in a3 and writes it back only around calls/end; current compiler either caches it in a saved register or repeatedly reloads the direct global. The retained redundant initial writeback is FAKE marked as observed-store shaping; there is no compiler-impossibility conclusion.

These are bounded source/type/control-flow forms, not a broad permutation search. Final selected sources freshly reproduce13/13,111/109,70/70. All loose C, JSON inputs, preparation scripts and instruction traces are preserved in ignored build/T-24055-probes; no instruction listing or binary is committed.

Reproduce inside Docker after baseline unit build:

```
tools/docker.sh python3 drafts/T-24055/reproduce.py
tools/docker.sh python3 drafts/T-24055/probe.py func_8014354C build/T-24055-probes/model-selected.json
tools/docker.sh python3 drafts/T-24055/probe.py func_80143758 build/T-24055-probes/fill-selected.json
tools/docker.sh python3 drafts/T-24055/probe.py func_80143968 build/T-24055-probes/scroll-selected.json
```

probe.py compiles a temporary full unit with guarded C enabled using the unchanged recipe; failures are diagnosed per candidate and cause a nonzero exit. Resolved traces go only to ignored build storage. No candidate score is a default linked actual C proof.
