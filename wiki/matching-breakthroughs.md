---
type: research
updated: 2026-10-15
sources: ["tools/queue.py", "tools/progress.py", "tools/bulk_m2c.py", "tools/neardupes.py", "wiki/tickets/T-12210-matching-bottleneck-research.md"]
---

# What will accelerate completion

The strongest evidenced change is to prioritize medium-sized functions and source families by verified bytes, and use compiler research on small, discriminating families. Faster permutation alone has not demonstrated a route through the remaining code. This investigation produced **three verified matches / 2,352 linked bytes** by resolving a source-role ambiguity that automatic copying correctly refused. That is a useful experiment, not a large completion breakthrough or a forecast for the whole backlog.

## Baseline and measurement boundaries

Base game source: `7a65b3d8`. Generated assembly copied from the read-only main checkout. All analysis, preprocessing, compilation, and checks ran in the existing native Docker image with one CPU and 1 GiB. The active cx run's later gains are not included. TT was counted for the whole-project census but not edited or selected for work.

`progress.py`: **5,733/6,958 functions (82.4%)**, **1,288,084/2,279,368 bytes (56.5%)**. Remaining: **1,225 functions / 991,284 progress bytes**. SDK code is excluded from those totals: another 722 assembly functions / 165,212 bytes, tracked separately. Completing the displayed game percentage will not by itself finish the SDK work.

Queue instruction-count sizes sum to 992,080 remaining bytes, 796 more than progress. The difference is reproduced by comparing each queue size (`4 * parsed instructions`, including trailing nops) against its `nonmatching` header size. The figures below explicitly distinguish these size conventions; none of these baseline counts is a fresh full-game checksum claim.

| Remaining size band (progress convention) | Functions | Bytes | Share of remaining bytes |
| --- | ---: | ---: | ---: |
| Under 600 bytes | 572 | 216,368 | 21.8% |
| 600–1,999 bytes | 592 | 586,572 | 59.2% |
| 2,000 bytes and above | 61 | 188,344 | 19.0% |

`bulk_m2c.py` defaults to a strict 600-byte ceiling and sorts smallest first. Its default selection therefore excludes **78.2% of the remaining bytes**, even before TT exclusion. This does not establish what past users selected with custom flags; it identifies a default that no longer fits byte-completion priorities. Raising the ceiling without changing search strategy or per-target effort is not a demonstrated solution.

The largest 100 account for 259,384 queue bytes (26.1% of the queue remainder); the largest 200 account for 395,820 (39.9%). The remaining work is sufficiently spread out that solving only a handful of giant functions will not finish it.

## What is actually blocked

The queue marks 424 functions / 400,460 queue bytes blocked and 801 / 591,620 unblocked. No remaining orphan-rodata flag O was found. J and S alone are not blockers when an object owns its rodata island. Major unblocked pools include main (155 / 107,988), TACO (112 / 90,680), RPG_BAT (80 / 79,148), and TAIIKU (67 / 49,020). Ownership still governs assignment; these are not new work claims.

R/V/U are **hypotheses, not proof of a compiler impossibility**. They flag 386 remaining functions, but also flag 457 of the 5,660 source-classified matched controls available through queue's matched-assembly directories. These controls exclude every remaining scoped function; the sample is smaller than progress's 5,733 because not every matched source has the expected matched-directory entry. This is not a precision estimate for the unmatched population. It does prove that these flags cannot justify a blanket source-search exclusion.

Only 134 remaining functions / 72,476 queue bytes have guarded drafts in current source. The other 1,091 may have archived drafts; **absence of a guarded body does not mean never attempted**. [[tickets/T-9810-permuter-farm-for-near-misses]] already collected 1,090 scoped seeds. The current source census must be joined to those archives before generating another thousand first drafts.

## Experiment: reuse blocked by literal roles

Exact reuse report: 8 mixed groups, 10 unmatched members, **2 plans and 8 skips**. Near reuse: **0 immediately fillable**, 5 skips / 3,380 instruction bytes, and 3 groups without a matched source / 7 functions / 3,128 bytes. This is a small ceiling for ordinary whole-function duplication on this snapshot.

Three near-copy skips were caused by a constant used both as a character ID and as a switch label. The instruction correspondence showed which uses changed, so the source could be copied with state labels preserved:

| Target | Matched sibling | Linked bytes | Verification |
| --- | --- | ---: | --- |
| BUNKASAI:func_8014FFE4 | BUNKASAI:func_801456E0 | 592 | Literal linked MATCH; BUNKASAI SHA1 OK |
| BUNKASAI:func_8015052C | BUNKASAI:func_80145C28 | 920 | Literal linked MATCH; BUNKASAI SHA1 OK |
| BUNKAKEN:func_801386C0 | BUNKAKEN:func_801475CC | 840 | Literal linked MATCH; BUNKAKEN SHA1 OK |

