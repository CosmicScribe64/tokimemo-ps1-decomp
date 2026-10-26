# Tokimeki Memorial (PS1) - Image and graphic text translation catalog

This catalog documents image assets and sprite regions containing Japanese text across Tokimeki Memorial: Forever with You (`SLPM-86053`).


This catalog documents **every image asset and sprite region containing Japanese text** (plus all suspected/borderline visual review candidates) across *Tokimeki Memorial: Forever with You* (`SLPM-86053`).

In accordance with the project requirements:
- **No replacement image binaries have been generated yet.**
- Every entry provides:
  1. **Asset & Region ID** (source path, frame/sheet index, pixel dimensions `W×H`)
  2. **Original Japanese Text** (exact transcription of visible lettering)
  3. **English Meaning / Translation**
  4. **Suggested Translated Graphic / Same-Size Abbreviation** (concrete lettering, font/pixel-budget fit, palette/shadow treatment, and layout notes so replacement graphics fit the exact original bounding box).


## Summary of visual review corpus

| Category | Source Container(s) | Confirmed Regions | Suspected Regions | Notes |
|---|---|---:|---:|---|
| Main Gameplay HUD, Calendar, Stats & Magazine | `CDROM/FUKUTEST/MAINROM.NBN` | 45 (235 palette variants) | 2 (4 variants) | Calendar suffixes, 9 stat labels, 7 weekday badges, *Memorial Spot* magazine headers, issue badges, genres, Konami ads |
| Options, Controls, Sound/Album & Cast Credits | `CDROM/FUKUTEST/OPTION.NBN` | 108 | 3 | 54 normal/highlighted button pairs (`16px` tall): speed, mouse/pad controls, minigames, 14 VA credits, 13 character names |
| Save/Load Memory Card Screen | `CDROM/FUKUTEST/FILESEL.NBN` | 15 | 0 | 4 command buttons (`48×32`), slot info card (`168×96`), 9 stats + 13 heroine evaluation labels |
| Name / Birthday / Blood Type Entry | `CDROM/NAME_ENT/NAME_ENT.NBN` | 14 | 0 | 4 full-screen entry layouts (`320×240`) + 5 charset tab pairs (`32×16`) |
| Title Screen Logo & Menu | `CDROM/FUKUTEST/TITLE.NBN` | 5 | 0 | Main logo (`288×146`) + `GAME START` / `OPTIONS` selected & unselected sprites |
| Exam Results Banner & Suffix | `CDROM/FUKUTEST/EXAMINE.NBN` | 2 | 0 | `期末試験結果発表` banner (`112×12` box) + `点` score suffix (`8×16`) |
| RPG Battle Command Sprites | `CDROM/BATTLE/VS_{HABU,KUMA,ROBO,SIKA,UFO}.BIN` | 45 (9 × 5 files) | 0 | Shared battle UI sheet: `攻撃`, `防御`, `奥義` (bright/dim `32×16`) + `ミス` (`16×8`) |
| Sports Festival Rule Cards | `CDROM/TAIIKU/{KASETU,NINSETU,SPSETU,TASETU}.BIN` | 5 | 0 | 5 full-screen (`320×240`) rule boards for the 4 Sports Festival events |
| All-Clear Group Photo Banner | `CDROM/LASTPHOT/ALL_C.NBN` | 3 | 0 | 1 bottom congratulatory banner split across 3 tile sheets |
| Background Stills (`BS` / `PROLODIR` / `TOKIMEKI`) | `CDROM/BS/*.BS`, `CDROM/PROLODIR/*.BS`, `CDROM/FUKUTEST/TOKIMEKI.BS` | 20 | 4 | Diegetic school gate signs, class rosters, club room signs, venue signs, chalkboards |
| Character Bust-Up Sprites | `CDROM/BUSTUP/{HU17,AS34,HI05,KO15}.BIN` | 1 | 3 | `HU17.BIN` yukata uchiwa fan (`祭`); 3 suspected sprites are non-text emblems |
| SD Chibi Reaction Bubbles | `CDROM/CLUB_SD/*`, `HOLI_SD/*`, `MAIN_SD/*` | 0 | 18 | All 18 suspected sprites use pictorial symbols (`...`, sweatdrop, `×`, peace hand, `K` shirt initial), no Japanese text |
| Opening FMV Video Stream | `CDROM/OPENXA.STR` | 16+ (cast/credit overlays) | 0 | Opening anime movie cast name pills (`288×192`, MDEC video frames) |


## 1. main gameplay hud, calendar, stats & magazine(`CDROM/FUKUTEST/MAINROM.NBN`)

`MAINROM.NBN` contains 47 distinct graphical frames/sheets (duplicated across 5 seasonal/UI palette groups in the raw export for 239 total records).

### 1.1 hud & calendar chrome

