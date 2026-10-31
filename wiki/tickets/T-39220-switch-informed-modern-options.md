---
id: T-39220
title: Add Switch-informed modern presentation options
status: In Review
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[switch-remake-reference]]", "[[switch-remake-footage]]", "[[phase4-design-2026-10-29]]"]
---

## Goal

Fold the fidelity-safe lessons from the 2025 Switch release into phase 4, as opt-in, display-only options that keep the native 320x240 image and game state unchanged.

## Acceptance criteria

- [ ] Optional command-screen "Side panels" (off by default, not in the Original preset): week schedule and, only on explicit opt-in, biorhythm, read from existing game state and drawn in the wings over the continued checker.
- [ ] Modern text box mirrors the native message window frame, proportions, name column and centred width, changing only the font; it covers the message, date, location and parameter windows and keeps the original font on command select.
- [ ] Every settings item has a one-line explanation.
- [ ] Button names inside translated text follow the chosen prompt style, substituted only in the text layer.
- [ ] Name entry accepts typed keyboard/system text, converted into the existing name buffer.
- [ ] Hold-to-fast-forward runs several native ticks per presented frame without skipping any tick; fidelity tests unchanged.
- [ ] Port-level save manager (many slots, memos) wrapping unmodified memory-card images; controls/manual page in the pause menu.
- [ ] User decision recorded before any decorative wallpaper wing fill (original assets only, no new art or branding).

## Notes

Evidence: [[switch-remake-reference]] (cited sources) and [[switch-remake-footage]] (frame log). The Switch keeps original art 4:3 with nearest-neighbour scaling, fills sides with a fixed wallpaper in events, offers optional command-screen side panels, and uses full 16:9 only with newly drawn art.

## Comments

### 2026-10-30 wing art implementation (decorative side art)

User decision (2026-10-30): the round-2 wing backgrounds should be seasonal or scene-driven, settable for certain scenes, with options. Implemented as presentation-only, opt-in side art:

- `tools/port/wing_art_content.py` (+ `test_wing_art_content.py`): exports the optional `wing_art` pack from the user's disc. 11 pinned source files (BS stills D_10_08, D_10_07B, D_10_07, I_06_13, D_18_05, D_01_54, D_02_05, D_22_06, X_01_13; PROLODIR/O_01.NBN petals; TARA_OBJ/HANABI.BIN) are checked by SHA-256 and recorded in the manifest. Integer Q12 grading on 5-bit channels, PS1 4x4 dither, deterministic bytes (two exports compared identical). Plates are 720x300 with the picture at (200, 30), so they cover 16:9 and 21:9 windows including the 15-row bands.
- Styles: seasonal set yozakura (spring), spring_day, summer, autumn, winter; location set school, park, town, seaside, legend_tree. Fixes from the round-2 caveats: yozakura lower-third lift and dimmer petals (alpha cap 0.8); non-mirrored centre zone in the bands so the mirror seam no longer meets itself; summer HANABI at additive 0.25, cropped to radius 30/58 and drawn only when it fits inside the visible side area, left/right alternating at mirrored positions; winter has no particles (TARA_OBJ/YUKI.BIN is a window sprite plus non-image data; no snow sprite found).
- Override table `overrides.json` (stage > background prefix > location > mode > tag > class; styles or `seasonal`/`black`); user rules from `user://wing-art-overrides.json` are tried first.
- `build_installed_runtime.py`: new optional dir role `wing_art`; an export failure is only an unavailable role. `installed_launch_profile.gd` accepts the role; no capability needs it.
- Runtime `presentation/wing_art_provider.gd`: composer wing provider + window backdrop. Modes off | seasonal | scene | fixed:<style>; season from the month in the context (read-only); 30 Hz presentation tick; dithered dissolve on style change; reduce_motion = one static frame and no dissolve; reduce_flashes drops fireworks. `native_display_host.gd`: backdrop TextureRect behind the frame (bars and bands at the same integer scale), `set_wing_art_pack`, `set_calendar_month`, `clock_usec`, `wing_art_preview`. Checker continuation (Class A + wing_fill=source) still wins. Boot wiring (pack path from the installed profile, month from game state, scene facts) is left for the root wiring step.
- Settings: `display.wing_art` (default off, in the Original preset as off, live key, late v2 key with no reset report), `accessibility.reduce_motion` (fidelity-neutral). Display page "Side art" with per-option tooltip + one-line explanation + live preview (`ui/settings/wing_art_preview.gd`); Text & Story page "Reduce motion".
- Tests: `tests/render/run_wing_art_tests.gd` (synthetic pack; 1040 checks; 2069 with WING_ART_PACK=real pack): fidelity matrix over every mode, month and width (centre byte-identical, native unchanged), determinism, reduce_motion static, fireworks containment, dissolve, scene/user override resolution, missing/corrupt pack fallback to black. Updated preferences migration, settings pages and host tests pass.
- Visual QA: `tests/render/render_wing_art_previews.gd` rendered 1920x1080 and 2560x1080 previews of every season, scene mode, location style and spring_day around r13 and opening captures, plus 960x540 animations per season and a season-change dissolve, in `.asset-cache/wing-art-impl/previews/` (not committed). Tuning after review: petal brightness lowered, autumn dimmed, fireworks strength lowered.

