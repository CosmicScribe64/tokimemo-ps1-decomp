extends RefCounted
## Original hud_day_bgm_load selector; this metadata grants no runtime capability.
const AUTHORITY := "955648930342f82b2e0d8a90d23b6fcec9ee47dfb109471ef35bcf9baa608bd1"
const VACATIONS := [[0, 0, 0], [0xd7d1, 0xd7af, 0xd7a4], [0xd73f, 0xd71e, 0xd717], [0xd780, 0xd752, 0xd743]]
const SEASONS := [[0xd3d5, 0xd387, 0xd37d], [0xd4e7, 0xd49e, 0xd488], [0xc70b, 0xc6c3, 0xc6b6], [0xd8cb, 0xd87e, 0xd874]]

static func build(data: Dictionary) -> Dictionary:
	for key: String in ["year", "month", "day", "vacation"]:
		if not data.get(key) is int:
			return {}
	if data["year"] < 0 or data["year"] > 255 or data["month"] < 1 or data["month"] > 12 \
		or data["day"] < 1 or data["day"] > 31 or data["vacation"] < 0 or data["vacation"] > 3:
		return {}
	var season: int = 0 if data["month"] in [3, 4, 5] else (1 if data["month"] in [6, 7, 8] else (2 if data["month"] in [9, 10, 11] else 3))
	var vacation: int = 1 if [data["year"], data["month"], data["day"]] == [96, 9, 1] else data["vacation"]
	var selected: Array = SEASONS[season] if vacation == 0 else VACATIONS[vacation]
	return {"source_profile_sha256": AUTHORITY, "selector": "hud_day_bgm_load",
		"date": [data["year"], data["month"], data["day"]], "vacation": vacation, "season": season,
		"commands": [0xc1, 5, 0x71], "binding": {"bank": 0, "se_header": 0xbf98, "se_body": 0xbf79,
			"music_header": selected[0], "music_body": selected[1], "sequence": selected[2]},
		"sequence_id": 0, "start_commands": [0x22, 0x21, 0x2f, 0x200]}
