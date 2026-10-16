# T-15100 unaligned aggregate-copy repair archive

All nine alternatives compile as complete translation units with the existing Docker IDO 5.3 pipeline. They are **NON_MATCHING**; no zero comparison, literal match or new matching coverage is claimed. Target code extents total 17,408 bytes. Initial failures and raw m2c remain immutable in the T-14500 census archive.

| Scope | Target code bytes | Diagnostic rows |
| --- | ---: | ---: |
| BUNKAKEN:func_80144F70 | 1424 | 426 |
| BUNKASAI:func_80142700 | 1176 | 336 |
| ENDING:func_8013A2E8 | 4648 | 1667 |
| GEKO:func_80142C9C | 3656 | 1122 |
| GYOZI:func_801368B8 | 2256 | 701 |
| KANGEI:func_8013769C | 1256 | 142 |
| SHOUGATU:func_80133B44 | 408 | 92 |
| SHOUGATU:func_801350F8 | 2308 | 995 |
| SHOUGATU:func_801359FC | 276 | 69 |


Each scope retains repaired body, declarations, source context, complete preprocessed compiler context, compiler diagnostics, comparison, result hashes and provenance. `catalog.jsonl` is a separate nine-row alternative-catalog input. It never overwrites the original failure catalog.

Reconstruction follows observed copy extents: 220/231-byte KANGEI tables, seven-by-five-byte fortune tables, three flag bytes, eleven-byte festival arrays, and nine GEKO tables including 627 bytes copied as 624 aligned bytes plus a three-byte tail. Ordinary local string initializers replace complete literal-copy sequences. GEKO also needs real 21-byte writable flags and byte-accurate stack aliases. Unexplained offsets and broadly inferred pointer/data types remain draft limitations; compilation is not a semantic-correctness proof.

ENDING's target consumes v0 after nine dialogue_render calls. The repaired draft uses the existing integer-return ABI view from src/main/8007C030.c under an explained MAIN_API_OVERRIDE_dialogue_render in its archived context. A 27-byte aggregate copied from D_8013C0D0 avoids trusting m2c's suspicious decoded literal prefix. This is draft context only, with no shared API or production source edit.

Run in Docker from source base 3d581b25, original census worktree mounted read-only at `/ref`, worktree at `/work`, working directory `/work`:

```
python3 tools/unaligned_draft_repairs.py
```

The script accepts scoped names to rerun a subset. Generated asm/disc are immutable inputs and must not be regenerated into shared hardlinks. No host dependencies, binaries, raw ASM or game assets are part of this archive. Original census baseline is 48362009; provenance records both source contexts. None of these drafts replaces a production assembly fallback.
