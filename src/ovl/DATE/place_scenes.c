/* place_scenes.c: src/ovl/DATE/80132000.c:244: stage 0 and 1 handlers of all 21 places: place setups, arrival, choice scoring, special-scene chooser, date end. Original object 0x80132000-0x80141EE0 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_D_80121808 /* byte store here (main_api.h: s32, EVENT) */
#define MAIN_API_OVERRIDE_normal_date_speak /* int view: the original keeps $v0 busy after the call, date_shrine_closing_talk (T-17202; main_api.h: void) */
#define MAIN_API_OVERRIDE_tpage_buf_clear /* matched with void() (main_api.h: void(void)) */
#include "common.h"
#include "ovl/DATE.h"

void tpage_buf_clear();

extern u8 D_80121808;
extern s32 normal_date_speak();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_art_museum);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_art_museum);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_bowling_alley);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_bowling_alley);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_concert_hall);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_concert_hall);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_zoo);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_zoo);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_shrine);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_shrine);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_game_center);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_game_center);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_karaoke);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_karaoke);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_movie_theater);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_movie_theater);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_pool);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_pool);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_planetarium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_planetarium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_sea);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_sea);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_botanical_garden);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_botanical_garden);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_shopping_street);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_shopping_street);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_skating_rink);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_skating_rink);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_ski_resort);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_ski_resort);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_stadium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_stadium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_aquarium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_aquarium);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_library);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_library);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_amusement_park);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_amusement_park);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_class_table_central_park);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_face_table_central_park);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5C0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5C4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5C8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5CC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5D0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5D4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5D8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5DC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5E0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5E4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5E8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5EC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5F0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5F4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5F8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B5FC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B600);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B604);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B608);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B60C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B610);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B614);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B618);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B61C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B620);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B624);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B628);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B62C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B630);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B634);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B638);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B63C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B640);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B644);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B648);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B64C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015B650);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_arrival);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_groups_per_variant);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_birthday_near);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_hint_roll);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_exhibit_artist_active);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_exhibit_sculpture_active);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_exhibit_painters_active);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_game_center_medals_open);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_park_new_rides);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_park_newest_rides);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_show_taken);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_parade_offered);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_movie_index);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_concert_index);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_ride_index);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_shop_index);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_first_sunday);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_stadium_game_day);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_ride_chosen);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_sign_palette_frame);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_movie_audio_pairs);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_concert_audio_pairs);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_opening_count_a);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_opening_count_b);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_syoushin_weight);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_affinity);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_bank_third_argument);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_bank_first_data_argument);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_bank_second_data_argument);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_load_command);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_sequence_command);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_place_names);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_firework_targets);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD34);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD38);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD3C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_particle_targets);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_shared_section_text);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD94);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD98);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BD9C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_shared_section_command);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDA4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDA8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDAC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_shared_section_voice);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDB4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDB8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDBC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_section_text);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDC4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDC8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDCC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDD0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDD4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDD8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDDC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDE0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDE4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDE8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDEC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDF0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDF4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDF8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BDFC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE00);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE04);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE08);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE0C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE10);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE14);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE18);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE1C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE20);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE24);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE28);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_section_command);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE30);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE34);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE38);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE3C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE40);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE44);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE48);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE4C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE50);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE54);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE58);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE5C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE60);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE64);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE68);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE6C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE70);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE74);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE78);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE7C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE80);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE84);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE88);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE8C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE90);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE94);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_section_voice);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BE9C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEA0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEA4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEA8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEAC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEB0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEB4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEB8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEBC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEC0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEC4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEC8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BECC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BED0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BED4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BED8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEDC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEE0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEE4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEE8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEEC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEF0);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEF4);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEF8);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BEFC);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", D_8015BF00);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_arrival_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_arrival_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_special_day_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_special_day_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_closing_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_closing_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_opening_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_opening_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_movie_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_movie_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_concert_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_concert_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_park_parade_group);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_park_parade_line);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/place_scenes.data", date_meeting_group);

/*
 * Matching notes (T-2010):
 * - Casts such as (u16) D_800CA150 or (u8) game_state.month pick the load width (lhu/lbu) the
 *   original uses where the shared header declares another type.
 * - *(s16 *)0x801CE12C reads data of another overlay by absolute address; a declared
 *   extern would change the register allocation of the load.
 */

void date_choice_tables_init(void) {
    date_class_table_art_museum = (DateMuseumChoiceRow *)0x801A427C;
    date_face_table_art_museum = (DateMuseumChoiceRow *)0x801A4D2C;
    date_class_table_bowling_alley = (u8 *)0x801A43EC;
    date_face_table_bowling_alley = (u8 *)0x801A4E9C;
    date_class_table_concert_hall = (u8 *)0x801A47F0;
    date_face_table_concert_hall = (u8 *)0x801A52A0;
    date_class_table_zoo = (u8 *)0x801A4190;
    date_face_table_zoo = (u8 *)0x801A4C40;
    date_class_table_shrine = (u8 *)0x801A4A04;
    date_face_table_shrine = (u8 *)0x801A54B4;
    date_class_table_game_center = (u8 *)0x801A4368;
    date_face_table_game_center = (u8 *)0x801A4E18;
    date_class_table_karaoke = (u8 *)0x801A4430;
    date_face_table_karaoke = (u8 *)0x801A4EE0;
    date_class_table_movie_theater = (u8 *)0x801A4684;
    date_face_table_movie_theater = (u8 *)0x801A5134;
    date_class_table_pool = (u8 *)0x801A497C;
    date_face_table_pool = (u8 *)0x801A542C;
    date_class_table_planetarium = (u8 *)0x801A4238;
    date_face_table_planetarium = (u8 *)0x801A4CE8;
    date_class_table_sea = (u8 *)0x801A49C0;
    date_face_table_sea = (u8 *)0x801A5470;
    date_class_table_botanical_garden = (u8 *)0x801A41F4;
    date_face_table_botanical_garden = (u8 *)0x801A4CA4;
    date_class_table_shopping_street = (u8 *)0x801A4064;
    date_face_table_shopping_street = (u8 *)0x801A4B14;
    date_class_table_skating_rink = (u8 *)0x801A4A28;
    date_face_table_skating_rink = (u8 *)0x801A54D8;
    date_class_table_ski_resort = (u8 *)0x801A4A6C;
    date_face_table_ski_resort = (u8 *)0x801A551C;
    date_class_table_stadium = (u8 *)0x801A4600;
    date_face_table_stadium = (u8 *)0x801A50B0;
    date_class_table_aquarium = (u8 *)0x801A412C;
    date_face_table_aquarium = (u8 *)0x801A4BDC;
    date_class_table_library = (u8 *)0x801A4324;
    date_face_table_library = (u8 *)0x801A4DD4;
    date_class_table_amusement_park = (DateRideChoiceRow *)0x801A4474;
    date_face_table_amusement_park = (DateRideChoiceRow *)0x801A4F24;
    date_class_table_central_park = (u8 *)0x801A4000;
    date_face_table_central_park = (u8 *)0x801A4AB0;
}

void date_fireworks_resources_init(void) {
    D_8015B5C0 = 0x801F2820;
    D_8015B5C4 = 0x801F2830;
    D_8015B5C8 = 0x801F28B4;
    D_8015B5CC = *(s16 *)0x801F28CC;
    D_8015B5D0 = 0x801B0000;
    D_8015B5D4 = 0x801CA000;
    D_8015B5D8 = 0x801E2200;
    D_8015B5DC = 0x801B2000;
    D_8015B5E0 = 0x801B6000;
    D_8015B5E4 = 0x801BA000;
    D_8015B5E8 = 0x801BE000;
    D_8015B5EC = 0x801C2000;
    D_8015B5F0 = 0x801C6000;
    D_8015B5F4 = 0x801CA200;
    D_8015B5F8 = 0x801CE200;
    D_8015B5FC = 0x801D2200;
    D_8015B600 = 0x801D6200;
    D_8015B604 = 0x801DA200;
    D_8015B608 = 0x801DE200;
}

void date_central_park_particles_init(void) {
    D_8015B60C = 0x801F0638;
    D_8015B610 = 0x801F0C34;
    D_8015B614 = 0x801F063C;
    D_8015B618 = 0x801F0C3C;
    D_8015B61C = 0x801F071C;
    D_8015B620 = 0x801F0CEC;
    D_8015B624 = *(s16 *)0x801F0D14;
    D_8015B628 = *(s16 *)0x801F0D18;
    D_8015B62C = 0x801E0000;
    D_8015B630 = 0x801E2000;
}

void date_parade_resources_init(void) {
    D_8015B634 = (u8 *)0x801BE078;
    D_8015B638 = (u8 *)0x801BE084;
    D_8015B63C = (u8 *)0x801BE088;
    D_8015B640 = *(s16 *)0x801BE090;
    D_8015B644 = (u8 *)0x801B0000;
}

void date_ambush_sections_init(void) {
    D_8015B648 = 0x801B0090;
    D_8015B64C = 0x801B00A4;
    D_8015B650 = 0x801B0170;
}

void date_arrival_init(void) {
    u8 girl_state;
    u8 arrival_stride_and_weekday;

    tpage_buf_clear();
    arrival_stride_and_weekday = 6;
    if ((D_800CA2F8 == 0xB) && (D_800CA2FC == 2)) {
        talk_group += date_groups_per_variant;
    } else {
        talk_group += D_800CA2FC * date_groups_per_variant;
    }
    talk_line = 0;
    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    /* FAKE: T-17202, `date_arrival * 0` mentions the promoted global before `girl_state`, so its load
     * (lui a0) is scheduled before the andi of the call result; the sum keeps the original order */
    date_arrival_group = date_arrival * 0 + ((girl_state >= 2) + (girl_state >= 3)) * ((date_arrival == DATE_ARRIVAL_WAIT_GREETING) + 1) + date_arrival * arrival_stride_and_weekday - (date_arrival == DATE_ARRIVAL_CORRECTED_PLACE) * 3;
    date_arrival_line = 0;
    *(s16 *)&D_800CA15C = 0;
    date_closing_line = 0;
    date_special_day_group = DATE_SPECIAL_DAY_MARCH;
    date_special_day_line = 0;
    date_park_parade_group = 0;
    date_park_parade_line = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    date_movie_line = 0;
    date_concert_line = 0;
    date_opening_group = 0;
    date_opening_line = 0;
    date_ride_chosen = 0;
    D_80122D34 = 0;
    if ((game_state.weekday == arrival_stride_and_weekday) && (game_state.day < 8)) {
        date_first_sunday = 1;
    } else {
        date_first_sunday = 0;
    }
    tpage_buf_clear(date_arrival);
    addr_init_bustup();
    talk_voice_wait_frames = 0;
    date_show_taken = 0;
    mode_step_next();
}

void date_scene_trigger(s16 scene_section, s16 mark_scene_seen, u8 scene_flag_index) {
    talk_work = scene_section;
    if (mark_scene_seen != 0) {
        game_state.scene_flags[scene_flag_index] = 1;
    }
    D_800CA368 = scene_flag_index;
    D_800CA364 = 1;
    D_80122D44 |= 2;
    game_mode_set(GAME_MODE_DATE_SCENE);
}

s32 date_scene_choose(void) {
    GameState *game;
    /* FAKE: byte-local order preserves stack slots +0x33/+0x31. T-11060 */
    u8 rank;
    u8 girl_state_flags;
    u8 girl_state_high_bit;
    u8 selection;
    /* FAKE: unsigned reload local preserves comparison operands. T-11060 */
    u32 place;
    u32 scene_selector;
    u32 alternate_girl_state;

    game = &game_state;
    girl_state_flags = get_g_zyotai_s(game->current_girl);
    girl_state_high_bit = girl_state_flags & 0x80;
    rank = girl_state_flags & 0x7F;
    alternate_girl_state = get_g_zyotai_h(game->current_girl);
    if (game->current_girl == GIRL_SHIORI) {
        if (((place = D_800CA2F8) == 1) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[3] == 0) && (game->mode_step == 0xD)) {
            date_scene_trigger(4, 1, 3);
            D_80122D44 |= 1;
            return 0;
        }
        if (((place = D_800CA2F8) == 0x10) && ((u8)game->year >= 0x60U) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (rank < 3U) && (alternate_girl_state < 2U) && (game->scene_flags[5] == 0)) {
            if ((game->girls[game->current_girl].scene_counters[0x1D] != 0) && (game->mode_stage == 1) && (game->mode_step == 0xA)) {
                date_scene_trigger(6, 1, 5);
                return 0;
            }
        }
        if (((place = D_800CA2F8) == 0) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((rank < 2U) && ((alternate_girl_state == 0) && ((game->scene_flags[2] == 0) && (game->mode_stage == 1) && (game->mode_step == 1))))) {
            date_scene_trigger(3, 1, 2);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xC) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (rank == 0) && (alternate_girl_state == 0) && (girl_state_high_bit != 0) && (game->scene_flags[4] == 0) && (D_80122CDC == 2) && (game->mode_stage == 1) && (game->mode_step == 0x23)) {
            date_scene_trigger(5, 1, 4);
            return 0;
        }
    }
    if (game->current_girl == GIRL_MIO) {
        if (((place = D_800CA2F8) == 0xC) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[9] == 0) && (D_80122CDC == 0) && ((date_park_new_rides != 0) && ((game->mode_stage == 1) && (game->mode_step == 0x23)))) {
            date_scene_trigger(2, 0, 9);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x11) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && ((game->scene_flags[10] == 0) && ((game->mode_stage == 1) && (game->mode_step == 0xA)))) {
            date_scene_trigger(3, 1, 0xA);
            return 0;
        }
        if (((place = D_800CA2F8) == 8) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((game->scene_flags[11] == 0) || ((game->scene_flags[11] == 1) && !(game->frame_count & 1))) && (game->mode_stage == 1) && (game->mode_step == 0xB)) {
            date_scene_trigger(4, 1, 0xB);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xE) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[12] == 0) && (date_movie_index == 6) && (game->mode_stage == 1) && (game->mode_step == 0x11)) {
            date_scene_trigger(5, 1, 0xC);
            return 0;
        }
    }
    if (game->current_girl == GIRL_YUINA) {
        if (((place = D_800CA2F8) == 4) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[15] == 0) && (((u8)game->year >= 0x60U) && (game->mode_stage == 1) && (game->mode_step == 8))) {
            date_scene_trigger(2, 1, 0xF);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xA) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (game->scene_flags[16] == 0) && ((game->year == 0x61) && ((game->mode_stage == 1) && (game->mode_step == 9)))) {
            date_scene_trigger(3, 1, 0x10);
            return 0;
        }
        if (((place = D_800CA2F8) == 9) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && (game->scene_flags[17] == 0) && ((u8)game->year >= 0x60U) && (date_game_center_medals_open != 0) && (D_80122CDC == 1) && (game->mode_stage == 1) && (game->mode_step == 0xC)) {
            date_scene_trigger(4, 1, 0x11);
            return 0;
        }
        if (((place = D_800CA2F8) == 2) && (game->month == 0xC) && ((scene_selector = (u8)date_shop_index) == 2) && (game->mode_stage == 1) && (game->mode_step == 0xF)) {
            date_scene_trigger(5, 1, 0x12);
            return 0;
        }
    }
    if (game->current_girl == GIRL_AYAKO) {
        if (((place = D_800CA2F8) == 0xB) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[22] == 0) && ((game->year == 0x61) && ((game->mode_stage == 1) && (game->mode_step == 9)))) {
            date_scene_trigger(2, 1, 0x16);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x11) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && ((game->scene_flags[23] == 0) && ((game->mode_stage == 1) && (game->mode_step == 0xA)))) {
            date_scene_trigger(3, 1, 0x17);
            return 0;
        }
        if (((place = D_800CA2F8) == 7) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && (game->scene_flags[24] == 0) && (((scene_selector = (u8)date_exhibit_artist_active) == 1) && ((game->mode_stage == 1) && (game->mode_step == 7)))) {
            date_scene_trigger(4, 1, 0x18);
            return 0;
        }
        if (((place = D_800CA2F8) == 6) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (rank < 2U) && (alternate_girl_state == 0) && (game->scene_flags[25] == 0) && ((game->year == 0x61) || (game->year == 0x62) || ((game->year == 0x60) && (game->month == 0xC))) && (game->mode_stage == 1) && (game->mode_step == 0xB)) {
            date_scene_trigger(5, 1, 0x19);
            return 0;
        }
    }
    if (game->current_girl == GIRL_SAKI) {
        if (((place = D_800CA2F8) == 1) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && ((((game->scene_flags[28] == 0) && (rank == 0) && (girl_state_high_bit != 0)) && (alternate_girl_state == 0)) && ((game->mode_stage == 1) && (game->mode_step == 0xD)))) {
            date_scene_trigger(2, 1, 0x1C);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x10) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (rank == 0) && (girl_state_high_bit != 0) && (alternate_girl_state == 0) && (game->scene_flags[29] == 0) && (game->mode_stage == 1) && (game->mode_step == 0xA) && (game->girls[game->current_girl].scene_counters[0x1D] != 0)) {
            date_scene_trigger(3, 1, 0x1D);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xD) && (date_stadium_game_day == 0) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && (game->scene_flags[30] == 0) && ((u8)((DateSelectionView *)&D_800CA2F8)->option < 2U) && (game->mode_stage == 1) && (game->mode_step == 8)) {
            date_scene_trigger(4, 1, 0x1E);
            return 0;
        }
        if (((place = D_800CA2F8) == 2) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[31] == 0) && (D_80122CDC == 1) && (game->mode_stage == 1) && (game->mode_step == 0xF)) {
            date_scene_trigger(5, 1, 0x1F);
            return 0;
        }
    }
    if (game->current_girl == GIRL_YUKARI) {
        if (((place = D_800CA2F8) == 5) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[35] == 0) && ((game->year == 0x61) && ((game->mode_stage == 1) && (game->mode_step == 9)))) {
            date_scene_trigger(3, 1, 0x23);
            return 0;
        }
        if (((place = D_800CA2F8) == 3) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (rank < 3U) && (alternate_girl_state < 2U) && (game->scene_flags[36] == 0) && ((u8)game->year >= 0x61U) && (game->mode_stage == 1) && (game->mode_step == 9)) {
            date_scene_trigger(4, 1, 0x24);
            return 0;
        }
        if (((place = D_800CA2F8) == 1) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((((game->scene_flags[37] == 0) && (rank == 0) && (girl_state_high_bit != 0)) && (alternate_girl_state == 0)) && ((game->mode_stage == 1) && (game->mode_step == 0xD)))) {
            date_scene_trigger(5, 1, 0x25);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x14) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[38] == 0) && (game->mode_stage == 1) && (game->mode_step == 0)) {
            date_scene_trigger(6, 1, 0x26);
            return 0;
        }
    }
    if (game->current_girl == GIRL_NOZOMI) {
        if (((place = D_800CA2F8) == 7) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[41] == 0) && ((date_exhibit_sculpture_active != 0) && ((game->mode_stage == 1) && (game->mode_step == 7)))) {
            date_scene_trigger(2, 1, 0x29);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x11) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (rank == 0) && (girl_state_high_bit != 0) && (alternate_girl_state == 0) && (game->scene_flags[42] == 0) && (game->param_athletics.value >= 0x96) && (game->mode_stage == 1) && (game->mode_step == 0xD)) {
            date_scene_trigger(3, 1, 0x2A);
            return 0;
        }
        if (((place = D_800CA2F8) == 5) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((game->scene_flags[43] == 0) && ((game->mode_stage == 1) && (game->mode_step == 9)))) {
            date_scene_trigger(4, 1, 0x2B);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xA) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[44] == 0) && (game->mode_stage == 1) && (game->mode_step == 9)) {
            date_scene_trigger(5, 1, 0x2C);
            return 0;
        }
    }
    if (game->current_girl == GIRL_MIRA) {
        if (((place = D_800CA2F8) == 2) && (D_80122CDC == 0) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && ((game->scene_flags[47] == 0) && ((game->mode_stage == 1) && (game->mode_step == 0xF)))) {
            date_scene_trigger(2, 1, 0x2F);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x11) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (game->mode_stage == 1) && (game->mode_step == 0xA)) {
            date_scene_trigger(3, 1, 0x30);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xC) && (D_80122CDC == 1) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((((game->scene_flags[49] == 0) && (rank == 0) && (girl_state_high_bit != 0)) && (alternate_girl_state == 0)) && ((game->mode_stage == 1) && (game->mode_step == 0x23)))) {
            date_scene_trigger(4, 0, 0x31);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x14) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[50] == 0) && (rank == 0) && (girl_state_high_bit != 0) && (alternate_girl_state == 0) && (game->mode_stage == 1) && (game->mode_step == 8)) {
            date_scene_trigger(5, 1, 0x32);
            return 0;
        }
        if ((game->scene_flags[51] == 0) && (rank == 0) && (girl_state_high_bit != 0) && (alternate_girl_state == 0) && (game->mode_stage == 0) && (game->mode_step == 0x29)) {
            date_scene_trigger(6, 1, 0x33);
            game->unk_54C[0].b[3] &= 0xFFDF;
            return 0;
        }
    }
    scene_selector = (u8)date_parade_offered;
    if (game->current_girl == GIRL_YUKO) {
        if (((place = D_800CA2F8) == 9) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[55] == 0) && ((date_game_center_medals_open == 0) || (D_80122CDC == 0)) && (game->mode_stage == 1) && (game->mode_step == 0xC)) {
            date_scene_trigger(2, 1, 0x37);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xC) && (scene_selector == 1) && ((u8)game->month >= 6U) && (((u8)game->month < 9U) && (rank == 0) && (girl_state_high_bit != 0) && ((alternate_girl_state == 0) && ((game->scene_flags[56] == 0) && ((game->mode_stage == 1) && (game->mode_step == 0x3C)))))) {
            date_scene_trigger(3, 1, 0x38);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xF) && (date_concert_index == 0xA) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((game->scene_flags[57] == 0) && ((game->mode_stage == 1) && (game->mode_step == 0x11)))) {
            date_scene_trigger(4, 1, 0x39);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x13) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[58] == 0) && (game->mode_stage == 1) && (game->mode_step == 8)) {
            date_scene_trigger(5, 1, 0x3A);
            return 0;
        }
        if (((place = D_800CA2F8) == 8) && (((game->year == 0x5F) && (game->month == 0xC) && (game->day == 0xA)) || ((game->year == 0x60) && (game->month == 0xC) && (game->day == 8)) || ((game->year == 0x61) && (game->month == 0xC) && (game->day == 7))) && (rank < 3U) && (((PlayerFlagBits *)&game->player_flags)->b17 != 0) && (game->mode_stage == 1) && (game->mode_step == 0)) {
            date_scene_trigger(6, 1, 0x3B);
            return 0;
        }
    }
    if (game->current_girl == GIRL_MEGUMI) {
        if (((place = D_800CA2F8) == 0xC) && (date_first_sunday != 0) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[62] == 0) && (game->mode_stage == 1) && (game->mode_step == 0x1B)) {
            date_scene_trigger(3, 1, 0x3E);
            return 0;
        }
        if (((place = D_800CA2F8) == 1) && ((u8)game->month >= 6U) && (((u8)game->month < 9U) && (rank == 0) && (girl_state_high_bit != 0) && ((alternate_girl_state == 0) && ((game->scene_flags[63] == 0) && ((game->mode_stage == 0) && (game->mode_step == 0x29)))))) {
            game->unk_54C[0].b[3] &= 0xFFDF;
            date_scene_trigger(4, 1, 0x3F);
            return 0;
        }
        if (((place = D_800CA2F8) == 4) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && ((game->scene_flags[64] == 0) && (game->mode_stage == 1) && (game->mode_step == 8))) {
            date_scene_trigger(5, 1, 0x40);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x13) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (game->scene_flags[65] == 0) && (rank == 0) && (girl_state_high_bit != 0) && (alternate_girl_state == 0) && (game->param_athletics.value >= 0x50) && (game->mode_stage == 1) && (game->mode_step == 8)) {
            date_scene_trigger(6, 1, 0x41);
            return 0;
        }
    }
    /* FAKE: this copy fixes one comparison operand order. T-11060 */
    selection = ((DateSelectionView *)&D_800CA2F8)->option;
    if (game->current_girl == GIRL_YUMI) {
        if (((place = D_800CA2F8) == 0xC) && ((u8)game->month >= 3U) && ((u8)game->month < 6U) && (game->scene_flags[69] == 0) && (game->mode_stage == 1) && (game->mode_step == 0)) {
            date_scene_trigger(3, 1, 0x45);
            return 0;
        }
        if (((place = D_800CA2F8) == 1) && ((u8)game->month >= 6U) && (((u8)game->month < 9U) && (rank == 0) && (girl_state_high_bit != 0) && ((alternate_girl_state == 0) && ((game->scene_flags[70] == 0) && (game->mode_stage == 1) && (game->mode_step == 1))))) {
            date_scene_trigger(4, 1, 0x46);
            return 0;
        }
        if (((place = D_800CA2F8) == 0xD) && ((u8)game->month >= 9U) && ((u8)game->month < 0xCU) && (game->scene_flags[71] == 0) && (((u8)selection >= 2U) && (game->mode_stage == 1) && (game->mode_step == 8))) {
            date_scene_trigger(5, 1, 0x47);
            return 0;
        }
        if (((place = D_800CA2F8) == 0x14) && ((game->month == 0xC) || (game->month == 1) || (game->month == 2)) && (rank == 0) && (girl_state_high_bit != 0) && ((alternate_girl_state == 0) && ((game->scene_flags[72] == 0) && (game->mode_stage == 1) && (game->mode_step == 0)))) {
            date_scene_trigger(6, 1, 0x48);
            return 0;
        }
    }
    if (((place = D_800CA2F8) == 0xB) && (selection == 2) && (game->mode_stage == 1) && (game->mode_step == 9)) {
        D_80122D44 = 2;
        game_mode_set(GAME_MODE_AMUSEMENT_PARK_2);
        return 0;
    }
    if ((game->month >= 3) && ((u8)game->month < 6U) && (game->girls[11].flags.f.story_stage >= 2U) && ((game->scene_flags[76] == 0) && ((game->mode_stage == 0) && (game->mode_step == 0xF)))) {
        talk_work = 3;
        game->saved_game_mode = GAME_MODE_DATE;
        game->saved_mode_stage = 0;
        game->saved_mode_step = game->mode_step + 1;
        D_80122CD4 = 0xB;
        game_mode_set(GAME_MODE_DATE_SCENE);
        game->scene_flags[76] = 1;
        D_80122D44 |= 2;
        return 0;
    }
    if (((place = D_800CA2F8) == 0x11) && ((u8)game->month >= 6U) && ((u8)game->month < 9U) && (game->girls[11].flags.f.story_stage >= 2U) && (game->scene_flags[78] == 0) && (game->mode_stage == 0) && (game->mode_step == 0xF)) {
        talk_work = 5;
        game->saved_game_mode = GAME_MODE_DATE;
        game->saved_mode_stage = 0;
        game->saved_mode_step = game->mode_step + 1;
        D_80122CD4 = 0xB;
        game_mode_set(GAME_MODE_DATE_SCENE);
        game->scene_flags[78] = 1;
        D_80122D44 |= 3;
        return 0;
    }
    mode_step_next();
}

