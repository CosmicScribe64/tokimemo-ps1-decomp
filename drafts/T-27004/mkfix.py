#!/usr/bin/env python3
"""Writes the review-fix tables fix_symbols.tsv, fix_types.tsv, fix_fields.tsv (T-27004 review round 1)."""
R = "review fix: "
H = "HYPOTHESIS: "

# kind scope old new note
symbols = [
 # game mode machine: glossary rows 'main state' (game_mode) and 'stage, step' (mode_stage, mode_step)
 ("func","main_step_next","mode_step_next",R+"glossary stem mode_step (was main_step_next); step += 1"),
 ("func","main_stage_set","mode_stage_set",R+"glossary stem mode_stage (was main_stage_set)"),
 ("func","main_step_set","mode_step_set",R+"glossary stem mode_step (was main_step_set)"),
 ("func","main_stage_next","mode_stage_next",R+"glossary stem mode_stage (was main_stage_next)"),
 ("func","main_state_next","game_mode_next",R+"glossary row 'main state': stem game_mode (was main_state_next)"),
 ("func","main_state_request_overlay","game_mode_request_overlay",R+"stem game_mode (was main_state_request_overlay)"),
 ("func","main_state_dispatch","game_mode_dispatch",R+"stem game_mode (was main_state_dispatch); object 80042540 owns game_mode_"),
 ("func","main_state_set","game_mode_set",R+"the English-patch name fails the glossary row 'main state': game_mode_set (phase2 re-check list, M1)"),
 ("func","main_loop","boot_loop",R+"object 800420D0 owns boot_ (ADR main map); bare main_ is not a prefix"),
 ("func","game_reset_to_title","game_mode_reset_to_title",R+"bare game_ is not a prefix; it is the game-mode reset (default branch of the mode switches)"),
 # sound driver: snd_ is an SDK stem; audio_ becomes the sound-driver module prefix (ADR main map row added)
 ("func","snd_command","audio_command",R+"snd_ is an SDK stem (ADR); audio_ is the sound-driver prefix added to the ADR main map"),
 ("func","snd_command_pump","audio_command_pump",R+"snd_ is an SDK stem; audio_ prefix"),
 ("func","snd_bank_load_start","audio_bank_load_start",R+"snd_ is an SDK stem; audio_ prefix"),
 ("func","snd_bank_load_poll","audio_bank_load_poll",R+"snd_ is an SDK stem; audio_ prefix"),
 ("func","snd_bank_start","audio_bank_start",R+"snd_ is an SDK stem; audio_ prefix"),
 ("func","snd_bank_state_reset","audio_bank_state_reset",R+"snd_ is an SDK stem; audio_ prefix"),
 # bustup -> portrait (glossary row 'bustup')
 ("func","bustup_read_start","portrait_read_start",R+"glossary row 'bustup': stem portrait (only the O.BIN names keep bustup_)"),
 ("func","bustup_read_step","portrait_read_step",R+"stem portrait"),
 ("func","bustup_show_checked","portrait_show_checked",R+"stem portrait"),
 ("func","bustup_build","portrait_build",R+"stem portrait"),
 ("func","bustup_layout_set","portrait_layout_set",R+"stem portrait"),
 ("func","bustup_quad_offsets","portrait_quad_offsets",H+"stem portrait; sets quad offsets per girl id of the two portrait quads"),
 ("func","bustup_wink_update","portrait_wink_update",R+"stem portrait"),
 # dialogue -> talk (glossary row 'kaiwa'; phase2 re-check list, M9)
 ("func","dialogue_names_set","talk_names_set",R+"glossary row 'kaiwa': stem talk"),
 ("func","dialogue_clear","talk_clear",R+"stem talk"),
 ("func","dialogue_run","talk_run",R+"stem talk (the talk runner)"),
 ("func","dialogue_text_step","talk_text_step",R+"stem talk"),
 ("func","dialogue_render","talk_render",R+"stem talk (phase2 re-check list, M9)"),
 ("func","dialogue_speak_part","talk_speak_part",R+"stem talk (phase2 re-check list, M9)"),
 ("func","voice_play","talk_voice_play",R+"voice is not a prefix; the voice line belongs to the talk runner (sends sound command 0x300 for a dialogue line)"),
 # event trap word -> scene (glossary trap row 'event')
 ("func","event_still_show","scene_still_show",R+"trap word event: a scripted scene picture (callers KANGEI, SHOUGATU) takes scene"),
 ("func","event_anim_show","scene_anim_show",R+"trap word event: scene animation (DATE event scenes) takes scene"),
 # HUD and sprite module prefixes
 ("func","icon_bar_update","hud_icon_bar_update",H+"icon_ is not a prefix; the command icon bar is part of the HUD (hud_)"),
 ("func","mascot_update","hud_mascot_update",H+"mascot_ is not a prefix; drawn by the HUD file 800674B0 (hud_)"),
 ("func","screen_frame_draw","hud_screen_frame_draw",H+"screen_ is not a prefix; the screen frame is drawn by the HUD file (hud_)"),
 ("func","light_effect_draw","sprite_light_effect_draw",H+"light_ is not a prefix; layered effect sprites (sprite_)"),
 # drawing
 ("func","frame_render","draw_frame",R+"frame_ is not a prefix; file 80043510 owns draw_ (the per-frame draw pass)"),
 ("func","screen_shake_update","draw_shake_update",R+"screen_ is not a prefix; it moves the draw origin screen_center_x/y (draw_)"),
 ("func","debug_number_show","draw_debug_number",R+"debug_ is OPTION's debug_menu_ stem; this draws a number (draw_)"),
 # actors
 ("func","actors_draw_enable","actor_draw_enable",R+"actor_ added to the ADR main map (sprite module); plural prefix dropped"),
 ("func","actors_draw","actor_draw_all",R+"actor_ prefix; draws every visible actor of the pool"),
 # CD commands: cmd is not in the abbreviation table
 ("func","cd_cmd_reset","cd_command_reset",R+"cmd is not in the abbreviation table"),
 ("func","cd_cmd_push","cd_command_push",R+"cmd is not in the abbreviation table"),
 ("func","cd_cmd_pump","cd_command_pump",R+"cmd is not in the abbreviation table"),
 # memory card
 ("func","card_start","memory_card_start",R+"trap word card: stem memory_card"),
 ("func","card_stop","memory_card_stop",R+"trap word card: stem memory_card"),
 # scheduler and day events
 ("func","day_events_build_week","day_event_queue_build_week",R+"prefix day_event_ (ADR) instead of the plural day_events_"),
 ("func","day_events_clear","day_event_queue_clear",R+"prefix day_event_ instead of day_events_"),
 ("func","ending_start","schedule_ending_start",R+"ending_ is the ENDING overlay's prefix (One owner); this is step 1 of the day screen in the scheduler object 8006CB30 (schedule_)"),
 ("func","quarter_flags_rotate","schedule_quarter_flags_rotate",R+"quarter_ is not a prefix; object 8006CB30 owns schedule_"),
 ("func","daily_flags_clear","day_event_today_clear",R+"daily_ is not a prefix; clears the per-day queue bits and the day's event count (day_event_)"),
 # mode handlers: bare mode_ is a trap word; each handler takes the prefix of its screen's module
 ("func","mode_command_screen","schedule_command_screen_handler",R+"trap word mode: handler of the command screen that ends in schedule_init (mode 0x34)"),
 ("func","mode_day_screen","schedule_day_screen_handler",R+"trap word mode: handler of the day screen (mode 0x31) in object 8006CB30 (schedule_)"),
 ("func","mode_boot","boot_mode_handler",R+"trap word mode: handler of game mode 0x00 (boot_)"),
 ("func","mode_title","title_mode_handler",R+"trap word mode: handler of game mode 0x12, title menu object 80061710 (title_)"),
 ("func","mode_holiday","holiday_mode_handler",R+"trap word mode: handler of game mode 0x46, days-off object 80075320 (holiday_)"),
 ("func","mode_overlay_loader","ovl_loader_mode_handler",R+"trap word mode: handler of game mode 0x02, loader object 800789E0 (ovl_)"),
 # data
 ("data","card_block","memory_card_block",R+"trap word card: stem memory_card"),
 ("data","cd_cmd_ring","cd_command_ring",R+"cmd is not in the abbreviation table"),
 ("data","overlay_entry","ovl_entry",R+"glossary row 'overlay': stem ovl"),
 ("data","dialogue_tables","talk_tables",R+"stem talk"),
 ("data","dialogue_group","talk_group",R+"stem talk"),
 ("data","dialogue_line","talk_line",R+"stem talk"),
 ("data","dialogue_text_table","talk_text_table",R+"stem talk"),
 ("data","dialogue_command_table","talk_command_table",R+"stem talk"),
 ("data","dialogue_voice_table","talk_voice_table",R+"stem talk"),
 ("data","dialogue_work","talk_work",R+"stem talk"),
 ("data","name_buf_family","name_code_family",R+"glossary row 'name codes': stem name_code"),
 ("data","name_buf_given","name_code_given",R+"stem name_code"),
 ("data","name_buf_player_form","name_code_player_form",R+"stem name_code"),
 ("data","name_buf_girl_short","name_code_girl_short",R+"stem name_code"),
 ("data","parameter_name_table","param_name_table",R+"glossary row 'parameter': stem param"),
 ("data","command_deltas","school_day_command_deltas",R+"command_ is not a prefix; read only by the school-day object 800737A0 (school_day_)"),
 ("data","phone_numbers","girl_phone_digits",R+"phone_ is the TEL overlay's prefix (One owner); 13 rows of 7 digits, one per girl (girl_)"),
 ("data","last_screen_mode","dec_bg_last_screen_mode",R+"last_ is not a prefix; selects the dec_bg draw variant (read in the MDEC background object 80057390)"),
 ("data","mascot_choice","hud_mascot_choice",H+"mascot_ is not a prefix; read by hud_mascot_update"),
]

