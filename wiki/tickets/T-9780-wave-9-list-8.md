---
id: T-9780
title: "Wave 9: list 8"
status: Done
assignee: wave-9-agent-8
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[register-allocation-wave7]]", "[[matching-notes]]"]
---

## Goal

Match the wave-9 list-8 functions within the 35 owned C files on branch w9-8.

## Acceptance criteria

- [x] Quick draft/build/diff pass and deep attempts on remaining functions; work at least four hours or deeply attempt every listed function.
- [x] Keep only byte-matching C; unresolved functions restored to INCLUDE_ASM.
- [x] Final Docker incremental ninja: 27/27 SHA1 OK, headers and globals checks pass.
- [x] sync_protos --check-branch and migrate_globals --check pass.
- [x] Inline review against CODING_STANDARDS.md; all findings resolved.
- [x] No commits, pushes, merges, tooling edits or edits to unowned C files.

## Notes

Shared brief and list: scratchpad/wave9-brief.md and scratchpad/wave9-list-8.txt.
Session began 2026-10-11 14:59 UTC. Scratch: scratchpad/w9-8/.

## Comments

2026-10-11: Read wiki index, shared brief, standards, workflow, register-allocation-wave7 and local skill setup. Claimed batch; quick pass followed by diff-ranked deep iteration.

Baseline Docker configure + ninja completed with all 27 SHA1 checks, headers OK and globals OK. The list-order quick pass now saves every draft/build/diff for deep iteration.

Deep scratch experiments started alongside the quick pass: 120 return-zero combinations for main func_80042940 remain four differing rows (last store/jr scheduling); five-minute permutation produced no improvement. GEKO func_80132A4C with a 22-byte aggregate and s32 fall-off reaches identical instruction rows. Exact/near duplicate reports contain no fillable owned functions.

Quick pass: EVENT `func_800F6E84` (376 bytes) verified by Docker ninja and funcdiff --resolve.

Type-repair diagnostic pass: copied draft-only GameState views feed migrate_globals in memory; opaque pointer offsets are rewritten with widths from target memory instructions. These are provisional isolated probes, not retained C. Real aggregate layouts and SDK packet types are preferred when integrating candidates.

GEKO `func_80132A4C` (216 bytes) verified in the real file with funcdiff --resolve and full Docker ninja (27/27 SHA1 checks). The 22-byte threshold aggregate is real; the unused first word and s32 fall-through are marked FAKE for frame/register matching.

Deep iteration batch: DATE `func_80139AC8` (488), TACO `func_8013801C` (324) and `func_80156A80` (1232), TT `func_801475C8` (124), RPG_BAT `func_801484E0` (372) pass real-file resolved diffs. Full-unit build pending. The TT loop needs marked empty branch/counter-copy source shapes; the TACO loop needs marked integer address arithmetic and setup-line scheduling. RPG_BAT's draft omitted the ignored third argument 0x5A to func_8014B738; the target call supplies it. Header generator added shared main global D_80122D34; overlay-local declarations appended only.

The seven-match batch (3132 bytes total) passes full Docker ninja, all 27 SHA1 checks, headers and globals. No unmatched draft is retained in tracked source.

Timed permutations completed for main return-zero helper, SHUGAKU func_801322F0, DATE func_8013E3C4/8013E6A4/801511B4, TT func_801475C8/80149108, main func_80072998 and RPG_BAT func_80148B08. The TT loop yielded a candidate, then cleanup and integrated verification. Other candidates remain in scratch; unresolved functions remain ASM. Aggregate copy repairs and a return-shape diagnostic pass continue.

DATE func_80149268 (168 bytes) matches with the actual Rec24 copy loop and marked s32 fall-through/setup-line scheduling. TT func_8013E6A0 (388 bytes) matches with two 0x80B8-byte frame records containing 0x5C draw and 0x14 display records; placing the saved state pointer first fixes its stack home. Both pass real-file resolved diff and respective overlay SHA1 checks. Nine retained functions total 3688 bytes; full Docker ninja after the header additions passes all 27 SHA1 checks.

