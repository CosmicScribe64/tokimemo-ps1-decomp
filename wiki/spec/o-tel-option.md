---
type: spec
updated: 2026-10-24
sources: ["src/ovl/TEL", "src/ovl/OPTION", "include/ovl/TEL.h", "include/ovl/OPTION.h", "include/main_api.h", "include/game.h", "wiki/families/option-family-results.md", "config/renames/proposals/o-tel-option-apply.tsv", "config/renames/proposals/o-tel-option-essential.tsv"]
---

# TEL and OPTION overlays (port spec)

Phase 2 analysis for [[tickets/T-27034-p2-o-tel-option]]. This page's proposal and essential inventories describe the earlier handoff; current identifiers and applied status follow the source tree and applied ledgers, and rows without ledger entries remain candidates or unresolved. No emulator evidence was collected in this pass. O.BIN function names remain unchanged; reviewed definition comments have since been applied.

## Current shared-core contract

Use the current applied types and fields in `include/main_api.h` and `include/game.h`: `GameState`, `GirlRecord`, `AppointmentEntry` / `AppointmentBits`, `game_state`, `game_mode_set`, `get_g_zyotai_s`, `get_g_zyotai_h`, `menu_set`, `menu_check`, `audio_command`, `appointment_find_today`, `k_sub_reset`, `talk_text_step`, and the shared `MODE_*` values. `GameState.u54C` stores eight appointment words; `AppointmentBits` exposes `place`, `girl`, and `active`. Avoid proposal-era pre-core member names.

TEL is the phone-call and girl-profile overlay (`phone_` prefix). OPTION owns the options pages, sound test and extras entry points (`options_`, `sound_test_`, `debug_menu_` prefixes). The sound test is an OPTION feature, not a general audio-driver module. UI/drawing, CD audio readiness, XA playback, dialogue and shared calendar display are supplied by main APIs.

## TEL state and phone call

`func_80132000` dispatches `mode_stage == 0` to the call flow and `mode_stage == 1` to profile flow; any other value resets to title. The call flow (`func_80132060`) switches on `mode_step` 0 through 4, each of which dispatches one step handler, then draws common phone HUD/calendar/text/cursor presentation. The broader call dispatcher (`func_80134BE0`) additionally routes the numbered mode steps for dial, response, profile inquiry, date selection and special paths. Mode-step and substep are reusable state bytes; their meaning is local to each branch.

When call step 0 rings, `func_80134DA0` initializes the target from `current_girl` (with value 0xB mapped to 0xC). When the call animation completes or the player cancels it, control is routed by these source predicates:

1. If an appointment exists today and its girl id matches `current_girl`, use mode step 0x10.
2. Else if year/month/day are exactly `0x62/2/0x16`, use mode step 0x10.
3. Else if the next-28-day date-slot builder returns zero, use mode step 0x10.
4. Else if the resolved phone target is 0xC, use mode step 0x40.
5. Otherwise advance to the normal reply path.

The first branch visibly checks a booked appointment for the called girl, but the exact narrative meaning of step 0x10 and the special date case are not confirmed here. Keep these predicates as code rules in a port without assigning story labels.

The reply handler (`func_80134FE4`) increments `girls[target].flags.b[3]`, then branches on rank. Rank 4 assigns affection 0x10 and moves to a separate path. For ranks below 2, girl 0, girl 7 with an existing bit in `flags.b[3]`, and girl 9 have special branches; the other below-2 ranks use another branch. Rank 2 has a separate branch, including a girl-10 exception before its normal path. Higher ranks route to another result. This is a direct source map; it does not establish conversational labels for those outcomes.

## Date invitation flow and rules

The phone's date invitation sequence first builds a list of available days, then a two-page place menu, then resolves chance and writes an appointment:

- **Candidate dates:** `func_8013655C` examines the next 28 days, rolls into the next month when needed, retains only `month_calendar[].day_kind` values 1 or 2, and drops dates for which `appointment_find_today(year, month, day)` finds an existing appointment. The returned count initializes the menu and selects cursor 0.
- **Place pages:** `func_8013D88C(page, year, month, day)` creates 11 menu entries per page. Page 0 corresponds to place ids 0–9 plus the next-page item; page 1 maps the first six rows to ids 10–15, then uses the weather/season callback to map rows to pool/sea/skating/ski ids, and enables shrine id 18 only under its flag/month/week test. The previous/next-page item is cursor index 10. Unlocked `progress_state` entries are broadly enabled; some later-page places are still constrained by month rules. Locked rows require specific packed `quarter_flags` bits and fixed year/month combinations. Their bit meanings are unknown, so the port should preserve explicit conditions until independently mapped.
- **Invitation chance:** the dialogue handler asks for a selected date and place. It computes `base = max(rank[current_girl] / 4, trivia / 4)`, adds `D_80141134[current_girl][place]` when nonzero (otherwise chance is zero), then subtracts `40 - looks` if looks is below 40. A random low byte succeeds when it is below the result. Regardless of success or failure, affection changes by `frame_count & 3`; syoushin decreases by 12 and clamps at zero. On success, it writes a packed `AppointmentEntry` with selected place/date/current girl and `active=1`, then calls the shared appointment updater.
- **Failure:** chance failure raises stress by 9 and guts by a random value 0–7. Although the source has comparisons against `0x66` and `0xB3`, in this `else` branch `rnd >= total`, so signed `total - rnd <= 0`; it is always below `0x66`. Therefore this branch always selects substep `0x40`; the later `0x60` and `0x80` branches are unreachable here. The result category is not semantically labeled.
- **Cancel/cleanup:** cross exits through the previous mode steps. `func_80139920` reads the girl in appointment slot 0 and clears bit `0x20` from active appointments for that girl before invoking the appointment updater. The bit's meaning is not asserted here.

These invitation rules are central to the Godot port: retain the 28-day horizon, calendar-kind filter, appointment collision check, place unlock gates, per-girl/place chance table, random comparison, stat changes and packed appointment writes. Current `AppointmentBits` names are applied; remaining bit meanings stay unknown.

## TEL profile and evaluation

The profile flow is stage 1. `func_80133D54` begins by clearing text and calling `func_8013CCD0`, then shows the appraisal summary and opens the girl selector. Circle accepts the special selector value 0xB or a girl whose `met` flag is set; un-met girls cannot be selected. Girl 10 and selector 0xB take special branches, while other entries proceed through Yoshio's voiced information dialogue and a subsequent mode step. `func_801345C4` loads the profile resources and calls `func_8013D03C` with the selected girl.

The summary (`func_8013CCD0`) invokes `func_80053418` for 16 rank/points outputs, but displays rows only for met girls 0–10. Points >= 500 yields no stars. Otherwise rank thresholds `<6`, `<11`, `<21`, `<41`, `<61`, and >=61 display 6, 5, 4, 3, 2, and 1 star respectively. The stored `girls[15].scene_counters[i]` value is stars+1. A separate actor appears if syoushin >= 0x3C; its blink interval is derived from the absolute difference from 0x3C, divided by 10, multiplied by -2 and offset by 8, with zero changed to one.

The detail page draws the selected target's phone digits, birthday, body measurements from a table selected by `day_count` bands (`<0x16D`, `<0x2DA`, later), zodiac calculated from month/day, blood type, club, and two note rows. Selector 0xB uses girl record 12 for birthday and blood type and a hard-coded special club label. The display does not itself establish the meaning of every note/table row. No score formula is inferred beyond the values returned by the shared rank/points routine.

## OPTION state, menu and sound test

`func_801335E8` is the options overlay's main stage dispatcher for stages 0–8. Each stage owns its own mode-step handlers; invalid stages reset to title. Stage 7 dispatches to `func_8013884C`, which routes mode-step ranges 0–3, 10–12, 20–23 and 30–40; sound-test selection/play/stop are specifically steps 21–23. Stage 8 dispatches to `func_80132A8C`. Shared per-frame work includes scroll animation, brightness presentation, cursor, title updates and gradient masking. Page functions build four halfword row arrays and call `menu_set`; records 96–113 of the shared display pool supply option-page labels/highlights. The family study `[[families/option-family-results]]` records the common zoom-in/zoom-out page template and its 0x44-byte actor rows. Keep those as screen presentation, separate from setting values.

The option menu sets settings through the existing `menu_mode`, `icon_mode`, speed-setting and audio APIs, and marks `options_dirty` on confirmed changes in relevant flows. The source-only pass did not trace every setting's persistence boundary or map every mode stage to its user-facing label. The main menu/overlay loader call-in must be traced by the main-module owner; this spec only records the local dispatch.