Open: needs code review against CODING_STANDARDS.md; boot wiring; real-TV playtest of motion.

### 2026-10-30 wing art review fixes (v2)

Review of the wing art patch: 5 findings, all accepted (none rejected); one is fixed only up to the point where the wide host itself is wired.

- Seams (high): `panorama()` tiled the still's top rows (`np.mod`), which left a hard seam every `rows` rows above the still (yozakura plate row 43 = window row 111 at 1080p, autumn row 23) and a seam inside the fade at the still top. The fill is now a reflected run (`reflect(ys, rows)`), so it is continuous at every fold and where it meets the still. Autumn and park also start higher (row0 79 -> 60) and use only canopy rows (sky 56/113 -> 34/40), so the trunk stubs in the top band are gone. New exporter test `test_sky_fill_has_no_seam_stronger_than_the_still`; it fails on the old tiling (step 107 against 9). Real-plate check over the 16:9 and 21:9 side areas: the strongest row edge in every plate is now inside the still's own picture (school 152, town 192, autumn and park 143), and the old seam rows 43, 23 and 55 no longer stand out.
- Season and scene facts (high): the wide `NativeDisplayHost` is not yet used by any game boot. `app/`, `flow/` and `scenes/` still draw through their own canvases, so nothing in real play reaches the host, the fixed styles included. The root wiring belongs to phase 4 and is outside this ticket. This change adds the bridge that wiring will call: `presentation/wing_art_scene_feed.gd` reads `rule_state.month` and `game_mode` read-only, takes the pack path from the installed profile (`paths.wing_art`), passes stage/background/location facts in the wing context and sets `host.wing_art_follows_game`. Integration test (`_feed_checks`): the real `rules/calendar.gd advance_day` moves May 30 to June 1. Through the feed the host shows yozakura, yozakura, then summer, and the state digest (`to_words` + `rule_state`) and the centre stay identical. Until a feed is attached, the Display page adds "Not active yet in this build ... shows the April look" to the Seasonal and Match the scene notes and tooltips.
- Options (medium): the five location styles are now fixed choices (`fixed:school|park|town|seaside|legend_tree`) in the preferences schema, the provider and the Side art list (13 options, each with a name and a one-line note). Per-scene-type choices are new late v2 keys `display.wing_art_command` (class A), `display.wing_art_story` (class B), `display.wing_art_other` (class C), each "Same as Side art" (auto), Off or any mode. They apply live, are fidelity-neutral on their own, and do nothing while Side art is Off. `WingArt.mode_for_scene()` picks the mode. The Display page also describes `wing-art-overrides.json` for per-background rules.
- Dim location plates (low): seaside now uses a cloud-only sky fill (sky 50/90 -> 40/0), gain 2376 -> 3072 and desat 1229 -> 819. legend_tree uses gain 2458 -> 2867, desat 1843 -> 1024 and a near-neutral tint. Both now show the sea and cove, or the tree's foliage, instead of flat dark fill.
- Animation QA (low): `render_wing_art_previews.gd` renders one frame per 30 Hz tick: 120 frames (4 s) per season and the full 240-tick (8 s) summer fireworks loop. The WebPs are rebuilt lossless at 33 ms. libwebp merges identical consecutive frames, so a WebP can hold fewer, longer frames (winter is one still frame by design: no particles). The assembler expands those frames back to ticks, checks each against its PNG and checks that the 640x480 centre is identical in every frame (`anim-report.json`). reduce_flashes still drops the fireworks entirely (covered in `run_wing_art_tests.gd`).

Artifacts (not committed): pack `.asset-cache/wing-art-impl/pack-v2/` (two exports byte-identical), previews and frames in `.asset-cache/wing-art-impl/previews-v2/`, patches `wing-art.v2.patch` (vs phase4 base) and `wing-art.v2.delta.patch` (vs the phase4-wave1 v2 tree).

Open: root wiring of NativeDisplayHost plus the feed into the playable boot (phase 4); code review; real-TV playtest of motion.
