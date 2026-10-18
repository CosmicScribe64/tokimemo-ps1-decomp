---
id: T-24040
title: OPTION menu sibling completion
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[option-family-results]]"]
---

## Goal

Recover OPTION func_8013394C, func_801342A8 and func_8013493C in80132000.c using matched same-file siblings. Preserve all existing matched bodies and earlier negative evidence.

## Acceptance criteria

- [x] Actual default C linked proof plus OPTION checksum for every win.
- [x] Preserve useful source-only negatives with provenance.
- [x] Header/global/prototype checks pass.
- [ ] Independent review.

## Notes

Read index, T-17800, T-20910 and option-family results first. Novel pass uses sibling selected-index/value-counter and state-machine recipes; current known types before padding hypotheses. One Docker compiler/ninja-j1, bounded25min; no other source/header changes without notice.

## Verified batch1

func_801342A8 (468 bytes) and func_8013493C (560 bytes) match1028 actual default linked bytes. Both target ASM macros removed; default --resolve --version default --no-build reports linked bytes equal targets; OPTION checksum and headers/globals/prototype guards pass (0new/15known). No shared-header edits or existing matched-body changes.

Both use the natural same-file hide/shrink/restore page recipe and Arrs4x8. 342A8 requires a meaningful cached u8 mode after its initial calls; without it, later state reload scheduling differs. Unknown unused frame area is explicitly FAKE:16words for342A8 and18words for3493C, declared before active rows to retain stack offsets0x38/0x34 and frame0xC0. Removing these gives smaller frames while active row offsets already match. Same-line flag-loop scheduling retains the marked T-7020 sibling recipe. These are documented frame/source-layout workarounds, not recovered author intent. 3493C keeps void signature after testing it also matches.

## Bounded final result

Two of three matched: func_801342A8(468) and func_8013493C(560),1028 actual default linked C bytes, pin68766d6d6. Target ASM macros removed; explicit --resolve --version default --no-build proves468/560 bytes. OPTION checksum passes3fd443d1e32404e5febd57d0586ef1c70ce8a811, headers/globals pass, prototype branch check0 new/15 known. No shared-header change or existing matched-body edit.

Direct sibling page recipe replaces m2c pointers/unrolled copies with natural flag loops and Arrs4x8. For3493C, all instructions except frame size already match (4 rows), while active rows are at the right offsets. An explicit FAKE18-word frame area before rows supplies the observed unused0x48 bytes and preserves rows atsp34. Void signature also matches and was retained. For342A8, meaningful cached u8 mode after initial calls fixes state reload/allocation differences; explicit FAKE16-word unused area gives frameC0 with rows atsp38. Putting frame area after rows was rejected because it moves active arrays. Existing same-line flag-loop scheduling is FAKE marked (T-7020). These unknown frame homes are source-layout workarounds, not recovered author intent.

Unresolved func_8013394C remains standard NON_MATCHING at5 rows/5 aligned, no verified-byte gain. The same-file typed sibling form repairs obsolete OptSpr44 to DisplayActor44; the older hand draft no longer compiles due removed type. Original keeps move v0,a2 in its first selector branch delay; current compiler drops that copy, shifting offsets. Signed selector/cast, assignment, bare return keep5; address-taken selector44/42, passing selector to speed calls36/8 or38/10, reused loop index20/8. No better form, no exhaustive search or compiler-impossibility claim. No FAKE needed for selected guarded body.

Read index, OPTION family, T-17800 and T-20910 before work. Only three assigned target bodies changed; all other C bodies preserved by literal target ASM replacement. All loose source forms, scripts, inputs and diagnostic instruction listings preserved intact in ignored build/T-24040-probes/. Older archives untouched.
