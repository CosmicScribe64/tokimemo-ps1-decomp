"""Source-backed review priorities for graphic localization, not text detection.

T-28002. These paths, RAM addresses and frame roles come from the game loaders
and HUD actor setup. Japanese-text presence still requires visual review.
No game pixels, dialogue, transcription, or replacement artwork is stored here.
"""
from copy import deepcopy

PROFILES = {
    "CDROM/FUKUTEST/MAINROM.NBN": {
        "priority": 0,
        "source_role": "main_screen_ui_pack",
        "screen_context": ["main_hud", "weekly_command_selection", "weekday_command_run",
                           "date_panel", "parameter_panel", "menu_icons", "message_window"],
        "evidence": ["src/main/80059B40.c:func_80059E00 (53 sectors from LBA 0x9D85)",
                     "src/main/80059B40.c:func_80059BE8 (texture-pointer relocation/uploads)",
                     "src/main/80062CD0.c:func_8006509C (command glyph sequence indices)",
                     "wiki/spec/m-hud.md sections 5-8"],
        "runtime_load_address": 0x80180000,
        "disc_lba": 0x9D85,
        "frame_roles": {
            **{frame: "command_icon_animation_frame" for frame in range(3, 60)},
            0x3C: "date_panel_labels",
            0x3D: "parameter_panel_labels",
            0x3E: "message_window_frame",
            **{frame: "decimal_digit" for frame in range(0x44, 0x4E)},
            **{frame: "weekday_label" for frame in range(0x52, 0x59)},
            0x71: "title_banner_left_cap",
            0x72: "title_banner_middle_tile",
            0x73: "title_banner_right_cap",
            **{frame: "information_magazine_graphic" for frame in range(0x76, 0x9B)},
        },
        "follow_up": ["review_each_frame_and_relevant_palette", "retain_sequence_animation_mapping"],
    },
    "CDROM/FUKUTEST/CALROM.NBN": {
        "priority": 1,
        "source_role": "calendar_month_images_and_records",
        "screen_context": ["main_hud", "calendar_check", "month_start"],
        "evidence": ["src/main/8006CB30.c:schedule_month_data_load_step (149 sectors from LBA 0x9B94)",
                     "src/main/8005A0B0.c command-screen calendar reload",
                     "wiki/spec/core.md calendar data section", "wiki/spec/m-hud.md section 13"],
        "runtime_load_address": 0x801A0000,
        "disc_lba": 0x9B94,
        "follow_up": ["decode_calendar_image_records", "map_month_frame_and_palette_regions"],
    },
    "CDROM/FUKUTEST/TITLE.NBN": {
        "priority": 2,
        "source_role": "title_texture_model_animation_pack",
        "screen_context": ["title_screen"],
        "evidence": ["src/main/80061710.c:func_80061A3C (214 sectors from LBA 0xA0CC)",
                     "wiki/spec/h-cdio.md title setup"],
        "runtime_load_address": 0x80180000,
        "disc_lba": 0xA0CC,
        "follow_up": ["decode_title_texture_palettes", "map_title_frames_and_screen_regions"],
    },
    "CDROM/FUKUTEST/EFONT162.TIM": {
        "priority": 2,
        "source_role": "font_image",
        "screen_context": ["font_resource_usage_unresolved"],
        "evidence": ["standard TIM header at source offset zero; usage not established"],
        "follow_up": ["review_glyph_sheet", "prove_runtime_font_usage_before_replacement"],
    },
    "SLPM_86.053": {
        "priority": 0,
        "source_role": "main_ui_frame_sequence_palette_tables",
        "screen_context": ["main_hud", "weekly_command_selection", "calendar_check", "system_settings"],
        "evidence": ["src/main/80059B40.c:func_80059EF0 (embedded palette table)",
                     "src/main/80062CD0.c frame/sequence/palette use",
                     "config/symbol_addrs.txt:sprite_frame_table"],
        "follow_up": ["pair_ui_texture_pack_with_exact_executable_revision",
                      "separate_graphic_labels_from_runtime_string_rendering"],
    },
}


def get_profile(path):
    """Return independent source evidence; never infer that Japanese text exists."""
    profile = PROFILES.get(path)
    if profile:
        return deepcopy(profile)
    if path.startswith("CDROM/BUSTUP/") and path.upper().endswith(".BIN"):
        return {"priority": 2, "source_role": "dialogue_portrait_pack",
                "screen_context": ["dialogue_portrait"],
                "evidence": ["src/main/80062CD0.c:portrait_read_start,portrait_build",
                             "src/main/8007C030.c portrait pointer checks and builders"],
                "frame_roles": {0: "static_source_portrait_frame"},
                "follow_up": ["review_each_portrait_and_source_page",
                              "retain_dynamic_expression_and_lip_sync_mapping_as_unresolved"]}
    return {"priority": 3, "source_role": "graphic_role_unresolved",
            "screen_context": ["unknown"], "evidence": [],
            "follow_up": ["decode_and_map_source_before_visual_review"]}
