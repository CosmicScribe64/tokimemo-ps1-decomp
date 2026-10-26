# Control token reference

This document describes all 28 double bracket control tokens used across the Tokimeki Memorial English translation files.

Each token marks an engine control code, line break, choice delimiter, or runtime string substitution. When translating dialogue and system lines, keep these tokens intact. You may reposition them within the sentence so the English reads naturally, but do not delete, rename, or invent markers.

## Dialogue formatting

These tokens control line breaks, row layout, and spacing in dialogue textboxes and system arrays.

### row
- Marker syntax: `[[T...:row]]`
- Purpose: Inserts a line break inside a dialogue box. Advances rendering to the next row.
- Example:
  - Japanese source: `こうして、[[T001:row]]文化祭は、終了した。`
  - English target: `And so, the[[T001:row]]festival was over.`
  - Line ID: `talk:CDROM/BUNKA/AS_S6.BIN:main:g0000:l0000`
- Contributor guidance: Keep this token to fit text within the dialogue box rows.

### row_separator
- Marker syntax: `[[T...:row_separator]]`
- Purpose: Delimits rows inside system text arrays and multi-row menu displays.
- Example:
  - Japanese source: `第一日目の試験科目は[[T001:row_separator]]語学だ。）`
  - English target: `Day one's exam is[[T001:row_separator]]language.)`
  - Line ID: `c:ETC:8014716c:2`
- Contributor guidance: Preserves row boundaries in engine menu arrays.

### line_feed
- Marker syntax: `[[T...:line_feed]]`
- Purpose: Represents an ASCII newline character in C formatted strings and console logs.
- Example:
  - Japanese source: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - English target: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - Line ID: `c:DATE:8013c190:1`
- Contributor guidance: Keep at the end of formatted strings.

### carriage_return
- Marker syntax: `[[T...:carriage_return]]`
- Purpose: Represents an ASCII carriage return character in serial or debug output strings.
- Example:
  - Japanese source: `[[T001:carriage_return]]`
  - English target: `[[T001:carriage_return]]`
  - Line ID: `c:main:80048cf8:3`
- Contributor guidance: Do not remove.

### tab
- Marker syntax: `[[T...:tab]]`
- Purpose: Represents an ASCII horizontal tab character used for column alignment in system menus.
- Example:
  - Japanese source: `今日の成績 [[T001:format_directive]][[T002:tab]]`
  - English target: `Today's Score [[T001:format_directive]][[T002:tab]]`
  - Line ID: `c:ETC:80146d4c:2`
- Contributor guidance: Preserves spacing columns in system menus.

## Branching and choices

These tokens delimit selectable options in choice menus and separate branching reaction variants.

### choice_option
- Marker syntax: `[[T...:choice_option]]`
- Purpose: Delimits selectable options in branching dialogue choice menus.
- Example:
  - Japanese source: `うん、いいよ。[[T001:choice_option]]いや、ちょっとね…。`
  - English target: `Sure, sounds good.[[T001:choice_option]]Well, it's just...`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0000:l0003`
- Contributor guidance: The engine splits options on this token. Translate each option on its respective side of the marker.

### conditional_choice_zero
- Marker syntax: `[[T...:conditional_choice_zero]]`
- Purpose: Marks choice branch zero execution flow.
- Example:
  - Japanese source: `浴衣がすごく似合うね。[[T001:row]]可愛いよ。[[T002:conditional_choice_zero]]`
  - English target: `That yukata really suits you.[[T001:row]]You look cute.[[T002:conditional_choice_zero]]`
  - Line ID: `talk:CDROM/SERIFU/DATE_FU.BIN:section-18:g0001:l0000`
- Contributor guidance: Keep at the end of the choice phrase.

### reply_variant
- Marker syntax: `[[T...:reply_variant]]`
- Purpose: Delimits alternative response variants in multi-path girl reactions.
- Example:
  - Japanese source: `じゃあ、ソッコーで行こうよ。[[T001:row]]ほら、早く、早く。[[T002:reply_variant]]ええーっ、だめなの？[[T003:row]]超楽しみにしてたのに…。[[T004:row]]じゃあ、いいわよ。`
  - English target: `Then let's go, pronto![[T001:row]]C'mon, hurry, hurry![[T002:reply_variant]]Whaaat, you can't?[[T003:row]]I was super excited...[[T004:row]]Fine, whatever.`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0001:l0000`
- Contributor guidance: Separate distinct reaction variants using this token.

### reply_skip_marker
- Marker syntax: `[[T...:reply_skip_marker]]`
- Purpose: Directs the script parser to jump past alternative reaction branches.
- Example:
  - Japanese source: `あ、ありがとう。[[T001:row]]嬉しいな。[[T002:reply_variant]][[T003:reply_skip_marker]]`
  - English target: `Th-thank you.[[T001:row]]I'm so happy.[[T002:reply_variant]][[T003:reply_skip_marker]]`
  - Line ID: `talk:CDROM/SERIFU/DATE_FU.BIN:section-18:g0002:l0000`
