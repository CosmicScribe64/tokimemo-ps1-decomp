---
type: research
updated: 2026-10-15
---

# MASTER typed-table pilot (T-13000 / T-13001)

Baseline origin/main 95c3fc9f. Isolated codex/types-globals-13000; original checkout and other teams remained read-only. Three functions add **3,648 literal linked text bytes**. This is a bounded MASTER result, not a prediction of reaching 80% coverage. Related work: [[data-types]], [[game-state]], [[broad-coverage-results]].

## Demonstrated layouts

`tools/type_recovery.py --unit MASTER --top 12` on generated MASTER assembly proposed three separate 0x1A-byte records. Original copies load words at +0,+4,+8,+C,+10,+14 and a halfword at +18. Each consumer later indexes the stack copy with a left shift by one and a signed `lh`. Existing `MasterHalfwords13` (thirteen s16, size 0x1A) therefore describes the accessed extent; it does not imply that adjacent tables form one array or that alignment gaps are members.

| Global | Consumer | Original stack copy | Literal bytes |
|---|---|---|---:|
| D_8013C7E8 | func_80139910 | sp+0x30 | 952 |
| D_8013C804 | func_8013A784 | sp+0x28 | 1,388 |
| D_8013C820 | func_8013ACF0 | sp+0x28 | 1,308 |

Evidence examples: func_80139910 starts copying at 0x80139918, stores the final halfword at 0x80139954 and reads an indexed signed halfword at 0x801399C4. func_8013A784 starts at 0x8013A78C; func_8013ACF0 starts at 0x8013ACF8 and indexes its copy at 0x8013ADF0. Generated assembly is not edited or committed.

All three declarations reuse the existing overlay type; two later-helper forward declarations prevent implicit return-type conflicts. No include/game.h, include/main_api.h or migrate_globals config changes.

## What actually unlocked the functions

The raw m2c output does not reconstruct these copies even with the correct extern type: it emits an unknown stack object and word casts. Replace the copy with aggregate assignment and the indexed read with table.v[index]. Full-TU compilation is necessary.

For the two later handlers, use the existing MasterSprite44 view of D_80120650, particularly +03 (u8 flags), +07 (u8 counter), and +16 (s16). The original keeps the flag-byte read behind the neighboring halfword store. Separate symbols let IDO hoist it. This independently repeats the existing matched func_80137044 / func_801375E8 pattern, without inventing another global view or migrating other teams' code.

The layout recovery alone does not explain every source detail. Scalar-byte address casts preserve selector live ranges; a cached Rec38 pointer and a constant-value local preserve stack offsets in the later handlers. These are explicitly FAKE-labelled in all three accepted bodies. The table layout is demonstrated; these exact original source forms are not. Reordering the real switch temporary before the table fixes the first handler's copy offset. The second handler reached only two differing offsets before the cached-pointer form fixed them.

## Rejected hypotheses and remaining work

The tool's highest MASTER proposal merges D_8013C5C0 through many tables at stride 12, and other proposals merge neighboring jump tables. These are hypotheses from linear register tracking, not safe declarations. Do not apply wholesale. This run analyzed MASTER only, so it does not replace the earlier all-game aggregate audits.

func_8013B804 was also tried using existing GameState event records. Plain switch and increment forms, with void and s32 return probes, still differed in register allocation. Its source remains INCLUDE_ASM; no byte gain is counted. The ignored build/pilot-v3.c preserves the rejected local experiment, not a new source-type claim.

## Verification

Evidence in ignored build/: pilot-proof.txt (funcdiff --resolve literal linked bytes), pilot-check.log (MASTER SHA1), pilot-v1/v2/v3 comparisons and type-master.txt. Header/global/prototype audit results are recorded in the ticket. Shared headers were unchanged, so a full 27-binary rebuild is not required for this pilot. One Docker CPU and one compile job were used. No host dependencies installed, no assembly patching, no push or PR.