void date_place_enter_check(void) {
    k_reset(0);
    message_window_set_title(scene_place_title);
    date_scene_choose();
}

void date_place_setup_neighborhood_park(void) {
    talk_text_table = D_8015BDDC;
    talk_command_table = D_8015BE48;
    talk_voice_table = D_8015BEB4;
    D_80122CF4 = 0;
    D_800CA2FC = 0;
    strcpy(scene_place_title, "近所の公園");
    date_groups_per_variant = 2;
    D_800CA22C[0] = 0;
    D_800CA22C[1] = 0;
    D_800CA22C[2] = 0;
    D_80122D08 = 0x21;
    bg_read_sub2(0x42D8);
    event_log_add(0, 0x21);
    mode_step_next();
}

void date_place_setup_central_park(void) {
    /* T-10900: the local state view preserves repeated byte reloads. */
    GameState *game;
    u8 girl_state_flags;
    s32 choice_offset;
    u32 alternate_girl_state;

    game = &game_state;

    talk_text_table = D_8015BE14;
    talk_command_table = D_8015BE80;
    talk_voice_table = D_8015BEEC;
    D_80122CF4 = 0;
    strcpy(scene_place_title, "きらめき中央公園");
    strcpy(scene_place_title_2, "並木道");
    date_groups_per_variant = 3;
    girl_state_flags = get_g_zyotai_s(game->current_girl);
    alternate_girl_state = get_g_zyotai_h(game->current_girl);
    if (((game->month == 3) && ((u8) game->day >= 0xFU)) || (game->month == 4)) {
        if (((game->scene_flags[3] == 0) && (game->current_girl == GIRL_SHIORI)) || ((game->scene_flags[28] == 0) && (game->current_girl == GIRL_SAKI) && (girl_state_flags == 0x80) && (alternate_girl_state == 0))) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 2;
            strcpy(scene_place_title_2, "桜並木");
        }
    } else if (!(game->girls[game->current_girl].scene_counters[1] & 1)) {
        if (((u8) game->month >= 6U) && ((u8) game->month < 9U) && (game->scene_flags[70] == 0) && (girl_state_flags == 0x80) && (alternate_girl_state == 0) && (game->current_girl == GIRL_YUMI)) {
            D_800CA2FC = 1;
            strcpy(scene_place_title_2, "池");
        } else {
            D_800CA2FC = 0;
        }
    } else if (((game->scene_flags[3] == 0) && (game->current_girl == GIRL_SHIORI) && ((u8) game->month >= 3U) && ((u8) game->month < 6U)) || ((game->scene_flags[28] == 0) && (game->current_girl == GIRL_SAKI) && ((u8) game->month >= 3U) && ((u8) game->month < 6U) && (alternate_girl_state == 0) && (girl_state_flags == 0x80))) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "池");
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 1;
    D_800CA22C[0] = (s16) *((game->current_girl * 9) + date_class_table_central_park + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_central_park + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_central_park + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 9) + date_face_table_central_park + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_central_park + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_central_park + choice_offset) + (2));
    if (((u8) game->month >= 3U) && ((u8) game->month < 9U)) {
        bg_read_sub2(0x42EB);
    } else {
        bg_read_sub2(0x42F4);
    }
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_aquarium(void) {
    GameState *game;
    s32 choice_offset;

    game = &game_state;

    talk_text_table = D_8015BE08;
    talk_command_table = D_8015BE74;
    talk_voice_table = D_8015BEE0;
    strcpy(scene_place_title, "水族館");
    date_groups_per_variant = 3;
    if ((game->girls[game->current_girl].scene_counters[6] == 0) && (((u8) game->year >= 0x61U) || ((game->year == 0x60) && ((u8) game->month >= 3U)))) {
        D_800CA2FC = 2;
        strcpy(scene_place_title_2, "イルカショー");
        D_80122D08 = 6;
    } else if (!(game->girls[game->current_girl].scene_counters[5] & 1)) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "館内");
        D_80122D08 = 5;
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "館内");
        D_80122D08 = 5;
    }
    choice_offset = D_800CA2FC * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 9) + date_class_table_aquarium + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_aquarium + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_aquarium + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 9) + date_face_table_aquarium + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_aquarium + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_aquarium + choice_offset) + (2));
    bg_read_sub2(0x4591);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_zoo(void) {
    GameState *game;
    s32 choice_offset;
    u32 visit_variant;

    game = &game_state;

    talk_text_table = D_8015BDCC;
    talk_command_table = D_8015BE38;
    talk_voice_table = D_8015BEA4;
    strcpy(scene_place_title, "動物園");
    date_groups_per_variant = 3;
    if ((game->girls[game->current_girl].scene_counters[8] == 0) && (((u8) game->year >= 0x60U) || ((game->year == 0x5F) && ((u8) game->month >= 0xCU)))) {
        D_800CA2FC = 2;
        strcpy(scene_place_title_2, "園内");
        D_80122D08 = 8;
    } else if (!(game->girls[game->current_girl].scene_counters[7] & 1)) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "園内");
        D_80122D08 = 7;
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "園内");
        D_80122D08 = 7;
    }
    if (game->current_girl == GIRL_MIRA) {
        D_80122D08 = 7;
        strcpy(scene_place_title_2, "園内");
        visit_variant = (u8) game->girls[game->current_girl].scene_counters[7] % 3U;
        if (visit_variant == 0) {
            D_800CA2FC = 0;
        } else if (visit_variant == 1) {
            D_800CA2FC = 1;
        } else {
            D_800CA2FC = 2;
        }
    }
    choice_offset = D_800CA2FC * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 9) + date_class_table_zoo + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_zoo + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_class_table_zoo + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 9) + date_face_table_zoo + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_zoo + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 9) + date_face_table_zoo + choice_offset) + (2));
    bg_read_sub2(0x457D);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_botanical_garden(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDF4;
    talk_command_table = D_8015BE60;
    talk_voice_table = D_8015BECC;
    strcpy(scene_place_title, "植物園");
    strcpy(scene_place_title_2, "園内");
    date_groups_per_variant = 3;
    if (game_state.current_girl == GIRL_MIRA) {
        date_groups_per_variant = 5;
    }
    if (!(game_state.girls[game_state.current_girl].scene_counters[9] & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 9;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_botanical_garden + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_botanical_garden + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_botanical_garden + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_botanical_garden + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_botanical_garden + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_botanical_garden + choice_offset + 2);
    bg_read_sub2(0x45D3);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_planetarium(void) {
    GameState *game;
    s32 choice_offset;

    game = &game_state;

    talk_text_table = D_8015BDEC;
    talk_command_table = D_8015BE58;
    talk_voice_table = D_8015BEC4;
    strcpy(scene_place_title, "プラネタリウム");
    date_groups_per_variant = 7;
    if (!(game->girls[game->current_girl].scene_counters[0xA] & 1)) {
        ((DateSelectionView *)&D_800CA2F8)->option = 0;
        strcpy(scene_place_title_2, "館内");
    } else {
        ((DateSelectionView *)&D_800CA2F8)->option = 1;
        strcpy(scene_place_title_2, "館内");
        if (game->current_girl == GIRL_YUMI) {
            talk_group = 1;
        }
    }
    D_80122D08 = 0xA;
    choice_offset = ((DateSelectionView *)&D_800CA2F8)->option * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 6) + date_class_table_planetarium + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_class_table_planetarium + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_class_table_planetarium + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 6) + date_face_table_planetarium + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_face_table_planetarium + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_face_table_planetarium + choice_offset) + (2));
    if ((game->param_science.value < 0x78) && (((DateSelectionView *)&D_800CA2F8)->option != 0)) {
        switch (game->current_girl) {
        case GIRL_SHIORI:
        case GIRL_AYAKO:
            D_800CA22C[0] = 4;
            D_800CA21C[0] = 4;
            break;
        case GIRL_YUINA:
        case GIRL_NOZOMI:
        case GIRL_YUKO:
            D_800CA22C[0] = 4;
            D_800CA21C[0] = 3;
            break;
        case GIRL_MIO:
        case GIRL_SAKI:
        case GIRL_YUKARI:
        case GIRL_MEGUMI:
            D_800CA21C[0] = 3;
            D_800CA22C[0] = 3;
            break;
        case GIRL_MIRA:
            D_800CA21C[0] = 3;
            D_800CA22C[0] = 3;
            break;
        case GIRL_YUMI:
            D_800CA21C[2] = 1;
            D_800CA22C[2] = 1;
            break;
        }
    }
    bg_read_sub2(0x45E6);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

/* FAKE: the signed narrow constant preserves the shared multiply/compare
 * range and its a2 register. Literal 3 strength-reduces the multiply.
 * T-10870: two three-halfword chains preserve each result row's stores. */
void date_place_setup_art_museum(void) {
    s16 fake_shared_constant = 3;
    s32 choice_offset;
    GameState *game;

    game = &game_state;
    talk_text_table = date_section_text;
    talk_command_table = date_section_command;
    talk_voice_table = date_section_voice;
    strcpy(scene_place_title, "美術館");
    date_groups_per_variant = fake_shared_constant;
    date_exhibit_painters_active = 0;
    date_exhibit_sculpture_active = 0;
    date_exhibit_artist_active = 0;
    if ((game->year == 0x61) && ((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
        ((DateSelectionView *)&D_800CA2F8)->option = 2;
        date_exhibit_artist_active = 1;
        strcpy(scene_place_title_2, "ガーギー展");
        if ((game->girls[game->current_girl].scene_counters[12] != 0) && (game->current_girl != fake_shared_constant)) {
            ((DateSelectionView *)&D_800CA2F8)->option = 5;
        }
        D_80122D08 = 0xC;
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 2;
        }
    } else if ((game->year == 0x61) && ((u8) game->month >= 3U) && ((u8) game->month < 6U)) {
        ((DateSelectionView *)&D_800CA2F8)->option = fake_shared_constant;
        date_exhibit_sculpture_active = 1;
        strcpy(scene_place_title_2, "彫塑展");
        if (game->girls[game->current_girl].scene_counters[13] != 0) {
            ((DateSelectionView *)&D_800CA2F8)->option = 5;
        }
        D_80122D08 = 0xD;
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 4;
        }
    } else if ((game->year == 0x5F) && ((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
        ((DateSelectionView *)&D_800CA2F8)->option = 4;
        date_exhibit_painters_active = 1;
        strcpy(scene_place_title_2, "有名画家展");
        if (game->girls[game->current_girl].scene_counters[14] != 0) {
            ((DateSelectionView *)&D_800CA2F8)->option = 5;
        }
        D_80122D08 = 0xE;
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 4;
        }
    } else if (!(game->girls[game->current_girl].scene_counters[11] & 1)) {
        ((DateSelectionView *)&D_800CA2F8)->option = 0;
        strcpy(scene_place_title_2, "館内");
        D_80122D08 = 0xB;
    } else {
        ((DateSelectionView *)&D_800CA2F8)->option = 1;
        strcpy(scene_place_title_2, "館内");
        D_80122D08 = 0xB;
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 2;
        }
    }
    choice_offset = ((DateSelectionView *)&D_800CA2F8)->option * fake_shared_constant;
    *D_800CA22C = (s16) *(game->current_girl * 0xF + (u8 *)date_class_table_art_museum + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 0xF + (u8 *)date_class_table_art_museum + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 0xF + (u8 *)date_class_table_art_museum + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 0xF + (u8 *)date_face_table_art_museum + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 0xF + (u8 *)date_face_table_art_museum + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 0xF + (u8 *)date_face_table_art_museum + choice_offset + 2);
    if (((DateSelectionView *)&D_800CA2F8)->option == 5) {
        switch (game->current_girl) {
        default:
            D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = fake_shared_constant;
            D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = fake_shared_constant;
            break;
        case GIRL_MIO:
        case GIRL_MIRA:
        case GIRL_YUKO:
            /* fallthrough */
        case GIRL_AYAKO:
            D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 4;
            D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 4;
            break;
        }
        if ((game->current_girl == fake_shared_constant) && (date_exhibit_artist_active != 0)) {
            D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = fake_shared_constant;
        }
        if (game->current_girl == GIRL_YUMI) {
            talk_group += 1;
        }
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 4;
        }
    }
    bg_read_sub2(0x44F1);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_library(void) {
    GameState *game;
    s32 choice_offset;

    game = &game_state;

    talk_text_table = D_8015BE0C;
    talk_command_table = D_8015BE78;
    talk_voice_table = D_8015BEE4;
    strcpy(scene_place_title, "図書館");
    date_groups_per_variant = 3;
    if (!(game->girls[game->current_girl].scene_counters[0xF] & 1)) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "図書館内");
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "図書館内");
        if (game->current_girl == GIRL_MIRA) {
            talk_group = 3;
        } else if (game->current_girl == GIRL_YUMI) {
            talk_group = 1;
        }
    }
    if ((game->current_girl == GIRL_YUKO) && (game->girls[game->current_girl].scene_counters[0xF] != 0)) {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "図書館内");
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0xF;
    D_800CA22C[0] = (s16) *((game->current_girl * 6) + date_class_table_library + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_class_table_library + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_class_table_library + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 6) + date_face_table_library + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_face_table_library + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 6) + date_face_table_library + choice_offset) + (2));
    bg_read_sub2(0x4544);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_game_center(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDD4;
    talk_command_table = D_8015BE40;
    talk_voice_table = D_8015BEAC;
    strcpy(scene_place_title, "ゲームセンター");
    date_groups_per_variant = 0;
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x10] & 1)) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "ビデオゲームコーナー");
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "ビデオゲームコーナー");
    }
    D_80122D08 = 0x10;
    if ((game_state.year >= 0x61) || ((game_state.year >= 0x60) && (game_state.month >= 9))) {
        date_game_center_medals_open = 1;
    } else {
        date_game_center_medals_open = 0;
    }
    choice_offset = D_800CA2FC * 3;
    *D_800CA22C = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset + 2);
    bg_read_sub2(0x44D6);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_bowling_alley(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDC4;
    talk_command_table = D_8015BE30;
    talk_voice_table = D_8015BE9C;
    strcpy(scene_place_title, "ボーリング場");
    date_groups_per_variant = 3;
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x12] & 1)) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "ボーリング場");
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "ボーリング場");
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x12;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_bowling_alley + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_bowling_alley + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_bowling_alley + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_bowling_alley + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_bowling_alley + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_bowling_alley + choice_offset + 2);
    bg_read_sub2(0x4631);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_karaoke(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDD8;
    talk_command_table = D_8015BE44;
    talk_voice_table = D_8015BEB0;
    strcpy(scene_place_title, "カラオケ");
    date_groups_per_variant = 3;
    if ((game_state.girls[game_state.current_girl].scene_counters[0x13] % 3) == 0) {
        D_800CA2FC = 0;
        strcpy(scene_place_title_2, "ＢＯＸ内");
    } else {
        D_800CA2FC = 1;
        strcpy(scene_place_title_2, "ＢＯＸ内");
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x13;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_karaoke + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_karaoke + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_karaoke + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_karaoke + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_karaoke + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_karaoke + choice_offset + 2);
    bg_read_sub2(0x4613);
    event_log_add(1, D_80122D08);
    if ((game_state.girls[game_state.current_girl].scene_counters[0x13] % 3) == 2) {
        D_800CA2FC = 2;
    }
    mode_step_next();
}

void date_place_setup_amusement_park(void) {
    date_virtual_ship_phase = 0;
    date_virtual_ship_subphase = 0;
    date_virtual_ship_step = 0;
    D_8015DE4C = 0;
    talk_text_table = D_8015BE10;
    talk_command_table = D_8015BE7C;
    talk_voice_table = D_8015BEE8;
    if ((game_state.year == 0x62) || ((game_state.year == 0x61) && (game_state.month >= 3))) {
        date_park_new_rides = 1;
    } else {
        date_park_new_rides = 0;
    }
    if ((game_state.year == 0x62) || ((game_state.year == 0x61) && (game_state.month == 0xC))) {
        date_park_newest_rides = 1;
    } else {
        date_park_newest_rides = 0;
    }
    date_groups_per_variant = 0;
    strcpy(scene_place_title, "遊園地");
    bg_read_sub2(0x432B);
    event_log_add(1, 0x14);
    mode_step_next();
}

void date_place_setup_shopping_street(void) {
    talk_text_table = D_8015BDF8;
    talk_command_table = D_8015BE64;
    talk_voice_table = D_8015BED0;
    date_groups_per_variant = 0;
    strcpy(scene_place_title, "ショッピング街");
    bg_read_sub2(0x4520);
    event_log_add(1, 2);
    mode_step_next();
}

void date_move_place_with_audio_command(void) {
    if (game_state.substep == 0) {
        audio_command(0x202);
        game_state.substep += 1;
    }
    normal_date_move_place();
}

void date_place_setup_stadium(void) {
    GameState *game;
    s32 choice_offset;
    u8 special_month;

    game = &game_state;

    talk_text_table = D_8015BE04;
    talk_command_table = D_8015BE70;
    talk_voice_table = D_8015BEDC;
    strcpy(scene_place_title, "スタジアム");
    strcpy(scene_place_title_2, "スタジアム内");
    date_groups_per_variant = 3;
    if ((game->month == 1) || (game->month == 3) || (game->month == 6) || (game->month == 0xB)) {
        special_month = 1;
    } else {
        special_month = 0;
    }
    date_stadium_game_day = special_month;
    if (special_month != 0) {
        if (!(game->girls[game->current_girl].scene_counters[0x1C] & 1)) {
            D_800CA2FC = 2;
        } else {
            D_800CA2FC = 3;
        }
        D_80122D08 = 0x1C;
    } else {
        if (!(game->girls[game->current_girl].scene_counters[0x1B] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        D_80122D08 = 0x1B;
    }
    choice_offset = D_800CA2FC * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 0xC) + date_class_table_stadium + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0xC) + date_class_table_stadium + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0xC) + date_class_table_stadium + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 0xC) + date_face_table_stadium + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0xC) + date_face_table_stadium + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0xC) + date_face_table_stadium + choice_offset) + (2));
    bg_read_sub2(0x45F8);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_movie_theater(void) {
    GameState *game;
    s32 choice_offset;
    u8 last_film;

    game = &game_state;

    talk_text_table = D_8015BDE0;
    talk_command_table = D_8015BE4C;
    talk_voice_table = D_8015BEB8;
    date_groups_per_variant = 0;
    if (game->year == 0x5F) {
        if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
            date_movie_index = 0;
        } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
            date_movie_index = 1;
        } else {
            date_movie_index = 2;
        }
    } else if (game->year == 0x60) {
        if ((game->month != 0) && ((u8)game->month < 3U)) {
            date_movie_index = 2;
        } else if (((u8) game->month >= 3U) && ((u8) game->month < 6U)) {
            date_movie_index = 3;
        } else if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
            date_movie_index = 4;
        } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
            date_movie_index = 5;
        } else {
            date_movie_index = 6;
        }
    } else {
        last_film = 0xA;
        if (game->year == 0x61) {
            if ((game->month != 0) && ((u8) game->month < 3U)) {
                date_movie_index = 6;
            } else if (((u8) game->month >= 3U) && ((u8) game->month < 6U)) {
                date_movie_index = 7;
            } else if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
                date_movie_index = 8;
            } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
                date_movie_index = 9;
            } else {
                date_movie_index = 0xA;
            }
        } else {
            date_movie_index = last_film;
        }
    }
    D_80122D08 = 0x23;
    strcpy(scene_place_title, "映画館");
    strcpy(scene_place_title_2, "館内");
    choice_offset = date_movie_index * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 0x21) + date_class_table_movie_theater + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x21) + date_class_table_movie_theater + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x21) + date_class_table_movie_theater + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 0x21) + date_face_table_movie_theater + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x21) + date_face_table_movie_theater + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x21) + date_face_table_movie_theater + choice_offset) + (2));
    bg_read_sub2(0x43F0);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_scene_reveal_start(void) {
    if (game_state.substep == 0) {
        if (D_800CA2F8 == 0xE) {
            audio_command(0x500);
        }
        game_state.substep = 1;
    }
    scene_bg_wipe_in();
}

