# T-14700 reserved type repairs

All fifteen alternatives compile with IDO 5.3 through the default full translation unit. Scores are resolved instruction-difference rows, diagnostic only. No candidate is literal-byte verified, no production C changed, and no affected SHA1 was needed because nothing was promoted.

Each directory contains selected body.c, declarations.h, context.c, result.json, complete compiler diagnostics, provenance, baseline source/header closure, and original-* artifacts copied unchanged from the reserved T-14500 attempt. Two tested offset-local variants are under alternatives/natural-offset/. No assembly or binary output is retained in this archive.

Reproduce serially in Docker with `tools/docker.sh python3 tools/type_repair_score.py` (optional function names restrict the set). Private generated ASM and immutable original inputs must already be available. This harness does not change production sources or headers.

Residual risks: generated C remains nonmatching. BUNKAKEN/BUNKASAI default switches and GEKO packed-word construction retain target uninitialized-stack reads; these require review before any executable use. ETC race reconstruction restores target branch-delay-slot counter resets and fixed-width aggregate accesses, but is not runtime validated. Main-API compatibility aliases in archived declarations are provisional context only, never proposed shared-header changes.

| Scope | Resolved difference rows |
|---|---:|
| BUNKAKEN:func_80145550 | 302 |
| BUNKAKEN:func_8014A5EC | 2 |
| BUNKASAI:func_80142BE8 | 445 |
| BUNKASAI:func_80147F8C | 2 |
| EN_NICHI:func_80138CCC | 376 |
| ETC:func_8013CCE4 | 138 |
| ETC:func_801480D0 | 1200 |
| EVENT:func_800FC040 | 55 |
| EVENT:func_801035BC | 82 |
| EVENT:func_8010AE7C | 22 |
| EVENT:func_8010F55C | 184 |
| EVENT:func_8010F74C | 150 |
| GEKO:func_80136C20 | 24 |
| GEKO:func_8013B10C | 67 |
| GEKO:func_80143F48 | 169 |