| Frame ID | Source Sheet / Offset | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|---|
| `frame-0057` .. `0059` | `chunk 4 (0x8000)` | `32×24` | `セーブ` / `不可` *(over memory card icon)* | Cannot Save / Save Unavailable | **`NO` / `SAVE`** stacked in bold 5px red pixel type with white/dark outline over the memory card icon (`32×24`), or a red **`X`** badge with `SAVE` below. |
| `frame-0060` | `chunk 4 (0x8000)` (`x=96, y=16`) | `40×139` *(panel)* / `16×16` *(cells)* | `年`, `月`, `日`, `( )` | Year, Month, Day suffixes (`95年 4月 23日 (日)`) | Replace `年` glyph cell (`9 rows` at `x=96, y=16`) with **`/`** separator (`7px` tall, palette ink 9 + shadow 10) and repoint `月` to `/` and `日` to blank so the top-left HUD reads **`95 / 4 / 23 (Su)`**. |
| `frame-0061` | `chunk 9 (0x12000)` (`x=96..127, y=0..119`) | `296×67` *(panel)* / three `32×40` label strips (`27px` max label width before `-`, `14px` row pitch) | **Col 1**: `体調` / `芸術` / `容姿`<br>**Col 2**: `文系` / `運動` / `根性`<br>**Col 3**: `理系` / `雑学` / `ストレス` | **Col 1**: Condition / Art / Looks<br>**Col 2**: Liberal Arts / Athletics / Guts<br>**Col 3**: Science / Trivia / Stress | **Col 1**: **`Health`** (`26px`, or **`Cond.`** `22px`) / **`Art`** (`14px`) / **`Looks`** (`23px`)<br>**Col 2**: **`Lit.`** (`14px`) / **`Sport`** (`23px`, or **`Fit.`** `15px`) / **`Guts`** (`19px`)<br>**Col 3**: **`Sci.`** (`15px`) / **`Trivia`** (`23px`) / **`Stress`** (`25px`)<br>Rendered in 7px-tall condensed proportional pixel font (ink index 9, drop shadow index 10 at `+1,+1`), preserving the `-` separator at `x+28..30`. |
| `frame-0082` | `chunk 4 (0x8000)` (`x=64, y=0`) | `16×16` | `月` | Monday | **`Mo`** (centered in `16×16` cell, 11px wide + 1px shadow) |
| `frame-0083` | `chunk 4 (0x8000)` (`x=80, y=0`) | `16×16` | `火` | Tuesday | **`Tu`** (centered in `16×16` cell) |
| `frame-0084` | `chunk 4 (0x8000)` (`x=96, y=0`) | `16×16` | `水` | Wednesday | **`We`** (centered in `16×16` cell) |
| `frame-0085` | `chunk 4 (0x8000)` (`x=112, y=0`) | `16×16` | `木` | Thursday | **`Th`** (centered in `16×16` cell) |
| `frame-0086` | `chunk 4 (0x8000)` (`x=64, y=16`) | `16×16` | `金` | Friday | **`Fr`** (centered in `16×16` cell) |
| `frame-0087` | `chunk 4 (0x8000)` (`x=80, y=16`) | `16×16` | `土` | Saturday | **`Sa`** (centered in `16×16` cell) |
| `frame-0088` | `chunk 4 (0x8000)` (`x=96, y=16`) | `16×16` | `日` | Sunday | **`Su`** (centered in `16×16` cell, red holiday palette preserved) |

### 1.2 information magazine(*Memorial Spot* or `メモリアルスポット`)

| Frame ID | Source Sheet / Offset | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|---|
| `frame-0118` | Magazine base page | `328×171` | `メモリアルスポット` / `MEMORIAL SPOT` / `MOVIE` / `CONCERT` / `EVENT` | Memorial Spot (Town Information Magazine) | Section headers (`MOVIE`, `CONCERT`, `EVENT`) and subtitle (`MEMORIAL SPOT`) are already in English. Either keep the stylized katakana masthead with its existing `MEMORIAL SPOT` sub-banner, or replace the katakana banner (`144×20`) with bold retro magazine lettering **`MEMORIAL SPOT`**. |
| `frame-0120` *(suspected)* | Magazine stadium graphic | `136×56` | `STADIUM` | Stadium | Already Latin text (`STADIUM`); no change required. |
| `frame-0121` | Issue badge (Spring Y1) | `32×16` | `創刊` | Inaugural Issue / Issue #1 | **`No.1`** or **`1st`** in white/pink pixel type inside the `32×16` badge. |
| `frame-0122` | Issue badge (Spring) | `56×16` | `3・4・5月号` | Mar / Apr / May Issue | **`Mar-May`** or **`3-4-5`** in green pixel type inside `56×16`. |
| `frame-0123` | Issue badge (Summer) | `56×16` | `6・7・8月号` | Jun / Jul / Aug Issue | **`Jun-Aug`** or **`6-7-8`** in blue pixel type inside `56×16`. |
| `frame-0124` | Issue badge (Autumn) | `56×16` | `9・10・11月号` | Sep / Oct / Nov Issue | **`Sep-Nov`** or **`9-10-11`** in orange pixel type inside `56×16`. |
| `frame-0125` | Issue badge (Winter) | `56×16` | `12・1・2月号` | Dec / Jan / Feb Issue | **`Dec-Feb`** or **`12-1-2`** in purple pixel type inside `56×16`. |
| `frame-0126` | `chunk 11 (0x16000)` (`x=0, y=64`) | `136×24` | `中央公園紹介` | Central Park Guide | **`Central Park`** (`128×24`, 16px bold serif/sans with 1px shadow; ink 2 green, shadow 4). |
| `frame-0127` | `chunk 11 (0x16000)` (`x=0, y=88`) | `120×24` | `美術館完成` | Art Museum Completed | **`Museum Opens`** (`120×24`, ink 7 pink, shadow 9). |
| `frame-0128` | `chunk 11 (0x16000)` (`x=0, y=112`) | `112×24` | `水族館特集` | Aquarium Feature | **`Aquarium`** (`112×24`, ink 12 blue, shadow 14). |
| `frame-0129` | `chunk 2 (0x4000)` (`x=0, y=184`) | `144×24` | `スタジアム完成` | Stadium Completed | **`New Stadium`** (`128×24` + clear 16px tail, ink 2 orange, shadow 4). |
| `frame-0130` | `chunk 2 (0x4000)` (`x=0, y=208`) | `112×24` | `植物園特集` | Botanical Garden Feature | **`Garden Guide`** (`112×24`, ink 7 pink, shadow 9). |
| `frame-0131` | `chunk 2 (0x4000)` (`x=0, y=232`) | `144×24` | `ボーリング特集` | Bowling Alley Feature | **`Bowling`** (`128×24` + clear 16px tail, ink 12 yellow/blue, shadow 14). |
| `frame-0132` | `chunk 7 (0xE000)` (`x=0, y=184`) | `152×24` | `カラオケBOX完成` | Karaoke Box Completed | **`Karaoke Opens`** (`128×24` + clear 24px tail, ink 2 pink, shadow 4). |
| `frame-0133` | `chunk 7 (0xE000)` (`x=0, y=208`) | `152×24` | `プラネタリウム特集` | Planetarium Feature | **`Planetarium`** (`128×24` + clear 24px tail, ink 7 purple, shadow 9). |
| `frame-0134` | Magazine genre tag | `40×8` | `コメディー` | Comedy | **`Comedy`** (`40×8`, 5×7 compact font) |
| `frame-0135` | Magazine genre tag | `48×8` | `ラブロマンス` | Romance | **`Romance`** (`48×8`, 5×7 compact font) |
| `frame-0136` | Magazine genre tag | `40×8` | `アクション` | Action | **`Action`** (`40×8`, 5×7 compact font) |
| `frame-0137` | Magazine genre tag | `40×8` | `サスペンス` | Suspense | **`Suspense`** (`40×8`, 4×7 condensed font) |
| `frame-0138` | Magazine genre tag | `24×8` | `ホラー` | Horror | **`Horror`** (`24×8`, 3×7 ultra-condensed or **`Horr.`**) |
| `frame-0139` | Magazine genre tag | `24×8` | `アニメ` | Anime | **`Anime`** (`24×8`, 4×7 condensed font) |
| `frame-0140` | Magazine genre tag | `40×8` | `クラシック` | Classical | **`Classic`** (`40×8`, 5×7 compact font) |
| `frame-0141` | Magazine genre tag | `32×8` | `アイドル` | Idol Pop | **`Idol`** (`32×8`, centered) |
| `frame-0142` | Magazine genre tag | `24×8` | `ロック` | Rock | **`Rock`** (`24×8`, centered) |
| `frame-0143` | Magazine genre tag | `64×8` | `ニューミュージック` | New Music / Pop Music | **`New Music`** or **`Pop Music`** (`64×8`) |
| `frame-0144` | Magazine event tag | `32×16` | `プロレス` | Pro Wrestling | **`Wrestl.`** or **`ProWres`** (`32×16`, or 2-line `Pro` / `Wres`) |
| `frame-0145` | Magazine event tag | `32×16` | `野球` | Baseball | **`Baseball`** (`32×16`, condensed 4px font or 2-line `Base-` / `ball`) |
| `frame-0151` | Konami ad insert 1 | `56×66` | `幻想水滸伝 GENSOSUIKODEN` / `'95発売予定` | Gensosuikoden (Suikoden) / Coming in '95 | Keep game logo art; replace bottom `56×12` strip `'95発売予定` with **`Out in '95`** or **`'95 Release`**. |
| `frame-0152` | Konami ad insert 2 | `56×66` | `極上パロディウス` / `好評発売中` | Gokujou Parodius / Now on Sale | Keep game logo art; replace bottom `56×12` strip `好評発売中` with **`Out Now!`** or **`On Sale!`**. |
| `frame-0153` | Konami ad insert 3 | `56×66` | `実況パワフルプロ野球'95` / `好評発売中` | Jikkyou Powerful Pro Yakyuu '95 / Now on Sale | Keep game logo art; replace bottom `56×12` strip `好評発売中` with **`Out Now!`**. |
| `frame-0154` | Konami ad insert 4 | `56×66` | `ツインビー対戦ぱずるだま` / `好評発売中` | TwinBee Taisen Puzzle-Dama / Now on Sale | Keep game logo art; replace bottom `56×12` strip `好評発売中` with **`Out Now!`**. |
| `frame-0173` *(suspected)* | Small ribbon icon | `16×16` | *(Pictorial ribbon icon)* | N/A (No text) | No change required. |
| `sheet-02000` | Unreferenced VRAM sheet (`0x2000`) | `128×128` | `星座:` / `B.W.H` / `電話:` / `血液型` / `日` / `考:` | Sign: / B.W.H / Tel: / Blood Type / Day / Note: | Unused prototype profile labels in VRAM (`Sign:`, `B.W.H`, `Tel:`, `Blood`). Safe to leave or replace in-place. |