void date_place_reload_steps(void) {
    switch (game_state.substep) {
    case 0:
        date_load_shared_script();
        game_state.mode_step -= 1;
        game_state.substep = 1;
        break;
    case 1:
    case 3:
    case 5:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 2:
        date_load_girl_script();
        game_state.mode_step -= 1;
        game_state.substep = 3;
        break;
    case 4:
        date_load_choice_tables();
        game_state.mode_step -= 1;
        game_state.substep = 5;
        break;
    case 6:
        if (D_800CA2F8 == 0xE) {
            audio_command(0x500);
        }
        (*(u8 *)&game_state.substep) += 1;
        break;
    case 7:
        normal_date_bg_fadein();
        break;
    }
}

void date_place_setup_concert_hall(void) {
    GameState *game;
    s32 choice_offset;
    u8 last_program;

    game = &game_state;

    talk_text_table = D_8015BDC8;
    talk_command_table = D_8015BE34;
    talk_voice_table = D_8015BEA0;
    date_groups_per_variant = 0;
    if (game->year == 0x5F) {
        if (((u8) game->month >= 4U) && ((u8) game->month < 6U)) {
            date_concert_index = 0;
        } else if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
            date_concert_index = 1;
        } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
            date_concert_index = 2;
        } else {
            date_concert_index = 3;
        }
    } else if (game->year == 0x60) {
        if ((game->month != 0) && ((u8)game->month < 3U)) {
            date_concert_index = 3;
        } else if (((u8) game->month >= 3U) && ((u8) game->month < 6U)) {
            date_concert_index = 4;
        } else if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
            date_concert_index = 5;
        } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
            date_concert_index = 6;
        } else {
            date_concert_index = 7;
        }
    } else {
        last_program = 0xB;
        if (game->year == 0x61) {
            if ((game->month != 0) && ((u8) game->month < 3U)) {
                date_concert_index = 7;
            } else if (((u8) game->month >= 3U) && ((u8) game->month < 6U)) {
                date_concert_index = 8;
            } else if (((u8) game->month >= 6U) && ((u8) game->month < 9U)) {
                date_concert_index = 9;
            } else if (((u8) game->month >= 9U) && ((u8) game->month < 0xCU)) {
                date_concert_index = 0xA;
            } else {
                date_concert_index = 0xB;
            }
        } else {
            date_concert_index = last_program;
        }
    }
    D_80122D08 = 0x24;
    strcpy(scene_place_title, "コンサート会場");
    strcpy(scene_place_title_2, "会場内");
    choice_offset = date_concert_index * 3;
    D_800CA22C[0] = (s16) *((game->current_girl * 0x24) + date_class_table_concert_hall + choice_offset);
    D_800CA22C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x24) + date_class_table_concert_hall + choice_offset) + (1));
    D_800CA22C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x24) + date_class_table_concert_hall + choice_offset) + (2));
    D_800CA21C[0] = (s16) *((game->current_girl * 0x24) + date_face_table_concert_hall + choice_offset);
    D_800CA21C[1] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x24) + date_face_table_concert_hall + choice_offset) + (1));
    D_800CA21C[2] = (s16) *(u8 *)((unsigned char *)((game->current_girl * 0x24) + date_face_table_concert_hall + choice_offset) + (2));
    bg_read_sub2(0x4466);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_pool(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing game->unk_F5F * 6 across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDE8;
    talk_command_table = D_8015BE54;
    talk_voice_table = D_8015BEC0;
    date_groups_per_variant = 3;
    strcpy(scene_place_title, "プール");
    strcpy(scene_place_title_2, "プール内");
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x1D] & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x1D;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_pool + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_pool + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_pool + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_pool + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_pool + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_pool + choice_offset + 2);
    bg_read_sub2(0x45BF);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_sea(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDF0;
    talk_command_table = D_8015BE5C;
    talk_voice_table = D_8015BEC8;
    date_groups_per_variant = 3;
    if (game_state.current_girl == GIRL_NOZOMI) {
        date_groups_per_variant = 0;
    }
    strcpy(scene_place_title, "海岸");
    strcpy(scene_place_title_2, "浜辺");
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x1E] & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
        if (game_state.current_girl == GIRL_YUKARI) {
            talk_group = 2;
        }
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x1E;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_sea + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_sea + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_sea + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_sea + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_sea + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_sea + choice_offset + 2);
    bg_read_sub2(0x45A3);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_shrine(void) {
    game_state.day_event_done_bits |= 0x10;
    talk_text_table = D_8015BDD0;
    talk_command_table = D_8015BE3C;
    talk_voice_table = D_8015BEA8;
    strcpy(scene_place_title, "縁日");
    strcpy(scene_place_title_2, "花火大会");
    date_groups_per_variant = 5;
    *D_800CA21C = 0;
    D_800CA21E = (u8)(get_g_zyotai_s(game_state.current_girl) & 0x7F);
    *D_800CA234 = 0;
    D_800CA236 = 0;
    D_800CA2FC = 0;
    D_80122D08 = 0x22;
    D_80122E9C = 0;
    bg_read_sub2(0x42CF);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_skating_rink(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing game->unk_F5F * 6 across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    talk_text_table = D_8015BDFC;
    talk_command_table = D_8015BE68;
    talk_voice_table = D_8015BED4;
    date_groups_per_variant = 3;
    strcpy(scene_place_title, "スケート場");
    strcpy(scene_place_title_2, "スケートリンク");
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x1F] & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x1F;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_skating_rink + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_skating_rink + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_skating_rink + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_skating_rink + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_skating_rink + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_skating_rink + choice_offset + 2);
    bg_read_sub2(0x456C);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_place_setup_ski_resort(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;
    u8 ski_visits;

    talk_text_table = D_8015BE00;
    talk_command_table = D_8015BE6C;
    talk_voice_table = D_8015BED8;
    date_groups_per_variant = 3;
    strcpy(scene_place_title, "スキー場");
    strcpy(scene_place_title_2, "ゲレンデ");
    ski_visits = game_state.girls[game_state.current_girl].scene_counters[0x20];
    if (!(ski_visits & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
    }
    if ((game_state.current_girl == GIRL_YUKARI) && (ski_visits != 0)) {
        D_800CA2FC = 1;
    }
    choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x20;
    *D_800CA22C = (s16) *(game->current_girl * 6 + date_class_table_ski_resort + choice_offset);
    D_800CA22C[1] = (s16) *(game->current_girl * 6 + date_class_table_ski_resort + choice_offset + 1);
    D_800CA22C[2] = (s16) *(game->current_girl * 6 + date_class_table_ski_resort + choice_offset + 2);
    *D_800CA21C = (s16) *(game->current_girl * 6 + date_face_table_ski_resort + choice_offset);
    D_800CA21C[1] = (s16) *(game->current_girl * 6 + date_face_table_ski_resort + choice_offset + 1);
    D_800CA21C[2] = (s16) *(game->current_girl * 6 + date_face_table_ski_resort + choice_offset + 2);
    bg_read_sub2(0x4558);
    event_log_add(1, D_80122D08);
    mode_step_next();
}

void date_choice_classes_derive(void) {
    s16 i;

    for (i = 0; i < 3; i++) {
        /* no temporary for the copied value: the store's address is then the first use of D_800CA224 (T-10400) */
        D_800CA224[i] = D_800CA22C[i];
        if (D_800CA224[i] == 0) {
            D_800CA224[i] = 1;
        }
        if (D_800CA224[i] == 4) {
            D_800CA224[i] = 3;
        }
        D_800CA234[i] = D_800CA22C[i] - 1;
        if (D_800CA234[i] == -1) {
            D_800CA234[i] = 0;
        }
        if (D_800CA234[i] == 3) {
            D_800CA234[i] = 2;
        }
    }
    mode_step_next();
}

void date_central_park_enter_spot(void) {
    if (D_800CA2FC == 0) {
        if ((game_state.month < 3) || (game_state.month >= 9)) {
            bg_read_sub2(0x4318);
        } else {
            bg_read_sub2(0x430F);
        }
        if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
            game_state.event_log_count--;
        }
        event_log_add(0, 0x25);
    } else if (D_800CA2FC == 1) {
        if ((game_state.month < 3) || (game_state.month >= 9)) {
            bg_read_sub2(0x4306);
        } else {
            bg_read_sub2(0x42FD);
        }
        if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
            game_state.event_log_count--;
        }
        event_log_add(0, 0x26);
    } else {
        bg_read_sub2(0x4322);
        if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
            game_state.event_log_count--;
        }
        event_log_add(0, 0x27);
    }
    mode_step_next();
}

void date_shopping_street_enter_shop(void) {
    GameState *game = &game_state;
    s32 choice_offset;
    u8 shop_visits;
    u8 fake_selector_read;

    /* FAKE: dead loads before the first branch let cvt_pass strip the CVTs of D_800CA2FC and game_state.current_girl so uopt keeps them in $a0 and $v1, and (u32)(s32) casts on the == 5 checks preserve constant-first operand order (`bne $a1, $v1`). T-24099 */
    fake_selector_read = D_800CA2FC;
    fake_selector_read = game_state.current_girl;
    date_shop_index = D_80122CDC;
    if (D_80122CDC == 0) {
        strcpy(scene_place_title_2, "ブティック");
        if (!(game_state.girls[game_state.current_girl].scene_counters[2] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        if ((game_state.current_girl == GIRL_SHIORI) || (game_state.current_girl == GIRL_MIRA)) {
            talk_group += D_800CA2FC * 6;
        } else {
            talk_group += D_800CA2FC * 2;
        }
        if ((game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_NOZOMI)) {
            talk_group += 1;
        }
        if ((u32)(s32)game_state.current_girl == GIRL_YUKARI) {
            talk_group += 2;
            if (talk_group) {}
        }
        D_80122D08 = 2;
        choice_offset = *(u8 *)&D_800CA2FC * 3;
        *D_800CA22C = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 1);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 2);
        *D_800CA21C = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 1);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 2);
        bg_read_sub2(0x4529);
    } else if (D_80122CDC == 1) {
        strcpy(scene_place_title_2, "ファンシーショップ");
        if (!(game_state.girls[game_state.current_girl].scene_counters[3] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        if (game_state.current_girl == GIRL_SHIORI) {
            talk_group += D_800CA2FC * 2 + 0xA;
        } else if (game_state.current_girl == GIRL_MIRA) {
            talk_group += D_800CA2FC * 2 + 8;
        } else if ((game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_MEGUMI)) {
            talk_group += D_800CA2FC * 2 + 6;
        } else if (game_state.current_girl == GIRL_YUMI) {
            talk_group += D_800CA2FC * 2 + 5;
        } else {
            talk_group += D_800CA2FC * 2 + 4;
        }
        if ((game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_NOZOMI)) {
            talk_group += 1;
        }
        if ((u32)(s32)game_state.current_girl == GIRL_YUKARI) {
            talk_group += 2;
            if (talk_group) {}
        }
        D_80122D08 = 3;
        choice_offset = *(u8 *)&D_800CA2FC * 3;
        *D_800CA22C = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 6);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 7);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 8);
        *D_800CA21C = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 6);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 7);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 8);
        bg_read_sub2(0x4532);
    } else {
        strcpy(scene_place_title_2, "ジャンク屋");
        shop_visits = game_state.girls[game_state.current_girl].scene_counters[4];
        if (!(shop_visits & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        /* FAKE: with D_800E71DF (`proof=date_shopping_street_enter_shop:94:56706346`, T-20820, T-24099) this emits the un-promoted lbu $t4 at 0x8013762C and matches byte-for-byte. */
        if ((D_800E71DF == GIRL_YUKARI) && (shop_visits != 0)) {
            D_800CA2FC = 1;
        }
        if (game_state.current_girl == GIRL_SHIORI) {
            talk_group += D_800CA2FC * 2 + 0xE;
        } else if (game_state.current_girl == GIRL_MIRA) {
            talk_group += D_800CA2FC * 2 + 0xC;
        } else if ((game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_MEGUMI)) {
            talk_group += D_800CA2FC * 2 + 0xA;
        } else if (game_state.current_girl == GIRL_YUMI) {
            talk_group += D_800CA2FC * 2 + 9;
        } else {
            talk_group += D_800CA2FC * 2 + 8;
        }
        if (game_state.current_girl == GIRL_NOZOMI) {
            talk_group += 1;
        } else if (((u32)(s32)game_state.current_girl == GIRL_YUKARI) && (D_800CA2FC == 1)) {
            talk_group += 1;
            if (talk_group) {}
        }
        D_80122D08 = 4;
        choice_offset = *(u8 *)&D_800CA2FC * 3;
        *D_800CA22C = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 12);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 13);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x12 + date_class_table_shopping_street + choice_offset + 14);
        *D_800CA21C = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 12);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 13);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x12 + date_face_table_shopping_street + choice_offset + 14);
        bg_read_sub2(0x453B);
    }
    date_choice_classes_derive();
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
}

void date_aquarium_enter_spot(void) {
    u8 log_count;

    log_count = game_state.event_log_count;
    /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
        game_state.event_log_count = log_count - 1;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x459A);
    mode_step_next();
}

void date_zoo_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x4587);
    mode_step_next();
}

void date_botanical_garden_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x45DC);
    mode_step_next();
}

void date_planetarium_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x45F0);
    mode_step_next();
}

void date_art_museum_enter_spot(void) {
    u8 log_count;

    log_count = game_state.event_log_count;
    /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
        game_state.event_log_count = log_count - 1;
    }
    event_log_add(0, D_80122D08);
    if (date_exhibit_artist_active != 0) {
        bg_read_sub2(0x4503);
    } else if (date_exhibit_sculpture_active != 0) {
        bg_read_sub2(0x450D);
    } else if (date_exhibit_painters_active != 0) {
        bg_read_sub2(0x4516);
    } else {
        bg_read_sub2(0x44FA);
    }
    mode_step_next();
}

void date_library_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x454E);
    mode_step_next();
}

void date_game_center_enter_spot(void) {
    u8 log_count;

    if (D_80122CDC == 0) {
        bg_read_sub2(0x44DF);
        log_count = game_state.event_log_count;
        /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
            game_state.event_log_count = log_count - 1;
        }
        event_log_add(0, 0x10);
    } else {
        bg_read_sub2(0x44E8);
        log_count = game_state.event_log_count;
        /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
            game_state.event_log_count = log_count - 1;
        }
        event_log_add(0, 0x11);
    }
    mode_step_next();
}

void date_bowling_alley_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x4627);
    mode_step_next();
}

void date_karaoke_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x461D);
    mode_step_next();
}

void date_amusement_park_load_background(void) {
    bg_read_sub2(0x4335);
    mode_step_next();
}

void date_amusement_park_enter_hero_show(void) {
    u8 show_repeat_variant;

    audio_command(0x207);
    strcpy(scene_place_title, "ヒーローショー");
    bg_read_sub2(0x4374);
    if (!(game_state.girls[game_state.current_girl].scene_counters[0x14] & 1)) {
        show_repeat_variant = 0;
    } else {
        show_repeat_variant = 1;
    }
    talk_group += show_repeat_variant * 2;
    /* FAKE: the store and the if on one source line; as1 schedules by line and then places
       the D_800CA2FC store after the record address as the original does. T-11580 */
    D_800CA2FC = show_repeat_variant; if ((game_state.event_log - 1)[game_state.event_log_count].scene == 1) {
        game_state.event_log_count -= 1;
    }
    /* FAKE: an element view of the scalar makes uopt forward the stored 0x14 to the argument,
       so one `li a1,20` serves the store and the call as in the original. T-11580 */
    (&D_80122D08)[0] = 0x14;
    event_log_add(0, (&D_80122D08)[0]);
    mode_step_next();
}

s32 date_amusement_park_choose_ride(void) {
    /* No return statement: $v0 is live at the exit, so uopt keeps D_80122CDC in $v0 (T-17202). */
    s32 background_resource;

    if (D_80122CDC == 0) {
        if ((date_park_newest_rides != 0) || (date_park_new_rides != 0)) {
            background_resource = 0x4359;
            if (date_ride_chosen == 0) {
                date_ride_index = DATE_RIDE_SCREAM_MACHINE;
            }
        } else {
            background_resource = 0x433E;
            if (date_ride_chosen == 0) {
                date_ride_index = DATE_RIDE_ROLLER_COASTER;
            }
        }
    } else if (D_80122CDC == 1) {
        talk_group += 5;
        if (date_park_newest_rides != 0) {
            background_resource = 0x4362;
            if (date_ride_chosen == 0) {
                date_ride_index = DATE_RIDE_VIRTUAL_SHIP;
            }
        } else {
            background_resource = 0x4347;
            if (date_ride_chosen == 0) {
                date_ride_index = DATE_RIDE_GHOST_HOUSE;
            }
        }
    } else {
        talk_group += 0xA;
        background_resource = 0x4350;
        if (date_ride_chosen == 0) {
            date_ride_index = DATE_RIDE_FERRIS_WHEEL;
        }
    }
    date_ride_chosen = 1;
    bg_read_sub2(background_resource);
    date_amusement_park_ride_setup();
    if ((game_state.event_log - 1)[game_state.event_log_count].scene == 1) {
        game_state.event_log_count -= 1;
    }
    event_log_add(0, D_80122D08);
    date_choice_classes_derive();
    if ((game_state.current_girl == GIRL_YUKO) && ((u8)date_ride_index == DATE_RIDE_SCREAM_MACHINE) && (game_state.girls[8].scene_counters[0x18] != 0)) {
        D_80122CDC = 1;
    } else {
        D_80122CDC = 0;
    }
}

void date_amusement_park_enter_night_parade(void) {
    strcpy(scene_place_title_2, "ナイトパレード");
    audio_command(0x204);
    bg_read_sub2(0x436B);
    game_state.event_log_count -= 1;
    event_log_add(0, 0x1A);
    D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 1;
    D_800CA234[0] = D_800CA234[1] = D_800CA234[2] = 0;
    if (game_state.current_girl == GIRL_YUINA) {
        D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;
        D_800CA234[0] = D_800CA234[1] = D_800CA234[2] = 2;
    }
    mode_step_next();
}

void date_amusement_park_enter_main_area(void) {
    bg_read_sub2(0x432B);
    if (date_show_taken != 0) {
        audio_command(0x200);
    }
    strcpy(scene_place_title_2, "遊園地");
    mode_step_next();
}

void date_stadium_enter_spot(void) {
    u8 log_count;

    if (D_800CA2FC < 2) {
        audio_command(0x200);
        bg_read_sub2(0x4601);
        log_count = game_state.event_log_count;
        /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
        if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
            game_state.event_log_count = log_count - 1;
        }
        event_log_add(0, D_80122D08);
    } else {
        audio_command(0x201);
        bg_read_sub2(0x460A);
        log_count = game_state.event_log_count;
        /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
        if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
            game_state.event_log_count = log_count - 1;
        }
        event_log_add(0, D_80122D08);
    }
    mode_step_next();
}

void date_movie_theater_enter_spot(void) {
    u8 log_count;

    log_count = game_state.event_log_count;
    /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
        game_state.event_log_count = log_count - 1;
    }
    event_log_add(0, D_80122D08);
    audio_command(0x1500);
    bg_read_sub2(0x43F9);
    mode_step_next();
}

void date_movie_theater_load_poster(void) {
    switch (date_movie_index) {
    case 0:
        bg_read_sub2(0x4402);
        break;
    case 1:
        bg_read_sub2(0x440B);
        break;
    case 2:
        bg_read_sub2(0x4414);
        break;
    case 3:
        bg_read_sub2(0x441C);
        break;
    case 4:
        bg_read_sub2(0x4426);
        break;
    case 5:
        bg_read_sub2(0x442F);
        break;
    case 6:
        bg_read_sub2(0x4453);
        break;
    case 7:
        bg_read_sub2(0x4440);
        break;
    case 8:
        bg_read_sub2(0x4449);
        break;
    case 9:
        bg_read_sub2(0x445D);
        break;
    case 10:
        bg_read_sub2(0x4437);
        break;
    }
    mode_step_next();
}

void date_movie_theater_start_film(void) {
    DateProgramAudioRequest *choice;
    DateFilmAudioRequests choices;

    choices = date_movie_audio_pairs;
    normal_date_bg_fadein();
    if (game_state.substep++ == 0) {
        audio_command(0xBF);
        choice = &choices.rows[date_movie_index];
        cd_audio_request_set(choice->packed_clip_start, choice->packed_clip_end, 0xF);
        audio_command(0x300);
    }
}

void date_movie_theater_narration_talk(void) {
    if (game_state.mode_step_timer.w++ == 0) {
        game_state.current_girl = GIRL_SOTOI;
    } else {
        game_state.current_girl = game_state.date_girl;
    }
    normal_date_speak();
}

void date_movie_theater_fade_out(void) {
    s32 fake_step_copy[2]; /* FAKE: the original keeps a 4-byte local at sp+0x28; only the first word is written (T-6050) */

    fake_step_copy[0] = game_state.mode_step;
    normal_date_bg_fadeout();
}

void date_movie_theater_load_screen_background(void) {
    audio_command(0x500);
    bg_read_sub2(0x43F0);
    mode_step_next();
}

void date_concert_hall_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x4470);
    mode_step_next();
}

void date_concert_hall_load_program_background(void) {
    switch (date_concert_index) {
    case 0:
        bg_read_sub2(0x44C4);
        break;
    case 1:
        bg_read_sub2(0x4478);
        break;
    case 2:
        bg_read_sub2(0x449E);
        break;
    case 3:
        bg_read_sub2(0x44B0);
        break;
    case 4:
        bg_read_sub2(0x4482);
        break;
    case 5:
        bg_read_sub2(0x4494);
        break;
    case 6:
        bg_read_sub2(0x44C4);
        break;
    case 7:
        bg_read_sub2(0x44A7);
        break;
    case 8:
        bg_read_sub2(0x448B);
        break;
    case 9:
        bg_read_sub2(0x44C4);
        break;
    case 10:
        bg_read_sub2(0x44BA);
        break;
    case 11:
        bg_read_sub2(0x44CD);
        break;
    }
    mode_step_next();
}

void date_concert_hall_start_program(void) {
    DateProgramAudioRequest *choice;
    DateConcertAudioRequests choices;

    choices = date_concert_audio_pairs;
    normal_date_bg_fadein();
    if (game_state.substep++ == 0) {
        audio_command(0xBF);
        choice = &choices.rows[date_concert_index];
        cd_audio_request_set(choice->packed_clip_start, choice->packed_clip_end, 0xF);
        audio_command(0x300);
    }
}

s32 date_concert_hall_wait_then_background(void) {
    s32 elapsed_frames;

    elapsed_frames = game_state.mode_step_timer.w + 1;
    game_state.mode_step_timer.w = elapsed_frames;
    if ((u32) elapsed_frames < 0x20U) {
        return 0;
    }
    bg_read_sub2(0x4466);
    mode_step_next();
}

void date_pool_enter_spot(void) {
    u8 log_count;

    bg_read_sub2(0x45C9);
    log_count = game_state.event_log_count;
    /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
        game_state.event_log_count = log_count - 1;
    }
    event_log_add(0, D_80122D08);
    mode_step_next();
}

void date_sea_enter_spot(void) {
    bg_read_sub2(0x45AC);
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    mode_step_next();
}

void date_sea_load_background(void) {
    bg_read_sub2(0x45A3);
    mode_step_next();
}

void date_sea_enter_deserted_island(void) {
    u8 log_count;

    strcpy(scene_place_title_2, "無人島");
    bg_read_sub2(0x45B6);
    log_count = game_state.event_log_count;
    /* FAKE: 8-byte records at GameState+0x758 are not a declared aggregate yet; byte read through the base. T-8070 */
    if (((u8 *)&game_state)[0x758 + log_count * 8] == 1) {
        game_state.event_log_count = log_count - 1;
    }
    event_log_add(0, 0x28);
    mode_step_next();
}

