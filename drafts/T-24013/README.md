# T-24013 RPG_BAT battle-family completion

Source: codex/complete-option after 48d6d605a, 2026-10-17. Bounded continuation of unstarted w4-p40, using T-17100 types and T-9960 / T-21001 RECT provenance. The separate review repair 7ec3c5626 fixes the prior GEKO probe exit status; it is unrelated to game bytes.

## Verified state machine

func_80139510 matches 2,424 literal linked bytes; worklist 2,616 includes associated rodata. The census's sixteen halfword locals are four SDK RECTs. Directly update D_8015EE30.unk_08 and D_8015EDA0 instead of splitting ranges across m2c temporaries. Existing file-level implicit-int func_8013E7C0 view already keeps the power counter in v1, closing the historical T-9960 allocation blocker. With separate RECT assignment statements the best form has 32 differing rows / 16 aligned edits: only the order of four groups of halfword stores differs. Pair each x/y assignment in one comma expression and each w/h in another to reproduce the exact original schedule. Reversing individual stores or nesting assignments worsens the order. Two unused s32 words before the RECTs supply eight original frame bytes; this is explicitly FAKE with its reason and ticket. Three existing callbacks require forward declarations; header additions preserve all other default bytes.

Docker ninja -j1 build/ovl/RPG_BAT.ok passes; funcdiff.py --unit RPG_BAT --resolve --version default --no-build func_80139510 reports linked target-byte equality (2,424 bytes). No inline asm, per-function build patches, or compiler modification.

## Unresolved attack scene

func_80138324 starts at 176 rows from T-17100 typed source (DisplayActor44). Direct global decrement/test removes the split EC74 range (162). Unsigned lvalue views of actor/target selectors and moving a genuine pointer declaration before the used spill produce 142 rows / 138 aligned edits (body.c). The views are experimental FAKEs and not promoted. Original actor index remains in t0 across the scene's cases, source in v1; record pointers use v1 in original and v0 in source. The spill offset is now 0x30 as in the original. Single pointer range, s32 return and implicit-int func_8013E97C views do not fix allocation. Original text/control-flow shape otherwise follows the matched enemy-attack siblings. Production stays INCLUDE_ASM.

## Unresolved transition scene

func_80152F30: current folded baseline321 rows / 309 aligned edits (body.c). Extra two-word frame experiment gives317; using the established implicit-int E7C0/E97C views with it gives307 / 295 (views.c, file_views.h). This is an experiment only: the views affect the whole TU, and no changed view was retained in src or header. Frame-only homes are marked FAKE in views.c. Direct timer updates, including unsigned GameState union view, worsen the shape to421-425 rows. Original color constant uses t0 and source t1; many later ranges shift, and original generic checks/schedules differ. Color/result type and inline-selector probes are recorded with the preserved loose diagnostics. Production stays INCLUDE_ASM.

## Diagnostics

probe.py scores JSON [label, full function body] pairs in the complete translation unit. Optional third argument prepends a file of per-file view guards for scratch-only tests. Every candidate ERROR is retained; any exception makes the script exit 1 after the batch. All loose scripts, JSON inputs, candidate sources and instruction diagnostics are preserved under ignored build/T-24013-probes, without deleting anything. Only the selected source drafts and provenance are committed.
