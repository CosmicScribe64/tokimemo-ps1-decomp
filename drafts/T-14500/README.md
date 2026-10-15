# Non-matching draft archive (T-14500)

These files are diagnostic C drafts and context snapshots. They are excluded from production source and build inputs. `candidate-zero` is an unverified score, not accepted matching code. Only independently linked literal-byte proof plus the affected unit checksum can authorize promotion.

`inventory.json` freezes every game-source INCLUDE_ASM at baseline 48362009, including retained guards. `catalog.jsonl` records outcomes; each unit/function directory retains its own result, full translation unit, body/declarations, raw decompiler C and diagnostics. `baseline/` snapshots the source and headers. `environment.json` records compiler/header/tool hashes. Historical archive records are provenance only because their old scores lacked full context.

Reproduce from the baseline checkout with the pinned project Docker image and `python3 tools/exhaustive_drafts.py`. The runner is serial, bounded and resumable. It never inserts drafts into production. The archive contains generated C and metadata, no original assembly or game binaries.