void date_skating_rink_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x4574);
    mode_step_next();
}

void date_ski_resort_enter_spot(void) {
    if (game_state.event_log[game_state.event_log_count - 1].scene == 1) {
        game_state.event_log_count--;
    }
    event_log_add(0, D_80122D08);
    bg_read_sub2(0x4562);
    mode_step_next();
}

void date_shrine_load_fireworks_data(void) {
    cd_read_sectors(0x86, 0x801B0000, 0xBC35);
    audio_command(0x200);
    date_fireworks_resources_init();
    mode_step_next();
}

void date_amusement_park_ride_setup(void) {
    GameState *game = &game_state;
    s32 scream_choice_offset;
    s32 coaster_choice_offset;
    s32 ship_choice_offset;
    s32 ghost_choice_offset;
    s32 wheel_choice_offset;

    strcpy(scene_place_title_2, "遊園地");
    if ((u8) date_ride_index == DATE_RIDE_ROLLER_COASTER) {
        strcpy(scene_place_title, "ジェットコースター");
        if (!(game->girls[game->current_girl].scene_counters[21] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        coaster_choice_offset = D_800CA2FC * 3;
        *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + coaster_choice_offset);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + coaster_choice_offset + 1);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + coaster_choice_offset + 2);
        *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + coaster_choice_offset);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + coaster_choice_offset + 1);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + coaster_choice_offset + 2);
        D_80122D08 = 0x15;
        return;
    }
    if ((u8) date_ride_index == DATE_RIDE_SCREAM_MACHINE) {
        strcpy(scene_place_title, "絶叫マシーンビビール");
        if (!(game->girls[game->current_girl].scene_counters[24] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        scream_choice_offset = D_800CA2FC * 3;
        D_80122D08 = 0x18;
        *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + scream_choice_offset + 18);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + scream_choice_offset + 19);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + scream_choice_offset + 20);
        *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + scream_choice_offset + 18);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + scream_choice_offset + 19);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + scream_choice_offset + 20);
        return;
    }
    if ((u8) date_ride_index == DATE_RIDE_GHOST_HOUSE) {
        strcpy(scene_place_title, "ゴーストハウス");
        if (!(game->girls[game->current_girl].scene_counters[22] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        ghost_choice_offset = D_800CA2FC * 3;
        D_80122D08 = 0x16;
        *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ghost_choice_offset + 6);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ghost_choice_offset + 7);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ghost_choice_offset + 8);
        *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ghost_choice_offset + 6);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ghost_choice_offset + 7);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ghost_choice_offset + 8);
        return;
    }
    if ((u8) date_ride_index == DATE_RIDE_VIRTUAL_SHIP) {
        strcpy(scene_place_title, "バーチャルシップ");
        if (!(game->girls[game->current_girl].scene_counters[25] & 1)) {
            D_800CA2FC = 0;
        } else {
            D_800CA2FC = 1;
        }
        ship_choice_offset = D_800CA2FC * 3;
        D_80122D08 = 0x19;
        *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ship_choice_offset + 24);
        D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ship_choice_offset + 25);
        D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + ship_choice_offset + 26);
        *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ship_choice_offset + 24);
        D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ship_choice_offset + 25);
        D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + ship_choice_offset + 26);
        return;
    }
    strcpy(scene_place_title, "観覧車");
    if (!(game->girls[game->current_girl].scene_counters[23] & 1)) {
        D_800CA2FC = 0;
    } else {
        D_800CA2FC = 1;
    }
    wheel_choice_offset = D_800CA2FC * 3;
    D_80122D08 = 0x17;
    *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + wheel_choice_offset + 12);
    D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + wheel_choice_offset + 13);
    D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + wheel_choice_offset + 14);
    *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + wheel_choice_offset + 12);
    D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + wheel_choice_offset + 13);
    D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + wheel_choice_offset + 14);
}

void date_game_center_skip_medals(void) {
    if (date_game_center_medals_open == 0) {
        talk_group += 1;
        mode_step_next();
        mode_step_next();
        D_80122CDC = 0;
    }
    mode_step_next();
}

s32 date_shopping_street_skip_extra_talk(void) {
    if ((game_state.current_girl == GIRL_SHIORI) && (talk_group == 0xA) && (D_80122CDC == 0)) {
        mode_step_next();
        return 0;
    }
    if ((game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_MEGUMI)) {
        if ((talk_group == 6) && (D_80122CDC == 0)) {
            mode_step_next();
            return 0;
        }
    }
    if ((GIRL_YUMI == game_state.current_girl) && (talk_group == 6) && (D_80122CDC == 0)) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        return 0;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void date_shopping_street_boutique_group_bonus(void) {
    u8 girl_state_flags;

    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    if ((D_80122CDC == 0) && ((game_state.current_girl == GIRL_SHIORI) || (game_state.current_girl == GIRL_MIRA)) && (D_800CA2FC == 0)) {
        talk_group = talk_group + ((girl_state_flags & 0x7F) >= 2U) + ((girl_state_flags & 0x7F) >= 3U);
    } else {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

void date_shopping_street_group_adjust(void) {
    u32 girl_state;

    girl_state = (u8) get_g_zyotai_s(game_state.current_girl) & 0x7F;
    talk_group = ((talk_group - (girl_state >= 2U)) - (girl_state >= 3U)) + 2;
    mode_step_next();
}

void date_shopping_street_empty_step(void) {
    mode_step_next();
}

void date_planetarium_low_science_group_bonus(void) {
    if (game_state.param_science.value < 0x78) {
        talk_group += 2;
    }
    mode_step_next();
}

void date_art_museum_seen_exhibit_score(void) {
    u32 girl_state;

    girl_state = (u8) get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (D_800CA2FC == 5) {
        talk_group = talk_group + (girl_state >= 2U) + (girl_state >= 3U);
        date_choice_score();
        D_80122CDC = 0;
        return;
    }
    mode_step_next();
}

void date_art_museum_choice_face(void) {
    portrait_layout_set(D_800CA21C[D_80122CDC]);
    mode_step_next();
}

void date_art_museum_seen_exhibit_skip(void) {
    if (D_800CA2FC == 5) {
        mode_step_next();
        game_state.mode_step += 9;
        return;
    }
    mode_step_next();
}

void date_game_center_enter_medal_corner(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing the girl-row multiply across stores
       (same idiom as date_amusement_park_ride_extra_tables). T-11580 */
    GameState *game = &game_state;
    s32 choice_offset;

    if (D_80122CDC != 0) {
        if (!(game_state.girls[game_state.current_girl].scene_counters[0x11] & 1)) {
            D_800CA2FC = 2;
            strcpy(scene_place_title_2, "メダルゲームコーナー");
        } else {
            D_800CA2FC = 3;
            strcpy(scene_place_title_2, "メダルゲームコーナー");
        }
        choice_offset = D_800CA2FC * 3;
        D_80122D08 = 0x11;
        *D_800CA22C = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset);
        D_800CA22C[1] = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset + 1);
        D_800CA22C[2] = (s16) *(game->current_girl * 0xC + date_class_table_game_center + choice_offset + 2);
        *D_800CA21C = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset);
        D_800CA21C[1] = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset + 1);
        D_800CA21C[2] = (s16) *(game->current_girl * 0xC + date_face_table_game_center + choice_offset + 2);
    }
    talk_group += D_800CA2FC * 2;
    /* FAKE: the byte view keeps uopt from CSEing the two D_800CA2FC reads, so ugen reuses
       its own load as the original does. T-11580 */
    if ((game_state.current_girl == GIRL_MIRA) && (*(u8 *)&D_800CA2FC >= 2)) {
        talk_group += 3;
    }
    mode_step_next();
}

void date_amusement_park_skip_show(void) {
    u8 girl_id;

    if (date_first_sunday == 0) {
        girl_id = game_state.current_girl;
        talk_group = 9;
        if (girl_id == 7) {
            talk_group = 0xA;
        } else if (girl_id == 9) {
            talk_group = 8;
        } else if ((girl_id == 4) || (girl_id == 8) || (girl_id == 5) || (girl_id == 0xA)) {
            talk_group = 7;
        }
        game_state.mode_step += 0x14;
    }
    mode_step_next();
}

s32 date_amusement_park_show_first_answer(void) {
    /* FAKE: unused byte local puts previous_step at 0x28(sp) as in the original. T-11580 */
    u8 fake_padding;
    s32 previous_step;

    if (D_80122CDC == 1) {
        /* FAKE: evaluates the F5F byte into $v0 before talk_group = 9 (EVENT twin func_80116940, T-11110). T-17202 */
        if (game_state.current_girl + 0) {
        }
        talk_group = 9;
        if (game_state.current_girl == GIRL_MIRA) {
            talk_group = 0xA;
        } else if (game_state.current_girl == GIRL_MEGUMI) {
            talk_group = 8;
        } else if ((game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_YUKARI) || (game_state.current_girl == GIRL_YUMI)) {
            talk_group = 7;
        }
        game_state.mode_step += 0x12;
        return;
    }
    if ((game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_YUKARI) || (game_state.current_girl == GIRL_YUMI)) {
        date_show_taken = 1;
        game_state.mode_step += 6;
        return;
    }
    if ((game_state.current_girl == GIRL_MEGUMI) && (game_state.scene_flags[0x3E] == 0)) {
        date_show_taken = 1;
        talk_group += 1;
        game_state.mode_step += 6;
        return;
    }
    if ((game_state.current_girl == GIRL_MEGUMI) && (game_state.scene_flags[0x3E] != 0)) {
        if (game_state.substep == 0) {
            portrait_layout_set(3);
            game_state.substep = 1;
        }
        previous_step = game_state.mode_step;
        normal_date_speak();
        if (previous_step != game_state.mode_step) {
            talk_group = 8;
            /* FAKE: T-17202, the element view keeps the new byte in a register (andi t8; move v0,t8)
             * for $v0, which is live at the exit of this s32 function with no return value */
            (&game_state.mode_step)[0] += 0x11;
        }
        /* no value on purpose: `return unk_110A` splits the byte into two ranges (T-17202) */
        return;
    }
    mode_step_next();
}

s32 date_amusement_park_show_second_answer(void) {
    /* FAKE: 4 spare bytes reproduce previous_step's stack home at 0x2A(sp). T-11140 */
    s32 fake_padding;
    s16 previous_step;

    if (D_80122CDC == 1) {
        if (game_state.current_girl == GIRL_MIRA) {
            previous_step = game_state.mode_step;
            normal_date_speak();
            if (previous_step != game_state.mode_step) {
                talk_group = 0xA;
                game_state.mode_step += 0xC;
            }
            return 0;
        }
        talk_group = 9;
        if (game_state.current_girl == GIRL_MEGUMI) {
            talk_group = 8;
        } else if ((game_state.current_girl == GIRL_SAKI) || (game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_YUKARI)) {
            talk_group = 7;
        }
        /* FAKE: dead test gives the unk_110A byte a second use in this block so uopt allocates $v0
           (EVENT twin func_80116AA8, T-11110). T-17202 */
        if ((s32)game_state.mode_step) {
        }
        game_state.mode_step += 0xC;
    } else {
        /* FAKE: dead test gives the F5F byte a second use so uopt allocates $v0 for it. T-17202 */
        if ((s32)game_state.current_girl) {
        }
        if (game_state.current_girl == GIRL_MIRA) {
            talk_group += 1;
        }
        date_show_taken = 1;
    }
    mode_step_next();
}

void date_amusement_park_ride_extra_tables(void) {
    /* FAKE: pointer view and byte-pointer indexing prevent uopt from CSEing game->unk_F5F * 0x24 across stores. T-11140 */
    GameState *game = &game_state;
    s32 choice_offset;

    mode_step_next();
    game_state.mode_step += 0x16;
    choice_offset = D_800CA2FC * 3;
    *D_800CA22C = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + choice_offset + 30);
    D_800CA22C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + choice_offset + 31);
    D_800CA22C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_class_table_amusement_park + choice_offset + 32);
    *D_800CA21C = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + choice_offset + 30);
    D_800CA21C[1] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + choice_offset + 31);
    D_800CA21C[2] = (s16) *(game->current_girl * 0x24 + (u8 *)date_face_table_amusement_park + choice_offset + 32);
    date_choice_classes_derive();
}

void date_amusement_park_group_bonus(void) {
    if (date_park_new_rides != 0) {
        talk_group += 0x10;
    }
    if (date_park_newest_rides != 0) {
        talk_group += 0x10;
    }
    mode_step_next();
}

void date_amusement_park_parade_prepare(void) {
    if (D_80122CDC == 1) {
        D_80122CDC = D_80122CEC;
        date_parade_offered = 0;
        mode_step_next();
        game_state.mode_step += 0xC;
    } else {
        if (game_state.mode_step_timer.w++ == 0) {
            game_state.day_event_done_bits |= 0x10;
            D_80122D34 = 1;
            if (game_state.girls[game_state.current_girl].scene_counters[26] != 0) {
                date_park_parade_group += 1;
            }
            cd_read_sectors(0x1DU, 0x801B0000, 0xBD41);
        }
        if (cd_read_sync_status() & 1) {
            date_parade_resources_init();
            load_palette((s32) D_8015B644, 0x11, 1, 3, 0);
            actor_clear(0x60);
            D_80120651 = 0xC;
            D_80120652 = 0x40;
            D_80120688 = 0x41000000;
            D_80120653 = 4;
            D_8012065C = D_8015B638;
            D_80120660 = D_8015B63C;
            D_80120684 = D_8015B634;
            D_80120664 = D_8015B640;
            D_80120666 = 0;
            D_80120668 = 0;
            D_80120656 = 1;
            D_80120693 = 0x11;
            D_80120676 = -0xA0;
            D_8012067A = -0x78;
            D_80120657 = 0x80;
            date_sign_palette_frame = 0;
            mode_step_next();
        }
    }
}

void date_amusement_park_parade_visit_count(void) {
    if (game_state.girls[game_state.current_girl].scene_counters[0x1A] == 0) {
        date_park_parade_group += 1;
    }
    mode_step_next();
}

void date_amusement_park_parade_mark_visit(void) {
    game_state.girls[game_state.current_girl].scene_counters[0x1A] += 1;
    mode_step_next();
    game_state.mode_step += 7;
}

void date_amusement_park_group_by_variant(void) {
    talk_group += D_800CA2FC * 2;
    mode_step_next();
}

void date_amusement_park_title(void) {
    strcpy(scene_place_title_2, "遊園地内");
    mode_step_next();
}

void date_movie_theater_group_by_class(void) {
    switch (game_state.current_girl) {
    case GIRL_SAKI:
        if (*D_800CA22C == 3) {
            talk_group += 1;
        } else if (*D_800CA22C == 2) {
            talk_group += 2;
        }
        break;
    case GIRL_MEGUMI:
        if (*D_800CA22C == 3) {
            if (D_800CA22C[1] == 3) {
                talk_group += 2;
            } else {
                talk_group += 1;
            }
        }
        break;
    case GIRL_YUKARI:
    case GIRL_NOZOMI:
        if (*D_800CA22C == 4) {
            talk_group += 1;
        } else if (*D_800CA22C == 3) {
            talk_group += 2;
        }
        break;
    default:
        if (*D_800CA22C == 4) {
            talk_group += 1;
        } else if (*D_800CA22C == 2) {
            talk_group += 2;
        }
        break;
    }
    date_choice_classes_derive();
}

void date_concert_hall_group_by_class(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
    case GIRL_MIO:
    case GIRL_MIRA:
        if (*D_800CA22C == 4) {
            talk_group += 1;
        } else if (*D_800CA22C == 2) {
            talk_group += 2;
        }
        break;
    case GIRL_SAKI:
    case GIRL_NOZOMI:
    case GIRL_MEGUMI:
        if ((*D_800CA22C == 3) && (D_800CA22C[1] == 2)) {
            talk_group += 1;
        } else if ((*D_800CA22C == 3) && (D_800CA22C[1] == 3)) {
            talk_group += 2;
        }
        break;
    case GIRL_YUKARI:
        if (*D_800CA22C == 3) {
            talk_group += 1;
        } else if (*D_800CA22C == 2) {
            talk_group += 2;
        }
        break;
    default:
        if (*D_800CA22C == 4) {
            talk_group += 1;
        } else if (*D_800CA22C == 3) {
            talk_group += 2;
        }
        break;
    }
    date_choice_classes_derive();
}

void date_shrine_festival_branch(void) {
    if (game_state.year == 0x5F) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKARI) || (game_state.current_girl == GIRL_YUKO) ||
            (GIRL_YUMI == game_state.current_girl)) {
            talk_group = 1;
        }
        game_state.girls[game_state.current_girl].tokimeki += 1;
        mode_step_next();
    } else if (game_state.year == 0x60) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKARI) || (game_state.current_girl == GIRL_YUKO) ||
            (GIRL_YUMI == game_state.current_girl)) {
            talk_group = 4;
        } else {
            talk_group = 6;
        }
        mode_step_set(0x14);
    } else {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKARI) || (game_state.current_girl == GIRL_YUKO) ||
            (GIRL_YUMI == game_state.current_girl)) {
            talk_group = 0xA;
        } else {
            talk_group = 0xC;
        }
        game_state.girls[game_state.current_girl].tokimeki += 1;
        mode_step_set(0x1F);
    }
    check_para_limit();
}

void date_shrine_jump_to_farewell(void) {
    mode_step_set(0x32);
}

void date_shrine_choice_tables_by_girl(void) {
    s32 girl_choice_offset;
    u8 *response_classes;
    u8 *response_faces;

    girl_choice_offset = game_state.current_girl * 3;
    response_classes = girl_choice_offset + date_class_table_shrine;
    D_800CA22C[0] = response_classes[0];
    D_800CA22C[1] = response_classes[1];
    D_800CA22C[2] = response_classes[2];
    response_faces = girl_choice_offset + date_face_table_shrine;
    D_800CA21C[0] = response_faces[0];
    D_800CA21C[1] = response_faces[1];
    D_800CA21C[2] = response_faces[2];
    date_choice_classes_derive();
}

void date_shrine_start_festival_stalls(void) {
    actor_clear_range(0);
    hud_cursor_show(0);
    k_reset(0);
    game_state.saved_game_mode = game_state.game_mode;
    game_state.saved_mode_stage = game_state.mode_stage;
    game_state.saved_mode_step = 0x3A;
    game_mode_set(GAME_MODE_FESTIVAL_STALLS);
}

/* FAKE: `two`/`three` hold the compare constants; they give the original's constant registers and bne operand order (found with the permuter, T-9380) */
void date_shrine_result_face(void) {
    s32 result_and_yuina_id;
    s32 tokimeki_bonus_result;

    if (game_state.year == 0x5F) {
        portrait_layout_set(1);
    } else if (D_80122E9C == 0) {
        portrait_layout_set(1);
    } else if (D_80122E9C == 1) {
        portrait_layout_set(2);
    } else {
        result_and_yuina_id = 2;
        tokimeki_bonus_result = 3;
        if (D_80122E9C == result_and_yuina_id) {
            portrait_layout_set(3);
        } else if ((D_80122E9C == tokimeki_bonus_result) && (game_state.current_girl != result_and_yuina_id)) {
            portrait_layout_set(0);
        } else if (D_80122E9C == tokimeki_bonus_result) {
            portrait_layout_set(1);
        } else {
            portrait_layout_set(3);
        }
    }
    mode_step_next();
}

void date_shrine_after_stalls_jump(void) {
    if (game_state.year == 0x5F) {
        mode_step_set(0xC);
        return;
    }
    mode_step_set(0x17);
}

void date_shrine_return_setup(void) {
    s32 fake_padding; /* FAKE: unused slot above rect, the original frame has it (real source unknown). T-8070 */
    RECT rect;
    u8 saved_spot;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    saved_spot = D_80122D08;
    talk_clear();
    D_80122D08 = saved_spot;
    k_reset(1);
    rect.x = 0x140;
    rect.y = 0x1A0;
    rect.w = 0x140;
    rect.h = 0x58;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0xA0;
    func_8009C7F8(&rect, 8, 0, 0);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_names_set(game_state.current_girl);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    audio_command(0x205);
    D_800CA21C[0] = 1;
    D_800CA21C[2] = 1;
    D_800CA21C[1] = 1;
    D_800CA224[0] = 1;
    D_800CA224[2] = 1;
    D_800CA224[1] = 1;
    D_800CA22C[0] = 1;
    D_800CA22C[2] = 1;
    D_800CA22C[1] = 1;
    *D_800CA234 = 0;
    D_800CA234[2] = 0;
    D_800CA234[1] = 0;
    D_80122CDC = 0;
    bg_read_sub2(0x42CF);
    mode_step_next();
}

void date_shrine_stall_reward(void) {
    talk_group += D_80122E9C;
    switch (D_80122E9C) {
    case 0:
        game_state.girls[game_state.current_girl].tokimeki += 1;
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.param_stress.value -= 0xA;
        portrait_layout_set(1);
        break;
    case 1:
        game_state.girls[game_state.current_girl].affection += 1;
        portrait_layout_set(1);
        break;
    case 2:
        game_state.girls[game_state.current_girl].tokimeki -= 1;
        portrait_layout_set(2);
        break;
    case 3:
        game_state.girls[game_state.current_girl].tokimeki += 5;
        if (game_state.current_girl != GIRL_YUINA) {
            portrait_layout_set(0);
        } else {
            portrait_layout_set(1);
        }
        break;
    case 4:
        portrait_layout_set(2);
        break;
    }
    check_para_limit();
    mode_step_next();
}

void date_amusement_park_parade_offer(void) {
    D_80122CEC = D_80122CDC;
    if (game_state.month == 8) {
        date_parade_offered = 1;
        date_park_parade_group = 0;
        date_park_parade_line = 0;
        portrait_layout_set(get_g_zyotai_h(game_state.current_girl));
        mode_step_next();
        return;
    }
    mode_step_next();
    game_state.mode_step += 0x10;
}

void date_neighborhood_park_start(void) {
    u8 girl_state_flags;

    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    talk_group = (((girl_state_flags & 0x7F) >= 2U) + ((girl_state_flags & 0x7F) >= 3U)) * 2;
    talk_line = 0;
    game_state.girls[game_state.current_girl].tokimeki += 1;
    game_state.girls[game_state.current_girl].affection += 1;
    check_para_limit();
    mode_step_next();
}

