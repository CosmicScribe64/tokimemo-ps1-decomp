---
id: T-24040
title: OPTION menu sibling completion
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[option-family-results]]"]
---

## Goal

Recover OPTION func_8013394C, func_801342A8 and func_8013493C in80132000.c using matched same-file siblings. Preserve all existing matched bodies and earlier negative evidence.

## Acceptance criteria

- [ ] Actual default C linked proof plus OPTION checksum for every win.
- [ ] Preserve useful source-only negatives with provenance.
- [ ] Header/global/prototype checks and independent review.

## Notes

Read index, T-17800, T-20910 and option-family results first. Novel pass uses sibling selected-index/value-counter and state-machine recipes; current known types before padding hypotheses. One Docker compiler/ninja-j1, bounded25min; no other source/header changes without notice.

## Verified batch1

func_801342A8 (468 bytes) and func_8013493C (560 bytes) match1028 actual default linked bytes. Both target ASM macros removed; default --resolve --version default --no-build reports linked bytes equal targets; OPTION checksum and headers/globals/prototype guards pass (0new/15known). No shared-header edits or existing matched-body changes.

Both use the natural same-file hide/shrink/restore page recipe and Arrs4x8. 342A8 requires a meaningful cached u8 mode after its initial calls; without it, later state reload scheduling differs. Unknown unused frame area is explicitly FAKE:16words for342A8 and18words for3493C, declared before active rows to retain stack offsets0x38/0x34 and frame0xC0. Removing these gives smaller frames while active row offsets already match. Same-line flag-loop scheduling retains the marked T-7020 sibling recipe. These are documented frame/source-layout workarounds, not recovered author intent. 3493C keeps void signature after testing it also matches.
