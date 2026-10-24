---
type: spec
updated: 2026-10-24
sources: ["src/ovl/TEL", "src/ovl/OPTION", "include/ovl/TEL.h", "include/ovl/OPTION.h", "include/main_api.h", "include/game.h", "wiki/families/option-family-results.md", "config/renames/proposals/o-tel-option.tsv"]
---

# TEL and OPTION overlays (port spec)

Phase 2 analysis for [[tickets/T-27034-p2-o-tel-option]]. These facts come from the C definitions and current shared headers. Proposed names live in `config/renames/proposals/o-tel-option.tsv`. No emulator evidence was collected in this pass. O.BIN names remain unchanged and need one-line definition comments when a naming batch is applied.

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

The source directory contains three TEL C files and three OPTION C files. This pass read the local call/profile dispatchers, phone date-list/place/chance logic, profile rendering and summary, options stage dispatch, menu-page family evidence, sound-test handlers and mode transitions. The TSV has 15 proposed function rows; it is a port-critical slice, not a census of all function definitions. Remaining work includes full TEL call substep outcomes, every profile special case, every OPTION page/setting, exact caller/loader edges, sound-test table names and category labels, O.BIN definition comments, data/global/type naming, and review of proposed names. No emulator run or build was performed.