/* FAKE: 1-element array indexing on unk_03F/unk_03E produces uixa instead of direct isvar so uopt keeps the global live range split and register coloring in bb=5..18 matching the original; s16 pointer cast on counts_b[3] emits direct halfword load/store; zero-multiplied expressions keep uopt expression order in the birthday comparison blocks; single-line else-if preserves Uloc line deltas for as1 scheduling at row 26. T-24083 */
void date_neighborhood_park_pick_greeting(void) {
    u32 month_diff;
    u8 girl_state;
    DateGreetingCounts first_greeting_counts;
    DateGreetingCounts second_greeting_counts;
    u8 first_status_match;
    s32 alternate_girl_state;

    first_greeting_counts = date_opening_count_a;
    second_greeting_counts = date_opening_count_b;
    if ((game_state.year == 0x61) && ((&game_state.month)[0] >= 6U)) {
        ((s16 *)&second_greeting_counts)[3] += 1;
    } else if (game_state.year == 0x62) { ((s16 *)&second_greeting_counts)[3] += 1; }
    date_opening_group = D_80122CDC;
    date_opening_line = (u32)game_state.frame_count % (u32)(((D_80122CDC == 0) * first_greeting_counts.value[game_state.current_girl]) + (second_greeting_counts.value[game_state.current_girl] * (D_80122CDC == 1)));
    date_birthday_near = 0;
    first_status_match = 0;
    if ((&game_state.month)[0] == game_state.girls[game_state.current_girl].birthday.date.month) {
        if (game_state.girls[game_state.current_girl].birthday.u * 0 + game_state.day < game_state.girls[game_state.current_girl].birthday.date.day) {
            date_birthday_near = 1;
        }
    } else {
        month_diff = ((&game_state.month)[0] == 2U) * 0 + (game_state.girls[game_state.current_girl].birthday.date.month - (&game_state.month)[0]) + 12;
        if ((month_diff % 12) == 1) {
            date_birthday_near = 1;
        }
    }
    date_hint_roll = 0;
    if ((((game_state.year == 0x61) && ((&game_state.month)[0] >= 3U)) || (game_state.year == 0x62)) && (rand() % 2)) {
        date_hint_roll = 1;
    }
    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    alternate_girl_state = get_g_zyotai_h(game_state.current_girl);
    if ((girl_state == 0) && ((alternate_girl_state & 0xFF) == 0)) {
        if (!game_state.girls[game_state.current_girl].flags.f.b5) {
            game_state.girls[game_state.current_girl].flags.f.b5 = 1;
            first_status_match = 1;
        }
    }
    if ((first_status_match != 0) && (D_80122CDC == 0)) {
        date_opening_group = 4;
        date_opening_line = 0;
    } else if ((date_birthday_near != 0) && (D_80122CDC == 0)) {
        date_opening_group = 2;
        date_opening_line = game_state.year - 0x5F;
        if (((game_state.girls[game_state.current_girl].birthday.date.month == 1) && ((&game_state.month)[0] == 1)) ||
            ((game_state.girls[game_state.current_girl].birthday.date.month == 2) && (((&game_state.month)[0] == 1) || ((&game_state.month)[0] == 2))) ||
            ((game_state.girls[game_state.current_girl].birthday.date.month == 3) && (((&game_state.month)[0] == 1) || ((&game_state.month)[0] == 2) || ((&game_state.month)[0] == 3)))) {
            date_opening_line = (&game_state.year)[0] - 0x60;
        }
        if (game_state.current_girl == GIRL_NOZOMI) {
            if ((girl_state < 2U) && ((alternate_girl_state & 0xFF) < 2U)) {
                date_opening_line = 1;
            } else {
                date_opening_line = 0;
            }
        }
    } else if ((date_hint_roll != 0) && (D_80122CDC == 0)) {
        date_opening_group = 3;
        date_opening_line = 0;
    }
    if ((game_state.current_girl == GIRL_YUKARI) && (date_opening_group == 0) && (date_opening_line == 3)) {
        game_state.girls[5].flags.f.b14 = 1;
    }
    mode_step_next();
}

s32 date_arrival_late_gate(void) {
    if ((date_arrival != DATE_ARRIVAL_WAIT_GREETING) || (date_player_waited == 1)) {
        mode_step_next();
        mode_step_next();
        return 0;
    }
    return date_arrival_greeting_speak();
}

s32 date_arrival_greeting_speak(void) {
    u8 girl_state_flags;
    u32 girl_state;

    talk_tables = (u8 *) &date_arrival_group;
    D_800CA138 = (u8 *) &date_arrival_line;
    D_800CA13C = D_8015BE1C;
    D_800CA140 = D_8015BE88;
    D_800CA144 = D_8015BEF4;
    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    girl_state = girl_state_flags & 0x7F;
    /* 2U: with two plain 2 IDO shares one constant register, the original does not (T-4020) */
    if ((date_arrival == ((unsigned int)DATE_ARRIVAL_CORRECTED_PLACE)) && (date_arrival_line == 2) && (girl_state >= 2U)) {
        portrait_layout_set(3);
    }
    return talk_run(D_80122CDC, 1, 0);
}

s32 date_special_day_greeting(void) {
    /* FAKE: the fields are read through a local pointer; an indirect load is not a plain variable for uopt, which gives the
     * 0x3E/0x40 selector tests the original's registers (autotweak gstate, T-20830). T-21520 */
    GameState *game = &game_state;

    if (game->substep++ == 0) {
        if ((game->month == 3) && (game->day == 0x15) && (game->year != 0x60)) {
            date_special_day_group = DATE_SPECIAL_DAY_MARCH;
        } else if ((game->month == 3) && (game->day == 0x14) && (game->year == 0x60)) {
            date_special_day_group = DATE_SPECIAL_DAY_MARCH;
        } else if ((game->year == 0x5F) && (game->month == 7) && (game->day == 0x17)) {
            date_special_day_group = DATE_SPECIAL_DAY_JULY_SUNDAY;
        } else if ((game->year == 0x60) && (game->month == 7) && (game->day == 0x1C)) {
            date_special_day_group = DATE_SPECIAL_DAY_JULY_SUNDAY;
        } else if ((game->year == 0x61) && (game->month == 7) && (game->day == 0x1B)) {
            date_special_day_group = DATE_SPECIAL_DAY_JULY_SUNDAY;
        } else if ((game->month == 9) && (game->day == 0x17) && (game->year != 0x60)) {
            if (game->frame_count & 1) {
                date_special_day_group = DATE_SPECIAL_DAY_SEPTEMBER_ODD;
            } else {
                date_special_day_group = DATE_SPECIAL_DAY_SEPTEMBER_EVEN;
            }
        } else if ((game->month == 9) && (game->day == 0x16) && (game->year == 0x60)) {
            if (game->frame_count & 1) {
                date_special_day_group = DATE_SPECIAL_DAY_SEPTEMBER_ODD;
            } else {
                date_special_day_group = DATE_SPECIAL_DAY_SEPTEMBER_EVEN;
            }
        } else if ((game->month == 0xC) && (game->day == 0x17)) {
            date_special_day_group = DATE_SPECIAL_DAY_DECEMBER_23;
        } else {
            mode_step_next();
            return 0;
        }
    }
    if (game->substep >= 0xBU) {
        game->substep = 0xA;
    }
    talk_tables = (u8 *) &date_special_day_group;
    D_800CA138 = (u8 *) &date_special_day_line;
    D_800CA13C = D_8015BE20;
    D_800CA140 = D_8015BE8C;
    D_800CA144 = D_8015BEF8;
    return talk_run(D_80122CDC, 1, 0);
}

void date_choice_reaction_speak(void) {
    s16 reaction_group;

    talk_tables = (u8 *) &reaction_group;
    D_800CA138 = (u8 *) &D_800CA22C[D_80122CDC];
    D_800CA13C = D_8015BD98;
    D_800CA140 = D_8015BDA8;
    reaction_group = 0;
    D_800CA144 = D_8015BDB8;
    if (talk_run(D_80122CDC, 0, 0) != 0) {
        mode_step_next();
    }
}

void date_choice_special_reactions(void) {
    GameState *game;
    s16 reaction_group;
    s16 reaction_line;

    game = &game_state;

    /* FAKE: discarded entry assignment suppresses compare CVT rewriting; otherwise the first
     * branch operands are exchanged (same fix as the EVENT twin func_80117D80, T-14300). T-17200 */
    reaction_group = D_800CA2FC;
    talk_tables = (u8 *) &reaction_group;
    D_800CA13C = D_8015BD98;
    D_800CA140 = D_8015BDA8;
    reaction_group = 0;
    D_800CA144 = D_8015BDB8;
    if ((D_80122CDC == 0) && (game->param_athletics.value < 0x28) && (D_800CA2FC == 1) && (D_800CA2F8 == 0x10) &&
        ((game->current_girl == GIRL_SHIORI) || (game->current_girl == GIRL_MIO) || (game->current_girl == GIRL_AYAKO) ||
         (game->current_girl == GIRL_SAKI) || (game->current_girl == GIRL_YUKARI) || (game->current_girl == GIRL_YUKO) ||
         (game->current_girl == GIRL_MEGUMI))) {
        D_800CA22C[0] = 3;
        D_800CA21C[0] = 3;
        D_800CA224[0] = 3;
        D_800CA234[0] = 2;
        portrait_layout_set(3);
        reaction_line = 5;
        D_800CA138 = (u8 *) &reaction_line;
        if (talk_run(D_80122CDC, 0, 0) != 0) {
            mode_step_next();
        }
    } else if (((D_80122CDC == 0) && (game->param_athletics.value < 0x28) && (D_800CA2FC == 0) && (D_800CA2F8 == 0x13) &&
                ((game->current_girl == GIRL_SHIORI) || (game->current_girl == GIRL_MIO) || (game->current_girl == GIRL_AYAKO) ||
                 (game->current_girl == GIRL_NOZOMI) || (game->current_girl == GIRL_MIRA))) ||
               ((D_80122CDC == 0) && (game->param_athletics.value < 0x64) && (D_800CA2FC == 0) && (D_800CA2F8 == 0x14) &&
                ((game->current_girl == GIRL_SHIORI) || (game->current_girl == GIRL_SAKI) || (game->current_girl == GIRL_NOZOMI) ||
                 (game->current_girl == GIRL_MIRA) || (game->current_girl == GIRL_YUKO) || (game->current_girl == GIRL_YUMI)))) {
        D_800CA22C[0] = 3;
        D_800CA21C[0] = 3;
        D_800CA224[0] = 3;
        D_800CA234[0] = 2;
        portrait_layout_set(3);
        reaction_line = 6;
        D_800CA138 = (u8 *) &reaction_line;
        if (talk_run(D_80122CDC, 0, 0) != 0) {
            mode_step_next();
        }
    } else {
        mode_step_next();
    }
}

void date_farewell_speak(void) {
    GameState *game;
    s16 farewell_group;
    u8 rank;
    u8 girl_state_flags;

    game = &game_state;
    rank = get_g_zyotai_s(game->current_girl) & 0x7F;
    if (game->substep >= 0xBU) {
        game->substep = 0xA;
    }
    if (game->substep++ == 0) {
        D_800CA158 = ((rank >= 2U) + (rank >= 3U) + (rank >= 4U)) * 3;
        if (rank == 4) {
            portrait_layout_set(4);
            D_800CA234[D_80122CDC] = 0;
        }
    }
    farewell_group = D_800CA234[D_80122CDC] + D_800CA158;
    if (game->appointment_flag != 0) {
        farewell_group = 0xA;
    }
    if ((game->appointment_flag != 0) && (rank < 2U) && (game->current_girl == GIRL_NOZOMI)) {
        farewell_group = 0x12;
    }
    /* FAKE: u8 cast on date_parade_offered emits lbu, and *(s16 *)&D_800CA15C emits lh to match original load instructions. T-24079 */
    if ((D_80122D08 >= 0) && (D_80122D08 == game->girl_last_date_spot[game->current_girl]) && (game->girl_last_date_spot_timer[game->current_girl] != 0) && (game->game_mode == 0x50) && ((u8)date_parade_offered == 0) && (game->appointment_flag == 0) && (D_800CA2F8 != 0) && (D_800CA2F8 != 0x12)) {
        farewell_group = 0xB;
        if (game->substep == 1) {
            portrait_layout_set(3);
            game->girls[game->current_girl].tokimeki -= 1;
            game->girls[game->current_girl].affection -= 2;
            game->girls[game->current_girl].syoushin += 5;
            check_para_limit();
        }
        if ((rank < 2U) && (game->current_girl == GIRL_NOZOMI)) {
            farewell_group = 0x13;
        }
    }
    talk_tables = (u8 *) &farewell_group;
    D_800CA138 = &D_800CA15C;
    girl_state_flags = get_g_zyotai_s(game->current_girl);
    if (((game->current_girl == GIRL_MIO) || (game->current_girl == GIRL_YUKARI)) && ((girl_state_flags & 0x7F) < 2U) && (*(s16 *)&D_800CA15C == 3)) {
        portrait_layout_set(1);
    }
    D_800CA13C = D_8015BE18;
    D_800CA140 = D_8015BE84;
    D_800CA144 = D_8015BEF0;
    talk_run(D_80122CDC, 1, 0);
}

s32 date_closing_speak(void) {
    u8 girl_state_flags;
    u32 girl_state;

    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    if (game_state.substep++ == 0) {
        girl_state = girl_state_flags & 0x7F;
        date_closing_group = (girl_state >= 2U) + (girl_state >= 3U) + (girl_state >= 4U);
    }
    talk_tables = (u8 *) &date_closing_group;
    D_800CA138 = (u8 *) &date_closing_line;
    D_800CA13C = D_8015BE28;
    D_800CA140 = D_8015BE94;
    D_800CA144 = D_8015BF00;
    return talk_run(D_80122CDC, 1, 0);
}

void date_neighborhood_park_greeting_speak(void) {
    talk_tables = (u8 *) &date_opening_group;
    D_800CA138 = (u8 *) &date_opening_line;
    D_800CA13C = D_8015BE24;
    D_800CA140 = D_8015BE90;
    D_800CA144 = D_8015BEFC;
    if (talk_run(D_80122CDC, 0, 0) != 0) {
        mode_step_next();
    }
}

s32 date_movie_theater_film_talk(void) {
    if (game_state.substep++ < 0x80) {
        return 0;
    }
    if (game_state.substep >= 0x97) {
        game_state.substep = 0x96;
    }
    date_movie_line = date_movie_index;
    talk_tables = &date_movie_group;
    D_800CA138 = &date_movie_line;
    D_800CA13C = D_8015BD9C;
    D_800CA140 = D_8015BDAC;
    D_800CA144 = D_8015BDBC;
    if (talk_run(D_80122CDC, 0, 0) != 0) {
        audio_command(0xC4);
        audio_command(0x201);
        mode_step_next();
        D_80122CFC = 0x80;
        return 0;
    }
    D_80122CF0 = 0xA;
    if ((cd_last_command() == 9) && (check_end_k() != 0)) {
        audio_command(0x201);
        D_80122CE4 = 0;
        k_reset(1);
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        talk_voice_wait_frames = 0;
        mode_step_next();
    }
}

s32 date_concert_hall_program_talk(void) {
    if (game_state.substep++ < 0x80) {
        return 0;
    }
    if (game_state.substep >= 0x97) {
        game_state.substep = 0x96;
    }
    date_concert_line = date_concert_index;
    talk_tables = &date_concert_group;
    D_800CA138 = &date_concert_line;
    D_800CA13C = date_shared_section_text;
    D_800CA140 = date_shared_section_command;
    D_800CA144 = date_shared_section_voice;
    if (talk_run(D_80122CDC, 0, 0) != 0) {
        audio_command(0xC4);
        audio_command(0x200);
        mode_step_next();
        D_80122CFC = 0x80;
        return 0;
    }
    D_80122CF0 = 0xA;
    if ((cd_last_command() == 9) && (check_end_k() != 0)) {
        audio_command(0x200);
        D_80122CE4 = 0;
        k_reset(1);
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        talk_voice_wait_frames = 0;
        mode_step_next();
    }
}

void date_amusement_park_talk_unshuffled(void) {
    talk_tables = &date_park_parade_group;
    D_800CA138 = &date_park_parade_line;
    D_800CA13C = D_8015BDE4;
    D_800CA140 = D_8015BE50;
    D_800CA144 = D_8015BEBC;
    talk_run(0xFF, 1, 0);
}

/* FAKE: non-void return type keeps $v0 live across the call in date_meeting_place_resolve so ugen allocates $v1 for game_state.mode_step. T-24079 */
s32 date_wrong_place_speak(void) {
    talk_tables = &date_meeting_group;
    D_800CA138 = &date_meeting_line;
    D_800CA13C = D_8015BD94;
    D_800CA140 = D_8015BDA4;
    D_800CA144 = D_8015BDB4;
    talk_run(0xFF, 1, 0);
}

void date_stadium_talk_speak(void) {
    talk_tables = (u8 *) &talk_group;
    if ((game_state.current_girl == GIRL_SHIORI) && (((u32) game_state.player_flags.h >> 0xC) == 6) && (D_800CA2FC == 0) && (talk_line == 0)) {
        talk_line += 1;
    }
    D_800CA138 = (u8 *) &talk_line;
    D_800CA13C = talk_text_table;
    D_800CA140 = talk_command_table;
    D_800CA144 = talk_voice_table;
    if (talk_run(D_80122CDC, 0, 0) != 0) {
        mode_step_next();
    }
}

void date_shopping_street_extra_face(void) {
    if (D_80122CDC == 2) {
        if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_AYAKO)) {
            mode_step_next();
            return;
        }
        if (game_state.current_girl == GIRL_YUKO) {
            face_expression_4();
            return;
        }
        if ((game_state.current_girl == GIRL_YUKARI) && (game_state.girls[5].scene_counters[4] == 0)) {
            mode_step_next();
            return;
        }
        if ((game_state.current_girl == GIRL_YUKARI) && (game_state.girls[5].scene_counters[4] != 0)) {
            talk_group += 1;
            face_expression_3();
            return;
        }
        face_expression_3();
        return;
    }
    mode_step_next();
}

void date_amusement_park_empty_step(void) {
    mode_step_next();
}

void date_sea_nozomi_check(void) {
    s32 response_class;

    if (game_state.current_girl != GIRL_NOZOMI) {
        mode_step_next();
        game_state.mode_step += 8;
        return;
    }
    if (game_state.param_athletics.value >= 0x96) {
        talk_group += 2;
        mode_step_set(0x2D);
        return;
    }
    response_class = D_800CA22C[0] = D_800CA22C[3] = 2;
    D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 3;
    D_800CA22C[2] = response_class;
    D_800CA234[0] = response_class;
    /* FAKE: the u16 view of the row gives its constant 3 the original's $v0 (autotweak, T-17202). */
    ((u16 *)D_800CA224)[0] = ((u16 *)D_800CA224)[1] = ((u16 *)D_800CA224)[2] = 3;
    mode_step_next();
}

void date_load_special_portrait(void) {
    if (D_800CA2F8 == 0x12) {
        date_load_shrine_portrait();
        mode_step_next();
    } else if (D_800CA2F8 == 0x14) {
        date_load_ski_portrait();
        mode_step_next();
    }
    mode_step_next();
}

void date_load_ski_portrait(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x5693);
        return;
    case GIRL_SAKI:
        portrait_read_start(0x67FA);
        return;
    case GIRL_YUKARI:
        portrait_read_start(0x60F2);
        return;
    case GIRL_NOZOMI:
        portrait_read_start(0x6368);
        return;
    case GIRL_MIRA:
        portrait_read_start(0x5C33);
        return;
    case GIRL_YUKO:
        portrait_read_start(0x517A);
        return;
    default:
        portrait_read_start(0x6A70);
        return;
    }
}

void date_load_shrine_portrait(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x56ED);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5EA9);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x52B5);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5A17);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x6854);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x603E);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x63C2);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5CBA);
        break;
    case GIRL_YUKO:
        portrait_read_start(0x506C);
        break;
    case GIRL_MEGUMI:
        portrait_read_start(0x660B);
        break;
    case GIRL_YUMI:
        portrait_read_start(0x69BC);
        break;
    }
}

void date_choice_score(void) {
    /* FAKE: unused locals fake_padding frame to 0x48 with trivia_multiplier/place_multiplier at sp+0x1C/sp+0x18 and girl_counter_weights at sp+0x28, and byte-pointer indexing on the third call prevents uopt from CSEing D_800CA22C[D_80122CDC]. T-24099 */
    s32 fake_top_padding;
    s32 trivia_multiplier;
    s32 place_multiplier;
    s32 fake_padding[2];
    DateGirlCounterWeights girl_counter_weights;

    girl_counter_weights = date_syoushin_weight;
    portrait_layout_set(D_800CA21C[D_80122CDC]);
    date_score_multipliers(3 - D_800CA22C[D_80122CDC], &trivia_multiplier, &place_multiplier);
    *(s32 *)&game_state.girls[game_state.current_girl].tokimeki_fraction += (((3 - D_800CA22C[D_80122CDC]) * trivia_multiplier * place_multiplier) << 16) / 16;
    printf("zatu%d basho%d\n", trivia_multiplier, place_multiplier);
    date_score_multipliers(2 - D_800CA22C[D_80122CDC], &trivia_multiplier, &place_multiplier);
    *(s32 *)&game_state.girls[game_state.current_girl].affection_fraction += (((2 - D_800CA22C[D_80122CDC]) * trivia_multiplier * place_multiplier) << 16) / 16;
    date_score_multipliers(2 - *(s16 *)((u8 *)D_800CA22C + (D_80122CDC << 1)), &trivia_multiplier, &place_multiplier);
    *(s32 *)&game_state.girls[game_state.current_girl].syoushin_fraction += ((girl_counter_weights.by_response_class[D_800CA22C[D_80122CDC]] * trivia_multiplier * place_multiplier) << 16) / 16;
    check_para_limit();
    mode_step_next();
}

void date_score_multipliers(s16 score_delta, s32 *trivia_multiplier_out, s32 *place_multiplier_out) {
    /* FAKE: 8 bytes of scalar locals before place_affinities place it at sp+0x18 in the 0x108 frame, goto loops keep the threshold-break branch layout, and empty girl_index test forces $v0 load. T-24079 */
    s32 girl_index;
    s16 i;
    u8 place_affinity;
    DatePlaceAffinityTable place_affinities;

    place_affinities = date_place_affinity;
    if (score_delta > 0) {
        i = 0;
loop1:
        if (game_state.param_trivia.value >= (0x64 - (i * 0x19))) {
            *trivia_multiplier_out = 5 - i;
        } else {
            i += 1;
            if (i < 5) {
                goto loop1;
            }
        }
        if (game_state.param_trivia.value == 0) {
            *trivia_multiplier_out = 0;
        }
        girl_index = game_state.current_girl;
        if (girl_index) {}
        place_affinity = place_affinities.by_girl_and_place[girl_index][D_800CA2F8];
        if (place_affinity == 0) {
            *place_multiplier_out = 5;
            return;
        }
        if ((place_affinity == 1) || (place_affinity == 2)) {
            *place_multiplier_out = 4;
            return;
        }
        if (place_affinity == 3) {
            *place_multiplier_out = 3;
            return;
        }
        *place_multiplier_out = 2;
        return;
    }
    i = 0;
loop2:
    if (game_state.param_trivia.value >= (0x64 - (i * 0x19))) {
        *trivia_multiplier_out = i + 3;
    } else {
        i += 1;
        if (i < 5) {
            goto loop2;
        }
    }
    if (game_state.param_trivia.value == 0) {
        *trivia_multiplier_out = 8;
    }
    girl_index = game_state.current_girl;
    if (girl_index) {}
    place_affinity = place_affinities.by_girl_and_place[girl_index][D_800CA2F8];
    if (place_affinity == 0) {
        *place_multiplier_out = 3;
        return;
    }
    if ((place_affinity == 1) || (place_affinity == 2)) {
        *place_multiplier_out = 4;
        return;
    }
    if (place_affinity == 3) {
        *place_multiplier_out = 5;
        return;
    }
    *place_multiplier_out = 6;
}

void date_pool_face_step(void) {
    portrait_layout_set(D_800CA21C[D_80122CDC]);
    mode_step_next();
}

void date_choice_mood_face_step(void) {
    if (game_state.appointment_flag != 0) {
        portrait_layout_set(1);
    } else {
        portrait_layout_set(D_800CA224[D_80122CDC]);
    }
    mode_step_next();
}

void date_sea_mood_face_step(void) {
    portrait_layout_set(D_800CA224[D_80122CDC]);
    mode_step_next();
}

