# T-20008: NAME_ENT introduction-scene reconstruction

`body.c` is a nonmatching reconstruction of `NAME_ENT:func_80143580` (4,308 original bytes) in `src/ovl/NAME_ENT/80142A80.c`. It is archived here because the compiler still differs substantially. Production keeps `INCLUDE_ASM`; this contributes **zero newly verified C bytes**.

Claim: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/50. Starting revision: `d73328b8`.

## Recovered behavior

The function advances a 20-step introduction scene: tile/palette loading, fades and horizontal movement, three display records, dialogue display, and the final transition. Assembly checks repair several problems in the older T-14500/T-17900 drafts:

- The initial work array holds eight addresses, starting at `0x80180000` with a `0x4000` stride. The low relocation on the subsequent increment is not part of the starting address. Later fills contain eight, six and six addresses.
- The sprite loop walks three `Rec24` records with a `0x24` stride. Its X/Y counters are words; the destination fields are halfwords. Color stores, including record 1 offset `0x14`, are bytes despite some legacy scalar declarations.
- The mode-byte loop writes `GameState.unk_10A5[0..2]`; it starts one byte after `unk_10A4`.
- The horizontal threshold tests in steps 1 and 8 compare the coordinate **before** decrementing it.
- Step 4 reloads the scene selector after its helper/fade work before falling through to step 5. Reusing the selector captured before the helper would restore an obsolete value.
- Step 13 copies 128 palette words: the first 32 from `unk_163C`, and the remaining 96 from external palette entries with the same absolute index. Palette colors are not copied with a byte or record stride.

The two work-area records use the existing `SpriteSlot24` aggregate through its real fields, including the halfword at record 1 offset `0x04`; no extra scalar declaration is needed. Both nested tile-fill loops and the explicit selector stores remain visible. The unresolved source forms and unsigned flag-store allocation controls are marked `FAKE` in the draft. These are evidence-preserving starting points, not completed matching C.

## Reproduce

Run from the repository root, inside the documented Docker environment:

```sh
bash tools/docker.sh python3 tools/viewsweep.py \
  --body drafts/T-20008/NAME_ENT/func_80143580/body.c \
  --scope NAME_ENT:func_80143580 --budget 150 -j 4 \
  --json build/T-20008-viewsweep.json
```

The existing tool inserts this body and `declarations.h` into the full production translation unit in scratch. It does not replace the production fallback. The declarations file supplies only missing function declarations for scratch context; any later production promotion must put main-exe declarations in `include/main_api.h` and overlay declarations in `include/ovl/NAME_ENT.h`.

At the recorded revision, the canonical-header body compiles to 1,078 instruction rows with **835 resolved diff rows / 448 aligned instruction edits**, against 1,077 original rows. The complete view sweep performs 39 compiles with no untested variants and finds the best diagnostic return view, `s32 func_8004A1A0();`, reaches **799 diff rows / 430 aligned edits**. No prototype change is adopted in production. These numbers describe this cleaned archive. Scalar work-area controls reached 387 aligned edits and a pointer palette walker reached 360; the archive uses the existing aggregate and removes unused locals. Its frame is 0x88 with the table at sp+0x64, versus the original 0x90/sp+0x6C. Do not add padding to reproduce offsets without source evidence.

Raw diff-row counts change scoring mode when instruction counts happen to coincide, so use aligned instruction edits when comparing those controls. Neither score is a byte-match proof.

## Remaining work

The original caches the selector in `t0`, primary fade in `a1`, secondary fade in `t2`, and shared flag constants in `ra`/`t1`. The draft still splits some of those ranges. Tile-loop bound setup, palette-loop induction, store scheduling and the step-7 byte update also differ. Scratch scalar GameState controls, selector/fade widths, separate phase counters and typed call arguments do not close the gap; this is not evidence of a compiler defect.

The ignored allocator trace was built from the documented `decompals/ido-static-recomp` revision `9c242adc890beef098020149d9554f48208f699d` with the repository's trace patch recipe. It identified an artificial cached array base, extra saved-register live ranges and coordinate narrowing. Ordinary indexed fills, word coordinate counters and unsigned resource-call literals address those specific draft problems. No compiler binaries, instruction listings, original assets or per-function compiler flags are included here.

Before promotion: resolve the remaining source differences, measure the canonical return view again, remove or justify every `FAKE`, run the actual default linked-function proof, and verify all 27 binary hashes plus header/global/prototype guards. A matching ASM fallback is not evidence that this draft matches.

## Validation

The default Docker ninja build completed 605 steps; header/global guards passed, all 27 binaries matched their configured SHA1 values, and the prototype branch check reported 0 new / 15 known disagreements. The default linked function comparison reports MATCH for 4,308 bytes because it still builds the original ASM fallback. The archived C comparison above remains nonzero.
