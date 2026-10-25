---
type: spec
updated: 2026-10-24
unit: EVENT
ticket: T-27023
---

# Overlay EVENT: step tables (appendix of the port spec)

Generated historical role-token listing of the 95 step tables of [[spec/o-event]]: for each table the entry at every step index. Tokens are role labels from the earlier proposal/twin mapping, not declaration-provider names; current declarations follow applied ledgers and the owner registry. The distinctions between EVENT development primitives and shipped twins remain intentional. A table is the whole state machine of its scene or place: the step byte indexes it, and each entry either ends with `next` or jumps. Facts only; no game text.

### shiori scene 1 (`spot_shiori_scene1_steps`, 39 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:next 6:still_show 7:script_load
8:wait_cd 9:next 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:dev_8004B5E4 15:wait_cd
16:show_checked 17:dev_80045BEC 18:place_init 19:bgm_start 20:talk 21:girl_in 22:talk 23:bggirl_fadeout
24:date_hide 25:bg_load 26:wait_cd 27:dec_init 28:change_bg 29:se_pair 30:dev_8004CFAC 31:vram_load
32:still_fade_in 33:talk 34:talk 35:still_fade_out 36:date_show 37:scene_exit 38:BAD_STATE
```

### shiori scene 2 (`spot_shiori_scene2_steps`, 22 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:script_load 4:wait_cd 5:bg_load 6:wait_cd 7:dec_init
8:change_bg 9:dev_8004B5E4 10:wait_cd 11:show_checked 12:dev_80045BEC 13:place_init 14:bgm_start 15:talk
16:girl_in 17:talk 18:bggirl_fadeout 19:talk 20:scene_exit 21:BAD_STATE
```

### shiori scene 3 (`spot_shiori_scene3_steps`, 45 steps)

```
0:assets_load 1:wait_cd 2:script_load 3:talk_reset 4:still_show_stats 5:reaction_rows_build 6:wait_cd 7:talk
8:girl_out 9:talk 10:bg_load 11:wait_cd 12:dec_init 13:bg_fadeout 14:date_hide 15:change_bg
16:still_in 17:talk 18:still_out 19:date_show 20:change_bg 21:bg_fadein 22:bustup_expression_variant_b 23:girl_in
24:talk 25:expression1 26:talk 27:expression0 28:talk 29:date_script_load 30:bggirl_fadeout 31:wait_sub
32:dev_800464E8 33:birthday_check 34:expression1 35:wait_cd 36:result_talk 37:expression_mood 38:skip_step_unless_flag 39:year_talk
40:mood_talk 41:girl_out 42:return 43:scene_close 44:BAD_STATE
```

### shiori scene 4 (`spot_shiori_scene4_steps`, 44 steps)

```
0:script_load 1:wait_cd 2:assets_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:girl_in 7:expression0
8:reaction_rows_build 9:talk 10:bggirl_fadeout 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:face1 19:talk 20:face0 21:talk 22:still_out 23:date_show
24:bg_load 25:wait_cd 26:dec_init 27:change_bg 28:date_script_load 29:bg_fadein 30:place_init 31:girl_in
32:birthday_check 33:expression1 34:wait_cd 35:result_talk 36:expression_mood 37:skip_step_unless_flag 38:year_talk 39:mood_talk
40:girl_out 41:return 42:scene_close 43:BAD_STATE
```

### shiori scene 5 (`spot_shiori_scene5_steps`, 58 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_out 6:wait_cd 7:bg_load
8:wait_cd 9:dec_init 10:move_place 11:label_set 12:dev_80046900 13:girl_in 14:talk 15:bggirl_fadeout
16:ride_movie_run 17:talk 18:date_hide 19:bg_load 20:wait_cd 21:dec_init 22:change_bg 23:still_in
24:talk 25:special_case 26:still_out 27:date_show 28:bg_load 29:wait_cd 30:dec_init 31:change_bg
32:bg_fadein 33:bustup_expression_variant_b 34:girl_in 35:talk 36:expression0 37:talk 38:girl_out 39:bg_load
40:wait_cd 41:dec_init 42:date_script_load 43:move_place 44:place_init 45:girl_in 46:wait_cd 47:birthday_check
48:expression1 49:result_talk 50:expression_mood 51:skip_step_unless_flag 52:year_talk 53:mood_talk 54:girl_out 55:return
56:scene_close 57:BAD_STATE
```

### shiori scene 6 (`spot_shiori_scene6_steps`, 45 steps)

```
0:script_load 1:wait_cd 2:assets_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:talk 7:bg_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_in 14:se_play 15:talk
16:still_out 17:date_show 18:change_bg 19:bg_fadein 20:girl_in 21:face 22:girl_out 23:bg_load
24:wait_cd 25:dec_init 26:date_script_load 27:wait_cd 28:move_place 29:place_init 30:read_bustup 31:wait_cd
32:show_checked 33:girl_in 34:birthday_check 35:expression1 36:result_talk 37:expression_mood 38:skip_step_unless_flag 39:year_talk
40:mood_talk 41:girl_out 42:return 43:scene_close 44:BAD_STATE
```

### shiori scene 8 (`spot_shiori_scene8_steps`, 47 steps)

```
0:special_case 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:portrait_read
8:wait_cd 9:show_checked 10:script_load 11:wait_cd 12:assets_load 13:wait_cd 14:talk 15:bgm_start
16:talk 17:dev_800464E8 18:place_init 19:talk 20:bggirl_fadeout 21:se_play 22:bg_load 23:wait_cd
24:dec_init 25:change_bg 26:date_hide 27:still_show 28:still_in 29:talk 30:face1 31:talk
32:face0 33:talk 34:still_out 35:date_show 36:bg_load 37:wait_cd 38:dec_init 39:change_bg
40:dev_800464E8 41:dev_80046900 42:talk 43:girl_out 44:return 45:scene_exit 46:BAD_STATE
```

### mio scene 1 (`spot_mio_scene1_steps`, 39 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:still_show 6:bg_load 7:wait_cd
8:dec_init 9:change_bg 10:script_load 11:dev_80045BEC 12:wait_cd 13:bgm_start 14:talk 15:bg_fadeout
16:date_hide 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:still_in 22:talk 23:still_out
24:date_show 25:change_bg 26:dev_8004B5E4 27:wait_cd 28:show_checked 29:bg_fadein 30:place_init 31:dev_8004A84C
32:girl_in 33:talk 34:expression0 35:talk 36:bggirl_fadeout 37:scene_exit 38:BAD_STATE
```

### mio scene 2 (`spot_mio_scene2_steps`, 67 steps)

```
0:talk_reset 1:set_state 2:girl_out 3:bg_load 4:wait_cd 5:dec_init 6:move_place 7:script_load
8:label_set 9:dev_80046900 10:dev_8004A89C 11:girl_in 12:wait_cd 13:talk_step_no_choice 14:select2 15:face
16:talk 17:bggirl_fadeout 18:step 19:script_load 20:wait_cd 21:wait_sub 22:next 23:still_show
24:talk 25:date_hide 26:bg_load 27:wait_cd 28:dec_init 29:change_bg 30:dev_8004CB64 31:still_in
32:talk 33:dev_8004CB34 34:dev_8004CB0C 35:talk 36:face1 37:talk 38:still_out 39:date_show
40:bg_load 41:wait_cd 42:dec_init 43:change_bg 44:dev_8004A89C 45:dev_800464E8 46:talk 47:girl_out
48:bg_load 49:wait_cd 50:dec_init 51:date_script_load 52:move_place 53:place_init 54:girl_in 55:wait_cd
56:birthday_check 57:face_from_row 58:result_talk 59:expression_mood 60:skip_step_unless_flag 61:year_talk 62:mood_talk 63:girl_out
64:return 65:scene_close 66:BAD_STATE
```

### mio scene 3 (`spot_mio_scene3_steps`, 39 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:face1 17:talk 18:still_out 19:date_show 20:bg_load 21:wait_cd 22:dec_init 23:change_bg
24:date_script_load 25:bg_fadein 26:place_init 27:girl_in 28:birthday_check 29:expression1 30:result_talk 31:expression_mood
32:skip_step_unless_flag 33:year_talk 34:mood_talk 35:girl_out 36:return 37:scene_close 38:BAD_STATE
```

### mio scene 4 (`spot_mio_scene4_steps`, 46 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:girl_out 5:bg_load 6:wait_cd 7:dec_init
8:move_place 9:script_load 10:wait_cd 11:talk 12:bg_fadeout 13:date_hide 14:bg_load 15:wait_cd
16:dec_init 17:change_bg 18:still_in 19:talk 20:still_out 21:date_show 22:change_bg 23:bg_fadein
24:girl_in 25:talk 26:girl_out 27:bg_load 28:wait_cd 29:dec_init 30:date_script_load 31:wait_cd
32:move_place 33:place_init 34:girl_in 35:birthday_check 36:expression1 37:result_talk 38:expression_mood 39:skip_step_unless_flag
40:year_talk 41:mood_talk 42:girl_out 43:return 44:scene_close 45:BAD_STATE
```