void date_amusement_park_yuina_reaction(void) {
    if (game_state.current_girl == GIRL_YUINA) {
        face_expression_3();
        game_state.girls[2].tokimeki -= 1;
        game_state.girls[2].affection -= 1;
    } else if ((game_state.current_girl == GIRL_MIO) && (game_state.girls[1].scene_counters[0x1A] == 0)) {
        face_expression_0();
        game_state.girls[2].tokimeki += 1;
        game_state.girls[2].affection += 1;
    } else {
        face_expression_1();
        game_state.girls[2].tokimeki += 1;
    }
    check_para_limit();
}

s32 date_ambush_check(void) {
    GameState *game = &game_state;
    s32 has_club_practice;
    s16 i;

    has_club_practice = 0;
    for (i = 0; i < 11; i++) {
        if (game->clubs[i].practice_points != 0) {
            has_club_practice = 1;
        }
        if (((((ClubFlagBits *)&game->clubs[i])->unk_25 == 3) || ((PlayerFlagBits *)&game->player_flags)->b19) && !((PlayerFlagBits *)&game->player_flags)->b20 && (game->year == 0x61) && (game->month == 9)) {
            game->scene_flags[0x55] = 2;
            ((PlayerFlagBits *)&game->player_flags)->b20 = 1;
            game->day_event_done_bits &= 8;
            D_80122CE0 = 5;
            game->saved_game_mode = game->game_mode;
            game->saved_mode_stage = game->mode_stage;
            game->saved_mode_step = 0x3E;
            game->date_place_history[0] = 0;
            game->date_place_history[1] = 0;
            game->date_place_history[2] = 0;
            game->date_place_history[3] = 0;
            mode_step_set(0x40);
            return 0;
        }
    }
    if ((has_club_practice == 0) && (game->year == 0x61) && (game->month == 9) && !((PlayerFlagBits *)&game->player_flags)->b20) {
        ((ClubFlagBits *)&game->clubs[11])->unk_25 = 3;
        game->scene_flags[0x55] = 2;
        ((PlayerFlagBits *)&game->player_flags)->b20 = 1;
        game->day_event_done_bits &= 8;
        D_80122CE0 = 5;
        game->saved_game_mode = game->game_mode;
        game->saved_mode_stage = game->mode_stage;
        game->saved_mode_step = 0x3E;
        game->date_place_history[0] = 0;
        game->date_place_history[1] = 0;
        game->date_place_history[2] = 0;
        game->date_place_history[3] = 0;
        mode_step_set(0x40);
        return 0;
    }
    if ((game->date_place_history[0] == D_800CA2F8) && (game->date_place_history[0] == game->date_place_history[1]) && !((PlayerFlagBits *)&game->player_flags)->b20 && (game->year >= 0x60) && ((*(s8 *)&D_800CA2F8) != 0)) {
        D_80122CE0 = 4;
        game->scene_flags[0x55] = 1;
        game->day_event_done_bits &= 8;
        ((PlayerFlagBits *)&game->player_flags)->b19 = 1;
        game->saved_game_mode = game->game_mode;
        game->saved_mode_stage = game->mode_stage;
        game->saved_mode_step = 0x3E;
        game->date_place_history[1] = 0;
        game->date_place_history[2] = 0;
        game->date_place_history[3] = 0;
        game->date_place_history[0] = 0;
        return mode_step_set(0x40);
    }
    return mode_step_next();
}

void date_ambush_prepare_battle(void) {
    cd_read_sectors(1U, 0x801B0000, 0xAFCA);
    date_ambush_sections_init();
    if ((D_800CA2F8 == 0) || (D_800CA2F8 == 1)) {
        talk_group = 0;
    } else {
        talk_group = 1;
    }
    talk_line = 0;
    talk_text_table = (s32)D_8015B648;
    talk_command_table = (s32)D_8015B64C;
    talk_voice_table = (s32)D_8015B650;
    D_80122D20 = 1;
    game_state.current_girl = GIRL_SOTOI;
    strcpy(name_code_girl_short, "不良");
    D_80122D4C = 1;
    mode_step_next();
}

void date_ambush_start_battle(void) {
    actor_clear_range(0);
    hud_cursor_show(0);
    game_state.current_girl = game_state.date_girl;
    k_reset(0);
    game_mode_set(GAME_MODE_RPG_BATTLE);
}

void date_ambush_after_battle(void) {
    s32 fake_padding[2]; /* FAKE: unused slots above rect, the original frame has them (real source unknown). T-9210 */
    RECT rect;
    s16 i;

    if (D_80122CE0 == 0) {
        if (game_state.scene_flags[85] == 2) {
            for (i = 0; i < 0xB; i++) {
                if (game_state.girls[i].flags.f.met) {
                    game_state.girls[i].tokimeki += 3;
                }
                if (i == game_state.date_girl) {
                    game_state.girls[i].tokimeki += 2;
                }
            }
        } else {
            game_state.girls[game_state.date_girl].tokimeki += 2;
        }
    } else {
        game_state.param_stress.value += 5;
    }
    check_para_limit();
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    talk_clear();
    k_reset(1);
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_names_set(game_state.current_girl);
    addr_init_bustup();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    hizuke_init();
    message_window_init();
    text_marker_hide();
    mode_step_next();
}

s32 date_end(void) {
    GameState *game = &game_state;
    if (game->substep == 0) {
        hud_day_bgm_load();
        game->substep++;
    }
    if (game->substep == 1) {
        if (game->mode_step_timer.u++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game->substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game->substep += 1;
        } else {
            return 0;
        }
    }
    if (!((AppointmentBits *)&game->unk_54C[0])->active) {
        if ((D_80122D08 > 0) && ((u8)D_800CA364 == 0)) {
            game->girls[game->current_girl].scene_counters[D_80122D08] += 1;
        }
        if ((game->param_looks.value >= 0x23) || ((game->param_looks.value >= 0x1E) && (game->current_girl != GIRL_MIRA))) {
            if (D_80122D08 > 0) {
                game->girl_last_date_spot[game->current_girl] = D_80122D08;
                game->girl_last_date_spot_timer[game->current_girl] = 0xC;
            }
            if (D_80122D08 == 8) {
                game->girl_last_date_spot[game->current_girl] = 7;
            }
            if (D_80122D08 == 6) {
                game->girl_last_date_spot[game->current_girl] = 5;
            }
        }
        if ((game->param_looks.value >= 0x23) || ((game->param_looks.value >= 0x1E) && (game->date_girl != GIRL_MIRA))) {
            if (D_800CA2F8 == 0x12) {
                game->girls[game->current_girl].tokimeki += 1;
            }
            if ((game->month == 5) && ((game->day == 4) || (game->day == 5)) && (game->girls[game->current_girl].flags.f.b4)) {
                game->girls[game->current_girl].tokimeki += 2;
            }
            if ((D_80122CE0 == 0) && (game->appointment_place_1 == D_800CA2F8)) {
                game->girls[game->current_girl].syoushin -= 0x28;
                if ((game->month == 5) && ((game->day == 3) || (game->day == 4))) {
                    game->girls[game->current_girl].flags.f.b4 = 1;
                }
            }
        }
    }
    check_para_limit();
    game->appointment_place_0 = D_800CA2F8;
    game->date_place_history[3] = game->date_place_history[2];
    game->date_place_history[2] = game->date_place_history[1];
    game->date_place_history[1] = game->date_place_history[0];
    game->date_place_history[0] = D_800CA2F8;
    D_80122D44 = 0;
    audio_command(0xCF);
    return day_event_next(1);
}

void date_birthday_check(void) {
    s32 birthday_bits;

    birthday_bits = game_state.girls[game_state.date_girl].birthday.w;
    if (((birthday_bits & 0xF) == game_state.month) && (((u32)(birthday_bits << 23) >> 27) == game_state.day)) {
        game_mode_set(GAME_MODE_DATE);
        mode_stage_set(2);
        return;
    }
    birthday_bits = game_state.player_birthday;
    if ((game_state.month == (birthday_bits & 0xF)) && (game_state.day == ((u32)(birthday_bits << 23) >> 27))) {
        game_mode_set(GAME_MODE_DATE);
        mode_stage_set(3);
        return;
    }
    mode_step_next();
}

void date_central_park_summer_group_bonus(void) {
    u8 girl_id;
    u8 place_variant;

    girl_id = game_state.current_girl;
    if ((girl_id == 1) && (game_state.month >= 6) && (game_state.month < 9)) {
        place_variant = D_800CA2FC;
        if (place_variant == 0) {
            talk_group += 9;
        } else if (place_variant == 1) {
            talk_group += 9;
        }
    }
    mode_step_next();
}

void date_zoo_mio_tables(void) {
    u8 girl_id;
    u8 place_variant;

    girl_id = game_state.current_girl;
    if (girl_id == 1) {
        place_variant = D_800CA2FC;
        if (place_variant == 0) {
            talk_group = 4;
        }
        if (place_variant == 1) {
            talk_group = 7;
        }
        if (place_variant == 2) {
            talk_group = 0xA;
        }
        if ((game_state.month >= 6) && (game_state.month < 9)) {
            talk_group = 0;
            D_800CA22C[0] = 1;
            D_800CA22C[1] = 4;
            D_800CA22C[2] = 2;
            D_800CA21C[0] = 2;
            D_800CA21C[1] = 3;
            D_800CA21C[2] = 3;
        }
        date_choice_classes_derive();
        return;
    }
    mode_step_next();
}

