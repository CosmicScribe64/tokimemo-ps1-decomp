# C Code Strings: General System & Menu Overlays (`ETC`)

- **File Path**: `system/c_strings/etc.md`
- **Total Lines**: 124
- **Translated Lines**: 124

| ID | Context | Japanese | Translation | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `8013f980:1` | album_model_init() | NOBJ [[T001:format_directive]][[T002:line_feed]] | NOBJ [[T001:format_directive]][[T002:line_feed]] | ASCII / format literal preserved. |
| `80141154:1` | album_save_step_card_unformatted() | 未初期化のメモリーカードです。 | Memory card not formatted. |  |
| `80141154:2` | album_save_step_card_unformatted() | 初期化していいですか？ | OK to format it? |  |
| `80141154:3` | album_save_step_card_unformatted() | 別のメモリーカードを | Insert another card |  |
| `80141154:4` | album_save_step_card_unformatted() | 差し込み口１ | in slot 1 |  |
| `80141154:5` | album_save_step_card_unformatted() | にセットして、 | `         , then press` |  |
| `80141154:6` | album_save_step_card_unformatted() | ボタンを押してください。 | `  to continue.` |  |
| `801413fc:1` | album_save_step_begin() | データセーブ | Save Data |  |
| `801415a4:1` | album_save_step_ask() | 今回のアルバムモードのデータを | Save this Album Mode data |  |
| `801415a4:2` | album_save_step_ask() | ときめきメモリアル・システムファイル | ` to the Tokimeki Memorial` |  |
| `801415a4:3` | album_save_step_ask() | にセーブしますか。 | system file? |  |
| `801415a4:4` | album_save_step_ask() | データセーブメッセージ | Save message |  |
| `8014175c:1` | album_save_step_card_check() | メモリーカードをチェックしています。 | Checking the memory card... |  |
| `8014175c:2` | album_save_step_card_check() | EXIST_CARD! | EXIST_CARD! | ASCII / format literal preserved. |
| `8014175c:3` | album_save_step_card_check() | UNFORMAT CARD! | UNFORMAT CARD! | ASCII / format literal preserved. |
| `8014175c:4` | album_save_step_card_check() | NO_CARD ! | NO_CARD ! | ASCII / format literal preserved. |
| `8014175c:5` | album_save_step_card_check() | NO COMUNICATION! | NO COMUNICATION! | ASCII / format literal preserved. |
| `80141980:1` | album_save_step_system_file_load() | ただいま、セーブ中です。 | Saving now. |  |
| `80141980:2` | album_save_step_system_file_load() | LOAD SYSTEM OK | LOAD SYSTEM OK | ASCII / format literal preserved. |
| `80141abc:1` | album_event_log_pack() | [[T001:format_directive]] [[T002:format_directive]] [[T003:format_directive]] [[T004:format_directive]] [[T005:format_directive]] [[T006:line_feed]] | [[T001:format_directive]] [[T002:format_directive]] [[T003:format_directive]] [[T004:format_directive]] [[T005:format_directive]] [[T006:line_feed]] |  |
| `80141abc:2` | album_event_log_pack() | ALBUM_MODE IN SAVE [[T001:format_directive]] [[T002:line_feed]] | ALBUM_MODE IN SAVE [[T001:format_directive]] [[T002:line_feed]] | ASCII / format literal preserved. |
| `80141be8:1` | album_save_step_write() | [C_GIRL][[T001:format_directive]] [[T002:line_feed]] | [C_GIRL][[T001:format_directive]] [[T002:line_feed]] | ASCII / format literal preserved. |
| `80141e10:1` | album_save_step_no_system_file() | システムファイルがありません。 | There is no system file. |  |
| `80141e10:2` | album_save_step_no_system_file() | （システムファイルがないと | (Without a system file, |  |
| `80141e10:3` | album_save_step_no_system_file() | アルバムモードのセーブはできません） | Album Mode can't be saved.) |  |
| `80141e10:4` | album_save_step_no_system_file() | システムファイルを作成し、 | Make a system file and |  |
| `80141e10:5` | album_save_step_no_system_file() | アルバムモードをセーブしますか？ | save the Album Mode? |  |
| `80141e10:6` | album_save_step_no_system_file() | システムファイルを作成しています。 | Making the system file. |  |
| `80141e10:7` | album_save_step_no_system_file() | （アルバムモードのデータをセーブ後、 | (After saving the Album data, |  |
| `80141e10:8` | album_save_step_no_system_file() | ファイルセレクト画面にて、 | on the file select screen, |  |
| `80141e10:9` | album_save_step_no_system_file() | システムファイルを作り直してください） | please remake the system file.) |  |
| `801421ac:1` | album_save_step_no_card() | メモリーカードを認識できません。 | No memory card found. |  |
| `801421ac:2` | album_save_step_no_card() | 差し込み口１のメモリーカードの | Please check the memory |  |
| `801421ac:3` | album_save_step_no_card() | 接続を確認してください。 | card in slot 1. |  |
| `801421ac:4` | album_save_step_no_card() | データのセーブを行ないますか？ | Save the data? |  |
| `80142390:1` | album_save_step_card_full() | メモリーカードに空きブロックがありません。 | The memory card has no free blocks. |  |
| `80142390:2` | album_save_step_card_full() | 他の空きブロックのあるメモリーカードを | Please prepare another memory |  |
| `80142390:3` | album_save_step_card_full() | 用意してください。 | card with space. |  |
| `80142390:4` | album_save_step_card_full() | データのセーブを行ないますか？ | Save the data? |  |
| `80142574:1` | album_save_step_system_file_broken() | システムファイルが壊れています。 | The system file is damaged. |  |
| `80142574:2` | album_save_step_system_file_broken() | システムファイルを作り直していいですか？ | May I remake the system file? |  |
| `80142574:3` | album_save_step_system_file_broken() | システムファイルを作成しています。 | Making the system file. |  |
| `80142574:4` | album_save_step_system_file_broken() | （アルバムモードのデータをセーブ後、 | (After saving the Album data, |  |
| `80142574:5` | album_save_step_system_file_broken() | ファイルセレクト画面のシステムを | select System on the file |  |
| `80142574:6` | album_save_step_system_file_broken() | クリックして、システムファイルを | select screen and remake |  |
| `80142574:7` | album_save_step_system_file_broken() | 作り直してください） | the system file.) |  |
| `80149394:1` | career_choice_init_screen() | 就職・進学 | Career |  |
| `80149f48:1` | career_choice_exam_question() | DEFAUT: | DEFAUT: | ASCII / format literal preserved. |
| `8014a360:1` | career_choice_result_code_set() | shinro [[T001:format_directive]] [[T002:line_feed]] | shinro [[T001:format_directive]] [[T002:line_feed]] | ASCII / format literal preserved. |
| `8014ab88:1` | career_choice_leave() | shinro [[T001:format_directive]] [[T002:line_feed]] | shinro [[T001:format_directive]] [[T002:line_feed]] | ASCII / format literal preserved. |
| `8014addc:1` | career_guidance_title_show() | 進路指導 | Guidance |  |
| `8014af28:1` | career_guidance_assess_params() | param [[T001:format_directive]] [[T002:format_directive]] [[T003:line_feed]] | param [[T001:format_directive]] [[T002:format_directive]] [[T003:line_feed]] | ASCII / format literal preserved. |
| `8014b0d0:1` | career_guidance_closest_girl() | [[T001:format_directive]] [[T002:format_directive]] [[T003:line_feed]] | [[T001:format_directive]] [[T002:format_directive]] [[T003:line_feed]] |  |
| `8014bc00:1` | career_guidance_university_talk() | 先生 | Sensei |  |
| `8014bde0:1` | career_guidance_university_verdict() | thl [[T001:format_directive]] [[T002:line_feed]] | thl [[T001:format_directive]] [[T002:line_feed]] | ASCII / format literal preserved. |
| `8014c06c:1` | career_guidance_job_talk() | 先生 | Sensei |  |
| `80132198:1` | confession_judge_stub_judge_init() | 告白判定 | Confession Check |  |
| `80132254:1` | confession_judge_stub_butler_init() | 外井告白シーン | Sotoi Confession Scene |  |
| `801322e0:1` | confession_judge_stub_message_init() | 女々しい野郎どもへ | Ballad of Wimpy Men |  |
| `80132394:1` | confession_judge_stub_letter_init() | 机の中に手紙が | Letter in Desk |  |
| `801324b0:1` | confession_scene_stub_data_load_init() | 告白シーンデータリード | Confession Scene Data Read |  |
| `801325a8:1` | confession_scene_stub_silhouette_init() | シルエット画面 | Silhouette Screen |  |
| `80132634:1` | confession_scene_stub_bustup_scroll_init() | バストアップスクロール | Bust-up Scroll |  |
| `801326c0:1` | confession_scene_stub_scene_init() | 告白シーン | Confession Scene |  |
| `801327ac:1` | epilogue_stub_prepare_init() | エピローグ用初期化 | Epilogue Init |  |
| `80132840:1` | epilogue_stub_show_init() | エピローグ | Epilogue |  |
| `80146400:1` | exam_correction_bonus() | 補正 [[T001:format_directive]] | `Modifier [[T001:format_directive]] ` |  |
| `801467c4:1` | exam_setup_init_screen() | 期末テスト | Finals |  |
| `80146d4c:1` | exam_score_day() | main [[T001:format_directive]] sub [[T002:format_directive]] bio [[T003:format_directive]] ret [[T004:format_directive]] | `main [[T001:format_directive]] sub [[T002:format_directive]] bio [[T003:format_directive]] ret [[T004:format_directive]] ` | ASCII / format literal preserved. |
| `80146d4c:2` | exam_score_day() | 今日の成績 [[T001:format_directive]][[T002:tab]] | Today's Score [[T001:format_directive]][[T002:tab]] |  |
| `80146d4c:3` | exam_score_day() | 合計点 [[T001:format_directive]][[T002:line_feed]] | Total Score [[T001:format_directive]][[T002:line_feed]] |  |
| `8014716c:1` | exam_day_start_language() | 期末試験 | Finals |  |
| `8014716c:2` | exam_day_start_language() | 第一日目の試験科目は[[T001:row_separator]]語学だ。） | Day one's exam is[[T001:row_separator]]language.) |  |
| `801471f4:1` | exam_day_start_math() | 期末試験 | Finals |  |
| `801471f4:2` | exam_day_start_math() | 第二日目の試験科目は[[T001:row_separator]]数学だ。） | Day two's exam is[[T001:row_separator]]math.) |  |
| `8014727c:1` | exam_day_start_science() | 期末試験 | Finals |  |
| `8014727c:2` | exam_day_start_science() | 第三日目の試験科目は[[T001:row_separator]]理科だ。） | Day three's exam is[[T001:row_separator]]science.) |  |
| `80147304:1` | exam_day_start_social_studies() | 期末試験 | Finals |  |
| `80147304:2` | exam_day_start_social_studies() | 第四日目の試験科目は[[T001:row_separator]]社会だ。） | Day four's exam:[[T001:row_separator]]history.) |  |
| `8014738c:1` | exam_day_start_art() | 期末試験 | Finals |  |
| `8014738c:2` | exam_day_start_art() | 第五日目の試験科目は[[T001:row_separator]]美術だ。） | Day five's exam is[[T001:row_separator]]art.) |  |
| `801474fc:1` | exam_day_comment() | もうだめだー。） | I'm doomed.) |  |
| `801474fc:2` | exam_day_comment() | ちょっとだめだな。） | Not so good.) |  |
| `801474fc:3` | exam_day_comment() | まあまあのできだな。） | That went okay.) |  |
| `801474fc:4` | exam_day_comment() | いいできだ。） | Nice work.) |  |
| `801474fc:5` | exam_day_comment() | バッチリだ。） | Nailed it.) |  |
| `8014790c:1` | exam_results_show() | 結果発表[[T001:terminal_byte_before_nul]] | Results[[T001:terminal_byte_before_nul]] |  |
| `8014790c:2` | exam_results_show() | 今日は、試験結果の[[T001:row_separator]]発表の日だ。） | Today the exam[[T001:row_separator]]results come out.) |  |
| `8014790c:3` | exam_results_show() | [[T001:format_directive]][[T002:line_feed]] | [[T001:format_directive]][[T002:line_feed]] |  |
| `80147d74:1` | exam_results_react() | ３科目以上赤点だった。[[T001:row_separator]]明日から、一週間補習だぁ。[[T002:row_separator]]勉強しかできないー！） | Failed 3+ subjects.[[T001:row_separator]]A week of make-up class.[[T002:row_separator]]Nothing but study now!) |  |
| `80147d74:2` | exam_results_react() | やったー、一番だ！[[T001:row_separator]]これで試験の帝王だな。） | Yes, top of the class![[T001:row_separator]]King of Exams, me.) |  |
| `801480d0:1` | exam_ranking_build() | [[[T001:format_directive]]:[[T002:format_directive]]][[T003:line_feed]] | [[[T001:format_directive]]:[[T002:format_directive]]][[T003:line_feed]] | ASCII / format literal preserved. |
| `801480d0:2` | exam_ranking_build() | 順位計算[[T001:line_feed]] | `Rank Calc[[T001:line_feed]] ` |  |
| `801480d0:4` | exam_ranking_build() | 順位作成[[T001:line_feed]] | `Rank Build[[T001:line_feed]] ` |  |
| `801480d0:5` | exam_ranking_build() | [[[T001:format_directive]]:[[T002:format_directive]] [[T003:format_directive]] [[T004:format_directive]]] [[T005:line_feed]] | [[[T001:format_directive]]:[[T002:format_directive]] [[T003:format_directive]] [[T004:format_directive]]] [[T005:line_feed]] | ASCII / format literal preserved. |
| `801480d0:6` | exam_ranking_build() | 順位ソート[[T001:line_feed]] | `Rank Sort[[T001:line_feed]] ` |  |
| `801480d0:8` | exam_ranking_build() | 順位確定[[T001:line_feed]] | `Rank Finalize[[T001:line_feed]] ` |  |
| `80144540:1` | clear_save_ask_init() | 今回のプレイをセーブしますか？ | Save this playthrough? |  |
| `801446e4:1` | clear_save_ask_answer() | メモリーカードをチェックしています | Checking the memory card... |  |
| `80144ac0:1` | clear_save_slot_menu_init() | データセーブ | Save Data |  |
| `80144f74:1` | clear_save_write_step() | データセーブ中です | Saving... |  |
| `80144f74:2` | clear_save_write_step() | しばらくお待ち下さい | Please wait. |  |
| `801451d4:1` | clear_save_no_card_step() | メモリーカードを認識できません | No memory card found. |  |
| `801451d4:2` | clear_save_no_card_step() | メモリーカードを差し込み口１に | Insert a memory card in |  |
| `801451d4:3` | clear_save_no_card_step() | セットし、ボタンを押してください。 | slot 1 and press a button. |  |
| `80145318:1` | clear_save_card_full_step() | メモリーカードに空きがありません | The memory card is full. |  |
| `80145318:2` | clear_save_card_full_step() | 別のメモリーカードに | Save to another |  |
| `80145318:3` | clear_save_card_full_step() | セーブしますか？ | memory card? |  |
| `80145318:4` | clear_save_card_full_step() | 別のメモリーカードを | Insert another card |  |
| `80145318:5` | clear_save_card_full_step() | 差し込み口１にセットして、 | in slot 1, then |  |
| `80145318:6` | clear_save_card_full_step() | ボタンを押してください。 | press a button. |  |
| `801455b0:1` | clear_save_card_unformatted_step() | 未初期化のメモリーカードです。 | Memory card not formatted. |  |
| `801455b0:2` | clear_save_card_unformatted_step() | 初期化していいですか？ | OK to format it? |  |
| `801455b0:3` | clear_save_card_unformatted_step() | 別のメモリーカードを | Insert another card |  |
| `801455b0:4` | clear_save_card_unformatted_step() | 差し込み口１ | in slot 1 |  |
| `801455b0:5` | clear_save_card_unformatted_step() | にセットして、 | `         , then press` |  |
| `801455b0:6` | clear_save_card_unformatted_step() | ボタンを押してください。 | `  to continue.` |  |
| `80147d74:3` | exam_results_react() | 今回の試験は終了した。） | The exams are over.) |  |
| `80147d74:4` | exam_results_react() | 今回の試験は終了した。） | The exams are over.) |  |
| `801480d0:3` | exam_ranking_build() | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | ASCII / format literal preserved. |
| `801480d0:7` | exam_ranking_build() | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | ASCII / format literal preserved. |
| `801480d0:9` | exam_ranking_build() | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | [[[T001:format_directive]]:[[T002:format_directive]]] [[T003:line_feed]] | ASCII / format literal preserved. |
| `801447b0:1` | clear_save_card_check_step() | セーブするアルバムを選んでください | Choose an album to save to. |  |
| `801447b0:2` | clear_save_card_check_step() | セーブするアルバムを選んでください | Choose an album to save to. |  |
