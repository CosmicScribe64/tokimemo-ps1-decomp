# T-24038 source provenance and bounded controls

Final source bodies live in `src/ovl/ETC/8013F980.c` and `src/ovl/MASTER/80139000.c`. Commits `ea53c475e` and `d3faacf8` replace only the assigned assembly targets; other production text is unchanged. No generated assembly or game data is archived.

## ETC func_8013FC64

Inputs: matched MAIN `func_8004AE54`, T-17013 matching notes, T-17300 renderer drafts, and target control flow. MAIN supplies the paired-coordinate digit loop, signed range guard, and division-before-increment order. Target uses full `SPRT` packets (explicit 8x8), CLUT `d * 16, 0x1FD`, separate sign bytes, and no MAIN enable guard.

The first plausible port scored 88 aligned diagnostic differences with a 0x70 frame instead of 0x78. Explicit loop offset scored 72; sign-offset forms scored 52/54. The winning real local order is packet, value, loop offset, count, negative flag, palette temporary. The sign block computes palette, increments count, then computes sign offset before acquiring the packet. This gives zero differences without guessed padding or FAKE code. The approved font declaration covers ten coordinate pairs ending immediately before the separate sign bytes.

## MASTER func_8013B448

Inputs: matched `func_80138088`, T-18600 typed draft, and target record stores. The old draft's two pointer-to-byte truncation expressions in cases 3/12 are replaced by direct `unk_75E[unk_F5E].unk_03` assignments. The selector uses the real club nibble and record byte fields, through one consistent GameState pointer.

The initial sibling port scored 247 diagnostic differences; state pointer alone 187. Correct fields scored 95, and fields plus state pointer 7. The sibling void view with chained increment scored 4. Ordinary increment followed by unsigned clamp gives zero. Explicit return and temporary count variants scored 16 and 4/9, respectively, and were rejected. The approved void prototype is supported by the side-effect-only sibling and unused caller result; the complete MASTER checksum validates the rebuilt caller as well as the target. No new FAKE is used.

## Verification

Docker only, existing image, one compiler and `ninja -j1`. Both targets are unguarded C with their target INCLUDE_ASM removed. `tools/funcdiff.py --no-build --resolve --version default ETC:func_8013FC64 MASTER:func_8013B448` proves literal linked equality (796/956 bytes). Both overlay checksums pass. Header/global checks pass; branch prototype check reports 0 new / 15 known disagreements. Worktree logs: `build/etc-master24038/final-proof.log`, `final-build.log`, `final-headers.log`, `final-globals.log`, `final-protos.log`, `body-audit.log`. Diagnostic costs above are search controls, not independent proof.