s32 date_zoo_mio_skip(void) {
    if ((game_state.current_girl == GIRL_MIO) && (game_state.month >= 6) && (game_state.month < 9) && (D_80122CDC == 1)) {
        mode_step_next();
        return 0;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void date_zoo_skip_three(void) {
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

s32 date_amusement_park_ride_special_mio(void) {
    /* FAKE: narrow constant and pointer view retain original register and branch operand order. T-11140 */
    GameState *game = &game_state;
    u16 fake_mio_and_seen_value;

    fake_mio_and_seen_value = 1;
    if ((game_state.current_girl == (u8)fake_mio_and_seen_value) && (*(u8 *)&date_ride_index == 3)) {
        if ((game_state.scene_flags[9] == 0) && ((game->month < 3) || (game_state.month >= 6))) {
            talk_group += 1;
            D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 3;
            D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 3;
            date_choice_classes_derive();
            portrait_layout_set(3);
            return 0;
        }
        if (fake_mio_and_seen_value == game_state.scene_flags[9]) {
            D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 4;
            D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 4;
            date_choice_classes_derive();
            portrait_layout_set(4);
            return 0;
        }
    } else if ((game_state.current_girl == GIRL_YUKO) && (*(u8 *)&date_ride_index == 2) &&
               (game_state.girls[8].scene_counters[0x17] != 0)) {
        talk_group += 3;
    } else if ((game_state.current_girl == GIRL_SAKI) && (*(u8 *)&date_ride_index == 3) &&
               (game_state.girls[4].scene_counters[0x18] != 0)) {
        talk_group += 3;
    }
    mode_step_next();
}

s32 date_amusement_park_ride_talk(void) {
    /* FAKE: pointer view prevents uopt from CSEing game->unk_F5F across the condition blocks, and *(u8 *)&date_ride_index emits lbu on the s8 global. T-24099 */
    GameState *game;

    game = &game_state;

    if ((game->current_girl == GIRL_MIRA) && (game->scene_flags[0x31] != 0) && (*(u8 *)&date_ride_index == 1)) {
        portrait_layout_set(4);
        D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 4;
        D_800CA234[0] = D_800CA234[1] = D_800CA234[2] = 2;
        D_800CA21C[0] = D_800CA21C[1] = D_800CA21C[2] = 4;
        D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;
        talk_group += 2;
        game->mode_step += 0xD;
        return 0;
    } else {
        if ((game->current_girl == GIRL_YUKO) && (*(u8 *)&date_ride_index == 3) && (game->girls[8].scene_counters[0x18] != 0)) {
            if (game->substep++ == 0) {
                talk_line = 2;
            }
        }
        if ((game->current_girl == GIRL_YUKO) && (*(u8 *)&date_ride_index == 4) && (game->girls[8].scene_counters[0x19] != 0)) {
            if (game->substep++ == 0) {
                talk_line = 2;
            }
        }
        if (game->substep >= 0xB) {
            game->substep = 0xA;
        }
        normal_date_speak();
        if ((game->current_girl == GIRL_YUKO) && (*(u8 *)&date_ride_index == 3) && (game->girls[8].scene_counters[0x18] == 0) && (talk_line == 3)) {
            audio_command(0x74);
            k_reset(1);
            talk_group += 1;
            talk_line = 0;
            D_80122CE4 = 0;
            mode_step_next();
        }
        if ((game->current_girl == GIRL_YUKO) && (*(u8 *)&date_ride_index == 4) && (game->girls[8].scene_counters[0x19] == 0) && (talk_line == 3)) {
            audio_command(0x74);
            k_reset(1);
            talk_group += 1;
            talk_line = 0;
            D_80122CE4 = 0;
            mode_step_next();
        }
    }
}

void date_amusement_park_ride_skip(void) {
    /* FAKE: narrow constant and casts retain the original branch operand order. T-11140
       The selector copy in the else chain puts the variable first in its compares (T-11580). */
    u16 fake_mio_and_seen_value;
    u8 ride_index;

    fake_mio_and_seen_value = 1;
    if ((game_state.current_girl == (u8)fake_mio_and_seen_value) && (*(u8 *)&date_ride_index == 3)) {
        if ((game_state.scene_flags[9] == 0) && ((game_state.month < 3) || (game_state.month >= 6))) {
            game_state.mode_step += 0xD;
        }
        if (fake_mio_and_seen_value == game_state.scene_flags[9]) {
            game_state.mode_step += 0xD;
        }
    } else {
        ride_index = *(u8 *)&date_ride_index;
        if ((game_state.current_girl == GIRL_SAKI) && (ride_index == 3) && (game_state.girls[4].scene_counters[0x18] != 0)) {
            game_state.mode_step += 7;
        } else if ((game_state.current_girl == GIRL_MIRA) && (ride_index == 1)) {
            game_state.mode_step += 7;
        } else if ((game_state.current_girl == GIRL_YUKO) && (ride_index == 2) && (game_state.girls[8].scene_counters[0x17] != 0)) {
            game_state.mode_step += 7;
        }
    }
    mode_step_next();
}

void date_amusement_park_yuina_second_answer(void) {
    if (game_state.current_girl == GIRL_YUINA) {
        portrait_layout_set(4);
        talk_group = 5;
        game_state.girls[2].tokimeki += 1;
        game_state.girls[2].affection += 1;
        game_state.girls[2].syoushin -= 0x14;
        check_para_limit();
        mode_step_set(0x5E);
        return;
    }
    date_scene_choose();
}

void date_festival_skip_five(void) {
    if ((game_state.current_girl == GIRL_YUINA) || (game_state.current_girl == GIRL_YUKO) || (game_state.current_girl == GIRL_YUKARI) || (GIRL_YUMI == game_state.current_girl)) {
        game_state.mode_step += 5;
    }
    mode_step_next();
}

void date_amusement_park_yuina_talk(void) {
    if (talk_line == 1) {
        game_state.current_girl = GIRL_SOTOI;
    } else {
        game_state.current_girl = GIRL_YUINA;
    }
    normal_date_speak();
    if (talk_line == 2) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void date_amusement_park_jump_to_end(void) {
    mode_step_set(0x3B);
}

void date_shopping_street_third_answer_branch(void) {
    /* FAKE: narrow constant retains the original branch operand order. T-11140 */
    u16 fake_third_choice;
    s32 previous_step;

    fake_third_choice = 2;
    if ((game_state.current_girl == GIRL_NOZOMI) && (game_state.girls[6].scene_counters[4] != 0) && (fake_third_choice == D_80122CDC)) {
        talk_group = 0;
        mode_step_set(0);
        return;
    }
    if ((game_state.current_girl == GIRL_YUKO) && (fake_third_choice == D_80122CDC)) {
        previous_step = game_state.mode_step;
        normal_date_speak();
        if (previous_step != game_state.mode_step) {
            talk_group = 0;
            mode_step_set(0);
        }
        return;
    }
    if ((game_state.current_girl == GIRL_YUKARI) && (game_state.girls[5].scene_counters[4] == 0) && (fake_third_choice == D_80122CDC)) {
        talk_group = 3;
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_shopping_street_nozomi_group_bonus(void) {
    if ((game_state.current_girl == GIRL_NOZOMI) && (game_state.girls[6].scene_counters[4] != 0) && (D_80122CDC == 2)) {
        talk_group += 1;
    }
    mode_step_next();
}

void date_sea_forced_worst_classes(void) {
    D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;
    D_800CA22C[0] = D_800CA22C[1] = D_800CA22C[2] = 4;
    D_800CA234[0] = D_800CA234[1] = D_800CA234[2] = 2;
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void date_botanical_garden_mira_skip(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 0) && (D_800CA2FC == 0)) {
        mode_step_next();
        return;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

s32 date_art_museum_mira_face(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 1) && ((D_800CA2FC == 2) || (D_800CA2FC == 0))) {
        face_expression_4();
        return 0;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void date_art_museum_variant_face(void) {
    if (D_800CA2FC == 0) {
        face_expression_4();
        return;
    }
    if (D_800CA2FC == 2) {
        face_expression_4();
        return;
    }
    date_art_museum_choice_face();
}

void date_library_talk(void) {
    if (talk_line == 1) {
        game_state.current_girl = game_state.date_girl;
    }
    if (talk_line == 2) {
        D_80122D20 = 0;
    }
    normal_date_speak();
}

void date_library_extra_branch(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_800CA2FC == 0)) {
        talk_group += D_80122CDC;
        mode_step_next();
        return;
    }
    if ((game_state.current_girl == GIRL_YUMI) && (D_800CA2FC == 0) && (D_80122CDC == 0)) {
        game_state.current_girl = GIRL_SOTOI;
        D_80122D20 = 1;
        mode_step_next();
        return;
    }
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void date_library_empty_step(void) {
    mode_step_next();
}

void date_game_center_mira_extra_talk(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_game_center_mira_worst_face(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        face_expression_4();
        return;
    }
    mode_step_next();
}

void date_game_center_mira_face_1(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 0) && (D_800CA2FC == 1)) {
        face_expression_1();
        return;
    }
    mode_step_next();
}

void date_karaoke_mira_extra_talk(void) {
    if ((game_state.current_girl == GIRL_MIRA) && (D_80122CDC == 2) && (D_800CA2FC == 1)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_amusement_park_speak(void) {
    normal_date_speak();
}

void date_library_yuko_skip(void) {
    if ((game_state.current_girl == GIRL_YUKO) && (game_state.girls[8].scene_counters[15] != 0)) {
        game_state.mode_step += 0x1E;
    } else if (game_state.current_girl == GIRL_YUKO) {
        D_800CA234[0] = 1;
    }
    mode_step_next();
}

void date_sea_yukari_extra_talk(void) {
    if ((game_state.current_girl == GIRL_YUKARI) && (D_800CA2FC == 0) && (D_80122CDC == 2)) {
        normal_date_speak();
        return;
    }
    if ((game_state.current_girl == GIRL_YUKARI) && (D_800CA2FC == 1) && (D_80122CDC == 0)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_sea_yukari_extra_talk_face(void) {
    if ((game_state.current_girl == GIRL_YUKARI) && (D_800CA2FC == 0) && (D_80122CDC == 2)) {
        if (game_state.mode_step_timer.w++ == 0) {
            portrait_layout_set(3);
        }
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_shrine_megumi_extra_talk(void) {
    if ((game_state.current_girl == GIRL_MEGUMI) && (D_80122CDC == 2)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_aquarium_yumi_extra_talk(void) {
    if ((GIRL_YUMI == game_state.current_girl) && (D_80122CDC == 1) && (D_800CA2FC == 2)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_planetarium_yumi_talks(void) {
    /* FAKE: this narrow constant and its first-test cast retain the
     * original branch operand order and saved-state stack home. T-10020. */
    u16 fake_yumi_id;
    s32 previous_step;

    fake_yumi_id = 0xA;
    if ((game_state.current_girl == (u8)fake_yumi_id) && (D_80122CDC == 0) && (D_800CA2FC == 0)) {
        if (game_state.substep == 0) {
            talk_group = 7;
            talk_line = 0;
            game_state.substep = 1;
        }
        normal_date_speak();
        return;
    }
    if ((fake_yumi_id == game_state.current_girl) && (D_80122CDC == 2) && (D_800CA2FC == 1) && (game_state.param_science.value < 0x78)) {
        if (talk_line == 1) {
            game_state.current_girl = GIRL_SOTOI;
            D_80122D20 = 1;
        }
        previous_step = game_state.mode_step;
        normal_date_speak();
        if (previous_step != game_state.mode_step) {
            D_80122D20 = 0;
        }
        game_state.current_girl = game_state.date_girl;
        return;
    }
    mode_step_next();
}

void date_art_museum_yumi_extra_talk(void) {
    if ((GIRL_YUMI == game_state.current_girl) && (D_80122CDC == 2) && (D_800CA2FC == 4)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_bowling_alley_yumi_extra_talk_unused(void) {
    if ((GIRL_YUMI == game_state.current_girl) && (D_80122CDC == 0) && (D_800CA2FC == 0)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_bowling_alley_yumi_extra_talk(void) {
    if ((GIRL_YUMI == game_state.current_girl) && (D_80122CDC == 1) && (D_800CA2FC == 1)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}

void date_amusement_park_yumi_ride_talk(void) {
    s32 current_step;
    s32 previous_step;

    if ((GIRL_YUMI == game_state.current_girl) && (D_80122CDC == 1) && ((u8) date_ride_index == DATE_RIDE_FERRIS_WHEEL) && (D_800CA2FC == 1)) {
        previous_step = game_state.mode_step;
        normal_date_speak();
        current_step = game_state.mode_step;
        if (previous_step != current_step) {
            talk_group = 0x37;
        }
    } else {
        mode_step_next();
    }
}

void date_shrine_closing_talk(void) {
    /* FAKE: unused word local above alternate_girl_state puts alternate_girl_state at 0x2B(sp) as in the original. T-17202 */
    s32 fake_padding;
    u8 alternate_girl_state;

    alternate_girl_state = get_g_zyotai_h(game_state.current_girl);
    normal_date_speak();
    if (game_state.mode_step_timer.w++ == 1) {
        if (talk_line == 2) {
            portrait_layout_set(alternate_girl_state);
        }
    }
}

void date_shrine_answer_face(void) {
    if (D_80122CDC == 0) {
        if (game_state.current_girl == GIRL_MIRA) {
            face_expression_1();
            return;
        }
        face_expression_0();
        return;
    }
    mode_step_next();
}

void date_shopping_street_megumi_face(void) {
    if (game_state.current_girl == GIRL_MEGUMI) {
        face_expression_0();
        return;
    }
    face_expression_1();
}

void date_load_girl_script(void) {
    switch (game_state.date_girl) {
    case GIRL_SHIORI:
        date_fill_sections_shiori();
        cd_read_sectors(0x17U, 0x80198000, 0xAD29);
        break;
    case GIRL_MIO:
        date_fill_sections_mio();
        cd_read_sectors(0x18U, 0x80198000, 0xAD82);
        break;
    case GIRL_YUINA:
        date_fill_sections_yuina();
        cd_read_sectors(0x15U, 0x80198000, 0xAD40);
        break;
    case GIRL_AYAKO:
        date_fill_sections_ayako();
        cd_read_sectors(0x16U, 0x80198000, 0xAD55);
        break;
    case GIRL_SAKI:
        date_fill_sections_saki();
        cd_read_sectors(0x16U, 0x80198000, 0xADDB);
        break;
    case GIRL_YUKARI:
        date_fill_sections_yukari();
        cd_read_sectors(0x17U, 0x80198000, 0xAD9A);
        break;
    case GIRL_NOZOMI:
        date_fill_sections_nozomi();
        cd_read_sectors(0x15U, 0x80198000, 0xADB1);
        break;
    case GIRL_MIRA:
        date_fill_sections_mira();
        cd_read_sectors(0x17U, 0x80198000, 0xAD6B);
        break;
    case GIRL_YUKO:
        date_fill_sections_yuko();
        cd_read_sectors(0x14U, 0x80198000, 0xAD15);
        break;
    case GIRL_MEGUMI:
        date_fill_sections_megumi();
        cd_read_sectors(0x15U, 0x80198000, 0xADC6);
        break;
    default:
        date_fill_sections_yumi();
        cd_read_sectors(0x16U, 0x80198000, 0xADF1);
        break;
    }
    mode_step_next();
}

void date_load_shared_script(void) {
    cd_read_sectors(2, 0x80197000, 0xAF46);
    date_shared_sections_init();
    mode_step_next();
}

void date_load_choice_tables(void) {
    cd_read_sectors(3, 0x801A4000, 0xAFA9);
    date_choice_tables_init_body();
    mode_step_next();
}

s32 date_sea_score_step(void) {
    /* FAKE: 4 spare bytes reproduce previous_step's stack home at 0x28(sp). T-11140 */
    s32 fake_padding;
    u32 previous_step;

    previous_step = game_state.mode_step;
    if (D_800CA2FC == 1) {
        switch (game_state.current_girl) {
        case GIRL_SHIORI:
        case GIRL_AYAKO:
        case GIRL_SAKI:
        case GIRL_MEGUMI:
            normal_date_girl_in();
            if (previous_step != game_state.mode_step) {
                game_state.mode_step -= 1;
                date_choice_score();
            }
            return 0;
        case GIRL_YUKARI:
            if (D_80122CDC != 0) {
                normal_date_girl_in();
                if (previous_step != game_state.mode_step) {
                    game_state.mode_step -= 1;
                    date_choice_score();
                }
                return 0;
            }
            break;
        case GIRL_YUKO:
            if (D_80122CDC < 2) {
                normal_date_girl_in();
                if (previous_step != game_state.mode_step) {
                    game_state.mode_step -= 1;
                    date_choice_score();
                }
                return 0;
            }
            break;
        }
    }
    date_choice_score();
}

s32 date_sea_variant_portrait_gate(void) {
    if (D_800CA2FC == 1) {
        switch (game_state.current_girl) {
        case GIRL_SHIORI:
        case GIRL_AYAKO:
        case GIRL_SAKI:
        case GIRL_YUKARI:
        case GIRL_YUKO:
        case GIRL_MEGUMI:
            mode_step_next();
            return 0;
        }
    }
    return normal_date_girl_in();
}

void date_place_bgm_load_start(void) {
    /* FAKE: 4-byte stack padding reproduces the 0x138-byte frame and stack offsets of the copied tables. T-24083 */
    s32 fake_padding;
    DatePlaceAudioValues words_a;
    DatePlaceAudioValues words_b;
    DatePlaceAudioValues words_c;
    s32 arg1;
    s32 arg2;
    DatePlaceAudioCommands next_ids;

    words_a = date_place_bank_third_argument;
    words_b = date_place_bank_first_data_argument;
    words_c = date_place_bank_second_data_argument;
    /* FAKE: dead read of D_800CA2F8 before the first call creates its live unit in the entry block so uopt colors D_800CA2F8 across the branch chain. T-24083 */
    arg1 = D_800CA2F8;
    arg1 = 0xBF98;
    arg2 = 0xBF79;
    next_ids = date_place_load_command;
    audio_command(0xCF);
    cd_read_checked = 0;
    if (D_800CA2F8 == 2) {
        arg1 = 0xC517;
        arg2 = 0xC514;
    } else if (D_800CA2F8 == 0x11) {
        arg1 = 0xC57E;
        arg2 = 0xC56F;
    } else if ((D_800CA2F8 == 1) || (D_800CA2F8 == 3)) {
        arg1 = 0xC1C3;
        arg2 = 0xC1AD;
    } else if ((D_800CA2F8 == 0xA) || (D_800CA2F8 == 9)) {
        arg1 = 0xC1D2;
        arg2 = 0xC1C5;
    } else if (D_800CA2F8 == 0xC) {
        arg1 = 0xC657;
        arg2 = 0xC623;
    } else if ((D_800CA2F8 == 0x11) || (D_800CA2F8 == 0x10)) {
        arg1 = 0xC57E;
        arg2 = 0xC56F;
    } else if ((D_800CA2F8 == 0x14) || (D_800CA2F8 == 0x13)) {
        arg1 = 0xC611;
        arg2 = 0xC604;
    } else if (D_800CA2F8 == 8) {
        arg1 = 0xC0C0;
        arg2 = 0xC0B6;
    } else if (D_800CA2F8 == 7) {
        arg1 = 0xC493;
        arg2 = 0xC47A;
    } else if (D_800CA2F8 == 0x12) {
        arg1 = 0xC4C5;
        arg2 = 0xC495;
    } else if (D_800CA2F8 == 0xE) {
        arg1 = 0xC5C9;
        arg2 = 0xC5B9;
    }
    audio_bank_load_start(1, arg1, arg2, words_b.by_place[D_800CA2F8], words_c.by_place[D_800CA2F8], words_a.by_place[D_800CA2F8]);
    if (audio_bank_load_poll() == 1) {
        audio_command(next_ids.by_place[D_800CA2F8]);
        mode_step_next();
    }
    mode_step_next();
}

void date_place_bgm_load_wait(void) {
    /* FAKE: 4 spare bytes reproduce the original frame (0x60) and the table's offset 0x30; real locals unknown. T-9180 */
    s32 fake_padding;
    DatePlaceAudioCommands tbl;

    tbl = date_place_sequence_command;
    if (audio_bank_load_poll() == 1) {
        audio_command(tbl.by_place[D_800CA2F8]);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void date_shopping_street_bgm(void) {
    if ((u8) game_state.mode_step >= 0x14U) {
        audio_command(0x203);
    } else if (date_shop_index == DATE_SHOP_BOUTIQUE) {
        audio_command(0x200);
    } else if (date_shop_index == DATE_SHOP_FANCY_SHOP) {
        audio_command(0x201);
    } else {
        audio_command(0x202);
    }
    mode_step_next();
}

s32 date_meeting_place_check(void) {
    /* FAKE: unused word reserves top frame slot so DatePlaceNameTable place_names sits at sp+0x2C in a 0x308 frame, and dead entry read lets cvt_pass strip CVTs on unk_724..unk_727 (same as EVENT twin func_8011AC18, T-11110). T-24079 */
    s32 fake_place_sum;
    DatePlaceNameTable place_names;

    fake_place_sum = game_state.appointment_place_0 + game_state.appointment_place_1 + game_state.appointment_place_2 + game_state.appointment_place_3;
    place_names = date_place_names;
    message_window_title_show(1);
    /* FAKE: u8 view on the store and (u8) cast on the call result keep the original $v0/$a0 load and ugen t-register numbering. T-24079 */
    D_80122CD4 = *(u8 *)&game_state.current_girl;
    if (((u8)get_g_zyotai_s(game_state.current_girl) & 0x7F) == 4) {
        date_meeting_group = DATE_MEETING_REFUSES;
        game_state.mode_step += 0x24;
        return;
    }
    if (((game_state.appointment_place_1 == game_state.appointment_place_0) && (date_arrival == DATE_ARRIVAL_DIRECT_GREETING)) ||
        (game_state.appointment_place_2 == 0xFF) ||
        ((game_state.appointment_place_2 == game_state.appointment_place_1) && (game_state.appointment_place_3 == 0xFF)) ||
        ((game_state.appointment_place_2 == game_state.appointment_place_1) && (game_state.appointment_place_3 == game_state.appointment_place_1))) {
        game_state.mode_step += 0x11;
        return;
    }
    if (game_state.appointment_place_2 == game_state.appointment_place_0) {
        strcpy(name_code_a, place_names.names[game_state.appointment_place_1]);
    } else {
        strcpy(name_code_a, place_names.names[(s32)game_state.appointment_place_2]);
    }
    if (game_state.appointment_place_3 != 0xFF) {
        date_meeting_group = DATE_MEETING_ASK_THREE;
        if (game_state.appointment_place_3 == game_state.appointment_place_0) {
            strcpy(name_code_m, place_names.names[game_state.appointment_place_1]);
        } else {
            strcpy(name_code_m, place_names.names[(s32)game_state.appointment_place_3]);
        }
    }
    mode_step_next();
}

void date_place_init_with_cost(void) {
    place_init();
    if ((D_800CA2F8 == 0xB) && (D_800CA2FC == 2)) {
        talk_group += date_groups_per_variant;
    } else {
        talk_group += D_800CA2FC * date_groups_per_variant;
    }
    talk_line = 0;
}

void date_meeting_place_resolve(void) {
    /* FAKE: unused locals fake_padding frame to 0x38 with previous_step at sp+0x2C, and dead date_arrival read forces the second lbu on date_arrival after date_arrival = 2. T-24079 */
    u8 girl_state;
    u8 fake_arrival_copy;
    s32 fake_padding;
    s32 previous_step;

    fake_arrival_copy = date_arrival;
    if ((game_state.appointment_place_1 == game_state.appointment_place_0) && (D_80122CDC == 0)) {
        previous_step = game_state.mode_step;
        date_wrong_place_speak();
        if (previous_step != game_state.mode_step) {
            date_player_waited = 1;
            date_arrival_group += 1;
            game_state.mode_step += 0xC;
        }
        return;
    }
    if ((game_state.appointment_place_1 != game_state.appointment_place_0) && (D_80122CDC == 0)) {
        /* FAKE: empty test on unk_110A loads it into $v1 before addiu. T-24079 */
        if ((s32)game_state.mode_step) {}
        game_state.mode_step += 0x1E;
        return;
    }
    date_meeting_group += D_80122CDC;
    date_arrival = DATE_ARRIVAL_CORRECTED_PLACE;
    girl_state = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    date_arrival_group = ((girl_state >= 2) + (girl_state >= 3)) * ((date_arrival == DATE_ARRIVAL_WAIT_GREETING) + 1) + date_arrival * 6 - (date_arrival == DATE_ARRIVAL_CORRECTED_PLACE) * 3;
    if (D_80122CDC == 1) {
        if (game_state.appointment_place_2 == game_state.appointment_place_0) {
            D_800CA2F8 = game_state.appointment_place_1;
        } else {
            D_800CA2F8 = game_state.appointment_place_2;
        }
        talk_group = 0;
    }
    if (D_80122CDC == 2) {
        if (game_state.appointment_place_3 == game_state.appointment_place_0) {
            D_800CA2F8 = game_state.appointment_place_1;
            talk_group = 0;
        } else {
            D_800CA2F8 = game_state.appointment_place_3;
            /* FAKE: u16 view gives STR dtype L instead of J so uopt keeps talk_group = 0 inside both arms instead of hoisting it above the branch. T-24079 */
            *(u16 *)&talk_group = 0;
        }
    }
    game_state.event_log_count -= 1;
    mode_step_next();
}

void date_wrong_place_outcome(void) {
    u8 *temp_v0;

    if (game_state.appointment_place_1 != D_800CA2F8) {
        date_meeting_group = DATE_MEETING_NOT_HERE;
        game_state.mode_step += 0x14;
        return;
    }
    mode_step_next();
    mode_step_next();
    game_state.girls[game_state.current_girl].syoushin += 8;
    check_para_limit();
}

void date_meeting_place_select(void) {
    if (game_state.appointment_place_3 != 0xFF) {
        normal_date_three_select();
        return;
    }
    normal_date_two_select();
}

void date_empty_step(void) {
    mode_step_next();
}

void date_portrait_address_init_step(void) {
    addr_init_bustup();
    mode_step_next();
}

void date_meeting_set_not_here_group(void) {
    date_meeting_group = DATE_MEETING_NOT_HERE;
    mode_step_next();
}

void date_looks_check(void) {
    u8 girl_id;

    girl_id = game_state.current_girl;
    if ((game_state.param_looks.value < 0x1E) || ((game_state.param_looks.value < 0x23) && (girl_id == 7))) {
        date_arrival_group = 0xC;
        game_state.mode_step += 0xF;
        game_state.girls[game_state.current_girl].tokimeki -= 1;
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        return;
    }
    mode_step_next();
}

void date_shrine_fireworks_setup(void) {
    s16 i;
    ActorFixed *p;

    if (D_800B5939 == 1) {
        for (i = 11; i < 16; i++) {
            game_state.tpage_buf[i] = -1;
            game_state.tpage_buf[i + 32] = -1;
        }
        bg_lba_cache[2] = 0;
    } else {
        for (i = 21; i < 26; i++) {
            game_state.tpage_buf[i] = -1;
            game_state.tpage_buf[i + 16] = -1;
        }
        bg_lba_cache[3] = 0;
    }
    D_800B5BD4 = 13;
    D_8015BD3C = 0x80;
    D_80122D14 = 1;
    D_8015BD34 = 0;
    D_8015BD38 = 1;
    date_fireworks_resources_init();
    load_palette(face_palette, 15, 1, 2, 0);
    ((ActorFixed *)actor_pool)[31].unk_43 = 15;
    ((SpriteQuad *)sprite_quads)[0].unk_12 = 0x1EF;
    ((SpriteQuad *)sprite_quads)[1].unk_12 = 0x1EF;
    for (i = 16; i < 32; i++) {
        load_palette(D_8015B5D0, i & 0xFF, 1, 0, i - 16);
    }
    load_palette(D_8015B5D4, 14, 1, 0, 0);
    load_palette(D_8015B5D8, 13, 1, 0, 0);
    srand(game_state.frame_count);
    for (i = 0; i < 10; i++) {
        actor_clear(i + 96);
        p = (ActorFixed *)actor_pool + i;
        p[96].unk_01 = 13;
        p[96].flags = 1;
        p[96].quad_flags = 0x1000000;
        p[96].draw_flags = 4;
        p[96].frame_table = D_8015B5C4;
        p[96].sequence_table = D_8015B5C8;
        p[96].tpage_table = D_8015B5C0;
        p[96].unk_14 = D_8015B5CC;
        p[96].sequence_slot = 3;
        p[96].frame = 0;
        p[96].unk_06 = 1;
        p[96].unk_43 = 0;
        p[96].x.h[1] = (rand() % 256) - 0x80;
        p[96].y.h[1] = 0xA0;
        p[96].unk_07 = 0;
        p[96].unk_05 = 0x80;
        p[96].unk_04 = 8;
        p[96].dx = (rand() % 0x8000) - 0x4000;
        date_firework_targets[i].w = ((u32)-(rand() % 9) << 16) - 0x280000;
    }
    ((ActorFixed *)actor_pool)[96].draw_flags |= 0x80;
    scene_still_show(D_8015B5DC, D_8015B5E0, D_8015B5E4, D_8015B5E8, D_8015B5EC, D_8015B5F0);
    for (i = 58; i < 64; i++) {
        ((SpriteQuad *)sprite_quads)[i].flags &= 0x7FFFFFFF;
        ((SpriteQuad *)sprite_quads)[i].unk_12 = 0x1EE;
        game_state.quad_layer[i] = 12;
    }
    for (i = 58; i < 64; i++) {
        ((SpriteQuad *)sprite_quads)[i].tint_r = ((SpriteQuad *)sprite_quads)[i].tint_g = ((SpriteQuad *)sprite_quads)[i].tint_b = 0x80;
    }
    mode_step_next();
}

void date_shrine_wait_long(void) {
    wait_sub_sub(0x3E8);
}

/* FAKE: the first chain is dead (uopt drops its stores as overwritten); it moves the shared `li v0,0x80` above the second `lui $at` as in the original. Found with the permuter, T-9380 */
void date_shrine_light_up(void) {
    (&D_80121808)[0] = (&D_80121809)[0] = (&D_8012180A)[0] = (&D_801217E4)[0] = (&D_801217E5)[0] = (&D_801217E6)[0] = (&D_8011F513)[0] = 0x80;
    D_80122CF4 = 1;
    (&D_80121808)[0] = (&D_80121809)[0] = (&D_8012180A)[0] = (&D_801217E4)[0] = (&D_801217E5)[0] = (&D_801217E6)[0] = (&D_8011F513)[0] = 0x80;
    mode_step_next();
}

void date_shrine_fireworks_fade(void) {
    s16 i;

    D_8015BD3C -= 4;
    ((ActorFixed *)actor_pool)[31].unk_07 = D_8015BD3C;
    ((SpriteQuad *)sprite_quads)[0].tint_r = ((SpriteQuad *)sprite_quads)[0].tint_g = ((SpriteQuad *)sprite_quads)[0].tint_b = ((ActorFixed *)actor_pool)[31].unk_07;
    ((SpriteQuad *)sprite_quads)[1].tint_r = ((SpriteQuad *)sprite_quads)[1].tint_g = ((SpriteQuad *)sprite_quads)[1].tint_b = ((ActorFixed *)actor_pool)[31].unk_07;
    for (i = 0; i < 4; i++) {
        ((ActorFixed *)actor_pool)[i + 96].unk_07 = ((ActorFixed *)actor_pool)[31].unk_07;
    }
    for (i = 58; i < 64; i++) {
        ((SpriteQuad *)sprite_quads)[i].tint_r = ((SpriteQuad *)sprite_quads)[i].tint_g = ((SpriteQuad *)sprite_quads)[i].tint_b = ((ActorFixed *)actor_pool)[31].unk_07;
    }
    if (((ActorFixed *)actor_pool)[31].unk_07 < 0x11) {
        D_80122D14 = 0;
    }
    if (((ActorFixed *)actor_pool)[31].unk_07 == 0) {
        /* FAKE: dead indirect store to D_80122CF4 is overwritten below; uopt drops the store and schedules lui $a0, 0x8000 into the beqz delay slot (same as date_shrine_light_up). T-24099 */
        (&D_80122CF4)[0] = 0x80000000;
        D_80122D14 = 0;
        D_80122CF4 = 0;
        for (i = 0; i < 10; i++) {
            ((ActorFixed *)actor_pool)[i + 96].draw_flags &= 0x7F;
        }
        ((ActorFixed *)actor_pool)[31].draw_flags &= ~0x80;
        ((SpriteQuad *)sprite_quads)[0].tint_r = ((SpriteQuad *)sprite_quads)[0].tint_g = ((SpriteQuad *)sprite_quads)[0].tint_b = 0x80;
        ((SpriteQuad *)sprite_quads)[1].tint_r = ((SpriteQuad *)sprite_quads)[1].tint_g = ((SpriteQuad *)sprite_quads)[1].tint_b = 0x80;
        ((SpriteQuad *)sprite_quads)[0].flags |= 0x80000000;
        ((SpriteQuad *)sprite_quads)[1].flags |= 0x80000000;
        for (i = 58; i < 64; i++) {
            ((SpriteQuad *)sprite_quads)[i].flags |= 0x80000000;
        }
        load_palette(face_palette, 0x10, 1, 2, 0);
        for (i = 11; i < 16; i++) {
            game_state.tpage_buf[i] = 0x77;
            game_state.tpage_buf[i + 32] = 0x77;
        }
        for (i = 21; i < 26; i++) {
            game_state.tpage_buf[i] = 0x77;
            game_state.tpage_buf[i + 32] = 0x77;
        }
        audio_command(0x205);
        actor_clear_upper();
        text_marker_hide();
        D_800B5BD4 = 0xF;
        mode_step_next();
    }
}

void date_shrine_fireworks_update(void) {
    s16 i;
    s32 y_diff;
    u8 unused;

    /* FAKE: dead load before the first call lets cvt_pass strip the CVTs of game_state.mode_step, so uopt keeps it in $a1. T-24099 */
    unused = game_state.mode_step;
    D_8015BD34 += 1;
    if ((D_8015BD34 % ((rand() % 60) + 60)) == 0) {
        if (D_8015BD38 < 4) {
            D_8015BD38 += 1;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].sequence_slot = 3;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].draw_flags |= 0x80;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].frame = 0;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].unk_08 = 0;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].flags = 1;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].y.h[1] = 0xA0;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].dx = (rand() % 0x8000) - 0x4000;
            date_firework_targets[D_8015BD38 - 1].w = ((u32)-(rand() % 9) << 16) - 0x480000;
            ((ActorFixed *)actor_pool)[D_8015BD38 + 95].x.h[1] = (rand() % 256) - 0x80;
        }
    }
    if (game_state.mode_step >= 0x26U) {
        D_8015BD38 = 1;
        for (i = 1; i < 4; i++) {
            ((ActorFixed *)actor_pool)[i + 96].draw_flags &= 0x7F;
        }
    }
    for (i = 0; i < D_8015BD38; i++) {
        if (((ActorFixed *)actor_pool)[i + 96].sequence_slot == 3) {
            ((ActorFixed *)actor_pool)[i + 96].y.w += (date_firework_targets[i].w - ((ActorFixed *)actor_pool)[i + 96].y.w) >> 5;
            ((ActorFixed *)actor_pool)[i + 96].x.w += ((ActorFixed *)actor_pool)[i + 96].dx;
            y_diff = ((ActorFixed *)actor_pool)[i + 96].y.h[1] - (s16)date_firework_targets[i].h[1];
            ((ActorFixed *)actor_pool)[i + 96].unk_07 = (y_diff / 2) + 0x40;
            if (y_diff < 10) {
                ((ActorFixed *)actor_pool)[i + 96].sequence_slot = (((ActorFixed *)actor_pool)[i + 96].sequence_slot - (rand() % 2)) - 1;
                if ((game_state.mode_step == 0x25) && ((u32)game_state.mode_step_timer.w >= 0x1F5U)) {
                    if (i < 2) {
                        ((ActorFixed *)actor_pool)[i + 96].sequence_slot = 0;
                    }
                }
                ((ActorFixed *)actor_pool)[i + 96].frame = 0;
                ((ActorFixed *)actor_pool)[i + 96].unk_08 = 0;
                ((ActorFixed *)actor_pool)[i + 96].flags = 3;
                ((ActorFixed *)actor_pool)[i + 96].unk_07 = 0x80;
            }
        } else if ((((ActorFixed *)actor_pool)[i + 96].frame == 0) && (((ActorFixed *)actor_pool)[i + 96].unk_08 == 0)) {
            if ((((ActorFixed *)actor_pool)[i + 96].draw_flags & 0x80) && (((ActorFixed *)actor_pool)[i + 96].sequence_slot != 0)) {
                audio_command(0x500);
            }
            if ((game_state.mode_step == 0x25) && ((u32)game_state.mode_step_timer.w >= 0xC9U) && ((u32)game_state.mode_step_timer.w < 0x1F5U)) {
                ((ActorFixed *)actor_pool)[i + 96].draw_flags &= ~0x80;
            } else if ((game_state.mode_step == 0x25) && ((u32)game_state.mode_step_timer.w >= 0x2BDU)) {
                ((ActorFixed *)actor_pool)[i + 96].draw_flags &= ~0x80;
            } else if ((game_state.mode_step == 0x25) && ((u32)game_state.mode_step_timer.w >= 0x1F5U) && (i >= 2)) {
                ((ActorFixed *)actor_pool)[i + 96].draw_flags &= ~0x80;
            } else {
                ((ActorFixed *)actor_pool)[i + 96].draw_flags |= 0x80;
                ((ActorFixed *)actor_pool)[i + 96].sequence_slot = 3;
                ((ActorFixed *)actor_pool)[i + 96].frame = 0;
                ((ActorFixed *)actor_pool)[i + 96].unk_08 = 0;
                ((ActorFixed *)actor_pool)[i + 96].flags = 1;
                ((ActorFixed *)actor_pool)[i + 96].y.h[1] = 0xA0;
                ((ActorFixed *)actor_pool)[i + 96].dx = (rand() % 0x8000) - 0x4000;
                date_firework_targets[i].w = ((u32)-(rand() % 9) << 16) - 0x280000;
                ((ActorFixed *)actor_pool)[i + 96].x.h[1] = (rand() % 256) - 0x80;
                if ((game_state.mode_step == 0x25) && ((u32)game_state.mode_step_timer.w >= 0x1F5U)) {
                    if (i == 0) {
                        ((ActorFixed *)actor_pool)[i + 96].x.h[1] = -0x50;
                    } else if (i == 1) {
                        ((ActorFixed *)actor_pool)[i + 96].x.h[1] = 0x50;
                    } else {
                        ((ActorFixed *)actor_pool)[i + 96].draw_flags &= 0x7F;
                    }
                }
            }
        } else if ((((ActorFixed *)actor_pool)[i + 96].sequence_slot == 0) && (((ActorFixed *)actor_pool)[i + 96].frame == 10) && (((ActorFixed *)actor_pool)[i + 96].unk_08 == 0)) {
            audio_command(0x500);
        }
    }
    if ((u8)D_8015BD3C < 0x80U) {
        prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA0, 0xE, D_8015BD3C, 4);
    } else {
        prim_gradient_rect(-0xA0, -0x78, 0x140, 0xA0, 0xE, 0xC0, 4);
    }
    dtd_on(0xE);
}

void date_central_park_load_particles(void) {
    cd_read_sectors(0x22, 0x801E0000, 0xBC13);
    date_central_park_particles_init();
    mode_step_next();
}

/* FAKE: s16 `two` holds the hoisted constant 2 in $s6 with reversed bne/xor operand order, and local u8 prototype for load_palette emits andi a3, s6, 0xff. T-11290 */
s32 date_central_park_particles_setup(void) {
    s32 drift;
    s16 size_variant;
    s16 i;
    s32 scale;
    s32 divisor;
    s32 random_span;
    s32 choice;
    WordView *position;
    s16 two;
    s32 load_palette(s32, u8, u8, u8, u8);

    if ((*(s32 *)D_8015B60C != 0x801E4000) || (((s32 *)D_8015B610)[0] != 0x801E8000) || (((s32 *)D_8015B610)[1] != 0x801EC000) ||
        (((s32 *)D_8015B61C)[0] != 0x801F0540) || (((s32 *)D_8015B61C)[1] != 0x801F0558) || (((s32 *)D_8015B61C)[2] != 0x801F0570) ||
        (((s32 *)D_8015B61C)[3] != 0x801F058C) || (((s32 *)D_8015B620)[0] != 0x801F0B64) || (((s32 *)D_8015B620)[1] != 0x801F0B80)) {
        game_state.mode_step -= 2;
        return 0;
    }
    two = 2;
    if (two == D_800CA2FC) {
        load_palette(D_8015B62C, 0x12, 1, (s32)two, 0);
    } else {
        load_palette(D_8015B630, 0x12, 1, (s32)two, 0);
    }
    for (i = 0; i < 0x14; i++) {
        actor_clear(i + 0x62);
        ((ActorFixed *) actor_pool)[i + 98].quad_flags = 0x01000000;
        ((ActorFixed *) actor_pool)[i + 98].unk_01 = 0xD;
        ((ActorFixed *) actor_pool)[i + 98].flags = 1;
        ((ActorFixed *) actor_pool)[i + 98].draw_flags = 4;
        if (two == D_800CA2FC) {
            ((ActorFixed *) actor_pool)[i + 98].frame_table = (u8 *)D_8015B614;
            ((ActorFixed *) actor_pool)[i + 98].sequence_table = (u8 *)D_8015B61C;
            ((ActorFixed *) actor_pool)[i + 98].tpage_table = (u8 *)D_8015B60C;
        } else {
            ((ActorFixed *) actor_pool)[i + 98].frame_table = (u8 *)D_8015B618;
            ((ActorFixed *) actor_pool)[i + 98].sequence_table = (u8 *)D_8015B620;
            ((ActorFixed *) actor_pool)[i + 98].tpage_table = (u8 *)D_8015B610;
        }
        ((ActorFixed *) actor_pool)[i + 98].sequence_slot = (s16) (rand() % 8);
        ((ActorFixed *) actor_pool)[i + 98].unk_43 = 0x12;
        choice = rand() % 2;
        scale = 0x1000 / ((s32) ((((s16) choice) * 2) + 2));
        size_variant = (s16) choice;
        ((ActorFixed *) actor_pool)[i + 98].unk_1A = (s16) scale;
        ((ActorFixed *) actor_pool)[i + 98].unk_1C = (s16) scale;
        ((ActorFixed *) actor_pool)[i + 98].unk_07 = 0x80;
        position = &date_particle_targets[i];
        ((ActorFixed *) actor_pool)[i + 98].x.h[1] = (s16) ((rand() % 300) - 0x96);
        position->w = (s32) ((ActorFixed *) actor_pool)[i + 98].x.w;
        random_span = rand();
        divisor = size_variant + 1;
        drift = ((s32) (((((rand() % 2) * 2) - 1) * (random_span % 8)) + 8)) / divisor;
        position->h[1] = (s16) (((s16) position->h[1]) + drift);
        ((ActorFixed *) actor_pool)[i + 98].y.h[1] = (s16) ((rand() % 160) - 0x78);
        ((ActorFixed *) actor_pool)[i + 98].dy = (s32) (((two != D_800CA2FC) << 15) + 0x8000) / divisor;
    }

    mode_step_next();
}

void date_central_park_particles_hide(void) {
    s16 i;

    for (i = 0; i < 20; i++) {
        ((Actor *)actor_pool)[i + 98].draw_flags &= 0x7F;
    }
}

/* s32 with no return (K&R): the early returns set $v0 = 0; falling off the loop leaves $v0 untouched. */
s32 date_central_park_particles_update(void) {
    s16 i;
    s16 size_variant;
    s32 choice;
    s32 scale;
    s32 random_span;
    s32 divisor;

    if (D_800CA2FC == 1) {
        return 0;
    }
    if ((D_800CA2FC != 2) && ((game_state.month < 9U) || (game_state.month >= 12U))) {
        return 0;
    }
    for (i = 0; i < 20; i++) {
        ((ActorFixed *)actor_pool)[i + 98].unk_07 = bg_brightness;
        ((ActorFixed *)actor_pool)[i + 98].draw_flags |= 0x80;
        ((ActorFixed *)actor_pool)[i + 98].dx += (date_particle_targets[i].w - ((ActorFixed *)actor_pool)[i + 98].x.w) >> 8;
        ((ActorFixed *)actor_pool)[i + 98].x.w += ((ActorFixed *)actor_pool)[i + 98].dx;
        ((ActorFixed *)actor_pool)[i + 98].y.w += ((ActorFixed *)actor_pool)[i + 98].dy;
        if (((ActorFixed *)actor_pool)[i + 98].y.h[1] >= 0x29) {
            ((ActorFixed *)actor_pool)[i + 98].y.h[1] = -0x78;
            choice = rand() % 2;
            size_variant = (s16)choice;
            scale = 0x1000 / (((s16)choice * 2) + 2);
            ((ActorFixed *)actor_pool)[i + 98].unk_1A = scale;
            ((ActorFixed *)actor_pool)[i + 98].unk_1C = scale;
            ((ActorFixed *)actor_pool)[i + 98].x.h[1] = (rand() % 300) - 0x96;
            date_particle_targets[i].w = ((ActorFixed *)actor_pool)[i + 98].x.w;
            random_span = rand();
            divisor = size_variant + 1;
            /* FAKE: s16 lvalue cast on WordView.h[1] emits lh with += operand order in ugen. T-24083 */
            *(s16 *)&date_particle_targets[i].h[1] += ((((rand() % 2) * 2) - 1) * (random_span % 8) + 8) / divisor;
            ((ActorFixed *)actor_pool)[i + 98].dy = (((D_800CA2FC != 2) << 15) + 0x8000) / divisor;
        }
    }
}

s32 date_mira_late_skip(void) {
    u32 girl_state;

    girl_state = (u8) get_g_zyotai_s(game_state.date_girl) & 0x7F;
    if ((game_state.date_girl == GIRL_MIRA) && (girl_state >= 3U) && (date_arrival == DATE_ARRIVAL_CORRECTED_PLACE)) {
        date_meeting_group = DATE_MEETING_REFUSES;
        mode_step_set(0x34);
        D_800CA2F8 += 0x32;
        return 0;
    }
    mode_step_next();
}

void date_invited_day_check(void) {
    /* FAKE: pointer view prevents uopt from CSEing unk_03E across || arms; u8 cast consumes $t8. T-11140 */
    GameState *game = &game_state;
    s32 girl_state_flags;

    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    if (((game_state.year == 0x5F) && (game_state.month == 0xA) && (game_state.day == 0xF) &&
         game_state.girls[game_state.current_girl].flags.f.b12) ||
        ((game->year == 0x60) && (game_state.month == 0xA) && (game_state.day == 0xD) &&
         game_state.girls[game_state.current_girl].flags.f.b13)) {
        game_state.girls[game_state.current_girl].flags.b[1] &= 0xFFEF;
        game_state.girls[game_state.current_girl].flags.b[1] &= 0xFFDF;
        if (((u8)girl_state_flags & 0x7F) != 4) {
            mode_step_next();
            return;
        }
        mode_step_next();
        mode_step_next();
        return;
    }
    mode_step_next();
    mode_step_next();
}

s32 date_invited_day_talk(void) {
    u8 girl_state_flags;
    u32 girl_state;

    girl_state_flags = get_g_zyotai_s(game_state.current_girl);
    girl_state = girl_state_flags & 0x7F;
    *(s16 *) &D_800CA158 = (girl_state >= 2U) + (girl_state >= 3U) + ((game_state.year >= 0x60U) * 3) + 0xC;
    talk_tables = (u8 *) &D_800CA158;
    D_800CA138 = &D_800CA15C;
    D_800CA13C = D_8015BE18;
    D_800CA140 = D_8015BE84;
    D_800CA144 = D_8015BEF0;
    return talk_run(D_80122CDC, 1, 0);
}

/* FAKE: unsigned byte view and integer fallthrough retain palette-state address scheduling. T-9550 */
s32 date_parade_sign_palette_cycle(void) {

    if ((u32)game_state.frame_count % 7U == 0) {
        (*(u8 *)&date_sign_palette_frame)++;
        if (*(u8 *)&date_sign_palette_frame >= 4U) date_sign_palette_frame = 0;
        load_palette((s32)D_8015B644, 0x11, 1, 3, *(u8 *)&date_sign_palette_frame);
    }
}

void date_girl_in_clear_appointment(void) {
    normal_date_girl_in();
    game_state.unk_54C[0].b[3] &= 0xFFDF;
}

void date_ambush_audio_bank_load_start(void) {
    audio_bank_load_start(0, 0xC043, 0xC01D, 0, 0, 0);
    if (audio_bank_load_poll() == 1) {
        audio_command(0xBF);
        mode_step_next();
    }
    mode_step_next();
}

void date_ambush_audio_bank_load_wait(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0xBF);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void date_ambush_bang(void) {
    k_reset(1);
    set_kanji_string(-0x80, 0x30, 1, "ドン", 0);
    audio_command(0x22);
    audio_command(0x21);
    audio_command(0x2F);
    audio_command(0x500);
    mode_step_next();
}

void date_shared_sections_init(void) {
    date_shared_section_text = 0x801972E8;
    D_8015BD94 = 0x8019772C;
    D_8015BD98 = 0x80197B04;
    D_8015BD9C = 0x80197EA4;
    date_shared_section_command = 0x801972FC;
    D_8015BDA4 = 0x80197778;
    D_8015BDA8 = 0x80197B10;
    D_8015BDAC = 0x80197EB4;
    date_shared_section_voice = 0x80197450;
    D_8015BDB4 = 0x80197994;
    D_8015BDB8 = 0x80197BD8;
    D_8015BDBC = 0x80197FEC;
}

void date_fill_sections_shiori(void) {
    date_section_text = 0x801986D0;
    D_8015BDC4 = 0x80198DCC;
    D_8015BDC8 = 0x801991F0;
    D_8015BDCC = 0x801996C0;
    D_8015BDD0 = 0x8019A73C;
    D_8015BDD4 = 0x8019AEC0;
    D_8015BDD8 = 0x8019BDC8;
    D_8015BDDC = 0x8019C1B8;
    D_8015BDE0 = 0x8019CDF0;
    D_8015BDE4 = 0x8019D048;
    D_8015BDE8 = 0x8019D39C;
    D_8015BDEC = 0x8019DA2C;
    D_8015BDF0 = 0x8019E030;
    D_8015BDF4 = 0x8019E3BC;
    D_8015BDF8 = 0x8019ED18;
    D_8015BDFC = 0x8019F41C;
    D_8015BE00 = 0x8019F7E0;
    D_8015BE04 = 0x8019FEE4;
    D_8015BE08 = 0x801A0540;
    D_8015BE0C = 0x801A09D4;
    D_8015BE10 = 0x801A22B8;
    D_8015BE14 = 0x801A33B0;
    D_8015BE18 = 0x80199F04;
    D_8015BE1C = 0x8019B658;
    D_8015BE20 = 0x8019C4B4;
    D_8015BE24 = 0x8019C94C;
    D_8015BE28 = 0x8019DD58;
    date_section_command = 0x80198768;
    D_8015BE30 = 0x80198DFC;
    D_8015BE34 = 0x80199220;
    D_8015BE38 = 0x80199708;
    D_8015BE3C = 0x8019A7B4;
    D_8015BE40 = 0x8019AF10;
    D_8015BE44 = 0x8019BDF8;
    D_8015BE48 = 0x8019C1E8;
    D_8015BE4C = 0x8019CE28;
    D_8015BE50 = 0x8019D068;
    D_8015BE54 = 0x8019D3CC;
    D_8015BE58 = 0x8019DA9C;
    D_8015BE5C = 0x8019E060;
    D_8015BE60 = 0x8019E3EC;
    D_8015BE64 = 0x8019EDB8;
    D_8015BE68 = 0x8019F44C;
    D_8015BE6C = 0x8019F810;
    D_8015BE70 = 0x8019FF44;
    D_8015BE74 = 0x801A0588;
    D_8015BE78 = 0x801A0A04;
    D_8015BE7C = 0x801A2480;
    D_8015BE80 = 0x801A33F8;
    D_8015BE84 = 0x80199F94;
    D_8015BE88 = 0x8019B6CC;
    D_8015BE8C = 0x8019C4DC;
    D_8015BE90 = 0x8019C978;
    D_8015BE94 = 0x8019DD78;
    date_section_voice = 0x80198AFC;
    D_8015BE9C = 0x80198F2C;
    D_8015BEA0 = 0x80199318;
    D_8015BEA4 = 0x801998D0;
    D_8015BEA8 = 0x8019AA3C;
    D_8015BEAC = 0x8019B0DC;
    D_8015BEB0 = 0x8019BF28;
    D_8015BEB4 = 0x8019C350;
    D_8015BEB8 = 0x8019CF24;
    D_8015BEBC = 0x8019D120;
    D_8015BEC0 = 0x8019D4FC;
    D_8015BEC4 = 0x8019DC94;
    D_8015BEC8 = 0x8019E158;
    D_8015BECC = 0x8019E51C;
    D_8015BED0 = 0x8019F150;
    D_8015BED4 = 0x8019F57C;
    D_8015BED8 = 0x8019F940;
    D_8015BEDC = 0x801A01C0;
    D_8015BEE0 = 0x801A0750;
    D_8015BEE4 = 0x801A0B34;
    D_8015BEE8 = 0x801A2EB0;
    D_8015BEEC = 0x801A3614;
    D_8015BEF0 = 0x8019A3CC;
    D_8015BEF4 = 0x8019BAD8;
    D_8015BEF8 = 0x8019C608;
    D_8015BEFC = 0x8019CAF8;
    D_8015BF00 = 0x8019DDF8;
}

void date_fill_sections_mio(void) {
    date_section_text = 0x801986C8;
    D_8015BDC4 = 0x80198D8C;
    D_8015BDC8 = 0x801991A0;
    D_8015BDCC = 0x801997D0;
    D_8015BDD0 = 0x8019AD68;
    D_8015BDD4 = 0x8019B548;
    D_8015BDD8 = 0x8019C404;
    D_8015BDDC = 0x8019C7D8;
    D_8015BDE0 = 0x8019D4D4;
    D_8015BDE4 = 0x8019D734;
    D_8015BDE8 = 0x8019DA64;
    D_8015BDEC = 0x8019E0A0;
    D_8015BDF0 = 0x8019E754;
    D_8015BDF4 = 0x8019EAD0;
    D_8015BDF8 = 0x8019F354;
    D_8015BDFC = 0x8019F938;
    D_8015BE04 = 0x8019FF48;
    D_8015BE08 = 0x801A05AC;
    D_8015BE0C = 0x801A0A50;
    D_8015BE10 = 0x801A1FCC;
    D_8015BE14 = 0x801A353C;
    D_8015BE18 = 0x8019A30C;
    D_8015BE1C = 0x8019BCCC;
    D_8015BE20 = 0x8019CB24;
    D_8015BE24 = 0x8019D03C;
    D_8015BE28 = 0x8019E3F4;
    date_section_command = 0x80198760;
    D_8015BE30 = 0x80198DBC;
    D_8015BE34 = 0x801991D0;
    D_8015BE38 = 0x80199838;
    D_8015BE3C = 0x8019ADE0;
    D_8015BE40 = 0x8019B598;
    D_8015BE44 = 0x8019C434;
    D_8015BE48 = 0x8019C808;
    D_8015BE4C = 0x8019D50C;
    D_8015BE50 = 0x8019D754;
    D_8015BE54 = 0x8019DA94;
    D_8015BE58 = 0x8019E110;
    D_8015BE5C = 0x8019E784;
    D_8015BE60 = 0x8019EB00;
    D_8015BE64 = 0x8019F3D4;
    D_8015BE68 = 0x8019F968;
    D_8015BE70 = 0x8019FFA8;
    D_8015BE74 = 0x801A05F4;
    D_8015BE78 = 0x801A0A80;
    D_8015BE7C = 0x801A219C;
    D_8015BE80 = 0x801A35B4;
    D_8015BE84 = 0x8019A3BC;
    D_8015BE88 = 0x8019BD3C;
    D_8015BE8C = 0x8019CB50;
    D_8015BE90 = 0x8019D068;
    D_8015BE94 = 0x8019E414;
    date_section_voice = 0x80198AF4;
    D_8015BE9C = 0x80198EEC;
    D_8015BEA0 = 0x801992C8;
    D_8015BEA4 = 0x80199AB8;
    D_8015BEA8 = 0x8019B068;
    D_8015BEAC = 0x8019B764;
    D_8015BEB0 = 0x8019C564;
    D_8015BEB4 = 0x8019C970;
    D_8015BEB8 = 0x8019D608;
    D_8015BEBC = 0x8019D80C;
    D_8015BEC0 = 0x8019DBA8;
    D_8015BEC4 = 0x8019E308;
    D_8015BEC8 = 0x8019E87C;
    D_8015BECC = 0x8019EC30;
    D_8015BED0 = 0x8019F698;
    D_8015BED4 = 0x8019FA98;
    D_8015BEDC = 0x801A0208;
    D_8015BEE0 = 0x801A07BC;
    D_8015BEE4 = 0x801A0BB0;
    D_8015BEE8 = 0x801A2C58;
    D_8015BEEC = 0x801A3938;
    D_8015BEF0 = 0x8019A9B4;
    D_8015BEF4 = 0x8019C12C;
    D_8015BEF8 = 0x8019CCB4;
    D_8015BEFC = 0x8019D204;
    D_8015BF00 = 0x8019E4CC;
}

void date_fill_sections_yuina(void) {
    date_section_text = 0x8019863C;
    D_8015BDC4 = 0x80198CE0;
    D_8015BDC8 = 0x80199074;
    D_8015BDCC = 0x801994E0;
    D_8015BDD0 = 0x8019A698;
    D_8015BDD4 = 0x8019AD74;
    D_8015BDD8 = 0x8019B96C;
    D_8015BDDC = 0x8019BD30;
    D_8015BDE0 = 0x8019C8D8;
    D_8015BDE4 = 0x8019CB24;
    D_8015BDE8 = 0x8019CD88;
    D_8015BDEC = 0x8019D268;
    D_8015BDF0 = 0x8019D7CC;
    D_8015BDF4 = 0x8019DB58;
    D_8015BDF8 = 0x8019E2B8;
    D_8015BDFC = 0x8019E7D8;
    D_8015BE04 = 0x8019EDFC;
    D_8015BE08 = 0x8019F490;
    D_8015BE0C = 0x8019F90C;
    D_8015BE10 = 0x801A0F20;
    D_8015BE14 = 0x801A2014;
    D_8015BE18 = 0x80199DDC;
    D_8015BE1C = 0x8019B35C;
    D_8015BE20 = 0x8019C008;
    D_8015BE24 = 0x8019C4D8;
    D_8015BE28 = 0x8019D584;
    date_section_command = 0x801986D4;
    D_8015BE30 = 0x80198D10;
    D_8015BE34 = 0x801990A4;
    D_8015BE38 = 0x80199528;
    D_8015BE3C = 0x8019A700;
    D_8015BE40 = 0x8019ADC4;
    D_8015BE44 = 0x8019B99C;
    D_8015BE48 = 0x8019BD60;
    D_8015BE4C = 0x8019C910;
    D_8015BE50 = 0x8019CB44;
    D_8015BE54 = 0x8019CDB8;
    D_8015BE58 = 0x8019D2D8;
    D_8015BE5C = 0x8019D7FC;
    D_8015BE60 = 0x8019DB88;
    D_8015BE64 = 0x8019E328;
    D_8015BE68 = 0x8019E808;
    D_8015BE70 = 0x8019EE5C;
    D_8015BE74 = 0x8019F4D8;
    D_8015BE78 = 0x8019F93C;
    D_8015BE7C = 0x801A10EC;
    D_8015BE80 = 0x801A205C;
    D_8015BE84 = 0x80199E78;
    D_8015BE88 = 0x8019B3CC;
    D_8015BE8C = 0x8019C030;
    D_8015BE90 = 0x8019C504;
    D_8015BE94 = 0x8019D5A4;
    date_section_voice = 0x80198A68;
    D_8015BE9C = 0x80198E40;
    D_8015BEA0 = 0x80199180;
    D_8015BEA4 = 0x801996F0;
    D_8015BEA8 = 0x8019A92C;
    D_8015BEAC = 0x8019AF90;
    D_8015BEB0 = 0x8019BACC;
    D_8015BEB4 = 0x8019BEC8;
    D_8015BEB8 = 0x8019CA0C;
    D_8015BEBC = 0x8019CBE0;
    D_8015BEC0 = 0x8019CECC;
    D_8015BEC4 = 0x8019D4D0;
    D_8015BEC8 = 0x8019D8F4;
    D_8015BECC = 0x8019DCB8;
    D_8015BED0 = 0x8019E5AC;
    D_8015BED4 = 0x8019E938;
    D_8015BEDC = 0x8019F0BC;
    D_8015BEE0 = 0x8019F6A0;
    D_8015BEE4 = 0x8019FA6C;
    D_8015BEE8 = 0x801A1B70;
    D_8015BEEC = 0x801A2278;
    D_8015BEF0 = 0x8019A358;
    D_8015BEF4 = 0x8019B714;
    D_8015BEF8 = 0x8019C15C;
    D_8015BEFC = 0x8019C684;
    D_8015BF00 = 0x8019D624;
}