## 2. options, controls, sound/album & cast credits(`CDROM/FUKUTEST/OPTION.NBN`)

Every text label in `OPTION.NBN` consists of a **normal (unselected)** sprite and a **highlighted (selected)** sprite of identical height (`16px`).

### 2.1 message speed, mouse/controller & minigame labels

| Frame IDs (Normal / Highlight) | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `0011` / `0012` | `44×16` | `超遅い` | Very Slow (Text Speed) | **`V.Slow`** (`44×16`) |
| `0013` / `0014` | `30×16` | `遅い` | Slow | **`Slow`** (`30×16`) |
| `0015` / `0016` | `30×16` | `普通` | Normal | **`Norm`** (`30×16`, or **`Normal`** if sprite width entry is widened to `44px`) |
| `0017` / `0018` | `30×16` | `速い` | Fast | **`Fast`** (`30×16`) |
| `0019` / `0020` | `44×16` | `超速い` | Very Fast | **`V.Fast`** (`44×16`) |
| `0025` / `0026` | `44×16` | `フリー` | Free (Cursor Mode) | **`Free`** (`44×16`) |
| `0027` / `0028` | `72×16` | `決定ボタン` | Confirm Button | **`Confirm Btn`** (`72×16`) |
| `0029` / `0030` | `100×16` | `クリックモード` | Click Mode | **`Click Mode`** (`100×16`) |
| `0031` / `0032` | `114×16` | `クリックスピード` | Click Speed | **`Click Speed`** (`114×16`) |
| `0033` / `0034` | `100×16` | `右ボタンで決定` | Right Button Confirms | **`Right Click OK`** (`100×16`) |
| `0035` / `0036` | `100×16` | `左ボタンで決定` | Left Button Confirms | **`Left Click OK`** (`100×16`) |
| `0037` / `0038` | `114×16` | `シングルクリック` | Single Click | **`Single Click`** (`114×16`) |
| `0039` / `0040` | `100×16` | `ダブルクリック` | Double Click | **`Double Click`** (`100×16`) |
| `0041` / `0042` | `58×16` | `初期状態` | Default Settings | **`Default`** (`58×16`) |
| `0043` / `0044` | `30×16` | `変更` | Change / Edit | **`Edit`** (`30×16`) |
| `0049` / `0050` | `114×16` | `×ボタンでカット` | Skip with X Button | **`X Button Skips`** (`114×16`) |
| `0051` / `0052` | `72×16` | `サブゲーム` | Minigames / Subgames | **`Minigames`** (`72×16`) |
| `0053` / `0054` | `86×16` | `フリートーク` | Free Talk (Voice Actor Commentaries) | **`Free Talk`** (`86×16`) |
| `0055` / `0056` | `86×16` | `エンディング` | Ending | **`Ending`** (`86×16`) |
| `0057` / `0058` | `100×16` | `アルバムモード` | Album Mode | **`Album Mode`** (`100×16`) |
| `0059` / `0060` | `170×16` | `ツインビータイムアタック` | TwinBee Time Attack | **`TwinBee Time Attack`** (`170×16`) |
| `0061` / `0062` | `198×16` | `スターダストシンフォニーEX` | Stardust Symphony EX | **`Stardust Symphony EX`** (`198×16`) |
| `0117` / `0118` | `86×16` | `バッドエンド` | Bad Ending | **`Bad Ending`** (`86×16`) |
| `0119` .. `0121` *(suspected)* | `48×32` | *(Jukebox / arcade cabinet graphic)* | N/A (Pictorial icon) | No change required. |
| `0122` / `0123` | `86×16` | `マウスライク` | Mouse-like (Control Mode) | **`Mouse-like`** (`86×16`) |
| `0124` / `0125` | `58×16` | `パッドA` | Pad A | **`Pad A`** (`58×16`) |
| `0126` / `0127` | `58×16` | `パッドB` | Pad B | **`Pad B`** (`58×16`) |
| `0128` / `0129` | `156×16` | `マウスライク+パッドA` | Mouse-like + Pad A | **`Mouse-like + Pad A`** (`156×16`) |

