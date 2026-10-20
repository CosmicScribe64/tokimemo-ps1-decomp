---
type: research
updated: 2026-10-17
---

# MASTER family results

MASTER is an event overlay (load address 0x80132000). Its code is a set of scene handlers dispatched through
function tables (`func_80132140`, `func_80133A2C`, `func_80139150`), two near-identical mini-events built on
the display records from record 96 on (`D_80120650`, read through the overlay's `MasterSprite44` view), and
small setters for the scene's three asset pointers. Earlier work:
[[master-types-pilot]] (T-13000/T-13001: three typed-table matches), [[tickets/T-0700-overlay-batch-a]].

## Types-first pass (T-18600)

T-20900 update: `MasterSprite44` is `DisplayFixActor44` (`unk_26` is `unk_24.h[1]`; the redraft record view follows), `MasterFixed32` is `Fix32`, `MasterClubBits` is `ScheduleClubFlags` (`count` is `unk_25`) ([[data-types]], "Shared-type unification").

[[tickets/T-18600-types-first-master]] recovered MASTER's shared types before function matching, redrafted the
11 functions still unmatched at `w2-good` (17,412 bytes, none guarded) and cut the rest into three packages
(branch `w3-master-types`). Drafts and the driver: `drafts/T-18600/` (README there). About 75 minutes of agent
time for types, driver, redraft and two matches.

### Type map

All changes keep MASTER's sha1.

| Type | Symbol(s) | Used by (unmatched) | Evidence |
| --- | --- | --- | --- |
| `MasterWords13` (`s32 v[13]`, 0x34) | `D_8013C2F4`, `D_8013C328`, `D_8013C35C`, `D_8013C390`, `D_8013C3C4`, `D_8013C3F8`, `D_8013C42C`, `D_8013C460`; `D_8013C7B4` (was the local `Tbl13`) | `func_801326B4` | eight tables copied whole into the frame (12 `lw/sw` pairs plus one word each), then read with `lw` at `sp + D_8013C2E0 * 4`; data: 13 text ids each (three zero words follow `D_8013C460`) |
| `MasterHalfwords13` (existing) | `D_8013C6A4` | `func_80135CF4` (matched) | six words and a halfword copied to `sp+0x28`, read with `lh` at `sp + D_8013C584 * 2`, as `D_8013C6C0` in the matched `func_80137044` |
| `FnTbl4`, `FnTbl7`, `FnTbl30`, `PtrTbl3` | `D_8013C2E4`, `D_8013C798`, `D_8013C5C8`, `D_8013C700` | | moved from four `.c` files into `MASTER.h` (the T-3330 step tables) |
| `MasterClubBits` (existing) | | `func_80135FB8` | the two local `MasterSlot` copies in `80132140.c` and `80139000.c` had the same layout (`grade` = `count`, `flag` = `b30`); one type now |
| `u8 D_8013C788[5]`, `u8 D_8013C780`, `s16 D_8013C794` | `D_8013C78C` is element 4 | `func_80139CC8` | the second mini-event's copy of `D_8013C574`..`D_8013C580`: `lbu`/`sb` accesses, a walk that stops at `D_8013C78D`, element 4 swapped with element 0 |
| `void func_8013348C(void)` | | `func_80133374` (matched caller) | the definition (matched) returns nothing; the call came before it |

Not changed, with the reason:

- **Resource records** `D_8013C710`..`D_8013C770` and `D_8013C4B0`..`D_8013C4D0`: 0x10-byte groups of three
  pointers and a halfword, written into the sprite records by matched handlers as scalars (65 lines).
  The unmatched functions read them only at constant offsets. Same decision as ENDING and SHUGAKU
  ([[ending-family-results]], [[shugaku-family-results]]).
- **The three-pointer groups** `D_8013C2D0`, `D_8013C4A0`, `D_8013C750`: the setters in `80132000.c` match as
  separate scalars.
- **type_recovery's top proposals**: `D_8013C5C0` x46 (stride 0xC) starts inside `D_8013C5B4` and runs over
  unrelated tables; the jump-table merges are not data. Already rejected by [[master-types-pilot]].
- **`D_80120650`** stays `u8[]` in `main_api.h` (EVENT overrides it as `s32`); the drafts and the matched code
  read it through `(MasterSprite44 *)`.

### Driver changes

`drafts/T-18600/redraft.py` is `drafts/T-18300/redraft.py` (T-16550 fold, T-16560 rewrites and their later
additions) with these changes:

- **views**: both `D_8011ECD0` and `D_80120650` are `MasterSprite44[]` in the m2c context; the fold picks the
  innermost range, so `D_80120657` becomes `D_80120650 + 7` (record 0 of the sprite base) and not
  `D_8011ECD0 + 0x1987`. The base label itself is not folded (the first run turned `&D_80120650` into
  `&D_8011ECD0[96]`).
- **record-copy** (new): m2c's 68-byte `M2cBlock44` copy between sprite records is one `MasterSprite44`
  assignment.
- **reroll** (new): IDO unrolls a constant loop over records by four, and m2c prints the copy as a pointer
  walk with temporaries and byte offsets. When every record of an iteration gets the same field updates, the
  walk becomes `for (i = a; i < b; i++) { REC[i].f op= c; }` (the GameState girls `D_800E6280.unk_1BC[i]`
  and the sprite records); for sprites the records peeled before the walk are folded into the loop. The
  GameState loops keep their peeled record 0, as in the matched `func_80133374`.
- **club-bits** (new): field-symbols writes GameState word +0x684 as the record `D_800E6280.unk_66C[6]`,
  which C cannot shift; the shifted bits 25..26 are `MasterClubBits.count`.

### Redraft

Scores are full-unit diff rows (as `drafts/T-14500`).

| Function | Bytes | Census | Plain | Typed | Final | Rewrites |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| `func_80132444` | 624 | 24 | 24 | 22 | 22 | fold (GameState fields) |
| `func_801326B4` | 3,264 | 957 | 957 | 397 | 397 | stack-table |
| `func_8013348C` | 500 | 144 | 144 | 158 | matched | hand |
| `func_801344E0` | 2,912 | 1,014 | 1,014 | 475 | 475 | display-records, record-copy, reroll |
| `func_80135CF4` | 708 | 149 | 149 | 74 | matched | stack-table, then hand |
| `func_80135FB8` | 1,084 | 153 | 153 | 190 | 153 | (typed worse) |
| `func_801363F4` | 2,608 | 698 | 698 | 479 | 479 | display-records, record-copy, reroll |
| `func_801391E4` | 1,580 | 270 | 270 | 270 | 270 | none apply |
| `func_80139CC8` | 2,748 | 885 | 887 | 678 | 678 | display-records, reroll |
| `func_8013B448` | 956 | 329 | fails | 291 | 264 | struct-bytes, draft_fixes |
| `func_8013B804` | 420 | 170 | 170 | 88 | 88 | fold (GameState fields) |

Against the census: 8 better (14,240 bytes), 3 the same, none worse; median 270 to 264 rows over all 11, sum
4,793 to 3,044 (the two matches counted at their draft scores, 144 and 74). The headers alone (plain) changed
nothing but `func_80139CC8` (+2), and `func_8013B448`'s fresh m2c draft does not compile (the census used a
repaired T-16100 body). The reroll alone took `func_801344E0` from 729 to 475, `func_801363F4` from
636 to 479 and `func_80139CC8` from 719 to 678. No draft scored 0.