For the first two, character ID 7 becomes 6 while `case 7:` remains unchanged. For the third, character ID 6 becomes 4 while `case 6:` remains unchanged. Text/script/sector constants were mapped from paired assembly immediates. No random search, compiler changes, new prototypes, or FAKE idioms were needed. Both affected unit checksums and header/global/prototype guards pass (0 new / 15 known prototype disagreements).

**Implication:** when a copy tool refuses an ambiguous literal, inspect its syntactic role before starting from m2c again. All three observed ambiguous-literal skips are now resolved. This experiment does not imply hundreds more existing near-copy matches; the measured current pool was small. A future automatic transformation must distinguish switch labels, array dimensions, offsets, and runtime expressions with parsed C and retain linked verification.

## Experiment: is register-sensitive retrieval hiding more whole-function twins?

A scratch fingerprint used the exact instruction order, immediate operands, load widths, and relative branch destinations, while normalizing symbolic relocation operands and call destinations. Two variants canonicalized registers by first appearance or erased register identities (keeping zero/sp/ra/gp fixed). Each returned 8 unmatched targets / 5,808 queue bytes with a matched counterpart. Every target was already in the exact-duplicate report. **No new whole-function family was found by this relaxation.**

This is a bounded negative result. It does not test changed constants, instruction reordering, different block layout, or reuse of partial functions. It argues against building another register-only whole-function matcher as the next project.

## Evidence-ranked research agenda

1. **Change the work portfolio: test 600–1,999-byte objects with coherent shared types.** This band holds 59.2% of remaining bytes. Run a matched comparison of equal wall-time allocations on medium and small targets, stratified by draft availability and prior attempts, measuring linked bytes per hour including failed work and verification. Start with unclaimed objects from main/TACO/RPG_BAT, keeping per-file ownership. Use current full-unit scores, not incomparable old stripped scores. This tests a scheduling hypothesis; it is not yet a measured throughput gain.
2. **Exploit existing drafts and record why each attempt stopped.** Join farm draft fingerprints, bulk state, guarded bodies, and wiki failure notes by unit/function. Refresh only candidates whose source/header/compiler context changed, and exact-verify old zero-score seeds before mutation. Existing farm state hashes the body, while `bulk_m2c.Runner.record` stores scope, draft, score, variant, and optional exact score without compiler/header identity. The gap is context-qualified comparison across these existing records, not lack of any checkpoint system. The historical farm ran 60.9 minutes on four workers: all four wins came from existing score-zero seeds, with **no new random-mutation win**. That is strong local evidence for seed recovery ahead of a larger random farm.
3. **Research partial source families and data types, not more exact whole-function hashes.** Whole-function reuse is largely exhausted, but scene handlers share smaller state-machine sections. Next experiment: take one unclaimed 1–2 KB handler, align its basic blocks/call sequences against matched handlers in its object/neighboring objects, reconstruct using their types and control-flow idioms, and compare effort with a fresh single-function m2c draft. Require at least three held-out siblings before claiming a reusable rule. Do not copy instruction fragments or certify matches with normalized fingerprints.
4. **Use compiler archaeology only with a narrowly falsifiable input/output rule.** The most concrete unresolved lead is the signed narrow selector-copy family in [[tickets/T-11570-w1-ovl-name_ent-80139740]]: disabling global copy propagation reduced three drafts to four differing lines; signed-load widening affected comparison order. Freeze those seven targets plus matched signed-load/local-copy controls, test widening and copy preservation separately and together, then run the full matched regression before considering any uniform pass. This follow-up was not executed here. Seven known targets do not justify presenting it as the answer to 386 heuristic flags or the whole remaining megabyte. Earlier blanket allocator hypotheses failed controls ([[uopt-register-allocation]]); IDO 5.2 did not improve matching ([[ido-52-evaluation]]).
5. **Keep SDK completion as a separate research track.** The displayed game percentage excludes 165 KB of SDK assembly. Follow object-boundary/version evidence in [[psyq-sdk]], not fresh game-style decompilation of every library function. Missing SDK versions and mixed-vintage identification remain unresolved; no new SDK match is claimed here.

The acceptance metric for each research lane should be **new literal-verified bytes per total hour, plus a falsified or supported mechanism**, with held-out targets. Raw candidates/sec, isolated best scores, and number of functions attempted do not measure completion speed.

## Upstream research checked