### 2.2 voice actor free talk credits(`OPTION.NBN` Frames `0063`–`0090`)

| Frame IDs (Normal / Highlight) | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `0063` / `0064` | `122×16` | `金月真美 (藤崎)` | Kingetsu Mami (Fujisaki) | **`Kingetsu Mami (Fujisaki)`** (`122×16` condensed, or **`M. Kingetsu (Fujisaki)`**) |
| `0065` / `0066` | `122×16` | `長沢美樹 (如月)` | Nagasawa Miki (Kisaragi) | **`Nagasawa Miki (Kisaragi)`** (`122×16` condensed, or **`M. Nagasawa (Kisaragi)`**) |
| `0067` / `0068` | `108×16` | `中友子 (紐緒)` | Naka Tomoko (Himoo) | **`Naka Tomoko (Himoo)`** (`108×16`) |
| `0069` / `0070` | `122×16` | `川口雅代 (片桐)` | Kawaguchi Masayo (Katagiri) | **`M. Kawaguchi (Katagiri)`** (`122×16`) |
| `0071` / `0072` | `122×16` | `鉄炮塚葉子 (朝日奈)` | Teppouzuka Yoko (Asahina) | **`Y. Teppouzuka (Asahina)`** (`122×16`) |
| `0073` / `0074` | `122×16` | `黒崎彩子 (古式)` | Kurosaki Ayako (Koshiki) | **`Kurosaki Ayako (Koshiki)`** (`122×16` condensed, or **`A. Kurosaki (Koshiki)`**) |
| `0075` / `0076` | `122×16` | `よしきくりん (早乙女)` | Yoshiki Kurin (Saotome) | **`Yoshiki Kurin (Saotome)`** (`122×16` condensed, or **`K. Yoshiki (Saotome)`**) |
| `0077` / `0078` | `108×16` | `관根明子 (如月)` → `関根明子 (如月)` | Sekine Akiko (Kisaragi) | **`Sekine Akiko (Kisaragi)`** (`108×16` condensed, or **`A. Sekine (Kisaragi)`**) |
| `0079` / `0080` | `122×16` | `栗原みきこ (美樹原)` | Kurihara Mikiko (Mikihara) | **`M. Kurihara (Mikihara)`** (`122×16`) |
| `0081` / `0082` | `108×16` | `五十嵐麗 (鏡)` | Igarashi Rei (Kagami) | **`Igarashi Rei (Kagami)`** (`108×16`) |
| `0083` / `0084` | `122×16` | `笹木綾子 (清川)` | Sasaki Ayako (Kiyokawa) | **`Sasaki Ayako (Kiyokawa)`** (`122×16` condensed, or **`A. Sasaki (Kiyokawa)`**) |
| `0085` / `0086` | `152×16` | `津野田なるみ (伊集院)` | Tsunoda Narumi (Ijuin) | **`Tsunoda Narumi (Ijuin)`** (`152×16`) |
| `0087` / `0088` | `136×16` | `菊池志穂 (館林)` | Kikuchi Shiho (Tatebayashi) | **`Kikuchi Shiho (Tatebayashi)`** (`136×16` condensed, or **`S. Kikuchi (Tatebayashi)`**) |
| `0089` / `0090` | `122×16` | `上田祐司 (好雄)` | Ueda Yuji (Yoshio) | **`Ueda Yuji (Yoshio)`** (`122×16`) |

### 2.3 album & ending character name labels(`OPTION.NBN` Frames `0091`–`0116`)

