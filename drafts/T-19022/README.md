# T-19022 drafts (TEL, wave 3 package w3-d-p22)

Source-only drafts of two functions that did not match; neither is a verified match. See wiki/matching-notes.md, section "T-19022".

- `TEL/func_80134FE4.c`: structure exact, 70 register rows differ (the global `D_801405E4` is coloured `$t0`, the original `$v1`).
- `TEL/func_80136724.c`: frame and dispatch exact, 452 register rows and 78 scheduling places differ. Carries the `TelApptWord` types it needs (they move into `TEL.h` on a match).
