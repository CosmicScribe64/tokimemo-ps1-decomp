# T-24049 ETC exam ranking reconstruction

Start c6847bd31; target src/ovl/ETC/80146400.c:func_801480D0.
No production source/header changes and zero new verified C bytes. The target
still uses INCLUDE_ASM. The earlier TEL pin remains unchanged in this ancestry.

exam-ranking.c adapts T-14700's hand repair in the T-14500 types archive to
current recovered data views. The old score1200 belongs to its old context;
it is not a fresh result on this checkout. T-17300 redrafts failed to compile,
and T-22029 subsequently restored the three separate ranking arrays because
a structure view hoisted an unwanted base register. No aggregate reversal or
header addition is attempted here.

## Observed semantic and type corrections

- Current arrays are s16 D_80150668[14] values, s32 D_80150684[14] order and
  s32 D_801506BC[14] ranks. Existing header declarations cover them and the
  thirteen-girl score/spread byte arrays.
- The random remainder divisors must be cast to s32: the target uses signed
  div for an unsigned byte spread. K&R promotes the byte to unsigned otherwise.
- Seed uses D_800E6280.unk_10F8. The player's score is
  ((EtcWork40 *)&D_801230D0)->unk_28[5], at work offset0x3C; no obsolete
  scalar extern is needed.
- The broken decompiler Shift-JIS spelling is the debug text "順位ソート".
- The final bonus loop in the old repair updated fifteen records. The target
  updates eleven: three peeled scalar stores, then two groups of four stores
  over Rec38 records3..10. The draft now uses i<11. This is an observed source
  semantic error, independent of compiler register selection.

The rest of the reconstruction is not a semantic equivalence proof. It must
still match the actual bytes before production use. The source retains plain
nested loops and current field accesses, without new FAKE source-shaping tricks.

## Full-TU serial default compiler evidence

Counts are printable diagnostic rows, not coverage or instruction-error counts.
Target is 702 resolved words/2808 bytes with a208-byte frame.

| Hypothesis | Candidate words | Rows |
| --- | ---: | ---: |
| Corrected semantic draft, current views (archived) |709|885|
| Scalar locals before the value array |709|885|
| Explicit selector switch, split first noise local, scalar row13 views |707|987|
| Same with volatile first-noise spill |704|1004|
| Generation counter separated from later loop counters |707|1053|
| Same with explicit next outer indices in both tie loops |691|1071|

The archived draft has a216-byte frame and saves six registers versus the
target's four. Its generation phase keeps the first random-noise result in
a saved register, while the target stores it at sp+0xBC across later calls.
It also carries a next-index range in a saved register. The target loop and
spill shapes are still materially different; this is not a register-only
near miss. Explicit noise spills and phase counters failed to recover them.
No blind declaration-order or compiler sweep was attempted.

## Reproduction and unchanged default control

Run from the repository root:

```
tools/docker.sh python3 drafts/T-24028/probe.py ETC func_801480D0 drafts/T-24049/exam-ranking.c
```

The helper substitutes actual scratch C into the current full TU, compiles with
default IDO5.3 flags and prints 702 target /709 candidate words,885 rows. It does
not edit production or claim a linked match. Compiler failures remain failures.
Scratch variants/objects/instruction comparisons remain ignored under
build/T-24049 and build/T-24028/reproduce/func_801480D0.

Docker ninja -j1 build/ovl/ETC.ok had no work, and independent SHA1 control of
build/ovl/ETC.bin against config/overlays/ETC.sha1 passed
(681c93b6e57c885f86e074c885a8bf6c3cca2319). This verifies the existing ASM fallback
build only. No literal C match was obtained or claimed. No object, assembly
listing or game data is committed. Ticket remains In Review for root review;
no push/merge.
