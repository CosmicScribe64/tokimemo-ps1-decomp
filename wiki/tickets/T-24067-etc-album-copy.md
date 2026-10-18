---
id: T-24067
title: Match ETC album record copy
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24064-etc-count-trio]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80141ABC (300 instruction bytes) in src/ovl/ETC/80140F50.c. This single function is split from the deferred T-24064 trio; both other ETC targets remain deferred. Preserve DATE's verified checkpoint 2da9bb5f8.

## Acceptance criteria
- Read existing album-copy notes and archived negative results before experiments.
- Reuse built checkout on codex/complete-etc-album-copy from 2da9bb5f8, one existing GPT-6.1 Sol medium worker/compiler, Docker only and serial ninja -j1.
- Focus only this function until match or actual external blocker; no broad batches or extra agents. Reserved OPTION/BUNKA_SD targets remain excluded.
- Coordinate exact shared-header/config changes before edits. Actual C requires unconditional default body, explicit linked literal funcdiff, ETC checksum, interface guards and audit of other bodies.
- Parent owns independent review, integration and push; wait for its DATE integration result before changing the baseline if needed.

## Notes
Prior source recovery identifies an eight-byte GsRec75E record copied into a save-side album cleared at root+0x58 for 0x800 bytes. Byte-only destination fields reproduce unaligned word operations. Correct eight-byte cursor stride and word counter reached35 then27 differing lines; saved-register order, copy temporary and stack arguments remain after five-minute search, pointer-order and end-pointer controls. Seek evidence-backed source/layout changes rather than repeat these sweeps.

## Comments
Parent assigned this exclusive target after receiving verified DATE pin; root integration remains in progress. Other deferred ETC work stays Ready in T-24064, not started by this ticket.

Initial semantic audit: source cursor starts at GameState base and advances eight bytes until base+0x800, while each record access is at cursor+0x75E. This walks all 256 recovered GsRec75E records; advancing a GameState pointer by eight elements would be wrong. Destination uses existing Work800E7D10.unk_058 and a byte-only GsRec75E assignment to preserve unaligned transfers. Save count/mode bytes are existing unk_027 indices 1/0. First control uses these canonical aggregates and a word count, without header changes. Parent reports DATE published at 17f1bb15b with all checks; baseline bodies need no update.

Bounded controls reproduce the old 27-row best only when the loop end has an independent address identity (private D_800E6A80 declaration or literal-address diagnostic); canonical common-base expression gives35 and explicit end local31. Actual field endpoint, argument-width/casts, postincrement count, byte-array/halfword copy layouts, explicit save pointer and signed save-byte stores do not improve the27-row result. For/while preheaders worsen to36. Remaining27 comprise saved save/end register order, stack-argument ordering, second copy temporary and mode-store/last-call constant scheduling. No production/header/config change; separate endpoint alias would require reporting before promotion. Existing record assignment/stride semantics are correct.

New source gain: retain mode=1 store and pass that stored field to the final printf. This restores the entire final mode/call sequence; private cost22/26 rows remains. SDK printf versus alias, void/s32/K&R diagnostic views, direct repeated argument reads, existing byte reuse and bounded source-line grouping offer no further improvement. No ABI/header change is proposed. Other groups remain exact frame/table-free layout, swapped saved endpoint/save base and one copy scratch register plus opposite equal-value stack argument ordering.

Approved existing private allocator tracer /private/tmp/t24047-uopt/variants/trace diagnoses three equal-priority hoisted constants: fmt/save/end bits52/53/54 each3.33333325, hence first-use tie colours s2/s3/s4. Desired order is fmt/end/save. Stock and trace on identical input have equal .text/.rel.text/.rodata/.rel.rodata; the target words also agree with the best diagnostic body. Explicit pre-loop end local outranks format and takes s2; explicit format initialization folds away. Private tracing alters no production tools or binary. The remaining copy t5/t6 is a separate ugen scratch choice.

New pinned source lead drafts/T-24067/body.c: cost18/28 rows, exact frame0x48/75 words. A FAKE eliminated format identity retains the format range and restores desired saved allocation. Canonical GameState+0x800 endpoint works in this form, removing all need for alias/interface changes. Safe replay confirms28 resolved rows. Grouped initializers reach26 at equal cost18; only preheader order, two stack stores and second copy scratch differ. Source-only checkpoint is being pinned while this same target remains active; no production C match claimed.
