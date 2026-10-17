---
id: T-9960
title: "Wave 10: list 6"
status: Done
assignee: w10-6
created: 2026-10-11
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the 21 large functions (60,464 bytes) assigned in scratchpad/wave10-list-6.txt, within the 17 owned C files. Follow the wave-10 brief and spend at least four hours on substantive matching work.

## Acceptance criteria

- [ ] First-pass drafts and resolved diffs for the assigned functions; deep iterations on promising candidates.
- [ ] Every retained match verified in Docker; useful near misses guarded and ticketed.
- [ ] Final incremental build passes all 27 unit checks and header/global guards.
- [ ] Prototype branch check and inline standards/spec review complete.
- [ ] Findings, attempts, and final progress documented; changes left uncommitted.

## Notes

Started 2026-10-11 20:16 UTC. Scratch is isolated to scratchpad/w10-6. Baseline worktree was clean. No generated build or assembly directories were present at start.

## Comments

- Read the wiki index, shared brief, local instructions, coding standards, workflow, allocator evidence, and relevant prior overlay notes. Prioritize control flow and recovered aggregate types before register tuning.

### Initial findings

- BUNKAKEN `func_80149144`: normalized m2c draft compiles in Docker. First resolved scratch-object diff has 167 changed instruction lines, predominantly temporary order and signed selector registers; control-flow size is already close. Saved draft and exact diff in scratchpad/w10-6.
- DATE `func_80144E20`: m2c exposes 21 local aggregate copies and a large stack frame; needs real wrapper types/copy extents before compilation. Do not treat this as a register-order failure.
- Early EVENT/MASTER draft calls ran before the baseline split generated their assembly; these were setup errors and will be retried, not counted as matching attempts.

### Verified matches

- BUNKAKEN `func_80149144`: 3,996 actual text bytes (`0xF9C`; queue estimate 4,404). Resolved real-file `funcdiff` MATCH and BUNKAKEN SHA1 OK. Fixed two equality operand orders, used unsigned word views for nested selectors, and switched directly on the shifted byte rather than m2c's derived-value local. No FAKE needed. Ten source variants checked including baseline; best standalone object only differed by its trailing alignment nop, which disappeared in the real object.
- BUNKASAI `func_80147214`: 3,448 text bytes (`0xD78`; queue estimate 3,780). The same operand-order, unsigned nested selector and direct shifted selector forms give real-file resolved MATCH and BUNKASAI SHA1 OK. Three targeted variants checked after the first-pass draft.

Baseline build completed with 27/27 SHA1 checks and both header/global guards. The queue's supplied sizes include more than the actual instruction spans; match-byte totals use disassembly/progress text bytes.
- MASTER `func_80138490`: 2,504 text bytes (`0x9C8`). Eight comparison-order/unsigned-selector variants reduced 113 changed instructions to a standalone alignment nop. Added the missing declaration of the already matched `func_80138E58` in the overlay header; real-file resolved diff and MASTER SHA1 pass. Total now three functions / 9,948 bytes.

### Survey and next deep attempts

All 21 drafts were generated and compiled or diagnosed in Docker. Type repair made eight remaining large routines compile; the rest need copied-table/record layouts. Five related BUNKAKEN/BUNKASAI state machines share two bit-field store blocks; recovered `b1` and three-bit selector accesses reduce two drafts from about 200 changes to 50. A five-minute single-worker permuter run is testing one of those drafts. Main globals D_80122E98/EB0/EB4 use word accesses; D_800E7D4E is an unsigned byte. Type recovery proposes a broader D_80122DC0 array, but the existing shared API retains scalar views across that range, so no broad type migration is attempted in this batch.
- MASTER `func_801350A8`: 2,080 text bytes (`0x820`). Recovered the eleven-word text-id aggregate (0x24-byte copy loop plus two trailing words = 0x2C), split m2c's merged case tail, fixed birthday operand order, and used a byte flag local reloaded at each test/store. A real table-index local declared before the aggregate and the byte flag after it reproduce frame 0x70/table +0x40. Removed exploratory padding; no FAKE retained. Scratch exact score zero, then real-file resolved MATCH and MASTER SHA1 OK. Batch: four functions / 12,028 bytes.

Scoped duplicate search after the first three matches: zero plans applied. The BUNKASAI bit-field permuter completed its 300-second run without score zero (best weighted score 520); no candidate applied.

### Second deep-pass checkpoint

- DATE `func_80144E20`: recovered all 21 copied callback tables (0xE00 data bytes). Declaration ordering reproduces the individual table offsets, but the frame is still eight bytes too large and the selected callback pointer has the wrong live range; best draft remains scratch-only.
- TAIIKU `func_80140A90`: replacing m2c's external memcpy placeholder with the existing `TaiikuScene44` assignment reduces the miss to 92 changed lines. Remaining regions are a two-record loop's pointer/bound registers, aggregate-copy tail scheduling, and the fade byte's live range.
- BUNKASAI `func_80139D84`: systematic ordering of the five independent continuation/flag stores reduced 50 changed lines to 22. Started a second five-minute single-worker permuter on that improved source. BUNKAKEN `func_8013FD40` has the same store pattern.
- BUNKAKEN `func_80143DD0`: repaired inferred extra call arguments, a byte store misrendered as a word store, and a record index misrendered as word-pointer arithmetic. A scratch unprototyped view of `func_800847B8` removes the spurious argument truncation; remaining bit-field and temporary-order regions still need work.
- MASTER `func_801344E0`: reconstructed the 0x44 sprite record and position/speed arrays; draft now compiles. Array loops reproduce the initial thirteen-record stat updates. Sprite initialization, fade-loop shape and local lifetime still differ.