| Frame IDs (Normal / Highlight) | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `0091` / `0092` | `70×16` | `藤崎 詩織` | Fujisaki Shiori | **`Fujisaki Shiori`** (`70×16` in 5px condensed font, or **`Fujisaki`** / widen sprite table entry to `96px`) |
| `0093` / `0094` | `70×16` | `如月 未緒` | Kisaragi Mio | **`Kisaragi Mio`** (`70×16`) |
| `0095` / `0096` | `70×16` | `紐緒 結奈` | Himoo Yuina | **`Himoo Yuina`** (`70×16`) |
| `0097` / `0098` | `70×16` | `片桐 彩子` | Katagiri Ayako | **`Katagiri Ayako`** (`70×16`) |
| `0099` / `0100` | `70×16` | `虹野 沙希` | Nijino Saki | **`Nijino Saki`** (`70×16`) |
| `0101` / `0102` | `84×16` | `古式 ゆかり` | Koshiki Yukari | **`Koshiki Yukari`** (`84×16`) |
| `0103` / `0104` | `56×16` | `清川 望` | Kiyokawa Nozomi | **`Kiyokawa`** (`56×16`, or **`Kiyokawa Nozomi`** if sprite width entry widened to `90px`) |
| `0105` / `0106` | `56×16` | `鏡 魅羅` | Kagami Mira | **`Kagami Mira`** (`56×16`) |
| `0107` / `0108` | `84×16` | `朝日奈 夕子` | Asahina Yuko | **`Asahina Yuko`** (`84×16`) |
| `0109` / `0110` | `70×16` | `美樹原 愛` | Mikihara Megumi | **`Mikihara Megumi`** (`70×16` condensed, or **`Mikihara`**) |
| `0111` / `0112` | `70×16` | `早乙女 優美` | Saotome Yumi | **`Saotome Yumi`** (`70×16`) |
| `0113` / `0114` | `70×16` | `館林 見晴` | Tatebayashi Miharu | **`Tatebayashi`** (`70×16`, or **`Tatebayashi Miharu`** if widened) |
| `0115` / `0116` | `84×16` | `伊集院 レイ` | Ijuin Rei | **`Ijuin Rei`** (`84×16`) |


## 3. save or load memory card screen(`CDROM/FUKUTEST/FILESEL.NBN`)