- Contributor guidance: Place immediately after the reply branch.

## Name substitutions

These tokens substitute character and player names dynamically into spoken dialogue.

### girl_name
- Marker syntax: `[[T...:girl_name]]`
- Purpose: Substituted at runtime with the active heroine's given name or nickname.
- Example:
  - Japanese source: `あっ、[[T001:girl_name]]。[[T002:row]]来てくれたんだ。`
  - English target: `Oh, [[T001:girl_name]]![[T002:row]]You came!`
  - Line ID: `talk:CDROM/BUNKA/AS_S6.BIN:main:g0003:l0000`
- Contributor guidance: Place where the girl's name naturally appears in the English sentence.

### player_form
- Marker syntax: `[[T...:player_form]]`
- Purpose: Substituted at runtime with the player's name using the heroine's specific address form or honorific.
- Example:
  - Japanese source: `[[T001:player_form]]。[[T002:row]]何、どうしたの？`
  - English target: `[[T001:player_form]],[[T002:row]]what's up?`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0003:l0000`
- Contributor guidance: Position where the player's name is spoken.

### player_family
- Marker syntax: `[[T...:player_family]]`
- Purpose: Substituted at runtime with the player's last name (family name).
- Example:
  - Japanese source: `[[T001:inline_label]]先生おっ、[[T002:player_family]]君[[T003:row]]待ってたよ。`
  - English target: `[[T001:inline_label]]Teacher: Ah, [[T002:player_family]]-kun![[T003:row]]I've been expecting you.`
  - Line ID: `talk:CDROM/SERIFU/EVENT_AS.BIN:initializer-00:g0002:l0001`
- Contributor guidance: Use where a teacher or acquaintance calls the protagonist by last name.

### player_given
- Marker syntax: `[[T...:player_given]]`
- Purpose: Substituted at runtime with the player's first name (given name).
- Example:
  - Japanese source: `よお、[[T001:player_given]]。`
  - English target: `Yo, [[T001:player_given]].`
  - Line ID: `talk:CDROM/BUNKA/BUN_PC.BIN:bun-pc:g0006:l0000`
- Contributor guidance: Use where close friends address the protagonist by first name.

### player_full
- Marker syntax: `[[T...:player_full]]`
- Purpose: Substituted at runtime with the player's full name (family name followed by given name).
- Example:
  - Japanese source: `あっ、俺、[[T001:player_full]]。[[T002:row]]こちらこそ、よろしく。[[T003:row]]（エセ外人みたいな娘だな。）`
  - English target: `Oh, I'm [[T001:player_full]][[T002:row]]Nice to meet you.[[T003:row]](Talks like a phony foreigner.)`
  - Line ID: `talk:CDROM/SERIFU/GASSHUKU.BIN:table-1:g0004:l0003`
- Contributor guidance: Use in self-introductions.

### yoshio_name
- Marker syntax: `[[T...:yoshio_name]]`
- Purpose: Substituted at runtime with Yoshio Saotome's name.
- Example:
  - Japanese source: `今日は、自由行動の日だ。[[T001:row]][[T002:yoshio_name]]と、一緒に[[T003:row]]行く約束なんだけど…。`
  - English target: `Today's a free day.[[T001:row]][[T002:yoshio_name]] and I[[T003:row]]planned to go out, but...`
  - Line ID: `talk:CDROM/SERIFU/SHUGAKU.BIN:table-12:g0010:l0000`
- Contributor guidance: Position where Yoshio is referenced.

## Contextual substitutions

These tokens insert contextual items, dates, fortunes, and school trip events.

### context_m
- Marker syntax: `[[T...:context_m]]`
- Purpose: Substituted with contextual school subjects, club names, or exam types.
- Example:
  - Japanese source: `[[T001:context_m]]は残念ながら[[T002:row]]不合格だったが、`
  - English target: `[[T001:context_m]] exam[[T002:row]]sadly didn't work out, but`
  - Line ID: `talk:CDROM/SERIFU/EPILOG.BIN:table-12:g0000:l0027`
- Contributor guidance: Treat as a noun phrase in the sentence.

### context_a
- Marker syntax: `[[T...:context_a]]`
- Purpose: Substituted with gift items, date items, or event props.
- Example:
  - Japanese source: `よし、[[T001:row]][[T002:context_a]]を、[[T003:row]]プレゼントしよう。`
  - English target: `I'll give her[[T001:row]][[T002:context_a]][[T003:row]]for her birthday.`
  - Line ID: `talk:CDROM/BUNKA/BUN_FU.BIN:main:g0006:l0000`
- Contributor guidance: Treat as an item noun.

### context_o
- Marker syntax: `[[T...:context_o]]`
- Purpose: Substituted with New Year shrine fortune results (omikuji outcomes such as Great Blessing or Modest Blessing).
- Example:
  - Japanese source: `ほら、見て。[[T001:row]][[T002:context_o]]だったよ。`
  - English target: `Look![[T001:row]]I got [[T002:context_o]]!`
  - Line ID: `talk:CDROM/SERIFU/SHOGATU.BIN:table-3:g0031:l0000`
