"""Source-pinned runtime script patch contracts and unresolved writes (T-28001).

This ledger never changes game data. A protected byte slot is not a translation
boundary: changing its position or width requires changing the runtime patch.
Generic AST results are candidate writes, not proof of their ultimate target.
"""
from __future__ import annotations

import hashlib
import re
import struct
import subprocess
from pathlib import Path

from .text_source import (REPOSITORY, _lex_c_tokens, _mask_c_comments, _mask_c_literals,
                          _read_original, _string_bytes, source_link_unit)
from .text import tokenize_text

SOURCE_REBASE = {'accepted_build_source_digest': '687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3',
 'revision': 'd7fb33f960395d4c32ea238aabda82f211689973',
 'source_manifest_sha256': '3fede6401e79c19a0982a596899744433658f749c7702b7c6c28d55dd487c0eb',
 'source_tree_sha256': '75eafb5958b44eef4c1ead490eea7bc06372d094e2cf939854a3ab7b67b638cd'}

SOURCE_PINS = {'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
 'include/game/appointment.h': 'fefe3a4c97bbb2812989d326d065d3948cb42119ed37d0e4ecce393154e3ec2a',
 'include/game/calendar.h': '44e07c9582c54980f4f8fe8c54adc061a0b9dcd68a5fd061014241ceb01974e9',
 'include/game/club.h': '2bfd606956b1baf60ea2e370ad1f11297fd40e721d2303b46cd7915d2b46552d',
 'include/game/day_event.h': 'efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d',
 'include/game/display_records.h': '54590f2da1130d31ded81ba2f52509f1781006646c75865b2f6fb832af8bc77b',
 'include/game/draw_records.h': '77cf6a1567a9877f55fe0238566b08af034c921da8c16f503f2c115f68efdcb2',
 'include/game/game_state.h': 'e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af',
 'include/game/girl.h': '82c7a558ed145d60e82d57f3a83d331d7fee53a9b89a2edd7c56c284e5c09c58',
 'include/game/layer.h': 'b0a33122c990abad593818fd9520ebd7f43250a4f93042492d9c12d851f97c29',
 'include/game/menu_records.h': '1849d0f486659abea223b922fe0d4397e5133387af39f98c50d83780ca9f58fa',
 'include/game/model_records.h': 'a8e261ca83f07087bb6b7a2e08e1a709d9cdbcb99ebede573c6a3fa84e92c1c0',
 'include/game/param.h': '627d01af9cc16b4e3a2819610acf9ee1fd174af5ab768c803facf1a9ec86ba55',
 'include/game/schedule_records.h': 'c895d5dc8902d05039261caa24e3347d02ba5f4b3e1708a4ec855ae6a79311cd',
 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
 'include/libc.h': '2cefe43794e98d5541095189ffe2fd48922d18246342becdab2e8a5b0d44701b',
 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
 'include/libgs.h': '466b559a2e91a91b60448325222f344d376c13d76227e6223379c3678615d10b',
 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
 'include/libsnd.h': '998095abfea131f2e2f489b46a936d2049fccff60689286d7939aa62b1429c30',
 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
 'include/main/color.h': '9d730ad7f3b353bd0014a8ce2fca8a3f44fc13d459917af7c7bb18a08e419965',
 'include/main/cursor.h': 'b4e359b7c6278fce81566e7042e872f2025d0bfb5d2ffa2c83595fb1e02c3164',
 'include/main/date.h': '10742c4761e1cf8c6116eb034701322ab5b05ab1f7632830e53a1c74a9140316',
 'include/main/day_event.h': '983a52118fe5973fa6b42b356f1033a4cb3df32748c07c9f0938ca4866ac355b',
 'include/main/debug.h': '4565888d481634611fed69f0215e97299a49597561b7f9406b805aedf7e2459d',
 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
 'include/main/event_log.h': '3dd48e44393b7d7a0671aad1a8eaf17b60e4e506befb8ba8a147f345fdcc8fa0',
 'include/main/game_mode.h': '104f931cec5e4b9133baccca3eeea596d4966e46d24bf719a082a7931d6a8da5',
 'include/main/girl.h': 'e605fa35dd623771ffcbd60070b7edef7a8beb07942ed19a826be7391c77a8fd',
 'include/main/hud.h': 'dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3',
 'include/main/import_views/entry_ui.h': 'eac3a572e784c88406acc5b4c40c019df3888aadd6df5c6061baeb09bfa49129',
 'include/main/import_views/entry_ui_scenes.h': 'f6ab201e31749a0a649a40cd51f54123fde6967a6d2bca6d0a309326ab3cfedf',
 'include/main/import_views/entry_ui_scenes_school_events.h': '1efff14748e011b3c4980a3636735e0d90c3e4e6dd7af2597ba0beb13c454fbb',
 'include/main/import_views/minigame_scenes_school_events.h': '0eb540cfcd85781805e3f25c11244afc6f657f11416fedf7fd0e0d080a6956c9',
 'include/main/import_views/resident_views.h': 'fe245704b691e495b0e1f4bcc9c1cf9478a99aafc57cf7187afdc3b96a9617dd',
 'include/main/import_views/scenes.h': 'e6e7abb06e10ecefdc007011ef75dc7e386b98344d1b6059542382812241b5ea',
 'include/main/import_views/scenes_school_events.h': '887df254ca70274155d79a878a6f94a9f9c2e7ae32c7d2cd87f11f3f997452a4',
 'include/main/import_views/school_events.h': 'c4c30472d024b447b37decb4ea3ee24a1182ef1735e94d386774e1fad0f08794',
 'include/main/layer.h': '2cca7cdd41d2a6ac4f8ce81adb3285241e802ea181906f4d5f086b969493eb06',
 'include/main/menu.h': 'b171ce815adeee4fdb2c4f86258ebcc2b7c1b75eafc2f26ce7e9dd9494ca35fe',
 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
 'include/main/name_code.h': 'c10233524227a0cb62a681c3cc3724a9c80068c19c9a46c1cfd778835168394b',
 'include/main/ovl.h': '6f9ff12d2c1995b1fab3bf7961ebd1ca69118e1747a6f38c1af55c3db673d455',
 'include/main/pad.h': '66db19784afce6a71b067e93b7a50f56be2431bb5ad31ebba4fc24ceb7dcb19c',
 'include/main/param.h': '9f58341a7b7905bfa5ac090cac1035647052704fedaf2b70422df4f1ffb2185e',
 'include/main/portrait.h': 'a47fd133b66b7e6ac079e890c456ca5e0546cc0fff1eff244146d1a5b9731486',
 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
 'include/main/scene.h': 'faf86435b5c4ae9aa47d2a59c2e7c412f02e8a93cbdae2b3a04f1a507bf80049',
 'include/main/schedule.h': 'f6173aaccf19802a6ff04d01eaef242c0120d2b98482b67cbe377605fe4405ef',
 'include/main/sd.h': 'ab812b3c7374b55b414c8bcb380636566d39edb9d2a005c252107a91065ae470',
 'include/main/select_girl.h': 'a85d48f4d950c163497e58ec4eb38a4f6605f49f2b73c99c820c897ce579edd3',
 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
 'include/main/talk.h': 'ea765428db83f6fda60eb89c7e753180ddfe2893cc9de0391b66b32dc2dadc22',
 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
 'include/main/view.h': '910777739861b865eea1f03d5a232ba14717dc07904aede3770a48e89d8d3198',
 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
 'include/ovl/amusement_park_date.h': '83dfd8ca41b79648d1de1347f79d2d38948f33e6f46ca199777af172b128e19d',
 'include/ovl/date.h': 'f803d8d3a6fce96573a5a4d8b4e4edea8fb8cb80fabe57c0eded344403371080',
 'include/ovl/new_year_christmas.h': '1ca1ad375392e0f0bafc1270b9686b1b9e846b0674f0a18ccf8fb0b911b30814',
 'include/ovl/school_trip.h': 'b50953d21c81520212d1d7195602450ce178b120cb00e22703c59c23078da138',
 'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
 'src/main/game_mode.c': '1e1d80846431688abe111f428a76651d6e70a904eb29bcfb2efae223b421f610',
 'src/main/scene_library.c': 'c8a0315a4d4542dc13c8e1788ad199fafd28d5104bce64f08bb3d3b63e4e1a18',
 'src/ovl/amusement_park_date/scene_choices_and_saved_return.c': '63949bff20a168de619c50001c75a4b2199d2cdf281b534aa514643e2f333b73',
 'src/ovl/date/girl_birthday.c': '04caabe18307cdcb14cbb0981a2d1e8aadb795e0eb732d69fae5e141a5d49ada',
 'src/ovl/new_year_christmas/new_year_dialogue.c': 'a1eadf971417b975731894ec9ec872884a88ad96eb25ff9038c4a11831350577',
 'src/ovl/new_year_christmas/new_year_outcome.c': '7df1122ab9b04474dffa7900fac461751652b8af6e161f1e279070e71fe3e637',
 'src/ovl/new_year_christmas/new_year_scene_setup.c': '798c786c524186945a8e30f828bc92d4e83ea602bc08da6d129cfc9c3868bddd',
 'src/ovl/new_year_christmas/scene_resource_tables.c': '2205afa7f916e2429275a50c9ed635b6366e1238d9d96dda97a64a6d1fe4621b',
 'src/ovl/new_year_christmas/seasonal_mode_stage_dispatch.c': '85113f9f8e1c5c40fbdc30b37b5d72c1aab4bbcb04931bb27b0971630068948f',
 'src/ovl/new_year_christmas/shrine_actor_sequences.c': 'aab52d5d2fc9e910e695cc413e83205242d4b3e64bfd324c0f1a7e9d6491a69d',
 'src/ovl/school_trip/destination_choice_and_detour_routing.c': 'e626e6c4752d808016ea138c0922d401f4658cabf3fa06fb79d8c1ae05e66043',
 'src/ovl/school_trip/trip_entry_and_script_resources.c': 'a93d44c072cdece81e0b547f32a3b7d999f549e2fe2e04e9a7af90f46943fd6a'}

