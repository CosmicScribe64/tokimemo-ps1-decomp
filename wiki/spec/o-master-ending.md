---
type: port-spec
updated: 2026-10-24
status: proposal
---

# Club mastery and endings (MASTER + ENDING)

This is a bounded source-backed slice for port planning, not a complete naming pass. The disc map assigns MASTER `master_` and ENDING `ending_`; shared words such as `confession_` and `epilogue_` are not overlay prefixes in the ADR and must not be used as row prefixes. O.BIN identifiers remain untouched. Current applied core identifiers (not historical aliases) are used below, including `game_state`, `mode_stage`, `mode_step`, `day_event_done_bits`, `player_flags`, `clubs`, and `event_log`.

## Club tournament / mastery flow (MASTER)

`func_80132140` dispatches `mode_stage` through `D_8013C2E4` (four callbacks). `func_80139150` dispatches a separate seven-stage sequence through `D_8013C798`. These are explicit state-machine entries; names for their precise tournament/award narrative await cross-caller confirmation.

In `func_801391E4` (`src/ovl/MASTER/80139000.c`), `mode_step` drives setup and result selection. At step 2, after CD readiness, it first calls `func_8008647C()` for a candidate and checks Rei (`girls[0xC]`) flag byte at index 3 against `0x41`. Otherwise, if the initial candidate rank is below 2, it selects eligible met/active girls whose rank is below 2 and whose club nibble (girl flags byte 2 >> 4) matches the player's club selector (`player_flags.h >> 12`). The loop overwrites the candidate, so the last qualifying index wins except for the explicit 0-versus-10 comparison. If both 0 and 10 qualify, rank eligibility selects the sole qualifying one; if both qualify, affection+tokimeki totals are compared and `>=` favors index 0. If initial candidate rank is not below 2, Miharu (index 11) is selected when her `flags.b[3] >= 2`; otherwise the selector becomes `0xFF`. The selected value `D_8013C790` is subsequently used for voice, portrait and current-girl setup, so this is named candidate selection rather than winner selection. These directions and the tie sign are directly established by the conditions in the body.

`func_8013B20C` is the result exit. Once its audio poll is ready, it sets `game_state.day_event_done_bits |= 0x10`; only when `D_8013C784 >= 3U` does it set `clubs[player_club].b30 = 1`, then it calls `day_event_next(1)`. The flag is strongly indicated as club mastery state by the code context and glossary, but its exact semantic bit name should remain open pending corroboration. A result-log helper (`func_8013B448`) appends scene `0x15` and the current date/girl to `event_log` before a tournament-class switch.

In `func_801391E4`, step 6 only calls `normal_date_speak()`. The practice-point margin is in `func_80139CC8` step 6: it computes `clubs[player_club].practice_points - 350` and maps it to `D_8013C784`: below -30 => 0; [-30,-21] => 1; [-20,-11] => 1; [-10,-1] => 2; [0,9] => 3; [10,19] => 4; [20,29] => 4; 30 or more => 5. (The consecutive intervals simplify to < -30:0, [-30,-11]:1, [-10,-1]:2, [0,9]:3, [10,29]:4, >=30:5.) `func_8013B20C` awards club `b30` when that score is at least 3, equivalent to practice points >=350. So the source establishes an effective practice-point mastery cutoff even though the stored bit's other uses and game-facing label remain to be fully cross-checked.

Other visible branches: in `func_80139910` the selector `player_flags.h >> 12 == 7` skips a background read and place movement; `func_80139810` starts a portrait read unless candidate is `0xFF`, otherwise advances without it. Their exact event labels are not asserted here.

## Graduation candidate selection (ENDING)

`func_80132334` dispatches stages 0 through 7 to callback groups; the default runs `func_80132400`. Stage names are scene-level roles, so only the dispatcher receives a proposed name in this slice.

`func_801330E4` fixes the graduation date to year `0x62`, month 3, day 1, Sunday, then calls `func_80053418(rank, points)`. It starts with Yoshio. Rei (index 12) wins the special branch only if `points[0] < 500`, `girls[12].flags.b[3] >= 70`, and `scene_flags[0x4F] == 2`. Otherwise, indices 0..10 are considered in order, and a met girl replaces the current best only when `points[i] > best`, with best initialized to 499. Equal scores do not replace the earlier winner. If no score exceeds 499, Yoshio remains selected. Only then, when Yoshio remains, a Miharu selection is possible: level is girl 11's story stage plus 2 for each positive scene flag 0x4C and 0x4E, clamped at 17; the chance test runs only when the story stage is nonzero and scene flag 0x4D is zero, and selects Miharu if `rand() % 256 < D_8013C3B0[level]`.

The next callback `func_80133460` either advances mode_step or, when candidate index is at least `GIRL_YOSHIO` and girl 14's `flags.w` bits 9..11 are below 3, sets mode_stage 7 and `D_800E7D34` bit 3. The selected-candidate/flag relationship is not fully decoded. Later `func_80139A58` explicitly sends Yoshio to stage 7, step 15 and every other candidate to stage 8.

`func_80137730` samples CD audio level during the ending: it reads `getCDlevel()` (or zero if `cd_audio_state` bit 2 is clear) into a four-frame ring. It selects the maximum using strict `<`, then the second-largest value excluding the selected max slot, also using strict `<`; ties preserve the earliest slot visited. It compares that second maximum against the current girl's three threshold bytes (first threshold forced to 8) to derive a level. For Shiori when `D_8011F522 == 8`, it reverses the nonzero level as `3-level`. If a prior result is nonzero and the frame tick is odd it holds the prior value; otherwise it writes zero for no signal/level, or level 1, 2, or 3 into `D_8011F524`. This describes the explicit audio-level and threshold processing; what the level represents is unresolved.

## Open work / limits

- This proposal covers 10 function rows only. It does not rename the many scripted scene callbacks or globals in either overlay.
- The precise encoding of `D_8013C784`, `clubs[].b30`, the event-log scene id `0x15`, `girl_state_rank` values, scene flags 0x4C-0x4F, and the final confession outcome remains to be independently traced.
- `func_80133460`'s branch is described literally; it is not used to infer a girl-specific result name.
- The many ending scene function groups and text/script tables have no port-facing names in this proposal.