Sound test is routed through OPTION stage 7, mode steps 21–23. Track selection (`func_80137C1C`) enables rows when `system_data.flags[sel]` or `game_state.girl_cleared[sel]` is set, waits for CD audio state bit 2, requests an XA range from the per-row table, and sends audio command 0x300. Cross sends stop command 0x74 and sets mode_step to current+2, routing forward to the stop/return step. The advance handler (`func_80137E38`) waits for completion bit 4, starts each subsequent range while the selected table count permits it, and routes back one step when playback finishes. It checks cross to stop early; it has no circle check. The table's exact label/category mapping and all source-menu routes into stage 7 remain to be traced before a port should present final track names.

## Proposed module files

Later file/header work can split TEL into `telephone/call_flow.c`, `telephone/date_invitation.c`, `telephone/profile_summary.c`, and `telephone/profile_details.c`, with topic headers for call state, invitations and profile tables. OPTION can split into `options/settings_pages.c`, `options/sound_test.c`, and `options/extras.c`, with headers for settings, sound-test tables and extras. These are proposed content boundaries from the inspected source; exact symbol ownership and entry files need a complete overlay pass.

## Coverage and open questions

The source tree contains three TEL C objects and three OPTION C objects. This pass read the call/profile dispatchers, date-list/place/chance logic, profile rendering and summary, all options page families, sound-test handlers, voice-test browser and debug game-data editors. The apply table has 65 TEL and 117 OPTION function rows, 21 data rows, and 9 evidence-backed parameter rows. Both function censuses are body-reviewed and named. Those were handoff-time follow-up items. Current naming status follows applied ledgers and owner closure; sound-test category labels and unsupported asset meanings remain unresolved where the body and data do not establish them. No emulator run or build was performed.


## Phase-2 preparation and completeness audit

These inventories describe the original naming and literal-site handoff, not current application status. Use current source and applied ledgers to identify completed names; retain any proposal row without an applied ledger entry as a candidate. A proposal does not itself establish a rename.

Directory census on 2026-10-24 found 65 TEL definitions across `appointment_call_scenes.c` (33), `call_and_invitation_flow.c` (29), and `profile_and_place_availability.c` (3), and 117 OPTION definitions across its three files (72+26+19). The apply table has an exact matching set of 65 TEL and 117 OPTION function rows, 21 data rows, and 9 evidence-backed parameter rows. All function bodies are reviewed and named, including the three profile/place availability definitions. Overlay globals/types, further locals and exact essential literals remain incomplete. TEL has no hardware/UI exception rows; the scene dispatchers, dialogue handlers, relationship-state changes and phone event paths are accounted for.

Confirmed handoff corrections retained here: step semantics remain code predicates where story meaning is unsupported; current girl profile target `0xB` maps to record 12; practice grade/function rules are not carried into this unit. Option sound-test behavior remains a state/command flow, and track labels are unresolved.

## Additional body-backed state rules

The stage-to-setting map can now be stated from `func_801335E8` and each page handler: stage 0 builds/routes the options root; stage 1 chooses text speed (`k_speed_set` 0, 1, or 3); stage 2 chooses `icon_mode` 3, 4, or 5; stage 3 edits `confirm_button`, `click_mode`, and `double_click_interval`; stage 4 maps four choices to `menu_mode` 4, 5, 7, or 6; stage 5 chooses cursor speeds 1, 2, 3, 4, or 6 and clears `input_device`; stage 6 is the voice-mode page (its selection handler writes `voice_mode`); stage 7 contains the sound-test state machine; stage 8 runs the overlay exit path. The input-setting handlers set `options_dirty` bits 0x20, 0x40, 0x80, or 8 for the respective values. The interval picker uses fixed 0x20 or computes 0xFF minus a countdown and clamps the result to [0x10, 0x40].

TEL's shared helper `func_80139E3C` reads the girl from appointment slot 0 and applies the same per-girl `D_801444A0[index]` value as affection decrease and syoushin increase. Both the missed-date response and wrong-date/place response call it before clearing bit 0x20 from matching active appointment records. This identifies the stored delta and writes; the source does not establish what the delta table's values mean beyond those effects. `func_8013BDDC` finds the first met girl 0..10 with syoushin >=0x5B; `func_8013BE38` clears that girl's syoushin and flags b16/b12/b13, then applies per-girl tokimeki and affection decreases to met girls 0..10. Flags b12/b13 are also consumed by separate first-eligible-girl scans that exclude rank 4. Their producers and game meanings remain unresolved.

