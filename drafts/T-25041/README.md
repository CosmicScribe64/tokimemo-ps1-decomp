# T-25041 KANGEI func_8013852C

Base 5e132362e after the BUNKA_SD match. Sole source is
src/ovl/KANGEI/80137B90.c. The baseline is 45 words / 180 bytes and has one
swapped pair at positions 35/36: target addu then lui, candidate lui then
addu. No compiler or header changes were made.

## Source mechanism

After computing D_80139ADC, add an empty nonvolatile field test:

```c
D_80139ADC = (u16)((D_800E6280.unk_F5F << 2)
                  - D_800E6280.unk_F5F) + year;
if (D_80139ADC) {}
```

The test makes uopt keep the assigned value in a register and emit the store
as rstr on the following source line. The previous direct str carried the
sum's line. That change puts the sum before the store-address lui in as1's
local schedule. Initially it also shifts the sum temporary from t6 to t7.
Removing the redundant signed-halfword conversion from the original
`(u16)(s16)year` restores t6. The retained `year` is u8, so removing that
roundtrip changes no value. The plain field test and the unsigned-false
comparison both match; the simpler plain test is selected and marked FAKE.

## Correction to the prior readiness explanation

The baseline trace agrees with T-24901 and T-25018: at time 16, the lui
and addu have aftercycles 4, with besttime 15 and 16 respectively, and as1
picks lui. The new after-storedfalse trace retains those exact values and
dependencies but picks addu. The address/store line is now the line after
the sum, and its original INST numbers move from 21/22 to 25/26 because
of the promoted value conversion and later store emission. Thus readiness
alone did not determine the tie. The prior negative `tmp = sum; store tmp`
result does not establish that every possible source-line separation fails.

No assembler patch or synthetic rearrangement was used. Captures are the
actual full-TU stock compiler input/output. All local traces, ucode and
binaries remain ignored under /private/tmp/tokimemo-t25037/build/t25041.
The copying capture scripts were reused from T-25037 without changing
production passes or flags.

## Complete experiment ledger

Costs are resolved aligned costs. Every result has 45 words. The first
family tests whether a separate empty condition changes the following-store
dependency; the second removes the one extra conversion proved by ucode.

| Variant | Cost | Finding |
| --- | ---: | --- |
| baseline | 2 | Original addu/lui pair |
| after-year, after-yearfalse | 2 | Same pair |
| after-girl | 10 | Additional instruction order changes |
| after-girlfalse | 2 | Same pair |
| after-stored, after-storedfalse | 4 | Schedule correct; sum and store use t7 instead of t6 |
| after-sum, after-sumfalse | 26 | Whole-expression commoning changes allocation |
| before-product, before-productfalse | 6 | Product/sum temporary shifts; old order remains |
| storetest-u16year | 0 | Remove redundant signed year conversion |
| storetest-s16year | 4 | Not the required temporary count |
| storetest-year | 0 | Plain byte year also matches |
| storetest-u16read, storetest-u16view, storetest-signedread | 4 | Read view does not correct the sum temporary |
| plain-test-year | 0 | Selected simplest test |
| no-test-year | 4 | Removing selected test loses match; it is necessary |

The source-level equivalent empty test was checked only after the new
store-promotion evidence; no old cast/temp/order grids were repeated.

## Proof and review

Guarded and promoted default C separately pass a private normal link through
tools/funcdiff_verify.py: correct address and extent, all 180 bytes equal to
the original. The final function has no assembly fallback.

Root reviewed the final source against standards and scope with no findings:
the u8 year makes the removed conversions redundant, the empty nonvolatile
test has no effect, and all unusual source controls are marked FAKE. Root
owns the full 27-unit integration check and main push.

All 8 current KANGEI C units were rebuilt through their generated normal
Ninja commands, followed by the normal linker, objcopy and SHA1 commands.
KANGEI.bin passes hash b94bbaceb7083c4b56d3b2ff56313cbcfbe6ac83. Header and
global guards pass; branch prototype checks report 0 new disagreements and
15 known. The existing warning in unchanged 80135F10.c about an ampersand
before an array does not affect verification.
