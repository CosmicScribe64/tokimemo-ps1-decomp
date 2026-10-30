---
type: reference
updated: 2026-10-30
---

# Switch release reference: widescreen and modern features

Research only (2026-10-30), done at the user's request to use the Nintendo Switch re-release as a reference for widescreen and modern features. The game is *ときめきメモリアル forever with you エモーショナル* (Konami, Japan-only, 8 May 2025). The research used official Konami pages, Japanese press reviews and Konami's official screenshots. No video was watched. Nothing was downloaded on purpose, and no repository file other than this page was changed. Our own design is in [[phase4-design-2026-10-29]] and [[widescreen-settings]].

Evidence labels used below:
- **[shot]**: observed in official screenshots. Konami marks every screenshot as a development build (「画面は開発中のもの」, https://www.konami.com/games/tokimeki/1r/).
- **[official]**: stated by Konami.
- **[3rd]**: reported by press or users.
- **[unknown]**: not established by any source found.

Screenshot paths are under `https://www.konami.com/games/tokimeki/1r/s/img/` (the same files are mirrored on `img.konami.com`). They are written below in shortened form, e.g. `system/play_01.jpg`.

## 1. What the Switch release does

### Display and widescreen

- **[shot] The output is always a 16:9 frame, and the original 4:3 picture is never stretched.** Official stills are 1200x675 (konami.com) or 1280x720 (Automaton). In the 1280x720 shot the 4:3 game area spans about x=160..1120, so it is 960x720: an exact 3x integer scale of 320x240 at full height (https://automaton-media.com/wp-content/uploads/2025/02/tokimemo-20250203-327208-001.jpg). [unknown] The docked output resolution, and what happens at 1080p, where 4.5x is not an integer.
- **[shot] The original pixels are scaled with nearest-neighbour and no filter.** Shiori's sprite, the backgrounds and the original font all show crisp, uniform blocks, with no blur, CRT or scanline effect (same Automaton shot). No source mentions a screen filter option. The name-entry backdrop is an exception: it looks smoothly upscaled and cropped under a pale overlay (`system/system_03.jpg`, `system/evs_01.jpg`). That is an impression from the image only.
- **[shot] Events, conversations, dates and minigames in Original mode use a fixed decorative pillarbox wallpaper.** Each side bar is a dark-green panel with a thin pale rule, the words "TOKIMEKI MEMORIAL" running vertically, and a small "forever with you emotional". The 4:3 image sits between the panels unchanged (`system/play_03.jpg`, `play_04`, `play_05`, `play_07`, `play_08`, `ss/ss_01.jpg`). The wallpaper looked the same in every scene seen. [unknown] Whether it varies by scene or season, or whether the player can choose black bars instead.
- **[shot] On the command screen the wings become extra information panels, not wallpaper** (`system/play_01.jpg`, `play_02`, `play_06`, `graphic_05`; Automaton `-002.jpg`).
  - Left panel, 「バイオリズム」: three boxed gauges, 身体 (red), 感性 (blue) and 頭脳 (green), each showing a symbol (◎ ○ △ × or a flower).
  - Right panel, 「今週の予定」: 8 consecutive dates with English weekday labels (SUN/MON...), weekends coloured, and a heart on a booked date.
  - The 4:3 core (stats, command icons, room strip, date pillar, message box) stays intact in the middle.
  - [3rd] Denfaminico says the biorhythm and weekly-schedule display can be switched on or off (「ON・OFF可能」, https://news.denfaminicogamer.jp/kikakuthetower/250502c). The name of the option is not known.
  - [official] Konami's page suggests using the biorhythm as a guide when choosing commands (https://www.konami.com/games/tokimeki/1r/).
- **Where the side-panel data comes from (our own source check, not a claim about the Switch).** On PS1 the biorhythm appears only as sine curves on the holiday calendar screen (`hud_biorhythm_graph_draw`, `src/main/hud_screens.c:570`, called from `calendar_view_update` at :612). The command screen never shows it. The three waves (23, 28 and 33 days) drive the "poor day" roll ([[spec/m-weekday]]). So the Switch wings bring existing PS1 data onto a screen where the PS1 did not show it, as a per-day symbol summary. This makes the information easier to reach than in the original, which is a gameplay-visible change.
- **[shot] New Graphics mode is native 16:9 with redrawn, wider backgrounds.**
  - The park scene in Original mode (`system/graphic_03.jpg`) next to New mode (`system/graphic_04.jpg`): the background is redrawn at high resolution and widened, with extra detail (children in the sandbox, people on the slide).
  - The Shiori park CG is full-bleed 16:9 in New mode (`system/graphic_01.jpg`) but 4:3 inside the wallpaper in Original mode (`system/play_05.jpg`).
  - The other New-mode shots show the same full-bleed treatment: the boutique, school gate, corridor and planetarium (`ss/ss_06`, `ss_08`, `ss_10`, `ss_14`, `ss_19`, `ss_20`).
- **[official] Scope of New Graphics.** It replaces standing art and stills in after-school and date events only (https://www.konami.com/games/tokimeki/1r/). [3rd] Famitsu adds that it includes new confession scenes, and that the voices are the same in both modes (https://www.famitsu.com/article/202505/40915).
- **[shot] HUD layout in New Graphics 16:9 scenes.**
  - The message window keeps its original size and its centred, 4:3-equivalent position.
  - The vertical date box moves out to the right edge of the 16:9 frame.
  - A location tag sits at the top-left edge of the frame.
  - The window chrome (pixel-art borders, checker strip) stays in the original style, and the checker band runs the full width (`system/graphic_03.jpg` vs `graphic_04.jpg`, `ss/ss_10.jpg`).
- **[shot] System screens are redesigned for 16:9.**
  - Title: a white background, the new logo, ゲームスタート / おまけ / オプション, and a "+ 取扱説明書" (manual) prompt (`system/system_01.jpg`).
  - Save/load: a full-width layout (`system/system_02.jpg`).
  - Name entry and EVS entry (`system/system_03.jpg`, `system/evs_03.jpg`).
  - Deluxe extras (BGM player, gallery): logo wallpaper with button-hint bars (`products/bonus_02.jpg`..`bonus_05.jpg`).
  - [3rd] The gallery zooms high-resolution art (https://www.gamespark.jp/article/2025/05/02/152197.html).
- **[shot]/[official] Opening movie.** The textless opening movie is a Deluxe extra. It is shown 4:3 with plain black bars (`products/bonus_01.jpg`). [3rd] Famitsu lists it as Deluxe-only (https://www.famitsu.com/article/202505/40915). [unknown] How the in-game opening plays in the standard edition.
- **[3rd] Handheld vs docked.** No display difference is reported. Reviewers only note that the game suits handheld play (https://www.famitsu.com/article/202505/40915).

### Text, font and language

- **[official] The 「キャラ表示」 option has four presets:**
  - Original
  - Original + new font
  - New graphics
  - New graphics + new font

  The new font covers four windows: location, date, parameters and message. Icons and other UI stay original. When new graphics are shown, the new font is forced on (https://www.konami.com/games/tokimeki/1r/; diagram with red boxes around the four windows in `system/graphic_02_2.jpg`). [3rd] The command-selection screen keeps the original font (https://automaton-media.com/articles/newsjp/tokimemo-20250203-327208/, https://dengekionline.com/article/202505/40364).
- **[shot] The new font is a smooth, anti-aliased high-resolution gothic face drawn inside the unchanged pixel-art window frames** (`ss/ss_08.jpg`, `ss_14`, `ss_19`). The options list and the name, EVS and save screens stay in a pixel-style font.
- **[3rd] Switching takes effect live.** It can be changed from Options without going back to the title screen (https://www.famitsu.com/article/202505/40915).
- **[3rd] Reception of the new font.** Dengeki says it is easier to read, especially for older eyes (https://dengekionline.com/article/202505/40364). Nintendo Life singles out the high-resolution kanji as helpful for importers (https://www.nintendolife.com/features/opinion-cult-classic-tokimeki-memorial-hits-switch-but-30-years-on-its-still-out-of-reach).
- **[3rd] Language.** The release is Japanese only and has no English localisation (Nintendo Life, same URL; https://gematsu.com/2025/02/tokimeki-memorial-forever-with-you-emotional-launches-may-8-in-japan). [unknown] The official eShop language field: the store page redirected to a queue and could not be read.
- **[unknown]** Text size, backlog or history, auto-advance and subtitle options. None was mentioned in any source reviewed.

### Other modern features

- **[3rd] Fast-forward.** Holding ZL fast-forwards stat-training animations and dialogue already seen, and can skip through dates (https://www.famitsu.com/article/202505/40915). Denfaminico reports fast display of already-read text.
- **[shot]/[3rd] Options list.** Visible items, partly truncated in the screenshot: キャラ表示, 文字表示速…, コマンド速…, アイコンモ…, メニューモ…, カーソル速…, 設定終了 (`system/graphic_02_1.jpg`). The options appear on a notebook page over the command screen. Cursor speed is confirmed by Famitsu. No volume settings are visible.
- **[3rd] Free Mode extras.** Command icons can be repositioned with ZL plus the stick, and stat values can be hidden (https://www.gamespark.jp/article/2025/05/02/152197.html).
- **[official]/[shot] Saves.**
  - 100 slots with editable memos (https://www.konami.com/games/tokimeki/1r/).
  - The screen shows 20 slots per page, Copy / Save / Clear / Load buttons, and a profile panel with stats (`system/system_02.jpg`).
  - [unknown] Quick save, autosave, save-anywhere and rewind are not mentioned anywhere.
- **[official]/[shot] Text input.**
  - Name entry keeps the original kana grid and adds a 漢字 tab and a 「ソフトウェアキーボードを使用」 (use software keyboard) button (`system/system_03.jpg`; https://www.konami.com/games/tokimeki/1r/).
  - [3rd] The software keyboard also works for save memos (Famitsu).
- **[official] EVS: heroines say the player's name.**
  - The player picks a built-in reading or creates one (reading + pitch accent, then a download). Creating one needs internet and a Nintendo Account.
  - Up to 5 readings each for surname, given name and nickname (https://www.konami.com/games/tokimeki/1r/).
  - [3rd] The voices are the original recordings with improved audio quality (https://automaton-media.com/articles/newsjp/tokimemo-20250203-327208/).
- **[official]/[3rd] Deluxe extras.** Textless opening, BGM soundtrack (original and remaster), about 280 illustrations (Automaton; Famitsu). [3rd] Denfaminico alone reports a newly recorded clear-bonus Free Talk.
- **[3rd] Patch.** Ver.1.2.0 (June 2025) fixed bugs only, including date counts in confession conditions and Yoshio's girlfriend confessing to the player (https://www.famitsu.com/article/202506/45764).

## 2. Comparison with our phase 4 design

Ours = [[phase4-design-2026-10-29]] and [[widescreen-settings]].

| Area | Switch release | Ours | Verdict |
| --- | --- | --- | --- |
| 4:3 picture | Never stretched; centred; integer 3x at 720p | Native 320x240 byte-identical; integer or fractional fit; centred aperture | Same |
| Filtering of original art | Nearest-neighbour, no filter option seen | Nearest by default; opt-in smooth fit | We do more (an option the Switch does not show) |
| Pillarbox in events and CGs (our Class B/C) | Fixed decorative branded wallpaper | Black for every class; no invented art | Different. A decorative opt-in fill is missing in ours |
| Command-screen wings (our Class A) | Info panels: biorhythm and 8-day schedule; toggleable [3rd] | Opt-in checker continuation (source-derived); wing HUD readouts deferred (item iv) | Different. Info panels are missing in ours (deferred) |
| Widescreen fill of scenes | Redrawn, widened New Graphics art for walks and dates | Rejected (no invented art); margin reveal only if a source trace proves it | Different by policy. The Switch's full 16:9 required new art |
| HUD reflow in 16:9 | Date box anchored to the frame edge; message box stays centred | Tier 2 Wide Layout deferred; message box can widen in the modern text box | Partly missing in ours (deferred) |
| High-resolution font | Opt-in; four windows (location, date, parameters, message); original pixel frames kept; original font on command select | Opt-in modern text box over the message band only; native glyphs by default | Same idea. Ours covers fewer windows |
| Live switching of display options | Yes, no return to title [3rd] | Applies live, no Keep/Revert for these prefs | Same |
| Presets | Four キャラ表示 presets | Original / Modern / Custom value bundles | Same idea |
| Read-skip / fast-forward | Hold ZL: seen text, training animations, dates | WP9 skip read/all, text speed, auto-advance | Same. The Switch also fast-forwards animations; ours does not yet |
| Cursor and command speed | Options for cursor, command and text speed | Text speed only | Missing in ours |
| Save slots | 100 slots, memos, profile preview | Native memory-card saves; quick save/load in the pause menu | Missing in ours (slot count, memo) |
| Name input | Kana grid + 漢字 tab + system keyboard | WP-D Latin page + physical keyboard typing | Same idea |
| Language | Japanese only | Japanese plus English packs | We do more |
| In-game manual | + button opens a manual | Hint line for the settings key only | Missing in ours |
| Movies | 4:3 with black bars | Class C pillarbox (black) | Same |
| Opt-out for wing panels | Toggle [3rd] | All enhancements opt-in, defaults unchanged | Same, but our default is more conservative |
| Fidelity proof | None published | Byte-identity matrix, emulator differential | We do more |

## 3. Recommendations for our port

Every item is presentation-only unless stated otherwise.
- **Fidelity-safe** means the item is opt-in, the native 320x240 Image and the tick, RNG and save traces stay unchanged, and it can be checked by the existing invariance matrix (WP7/WP10).
- **Not fidelity-safe** means it changes timing, input semantics or what the player is shown about game state.

1. **Keep the integer nearest-fit default, and treat the Switch as confirmation of it.** At 720p it uses exactly 3x with no filter. **Fidelity-safe** (already our design). Add a 1080p contact sheet to WP7 so integer 4x with bars can be compared with fractional 4.5x, because the Switch's own 1080p choice is unknown.
2. **Add a decorative wing fill for Class B/C as a third `display.wing_fill` value** (for example `frame`). The Switch shows that a static, branded side panel reads better than black bars and does not touch the picture.
   - Source it from original material only: the seasonal checker tile, the message-window frame or palette colours, or a plain colour sampled from the scene border.
   - Label it as port-authored. It must not look like the 4:3 image continues.
   - **Fidelity-safe** (opt-in, wings only; WP2's composer invariant already covers it). It needs a user decision, because it is a port-authored design element.
3. **Promote deferred item (iv), wide-HUD readouts, to a ticket, modelled on the Switch side panels.**
   - Left wing: a biorhythm summary computed from the same 23/28/33-day waves the scheduler uses. Right wing: the week's calendar and appointments from `live_hud_adapter`.
   - Draw it in the window-space modern layer. It must read state only and never write it.
   - **The native Image stays unchanged, but this is not fully fidelity-neutral.** It shows the player, on the command screen, information the PS1 shows only on the calendar screen. That changes how easy the game is to read.
   - Ship it default off, with its own toggle (matching the Switch's on/off), and keep it out of the 'Original (PS1 exact)' preset.
4. **Widen the modern text box to the Switch's four windows: location, date, parameters and message.** Keep the original font on command selection and keep the original pixel-art frames, as the Switch does (high-resolution text inside the native chrome). **Fidelity-safe** (window-space overlay, opt-in). This fits English better too, because it frees the HUD labels from 12x12 glyph limits. Extend WP4 and the T-39016 text layer instead of redrawing frames.
5. **Do not widen scenes with new or AI art to match New Graphics.** The Switch reached full 16:9 only with commissioned redraws. That supports [[widescreen-settings]]: original art cannot fill 16:9. Keep Class B/C pillarboxed. This is a recommendation to keep our policy; any future art pack would be **not fidelity-safe** and needs a separate user decision.
6. **Plan, but keep deferred, a small Tier 2 variant: edge-anchored HUD in wide mode.**
   - Move only the date pillar and location tag to the frame edges, and keep the message box centred and unchanged in size, as the Switch does.
   - **Not fidelity-safe** if done by moving native primitives (it breaks centre byte identity). It becomes safe only if those widgets are redrawn in the window-space layer and the native copies are masked in the presentation layer only.
   - Needs the user's approval, per the existing Tier 2 deferral.
7. **Extend fast-forward to training and stat animations (hold-to-accelerate).**
   - Only **fidelity-safe for state** if it runs several native ticks per displayed frame. It must never skip or reorder ticks, so the tick, RNG and save traces stay equal while wall-clock pacing changes.
   - Pacing parity (WP8) does not hold while the button is held. Make it opt-in, and report it as mode M in the matrix.
8. **Add cursor and command speed options only as presentation holds.** The Switch has both. If a speed option changes how many ticks a menu takes, it changes RNG call timing: **not fidelity-safe**. Prefer an accelerate-only (tick-batching) design like item 7, default off.
9. **Add a port-level save manager (many slots, memo, profile preview) that wraps the native memory-card images and does not change their format.** **Fidelity-safe** if each slot stores an unmodified native save image plus a sidecar memo, and the existing memory-card path stays the default. Coordinate with T-39007 and WP10.
10. **Add an in-game controls/manual page in the modern pause menu,** following the Switch's "+ 取扱説明書" prompt. **Fidelity-safe** (modern layer only).
11. **Allow a system or OS text field for name entry next to the native grid, writing native full-width codes and respecting the 4-character limit,** following the Switch's software-keyboard button. **Fidelity-safe** under WP-D's rules.
12. **Keep movies pillarboxed in black,** as the Switch does even in its extras. No change needed.

## 4. Open questions

- Is the Switch wallpaper fixed, or does it change by scene or season? Is there a black-bar or plain option? What is the option that toggles the side panels called? Watching the official PV (YouTube `t58nREjfHDc`) or the Nintendo Direct (`dPVI8tg0jyo`) could settle this. No footage was viewed.
- What is the docked 1080p scale: 4x with borders, or a fractional 4.5x?
- Do the Switch biorhythm symbols map to the same "poor day" thresholds as our scheduler spec (critical day, weak phase)? If so, recommendation 3 can reuse that exact rule.
- Which scenes besides walks and dates get widened New Graphics? Do the command and minigame screens always stay 4:3?
- What is in the complete options list (the truncated labels, any volume or voice settings)? Does the game have backlog, auto-advance, quick save or rewind? None of these was confirmed or ruled out.
- How does the standard edition play the in-game opening at boot?
- The supported-languages field on the eShop page could not be read (queue redirect).
- Fan and social-media reaction to the redrawn art and the anti-aliased font was not surveyed.