| Frame / Region ID | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `frame-0000` (+ highlighted pair on `page-0000`) | `48×32` | `コピー` | Copy | **`Copy`** (centered in `48×32` 3D bevel button) |
| `frame-0001` (+ highlighted pair) | `48×32` | `ニュー` | New | **`New`** (centered in `48×32` button) |
| `frame-0002` (+ highlighted pair) | `48×32` | `クリア` | Clear / Delete | **`Clear`** (centered in `48×32` button) |
| `frame-0003` (+ highlighted pair) | `48×32` | `ロード` | Load | **`Load`** (centered in `48×32` button) |
| `frame-0019` (`page-0003` top-left card) | `168×96` | `19__年__月__日`<br>`名前` / `あだ名`<br>`所属部`<br>`クリア人数` … `人`<br>`システム` | `19__/__/__`<br>Name / Nickname<br>Club<br>Cleared Heroines<br>System Data | **`19__/__/__`** (replace `年`/`月` with `/`, erase `日`)<br>**`Name`** (`30×12`) / **`Nickname`** (`44×12`)<br>**`Club`** (`30×12`)<br>**`Clears`** (`44×12`, erase `人` suffix at `x=108..119`)<br>**`Sys.`** (`24×12`) |
| `frame-0020` .. `0022` (`page-0004` bottom strip) | `280×152` *(composite)* | **Headers**: `パラメータ`, `女の子の評価`, `クリア回数`<br>**9 Stats** (`24×10` to `38×10`): `体調`, `文系`, `理系`, `芸術`, `運動`, `雑学`, `容姿`, `根性`, `ストレス`<br>**13 Heroines** (`24×10` to `38×10`): `藤崎`, `如月`, `紐緒`, `片桐`, `虹野`, `古式`, `清川`, `鏡`, `伊集院`, `朝日奈`, `美樹原`, `早乙女`, `館林` | **Headers**: Parameters, Girls' Ratings, Times Cleared<br>**9 Stats**: Condition, Liberal Arts, Science, Art, Athletics, Trivia, Looks, Guts, Stress<br>**13 Heroines**: Fujisaki, Kisaragi, Himoo, Katagiri, Nijino, Koshiki, Kiyokawa, Kagami, Ijuin, Asahina, Mikihara, Saotome, Tatebayashi | **Headers**: **`Stats`** (`50×12`), **`Girls' ratings`** (`88×12`), **`Clears`** (`42×12`)<br>**9 Stats**: **`Health`**, **`Lit.`**, **`Sci.`**, **`Art`**, **`Sport`**, **`Trivia`**, **`Looks`**, **`Guts`**, **`Stress`**<br>**13 Heroines** (`29px` max width before `-`):<br>• Option A (Given names, fits `29px` cleanly): **`Shiori`**, **`Mio`**, **`Yuina`**, **`Ayako`**, **`Saki`**, **`Yukari`**, **`Nozomi`**, **`Mira`**, **`Rei`**, **`Yuko`**, **`Megumi`**, **`Yumi`**, **`Miharu`**<br>• Option B (Abbreviated surnames): **`Fujis.`**, **`Kisar.`**, **`Himoo`**, **`Katag.`**, **`Nijino`**, **`Koshi.`**, **`Kiyok.`**, **`Kagami`**, **`Ijuin`**, **`Asahi.`**, **`Mikih.`**, **`Saoto.`**, **`Tateb.`** |
| `frame-0030` / `0031` | `16×32` | `有` / `無` | Present (Yes) / Absent (No) | **`Y`** / **`N`** (centered in `16×32` system-file indicator badge) |


## 4. name, nickname, birthday & blood type entry(`CDROM/NAME_ENT/NAME_ENT.NBN`)

| Frame ID | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `frame-0000` / `0001` / `0002` / `0014` | `320×240` *(full layout pages)* | **Top Box**: `名前:`, `あだ名:`, `誕生日:`, `月`, `日`, `血液型:`, `型`<br>**Month Grid**: `誕生月`, `1月`..`12月`<br>**Blood Grid**: `血液型`, `A型`, `B型`, `AB型`, `O型`<br>**Confirm**: `はい`, `いいえ` | **Top Box**: Name:, Nickname:, Birthday:, Month/Day, Blood Type:<br>**Month Grid**: Birth Month, Jan..Dec<br>**Blood Grid**: Blood Type, Type A, B, AB, O<br>**Confirm**: Yes, No | **Top Box**: **`Name:`** (`36×14`), **`Nickname:`** (`48×14`), **`Birthday:`** (`48×14`), replace `月` (`14×14`) with **`/`**, clear `日` (`14×14`), **`Blood type:`** (`48×14`), clear suffix `型` (`16×14`).<br>**Month Grid**: Header **`Birthday`** (`64×14`), cells **`1`**..**`12`** (or **`Jan`**..**`Dec`** in `28×16` cells).<br>**Blood Grid**: Header **`Blood type`** (`64×14`), cells **`A`**, **`B`**, **`AB`**, **`O`**.<br>**Confirm**: **`Yes`** (`32×14`), **`No`** (`32×14`). |
| `frame-0015` / `0016` | `32×16` | `かな` *(normal / lit)* | Hiragana | **`Hira`** (or **`ABC`** if remapped to Latin keyboard; `32×16`) |
| `frame-0017` / `0018` | `32×16` | `カナ` *(normal / lit)* | Katakana | **`Kata`** (`32×16`) |
| `frame-0019` / `0020` | `32×16` | `記号` *(normal / lit)* | Symbols | **`Sym`** (`32×16`) |
| `frame-0021` / `0022` | `32×16` | `漢字` *(normal / lit)* | Kanji | **`Kanji`** (`32×16`) |
| `frame-0023` / `0024` | `32×16` | `終了` *(normal / lit)* | Done / End | **`Done`** (`32×16`) |


## 5. title screen logo & menu(`CDROM/FUKUTEST/TITLE.NBN`)

| Frame ID | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `frame-0000` | `288×146` | `ときめきメモリアル forever with you` | Tokimeki Memorial: forever with you | Keep the iconic heart-emblem logo; optionally add or replace the main ribbon lettering with **`TOKIMEKI MEMORIAL`** above the existing English **`forever with you`** script. |
| `frame-0001` | `144×16` | `ゲームスタート` *(selected blue glow)* | GAME START | **`GAME START`** (`112×16` centered in `144×16`, blue gradient palette indices 2–6 with dark outline index 7). |
| `frame-0002` | `112×16` | `ゲームスタート` *(unselected grey)* | GAME START | **`GAME START`** (`112×16`, grey metallic gradient indices 2–6 with outline index 7). |
| `frame-0003` | `144×16` | `オプション` *(selected blue glow)* | OPTIONS | **`OPTIONS`** (`80×16` centered in `144×16`, blue gradient palette indices 2–6). |
| `frame-0004` | `80×16` | `オプション` *(unselected grey)* | OPTIONS | **`OPTIONS`** (`80×16`, grey metallic gradient indices 2–6). |


## 6. exam results screen(`CDROM/FUKUTEST/EXAMINE.NBN`)

| Frame ID | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `frame-0000` | `320×152` *(banner box `112×12` at `x=20, y=56`)* | `期末試験結果発表` | Final Exam Results Announcement | **`Exam Results`** centered inside the `112×12` banner box (`x=20..131, y=56..67`, dark ink index 3 on light plaque index 13). |
| `frame-0001` | `8×16` | `点` | points (score suffix, e.g., `425点`) | Blank out (`transparent`) so scores read `425`, or render compact **`pt`** in `8×16`. |


## 7. rpg battle command sprites(`CDROM/BATTLE/VS_{HABU,KUMA,ROBO,SIKA,UFO}.BIN`)

All 5 files (`VS_HABU.BIN`, `VS_KUMA.BIN`, `VS_ROBO.BIN`, `VS_SIKA.BIN`, `VS_UFO.BIN`) share an identical battle UI sprite sheet (`128×72` region in `source-index-0002.png` at `0x42000`):

| Region / Cell | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `x=0..31, y=16..31` (bright) & `y=32..47` (dim) | `32×16` | `攻撃` | Attack / Fight | **`Fight`** (`32×16`, gold gradient palette indices 9–14 + dark outline index 8) |
| `x=32..63, y=16..31` (bright) & `y=32..47` (dim) | `32×16` | `防御` | Defend / Guard | **`Guard`** (`32×16`, gold gradient + dark outline) |
| `x=64..95, y=16..31` (bright) & `y=32..47` (dim) | `32×16` | `奥義` | Secret Art / Special Skill | **`Skill`** (`32×16`, gold gradient + dark outline) |
| `x=0..15, y=56..63` | `16×8` | `ミス` | Miss | **`Miss`** (`16×8`, 4px compact pixel type) |


## 8. sports festival instruction cards(`CDROM/TAIIKU/{KASETU,NINSETU,SPSETU,TASETU}.BIN`)

Each file contains full-screen (`320×240`) chalkboard-style event rule cards (`256×216` active text area at `x=32..288, y=20..236`):

| Asset Path | Card Index & Size | Original Japanese | English Translation & Suggested Same-Size Layout (`320×240`) |
|---|---|---|---|
| `CDROM/TAIIKU/KASETU.BIN` | `index 0` (`320×240`) | **Title**: `借り物競走説明`<br>1. `◯ボタン連打で走ります。`<br>2. `×ボタンでジャンプします。`<br>3. `ハードルをジャンプで飛び越えて下さい。`<br>4. `パンの前では×ボタンでジャンプして、`<br>　 `さらに×ボタンを押すとパンをくわえます。`<br>5. `テーブルの前でお題の品物を×ボタンで`<br>　 `取ってゴールまで走って下さい。` | **Title**: **`Borrowing Race`** (yellow ink 2)<br>• `Tap O repeatedly to run.`<br>• `Press X to jump over the hurdles.`<br>• `Jump in front of the bread and press`<br>  `X again to grab it in your mouth.`<br>• `Pick up the matching item at the`<br>  `table with X and run to the finish!` |
| `CDROM/TAIIKU/NINSETU.BIN` | `index 0` (`320×240`, casual)<br>`index 1` (`320×240`, polite) | **Title**: `二人三脚 説明`<br>1. `◯ボタンと方向キーの右を交互に押すと`<br>　 `走ります。`<br>2. `パートナーとタイミングを合わせて`<br>　 `交互に押して下さい。`<br>3. `タイミングが合わないと転びます。`<br>4. `早くゴールした方が勝ちです。` | **Title**: **`Three-Legged Race`** (yellow ink 2)<br>• `Alternate pressing O and Right`<br>  `on the D-Pad to run.`<br>• `Match your partner's rhythm as`<br>  `you alternate buttons.`<br>• `If your timing is off, you'll trip!`<br>• `First team to the finish line wins!` |
| `CDROM/TAIIKU/SPSETU.BIN` | `index 0` (`320×240`) | **Title**: `スプーンリレー説明`<br>1. `◯ボタン連打で走ります。`<br>2. `走るとたまごが左右にゆれますので`<br>　 `方向キーの左右でバランスを取って下さい。`<br>3. `たまごを落としてしまったら止まって`<br>　 `ひろいなおします。`<br>4. `たまごを落とさないように早くゴール`<br>　 `して下さい。` | **Title**: **`Egg & Spoon Race`** (or **`Spoon Relay`**)<br>• `Tap O repeatedly to run.`<br>• `Running makes the egg wobble.`<br>• `Use Left and Right on the D-Pad`<br>  `to keep the egg balanced.`<br>• `If you drop the egg, you must stop`<br>  `and pick it back up.` |
| `CDROM/TAIIKU/TASETU.BIN` | `index 0` (`320×240`) | **Title**: `玉転がし 説明`<br>1. `◯ボタン連打で大玉を転がします。`<br>2. `力の入れすぎや、入れなさすぎで`<br>　 `玉から離れてしまいます。`<br>3. `うまく玉との距離を保ちながら`<br>　 `ゴールまで転がして下さい。` | **Title**: **`Giant Ball Roll`** (or **`Ball Rolling`**)<br>• `Tap O repeatedly to push the ball.`<br>• `Pushing too hard or too softly will`<br>  `throw off your distance.`<br>• `Keep a steady pace to stay close`<br>  `to the ball all the way to the goal!` |


## 9. all-clear group photo banner(`CDROM/LASTPHOT/ALL_C.NBN`)

| Frame ID | Pixel Size | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| `frame-0000` + `source-index-0001` / `0002` | `264×190` *(banner strip `264×18` across bottom)* | `オールクリアおめでとう！` | Congratulations on Clearing All Routes! | **`All Clear! Congratulations!`** (or **`Congratulations on Clearing All Routes!`**) in bold yellow/gold pixel lettering with dark outline across the bottom photo border (`264×18`). |


## 10. background stills(`CDROM/BS/*.BS`, `CDROM/PROLODIR/*.BS`, `CDROM/FUKUTEST/TOKIMEKI.BS`)

Diegetic signage, chalkboards, and title cards in background MDEC `.BS` images:

| Asset Path | Pixel Size | Status | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|---|
| `CDROM/FUKUTEST/TOKIMEKI.BS` | `320×240` | Confirmed | `ときめきメモリアル forever with you` | Tokimeki Memorial: forever with you | Title splash screen; matches `TITLE.NBN` frame 0. Keep iconic logo or pair **`TOKIMEKI MEMORIAL`** with existing `forever with you`. |
| `CDROM/PROLODIR/P_01_01.BS` | `320×160` | Confirmed | `私立きらめき高等学校` *(vertical gate pillar plate)* | Kirameki Private High School | Note: The arch next to it already says **`KIRAMEKI HIGH SCHOOL`** in English! The vertical wooden plaque (`14×72`) can either remain as authentic Japanese school-gate kanji or carry vertical **`KIRAMEKI HIGH`**. |
| `CDROM/PROLODIR/P_01_02.BS` | `640×160` | Confirmed | `1年A組` / `1年B組` / `1年C組` + student name columns | Class 1-A / Class 1-B / Class 1-C Roster Board | Replace top headers (`36×12` each) with **`Class 1-A`**, **`Class 1-B`**, **`Class 1-C`**. |
| `CDROM/PROLODIR/P_01_03.BS` & `P_01_03B.BS` | `320×160` | Confirmed | Vertical roster slips: `田中 紗希`, `橋本 良子`, `藤崎 詩織` *(center)*, `本田 めぐみ`, `水川 めぐみ`, `最上 美穂` | Roster close-up highlighting **Fujisaki Shiori** | Rotate or stack horizontal initials/names in the vertical slips to highlight **`Fujisaki Shiori`** (`S. Fujisaki` /vertical `FUJISAKI SHIORI`) in the center column so the visual beat of spotting Shiori's name on the class list is preserved. |
| `CDROM/PROLODIR/P_01_04B.BS` & `P_01_07.BS` | `320×160` | Suspected | *(Unreadable 1–2px chalk/bulletin squiggles)* | N/A (No legible text) | No change required. |
| `CDROM/BS/D_01_47.BS` | `320×160` | Confirmed | `きらめき高校 体育祭` *(vertical standing sign)* + `私立きらめき高等学校` *(gate pillar)* | Kirameki High Sports Festival | Standing signboard (`28×96`): vertical/stacked **`KIRAMEKI HIGH`** / **`SPORTS FEST`** (`Sports` / `Festival`). |
| `CDROM/BS/D_01_48.BS` | `320×160` | Confirmed | `きらめき高校 文化祭` *(vertical standing sign)* + `私立きらめき高等学校` *(gate pillar)* | Kirameki High Cultural Festival | Standing signboard (`28×96`): stacked **`KIRAMEKI HIGH`** / **`CULTURE FEST`** (`Cultural` / `Festival`). |
| `CDROM/BS/D_01_67.BS` | `320×160` | Confirmed | `科学部` *(vertical door plaque)* | Science Club | Door plaque (`14×36`): vertical **`SCI`** or **`LAB`** (`Science Club`). |
| `CDROM/BS/D_01_69.BS` | `320×160` | Confirmed | `電脳部` *(wall banner)* | Computer Club | Wall banner (`48×18`): **`PC CLUB`** or **`COMP. CLUB`**. |
| `CDROM/BS/D_01_70.BS` | `320×160` | Suspected | *(Art Club easel paintings)* | N/A (No legible text) | No change required. |
| `CDROM/BS/D_03_06.BS` | `320×160` | Confirmed | `甲` / `子` / `園` *(3 vertical ivy plaques on stadium wall)* | Koshien Stadium | 3 plaques (`16×16` each): **`K`** / **`S`** / **`E`** or horizontal **`KOSHIEN`** across the scoreboard header. |
| `CDROM/BS/D_03_08.BS` | `320×160` | Confirmed | `きらめき高校` *(team march placard)* | Kirameki High | March placard (`22×64`): vertical **`KIRAMEKI`** or **`K.H.S.`**. |
| `CDROM/BS/D_03_09.BS` | `320×160` | Confirmed | `衛生室` *(door nameplate)* | Infirmary / Nurse's Office | Door plate (`28×10`): **`NURSE`** or **`CLINIC`**. |
| `CDROM/BS/D_11_11.BS` | `320×160` | Confirmed | `国際吹奏楽コンクール優勝公演` *(stage banner)* | International Brass Band Competition Winners' Concert | Stage banner (`168×14`): **`INT'L BRASS BAND WINNERS CONCERT`**. |
| `CDROM/BS/D_15_03.BS` | `320×160` | Confirmed | `スピンコースター` / `ビート` *(coaster arch sign)* | Spin Coaster Beat | Arch sign (`84×16`): **`SPIN COASTER`**. |
| `CDROM/BS/D_19_02.BS` | `320×160` | Confirmed | `市立中央美術館` *(stone monument)* | Central City Art Museum | Stone monument (`24×68`): stacked **`CITY`** / **`ART`** / **`MUSEUM`** or vertical **`ART MUSEUM`**. |
| `CDROM/BS/D_29_09.BS` | `320×160` | Confirmed | `←入口` *(hanging sign)*, `ラブリーショップ MAIKO` *(rink dasher board)* | `← Entrance`, `Lovely Shop MAIKO` | Hanging sign (`44×16`): **`← ENTER`** or **`← ENTRY`**. Dasher board (`64×12`): **`LOVELY SHOP MAIKO`**. |
| `CDROM/BS/I_17_01.BS` | `320×160` | Confirmed | `チケット` *(booth sign next to `GREEN GARDEN`)* | Tickets | Booth sign (`36×12`): **`TICKETS`**. |
| `CDROM/BS/I_28_02.BS` | `320×160` | Confirmed | `レンタルシューズ` *(neon counter sign)* | Shoe Rental / Rental Shoes | Neon sign (`80×14`): **`SHOE RENTAL`**. |
| `CDROM/BS/C_01_28.BS` | `320×88` | Confirmed | `期末テスト` *(chalkboard)* + `月 日 ( )` | Final Exams + Month/Day | Chalkboard (`64×18`): **`FINAL EXAMS`** in white chalk pixel lettering. |
| `CDROM/BS/C_01_30.BS` | `320×88` | Confirmed | `文` *(yellow traffic diamond)* + `神宮前` *(utility pole)* | School Zone symbol + Jingumae | Keep authentic Japanese street sign or replace `神宮前` (`12×32`) with vertical **`JINGUMAE`**. |
| `CDROM/BS/I_11_15.BS` | `320×160` | Suspected | `美術・芸術` *(library aisle sign)* | Fine Arts | Hanging sign (`36×10`): **`FINE ARTS`** or **`ART`**. |


## 11. character bust-up sprites(`CDROM/BUSTUP/*.BIN`) & sd sprites (`CDROM/*_SD/*.BIN`)

| Asset Path | Pixel Size | Status | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|---|
| `CDROM/BUSTUP/HU17.BIN` | `128×160` | Confirmed | `祭` *(on Shiori's summer festival uchiwa fan, `24×24`)* | Festival (`Matsuri`) | Traditional Japanese festival fan kanji (`祭`). **Recommendation**: Keep `祭` as an authentic cultural motif on her yukata fan (just like a Happi coat crest), or replace with **`FEST`** / a firework crest inside the `24×24` fan face. |
| `CDROM/BUSTUP/AS34.BIN`, `HI05.BIN`, `KO15.BIN` | `128×160` | Suspected | *(Shirt triangle logo, cyborg diagram, ski hat band)* | N/A (Pictorial graphics only) | No Japanese text present; no change required. |
| `CDROM/CLUB_SD/*`, `HOLI_SD/*`, `MAIN_SD/*` (18 files) | `64×88` .. `176×120` | Suspected | `...`, sweatdrop, `×`, peace hand, `K` on tennis shirt | N/A (Universal comic symbols / Latin `K`) | No Japanese text present; no change required. |


## 12. opening fmv movie(`CDROM/OPENXA.STR`)

The opening anime FMV (`288×192` MDEC video stream, 829 frames) displays pink gradient pill overlays introducing each heroine and her voice actress (e.g., frames `392`–`407` show `清川 望 / 笹木綾子` top-left and `鏡 魅羅 / 五十嵐麗` bottom-right), as well as opening title/staff credit overlays:

| Frame Sequence | Overlay Region | Original Japanese | English Translation | Suggested Graphic / Abbreviation (Same Size) |
|---|---|---|---|---|
| Cast Introduction Pills (e.g., Frames `392`–`407` and peer heroine shots) | Top-left & Bottom-right pink pills (`~96×40` each) | **Top pill**: Character Name (e.g., `清川 望`, `鏡 魅羅`)<br>**Bottom pill**: Voice Actress Name (e.g., `笹木綾子`, `五十嵐麗`) | **Top pill**: `Kiyokawa Nozomi`, `Kagami Mira` (all 13 heroines in Family-Given order)<br>**Bottom pill**: `CV: Sasaki Ayako`, `CV: Igarashi Rei` | Render two-line white bold sans-serif text with black drop shadow over the pink gradient pill (`96×40`):<br>• Line 1 (`96×18`): **`Kiyokawa Nozomi`** / **`Kagami Mira`**<br>• Line 2 (`96×18`): **`CV: Sasaki Ayako`** / **`CV: Igarashi Rei`** (or soft-subtitled at the bottom of the frame to avoid MDEC macroblock re-encoding artifacts). |
