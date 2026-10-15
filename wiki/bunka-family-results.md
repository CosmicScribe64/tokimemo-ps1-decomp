---
type: research
updated: 2026-10-15
---

# BUNKAKEN and BUNKASAI family reconstruction

Scope: [[tickets/T-12700-bunka-families]], from 95c3fc9f. Existing broad experiment evidence was read from the integration checkout's wiki/matching-breakthroughs.md and wiki/broad-coverage-results.md. That prior 190-target default sweep returned five functions / 5,036 bytes; this work does not repeat it.

## Aggregate address form

The existing CharFlags bitfield view fixes BUNKAKEN flag tests/stores, but a direct global member expression still leaves aggregate-base register differences in several handlers. A local `GameState *g = &D_800E6280` used for all aggregate accesses follows already matched siblings in src/ovl/BUNKAKEN/8013BFE0.c and src/ovl/BUNKAKEN/8014A6A0.c. On the first two candidates this gives zero full-TU differences; replacing only the store with the pointer does not. This is source-form reuse, not a new compiler pass or independent type discovery.

The same form gives zero full-TU differences for func_80142A40, func_80142FD4 and func_801438F8. func_80141B80 remains nonmatching (134 rows). Literal proofs and accepted counts are recorded in [[tickets/T-12701-bunkaken-address-family]]. All unsuccessful scratch variants remain in build/bunka12701.

## Reload after calls

The BUNKASAI func_80146A78 archived draft saves a calendar state byte across calls. Reloading the existing GameState field for its final comparison removes the spill and reproduces the target's frame and instruction sequence. Local GameState pointer variants worsen this candidate. Literal proof is recorded in [[tickets/T-12702-bunkasai-family]]; scratch variants remain in build/b12702.

No compiler flags, generated assembly, or shared main types are changed by these experiments. A zero candidate score is not counted as an accepted match until linked bytes and the overlay SHA1 both pass.

## Verified checkpoint

Five BUNKAKEN handlers add 6,544 literal linked bytes, with BUNKAKEN SHA1 OK. Two BUNKASAI handlers add 2,520 bytes, with BUNKASAI SHA1 OK: func_80146A78 (1,216) and func_8013BB94 (1,304). The latter needs existing BunkaReturnFlags stores before the three continuation-state copies; bitfields alone reduce the mismatch but leave instruction ordering wrong. Total accepted at this checkpoint: seven functions / 9,064 bytes. Header validation passes. No whole-game checksum claim is made.

The continuation-state ordering also resolves BUNKAKEN func_80141B80: existing BunkakenModeFlags assignments before the three continuation copies reduce 134 -> 24 -> 0 rows (bitfields, then order). Its 1,860 linked bytes and BUNKAKEN SHA1 pass. BUNKAKEN now adds 8,404 bytes; both lanes total eight functions / 10,924 bytes.

Further transfer adds only the two still-assembly handlers func_80138A08 (1,200 bytes) and func_8013A8CC (804 bytes), both literal MATCH with BUNKAKEN SHA1 OK. Historical seeds for 3C0AC, 3C634 and 3CA6C were already matched controls in the baseline and contribute no new coverage. Current accepted total is ten functions / 12,928 bytes. The still-unmatched 41088 remains 228 rows; BUNKASAI 3B25C remains 122 rows after direct stores, and aggregate-pointer alternatives worsen it. The guarded BUNKASAI 51364 scheduling gap also remains; no rejected candidate enters production.

A targeted fresh test in BUNKAKEN 8014E440.c transfers the same source forms to func_8014E5E0 (688 bytes), func_8014E890 (608) and func_8014EAF0 (868). All three pass literal proof and BUNKAKEN SHA1, adding 2,164 bytes. Further adjacent handlers are tested sequentially with a two-miss stopping rule; this is a bounded family test, not another default sweep. Accepted cumulative gain: thirteen functions / 15,092 bytes.

All eight further handlers in BUNKAKEN 8014E440.c pass literal proof and overlay SHA1: 4EE54 (960), 4F214 (1,040), 4F624 (948), 4F9D8 (1,308), 4FEF4 (848), 50244 (600), 50680 (728), 50958 (880). New gain: 7,312 bytes. The file is entirely C. BUNKAKEN adds nineteen functions / 19,884 bytes; both lanes add twenty-one / 22,404 bytes. Evidence is build/bunka12701/eight-proof.txt and eight-build.log.

Adjacent BUNKAKEN func_8013EC98 adds 780 literal verified bytes. Its remaining argument-narrowing differences disappear with the existing per-TU old-style declaration of func_80083440; the callee narrows its own argument. The override is explained at the include site and leaves main_api.h unchanged. BUNKAKEN SHA1 passes with all prior matching functions retained. Cumulative accepted gain: twenty-two functions / 23,184 bytes.

Eight adjacent BUNKAKEN handlers add 6,112 literal verified bytes with overlay SHA1 OK; 8013EB30.c is now fully C. The local K&R view preserves all pre-existing C in the object. BUNKAKEN cumulative gain is twenty-eight functions / 26,776 bytes. Proof: build/bunka12701/adjacent-proof.txt and adjacent-build.log.

BUNKASAI held-out 8014DD90.c now has four newly verified handlers: 4DF04 (936), 4E2AC (832), 4E9D0 (1,364), 4FC70 (884), totaling 4,016 bytes. All literal proofs, overlay SHA1 and headers pass (build/b12703/final-proof.log). Existing CharFlags b12/b13 stores close the first candidate's last 14 rows. The fourth needs direct switch stores instead of m2c's artificial goto across case labels (128 -> 0). No header changes or FAKE workarounds. Both lanes now add thirty-four functions / 33,312 bytes.