### mio scene 5 (`spot_mio_scene5_steps`, 53 steps)

```
0:talk_reset 1:assets_load 2:wait_cd 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:bggirl_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_in 14:talk 15:still_out
16:date_show 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:bg_fadein 22:talk 23:bg_fadeout
24:bg_load 25:wait_cd 26:dec_init 27:change_bg 28:bg_fadein 29:place_init 30:girl_in 31:talk
32:expression0 33:talk_step_no_choice 34:select2 35:branch 36:talk 37:bggirl_fadeout 38:date_script_load 39:wait_cd
40:bg_fadein 41:girl_in 42:birthday_check 43:expression1 44:result_talk 45:expression_mood 46:skip_step_unless_flag 47:year_talk
48:mood_talk 49:girl_out 50:return 51:scene_close 52:BAD_STATE
```

### yuina scene 1 (`spot_yuina_scene1_steps`, 42 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:dev_8004B5E4 4:wait_cd 5:show_checked 6:assets_load 7:wait_cd
8:still_show 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:script_load 14:dev_80045BEC 15:place_init
16:wait_cd 17:bgm_start 18:talk 19:bg_fadeout 20:date_hide 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:still_in 26:dev_8004CBC4 27:talk 28:still_out 29:date_show 30:change_bg 31:dev_80045BEC
32:dev_8004A8C4 33:girl_in 34:talk 35:expression1 36:talk 37:girl_out 38:talk 39:return
40:scene_exit 41:BAD_STATE
```

### yuina scene 2 (`spot_yuina_scene2_steps`, 41 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:date_show 18:change_bg 19:bg_fadein 20:dev_8004A89C 21:girl_in 22:talk 23:girl_out
24:bg_load 25:wait_cd 26:dec_init 27:date_script_load 28:move_place 29:place_init 30:girl_in 31:wait_cd
32:birthday_check 33:expression1 34:result_talk 35:expression_mood 36:mood_talk 37:girl_out 38:return 39:scene_close
40:BAD_STATE
```

### yuina scene 3 (`spot_yuina_scene3_steps`, 39 steps)

```
0:assets_load 1:wait_cd 2:script_load 3:wait_sub 4:wait_cd 5:talk_reset 6:still_show_stats 7:bg_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_in 14:talk 15:talk
16:se_play 17:set_state 18:talk 19:still_fade_out 20:date_show 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:bg_fadein 26:place_init 27:date_script_load 28:girl_in 29:wait_cd 30:birthday_check 31:expression1
32:result_talk 33:expression_mood 34:mood_talk 35:girl_out 36:return 37:scene_close 38:BAD_STATE
```

### yuina scene 4 (`spot_yuina_scene4_steps`, 39 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:bggirl_fadeout
8:talk 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:se_play
16:talk 17:still_out 18:date_show 19:bg_load 20:wait_cd 21:date_script_load 22:dec_init 23:change_bg
24:bg_fadein 25:place_init 26:girl_in 27:birthday_check 28:expression1 29:wait_cd 30:result_talk 31:expression_mood
32:skip_step_unless_flag 33:year_talk 34:mood_talk 35:girl_out 36:return 37:scene_close 38:BAD_STATE
```

### yuina scene 5 (`spot_yuina_scene5_steps`, 38 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:bggirl_fadeout
8:date_hide 9:still_in 10:talk 11:still_out 12:date_show 13:bg_load 14:wait_cd 15:dec_init
16:change_bg 17:bg_fadein 18:girl_in 19:talk 20:expression3 21:talk 22:girl_out 23:bg_load
24:wait_cd 25:dec_init 26:move_place 27:place_init 28:girl_in 29:birthday_check 30:expression1 31:result_talk
32:expression_mood 33:mood_talk 34:girl_out 35:return 36:scene_close 37:BAD_STATE
```

### ayako scene 1 (`spot_ayako_scene1_steps`, 32 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:still_show 6:bg_load 7:wait_cd
8:dec_init 9:change_bg 10:script_load 11:dev_80045BEC 12:place_init 13:wait_cd 14:bgm_start 15:talk
16:bg_fadeout 17:date_hide 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:still_in 23:talk
24:dev_8004CB64 25:talk 26:dev_8004CB34 27:talk 28:still_out 29:date_show 30:scene_exit 31:BAD_STATE
```

### ayako scene 2 (`spot_ayako_scene2_steps`, 49 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:expression0 9:talk 10:expression3 11:talk 12:expression2 13:talk 14:expression1 15:talk
16:bggirl_fadeout 17:date_hide 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:still_in 23:step
24:talk 25:still_out 26:date_show 27:change_bg 28:bg_fadein 29:girl_in 30:talk 31:girl_out
32:bg_load 33:wait_cd 34:dec_init 35:date_script_load 36:move_place 37:place_init 38:girl_in 39:wait_cd
40:birthday_check 41:expression1 42:result_talk 43:expression_mood 44:mood_talk 45:girl_out 46:return 47:scene_close
48:BAD_STATE
```

### ayako scene 3 (`spot_ayako_scene3_steps`, 48 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:dev_8004A89C 6:girl_in 7:wait_cd
8:talk 9:bggirl_fadeout 10:date_hide 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:still_in
16:talk 17:set_state 18:talk 19:still_out 20:date_show 21:change_bg 22:dev_8004A89C 23:dev_800464E8
24:talk 25:expression2 26:talk 27:girl_out 28:bg_load 29:wait_cd 30:dec_init 31:date_script_load
32:move_place 33:place_init 34:wait_cd 35:read_bustup 36:wait_cd 37:show_checked 38:girl_in 39:birthday_check
40:expression1 41:result_talk 42:expression_mood 43:mood_talk 44:girl_out 45:return 46:scene_close 47:BAD_STATE
```

### ayako scene 4 (`spot_ayako_scene4_steps`, 42 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:talk 18:still_in 19:talk 20:still_out 21:date_show 22:bg_load 23:wait_cd
24:dec_init 25:change_bg 26:date_script_load 27:bg_fadein 28:place_init 29:girl_in 30:birthday_check 31:expression1
32:wait_cd 33:result_talk 34:expression_mood 35:skip_step_unless_flag 36:year_talk 37:mood_talk 38:girl_out 39:return
40:scene_close 41:BAD_STATE
```

### ayako scene 5 (`spot_ayako_scene5_steps`, 45 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show 4:script_load 5:bg_fadein 6:wait_cd 7:talk
8:bg_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk_step_no_choice
16:select2 17:special_case 18:talk 19:still_out 20:date_show 21:change_bg 22:bg_fadein 23:girl_in
24:expression 25:talk 26:branch 27:expression0 28:talk 29:girl_out 30:bg_load 31:wait_cd
32:dec_init 33:move_place 34:place_init 35:girl_in 36:birthday_check 37:expression1 38:result_talk 39:expression_mood
40:mood_talk 41:girl_out 42:return 43:scene_close 44:BAD_STATE
```

### saki scene 1 (`spot_saki_scene1_steps`, 35 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:still_show 6:bg_load 7:wait_cd
8:dec_init 9:change_bg 10:dev_8004B5E4 11:wait_cd 12:show_checked 13:script_load 14:dev_80045BEC 15:wait_cd
16:place_init 17:bgm_start 18:talk 19:girl_in 20:talk 21:branch 22:talk 23:bggirl_fadeout
24:date_hide 25:bg_load 26:wait_cd 27:dec_init 28:change_bg 29:still_in 30:talk 31:still_out
32:date_show 33:scene_exit 34:BAD_STATE
```

