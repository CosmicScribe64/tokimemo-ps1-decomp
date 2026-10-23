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

## Closure candidate

Removing the reconstructed cross-switch shared-store gotos gives322 words/cost22.
Inlining the first two switch selectors gives322/cost10: only five initial
v0/a0 substitutions remain. An empty use of temp_a0 after the special selection
condition gives322/cost0. Empty record-pointer use stayscost10; an explicit
record local givescost12, and keeping it live grows323/cost35.

Removing unused switch/merge temporaries preservescost0. Removing the saved
sp28 local and its assignments givescost8; it remains with a FAKE explanation.
Production adds canonical C0/D0 header declarations. All original calls and
selection cases remain, including the exceptional selection column2.

Final body.c and replay.py reproduce the candidate after production promotion.
To replay causal controls in order, run controls.py, head-controls.py, cleanup.py
under Docker; these write only build/T-25049. recovered.c preserves the148-cost
starting point. All scripts replace the actual function even after promotion.
Full literal/checksum acceptance remains pending root integration results.

Accepted: independent coordinator standards/spec review has no findings. Full
520-step rebuild passes all27 binary checksums; default C matches1288 literal
bytes. Headers/globals pass; prototypes zero new/15 known. Final and causal
replays repeated after promotion with identical reported costs. Source file
800737A0.c is now fully matched.
