# T-25049 event selector recovery

Run `tools/docker.sh python3 drafts/T-25049/replay.py` from the repository root.
The script reconstructs the T-17000 body in a private full translation unit;
production source and headers remain unchanged.

Target: MAIN func_80073CB0, 1288 bytes, 322 words. Fresh corrected candidate:
322 words, 148 aligned differences. No match or coverage credit.

Assembly uses byte loads from the C0 and D0 table regions with unscaled column
indices, so historical s32 scalar declarations and pointer arithmetic are wrong.
Private views use u8 arrays of16 bytes, with case offsets0,2,4,6,9,11 and fixed13.
These are addressable regions, not a claim that the original source used flat arrays.
Canonical 6890/A8 tables already exist. Header ownership needs review before any
production promotion; currently the new declarations occur only in the draft.

The second fallback selector must read GameState.unk_0F4.h: original instruction
at80074074 loads D_800E6374. The historical draft used unk_000. Corrected source
retains148 comparison cost; that metric alone did not expose the semantic error.
First mismatch is an initial pointer/value v0/a0 swap; subsequent switch selector
promotion also differs. No new compiler rule, option or assembly patch is used.