### Unreviewed TEL function definitions

These exact definitions are unresolved, not accepted hardware residue. Resolve each before the unit is complete.

| Source file | Unreviewed definitions | Why unresolved |
| --- | --- | --- |
| `src/ovl/TEL/call_and_invitation_flow.c` | — | All 29 definitions have body-backed rows. The added rows cover the incoming-call answer and appointment branch, selected-girl information menu and reports, Rei call threshold/dialogue paths, and answering-machine exit. Generic report-dialogue names describe the selected-girl and voice-table operations; individual narrative categories are not inferred. |
| `src/ovl/TEL/appointment_call_scenes.c` | — | All 33 definitions have body-backed rows, including the outgoing-call mode-stage dispatcher, Miharu and fireworks phone dialogue handlers, first-met-girl high-syoushin selection/penalty path, gift response, club-absence flag write, and culture-festival club notice. The fireworks caller's identity is not established by this body pass. The exact story meaning of several flags remains unresolved, but these bodies are not unreviewed. |
| `src/ovl/TEL/profile_and_place_availability.c` | — | All three definitions have body-backed rows: the profile appraisal summary, selected-girl detail renderer, and date-place availability builder. Their display/table meanings are stated only where supported by direct reads and consumers. |

### Unreviewed TEL data symbols

These tables describe earlier candidate proposals for invitation chance and relationship-delta tables, date-selection state, voice-test selectors, labels and audio-bank ranges. Current names follow applied ledgers; labels without a recorded rename still need definition/consumer classification before proposing names. Do not infer meaning from adjacency or string content.

| Source file | Unreviewed data labels | Why unresolved |
| --- | --- | --- |
| `include/ovl/TEL.h` | `D_80140550`, `D_80140554`, `D_80140558`, `D_8014055C`, `D_80140560`, `D_801406E0`, `D_80140710`, `D_80140770`, `D_80140AD8`, `D_80140B04`, `D_80140B5C`, `D_80140D18`, `D_80140D44`, `D_80140D9C`, `D_80140F0C`, `D_80140F38`, `D_80140F90`, `D_80141040`, `D_80141044`, `D_80141048`, `D_8014104C`, `D_80141064`, `D_8014107C`, `D_80141080`, `D_80141084`, `D_80141094`, `D_80141098`, `D_8014109C`, `D_801410AC`, `D_801410BC`, `D_801410C8`, `D_801410CC`, `D_801410D4`, `D_80141104`, `D_801413C8`, `D_801413F8`, `D_80141458`, `D_80141680`, `D_801416B0`, `D_80141710`, `D_801418D0`, `D_8014197C`, `D_801419AC`, `D_80141A0C`, `D_80141AB0`, `D_80141D80`, `D_80141DB4`, `D_80141E1C`, `D_80141FE0`, `D_80142254`, `D_80142288`, `D_801422F0`, `D_80142570`, `D_80142798`, `D_801427C8`, `D_80142828`, `D_801429E0`, `D_80142B40`, `D_80142B70`, `D_80142BD0`, `D_80142CB0`, `D_80142E70`, `D_80142EA0`, `D_80142F00`, `D_8014306C`, `D_801431E8`, `D_80143218`, `D_80143278`, `D_8014333C`, `D_801434D4`, `D_80143500`, `D_80143558`, `D_801437BC`, `D_80143958`, `D_80143984`, `D_801439DC`, `D_80143BA0`, `D_80143BCC`, `D_80143C24`, `D_80144194`, `D_80144198`, `D_8014419C`, `D_801441A0`, `D_801441A4`, `D_801441A8`, `D_801441D0`, `D_801441F8`, `D_801441FC`, `D_80144200`, `D_80144204`, `D_80144208`, `D_8014420C`, `D_80144210`, `D_80144240`, `D_80144270`, `D_80144274`, `D_80144278`, `D_8014427C`, `D_80144280`, `D_80144284`, `D_80144288`, `D_8014428C`, `D_80144290`, `D_80144294`, `D_80144298`, `D_801442E8`, `D_80144338`, `D_8014433C`, `D_80144340`, `D_80144344`, `D_80144348`, `D_8014434C`, `D_80144350`, `D_80144380`, `D_801443B0`, `D_801443B4`, `D_801443B8`, `D_801443BC`, `D_801443C0`, `D_801443C4`, `D_801443C8`, `D_801443F8`, `D_80144428`, `D_8014442C`, `D_80144430`, `D_80144434`, `D_80144438`, `D_8014443C`, `D_80144440`, `D_80144470`, `D_80144738`, `D_80144768`, `D_801447B8`, `D_80144AA4`, `D_80144AD4`, `D_80144B24`, `D_80144D6C`, `D_80144D9C`, `D_80144DEC`, `D_80144E3C`, `D_80144E44`, `D_80144E4C`, `D_80144EAC`, `D_80144FB8`, `D_80144FBC`, `D_80144FC0`, `D_80144FC4`, `D_80144FC8`, `D_80144FE8`, `D_801450C8`, `D_801450CC`, `D_801450D0`, `D_801450D4`, `D_801450D8`, `D_801450DC`, `D_801450E0`, `D_801450E4`, `D_8014511C`, `D_801452C8`, `D_801452F0`, `D_80145340`, `D_8014544C`, `D_80145450`, `D_80145454`, `D_80145458`, `D_8014545C`, `D_80145460`, `D_80145464`, `D_80145468`, `D_801454A0`, `D_80145C44`, `D_80145C78`, `D_80145CAC`, `D_80146008`, `D_80146038`, `D_80146090`, `D_801462F0`, `D_80146320`, `D_80146378`, `D_801465B0`, `D_801465E0`, `D_80146638`, `D_80146904`, `D_80146934`, `D_8014698C`, `D_80146BF4`, `D_80146C24`, `D_80146C7C`, `D_80146EE4`, `D_80146F14`, `D_80146F6C`, `D_80146FD0` | Definition/consumer role and port-facing semantics have not all been reconciled with the body; name from observed uses before classifying game logic or presentation residue. |
### Unreviewed OPTION function definitions