### saki scene 2 (`spot_saki_scene2_steps`, 37 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:branch 17:talk 18:still_out 19:date_show 20:bg_load 21:wait_cd 22:dec_init 23:date_script_load
24:change_bg 25:bg_fadein 26:girl_in 27:place_init 28:birthday_check 29:expression1 30:result_talk 31:expression_mood
32:mood_talk 33:girl_out 34:return 35:scene_close 36:BAD_STATE
```

### saki scene 3 (`spot_saki_scene3_steps`, 55 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:girl_out 9:talk 10:bg_fadeout 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:dev_8004CB94 19:talk 20:face1 21:talk 22:still_out 23:date_show
24:change_bg 25:dev_8004A89C 26:dev_800464E8 27:talk 28:expression2 29:talk 30:expression3 31:talk
32:expression0 33:talk 34:girl_out 35:bg_load 36:wait_cd 37:dec_init 38:date_script_load 39:wait_cd
40:move_place 41:place_init 42:read_bustup 43:wait_cd 44:show_checked 45:girl_in 46:birthday_check 47:expression1
48:result_talk 49:expression_mood 50:mood_talk 51:girl_out 52:return 53:scene_close 54:BAD_STATE
```

### saki scene 4 (`spot_saki_scene4_steps`, 40 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:expression1 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:date_show 18:change_bg 19:bg_fadein 20:talk 21:girl_in 22:talk 23:girl_out
24:bg_load 25:wait_cd 26:dec_init 27:move_place 28:date_script_load 29:place_init 30:girl_in 31:birthday_check
32:expression1 33:result_talk 34:expression_mood 35:mood_talk 36:girl_out 37:return 38:scene_close 39:BAD_STATE
```

### saki scene 5 (`spot_saki_scene5_steps`, 51 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:expression1 7:talk
8:girl_out 9:talk 10:bg_fadeout 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:set_state 19:talk 20:set_state 21:face0 22:talk 23:still_out
24:date_show 25:talk 26:change_bg 27:bg_fadein 28:bustup_expression_variant_b 29:girl_in 30:talk 31:girl_out
32:bg_load 33:wait_cd 34:dec_init 35:date_script_load 36:move_place 37:place_init 38:girl_in 39:wait_cd
40:birthday_check 41:expression1 42:result_talk 43:expression_mood 44:skip_step_unless_flag 45:year_talk 46:mood_talk 47:girl_out
48:return 49:scene_close 50:BAD_STATE
```

### yukari scene 1 (`spot_yukari_scene1_steps`, 32 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:dev_8004B5E4
8:wait_cd 9:show_checked 10:assets_load 11:wait_cd 12:script_load 13:dev_80045BEC 14:place_init 15:wait_cd
16:bgm_start 17:talk 18:bg_fadeout 19:date_hide 20:still_show 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:still_in 26:talk 27:still_out 28:date_show 29:talk 30:scene_exit 31:BAD_STATE
```

### yukari scene 2 (`spot_yukari_scene2_steps`, 19 steps)

```
0:init 1:step 2:bg_load 3:wait_cd 4:dec_init 5:change_bg 6:dev_8004B5E4 7:wait_cd
8:show_checked 9:dev_80045BEC 10:place_init 11:talk 12:girl_in 13:talk 14:girl_out 15:talk
16:return 17:scene_exit 18:BAD_STATE
```

### yukari scene 3 (`spot_yukari_scene3_steps`, 38 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:girl_out 9:talk 10:bg_fadeout 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:still_out 19:date_show 20:bg_load 21:wait_cd 22:dec_init 23:change_bg
24:date_script_load 25:wait_cd 26:bg_fadein 27:place_init 28:girl_in 29:birthday_check 30:expression1 31:result_talk
32:expression_mood 33:mood_talk 34:girl_out 35:return 36:scene_close 37:BAD_STATE
```

### yukari scene 4 (`spot_yukari_scene4_steps`, 44 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:girl_out 9:talk 10:bg_fadeout 11:se_play 12:talk 13:date_hide 14:bg_load 15:wait_cd
16:dec_init 17:change_bg 18:still_in 19:talk 20:still_out 21:date_show 22:change_bg 23:bg_fadein
24:girl_in 25:talk 26:expression1 27:talk 28:girl_out 29:bg_load 30:wait_cd 31:dec_init
32:move_place 33:place_init 34:girl_in 35:birthday_check 36:expression1 37:result_talk 38:expression_mood 39:mood_talk
40:girl_out 41:return 42:scene_close 43:BAD_STATE
```

### yukari scene 5 (`spot_yukari_scene5_steps`, 40 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:expression3 9:talk 10:bggirl_fadeout 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:still_out 19:date_show 20:bg_load 21:wait_cd 22:dec_init 23:change_bg
24:date_script_load 25:bg_fadein 26:place_init 27:girl_in 28:wait_cd 29:birthday_check 30:expression1 31:result_talk
32:expression_mood 33:skip_step_unless_flag 34:year_talk 35:mood_talk 36:girl_out 37:return 38:scene_close 39:BAD_STATE
```

### yukari scene 6 (`spot_yukari_scene6_steps`, 56 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:expression1
8:talk 9:branch 10:talk 11:branch 12:expression2 13:talk 14:bggirl_fadeout 15:talk
16:date_hide 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:still_in 22:talk 23:still_out
24:date_show 25:bg_load 26:wait_cd 27:dec_init 28:change_bg 29:bg_fadein 30:dev_80046900 31:dev_8004A89C
32:girl_in 33:talk 34:expression1 35:talk 36:expression3 37:talk 38:girl_out 39:bg_load
40:wait_cd 41:dec_init 42:date_script_load 43:wait_cd 44:move_place 45:place_init 46:girl_in 47:birthday_check
48:expression1 49:result_talk 50:expression_mood 51:mood_talk 52:girl_out 53:return 54:scene_close 55:BAD_STATE
```

### nozomi scene 1 (`spot_nozomi_scene1_steps`, 29 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:assets_load
8:wait_cd 9:still_show 10:dev_8004B5E4 11:wait_cd 12:show_checked 13:dev_80045BEC 14:bgm_start 15:talk
16:bg_fadeout 17:date_hide 18:still_in 19:talk 20:still_out 21:date_show 22:bg_fadein 23:place_init
24:girl_in 25:talk 26:bggirl_fadeout 27:scene_exit 28:BAD_STATE
```

### nozomi scene 2 (`spot_nozomi_scene2_steps`, 50 steps)

```
0:talk_reset 1:still_show_stats 2:script_load 3:girl_in 4:wait_cd 5:assets_load 6:wait_cd 7:branch_on_mood
8:talk 9:se_play 10:bggirl_fadeout 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:date_hide
16:still_in 17:talk 18:still_out 19:date_show 20:bg_load 21:wait_cd 22:dec_init 23:change_bg
24:bg_fadein 25:place_init 26:dev_8004A89C 27:girl_in 28:talk 29:girl_out 30:talk 31:bg_fadeout
32:wait_sub 33:bg_fadein 34:place_init 35:step 36:girl_in 37:talk 38:date_script_load 39:wait_sub
40:wait_cd 41:birthday_check 42:expression1 43:result_talk 44:expression_mood 45:mood_talk 46:girl_out 47:return
48:scene_close 49:BAD_STATE
```

### nozomi scene 3 (`spot_nozomi_scene3_steps`, 55 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:wait_sub
8:talk 9:bggirl_fadeout 10:date_hide 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:se_play
16:still_in 17:talk 18:set_state 19:branch 20:set_state 21:se_play 22:talk 23:still_out
24:date_show 25:change_bg 26:bg_fadein 27:dev_8004A89C 28:girl_in 29:talk 30:expression2 31:talk
32:expression0 33:talk 34:girl_out 35:bg_load 36:wait_cd 37:dec_init 38:move_place 39:place_init
40:read_bustup 41:wait_cd 42:show_checked 43:date_script_load 44:girl_in 45:wait_cd 46:birthday_check 47:expression1
48:result_talk 49:expression_mood 50:mood_talk 51:girl_out 52:return 53:scene_close 54:BAD_STATE
```

### nozomi scene 4 (`spot_nozomi_scene4_steps`, 41 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:branch_on_mood
8:talk 9:bggirl_fadeout 10:date_hide 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:still_in
16:talk 17:face1 18:talk 19:still_out 20:date_show 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:bg_fadein 26:date_script_load 27:place_init 28:girl_in 29:wait_cd 30:birthday_check 31:expression1
32:result_talk 33:expression_mood 34:skip_step_unless_flag 35:year_talk 36:mood_talk 37:girl_out 38:return 39:scene_close
40:BAD_STATE
```

