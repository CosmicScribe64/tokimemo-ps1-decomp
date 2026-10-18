---
type: research
updated: 2026-10-17
---

# GEKO family results

## Exact polygon clone (T-24017)

[[tickets/T-24017-clone-ports]] ported matched ENDING func_80139B48 to GEKO func_8013FD88 in src/ovl/GEKO/8013F360.c. SDK POLY_G4, canonical GameState offsets and the inherited FAKE explicit colour masks/source-line trios suffice unchanged. Added only the SDK include and an overlay-local forward prototype. Target is unguarded C with no INCLUDE_ASM. Fresh Docker ninja -j1 GEKO checksum passes; default funcdiff --resolve --no-build confirms 476 literal linked bytes. No other GEKO function changed.

## T-24050 typed selection checkpoint

[[tickets/T-24050-geko-scene-near-match]] leaves func_80134808 unchanged assembly. Current-context typed word-flag/narrow-state source scores103, and a separate record-array base form scores80 (FAKE marked for avoiding unwanted base promotion). The target still differs in next-index/best-index lifetimes, shared selection-update CFG and saved-register allocation. Explicit index joins, old-flow word tests and byte-state forms are exhausted in drafts/T-24050. A tested scratch-only reproducer is included; no matched sibling above0.70 exists in the current census. No production/header change or new verified byte.