[m2c's primary documentation](https://github.com/matt-kempster/m2c/blob/master/README.md) describes multi-function type inference, unknown-struct-field inference, and editable stack-struct templates. The project wrapper normally invokes one function with header/callee context; a bounded multi-function experiment may improve shared-type reconstruction. Availability in the project's pinned version should be confirmed before adopting an option. This is a proposed experiment, not a demonstrated missing feature or gain.

[decomp-permuter's primary documentation](https://github.com/simonlindholm/decomp-permuter) describes its strongest use near the end of matching, when register allocation remains, and notes limitations with structural differences and stack positions. That supports using it after source/type/control-flow reconstruction; the project's own farm result is the stronger evidence for this backlog.

## Reproduction artifacts

Ignored local evidence under `build/acceleration/`: `progress.txt`, `queue-summary.txt`, `census.json`, `baseline-metrics.txt`, `dupes.txt`, `neardupes.txt`, `relaxed-twins.json`, `twin-plan.txt`, `twin-plan3.txt`, `twin-proof.txt`, `twin3-proof.txt`, and unit build logs. Scratch census and family scripts reside there too. Run the checked-in tools with `progress.py`, `queue.py --summary`, `dupes.py -v`, and `neardupes.py -v` for fresh inventories; counts will change after these three matches and other integrations. See [[tickets/T-12210-matching-bottleneck-research]].

## Executed medium-function experiment (T-12220)

Baseline f1010b54: 5,736/6,958 functions, 1,290,436/2,279,368 game bytes. Two fresh m2c seed drafts (BUNKASAI func_80150234 and func_80145930, 760 bytes each) matched after existing GameState name normalization. No family reconstruction was needed for these seeds.

The frozen cohort took the first 12 remaining names alphabetically in each of BUNKAKEN and BUNKASAI, restricted to 600–1,999 bytes, excluding the two seeds. Existing bulk_m2c ran one default variant with --time 0 --score-jobs 1 --jobs 1 --no-order --no-regdiff under a one-CPU, 1 GiB Docker cap. It matched 16/24, 13,228 linked bytes, in 260.18 seconds including its per-target gates and final migration/header check. This duration excludes setup, seed work, later revalidation, documentation and review. It is not end-to-end throughput or a comparison against small functions.

Two remaining handlers shared a 14-row exact mismatch: byte OR stores reused the address register, whereas the target used a separate byte-load destination. Replacing `.unk_0C.b[1] |= 0x10/0x20` with the existing `.unk_0C.f.b12/b13 = 1` representation reduced both full-unit scores from 14 to zero. Both then passed literal linked-byte verification. This applies an already documented CharFlags compiler idiom (include/main_api.h, T-9020); it does not discover a new compiler rule. Two related examples are insufficient to claim broad transfer or three held-out successes.

Total: **20 functions / 16,272 literal linked bytes**, comprising two seeds, 16 initial cohort wins, and two bitfield fixes. Six cohort targets remain assembly; scratch drafts are retained outside source. No tool implementation or global default was changed. Both affected overlay SHA1 checks pass; check_headers, migrate_globals --check, and sync_protos --check-branch pass (0 new disagreements, 15 known). Full-game checksum build was not run. Final progress: 5,756/6,958 functions and 1,306,708/2,279,368 game bytes (57.3%).

| Cohort function | Bytes | Outcome |
|---|---:|---|
| BUNKASAI:func_80134A90 | 616 | direct match |
| BUNKAKEN:func_8013AF00 | 624 | direct match |
| BUNKASAI:func_80138EF8 | 704 | direct match |
| BUNKAKEN:func_80138EB8 | 716 | direct match |
| BUNKAKEN:func_80137CD8 | 736 | direct match |
| BUNKASAI:func_801325F4 | 736 | bitfield fix matched |
| BUNKASAI:func_801347A8 | 744 | direct match |
| BUNKAKEN:func_80139780 | 748 | direct match |
| BUNKASAI:func_80138BE4 | 788 | bitfield fix matched |
| BUNKAKEN:func_8013A8CC | 804 | remaining; exact score 78 |
| BUNKASAI:func_80135914 | 824 | direct match |
| BUNKASAI:func_8013A614 | 840 | direct match |
| BUNKASAI:func_801391B8 | 844 | direct match |
| BUNKAKEN:func_80138340 | 896 | direct match |
| BUNKAKEN:func_80139A6C | 896 | direct match |
| BUNKAKEN:func_80137FB8 | 904 | direct match |
| BUNKASAI:func_80135554 | 960 | direct match |
| BUNKASAI:func_80139504 | 1052 | direct match |
| BUNKAKEN:func_8013C634 | 1080 | remaining; exact score 124 |
| BUNKAKEN:func_8013CA6C | 1108 | remaining; exact score 119 |
| BUNKASAI:func_80139920 | 1124 | direct match |
| BUNKAKEN:func_80138A08 | 1200 | remaining; exact score 104 |
| BUNKAKEN:func_8013C0AC | 1416 | remaining; exact score 130 |
| BUNKASAI:func_80135C4C | 1508 | remaining; exact score 276 |

The practical next step is a bounded sweep of unclaimed 600–1,999-byte functions using existing drafts and exact rescoring, followed by grouping near misses by known type/code-generation patterns. Keep a small-function control sample and include failed attempts, setup and review in the timing before claiming a general speedup. This selected pair of related overlays supports changing target selection; it does not establish a game-wide 67% success rate. Do not start with an unbounded random-mutation farm.

Evidence: build/family/final-proof.txt (18 functions), pair-proof.txt (two fixes), final-build.log and pair-build.log (unit checksums), progress-final.txt. Cohort and attempt records: /private/tmp/tokimemo-family-out/cohort.json and state.jsonl; run.log records elapsed time. Scratch scripts in build/family/ are local and ignored. The table above preserves the complete cohort and outcomes in versioned documentation.