### nozomi scene 5 (`spot_nozomi_scene5_steps`, 40 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:branch_on_mood
8:talk 9:bggirl_fadeout 10:date_hide 11:se_play 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:still_out 19:date_show 20:change_bg 21:bg_fadein 22:girl_in 23:talk
24:girl_out 25:bg_load 26:wait_cd 27:dec_init 28:move_place 29:place_init 30:girl_in 31:birthday_check
32:expression1 33:result_talk 34:expression_mood 35:mood_talk 36:girl_out 37:return 38:scene_close 39:BAD_STATE
```

### mira scene 1 (`spot_mira_scene1_steps`, 38 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:dev_8004B5E4
8:wait_cd 9:show_checked 10:script_load 11:dev_80045BEC 12:wait_cd 13:assets_load 14:wait_cd 15:still_show
16:place_init 17:bgm_start 18:talk 19:bg_fadeout 20:date_hide 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:still_in 26:talk 27:label_set 28:talk 29:label_set 30:talk 31:names_set
32:talk 33:still_out 34:talk 35:date_show 36:scene_exit 37:BAD_STATE
```

### mira scene 2 (`spot_mira_scene2_steps`, 40 steps)

```
0:talk_reset 1:assets_load 2:wait_cd 3:still_show 4:script_load 5:wait_cd 6:talk 7:select3
8:talk 9:bggirl_fadeout 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:date_hide 15:palette_load
16:se_play 17:still_in 18:talk 19:still_out 20:date_show 21:bg_load 22:wait_cd 23:dec_init
24:change_bg 25:date_script_load 26:bg_fadein 27:place_init 28:girl_in 29:wait_cd 30:birthday_check 31:outcome_apply
32:face_from_row 33:result_talk 34:expression_mood 35:mood_talk 36:girl_out 37:return 38:scene_close 39:BAD_STATE
```

### mira scene 3 (`spot_mira_scene3_steps`, 38 steps)

```
0:talk_reset 1:assets_load 2:wait_cd 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:date_show 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:read_bustup 23:wait_cd
24:show_checked 25:date_script_load 26:bg_fadein 27:place_init 28:girl_in 29:wait_cd 30:expression1 31:result_talk
32:expression_mood 33:mood_talk 34:girl_out 35:return 36:scene_close 37:BAD_STATE
```

### mira scene 4 (`spot_mira_scene4_steps`, 51 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:girl_out 5:amusement_ride_choose 6:wait_cd 7:dec_init
8:move_place 9:place_init 10:girl_in 11:talk 12:expression3 13:talk 14:bggirl_fadeout 15:date_hide
16:talk 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:still_in 22:talk 23:still_out
24:date_show 25:change_bg 26:bg_fadein 27:girl_in 28:talk 29:expression0 30:talk 31:girl_out
32:bg_load 33:wait_cd 34:dec_init 35:move_place 36:date_script_load 37:dev_80046900 38:girl_in 39:wait_cd
40:birthday_check 41:expression1 42:result_talk 43:expression_mood 44:skip_step_unless_flag 45:year_talk 46:mood_talk 47:girl_out
48:return 49:scene_close 50:BAD_STATE
```

### mira scene 5 (`spot_mira_scene5_steps`, 42 steps)

```
0:assets_load 1:wait_cd 2:script_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:talk 7:bggirl_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_in 14:talk 15:dev_8004CE48
16:se_play 17:still_out 18:dev_8004CE70 19:date_show 20:change_bg 21:bg_fadein 22:girl_in 23:talk
24:girl_out 25:bg_load 26:wait_cd 27:dec_init 28:move_place 29:date_script_load 30:place_init 31:girl_in
32:wait_cd 33:birthday_check 34:expression1 35:result_talk 36:expression_mood 37:mood_talk 38:girl_out 39:return
40:scene_close 41:BAD_STATE
```

### mira scene 6 (`spot_mira_scene6_steps`, 15 steps)

```
0:talk_reset 1:stat_update 2:script_load 3:wait_cd 4:talk 5:girl_in 6:dev_8004A89C 7:talk
8:expression2 9:talk 10:girl_out 11:talk 12:return 13:scene_close 14:BAD_STATE
```

### mira scene 8 (`spot_mira_scene8_steps`, 21 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:dev_80045BEC
8:place_init 9:dev_8004B5E4 10:wait_cd 11:show_checked 12:script_load 13:wait_cd 14:girl_in 15:talk
16:girl_out 17:talk 18:return 19:scene_exit 20:BAD_STATE
```

### yuko scene 1 (`spot_yuko_scene1_steps`, 52 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:script_load
8:wait_cd 9:assets_load 10:wait_cd 11:still_show 12:dev_8004B5E4 13:wait_cd 14:show_checked 15:dev_80045BEC
16:place_init 17:talk 18:bg_load 19:wait_cd 20:dec_init 21:move_place 22:dev_80046900 23:bgm_start
24:talk 25:bg_fadeout 26:date_hide 27:bg_load 28:wait_cd 29:dec_init 30:change_bg 31:still_in
32:talk 33:still_out 34:talk 35:change_bg 36:bg_fadein 37:talk 38:bg_fadeout 39:names_set
40:date_show 41:bg_load 42:wait_cd 43:dec_init 44:change_bg 45:bg_fadein 46:girl_in 47:talk
48:girl_out 49:return 50:scene_exit 51:BAD_STATE
```

### yuko scene 2 (`spot_yuko_scene2_steps`, 41 steps)

```
0:script_load 1:wait_cd 2:assets_load 3:wait_cd 4:assets_load 5:wait_cd 6:talk_reset 7:still_show_stats
8:talk 9:expression1 10:talk 11:girl_out 12:talk 13:bg_fadeout 14:date_hide 15:bg_load
16:wait_cd 17:dec_init 18:change_bg 19:still_in 20:talk 21:still_out 22:date_show 23:bg_load
24:wait_cd 25:dec_init 26:change_bg 27:date_script_load 28:bg_fadein 29:place_init 30:girl_in 31:wait_cd
32:birthday_check 33:expression1 34:result_talk 35:expression_mood 36:mood_talk 37:girl_out 38:return 39:scene_close
40:BAD_STATE
```

### yuko scene 3 (`spot_yuko_scene3_steps`, 35 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:bggirl_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_fade_in 14:talk 15:still_fade_out
16:date_show 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:date_script_load 22:bg_fadein 23:place_init
24:girl_in 25:wait_cd 26:birthday_check 27:expression1 28:result_talk 29:expression_mood 30:mood_talk 31:girl_out
32:return 33:scene_close 34:BAD_STATE
```

### yuko scene 4 (`spot_yuko_scene4_steps`, 47 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:bg_fadeout 6:date_hide 7:wait_cd
8:bg_load 9:wait_cd 10:dec_init 11:change_bg 12:still_in 13:talk 14:still_out 15:talk
16:date_show 17:bg_load 18:wait_cd 19:dec_init 20:change_bg 21:bg_fadein 22:dev_8004A89C 23:girl_in
24:talk 25:expression1 26:talk 27:girl_out 28:bg_load 29:wait_cd 30:dec_init 31:move_place
32:date_script_load 33:place_init 34:girl_in 35:wait_cd 36:birthday_check 37:expression1 38:result_talk 39:expression_mood
40:skip_step_unless_flag 41:year_talk 42:mood_talk 43:girl_out 44:return 45:scene_close 46:BAD_STATE
```

### yuko scene 5 (`spot_yuko_scene5_steps`, 42 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:girl_in 6:wait_cd 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:se_play 18:date_show 19:change_bg 20:bg_fadein 21:girl_in 22:talk 23:girl_out
24:talk 25:bg_load 26:wait_cd 27:dec_init 28:move_place 29:date_script_load 30:place_init 31:girl_in
32:wait_cd 33:birthday_check 34:expression1 35:result_talk 36:expression_mood 37:mood_talk 38:girl_out 39:return
40:scene_close 41:BAD_STATE
```

### yuko scene 6 (`spot_yuko_scene6_steps`, 42 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:talk 7:girl_out
8:bg_fadeout 9:bgm_start 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:date_hide 15:still_in
16:talk 17:face1 18:special_case 19:still_out 20:talk 21:date_show 22:change_bg 23:bg_fadein
24:place_init 25:bustup_expression_variant_b 26:girl_in 27:talk 28:expression1 29:talk 30:date_script_load 31:wait_sub
32:wait_cd 33:birthday_check 34:expression1 35:result_talk 36:expression_mood 37:mood_talk 38:girl_out 39:return
40:scene_close 41:BAD_STATE
```

### megumi scene 1 (`spot_megumi_scene1_steps`, 45 steps)

```
0:init 1:assets_load 2:wait_cd 3:sound_bank_load 4:wait_bank 5:script_load 6:wait_cd 7:bg_load
8:wait_cd 9:dec_init 10:change_bg 11:still_show 12:dev_8004B5E4 13:wait_cd 14:show_checked 15:dev_80045BEC
16:place_init 17:bgm_start 18:talk 19:dev_8004A89C 20:girl_in 21:talk 22:branch 23:expression0
24:talk 25:branch 26:bggirl_fadeout 27:bg_load 28:wait_cd 29:dec_init 30:change_bg 31:date_hide
32:dev_8004CFAC 33:step 34:se_pair 35:still_fade_in 36:talk 37:still_fade_out 38:date_show 39:branch
40:girl_out 41:return 42:talk 43:scene_exit 44:BAD_STATE
```

### megumi scene 2 (`spot_megumi_scene2_steps`, 20 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:dev_8004B5E4
8:wait_cd 9:show_checked 10:bgm_start 11:dev_80045BEC 12:place_init 13:girl_in 14:talk 15:girl_out
16:talk 17:return 18:scene_exit 19:BAD_STATE
```

