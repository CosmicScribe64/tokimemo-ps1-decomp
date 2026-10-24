# Glossary

The game's terms and the identifier stems the code uses for them. Names in `src/` and `include/` take their
stems from here (CODING_STANDARDS section 4, `docs/adr/0001-phase-2-naming.md`). Terms are romanized; the
glossary holds proper names and short terms only, never game text.

Columns: **term** (the game's word, romanized, or an English term where the game has none), **meaning**,
**stem** (what identifiers use; constants use the upper-case form), **K/T** (K keeps the game's word, T
translates it), **source**.

Sources:

- *strings*: names and short labels in the executable's data.
- *disc*: file and folder names on the disc.
- *O.BIN*: the developer symbol table (`wiki/obin.md`).
- *patch*: the English-patch notes (`wiki/ref-english-patch.md`).
- *code*: what the code does (tables, values, callers).
- *sound*: the bank ids of the sound test.

A row whose meaning comes only from a name or a guess says so; code built on such a row marks the name as a
HYPOTHESIS.

The overlay and module prefixes are listed in the maps of the ADR. A prefix is a stem in a table below, an
abbreviation from the Abbreviations table, or a row of the Module words table. That table holds the plain
English words for modules (`boot`, `sprite`, `help`) and the short forms of stems (`sports` for
`sports_festival`, `trip` for `school_trip`). Every prefix in the ADR maps meets this rule.

A game-term word, one for a thing in the game's world or rules, comes from the tables here. An ordinary
technical word (`list`, `clear`, `advance`, `glyph`, `buffer`) is plain English and needs no row. Abbreviations
are allowed only from the Abbreviations table at the end.

## Traps

Words with more than one sense. Use the stem in the right column, never the bare word.

| word | senses | stems |
|---|---|---|
| date | a romantic outing; a calendar date; the DATE overlay | `date` (outing only); `calendar_date` or `day`; folder `date` |
| event | a day-event record in the queue; a scripted scene with a girl; the EVENT overlay; a memory-card event; a yearly school event (gyouji) | `day_event`; `scene`; folder `date_spots_unused`; `memory_card_event`; `annual_event`, folder `annual_events_unused` |
| mode | the game mode byte that selects the running screen; the menu mode setting chosen in the options | `game_mode` (`main_state` in the patch notes), also the prefix of the main loop module; `mode_stage` and `mode_step` for its two step bytes; `menu_mode` |
| stage | the step of a game mode; a level of the TACO shooter; the culture-festival stage | `mode_stage`; `shooter_level`; `stage_show` |
| state | the game mode (`main_state` in the patch notes); a girl's rank (zyotai) | `game_mode`; `girl_state` |
| week_day | O.BIN's name for the school-day command screen, not the day of the week | `school_day`; `weekday` for Monday-Sunday |
| holiday | a day without school; the calendar's holiday table (GameState 0x044) | `holiday`; `holiday_table` |
| master | mastering a club (the MASTER overlay); any other use of the word | `master` only for clubs |
| card | a memory card; a karuta card of the stage show | `memory_card`; `karuta` |
| pad | the controller; a padding field in a struct | `pad_` prefix of the controller code; `pad` field |

O.BIN spellings stay as applied (`schdeule_girls_param`, `syogatu`, `zyotai`); new names use the stems below.

## People

The girl records (`Rec38`, 16 entries) and the name tables share one index. Stems are given names, lower case.

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| shujinkou | the player character, named at the start | `player` | T | strings, O.BIN (`get_p_name`) |
| heroine | a girl the player can date; `girl` also names any girl-record index (0-15); "heroine" only in prose | `girl` | T | O.BIN (`select_girl`, `change_girl`) |
| Fujisaki Shiori | girl 0 | `shiori` | K | strings (name table) |
| Kisaragi Mio | girl 1 | `mio` | K | strings |
| Himoo Yuina | girl 2 | `yuina` | K | strings |
| Katagiri Ayako | girl 3 | `ayako` | K | strings |
| Nijino Saki | girl 4 | `saki` | K | strings |
| Koshiki Yukari | girl 5 | `yukari` | K | strings |
| Kiyokawa Nozomi | girl 6 | `nozomi` | K | strings |
| Kagami Mira | girl 7 | `mira` | K | strings |
| Asahina Yuko | girl 8 | `yuko` | K | strings |
| Mikihara Megumi | girl 9 | `megumi` | K | strings |
| Saotome Yumi | girl 10 | `yumi` | K | strings |
| Tatebayashi Miharu | girl 11 | `miharu` | K | strings |
| Ijuin Rei | record 12, the rival | `rei` | K | strings |
| Saotome Yoshio | record 13, the friend who gives information | `yoshio` | K | strings, O.BIN (`change_yoshio`) |
| Sotoi | record 14, the Ijuin family's butler | `sotoi` | K | strings |
| record 15 | no name in the tables; use unknown | none | | code (16 records, 15 names) |
| girl codes | two-letter codes in disc file names (AS, FU, HI, KA, KG, KI, KO, KY, MI, NI, SA, TA, IJ, YO) and four-letter family-name abbreviations in sound names | not stems | | disc, sound |
| -chan, -san, -kun | honorifics; parts of text, not names | not stems | | strings |

## Parameters and scores

The nine parameters are words at GameState 0x0FC-0x11C, in the order of the name table at `D_800B32D0`.

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| parameter | one of the player's nine stats | `param` | T | O.BIN (`parameter_up_down`, `check_para_limit`) |
| taichou | condition (health), parameter 0 | `condition` | T | strings |
| bunkei | liberal arts, parameter 1 | `liberal_arts` | T | strings |
| rikei | science, parameter 2 | `science` | T | strings |
| geijutsu | art, parameter 3 | `art` | T | strings |
| undou | athletics, parameter 4 | `athletics` | T | strings |
| zatsugaku | trivia, parameter 5 | `trivia` | T | strings |
| youshi | looks, parameter 6 | `looks` | T | strings |
| konjou | guts, parameter 7 | `guts` | T | strings |
| stress | stress, parameter 8 | `stress` | T | strings |
| yuukou | a girl's affection score (girl record +6) | `affection` | T | O.BIN (`yuukou_down`, `get_h_yuukou`), code |
| tokimeki | a girl's heart-throb score (girl record +2); the tokimeki meter | `tokimeki` | K | O.BIN (`get_h_tokimeki`), code |
| zyotai | a girl's state: a rank 0-4 computed from affection and tokimeki | `girl_state` | T | O.BIN (`get_g_zyotai_h`), code |
| syoushin | a per-girl counter (record +0xA) that rises by 5 a day while affection is under 10; meaning unknown | none yet | | O.BIN (`syoushin_up`), code |
| blood type | the player's blood type, entered at the start | `blood_type` | T | strings |

## Calendar and days

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| year, month, day, weekday | the calendar bytes at GameState 0x03E-0x041; weekday 0 is Monday | `year`, `month`, `day`, `weekday` | T | code (start 1995-04-04, a Tuesday) |
| hizuke | the calendar date and its panel on the day screen | `calendar_date` | T | O.BIN (`hizuke_init`) |
| heijitsu | school day and its command screen | `school_day` | T | O.BIN (`week_day`), strings |
| kyuujitsu | a day without school | `holiday` | T | O.BIN (`holiday_*`), strings |
| gakki | school term | `term` | T | strings |
| haru, natsu, fuyu yasumi | spring, summer and winter break | `spring_break`, `summer_break`, `winter_break`; `vacation` for any | T | strings, O.BIN (`vacation_day_init`), sound |
| kyousei hoshuu | the forced remedial week | `remedial_week` | T | strings |
| schedule | the schedule screen (game mode 0x30) and the code that plans each day | `schedule` | T | O.BIN (`schedule_init`) |
| yakusoku | an appointment for a date: place, month, day, girl and an active bit; 8 slots at GameState 0x54C | `appointment` | T | patch, code |
| day event | a record of the day-event queue (type, girl, scene, flags) that picks the day's scene | `day_event` | T | code (queue at GameState 0x69C) |
| event log | 256 records of 8 bytes at GameState 0x75E, with a count | `event_log` | T | code |
| tanjoubi | birthday; the player's is a word at GameState 0x0F8 (month in bits 0-3, day in bits 4-8) | `birthday` | T | code, O.BIN (`birth_day_check`) |

## Places

The 21 date places, ids 0-20 in the order of `date_place_names` (DATE). Kirameki is the town and school name.

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| Kirameki | the town and the school | `kirameki` | K | strings |
| neighborhood park | place 0 | `neighborhood_park` | T | strings |
| Kirameki Central Park | place 1 | `central_park` | T | strings |
| shopping street | place 2 | `shopping_street` | T | strings |
| aquarium | place 3 | `aquarium` | T | strings |
| zoo | place 4 | `zoo` | T | strings |
| botanical garden | place 5 | `botanical_garden` | T | strings |
| planetarium | place 6 | `planetarium` | T | strings |
| art museum | place 7 | `art_museum` | T | strings |
| library | place 8 (the public library) | `library` | T | strings |
| game center | place 9 | `game_center` | T | strings |
| bowling alley | place 10 | `bowling_alley` | T | strings |
| karaoke | place 11 | `karaoke` | T | strings |
| amusement park (yuuenchi) | place 12; its own overlay DATE2 | `amusement_park` | T | strings, sound (`YUUEN`) |
| stadium | place 13 | `stadium` | T | strings |
| movie theater | place 14 | `movie_theater` | T | strings |
| concert hall | place 15 | `concert_hall` | T | strings |
| pool | place 16 | `pool` | T | strings |
| sea | place 17 | `sea` | T | strings |
| shrine | place 18 | `shrine` | T | strings |
| skating rink | place 19 | `skating_rink` | T | strings |
| ski resort | place 20 | `ski_resort` | T | strings |
| school and town scenes | corridor, classroom, rooftop, behind the school, school gate, school library (toshoshitsu), lab, art room, music room, gym, schoolyard, courtyard, club room, staff room, front of the player's home, entrance, backstage, a girl's room, abandoned factory, Ijuin mansion, national stadium | `corridor`, `classroom`, `rooftop`, `behind_school`, `school_gate`, `school_library`, `lab`, `art_room`, `music_room`, `gym`, `schoolyard`, `courtyard`, `club_room`, `staff_room`, `home_front`, `entrance`, `backstage`, `girl_room`, `abandoned_factory`, `ijuin_mansion`, `national_stadium` | T | strings |
| Koshien | the national high-school baseball stadium | `koshien` | K | strings, disc (`KOSIEN`) |

## Clubs

Ids 0-11 are the order of `club_name_table` (`D_800B3908`).

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| bukatsu | a club the player can join | `club` | T | O.BIN (`join_club`), strings |
| bungei | literature club, 0 | `literature` | T | strings |
| engeki | drama club, 1 | `drama` | T | strings |
| kagaku | science club, 2 | `science_club` | T | strings |
| dennou | computer club, 3 | `computer` | T | strings |
| bijutsu | art club, 4 | `art_club` | T | strings |
| suisougaku | brass band, 5 | `brass_band` | T | strings |
| yakyuu | baseball, 6 | `baseball` | T | strings |
| soccer | soccer, 7 | `soccer` | T | strings |
| tennis | tennis, 8 | `tennis` | T | strings |
| suiei | swimming, 9 | `swimming` | T | strings |
| basketball | basketball, 10 | `basketball` | T | strings |
| mushozoku | no club, 11 | `no_club` | T | strings |
| taibu | quitting a club | `quit_club` | T | O.BIN (`holiday_taibu_club_*`) |
| okugi | a club's secret technique, learned by mastering the club | `secret_technique` | T | strings, disc (`MASTER/`) |
| master | mastering a club, and the tournaments and prizes that follow | `master` | T | disc (`MASTER.EXN`), strings |

`science_club` and `art_club` keep the parameter stems `science` and `art` free.

## Events and scenes

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| taiiku-sai | the sports festival | `sports_festival` | T | strings, disc (`TAIIKU/`), sound (`UNDOKAI`) |
| folk dance | the folk dance at the sports festival | `folk_dance` | T | disc (`TAIIKU/F_DANCE`) |
| bunka-sai | the culture festival | `culture_festival` | T | strings, sound (`SCLFEST`) |
| shuugaku ryokou | the school trip | `school_trip` | T | strings, sound (`SYUGAKU`) |
| shougatsu | New Year | `new_year` | T | strings, O.BIN (`pre_syogatu*`) |
| hatsumode | the first shrine visit of the year | `hatsumode` | K | strings |
| omikuji | a fortune slip drawn at the shrine | `omikuji` | K | strings |
| Christmas party | the Christmas party at the Ijuin mansion | `christmas` | T | strings, O.BIN (`pre_xmas*`), disc (`XMAS.BIN`) |
| Valentine's Day | February 14 scenes | `valentine` | T | disc (`VALEN.EXN`, `VALEN.BIN`) |
| White Day | March 14 scenes; which overlay plays them is not confirmed | `white_day` | T | disc (`WHITE.BIN`) |
| kimatsu shiken | the final exams and the rank list | `final_exam` | T | strings, sound (`EXAMIN`) |
| shinro shidou | career guidance | `career_guidance` | T | strings |
| shuushoku, shingaku | job or college choice at the end of school | `career_choice` | T | strings |
| sotsugyou | graduation | `graduation` | T | strings |
| kokuhaku | the confession at graduation | `confession` | T | strings |
| densetsu no ki | the tree legend that frames the confession | `legend_tree` | T | strings |
| ending, epilogue | the graduation ending and the girls' futures | `ending`, `epilogue` | T | disc (`ENDING.BIN`, `EPILOG.BIN`) |
| gasshuku | the club summer camp | `summer_camp` | T | strings, sound (`GASSYUKU`) |
| kimodameshi | the test of courage at the camp | `test_of_courage` | T | strings |
| kangei | the KANGEI overlay's word ("welcome"); its link to the summer camp is a guess | none until confirmed | | disc |
| renshuu shiai | a club practice match | `practice_match` | T | disc (`SHIAI/`) |
| omimai | a girl's visit when the player is ill | `sick_visit` | T | strings, disc (`OMIMAI.BIN`) |
| ennichi | festival stalls | `festival_stalls` | T | disc (`EN_GAME/`), sound (`ENNITI`) |
| hanabi taikai | the fireworks festival | `fireworks` | T | strings |
| deeto | a date with a girl | `date` | T | O.BIN (`normal_date_*`), disc (`DATE.EXN`) |
| sasoi | an invitation, from a girl or to her | `invite` | T | O.BIN (`date_sasoi_judge`), disc (`G_INVITE`) |
| geko | walking home from school; a meeting on the way is an encounter (next row) | `walk_home` | T | O.BIN (`geko_judge`), disc (`GEKO.BIN`) |
| encounter | a meeting with a girl outside a date: on the way home, an invitation, a birthday; the GEKO overlay (folder `girl_encounters`, prefix `encounter_`) plays them | `encounter` | T | disc (`GEKO.EXN`); the grouping is a guess until GEKO is confirmed |
| denwa | a phone call with a girl; the TEL overlay (folder `telephone`, prefix `phone_`), which also shows the girl profile page | `phone` | T | disc (`TEL.EXN`) |
| kaiwa | a talk with a girl: the script sections, lines and choices that the talk runner steps through | `talk` | T | patch (the talk runner `func_80082764`) |
| gyouji | a yearly school event; the GYOZI overlay's word, a near copy of SHOUGATU | `annual_event`, prefix `annual_` | T | disc (`GYOZI.EXN`); the meaning is a guess from the name |
| bunkaken | the BUNKAKEN overlay's word; it plays a culture-festival tour that visits six culture clubs | `culture_festival_tour`, prefix `tour_` | T | disc (`BUNKAKEN.EXN`), code; a guess until BUNKAKEN is confirmed |
| uwasa | a rumor report | `rumor` | T | O.BIN (`uwasa_main`) |
| spot | a place inside a date or scene | `spot` | T | code |
| arrival | whether the player arrives on time for a date | `arrival` | T | patch (`date_arrival`) |
| purezento | a gift; 93 gift names in DATE | `gift` | T | patch (`date_gift_names`) |
| section | one of a girl's script sections (81 per script file) | `section` | T | patch (`date_fill_sections_*`) |

## Systems and UI

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| main state | the game mode byte (GameState 0x1108) that selects the screen or overlay; the patch name `main_state_set` becomes `game_mode_set` | `game_mode` | T | patch (`main_state_set`), code |
| stage, step | the two bytes after the game mode that a mode's code steps through | `mode_stage`, `mode_step` | T | code |
| overlay | an `.EXN` file loaded into the shared code slot | `ovl` | T | O.BIN, patch (`ovl_load_*`) |
| scene | a scripted sequence that an overlay plays | `scene` | T | code |
| menu mode | the cursor control setting (mouse-like, A, B, mouse and A) | `menu_mode` | T | strings, code |
| bustup | a character portrait sprite | `portrait` | T | O.BIN (`bustup_*`) |
| SD | a super-deformed (small, big-headed) character sprite | `sd` | K | disc (`*_SD`), O.BIN (`addr_init_*_sd`) |
| event face | a girl's face picture in a scene | `event_face` | T | O.BIN (`ev_*`) |
| kuchi | a mouth picture of an event face | `mouth` | T | O.BIN |
| me | an eye picture of an event face | `eyes` | T | O.BIN (`ev_me`) |
| yubi | the finger-shaped cursor | `finger_cursor` | T | O.BIN (`Yubi`) |
| taku | a choice in a menu | `choice` | T | O.BIN (`x_taku_*`) |
| message window | the text window | `message_window` | T | O.BIN |
| kanji | the typewriter text system | `text`; O.BIN `k_disp_*` | T | O.BIN (`set_kanji_string`) |
| speaker label | the name shown above a line | `speaker` | T | O.BIN (`gnsx`, `sndisp`) |
| name codes | placeholders in text for the player's and girls' names | `name_code` | T | patch |
| hud | the panels of the day screen: parameters, calendar date, portrait | `hud` | T | code |
| save, load | the memory-card save and load | `save`, `load`, `memory_card` | T | O.BIN (`card_ev_set`) |
| name entry | the entry of the player's name, birthday and blood type at the start; the NAME_ENT overlay | `name_entry` | T | disc (`NAME_ENT.EXN`), patch (kana grid) |
| prologue | the voiced opening lines that NAME_ENT plays, in the attract run too | `prologue` | T | patch (`prologue_line_start`), emulator run (T-20500) |
| sound test | the OPTION screen that plays music and sound effects by bank id | `sound_test` | T | sound, strings |
| debug menu | a developer menu whose text is in OPTION | `debug_menu` | T | patch (OPTION text offsets) |
| system file | the memory-card system file; its contents are not decoded yet | `system_file` | T | strings |
| album | the album of scene pictures | `album` | T | strings |
| dec_bg | a background picture decoded by the MDEC | `bg` | T | O.BIN (`dec_bg_*`) |
| STR | a streamed movie | `movie` | T | O.BIN (`strInit`) |
| XA | CD-XA streamed audio | `xa` | K | O.BIN (`xa_wait`) |
| BGM, SE | music tracks and sound effects | `bgm`, `se` | K | sound |
| johoshi | the information magazine | `magazine` | T | O.BIN |

Developer abbreviations not resolved yet (`srn`, `mod`, `dtd`, `don`, `tarao`, `Sw_`, `Hw_`) keep their O.BIN form until the code shows what they mean.

## Mini games

| term | meaning | stem | K/T | source |
|---|---|---|---|---|
| shooter (TACO) | the hidden 3D shooter with three levels and bosses | `shooter` | T | strings, disc (`TACO/`), sound (`SHOOT`) |
| TT game | the hidden 2D shooter; TwinBee-like is a guess from disc and sound names | to agree with the TT contributor | | disc (`TWINBEE.BIN`, `TWINSGD/`), sound (`TWIN`) |
| kingyo sukui | goldfish scooping | `goldfish_scooping` | T | disc (`EN_GAME/KINGYO`) |
| shateki | the shooting gallery | `shooting_gallery` | T | disc (`EN_GAME/SHATEKI`) |
| RPG battle | the battle mini game with items and money | `rpg` | T | strings, disc (`BATTLE/`) |
| bancho | a delinquent gang boss, enemy of the RPG battle | `bancho` | K | strings |
| karuta, Kintaro | culture-festival stage shows | `karuta`, `kintaro` | K | disc (`BUNKASD/`) |
| UFO catcher | a crane game | `ufo_catcher` | T | disc (`UFOCATCH.BIN`) |

## Module words

Prefix words that are neither a stem nor an abbreviation: plain English words for a module's subject, and short
forms of stems. The ADR maps use them as prefixes. An overlay's word is a word of its folder's English name
or of a stem for one of its scenes.

| word | meaning | module | form |
|---|---|---|---|
| `tour` | the tour of six culture clubs | BUNKAKEN | short form of `culture_festival_tour` |
| `culture` | the culture festival | BUNKASAI | short form of `culture_festival` |
| `stage_show` | the culture festival's stage shows | BUNKA_SD | plain words |
| `amusement` | the amusement park date | DATE2 | short form of `amusement_park` |
| `stalls` | the festival stall games | EN_NICHI | short form of `festival_stalls` |
| `exam` | the final exams and the rank list | ETC | short form of `final_exam` |
| `career` | career guidance and choice | ETC | short form of `career_guidance`, `career_choice` |
| `annual` | the yearly school events | GYOZI | short form of `annual_event` |
| `camp` | the summer camp | KANGEI | short form of `summer_camp` |
| `help` | the help pages | OLH | plain word |
| `options` | the options screen | OPTION | plain word |
| `practice` | the club practice match | RENSYU | short form of `practice_match` |
| `trip` | the school trip | SHUGAKU | short form of `school_trip` |
| `sports` | the sports festival | TAIIKU | short form of `sports_festival` |
| `boot` | startup | main, `80041000`, `800420D0` | plain word |
| `draw` | 2D drawing | main, `80043510` | plain word |
| `sprite` | sprites | main, `80047550`, `800490C0` | plain word |
| `color` | color bars and palettes | main, `80049FF0`, `8004AD60` | plain word |
| `layer` | screen layers | main, `8004C3F0` | plain word |
| `model` | 3D model transfer | main, `8004C3F0` | plain word |
| `menu` | menus | main, `8004F6B0`, `8004F870` | plain word |
| `timer` | timers | main, `80053650` | plain word |
| `view` | the 3D view setup | main, `80058D20` | plain word |
| `mouse` | the mouse-like cursor control | main, `80059710` | plain word |
| `settings` | the system settings menu | main, `8005A0B0` | plain word |
| `title` | the title menu | main, `80061710` | plain word |
| `calendar` | the calendar panel | main, `80062CD0`, `800674B0` | short form of `calendar_date` |
| `message` | the message window | main, `80062CD0`, `800674B0` | short form of `message_window` |
| `face` | event faces | main, `8007C030` | short form of `event_face` |
| `select_girl` | the girl selection screen | main, `8007C030` | O.BIN's stem (`select_girl_init`) |
| `fade` | screen fades | main, `80085E30` | plain word |
| `cursor` | the finger cursor | main, `80085E30` | short form of `finger_cursor` |

## Abbreviations

The only abbreviations a new name may use. A unit that needs another one adds its row in the same commit as
the first name that uses it.

| abbreviation | stands for | source |
|---|---|---|
| `2d`, `3d` | two- and three-dimensional; never at the start of a name (`shooter_3d`) | common use |
| `bg` | a background picture decoded by the MDEC | O.BIN (`dec_bg_*`) |
| `bgm`, `se` | music track, sound effect | sound |
| `buf` | buffer | patch (`ovl_load_buf`) |
| `cd` | the CD-ROM drive and its reads | SDK (`libcd`) |
| `clut` | color lookup table | SDK |
| `dst` | destination; a parameter name of copy-like functions | SDK (`strcpy`) |
| `gpu` | the graphics processor | SDK (`libgpu`) |
| `hud` | the panels of the day screen | code |
| `init` | initialize | O.BIN (`hizuke_init`) |
| `lba` | logical block address: a sector number on the disc | patch (`ovl_load_lba`) |
| `misc` | miscellaneous (the ETC overlay's folder `misc_scenes`) | disc (`ETC.EXN`) |
| `ot` | ordering table | SDK |
| `ovl` | overlay | O.BIN, patch |
| `param` | parameter | O.BIN |
| `prim` | GPU primitive | SDK |
| `rpg` | role-playing game (the RPG battle) | strings |
| `sd` | super-deformed sprite | disc, O.BIN |
| `src` | source; a parameter name of copy-like functions | SDK (`strcpy`) |
| `sys` | system helpers of the main executable, beside the pad code (`sys_` prefix) | ADR (main prefixes) |
| `tpage` | texture page | SDK |
| `vram` | video memory | SDK |
| `vsync` | vertical blank sync | SDK (`VSync`) |
| `xa` | CD-XA streamed audio | O.BIN (`xa_wait`) |