SHOUGATU_ROUTE = {'assembly_pins': {'asm/matchings/main/game_mode/mode_stage_next.s': 'b4ff29353ec3e3d37bc62158fae8b2046d056bcd3e112397f440cfdd7a1c45be',
                   'asm/matchings/main/game_mode/mode_step_next.s': 'ce766a77a179a3855b374688e31bbb6efe19b214ecdc72bf8cd439c538cda936',
                   'asm/matchings/main/scene_library/dec_init.s': '32a6feb76e18fc65ea97e6f73a81657c52d7d3863791b1cd09abc2e6faa198f4',
                   'asm/matchings/main/scene_library/normal_date_move_place.s': 'f2b0c58aa3c6beea4335151a5721dbb676f06ce1127866035a7eb037717ff020',
                   'asm/matchings/main/scene_library/normal_date_two_select.s': '294303264159fb4486afb1facf9ad07839ec3e99a1031380dcd1d0a733720350',
                   'asm/matchings/main/scene_library/portrait_show_checked.s': '31bab24a6607a4c5819f0c2993eccaad2ec2dea0bd14d3e4d87b795b0fb864e5',
                   'asm/matchings/main/scene_library/scene_wait_cd_read.s': 'daa22d9e14547c22c4fcbc68c7d12c0ac4145ebe880066812c98be2eaabcfe70',
                   'asm/matchings/main/scene_library/select_girl.s': '8386b1501cbb72902f6b3b7eb89ce9026307f4859ba8fd687b1faed68ae0808f',
                   'asm/matchings/main/scene_library/select_girl_init.s': 'ca6db9ffdae19a05e2b762eeac886fddbaf25cd67c1da9edf0f229538ff5c8ec',
                   'asm/matchings/main/scene_library/select_girl_main.s': '8d1eb16211561a87727d6d478628df8d93310937ebd1c5793220f106084ee1ec',
                   'asm/matchings/main/scene_library/talk_run.s': '7038b1340a01d7ac398ab1b153023ac3f48e897a64c56dccc20c9cb9daa630d1',
                   'asm/matchings/main/scene_library/talk_speak_part.s': '677c6706232b230242b5626465a56967262b7edec844ba7de9db9c6cef92a4af',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_dialogue_conditional_step_advance.s': 'c29627226fd6fe55136d9e839bbe71f371c7e8485e452d6d08ab7e82e8420618',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_dialogue_fallback_table_setup.s': '0423c6de6ec392cd051259933626d457e68a263607c734892f78aa06e184d8f9',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_dialogue_group_reset.s': 'a0326145cb9866c5835c789b6faa02c5c1dca072db0100c0bf929d92af4ef1d0',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_dialogue_mode_step_dispatch.s': 'c982636b3ed81eb8f2408bfef74f48b57a4712d7df0c291a908b6efc5005c413',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_dialogue_status_branch_update.s': '711cb2617a6b8f4ff65ad83daae1da3bbf7f3a9ae3bd37725c8435b91c290553',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_shrine_invitation_prompt.s': '8804c3acf422d3bf01bb42bf576c2b275a318d8841d43bb87128f359c08b7d95',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_target_dialogue_setup.s': 'af860315328e870149a9195d620a273018bbc0e4a138a94c50c26748f2ce4b60',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_scene_setup/new_year_girl_dialogue_tables_select.s': '5d6db17ad7d3b0fd743cc36c30c132b7dee37acd4a1096120f00e665d2d62086',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/seasonal_mode_stage_dispatch/new_year_mode_handler.s': '97bc7b258b23ce3ea5e1ce2cbcae33019c8e3de2282d41f9363e88c8bd0e1bf8',
                   'asm/ovl/SHOUGATU/matchings/new_year_christmas/seasonal_mode_stage_dispatch/new_year_talk_run_current.s': '0d8b750206462579f3932300e7d226a5f204e75f7a2db638891953a0327fe914'},
 'assumptions': ['normal stage-2 dispatch entered through step 0 initialization',
                 'no external writes to mode stage/step, current girl, or SHOUGATU text root during this '
                 'route',
                 'step 11 successful non-Shiori selection with b3 set maps the root and advances to 14',
                 'step 19 low-seven-bit girl status is not 4, bypassing stage-reset step 21',
                 'required talk FF, background/CD, portrait validation and menu guards complete on the '
                 'source-visible route'],
 'binary_pins': {'CDROM/EXEDIR/SHOUGATU.EXN': '84842bc3e8540ef9853d38ca04bb453eddb60a5a9b14786896ac618b3b84bb13',
                 'SLPM_86.053': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'},
 'data_assembly': {'address': 2148809668,
                   'label': 'D_80143BC4',
                   'path': 'asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s',
                   'raw_sha256': 'b9b07ce40102146f0535de68ceaa9889733347555b49832138f7cf63591c2891',
                   'required_slots': {11: 2148743008,
                                      13: 2148743284,
                                      19: 2148743388,
                                      21: 2148743548,
                                      28: 2148744004},
                   'sha256': 'b78a107230c27b1ebd9299649c499f4d8122384c61e106fdcc7ce6494301dd10',
                   'word_count': 63},
 'dispatch_source': 'CDROM/EXEDIR/SHOUGATU.EXN',
 'excluded_roots': {'0': 'stage reset clears step',
                    '12': 'assigned outside stage-2 table route',
                    '14': 'b3-zero fallback loops 12 to 13 to 9',
                    '2': 'Shiori stores gated off; same-girl early return loops 12 to 13 to 9'},
 'indices': [1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13],
 'root_conditions': {1: ['current_girl == GIRL_YUKO'],
                     3: ['current_girl == GIRL_YUINA'],
                     4: ['current_girl == GIRL_AYAKO'],
                     5: ['current_girl == GIRL_MIRA'],
                     6: ['current_girl == GIRL_MIO'],
                     7: ['current_girl == GIRL_YUKARI'],
                     8: ['current_girl == GIRL_NOZOMI',
                         '(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2 at root selection'],
                     9: ['current_girl == GIRL_NOZOMI',
                         '(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) >= 2 at root selection'],
                     10: ['current_girl == GIRL_MEGUMI'],
                     11: ['current_girl == GIRL_SAKI'],
                     13: ['current_girl == GIRL_YUMI']},
 'source_pins': {'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
                 'include/game/appointment.h': 'fefe3a4c97bbb2812989d326d065d3948cb42119ed37d0e4ecce393154e3ec2a',
                 'include/game/calendar.h': '44e07c9582c54980f4f8fe8c54adc061a0b9dcd68a5fd061014241ceb01974e9',
                 'include/game/club.h': '2bfd606956b1baf60ea2e370ad1f11297fd40e721d2303b46cd7915d2b46552d',
                 'include/game/day_event.h': 'efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d',
                 'include/game/display_records.h': '54590f2da1130d31ded81ba2f52509f1781006646c75865b2f6fb832af8bc77b',
                 'include/game/draw_records.h': '77cf6a1567a9877f55fe0238566b08af034c921da8c16f503f2c115f68efdcb2',
                 'include/game/game_state.h': 'e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af',
                 'include/game/girl.h': '82c7a558ed145d60e82d57f3a83d331d7fee53a9b89a2edd7c56c284e5c09c58',
                 'include/game/layer.h': 'b0a33122c990abad593818fd9520ebd7f43250a4f93042492d9c12d851f97c29',
                 'include/game/menu_records.h': '1849d0f486659abea223b922fe0d4397e5133387af39f98c50d83780ca9f58fa',
                 'include/game/param.h': '627d01af9cc16b4e3a2819610acf9ee1fd174af5ab768c803facf1a9ec86ba55',
                 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
                 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
                 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
                 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
                 'include/libc.h': '2cefe43794e98d5541095189ffe2fd48922d18246342becdab2e8a5b0d44701b',
                 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
                 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
                 'include/libsnd.h': '998095abfea131f2e2f489b46a936d2049fccff60689286d7939aa62b1429c30',
                 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
                 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
                 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
                 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
                 'include/main/cursor.h': 'b4e359b7c6278fce81566e7042e872f2025d0bfb5d2ffa2c83595fb1e02c3164',
                 'include/main/date.h': '10742c4761e1cf8c6116eb034701322ab5b05ab1f7632830e53a1c74a9140316',
                 'include/main/day_event.h': '983a52118fe5973fa6b42b356f1033a4cb3df32748c07c9f0938ca4866ac355b',
                 'include/main/debug.h': '4565888d481634611fed69f0215e97299a49597561b7f9406b805aedf7e2459d',
                 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
                 'include/main/event_log.h': '3dd48e44393b7d7a0671aad1a8eaf17b60e4e506befb8ba8a147f345fdcc8fa0',
                 'include/main/game_mode.h': '104f931cec5e4b9133baccca3eeea596d4966e46d24bf719a082a7931d6a8da5',
                 'include/main/girl.h': 'e605fa35dd623771ffcbd60070b7edef7a8beb07942ed19a826be7391c77a8fd',
                 'include/main/hud.h': 'dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3',
                 'include/main/import_views/entry_ui.h': 'eac3a572e784c88406acc5b4c40c019df3888aadd6df5c6061baeb09bfa49129',
                 'include/main/import_views/entry_ui_scenes_school_events.h': '1efff14748e011b3c4980a3636735e0d90c3e4e6dd7af2597ba0beb13c454fbb',
                 'include/main/import_views/minigame_scenes_school_events.h': '0eb540cfcd85781805e3f25c11244afc6f657f11416fedf7fd0e0d080a6956c9',
                 'include/main/import_views/resident_views.h': 'fe245704b691e495b0e1f4bcc9c1cf9478a99aafc57cf7187afdc3b96a9617dd',
                 'include/main/import_views/scenes.h': 'e6e7abb06e10ecefdc007011ef75dc7e386b98344d1b6059542382812241b5ea',
                 'include/main/import_views/scenes_school_events.h': '887df254ca70274155d79a878a6f94a9f9c2e7ae32c7d2cd87f11f3f997452a4',
                 'include/main/layer.h': '2cca7cdd41d2a6ac4f8ce81adb3285241e802ea181906f4d5f086b969493eb06',
                 'include/main/menu.h': 'b171ce815adeee4fdb2c4f86258ebcc2b7c1b75eafc2f26ce7e9dd9494ca35fe',
                 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
                 'include/main/name_code.h': 'c10233524227a0cb62a681c3cc3724a9c80068c19c9a46c1cfd778835168394b',
                 'include/main/ovl.h': '6f9ff12d2c1995b1fab3bf7961ebd1ca69118e1747a6f38c1af55c3db673d455',
                 'include/main/param.h': '9f58341a7b7905bfa5ac090cac1035647052704fedaf2b70422df4f1ffb2185e',
                 'include/main/portrait.h': 'a47fd133b66b7e6ac079e890c456ca5e0546cc0fff1eff244146d1a5b9731486',
                 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
                 'include/main/scene.h': 'faf86435b5c4ae9aa47d2a59c2e7c412f02e8a93cbdae2b3a04f1a507bf80049',
                 'include/main/schedule.h': 'f6173aaccf19802a6ff04d01eaef242c0120d2b98482b67cbe377605fe4405ef',
                 'include/main/sd.h': 'ab812b3c7374b55b414c8bcb380636566d39edb9d2a005c252107a91065ae470',
                 'include/main/select_girl.h': 'a85d48f4d950c163497e58ec4eb38a4f6605f49f2b73c99c820c897ce579edd3',
                 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
                 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
                 'include/main/talk.h': 'ea765428db83f6fda60eb89c7e753180ddfe2893cc9de0391b66b32dc2dadc22',
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                 'include/ovl/new_year_christmas.h': '1ca1ad375392e0f0bafc1270b9686b1b9e846b0674f0a18ccf8fb0b911b30814',
                 'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
                 'src/main/game_mode.c': '1e1d80846431688abe111f428a76651d6e70a904eb29bcfb2efae223b421f610',
                 'src/main/scene_library.c': 'c8a0315a4d4542dc13c8e1788ad199fafd28d5104bce64f08bb3d3b63e4e1a18',
                 'src/ovl/new_year_christmas/new_year_outcome.c': '7df1122ab9b04474dffa7900fac461751652b8af6e161f1e279070e71fe3e637',
                 'src/ovl/new_year_christmas/scene_resource_tables.c': '2205afa7f916e2429275a50c9ed635b6366e1238d9d96dda97a64a6d1fe4621b',
                 'src/ovl/new_year_christmas/shrine_actor_sequences.c': 'aab52d5d2fc9e910e695cc413e83205242d4b3e64bfd324c0f1a7e9d6491a69d'},
 'units': {'CDROM/EXEDIR/SHOUGATU.EXN': {'file_bias': 0, 'load_address': 2148737024},
           'SLPM_86.053': {'file_bias': 2048, 'load_address': 2147749888}}}

MUTATION_SPECS = [{'array': 'reply_prefix',
  'conditions': ['talk_choice == 0', 'root selected by date_gift_start current_girl switch'],
  'evidence': ['src/ovl/date/girl_birthday.c::date_gift_pick_reply_group',
               'src/ovl/date/girl_birthday.c::date_gift_sections_init',
               'asm/ovl/DATE/matchings/date/girl_birthday/date_gift_pick_reply_group.s'],
  'function': 'date_gift_pick_reply_group',
  'group': 14,
  'id': 'date-birthday-initial-glyph',
  'indices': [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12],
  'initializer': 'date_gift_sections_init',
  'initializer_source': 'src/ovl/date/girl_birthday.c',
  'length': 2,
  'line': 0,
  'pins': ['include/common.h',
           'include/game/appointment.h',
           'include/game/calendar.h',
           'include/game/club.h',
           'include/game/day_event.h',
           'include/game/display_records.h',
           'include/game/draw_records.h',
           'include/game/game_state.h',
           'include/game/girl.h',
           'include/game/layer.h',
           'include/game/menu_records.h',
           'include/game/model_records.h',
           'include/game/param.h',
           'include/game/schedule_records.h',
           'include/game/system_data.h',
           'include/game/word_views.h',
           'include/game_enums.h',
           'include/include_asm.h',
           'include/libapi.h',
           'include/libgpu.h',
           'include/libgs.h',
           'include/libgte.h',
           'include/main/actor.h',
           'include/main/audio.h',
           'include/main/bg.h',
           'include/main/cd.h',
           'include/main/date.h',
           'include/main/day_event.h',
           'include/main/draw.h',
           'include/main/event_log.h',
           'include/main/game_mode.h',
           'include/main/girl.h',
           'include/main/hud.h',
           'include/main/import_views/entry_ui.h',
           'include/main/import_views/entry_ui_scenes.h',
           'include/main/import_views/entry_ui_scenes_school_events.h',
           'include/main/import_views/minigame_scenes_school_events.h',
           'include/main/import_views/scenes.h',
           'include/main/import_views/scenes_school_events.h',
           'include/main/layer.h',
           'include/main/message.h',
           'include/main/name_code.h',
           'include/main/pad.h',
           'include/main/param.h',
           'include/main/portrait.h',
           'include/main/prim.h',
           'include/main/scene.h',
           'include/main/sprite.h',
           'include/main/state.h',
           'include/main/talk.h',
           'include/main/text.h',
           'include/main/tpage.h',
           'include/main/view.h',
           'include/main/vram.h',
           'include/ovl/date.h',
           'src/ovl/date/girl_birthday.c'],
  'script': 'CDROM/SERIFU/G_BIRTH.BIN',
  'selection': 'explicit_switch_roots_with_conditional_nozomi_alternative',
  'source': 'src/ovl/date/girl_birthday.c',
  'write_lines': [248, 249]},
 {'array': 'buf',
  'conditions': ['talk_choice == 0',
                 'talk_text_table selects fixed root 0x8013A774 through '
                 'amusement_karaoke_dialogue_table_bind'],
  'evidence': ['src/ovl/amusement_park_date/scene_choices_and_saved_return.c::amusement_response_group_text_insert',
               'src/ovl/amusement_park_date/scene_choices_and_saved_return.c::amusement_karaoke_dialogue_pointer_setup',
               'asm/ovl/DATE2/matchings/amusement_park_date/scene_choices_and_saved_return/amusement_response_group_text_insert.s'],
  'function': 'amusement_response_group_text_insert',
  'group': 14,
  'id': 'date2-birthday-initial-glyph',
  'indices': [1],
  'initializer': 'amusement_karaoke_dialogue_pointer_setup',
  'initializer_source': 'src/ovl/amusement_park_date/scene_choices_and_saved_return.c',
  'length': 2,
  'line': 0,
  'pins': ['include/common.h',
           'include/game/appointment.h',
           'include/game/calendar.h',
           'include/game/club.h',
           'include/game/day_event.h',
           'include/game/draw_records.h',
           'include/game/game_state.h',
           'include/game/girl.h',
           'include/game/layer.h',
           'include/game/menu_records.h',
           'include/game/param.h',
           'include/game/system_data.h',
           'include/game/word_views.h',
           'include/game_enums.h',
           'include/include_asm.h',
           'include/libapi.h',
           'include/libgpu.h',
           'include/main/actor.h',
           'include/main/audio.h',
           'include/main/bg.h',
           'include/main/cd.h',
           'include/main/cursor.h',
           'include/main/date.h',
           'include/main/day_event.h',
           'include/main/draw.h',
           'include/main/event_log.h',
           'include/main/game_mode.h',
           'include/main/girl.h',
           'include/main/hud.h',
           'include/main/import_views/entry_ui.h',
           'include/main/import_views/scenes.h',
           'include/main/import_views/scenes_school_events.h',
           'include/main/layer.h',
           'include/main/menu.h',
           'include/main/message.h',
           'include/main/name_code.h',
           'include/main/pad.h',
           'include/main/param.h',
           'include/main/portrait.h',
           'include/main/prim.h',
           'include/main/scene.h',
           'include/main/sprite.h',
           'include/main/state.h',
           'include/main/talk.h',
           'include/main/text.h',
           'include/main/tpage.h',
           'include/main/vram.h',
           'include/ovl/amusement_park_date.h',
           'src/ovl/amusement_park_date/scene_choices_and_saved_return.c'],
  'script': 'CDROM/SERIFU/G_BIRTH.BIN',
  'selection': 'explicit_fixed_root',
  'source': 'src/ovl/amusement_park_date/scene_choices_and_saved_return.c',
  'write_lines': [125, 126]},
 {'array': 'buf',
  'conditions': ['game_state.player_flags.b[1] & 0xF', 'game_state.substep == 0'],
  'evidence': ['src/ovl/school_trip/destination_choice_and_detour_routing.c::trip_health_status_departure_message',
               'src/ovl/school_trip/trip_entry_and_script_resources.c::trip_girl_script_pointer_table_setup',
               'asm/ovl/SHUGAKU/matchings/school_trip/destination_choice_and_detour_routing/trip_health_status_departure_message.s'],
  'function': 'trip_health_status_departure_message',
  'group': 2,
  'id': 'school-trip-initial-line-shortening',
  'indices': [11],
  'initializer': 'trip_girl_script_pointer_table_setup',
  'initializer_source': 'src/ovl/school_trip/trip_entry_and_script_resources.c',
  'length': 3,
  'line': 0,
  'pins': ['include/common.h',
           'include/game/appointment.h',
           'include/game/calendar.h',
           'include/game/club.h',
           'include/game/day_event.h',
           'include/game/display_records.h',
           'include/game/draw_records.h',
           'include/game/game_state.h',
           'include/game/girl.h',
           'include/game/layer.h',
           'include/game/menu_records.h',
           'include/game/param.h',
           'include/game/system_data.h',
           'include/game/word_views.h',
           'include/game_enums.h',
           'include/include_asm.h',
           'include/libapi.h',
           'include/libgpu.h',
           'include/main/actor.h',
           'include/main/audio.h',
           'include/main/bg.h',
           'include/main/cd.h',
           'include/main/color.h',
           'include/main/cursor.h',
           'include/main/date.h',
           'include/main/day_event.h',
           'include/main/draw.h',
           'include/main/event_log.h',
           'include/main/game_mode.h',
           'include/main/girl.h',
           'include/main/hud.h',
           'include/main/import_views/entry_ui.h',
           'include/main/import_views/entry_ui_scenes_school_events.h',
           'include/main/import_views/scenes.h',
           'include/main/import_views/scenes_school_events.h',
           'include/main/import_views/school_events.h',
           'include/main/layer.h',
           'include/main/message.h',
           'include/main/name_code.h',
           'include/main/param.h',
           'include/main/portrait.h',
           'include/main/prim.h',
           'include/main/scene.h',
           'include/main/schedule.h',
           'include/main/sprite.h',
           'include/main/state.h',
           'include/main/talk.h',
           'include/main/text.h',
           'include/main/tpage.h',
           'include/main/vram.h',
           'include/ovl/school_trip.h',
           'src/ovl/school_trip/destination_choice_and_detour_routing.c',
           'src/ovl/school_trip/trip_entry_and_script_resources.c'],
  'script': 'CDROM/SERIFU/SHUGAKU.BIN',
  'selection': 'explicit_fixed_root',
  'source': 'src/ovl/school_trip/destination_choice_and_detour_routing.c',
  'write_lines': [33, 34, 35]},
 {'array': 'sp40',
  'command_value': 7,
  'conditions': ['game_state.current_girl != GIRL_SHIORI', 'game_state.substep == 0'],
  'evidence': ['src/ovl/new_year_christmas/new_year_dialogue.c::new_year_shrine_invitation_prompt',
               'src/ovl/new_year_christmas/seasonal_mode_stage_dispatch.c::new_year_resource_addresses_init',
               'asm/ovl/SHOUGATU/matchings/new_year_christmas/new_year_dialogue/new_year_shrine_invitation_prompt.s'],
  'function': 'new_year_shrine_invitation_prompt',
  'group': 0,
  'id': 'new-year-selected-line-and-command',
  'indices': [1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13],
  'initializer': 'new_year_resource_addresses_init',
  'initializer_source': 'src/ovl/new_year_christmas/seasonal_mode_stage_dispatch.c',
  'length': 3,
  'line': 0,
  'pins': ['include/common.h',
           'include/game/appointment.h',
           'include/game/calendar.h',
           'include/game/club.h',
           'include/game/day_event.h',
           'include/game/display_records.h',
           'include/game/draw_records.h',
           'include/game/game_state.h',
           'include/game/girl.h',
           'include/game/layer.h',
           'include/game/menu_records.h',
           'include/game/param.h',
           'include/game/system_data.h',
           'include/game/word_views.h',
           'include/game_enums.h',
           'include/include_asm.h',
           'include/libapi.h',
           'include/libc.h',
           'include/libgpu.h',
           'include/libgte.h',
           'include/libsnd.h',
           'include/main/actor.h',
           'include/main/audio.h',
           'include/main/bg.h',
           'include/main/cd.h',
           'include/main/cursor.h',
           'include/main/date.h',
           'include/main/day_event.h',
           'include/main/debug.h',
           'include/main/draw.h',
           'include/main/event_log.h',
           'include/main/game_mode.h',
           'include/main/girl.h',
           'include/main/hud.h',
           'include/main/import_views/entry_ui.h',
           'include/main/import_views/entry_ui_scenes_school_events.h',
           'include/main/import_views/minigame_scenes_school_events.h',
           'include/main/import_views/resident_views.h',
           'include/main/import_views/scenes.h',
           'include/main/import_views/scenes_school_events.h',
           'include/main/layer.h',
           'include/main/menu.h',
           'include/main/message.h',
           'include/main/name_code.h',
           'include/main/ovl.h',
           'include/main/param.h',
           'include/main/portrait.h',
           'include/main/prim.h',
           'include/main/scene.h',
           'include/main/schedule.h',
           'include/main/sd.h',
           'include/main/select_girl.h',
           'include/main/sprite.h',
           'include/main/state.h',
           'include/main/talk.h',
           'include/main/text.h',
           'include/main/tpage.h',
           'include/main/vram.h',
           'include/ovl/new_year_christmas.h',
           'include/psx_types.h',
           'src/main/game_mode.c',
           'src/main/scene_library.c',
           'src/ovl/new_year_christmas/new_year_dialogue.c',
           'src/ovl/new_year_christmas/new_year_outcome.c',
           'src/ovl/new_year_christmas/new_year_scene_setup.c',
           'src/ovl/new_year_christmas/scene_resource_tables.c',
           'src/ovl/new_year_christmas/seasonal_mode_stage_dispatch.c',
           'src/ovl/new_year_christmas/shrine_actor_sequences.c'],
  'runtime_root': 'new_year_dialogue_text_table',
  'script': 'CDROM/SERIFU/SHOGATU.BIN',
  'selection': 'source_proved_conditional_stage2_route',
  'selector_contract': SHOUGATU_ROUTE,
  'source': 'src/ovl/new_year_christmas/new_year_dialogue.c',
  'write_lines': [292, 293, 294, 295]}]
NAME_BUFFERS = {"name_code_player_form", "name_code_family", "name_code_given", "name_code_girl_short",
                "girl_name_buf", "D_800CA23C", "D_800CA248", "D_800CA250", "D_800CA25C", "D_800CA280"}
ROOT_SEEDS = {"talk_text_table", "talk_command_table", "D_800CA13C", "D_800CA140", "D_800CA2D0", "trip_talk_text_table"}
WRITE_CALLS = {"strcpy", "strncpy", "strcat", "strncat", "sprintf", "snprintf", "vsprintf", "vsnprintf",
               "memcpy", "memmove", "memset", "get_p_name", "get_g_name"}
STRING_OUTPUT_CALLS = WRITE_CALLS - {"memcpy", "memmove", "memset"}


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _source_path(repo, relative):
    path = (repo / relative).resolve(strict=True)
    if not path.is_relative_to(repo) or not path.is_file():
        raise ValueError("mutation source is outside the selected repository")
    return path


def _function_body(text, name):
    tokens = list(_lex_c_tokens(text))
    code = _mask_c_literals(_mask_c_comments(text), tokens)
    match = re.search(r"\b" + re.escape(name) + r"\s*\([^;{}]*\)\s*\{", code)
    if match is None:
        raise ValueError("source function not found")
    start = code.find("{", match.start())
    depth = 1
    stop = start + 1
    while depth and stop < len(code):
        depth += (code[stop] == "{") - (code[stop] == "}")
        stop += 1
    if depth:
        raise ValueError("unbalanced source function")
    return code[start:stop], text.count("\n", 0, start) + 1


def _initializer(repo, spec):
    source = _source_path(repo, spec["initializer_source"])
    body, first_line = _function_body(source.read_text(), spec["initializer"])
    assignments = [(m.group(1), int(m.group(2), 16)) for m in
                   re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", body)]
    if not assignments or len(assignments) % 3:
        raise ValueError("initializer is not a bounded parallel table triad")
    count = len(assignments) // 3
    triads = [tuple(assignments[i + count * column][1] for column in range(3)) for i in range(count)]
    return assignments[:count], triads


def _replacement(catalog, repo, spec):
    matches = [site for site in catalog.get("source_inventory", {}).get("c_literal_sites", [])
               if site["source_site"].get("path") == spec["source"] and
               site["source_site"].get("function") == spec["function"] and
               site["source_site"].get("array_symbol") == spec["array"]]
    if len(matches) != 1:
        raise ValueError("replacement requires one source-inventory literal site")
    site = matches[0]
    raw = bytes.fromhex(site["raw_hex"])
    capacity = site["source_site"].get("array_length")
    if capacity != len(raw) + 1 or spec["length"] > capacity:
        raise ValueError("replacement literal capacity does not establish requested bytes")
    text = _source_path(repo, spec["source"]).read_text()
    actual = []
    for token in _lex_c_tokens(text):
        if token["kind"] != "string" or text.count("\n", 0, token["start"]) + 1 != site["source_site"]["line"]:
            continue
        prefix = text[max(0, token["start"] - 120):token["start"]]
        if re.search(r"\b" + re.escape(spec["array"]) + r"\s*\[\s*" + str(capacity) + r"\s*\]\s*=\s*$", prefix):
            actual.append(_string_bytes(token["literal"]))
    if actual != [raw]:
        raise ValueError("replacement source literal does not agree with source-inventory bytes")
    return {"c_literal_site_id": site["id"], "source_path": spec["source"],
            "source_line": site["source_site"]["line"], "array_symbol": spec["array"],
            "array_capacity": capacity, "raw_hex": (raw + b"\0")[:spec["length"]].hex(),
            "includes_terminal_nul": spec["length"] == capacity}


def _target(record, section, data, base, length, command_value=None):
    text = record["text"]
    raw = bytes.fromhex(text["raw_hex"])
    offset = text["offset"]
    if (record.get("source_sha256") != _sha(data) or text.get("length") != len(raw) or
            data[offset:offset + len(raw) + 1] != raw + b"\0" or not 0 <= offset <= len(data) - length):
        raise ValueError("target text extent or patch bytes disagree with original input")
    group_slot = section["text_table"] - base + record["group"] * 4
    if not 0 <= group_slot <= len(data) - 4:
        raise ValueError("target group slot outside input")
    group_pointer = struct.unpack_from("<I", data, group_slot)[0]
    line_slot = group_pointer - base + record["line"] * 4
    if not 0 <= line_slot <= len(data) - 4:
        raise ValueError("target line slot outside input")
    pointer = struct.unpack_from("<I", data, line_slot)[0]
    if pointer != base + offset or line_slot != text["pointer_offset"] or text["pointer_value"] != pointer:
        raise ValueError("original linked tables do not resolve to target record")
    fits = length <= len(raw) + 1
    result = {"record_id": record["id"], "section": section["id"], "group": record["group"], "line": record["line"],
              "source_path": record["source_path"], "source_sha256": record["source_sha256"],
              "offset": offset, "relative_byte_offset": 0, "length": length,
              "original_raw_hex": data[offset:offset + length].hex(),
              "write_fits_exported_extent": fits, "exported_text_capacity": len(raw) + 1,
              "pointer_evidence": [{"offset": group_slot, "value": group_pointer}, {"offset": line_slot, "value": pointer}],
              "translation_requires_patch_aware_relocation": True}
    if command_value is not None:
        command = record["command"]
        command_group_slot = section["command_table"] - base + record["group"] * 4
        if not 0 <= command_group_slot <= len(data) - 4:
            raise ValueError("command group slot outside input")
        command_pointer = struct.unpack_from("<I", data, command_group_slot)[0]
        command_offset = command_pointer - base + record["line"]
        if (not 0 <= command_offset < len(data) or command_offset != command["offset"] or
                data[command_offset] != command["value"]):
            raise ValueError("command pointer or byte differs from original input")
        result["command_patch"] = {"offset": command_offset, "original_value": command["value"], "replacement_value": command_value,
                                   "pointer_evidence": {"offset": command_group_slot, "value": command_pointer}}
    return result


def _conditional_view(record, replacement, conditions, command_value=None):
    """Derive one bounded runtime C-string view without modifying its source."""
    original = bytes.fromhex(record["text"]["raw_hex"]) + b"\0"
    patch = bytes.fromhex(replacement["raw_hex"])
    if len(patch) > len(original):
        return {"status": "target_capacity_unproved"}
    storage = patch + original[len(patch):]
    terminator = storage.find(b"\0")
    if terminator < 0:
        return {"status": "original_terminator_overwritten", "storage_raw_hex": storage.hex()}
    raw = storage[:terminator]
    command = record["command"]["value"] if command_value is None else command_value
    return {"status": "conditional_derived_view", "source_record_id": record["id"],
            "conditions": list(conditions), "command_value": command,
            "source_storage_unchanged": True, "storage_raw_hex": storage.hex(),
            "ignored_storage_after_nul_hex": storage[terminator + 1:].hex(),
            "text": {"raw_hex": raw.hex(), "length": len(raw), "terminator_hex": "00",
                     **tokenize_text(raw, command)}}


def _selector_route(repo, inputs, spec):
    """Validate a reviewed conditional route against immutable source/code pins."""
    contract = spec["selector_contract"]
    if list(spec["indices"]) != list(contract["indices"]):
        raise ValueError("selector roots differ from the reviewed conditional route")
    for path, digest in contract["source_pins"].items():
        if _sha(_source_path(repo, path).read_bytes()) != digest:
            raise ValueError("selector route source differs from pinned proof")
    binaries = {}
    for path, digest in contract["binary_pins"].items():
        data, error = _read_original(inputs / path, inputs)
        if error or data is None or _sha(data) != digest:
            raise ValueError("selector original code input differs from pinned proof")
        binaries[path] = data
    functions = []
    instruction = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)")
    for path, digest in contract["assembly_pins"].items():
        assembly = _source_path(repo, path).read_bytes()
        if _sha(assembly) != digest:
            raise ValueError("selector assembly differs from pinned proof")
        source_path = contract["dispatch_source"] if "/ovl/" in path else "SLPM_86.053"
        unit = contract["units"][source_path]
        data = binaries[source_path]
        words = []
        for line, text in enumerate(assembly.decode("utf-8").splitlines(), 1):
            match = instruction.search(text)
            if match is None or match[3].startswith("."):
                continue
            pc, raw = int(match[1], 16), bytes.fromhex(match[2])
            offset = pc - unit["load_address"] + unit["file_bias"]
            if offset < unit["file_bias"] or data[offset:offset + 4] != raw:
                raise ValueError("selector instruction disagrees with original code")
            if words and pc != words[-1]["pc"] + 4:
                raise ValueError("selector function instruction extent is not contiguous")
            words.append({"pc": pc, "offset": offset, "raw_hex": raw.hex(), "assembly_line": line})
        if not words:
            raise ValueError("selector assembly has no verified instructions")
        functions.append({"assembly_path": path, "assembly_sha256": digest,
                          "source_path": source_path, "source_sha256": contract["binary_pins"][source_path],
                          "instructions": words})
    dispatch = contract["data_assembly"]
    assembly = _source_path(repo, dispatch["path"]).read_bytes()
    if _sha(assembly) != dispatch["sha256"]:
        raise ValueError("selector dispatch assembly differs from pinned proof")
    label = re.escape(dispatch["label"])
    match = re.search(r"(?m)^dlabel " + label + r"\s*$([\s\S]*?)^enddlabel " + label + r"\s*$", assembly.decode("utf-8"))
    if match is None:
        raise ValueError("selector dispatch label extent missing")
    words = []
    data = binaries[contract["dispatch_source"]]
    unit = contract["units"][contract["dispatch_source"]]
    for item in instruction.finditer(match[1]):
        if item[3] != ".word":
            raise ValueError("selector dispatch contains an unsupported initializer")
        address, raw = int(item[1], 16), bytes.fromhex(item[2])
        offset = address - unit["load_address"] + unit["file_bias"]
        if (address != dispatch["address"] + len(words) * 4 or offset < unit["file_bias"] or
                data[offset:offset + 4] != raw):
            raise ValueError("selector dispatch word disagrees with original input")
        words.append(int.from_bytes(raw, "little"))
    raw_words = b"".join(word.to_bytes(4, "little") for word in words)
    if len(words) != dispatch["word_count"] or _sha(raw_words) != dispatch["raw_sha256"]:
        raise ValueError("selector dispatch words differ from reviewed route")
    for slot, target in dispatch["required_slots"].items():
        if not 0 <= slot < len(words) or words[slot] != target:
            raise ValueError("selector dispatch semantic slot differs from reviewed route")
    return {"status": "source_proved_conditional_route", "runtime_reachability_proven": False,
            "indices": list(contract["indices"]), "assumptions": list(contract["assumptions"]),
            "root_conditions": dict(contract.get("root_conditions", {})),
            "excluded_roots": dict(contract["excluded_roots"]), "binary_pins": dict(contract["binary_pins"]),
            "source_pins": dict(contract["source_pins"]), "function_instruction_evidence": functions,
            "dispatch": {**dispatch, "words": words, "source_path": contract["dispatch_source"],
                         "source_sha256": contract["binary_pins"][contract["dispatch_source"]]}}


def enrich_script_mutations(catalog, repository=REPOSITORY, files_root=None, *, specs=None, source_pins=None,
                            scan_candidates=True, candidate_files=None):
    """Attach validated patch slots and a separate unresolved source-write ledger."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    specs = MUTATION_SPECS if specs is None else specs
    pins = SOURCE_PINS if source_pins is None else source_pins
    rules, blocked = [], []
    for spec in specs:
        rule = {"id": spec["id"], "conditions": list(spec["conditions"]), "selection_status": spec["selection"],
                "source": {"path": spec["source"], "function": spec["function"], "write_lines": spec["write_lines"]},
                "evidence": spec.get("evidence", []), "targets": []}
        rules.append(rule)
        try:
            for path in spec["pins"]:
                if path not in pins or _sha(_source_path(repo, path).read_bytes()) != pins[path]:
                    raise ValueError("source differs from pinned mutation proof")
            rule["source_pins"] = {path: pins[path] for path in spec["pins"]}
            if "selector_contract" in spec:
                rule["selector_route"] = _selector_route(repo, inputs, spec)
                rule["conditions"].extend(rule["selector_route"]["assumptions"])
            roots, triads = _initializer(repo, spec)
            rule["replacement"] = _replacement(catalog, repo, spec)
            indices = spec.get("indices")
            if indices is None:
                indices = set()
                references = []
                root_map = {symbol: i for i, (symbol, _) in enumerate(roots)}
                for path in sorted((repo / Path(spec["source"]).parent).glob("*.c")):
                    source = path.read_text()
                    code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
                    for match in re.finditer(r"\b" + re.escape(spec["runtime_root"]) + r"\s*=\s*([A-Za-z_]\w*)\s*;", code):
                        if match.group(1) in root_map:
                            index = root_map[match.group(1)]
                            indices.add(index)
                            references.append({"path": path.relative_to(repo).as_posix(),
                                               "line": source.count("\n", 0, match.start()) + 1, "table_index": index,
                                               "source_sha256": _sha(path.read_bytes())})
                indices = sorted(indices)
                rule["root_assignment_candidates"] = references
                blocked.append({"rule_id": spec["id"], "status": "runtime_root_selection_not_fully_proved"})
            files = [item for item in catalog["files"] if item["path"] == spec["script"] and "script" in item]
            if len(files) != 1:
                raise ValueError("required linked script is absent or ambiguous")
            parsed = files[0]["script"]
            data, error = _read_original(inputs / spec["script"], inputs)
            if error or data is None or _sha(data) != parsed["source"]["sha256"]:
                raise ValueError("script original input missing, unsafe, or checksum differs")
            layout = parsed["layout"]
            for index in indices:
                expected = triads[index]
                sections = [section for section in layout["sections"]
                            if tuple(section[key] for key in ("text_table", "command_table", "voice_table")) == expected]
                if len(sections) != 1:
                    raise ValueError("initializer root does not identify one exported section")
                section = sections[0]
                records = [record for record in parsed["records"] if record["section"] == section["id"] and
                           record["group"] == spec["group"] and record["line"] == spec["line"] and "text" in record]
                if len(records) != 1:
                    raise ValueError("patch target is not one exported text record")
                if records[0].get("source_path") != spec["script"]:
                    raise ValueError("patch target source path differs from the selected script")
                target = _target(records[0], section, data, layout["load_address"], spec["length"], spec.get("command_value"))
                target["initializer_table_index"] = index
                route_conditions = rule.get("selector_route", {}).get("root_conditions", {}).get(index, [])
                if route_conditions:
                    target["selector_conditions"] = list(route_conditions)
                if spec["selection"] != "source_assignment_union_requires_runtime_selector_proof":
                    target["conditional_runtime_view"] = _conditional_view(records[0], rule["replacement"],
                                                                             rule["conditions"] + list(route_conditions), spec.get("command_value"))
                    if target["conditional_runtime_view"]["status"] == "original_terminator_overwritten":
                        blocked.append({"rule_id": spec["id"], "record_id": target["record_id"],
                                        "status": "runtime_terminator_outside_exported_extent"})
                rule["targets"].append(target)
                if not target["write_fits_exported_extent"]:
                    blocked.append({"rule_id": spec["id"], "record_id": target["record_id"], "status": "target_allocation_capacity_unproved"})
            rule["status"] = "source_pinned_slots_validated"
        except (OSError, ValueError, KeyError, TypeError, IndexError) as exc:
            rule["status"] = "unresolved_mutation"
            blocked.append({"rule_id": spec["id"], "status": "mutation_proof_failed", "reason": str(exc)})
    candidate_report = inventory_mutation_candidates(repo, catalog, candidate_files) if scan_candidates else {
        "status": "not_requested", "candidates": [], "blocked": []}
    known_sites = {(spec["source"], line) for spec in specs for line in spec["write_lines"]}
    for candidate in candidate_report["candidates"]:
        candidate["known_patch_rule_ids"] = [spec["id"] for spec in specs
                                             if candidate["source_path"] == spec["source"] and candidate["line"] in spec["write_lines"]]
    unresolved = [candidate["id"] for candidate in candidate_report["candidates"]
                  if (candidate["source_path"], candidate["line"]) not in known_sites and
                  candidate["kind"] not in ("script_root_binding", "source_literal_initializer")]
    opaque_count = len(candidate_report.get("opaque_value_dependencies", []))
    address_count = len(candidate_report.get("unmodeled_address_dependencies", []))
    catalog["runtime_text_mutations"] = {"schema": "tokimemo.text.runtime-mutations.v1", "rules": rules,
                                         "source_write_inventory": candidate_report, "blocked": blocked + candidate_report["blocked"],
                                         "unresolved_candidate_ids": unresolved,
                                         "completeness": {"complete": False, "unresolved_writes_block_completion": True,
                                                          "opaque_dependencies_block_completion": True,
                                                          "unmodeled_address_dependencies_block_completion": True,
                                                          "unresolved_opaque_dependencies": opaque_count,
                                                          "unresolved_address_dependencies": address_count},
                                         "coverage": {"rules": len(rules), "target_views": sum(len(rule["targets"]) for rule in rules),
                                                      "unresolved_candidate_writes": len(unresolved),
                                                      "opaque_value_dependencies": opaque_count,
                                                      "unmodeled_address_dependencies": address_count},
                                         "limits": ["no_original_bytes_modified", "conditions_not_executed",
                                                    "generic_write_candidates_require_alias_and_state_resolution",
                                                    "assembly_only_mutations_not_enumerated"]}
    return catalog


def inventory_mutation_candidates(repository, catalog, files=None):
    """Inventory C string-writing calls and writes through possible text aliases.

    Numeric address matches and lexical aliases select candidates only; they
    never prove that an assignment modifies a loaded script or language string.
    Structural value roles exclude array selectors and member-name tokens from
    alias seeds. Arithmetic, element types, and helper returns remain possible
    dependencies, with opaque return expressions recorded separately.
    """
    from pycparser import c_ast, c_parser
    from .text_ast import _GENERATOR, _ids, _redacted_expr
    from .text_renderers import known_calls, read_argument_specs
    def value_symbols(node):
        """Possible pointer/storage bases; selectors are not returned values."""
        if node is None:
            return set()
        if isinstance(node, c_ast.ID):
            return {node.name}
        if isinstance(node, c_ast.ArrayRef):
            return value_symbols(node.name)
        if isinstance(node, c_ast.StructRef):
            return value_symbols(node.name)
        if isinstance(node, c_ast.Cast):
            return value_symbols(node.expr)
        if isinstance(node, c_ast.UnaryOp):
            return set() if node.op in ("sizeof", "_Alignof") else value_symbols(node.expr)
        if isinstance(node, c_ast.TernaryOp):
            return value_symbols(node.iftrue) | value_symbols(node.iffalse)
        if isinstance(node, c_ast.ExprList):
            return value_symbols(node.exprs[-1]) if node.exprs else set()
        if isinstance(node, c_ast.BinaryOp):
            return value_symbols(node.left) | value_symbols(node.right)
        # Function return values have no proved relationship to their arguments.
        return set()

    def alias_dependencies(node):
        """Keep helper-return alias possibilities without seeding its scalar inputs."""
        dependencies = value_symbols(node)
        class Calls(c_ast.NodeVisitor):
            def visit_FuncCall(self, call):
                for argument in call.args.exprs if call.args else []:
                    dependencies.update(value_symbols(argument))
                self.generic_visit(call)
        Calls().visit(node)
        return dependencies

    def write_dependencies(node):
        """Retain opaque destination bases, without following selector operands."""
        if isinstance(node, (c_ast.ArrayRef, c_ast.StructRef)):
            return write_dependencies(node.name)
        if isinstance(node, (c_ast.Cast, c_ast.UnaryOp)):
            return write_dependencies(node.expr)
        return alias_dependencies(node) if isinstance(node, c_ast.FuncCall) else value_symbols(node)

    def expression_variables(node):
        """Variables used anywhere in an expression, excluding member/callee names."""
        if node is None:
            return set()
        if isinstance(node, c_ast.ID):
            return {node.name}
        if isinstance(node, c_ast.StructRef):
            return expression_variables(node.name)
        if isinstance(node, c_ast.FuncCall):
            return expression_variables(node.args)
        if isinstance(node, c_ast.UnaryOp) and node.op in ("sizeof", "_Alignof"):
            return set()
        return set().union(*(expression_variables(child) for _, child in node.children()))
    repo = Path(repository).resolve(strict=True)
    paths = sorted((repo / "src/main").rglob("*.c")) + sorted((repo / "src/ovl").rglob("*.c")) if files is None else [repo / path for path in files]
    root_values = {section[key] for file in catalog.get("files", []) for section in file.get("script", {}).get("layout", {}).get("sections", [])
                   for key in ("text_table", "command_table") if key in section}
    roots_by_unit = {}
    source_inputs = []
    source_read_blocked = []
    for path in paths:
        try:
            if not path.resolve(strict=True).is_relative_to(repo):
                raise ValueError("source escapes repository")
            source = path.read_text(encoding="utf-8")
            if "\ufffd" in source:
                raise UnicodeError("replacement character in source")
        except (OSError, ValueError, UnicodeError) as exc:
            source_read_blocked.append({"source_path": path.as_posix(), "status": "mutation_source_unreadable",
                                        "error_type": type(exc).__name__})
            continue
        code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
        relative_source = path.relative_to(repo).as_posix()
        unit = source_link_unit(repo, relative_source)
        if unit is None:
            source_read_blocked.append({"source_path": relative_source,
                                        "status": "mutation_source_link_unit_unresolved"})
            continue
        roots = roots_by_unit.setdefault(unit, set(ROOT_SEEDS))
        source_inputs.append((path, source, code, unit))
        for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", code):
            if int(match.group(2), 16) in root_values:
                roots.add(match.group(1))
    while True:
        before = sum(len(roots) for roots in roots_by_unit.values())
        for _, _, code, unit in source_inputs:
            roots = roots_by_unit[unit]
            for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*)\s*;", code):
                if match.group(2) in roots:
                    roots.add(match.group(1))
        if sum(len(roots) for roots in roots_by_unit.values()) == before:
            break
    candidates, blocked, parsed_files, skipped_files, opaque_values = [], list(source_read_blocked), [], [], []
    address_dependencies = []
    generator = _GENERATOR()
    for path, source, code, unit in source_inputs:
        roots = roots_by_unit[unit]
        display_calls = {name for name in known_calls(unit)
                         if any(spec["role"] == "display" for spec in read_argument_specs(name, unit))}
        rel = path.relative_to(repo).as_posix()
        if files is None and not any(re.search(r"\b" + re.escape(name) + r"\b", code) for name in WRITE_CALLS | roots | NAME_BUFFERS | display_calls):
            skipped_files.append(rel)
            continue
        try:
            preprocessed = subprocess.run(["gcc", "-E", "-std=gnu89", "-DM2CTX", "-Iinclude", rel], cwd=repo,
                                          check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True).stdout
            tree = c_parser.CParser().parse(preprocessed, filename=rel)
        except Exception as exc:
            blocked.append({"source_path": rel, "status": "mutation_source_ast_unavailable", "error_type": type(exc).__name__})
            continue
        parsed_files.append(rel)
        current = {"function": None, "locals": set(), "conditions": []}

        def coordinate(node):
            if node.coord is None:
                return None
            location = Path(node.coord.file)
            if location.is_absolute():
                try:
                    location = location.relative_to(repo)
                except ValueError:
                    return None
            return node.coord.line if location.as_posix() == rel else None

        def emit(node, kind, target, value, call=None):
            line = coordinate(node)
            if line is None:
                return
            candidates.append({"id": f"mutation-candidate:{rel}:{line}:{len(candidates)}", "source_path": rel,
                               "source_sha256": _sha(source.encode()), "line": line, "function": current["function"],
                               "kind": kind, "callee": call, "destination_expression": _redacted_expr(target, generator),
                               "operator": getattr(node, "op", None),
                               "value_expression": _redacted_expr(value, generator),
                               "destination_symbols": sorted(_ids(target)),
                               "syntactic_guard_context": list(current["conditions"]),
                               "status": "candidate_not_proven_runtime_target"})

        def opaque_value(node, expression, role, destination=None):
            class Calls(c_ast.NodeVisitor):
                found = False
                def visit_FuncCall(self, call):
                    self.found = True
            visitor = Calls()
            visitor.visit(expression)
            if visitor.found and coordinate(node) is not None:
                opaque_values.append({"source_path": rel, "source_sha256": _sha(source.encode()),
                                      "line": coordinate(node), "function": current["function"],
                                      "role": role, "expression": _redacted_expr(expression, generator),
                                      "destination_expression": _redacted_expr(destination, generator) if destination is not None else None,
                                      "status": "opaque_return_value_not_proven_alias"})

        def address_dependency(node, destination):
            dependencies = ((expression_variables(destination) - write_dependencies(destination)) &
                            (roots | NAME_BUFFERS | current["locals"]))
            line = coordinate(node)
            if dependencies and line is not None:
                address_dependencies.append({"source_path": rel, "source_sha256": _sha(source.encode()),
                                             "line": line, "function": current["function"],
                                             "destination_expression": _redacted_expr(destination, generator),
                                             "text_derived_selector_symbols": sorted(dependencies),
                                             "syntactic_guard_context": list(current["conditions"]),
                                             "status": "unmodeled_address_dependency_not_proven_text_write"})

        class Visitor(c_ast.NodeVisitor):
            def visit_FuncDef(self, node):
                old = dict(current)
                current.update(function=node.decl.name, locals=set(), conditions=[])
                class Reads(c_ast.NodeVisitor):
                    def visit_FuncCall(self, call):
                        name = call.name.name if isinstance(call.name, c_ast.ID) else None
                        args = call.args.exprs if call.args else []
                        for spec in read_argument_specs(name, unit):
                            if spec["role"] == "display" and spec["index"] < len(args):
                                current["locals"].update(value_symbols(args[spec["index"]]))
                                opaque_value(call, args[spec["index"]], "display_argument")
                        if name in STRING_OUTPUT_CALLS and args:
                            current["locals"].update(value_symbols(args[0]))
                            opaque_value(call, args[0], "string_output_destination")
                        self.generic_visit(call)
                Reads().visit(node.body)
                alias_rows = []
                class Aliases(c_ast.NodeVisitor):
                    def visit_Assignment(self, assignment):
                        if isinstance(assignment.lvalue, c_ast.ID):
                            alias_rows.append((assignment.lvalue.name, alias_dependencies(assignment.rvalue)))
                            opaque_value(assignment, assignment.rvalue, "assignment_value", assignment.lvalue)
                        self.generic_visit(assignment)
                    def visit_Decl(self, declaration):
                        if declaration.name and declaration.init is not None:
                            alias_rows.append((declaration.name, alias_dependencies(declaration.init)))
                            opaque_value(declaration, declaration.init, "initializer_value", c_ast.ID(declaration.name))
                        self.generic_visit(declaration)
                Aliases().visit(node.body)
                while True:
                    before = len(current["locals"])
                    for name, identifiers in alias_rows:
                        if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                            current["locals"].add(name)
                    if len(current["locals"]) == before:
                        break
                self.visit(node.body)
                current.update(old)
            def visit_If(self, node):
                self.visit(node.cond)
                condition = _redacted_expr(node.cond, generator)
                current["conditions"].append(condition)
                self.visit(node.iftrue)
                current["conditions"].pop()
                if node.iffalse:
                    current["conditions"].append("!(" + condition + ")")
                    self.visit(node.iffalse)
                    current["conditions"].pop()
            def visit_For(self, node):
                if node.init:
                    self.visit(node.init)
                if node.cond:
                    self.visit(node.cond)
                current["conditions"].append("for_condition:" + (_redacted_expr(node.cond, generator) if node.cond else "true"))
                self.visit(node.stmt)
                if node.next:
                    self.visit(node.next)
                current["conditions"].pop()
            def visit_While(self, node):
                self.visit(node.cond)
                current["conditions"].append("while_condition:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_DoWhile(self, node):
                current["conditions"].append("do_while_post_test:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
                self.visit(node.cond)
            def visit_Switch(self, node):
                self.visit(node.cond)
                current["conditions"].append("switch:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_Case(self, node):
                current["conditions"].append("case:" + _redacted_expr(node.expr, generator))
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Default(self, node):
                current["conditions"].append("default_case:fallthrough_possible")
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Assignment(self, node):
                identifiers = write_dependencies(node.lvalue)
                opaque_value(node, node.lvalue, "write_destination", node.lvalue)
                address_dependency(node, node.lvalue)
                if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                    kind = "script_root_binding" if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in roots else "text_or_buffer_write"
                    emit(node, kind, node.lvalue, node.rvalue)
                self.generic_visit(node)
            def visit_UnaryOp(self, node):
                if node.op in ("++", "--", "p++", "p--"):
                    opaque_value(node, node.expr, "write_destination", node.expr)
                    address_dependency(node, node.expr)
                    if write_dependencies(node.expr) & (roots | NAME_BUFFERS | current["locals"]):
                        emit(node, "text_or_buffer_write", node.expr, node.expr)
                self.generic_visit(node)
            def visit_Decl(self, node):
                if current["function"] and node.init is not None and node.name in current["locals"]:
                    kind = "source_literal_initializer" if isinstance(node.init, c_ast.Constant) and node.init.type == "string" else "text_or_buffer_write"
                    emit(node, kind, c_ast.ID(node.name), node.init)
                self.generic_visit(node)
            def visit_FuncCall(self, node):
                name = node.name.name if isinstance(node.name, c_ast.ID) else None
                args = node.args.exprs if node.args else []
                if name in WRITE_CALLS and args:
                    emit(node, "string_or_byte_copy", args[0], node.args, name)
                    address_dependency(node, args[0])
                self.generic_visit(node)
        Visitor().visit(tree)
    return {"schema": "tokimemo.text.mutation-candidates.v1", "source_files_enumerated": len(source_inputs),
            "parsed_files": parsed_files, "files_skipped_by_candidate_prefilter": skipped_files,
            "candidates": candidates, "blocked": blocked,
            "opaque_value_dependencies": opaque_values,
            "unmodeled_address_dependencies": address_dependencies,
            "root_seed_count": sum(len(roots) for roots in roots_by_unit.values()), "limits": ["candidate_alias_closure_not_pointer_proof",
                                                       "structural_value_dependencies_exclude_selectors_not_full_c_type_or_control_flow",
                                                       "arithmetic_and_element_scalar_pointer_roles_remain_conservative",
                                                       "opaque_helper_results_are_unresolved_possible_aliases_not_proven_passthrough",
                                                       "text_derived_selector_dependencies_remain_unresolved_address_diagnostics",
                                                       "all_string_and_byte_copy_calls_are_candidates_not_necessarily_text",
                                                       "opaque_helpers_and_unseeded_byte_writes_require_further_analysis",
                                                       "syntactic_guards_are_not_resolved_path_predicates",
                                                       "case_fallthrough_and_label_goto_edges_not_resolved",
                                                       "control_conditions_recorded_without_execution"]}