### megumi scene 3 (`spot_megumi_scene3_steps`, 47 steps)

```
0:script_load 1:wait_cd 2:assets_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:talk 7:bggirl_fadeout
8:date_hide 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:still_in 14:talk 15:still_out
16:date_show 17:change_bg 18:bg_fadein 19:bustup_expression_variant_b 20:girl_in 21:talk 22:talk 23:expression3
24:talk 25:expression4 26:talk 27:expression0 28:talk 29:girl_out 30:bg_load 31:wait_cd
32:dec_init 33:move_place 34:place_init 35:date_script_load 36:girl_in 37:wait_cd 38:birthday_check 39:expression1
40:result_talk 41:expression_mood 42:mood_talk 43:girl_out 44:return 45:scene_close 46:BAD_STATE
```

### megumi scene 4 (`spot_megumi_scene4_steps`, 63 steps)

```
0:script_load 1:wait_cd 2:assets_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:talk 7:se_play
8:talk 9:girl_in 10:talk 11:se_play 12:talk 13:bggirl_fadeout 14:date_hide 15:bg_load
16:wait_cd 17:dec_init 18:change_bg 19:still_in 20:talk 21:se_play 22:talk 23:se_play
24:talk 25:se_play 26:talk 27:se_play 28:talk 29:face1 30:talk 31:face0
32:talk 33:se_play 34:talk 35:se_play 36:talk 37:face1 38:talk 39:se_play
40:talk 41:face0 42:talk 43:still_out 44:date_show 45:change_bg 46:bg_fadein 47:date_script_load
48:place_init 49:girl_in 50:wait_cd 51:birthday_check 52:expression1 53:result_talk 54:expression_mood 55:skip_step_unless_flag
56:year_talk 57:mood_talk 58:girl_out 59:return 60:scene_close 61:dev_8004BB08 62:BAD_STATE
```

### megumi scene 5 (`spot_megumi_scene5_steps`, 36 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:talk 5:bg_fadeout 6:date_hide 7:bg_load
8:wait_cd 9:dec_init 10:change_bg 11:still_in 12:talk 13:still_out 14:date_show 15:change_bg
16:bg_fadein 17:dev_8004A89C 18:girl_in 19:talk 20:girl_out 21:bg_load 22:wait_cd 23:dec_init
24:move_place 25:place_init 26:girl_in 27:birthday_check 28:expression1 29:result_talk 30:expression_mood 31:mood_talk
32:girl_out 33:return 34:scene_close 35:BAD_STATE
```

### megumi scene 6 (`spot_megumi_scene6_steps`, 36 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:girl_in 5:script_load 6:wait_cd 7:talk
8:expression4 9:talk 10:expression0 11:talk 12:bggirl_fadeout 13:date_hide 14:still_in 15:talk
16:still_out 17:date_show 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:date_script_load 23:bg_fadein
24:place_init 25:girl_in 26:wait_cd 27:birthday_check 28:expression1 29:result_talk 30:expression_mood 31:mood_talk
32:girl_out 33:return 34:scene_close 35:BAD_STATE
```

### yumi scene 1 (`spot_yumi_scene1_steps`, 47 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:assets_load
8:wait_cd 9:still_show 10:dev_8004B5E4 11:wait_cd 12:show_checked 13:dev_80045BEC 14:place_init 15:bgm_start
16:talk 17:girl_in 18:talk 19:branch 20:talk 21:bggirl_fadeout 22:date_hide 23:bg_load
24:wait_cd 25:dec_init 26:change_bg 27:still_in 28:talk 29:face1 30:talk 31:face0
32:talk 33:still_out 34:date_show 35:branch 36:bg_load 37:wait_cd 38:dec_init 39:change_bg
40:bg_fadein 41:girl_in 42:talk 43:girl_out 44:return 45:scene_exit 46:BAD_STATE
```

### yumi scene 2 (`spot_yumi_scene2_steps`, 21 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:dev_8004B5E4
8:wait_cd 9:show_checked 10:bgm_start 11:dev_80045BEC 12:place_init 13:girl_in 14:branch_on_mood 15:talk
16:girl_out 17:talk 18:return 19:scene_exit 20:BAD_STATE
```

### yumi scene 3 (`spot_yumi_scene3_steps`, 41 steps)

```
0:assets_load 1:wait_cd 2:script_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:girl_in 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:date_show 18:change_bg 19:bg_fadein 20:girl_in 21:expression1 22:talk 23:girl_out
24:bg_load 25:wait_cd 26:dec_init 27:move_place 28:place_init 29:date_script_load 30:girl_in 31:wait_cd
32:birthday_check 33:expression1 34:result_talk 35:expression_mood 36:mood_talk 37:girl_out 38:return 39:scene_close
40:BAD_STATE
```

### yumi scene 4 (`spot_yumi_scene4_steps`, 44 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:talk 5:expression3 6:talk 7:girl_out
8:bg_load 9:wait_cd 10:dec_init 11:move_place 12:dev_80046900 13:dev_8004A84C 14:girl_in 15:talk
16:bggirl_fadeout 17:date_hide 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:still_in 23:talk
24:still_out 25:date_show 26:bg_load 27:wait_cd 28:dec_init 29:change_bg 30:bg_fadein 31:date_script_load
32:place_init 33:girl_in 34:wait_cd 35:birthday_check 36:expression1 37:result_talk 38:expression_mood 39:mood_talk
40:girl_out 41:return 42:scene_close 43:BAD_STATE
```

### yumi scene 5 (`spot_yumi_scene5_steps`, 44 steps)

```
0:assets_load 1:wait_cd 2:talk_reset 3:still_show_stats 4:script_load 5:wait_cd 6:expression1 7:talk
8:bggirl_fadeout 9:date_hide 10:bg_load 11:wait_cd 12:dec_init 13:change_bg 14:still_in 15:talk
16:still_out 17:date_show 18:change_bg 19:bg_fadein 20:talk 21:bustup_expression_variant_b 22:girl_in 23:talk
24:girl_out 25:bg_load 26:wait_cd 27:dec_init 28:move_place 29:date_script_load 30:place_init 31:girl_in
32:wait_cd 33:birthday_check 34:expression1 35:result_talk 36:expression_mood 37:skip_step_unless_flag 38:year_talk 39:mood_talk
40:girl_out 41:return 42:scene_close 43:BAD_STATE
```

### yumi scene 6 (`spot_yumi_scene6_steps`, 52 steps)

```
0:assets_load 1:wait_cd 2:script_load 3:wait_cd 4:talk_reset 5:still_show_stats 6:girl_in 7:talk
8:se_play 9:bggirl_fadeout 10:talk 11:date_hide 12:bg_load 13:wait_cd 14:dec_init 15:change_bg
16:still_in 17:talk 18:wait_sub 19:se_play 20:face1 21:talk 22:still_out 23:date_show
24:bg_load 25:wait_cd 26:dec_init 27:change_bg 28:bg_fadein 29:dev_8004A89C 30:girl_in 31:talk
32:expression0 33:talk 34:girl_out 35:bg_load 36:wait_cd 37:dec_init 38:move_place 39:place_init
40:date_script_load 41:girl_in 42:wait_cd 43:birthday_check 44:expression1 45:result_talk 46:expression_mood 47:mood_talk
48:girl_out 49:return 50:scene_close 51:BAD_STATE
```

### yumi scene 7 (`spot_yumi_scene7_steps`, 55 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:script_load 6:wait_cd 7:still_show
8:talk 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:read_bustup 14:wait_cd 15:show_checked
16:dev_80045BEC 17:place_init 18:girl_in 19:talk 20:girl_out 21:bg_load_labeled 22:wait_cd 23:dec_init
24:move_place 25:dev_80046900 26:talk 27:branch 28:talk 29:date_hide 30:dev_8004CE48 31:se_play
32:dev_8004CE70 33:bg_load 34:wait_cd 35:dec_init 36:change_bg 37:step 38:special_case 39:dev_8004CE70
40:dev_8004CE70 41:talk 42:still_out 43:set_state 44:bg_load 45:wait_cd 46:dec_init 47:change_bg
48:date_show 49:dev_800464E8 50:talk 51:girl_out 52:return 53:scene_exit 54:BAD_STATE
```