Matched: `func_80135CF4` (708 bytes) and `func_8013348C` (500 bytes). Hand drafts (`hand.c`, register
allocation only unless noted): `func_80132444` 22 lines, `func_8013B804` 72 lines, `func_80135FB8` 132 lines
(a port of its matched twin `func_80133F80`).

### Idioms found on the way

- A local declared above a stack table or a spilled byte moves it down by one slot. `func_80135CF4` needs an
  index local declared before its `MasterHalfwords13` copy (table at `sp+0x28`), as `func_80137044` does.
  `func_8013348C` needs the switch selector in a named `u32` local declared before the event kind, which then
  lives at `sp+0x2B` and not `sp+0x2F`.
- The byte view `(*(u8 *)&D_8013C584)` keeps every read of the selector in `v1` (FAKE, as in
  `func_80137044`).
- `func_80135FB8`'s case 0 is `func_80133F80`'s case 3 with the threshold `0x140` instead of a table entry;
  the port reaches 132 lines, but IDO keeps `&D_8013C57C` in `s2` from the first loop on, where the original
  materializes it only for the shuffle loop.
- `func_8013B804` stays a register-only near miss with every form tried here and in [[master-types-pilot]]:
  the original gives the selector load and the GameState base the same register (`v1`) and the first case
  constant `a0`.
- `func_80132444` stores `|= 3` into the low byte of `D_800E7D50` (`lbu`/`sb`; `main_api.h` declares `s32`);
  only case 4's registers differ.