RPG_BAT func_80136070 (316), func_801361AC (456), func_801421E0 (868) pass real-file resolved diffs. Restored ignored third arguments to func_8014B738 individually from assembly (0xB4, 0xC8, 0x5A and 0x3C); recovered D_8015EDB4 as the existing D_8015EDB0.unk_04 aggregate member. Appended dialogue-buffer declaration and forward declarations for existing void helpers. Twelve retained functions total 5328 bytes; full SHA1 build running.

The twelve-match batch passes full ninja, all 27 checksums. TACO func_8015776C (884 bytes) now passes real-file resolved diff: five s32 coordinate pairs (0x28) and five rotation words (0x14), existing TcPos by-value argument, explicit negation/shift and removal of redundant scalar locals recover all instructions and the frame naturally. Both void/s32 return forms match; retained void. Thirteen functions total 6212 bytes; TACO checksum running.

TACO checksum passes. RPG_BAT func_80142544 (364 bytes) passes integrated resolved diff after direct counter-member reads and marked s32 fall-through. Coordinate stores use existing RpgObjectBlock groups 43/44, avoiding separate declarations for D_801211D6/D_801211D8. Fourteen matches total 6576 bytes; RPG checksum running.

Main func_800722C4 (116 bytes) passes the integrated resolved diff with a local calendar-byte bitfield view and GameState pointer. Fifteen matches total 6692 bytes. The full after-fourteen Docker ninja completed all 27 SHA1 checks; RPG_BAT rotation helper checksum also passes.

Calendar helper checksum passes. Review cleanup: use fixed-width u32 bitfields and put CalendarFlag in the preserved main_api.h types section, as section 8a requires for game types; the header generator preserves that section. u8/u16 bitfield types change expression/register order (16 rows), while u32 is identical.

Deep-pass audit: appended 29 bounded register-gap rows to wiki/data/t0018-cases.md after running register_shapes for the compared functions (new TT initializer classified separately next). Each unresolved body stays INCLUDE_ASM. Further type repairs of main func_8006D6E0, TT func_80149400, GEKO func_80133024 and OLH func_80135B8C remain scratch only, with no matching claim.

Further calendar cleanup: the existing aligned CharFlags word view matches both the bit-13-to-bit-14 copy and clearing bit 13. Removed provisional CalendarFlag and all manual shift/mask expressions; isolated natural bitfield source matches every instruction. The 120 return-shape variants/300-second search in the return-zero family were applied to representative func_80042940; the other three helpers received focused return-shape checks, not independent 120-variant searches.

ENDING func_8013B510 (212 bytes) passes the integrated resolved diff. Existing Entry8 models its eight-byte records; explicit early return, predecremented s16 timer and counter normalization in the loop condition recover all instructions. Marked counter-copy and s32 fall-through choices. Sixteen matches total **6896 bytes**: size audit against asm headers corrects DATE func_80149268 from the list's 168 to 160 bytes (fifteen-match total was 6684, not 6692). Historical log entries remain unchanged. Full build after calendar cleanup passes all 27 SHA1 checks; scoped duplicate fill reports zero plans.

GEKO func_80133024 exposed another m2c argument-inference issue: func_80132D04 spills its second argument in a jal delay slot and later reads it from the incoming home slot. The draft omitted this consumed argument. Scratch repairs restore both s16 arguments but still differ in month/day/threshold register allocation. OLH func_80135B8C now has matching array homes, 192-byte frame and saved registers after four eight-halfword buffers, wide y, pointer-init ordering and provisional padding; twelve scheduling rows remain, with a five-minute search underway. Neither unresolved body is retained.

Inline review in progress: corrected spacing in the retained RPG_BAT func_80142544; reverified resolved MATCH. All sixteen retained functions pass full Docker ninja, 27/27 SHA1 checks. Deep TT func_80132CCC probes recover the timer update as an assignment expression, remove unused draft locals and place the remaining scalar declarations between saved pointers; frame and stack homes now match, leaving only the initial t6/t7 address pair (plus isolated-object padding). A five-minute permutation is underway. Main func_80042AC8 remains a broad 55-row allocation/store-CSE mismatch after 36 width/order/assignment-expression variants; no draft retained.