### miharu scene 1 (`spot_miharu_scene1_steps`, 22 steps)

```
0:init 1:bg_load 2:wait_cd 3:dec_init 4:change_bg 5:dev_8004B5E4 6:wait_cd 7:show_checked
8:dev_80045BEC 9:se_play 10:dev_8004CE48 11:dev_8004CE70 12:bgm_start 13:talk 14:girl_in 15:branch
16:talk 17:girl_out 18:talk 19:return 20:scene_exit 21:BAD_STATE
```

### miharu scene 2 (`spot_miharu_scene2_steps`, 25 steps)

```
0:init 1:init 2:bg_load 3:wait_cd 4:dec_init 5:change_bg 6:script_load 7:dev_80045BEC
8:wait_cd 9:assets_read 10:wait_cd 11:still_show 12:talk 13:bg_fadeout 14:bg_load 15:wait_cd
16:dec_init 17:change_bg 18:still_in 19:talk 20:still_out 21:branch 22:talk 23:scene_exit
24:BAD_STATE
```

### miharu scene 3 (`spot_miharu_scene3_steps`, 14 steps)

```
0:set_state 1:portrait_read 2:wait_cd 3:show_checked 4:girl_in 5:talk 6:girl_out 7:talk
8:read_bustup 9:wait_cd 10:show_checked 11:dev_8004BB08 12:scene_exit 13:BAD_STATE
```