types = [
 ("DialogueWork","TalkWork",R+"stem talk (phase2 re-check list, M9)"),
 ("DialogueTables","TalkTables",R+"stem talk"),
 ("DialogueVoiceRef","TalkVoiceRef",R+"stem talk"),
 ("DialogueSelectors","TalkSelectors",R+"stem talk"),
 ("CardFileRequest","MemoryCardFileRequest",R+"trap word card: stem memory_card"),
 ("EventFlagBlock","SceneFlagBlock",R+"trap word event: the block holds the one-shot scene flags (field scene_flags)"),
 ("TacoSettingsBits","ShooterSettingsBits",R+"TACO's stem is shooter (disc name Taco is not a stem)"),
 ("SaveTacoSettings","SaveShooterSettings",R+"TACO's stem is shooter"),
 ("GirlFlagBitsSched","ScheduleGirlFlagBits",R+"sched is not in the abbreviation table"),
 ("PlayerFlagsHiBits","PlayerFlagsUpperBits",R+"hi is not in the abbreviation table"),
 # MainMode -> GameMode is not applicable: rename.py has no row kind for an enum tag or its MODE_ constants (plan: 0 definitions visible)
]

# kind scope(type) old new note
fields = [
 ("GameState","mode","game_mode",R+"trap word mode: stem game_mode; set by game_mode_set, dispatched by game_mode_dispatch"),
 ("GameState","stage","mode_stage",R+"glossary row 'stage, step': mode_stage"),
 ("GameState","step","mode_step",R+"glossary row 'stage, step': mode_step"),
 ("GameState","mode_timer","game_mode_timer",R+"stem game_mode"),
 ("GameState","stage_timer","mode_stage_timer",R+"stem mode_stage"),
 ("GameState","step_timer","mode_step_timer",R+"stem mode_step"),
 ("GameState","mode_scratch","game_mode_scratch",R+"stem game_mode"),
 ("GameState","stage_scratch","mode_stage_scratch",R+"stem mode_stage"),
 ("GameState","saved_mode","saved_game_mode",R+"stem game_mode"),
 ("GameState","saved_stage","saved_mode_stage",R+"stem mode_stage"),
 ("GameState","saved_step","saved_mode_step",R+"stem mode_step"),
 ("GameState","overlay_target_mode","ovl_target_game_mode",R+"glossary row 'overlay': stem ovl; stem game_mode"),
 ("GameState","overlay_tpage_table","ovl_tpage_table",R+"stem ovl"),
 ("GameState","overlay_frame_table","ovl_frame_table",R+"stem ovl"),
 ("GameState","overlay_sequence_table","ovl_sequence_table",R+"stem ovl"),
 ("GameState","overlay_palette_a","ovl_palette_a",R+"stem ovl"),
 ("GameState","overlay_image_buffer","ovl_image_buffer",R+"stem ovl"),
 ("GameState","overlay_palette_b","ovl_palette_b",R+"stem ovl"),
 ("GameState","card_busy","memory_card_busy",R+"trap word card: stem memory_card"),
 ("GameState","card_result","memory_card_result",R+"stem memory_card"),
 ("GameState","card_job","memory_card_job",R+"stem memory_card"),
 ("GameState","card_step","memory_card_step",R+"stem memory_card"),
 ("GameState","card_retry_count","memory_card_retry_count",R+"stem memory_card"),
 ("GameState","card_timer","memory_card_timer",R+"stem memory_card"),
 ("GameState","event_flags","scene_flags",R+"trap word event: one-shot scene bytes indexed by the scene number"),
 ("GameState","events_today","day_event_count_today",R+"trap word event: counts the day-event records run today"),
 ("GameState","daily_event_bits","day_event_done_bits",R+"trap word event: per-day done bits of the day-event queue reader"),
 ("GameState","girl_event_bits","girl_scene_bits",H+"trap word event: GEKO tests and sets bits for its scenes"),
 ("GameState","appt_place_0","appointment_place_0",R+"appt is not in the abbreviation table"),
 ("GameState","appt_place_1","appointment_place_1",R+"appt is not in the abbreviation table"),
 ("GameState","appt_place_2","appointment_place_2",R+"appt is not in the abbreviation table"),
 ("GameState","appt_place_3","appointment_place_3",R+"appt is not in the abbreviation table"),
 ("GameState","actors_draw_enabled","actor_draw_enabled",R+"follows actor_draw_enable"),
 ("GameState","player_flags_hi","player_flags_upper",R+"hi is not in the abbreviation table"),
 ("GameState","frame_hcount_limit","frame_scanline_limit",H+"hcount is not in the abbreviation table; scanline count the frame waits for"),
 ("GameState","tpage_req","tpage_slot_state",R+"req is not in the abbreviation table; 64 per-frame texture-page slot states"),
 ("GirlRecord","tokimeki_frac","tokimeki_fraction",R+"frac is not in the abbreviation table"),
 ("GirlRecord","affection_frac","affection_fraction",R+"frac is not in the abbreviation table"),
 ("GirlRecord","syoushin_frac","syoushin_fraction",R+"frac is not in the abbreviation table"),
 ("GirlRecord","event_counters","scene_counters",H+"trap word event: per-girl progress bytes indexed by the scene number"),
 ("ParamValue","frac","fraction",R+"frac is not in the abbreviation table"),
 ("Actor","x_frac","x_fraction",R+"frac is not in the abbreviation table"),
 ("Actor","y_frac","y_fraction",R+"frac is not in the abbreviation table"),
 ("Actor","dx_frac","dx_fraction",H+"frac is not in the abbreviation table; x step per frame"),
 ("Actor","dy_frac","dy_fraction",H+"frac is not in the abbreviation table; y step per frame"),
 ("TextEntry","attr","attribute",R+"attr is not in the abbreviation table"),
 ("ClubRecord","exp","practice_points",H+"exp is not in the abbreviation table; club practice points"),
]

with open("fix_symbols.tsv","w") as f:
    for k,o,n,note in symbols: f.write("\t".join((k,"main",o,n,note))+"\n")
with open("fix_types.tsv","w") as f:
    for o,n,note in types: f.write("\t".join(("type","main",o,n,note))+"\n")
with open("fix_fields.tsv","w") as f:
    for t,o,n,note in fields: f.write("\t".join(("field",t,o,n,note))+"\n")
print(len(symbols),len(types),len(fields))