### Four more matches and full checkpoint

- BUNKASAI `func_80139D84` (0x890 / 2,192 bytes) and BUNKAKEN `func_8013FD40` (0x770 / 1,904 bytes): both continuation flag stores precede the three continuation-byte copies in C. That ordering gives exact code. Added byte bit-field views in each overlay header and generated the four missing shared scalar declarations with `sync_protos.py --add-global`. Real-file resolved diffs and both overlay SHA1 checks pass.
- BUNKASAI `func_8014F36C` (0x904 / 2,308 bytes): the same ordering also applies to mode 2 (encoded as bit value 8). The real-file resolved diff matches and its overlay checksum passes.
- BUNKAKEN `func_8014C374` (0x7D8 / 2,008 bytes): repaired the character bit-2 test/store and used a local `GameState *g` for the existing aggregate. This resolves the remaining selector/base-pointer allocation differences. The full build passes with the new function compiled from C.
- Required forward declarations were appended for the already-defined overlay helpers `func_8013B120`, `func_80140EF4`, `func_801508C4`, and `func_8014E3DC`.

Eight matches total 20,440 text bytes. The full incremental checkpoint passed all 27 SHA1 checks and the global guard; the header guard passed in the preceding affected-overlay build. The second five-minute BUNKASAI permuter run did not improve its old 22-line seed; the manual store-order search had already found the exact source.

### Ninth match and remaining aggregate work

- BUNKAKEN `func_80152680`: 2,252 bytes (`0x8CC`), resolved MATCH and overlay SHA1 OK. Recovered 12/11/4-entry text-pointer aggregates, a used index before them, and the GameState pointer view. Direct counter updates and removal of an m2c self-assignment restore the frame and scheduling. The caller needs an unprototyped `func_800847B8` view to pass its full word; the override is scoped and documented. Batch total: nine functions / 22,692 text bytes.
- RPG_BAT `func_80146C30`: restored seven third arguments omitted by m2c's inferred two-argument `func_8014B738` signature and recovered the three-angle array. Best scratch source has 24 changed lines; five-minute permutation found no improvement. Remaining differences are pointer/bound register order and the constant 144's live range.
- RPG_BAT `func_80139510`: four RECT locals, restored call arguments, and real 0x44 object-record members reduce the miss to 37 lines. The remaining frame, rectangle store scheduling, and power-counter register are being isolated.
- MASTER `func_80139CC8`: m2c misrenders the second store of a four-byte-array swap when it shares a high relocation with the fifth byte. Corrected the scratch C from the actual LO relocation; no incorrect swap is retained. EVENT's three drafts now compile after recovering Rec24 fields and RECT locals, but still require control-flow work.

### Third deep-pass checkpoint

- RPG_BAT `func_80139510`: paired x/y and w/h assignments restore the four RECT store schedules, reducing 37 lines to 21. An exploratory eight-byte local reproduces the frame and leaves 17 lines around D_8015EDA0 in v0 rather than v1. Scalar, unsigned, aggregate-view, chained-assignment and clamp forms did not resolve that range. The five-minute permuter on the 21-line seed found no improvement; no workaround is yet retained.
- EVENT `func_8011D5C4`: moved its common animation tail after the switch, reused a narrow index and recovered the post-loop record access. Best source improves 729 to 532 changed lines. `func_8011C750` drops from 1,354 to 607 after recovering its two initialization loops, four reset cases and phase-bounce control flow. `func_8011DDB8` drops from 1,735 to 1,084 after the corresponding index/record repair and removal of inferred stale-register arguments. All remain scratch-only. EVENT has zero PsyQ gcc fingerprints.
- DATE `func_80144E20`: direct table indexing and the local GameState view improve 588 to 515 changed lines and remove the extra frame bytes. Callback selection and later control flow still differ.
- MASTER graphics functions: tested explicit leading sprite updates, pointer lifetime reuse and the actual adjacent-table end address. Pointer reuse alone worsens allocation; the adjacent end reduces func_801344E0 to 420 lines. The record-copy tail and later temporary sequence remain the main differences.

BUNKAKEN func_80143DD0 now reaches eight differing lines. The signed-halfword nibble cast, explicit bit equality, and direct selected-character accesses fix all preceding regions. A five-minute permuter is testing the masked-switch residue. The nine-match checkpoint passes all 27 unit hashes. The MASTER/80139000 data-island dry run confirms a viable data range, but applying it requires forbidden YAML edits; retain assembly and hand off that integration step. Started a supplemental survey of 83 non-assigned functions within the existing seventeen owned files, as allowed after the deep pass.

2026-10-17: Closing as superseded. The Codex run hit its usage limit with 9 of 21 functions merged (BUNKAKEN, BUNKASAI and MASTER, 22,692 bytes), all still C on main; RPG_BAT func_80146C30 has matched since and the other open ones are in the T-14500 census. Review: no standards findings in the merged code (no FAKE or NON_MATCHING needed, new types document their sizes, the func_800847B8 override carries its reason). The 8-line BUNKAKEN func_80143DD0 and 21-line RPG_BAT func_80139510 near misses were never committed, so a followup recovers them.

Followup opened: [[tickets/T-21001-recover-wave-10-list-6-near-misses|T-21001]].
