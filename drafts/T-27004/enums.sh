#!/bin/bash
cd /Users/lila/Documents/Projects/tokimemo-wt/p2-core-apply
C=drafts/T-27004/const.sh
run() { e=$1; shift; echo "== $e"; $C $e --apply "$@" | head -3; git add -u; git add include/game_enums.h config/renames/core.txt; }
run Parameter parameter_index
run Weekday '*.weekday'
run Club '*.club'
run Place place_index
run HealthState PlayerFlagBits.health
run Vacation GameState.vacation
run DayKind MonthCalendar.day_kind
run CalendarMarker MonthCalendar.marker
run DayEventType '*.type'
run MainMode GameState.saved_mode GameState.overlay_target_mode 'main:main_state_set(arg0)'
run InputDevice GameState.input_device
run VoiceMode GameState.voice_mode
run MenuMode GameState.menu_mode
run IconMode GameState.icon_mode
run PadBits GameState.pad_held GameState.pad_prev GameState.pad_pressed GameState.pad_released
run MouseButtons GameState.mouse_buttons
run PlayerFlag GameState.player_flags