- Contributor guidance: Treat as a fortune grade noun.

### context_s
- Marker syntax: `[[T...:context_s]]`
- Purpose: Substituted with sightseeing spots or souvenir names during the Kyoto school trip.
- Example:
  - Japanese source: `そうだ、思い出したぞ。[[T001:row]][[T002:context_s]]だ。`
  - English target: `Right, I remember.[[T001:row]]It's [[T002:context_s]]!`
  - Line ID: `talk:CDROM/SERIFU/SHUGAKU.BIN:table-11:g0001:l0000`
- Contributor guidance: Treat as a place or souvenir noun.

### context_w
- Marker syntax: `[[T...:context_w]]`
- Purpose: Substituted with shrine paper fortunes or wish items.
- Example:
  - Japanese source: `それじゃあ、[[T001:row]]俺は[[T002:context_w]]だよ。`
  - English target: `Well,[[T001:row]]mine was: [[T002:context_w]]`
  - Line ID: `talk:CDROM/SERIFU/SHOGATU.BIN:table-4:g0025:l0001`
- Contributor guidance: Treat as a wish or fortune noun.

## System directives

These tokens handle system-level format specifiers and speaker prefixes.

### format_directive
- Marker syntax: `[[T...:format_directive]]`
- Purpose: Represents a C printf format directive (`%s`, `%d`, etc.) inside system source strings.
- Example:
  - Japanese source: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - English target: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - Line ID: `c:DATE:8013c190:1`
- Contributor guidance: Match each directive to its respective variable.

### inline_label
- Marker syntax: `[[T...:inline_label]]`
- Purpose: Prefixes an inline speaker badge or narrator prompt before dialogue text.
- Example:
  - Japanese source: `[[T001:inline_label]]部員君、静かにしてくれ。[[T002:row]]デッサンの邪魔だよ。`
  - English target: `[[T001:inline_label]]Member: Keep it down.[[T002:row]]I'm trying to sketch.`
  - Line ID: `talk:CDROM/BUNKA/KG_S6W.BIN:main:g0087:l0001`
- Contributor guidance: Keep at the start of the line before the speaker label.

### terminal_byte_before_nul
- Marker syntax: `[[T...:terminal_byte_before_nul]]`
- Purpose: Preserves a non-NUL terminal control byte at the end of specific system strings.
- Example:
  - Japanese source: `『[[T001:terminal_byte_before_nul]]`
  - English target: `"[[T001:terminal_byte_before_nul]]`
  - Line ID: `c:ENDING:80132420:1`
- Contributor guidance: Keep at the end of the text.

## Storage and engine markers

These tokens preserve raw hardware control bytes and unmapped sequences.

### unknown_renderer_bytes
- Marker syntax: `[[T...:unknown_renderer_bytes]]`
- Purpose: Preserves raw bytes in overlay tables where font encoding or hardware shift units are unmapped.
- Example:
  - Japanese source: `クラシックのＣＤ (`[[T001:unknown_renderer_bytes]]`)`
  - English target: `Classical CD[[T001:unknown_renderer_bytes]]`
  - Line ID: `overlay-table:CDROM/EXEDIR/BUNKAKEN.EXN:name_grid:00`
- Contributor guidance: Keep the token at the end of the translated English string so the original byte container is preserved.

### unknown
- Marker syntax: `[[T...:unknown]]`
- Purpose: Marks an unmapped control sequence currently under reverse-engineering.
- Example:
  - Japanese source: `私、シューティング[[T001:row]]苦手っていうかぁ…、[[T002:row]]ね、[[T003:unknown]]竄[[T004:unknown]][[T005:unknown]][[T006:unknown]][[T007:unknown]][[T008:unknown]][[T009:unknown]]`
  - English target: `I'm not really good[[T001:row]]at shooters...[[T002:row]]Right, [[T003:unknown]]![[T004:unknown]][[T005:unknown]][[T006:unknown]][[T007:unknown]][[T008:unknown]][[T009:unknown]]`
  - Line ID: `talk:CDROM/BUNKA/AS_S6W.BIN:main:g0078:l0000`
- Contributor guidance: Keep all sequential unknown markers in order.

### storage_byte_1b
- Marker syntax: `[[T...:storage_byte_1b]]`
- Purpose: Preserves a raw control byte with value 0x1B in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/BUNKA_SD.EXN:0000a1bc:3a898eecdae1`
- Contributor guidance: Do not remove.

### storage_byte_1a
- Marker syntax: `[[T...:storage_byte_1a]]`
- Purpose: Preserves a raw control byte with value 0x1A in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/BUNKA_SD.EXN:0000a2bc:fdbb66f97af0`
- Contributor guidance: Do not remove.

### storage_byte_0c
- Marker syntax: `[[T...:storage_byte_0c]]`
- Purpose: Preserves a raw control byte with value 0x0C in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/TT.EXN:00024d40:bf60e4349cac`
- Contributor guidance: Do not remove.