TT func_80132CCC passes the integrated resolved diff (572 bytes from the asm header, rather than the list's padded 580). Initializing the saved scene/state pointers in their declarations, reading the selector directly through TtCore30 and preserving one marked unused selector home removes the initial and later temporary swaps. TtButtons06 replaces the remaining halfword pointer arithmetic. Void return matches naturally. Seventeen functions total **7468 bytes**. Review also corrected the new drawing/display view fields to the required unk_XX spelling; func_8013E6A0 still matches. Full build running.

RPG_BAT func_801596E8 (1112 bytes) passes the integrated resolved diff. Flag byte D_801215FF and health halfword D_80121622 belong to the same 0x44-byte object record (index 155); expressing both through one RpgHealthBlock view prevents the premature health load. Separate-symbol and mixed views caused the prior four-row scheduling gap. Void return and separate statements match naturally; removed the unused draft local. Eighteen functions total **8580 bytes**. This resolves the previously appended T-0018 row; historical rows remain unchanged. The seventeen-match full build passes all 27 SHA1 checks.

Final deep-pass audit: OLH func_80135B8C remains eight isolated resolved scheduling rows from matching. Alternate result placement fixes the delay-slot copy but changes the buffer homes; additional pointer, argument-width, temporary, array and induction-counter views do not close the gap. Appended the thirtieth T-0018 row (new scheduling classification checked next). TACO func_80138964 remains a broad 106-row parameter/value allocation gap after control-flow, parameter-reuse, register and adjacent-record views; no source retained for either function. Full after-eighteen Docker ninja passes all 27 SHA1 checks.

Inline review final pass: replaced provisional TtDraw5C/TtDisp14 layouts with direct fields in the game-specific TtFrame80B8 view, retaining SDK RECT members. This avoids duplicating SDK environment structs; func_8013E6A0 remains a resolved MATCH. Existing CharFlags, Rec24, Entry8 and RpgObjectBlock types are reused; new aggregate offsets and extents are documented. All retained dummy homes, empty branches, integer-address, shared-line and fall-through choices have FAKE explanations and T-9780 references. Source stays C89, unresolved bodies remain INCLUDE_ASM, original order/placeholders are preserved, and no generated asm, raw data or tooling is edited. Final header/global checks: zero new prototype disagreements (17 known), branch check OK, globals OK. Scoped duplicate scan: zero plans; final checksum build and ownership/size audit pending.

Final source/asm-header audit confirms exactly 18 retained functions / 8580 function-range bytes and all changed C files are owned by this list. DATE func_80149268 and TT func_80132CCC each exclude eight bytes of list alignment padding. Documentation review corrected the newly added func_80042808 row: the exhaustive 120-form/five-minute family search ran on representative func_80042940; its siblings had focused checks. No historical row or old log entry was changed.

Final Docker incremental ninja (no -k) completes successfully with all 27 SHA1 OK, build/headers.ok and build/globals.ok present. Progress: grand total 4888/6958 functions, 785240/2279368 bytes, 34.4%; baseline was 4870 functions / 776660 bytes. Inline CODING_STANDARDS.md checklist is complete: all matching claims verified, no NON_MATCHING additions, C89/fixed-width/layout rules checked, SDK structs not duplicated, game symbols and original function order preserved, every retained FAKE documented, shared declarations generator-managed, no debug/dead source or raw assets, Docker-only tools, no staged files or commits, wiki/status consistency checked. Findings are resolved. Thirty T-0018 rows were added; one (RPG_BAT func_801596E8) was subsequently resolved and its history retained. Top non-register blocker is unrecovered aggregate layout; DATE SDK hand-written rotation helpers intentionally remain ASM. Tool reports include missed ignored call arguments, a missed consumed jal-delay-slot argument spill, a false argument inferred from an address-high base, and permutation setup exceeding the requested search limit.

Coverage audit: all 138 list entries have saved quick-pass draft/build/diff records (no missing entries). Deep attempts were ranked by repaired resolved differences and bounded by source/type/register experiments and five-minute searches. Explicit final check_headers.py reports headers OK. Saved logs and rejected candidates remain only in scratchpad/w9-8/; no scratch output or game data is staged.

Closed after the four-hour session window, 2026-10-11 14:59:50–18:59:50 UTC. Inline review passes with all findings resolved; final source diff remains clean. Ticket/kanban moved to Done. Changes are uncommitted for the orchestrator.
