
## Exact polygon clone (T-24017)

[[tickets/T-24017-clone-ports]] ported matched ENDING func_80139B48 to GEKO func_8013FD88 in src/ovl/GEKO/8013F360.c. SDK POLY_G4, canonical GameState offsets and the inherited FAKE explicit colour masks/source-line trios suffice unchanged. Added only the SDK include and an overlay-local forward prototype. Target is unguarded C with no INCLUDE_ASM. Fresh Docker ninja -j1 GEKO checksum passes; default funcdiff --resolve --no-build confirms 476 literal linked bytes. No other GEKO function changed.