### miharu scene 4 (`spot_miharu_scene4_steps`, 56 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:assets_load 4:wait_cd 5:script_load 6:wait_cd 7:still_show
8:talk 9:bg_load 10:wait_cd 11:dec_init 12:change_bg 13:bgm_start 14:dev_80045BEC 15:place_init
16:talk 17:dev_8004CE48 18:dev_8004CE70 19:talk 20:portrait_read 21:wait_cd 22:show_checked 23:girl_in
24:talk 25:girl_out 26:bg_load 27:wait_cd 28:dec_init 29:move_place 30:girl_in 31:talk
32:special_case 33:girl_out 34:wait_sub 35:step 36:talk 37:bg_fadeout 38:bg_load 39:wait_cd
40:dec_init 41:change_bg 42:still_in 43:talk 44:still_out 45:bg_load 46:wait_cd 47:dec_init
48:change_bg 49:dev_800464E8 50:talk 51:girl_out 52:talk 53:return 54:scene_exit 55:BAD_STATE
```

### miharu scene 5 (`spot_miharu_scene5_steps`, 14 steps)

```
0:set_state 1:portrait_read 2:wait_cd 3:show_checked 4:girl_in 5:talk 6:girl_out 7:talk
8:read_bustup 9:wait_cd 10:show_checked 11:dev_8004BB08 12:scene_exit 13:BAD_STATE
```

### rei scene 1 (`spot_rei_scene1_steps`, 51 steps)

```
0:init 1:bg_load 2:wait_cd 3:dec_init 4:change_bg 5:dev_8004BBCC 6:dev_8004B5E4 7:wait_cd
8:show_checked 9:dev_8004BBF4 10:script_load 11:dev_80045BEC 12:wait_cd 13:talk 14:girl_in 15:talk_step_no_choice
16:select2 17:branch 18:talk 19:girl_out 20:branch 21:talk 22:bg_fadeout 23:date_hide
24:dev_8004B5E4 25:wait_cd 26:show_checked 27:bg_load 28:wait_cd 29:dec_init 30:change_bg 31:bg_fadein
32:step 33:girl_in 34:talk 35:girl_out 36:talk 37:return 38:date_show 39:scene_exit
40:dev_8004B5E4 41:wait_cd 42:show_checked 43:talk 44:girl_in 45:talk 46:girl_out 47:talk
48:return 49:scene_exit 50:BAD_STATE
```

### rei scene 2 (`spot_rei_scene2_steps`, 37 steps)

```
0:init 1:sound_bank_load 2:wait_bank 3:bg_load 4:wait_cd 5:dec_init 6:change_bg 7:assets_load
8:wait_cd 9:still_show 10:dev_8004B5E4 11:wait_cd 12:show_checked 13:script_load 14:dev_80045BEC 15:wait_cd
16:talk 17:dev_8004CE48 18:se_play 19:talk 20:dev_80045BC0 21:bgm_start 22:talk 23:date_hide
24:still_in 25:talk 26:still_out 27:date_show 28:bg_fadein 29:dev_8004A8C4 30:girl_in 31:talk
32:girl_out 33:talk 34:return 35:scene_exit 36:BAD_STATE
```

### rei scene 3 (`spot_rei_scene3_steps`, 41 steps)

```
0:init 1:set_state 2:bg_load 3:wait_cd 4:dec_init 5:change_bg 6:dev_8004BBCC 7:dev_8004B5E4
8:wait_cd 9:show_checked 10:dev_8004BBF4 11:script_load 12:dev_80045BEC 13:girl_in 14:wait_cd 15:talk
16:dev_800463C0 17:talk 18:dev_8004B5E4 19:wait_cd 20:show_checked 21:girl_in 22:talk 23:bggirl_fadeout
24:wait_sub 25:dev_800464E8 26:talk 27:girl_out 28:bg_fadeout 29:dev_8004BBCC 30:dev_8004B5E4 31:wait_cd
32:show_checked 33:talk 34:bg_fadein 35:girl_in 36:talk 37:girl_out 38:return 39:scene_exit
40:BAD_STATE
```

### yoshio scene 1 (`spot_yoshio_scene1_steps`, 42 steps)

```
0:init 1:assets_load 2:wait_cd 3:wait_sub 4:bg_load 5:wait_cd 6:dec_init 7:change_bg
8:dev_8004B5E4 9:wait_cd 10:show_checked 11:script_load 12:dev_80045BEC 13:place_init 14:girl_in 15:wait_cd
16:talk 17:bggirl_fadeout 18:bg_load 19:wait_cd 20:dec_init 21:change_bg 22:date_hide 23:names_set
24:dev_8004B5E4 25:wait_cd 26:show_checked 27:set_state 28:still_show 29:still_in 30:talk 31:dev_8004A824
32:dev_80046194 33:set_state 34:step 35:set_state 36:step 37:step 38:talk 39:date_show
40:scene_exit 41:BAD_STATE
```

### place date, stage 0 (arrival) (`spot_place_arrival_steps`, 61 steps)

```
0:place_init 1:place_bank_load 2:place_bgm_start 3:common_script_load 4:wait_cd 5:date_script_load 6:wait_cd 7:place_tables_load
8:wait_cd 9:arrival 10:wait_cd 11:dec_init 12:change_bg 13:place_intro 14:dev_80045BEC 15:place_init
16:meeting_names 17:arrival_talk_step_common 18:meeting_choice_menu 19:meeting_decide 20:arrival_talk_step_common 21:bg_fadeout 22:arrival 23:wait_cd
24:dec_init 25:change_bg 26:place_bank_load 27:place_bgm_start 28:bg_fadein 29:place_init 30:meeting_place_check 31:arrival_talk_step_common
32:mira_mood_jump 33:portrait_read_pair 34:read_bustup 35:wait_cd 36:bustup_addr_init 37:dev_8004AB74 38:reaction_rows_build 39:arrival_wait
40:low_looks_penalty 41:date_event_choose 42:arrival_talk_gate 43:wait_sub 44:girl_in 45:arrival_talk 46:all_places_equal 47:special_day_talk
48:stage_next 49:arrival_talk_step_common 50:cell_seven 51:wait_sub 52:arrival_talk_step_common 53:return 54:scene_close 55:arrival_talk
56:girl_out 57:arrival_talk 58:return 59:scene_close 60:BAD_STATE
```

### place 0 neighborhood_park, stage 1 (`spot_place_neighborhood_park_steps`, 18 steps)

```
0:branch_on_mood 1:date_event_choose 2:speak012 3:select2 4:special_case 5:speak 6:talk_variant 7:expression1
8:result_talk 9:expression_mood 10:birthday_check 11:skip_step_unless_flag 12:year_talk 13:mood_talk 14:girl_out 15:return
16:scene_close 17:BAD_STATE
```

### place 1 kirameki_central_park, stage 1 (`spot_place_kirameki_central_park_steps`, 35 steps)

```
0:expression_mood 1:date_event_choose 2:branch 3:speak 4:girl_out 5:bg_load 6:wait_cd 7:dec_init
8:assets_load 9:wait_cd 10:step 11:move_place 12:dev_80046900 13:date_event_choose 14:girl_in 15:speak
16:select3 17:outcome_apply 18:speak 19:talk_until_done 20:girl_out 21:move_place 22:place_init 23:girl_in
24:birthday_check 25:face_from_row 26:result_talk 27:expression_mood 28:skip_step_unless_flag 29:year_talk 30:mood_talk 31:girl_out
32:return 33:scene_close 34:BAD_STATE
```

### place 2 shopping_street, stage 1 (`spot_place_shopping_street_steps`, 48 steps)

```
0:expression_mood 1:speak012 2:select3 3:special_case 4:branch 5:speak 6:speak_branch 7:girl_out
8:shopping_street_shop_choose 9:wait_cd 10:dec_init 11:bgm_start 12:move_place 13:dev_80046900 14:girl_in 15:date_event_choose
16:branch_on_mood 17:speak 18:branch_on_mood 19:girl_out 20:speak 21:girl_in 22:set_state 23:speak
24:select3 25:outcome_apply 26:speak 27:branch 28:speak 29:expression 30:speak 31:talk_until_done
32:girl_out 33:move_place 34:bgm_start 35:place_init 36:girl_in 37:birthday_check 38:face_from_row 39:result_talk
40:expression_mood 41:skip_step_unless_flag 42:year_talk 43:mood_talk 44:girl_out 45:return 46:scene_close 47:BAD_STATE
```

### place 3 aquarium, stage 1 (`spot_place_aquarium_steps`, 32 steps)

```
0:expression_mood 1:date_event_choose 2:speak 3:girl_out 4:bg_load 5:wait_cd 6:dec_init 7:move_place
8:dev_80046900 9:date_event_choose 10:girl_in 11:speak 12:select3 13:outcome_apply 14:speak 15:speak_branch
16:talk_until_done 17:girl_out 18:move_place 19:place_init 20:girl_in 21:birthday_check 22:face_from_row 23:result_talk
24:expression_mood 25:skip_step_unless_flag 26:year_talk 27:mood_talk 28:girl_out 29:return 30:scene_close 31:BAD_STATE
```

### place 4 zoo, stage 1 (`spot_place_zoo_steps`, 35 steps)

```
0:expression_mood 1:special_case 2:speak 3:girl_out 4:bg_load 5:wait_cd 6:dec_init 7:move_place
8:dev_80046900 9:date_event_choose 10:girl_in 11:speak 12:select3 13:outcome_apply 14:speak 15:branch
16:dev_800463C0 17:speak 18:set_state 19:talk_until_done 20:girl_out 21:move_place 22:place_init 23:girl_in
24:birthday_check 25:face_from_row 26:result_talk 27:expression_mood 28:skip_step_unless_flag 29:year_talk 30:mood_talk 31:girl_out
32:return 33:scene_close 34:BAD_STATE
```

### place 5 botanical_garden, stage 1 (`spot_place_botanical_garden_steps`, 34 steps)

```
0:expression_mood 1:date_event_choose 2:speak 3:girl_out 4:bg_load 5:wait_cd 6:dec_init 7:move_place
8:dev_80046900 9:date_event_choose 10:girl_in 11:speak 12:select3 13:branch 14:speak 15:speak
16:outcome_apply 17:speak 18:talk_until_done 19:girl_out 20:move_place 21:place_init 22:girl_in 23:birthday_check
24:face_from_row 25:result_talk 26:expression_mood 27:skip_step_unless_flag 28:year_talk 29:mood_talk 30:girl_out 31:return
32:scene_close 33:BAD_STATE
```

### place 6 planetarium, stage 1 (`spot_place_planetarium_steps`, 36 steps)

```
0:speak 1:girl_out 2:bg_load 3:wait_cd 4:dec_init 5:move_place 6:dev_80046900 7:girl_in
8:speak 9:bggirl_fadeout 10:wait_sub 11:date_event_choose 12:dev_800464E8 13:speak 14:branch 15:speak
16:select3 17:outcome_apply 18:speak 19:speak_branch 20:talk_until_done 21:girl_out 22:move_place 23:place_init
24:girl_in 25:birthday_check 26:face_from_row 27:result_talk 28:expression_mood 29:skip_step_unless_flag 30:year_talk 31:mood_talk
32:girl_out 33:return 34:scene_close 35:BAD_STATE
```

### place 7 art_museum, stage 1 (`spot_place_art_museum_steps`, 37 steps)

```
0:speak 1:girl_out 2:bg_load 3:wait_cd 4:dec_init 5:move_place 6:dev_80046900 7:date_event_choose
8:girl_in 9:step 10:speak 11:branch_on_mood 12:select3 13:expression 14:expression 15:speak
16:outcome_apply 17:speak 18:outcome_apply 19:speak 20:speak_branch 21:talk_until_done 22:girl_out 23:move_place
24:place_init 25:girl_in 26:birthday_check 27:face_from_row 28:result_talk 29:expression_mood 30:skip_step_unless_flag 31:year_talk
32:mood_talk 33:girl_out 34:return 35:scene_close 36:BAD_STATE
```

### place 8 library, stage 1 (`spot_place_library_steps`, 36 steps)

```
0:date_event_choose 1:expression_mood 2:date_event_choose 3:speak 4:branch 5:girl_out 6:bg_load 7:wait_cd
8:dec_init 9:move_place 10:dev_80046900 11:date_event_choose 12:girl_in 13:speak 14:select3 15:outcome_apply
16:speak 17:branch 18:speak 19:branch 20:talk_until_done 21:girl_out 22:move_place 23:place_init
24:girl_in 25:birthday_check 26:face_from_row 27:result_talk 28:expression_mood 29:skip_step_unless_flag 30:year_talk 31:mood_talk
32:girl_out 33:return 34:scene_close 35:BAD_STATE
```

### place 9 game_center, stage 1 (`spot_place_game_center_steps`, 39 steps)

```
0:speak 1:branch 2:speak012 3:select2 4:label_set 5:reaction_rows_build 6:girl_out 7:bg_load
8:wait_cd 9:dec_init 10:move_place 11:dev_80046900 12:girl_in 13:date_event_choose 14:speak 15:select3
16:outcome_apply 17:speak_branch 18:speak_branch 19:expression 20:speak_branch 21:expression 22:speak 23:talk_until_done
24:girl_out 25:move_place 26:place_init 27:girl_in 28:birthday_check 29:face_from_row 30:result_talk 31:expression_mood
32:skip_step_unless_flag 33:year_talk 34:mood_talk 35:girl_out 36:return 37:scene_close 38:BAD_STATE
```

### place 10 bowling_alley, stage 1 (`spot_place_bowling_alley_steps`, 32 steps)

```
0:expression_mood 1:date_event_choose 2:speak 3:girl_out 4:bg_load 5:wait_cd 6:dec_init 7:move_place
8:dev_80046900 9:date_event_choose 10:girl_in 11:speak 12:select3 13:outcome_apply 14:speak 15:speak_branch
16:talk_until_done 17:girl_out 18:move_place 19:place_init 20:girl_in 21:birthday_check 22:face_from_row 23:result_talk
24:expression_mood 25:skip_step_unless_flag 26:year_talk 27:mood_talk 28:girl_out 29:return 30:scene_close 31:BAD_STATE
```

### place 11 karaoke, stage 1 (`spot_place_karaoke_steps`, 32 steps)

```
0:expression_mood 1:date_event_choose 2:speak 3:girl_out 4:bg_load 5:wait_cd 6:dec_init 7:move_place
8:dev_80046900 9:date_event_choose 10:girl_in 11:speak 12:select3 13:outcome_apply 14:speak_branch 15:speak
16:talk_until_done 17:girl_out 18:move_place 19:place_init 20:girl_in 21:birthday_check 22:face_from_row 23:result_talk
24:expression_mood 25:skip_step_unless_flag 26:year_talk 27:mood_talk 28:girl_out 29:return 30:scene_close 31:BAD_STATE
```

### place 12 amusement_park, stage 1 (`spot_place_amusement_park_steps`, 103 steps)

```
0:speak 1:label_set 2:girl_out 3:bg_load 4:wait_cd 5:dec_init 6:move_place 7:dev_80046900
8:date_event_choose 9:girl_in 10:branch 11:speak012 12:select2 13:branch 14:speak 15:set_state
16:speak012 17:select2 18:speak_branch 19:speak 20:girl_out 21:bg_load_labeled 22:wait_cd 23:dec_init
24:move_place 25:place_init 26:girl_in 27:face 28:speak 29:special_case 30:speak 31:expression_mood
32:branch 33:speak 34:select3 35:date_event_choose 36:girl_out 37:amusement_ride_choose 38:wait_cd 39:dec_init
40:move_place 41:place_init 42:girl_in 43:special_case 44:special_case 45:special_case 46:girl_out 47:bg_fadeout
48:ride_movie_run 49:bg_fadein 50:girl_in 51:set_state 52:special_case 53:select3 54:outcome_apply 55:speak_branch
56:date_event_choose 57:speak_branch 58:talk_until_done 59:face 60:date_event_choose 61:ride_talk 62:select2 63:branch
64:girl_out 65:bg_load_labeled 66:wait_cd 67:dec_init 68:move_place 69:dev_80046900 70:girl_in 71:expression
72:ride_talk 73:branch 74:ride_talk 75:set_state 76:girl_out 77:bg_load_labeled 78:wait_cd 79:dec_init
80:move_place 81:dev_80046900 82:girl_in 83:birthday_check 84:face_from_row 85:result_talk 86:expression_mood 87:skip_step_unless_flag
88:year_talk 89:mood_talk 90:girl_out 91:return 92:scene_close 93:BAD_STATE 94:speak 95:expression2
96:speak 97:girl_out 98:speak 99:girl_in 100:speak 101:jump_101 102:BAD_STATE
```

### place 13 stadium, stage 1 (`spot_place_stadium_steps`, 29 steps)

```
0:speak 1:girl_out 2:step 3:wait_cd 4:dec_init 5:move_place 6:dev_80046900 7:girl_in
8:date_event_choose 9:speak 10:select3 11:outcome_apply 12:talk_variant 13:talk_until_done 14:girl_out 15:move_place
16:place_init 17:girl_in 18:birthday_check 19:face_from_row 20:result_talk 21:expression_mood 22:skip_step_unless_flag 23:year_talk
24:mood_talk 25:girl_out 26:return 27:scene_close 28:BAD_STATE
```

### place 14 movie_theater, stage 1 (`spot_place_movie_theater_steps`, 45 steps)

```
0:speak 1:girl_out 2:bg_load 3:wait_cd 4:dec_init 5:move_place 6:dev_80046900 7:girl_in
8:speak 9:girl_out 10:bg_fadeout 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:bg_fadein
16:talk_variant 17:date_event_choose 18:bg_fadeout 19:change_bg 20:bg_fadein 21:speak 22:bg_load 23:wait_cd
24:dec_init 25:move_place 26:girl_in 27:place_init 28:speak 29:select3 30:branch 31:outcome_apply
32:speak 33:talk_until_done 34:birthday_check 35:face_from_row 36:result_talk 37:expression_mood 38:skip_step_unless_flag 39:year_talk
40:mood_talk 41:girl_out 42:return 43:scene_close 44:BAD_STATE
```

### place 15 concert_hall, stage 1 (`spot_place_concert_hall_steps`, 41 steps)

```
0:speak 1:girl_out 2:bg_load 3:wait_cd 4:dec_init 5:move_place 6:dev_80046900 7:girl_in
8:speak 9:girl_out 10:bg_fadeout 11:bg_load 12:wait_cd 13:dec_init 14:change_bg 15:bg_fadein
16:talk_variant 17:date_event_choose 18:bg_load 19:wait_cd 20:dec_init 21:move_place 22:place_init 23:girl_in
24:speak 25:select3 26:branch 27:outcome_apply 28:speak 29:talk_until_done 30:birthday_check 31:face_from_row
32:result_talk 33:expression_mood 34:skip_step_unless_flag 35:year_talk 36:mood_talk 37:girl_out 38:return 39:scene_close
40:BAD_STATE
```

### place 16 pool, stage 1 (`spot_place_pool_steps`, 37 steps)

```
0:speak 1:girl_out 2:dev_8004B9B8 3:wait_cd 4:show_checked 5:bg_load 6:wait_cd 7:dec_init
8:move_place 9:dev_80046900 10:date_event_choose 11:girl_in 12:speak 13:select3 14:outcome_apply 15:speak
16:athletics_check_talk 17:talk_until_done 18:girl_out 19:move_place 20:place_init 21:read_bustup 22:wait_cd 23:show_checked
24:face 25:girl_in 26:birthday_check 27:face_from_row 28:result_talk 29:expression_mood 30:skip_step_unless_flag 31:year_talk
32:mood_talk 33:girl_out 34:return 35:scene_close 36:BAD_STATE
```

### place 17 sea, stage 1 (`spot_place_sea_steps`, 47 steps)

```
0:speak 1:girl_out 2:dev_8004B9B8 3:wait_cd 4:show_checked 5:bg_load 6:wait_cd 7:dec_init
8:move_place 9:dev_80046900 10:date_event_choose 11:step 12:branch 13:speak 14:girl_out 15:speak
16:wait_sub 17:dev_8004A89C 18:girl_in 19:speak 20:set_state 21:speak 22:select3 23:outcome_apply
24:speak 25:speak_branch 26:face 27:athletics_check_talk 28:talk_until_done 29:girl_out 30:move_place 31:place_init
32:read_bustup 33:wait_cd 34:show_checked 35:face 36:girl_in 37:birthday_check 38:result_talk 39:expression_mood
40:skip_step_unless_flag 41:year_talk 42:mood_talk 43:girl_out 44:return 45:scene_close 46:BAD_STATE
```

### place 18 shrine, stage 1 (`spot_place_shrine_steps`, 61 steps)

```
0:branch 1:speak012 2:select2 3:expression 4:speak 5:next 6:speak 7:expression_mood
8:special_case 9:speak 10:special_case 11:speak 12:expression1 13:speak 14:girl_out 15:bg_fadeout
16:next 17:bg_fadein 18:girl_in 19:jump 20:speak 21:special_case 22:stat_update 23:expression1
24:speak 25:girl_out 26:bg_fadeout 27:next 28:bg_fadein 29:girl_in 30:jump 31:speak
32:girl_out 33:step 34:next 35:bg_fadeout 36:still_show 37:step 38:set_state 39:speak
40:special_case 41:select3 42:outcome_apply 43:speak 44:speak_branch 45:talk_until_done 46:palette_load 47:bg_fadein
48:place_init 49:girl_in 50:birthday_check 51:face_from_row 52:result_talk 53:expression_mood 54:skip_step_unless_flag 55:year_talk
56:mood_talk 57:girl_out 58:return 59:scene_close 60:BAD_STATE
```

### place 19 skating_rink, stage 1 (`spot_place_skating_rink_steps`, 31 steps)

```
0:date_event_choose 1:speak 2:girl_out 3:bg_load 4:wait_cd 5:dec_init 6:move_place 7:dev_80046900
8:date_event_choose 9:girl_in 10:speak 11:select3 12:outcome_apply 13:speak 14:athletics_check_talk 15:talk_until_done
16:girl_out 17:move_place 18:place_init 19:girl_in 20:birthday_check 21:face_from_row 22:result_talk
23:expression_mood 24:skip_step_unless_flag 25:year_talk 26:mood_talk 27:girl_out 28:return 29:scene_close 30:BAD_STATE
```

The 31-entry copy is evidenced at `src/ovl/EVENT/8010BB90.c:1106` (`FnTbl31` over `D_801250D0`); the table occupies EVENT data entries `s10904`–`s10936` (`0x801250D0`–`0x80125148`).

### place 20 ski_resort, stage 1 (`spot_place_ski_resort_steps`, 31 steps)

```
0:date_event_choose 1:speak 2:girl_out 3:bg_load 4:wait_cd 5:dec_init 6:move_place 7:dev_80046900
8:date_event_choose 9:girl_in 10:speak 11:select3 12:outcome_apply 13:speak 14:athletics_check_talk 15:talk_until_done
16:girl_out 17:move_place 18:place_init 19:girl_in 20:birthday_check 21:face_from_row 22:result_talk 23:expression_mood
24:skip_step_unless_flag 25:year_talk 26:mood_talk 27:girl_out 28:return 29:scene_close 30:BAD_STATE
```