### Packages

Three file-disjoint packages from `inventory2.py`/`mklists3.py` (lists in the orchestrator's scratchpad,
`w3-master-p01-list.txt` to `w3-master-p03-list.txt`). `func_801344E0` (475) and `func_80139CC8` (678) are
not in a package.

| Package | Ticket | Owned C file | Functions | Bytes |
| --- | --- | --- | --- | ---: |
| w3-master-p01 | T-18601 | `80132140.c` | `func_80132444` (start from `hand.c`), `func_801326B4` | 4,044 |
| w3-master-p02 | T-18602 | `80139000.c` | `func_8013B804` (`hand.c`), `func_8013B448`, `func_801391E4` | 3,056 |
| w3-master-p03 | T-18603 | `80133680.c` | `func_80135FB8` (start from `drafts/T-18600/.../hand.c`), `func_801363F4` | 3,772 |

## T-24038 event-record handler

[[tickets/T-24038-etc-master-siblings]] recovers `func_8013B448` in `src/ovl/MASTER/80139000.c`: 956 default linked bytes match. The matched `func_80138088` logger provides the direct `GameState.unk_75E[index]` field idiom. Correcting the old case 3/12 pointer truncation artifacts, retaining one GameState pointer, and using ordinary increment then unsigned clamp closes the draft. The sibling's void view is supported by the unused caller result; the approved prototype also rebuilds the earlier caller, and the whole MASTER checksum passes. No new FAKE code is needed. Source-only provenance and rejected controls are in `drafts/T-24038/README.md`; proof/guards are under `build/etc-master24038/`. The two-target package totals 1,752 bytes and awaits independent review.

## T-24043 larger final-event handler archive

[[tickets/T-24043-master-large-handler]] leaves production `func_80139CC8` in assembly and adds no verified C bytes. `drafts/T-24043/` preserves a corrected full-TU source baseline with real `DisplayFixActor44`/`Fix32` fields, a legal five-byte array swap that saves both old values, and probability reloads after RNG calls. Natural thresholds/shared locals reach the target 0x48 frame without padding; a tagged FAKE scalar probability-byte view controls promotion. The archive measures cost 342 / 524 printed rows; this aligned cost is not the old T-18600 row metric or a linked match proof.

Three live spill homes remain +4, the external-array swap adds a LUI, case 8/later loops rotate registers, and case 9 branches differ. Reordering declarations and bounded induction/control-flow forms did not close these gaps. A private tentative array definition emits COMMON and changes nothing; a private strong `.data` definition removes the local swap differences but leaves the other blockers (357/375). This isolates an ownership effect without establishing a full data-island solution. No production, header or config edits were made; all previous matching bodies remain exact. Archive README records the controls and reproducible measurement command.
## T-24048 animation source pass

[[tickets/T-24048-master-animation]] retried func_801344E0 in its own TU, without changing production C or headers. Fresh shared-type control reproduces475 rows/469 aligned; natural clone-pointer lifetime, explicit primary fade updates and direct counter increments produce a readable434/428 source lead. No match or new bytes. The current frame is72 versus64; the clone end pointer is hoisted across RNG and copy setup consumes a different temporary rotation. The scratch end-label hypothesis does not fix either. Historical T-10630's350-row handoff is unavailable in this checkout, so434 is not claimed as the historical best. Source-only selection, bounded negatives and reproducer: drafts/T-24048/README.md. No compiler-impossibility conclusion; no per-function patch.

## T-24514 narrow source reconstruction

[[tickets/T-24514-master]] adds zero verified C bytes. Current shared types bring
the35FB8 matched-twin port to58 aligned; bounded initialization/shuffle-pointer
lifetime controls do not improve it. The retained363F4 reconstruction uses its
own early cases and checks/actions with the archived, nonmatching T-24048
344E0 shared-loop source. Indexed clone plus removal of a one-use RNG local
scores342 and restores the64-byte frame. First213 instructions align, then the
copy end uses t3 instead of original t2; clone compare/spill homes and later
register rotation remain. Byte/integer pre-copy store views342/439 do not close
it. This is a source boundary, not a compiler-impossibility claim.

Narrow compiling bodies, exact controls and Docker replay live in
`drafts/T-24514/`; no full-TU context or original assembly dump is retained.
Production and headers are unchanged.326B4 historical correct-frame source
rescores538; its known selector-promotion blocker was not exhaustively retried.