These exact definitions are unresolved, not accepted hardware residue. Resolve each before the unit is complete.

| Source file | Unreviewed definitions | Why unresolved |
| --- | --- | --- |
| `src/ovl/OPTION/settings_sound_test_and_extras.c` | — | All 72 definitions are body-reviewed and named. The rows cover setting-page setup, selection and exit dispatch, extras unlock gates, album/gallery routing and brightness/audio transitions. `func_801320B0` and `func_801320B8` are empty callbacks with no state effects. |
| `src/ovl/OPTION/voice_test_browser.c` | — | All 26 definitions are body-reviewed and named. Helpers initialize voice-test selector rows, update its 13 actor choices, fade in the view, draw debug status values and draw its two gradients; no hardware-only exception is claimed. |
| `src/ovl/OPTION/debug_game_data_editors.c` | — | All 19 definitions are body-reviewed and named. Debug pages edit calendar/player birthday, player parameters/flags, girl relationship values/flags, day-event/appointment fields and launch event game modes. These are game-state writes made through OPTION debug tools. |

### Unreviewed OPTION data symbols

These tables describe earlier candidate proposals for invitation chance and relationship-delta tables, date-selection state, voice-test selectors, labels and audio-bank ranges. Current names follow applied ledgers; labels without a recorded rename still need definition/consumer classification before proposing names. Do not infer meaning from adjacency or string content.

| Source file | Unreviewed data labels | Why unresolved |
| --- | --- | --- |
| `include/ovl/OPTION.h` | `D_8013D180`, `D_8013D2C4`, `D_8013D2FC`, `D_8013D300`, `D_8013D304`, `D_8013D308`, `D_8013D350`, `D_8013D360`, `D_8013D364`, `D_8013D372`, `D_8013D380`, `D_8013D38E`, `D_8013D39C`, `D_8013D3A8`, `D_8013D3B0`, `D_8013D3B4`, `D_8013D3B8`, `D_8013D3BC`, `D_8013D3CC`, `D_8013D3E0`, `D_8013D3F0`, `D_8013D3F4`, `D_8013E730`, `D_8013E740`, `D_8013E744`, `D_8013E784`, `D_8013E7C4`, `D_8013E804`, `D_8013E844` | Definition/consumer role and port-facing semantics have not all been reconciled with the body; name from observed uses before classifying game logic or presentation residue. |
