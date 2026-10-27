extends RefCounted
## T-29032. D_800B5BC8 is a source u8, advanced by hud_screen_frame_draw.
## The source handler draws before GameSession increments frame_count. Rendering
## reuses the resulting primitive state and never advances this counter.
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")
const Calendar = preload("res://rules/calendar.gd")
const KEY := "hud_season_fade"

static func draw_step(state: State, result: Session.FrameResult) -> String:
	var resources: Dictionary = state.rule_state["resources"]
	if not resources.has(KEY):
		return ""
	if not resources[KEY] is int or resources[KEY] < 0 or resources[KEY] > 255:
		return "native HUD season fade must be an original u8"
	for intent: Dictionary in result.presentation:
		if intent.get("kind") == "native_hud_season_draw":
			return "native HUD season draw already ran for this logical frame"
	var frame: int = -1
	for record: Dictionary in result.trace:
		if record.get("phase") == "mode":
			if frame != -1 or not record.get("state") is Dictionary or not record["state"].get("frame_count") is int:
				return "native HUD season draw lacks one source mode frame"
			frame = record["state"]["frame_count"]
	if frame < 0 or ((frame + 1) & 0xffffffff) != state.frame_count:
		return "native HUD season draw differs from the source pre-increment frame"
	if (frame & 3) == 3 and resources[KEY] >= 0xef:
		resources[KEY] = (resources[KEY] + 1) & 255
	result.presentation.append({"kind": "native_hud_season_draw", "source_frame": frame, "fade": resources[KEY]})
	return ""

static func view(rule: Dictionary) -> Dictionary:
	for key: String in ["year", "month", "day"]:
		if not rule.get(key) is int:
			return {"error": "native season view requires source date integers"}
	if rule["year"] < 95 or rule["year"] > 98 or rule["month"] < 1 or rule["month"] > 12 or rule["day"] < 1 or rule["day"] > Calendar.days_in_month(rule["year"], rule["month"]):
		return {"error": "native season view date is outside the original calendar"}
	if not rule.get("resources") is Dictionary:
		return {"error": "native season view lacks primitive resources"}
	var month: int = rule["month"]
	var season: int = 3 if month in [1, 2, 12] else int((month - 3) / 3)
	var fade: Variant = rule["resources"].get(KEY, 0)
	if not fade is int or fade < 0 or fade > 255:
		return {"error": "native season view fade is not a source u8"}
	# Month advance and CALROM promotion are separate source operations. Before
	# promotion a quarter's first day retains the prior CLUT, including when the
	# previous quarter's u8 has already wrapped to zero.
	if month % 3 == 0 and rule["day"] == 1:
		var calendar: Variant = rule.get("calendar")
		if not calendar is Array or calendar.size() != 2 or not calendar[0] is Dictionary:
			return {"error": "native season view lacks the current source calendar"}
		var days: Variant = calendar[0].get("day_kind")
		if not days is Array or days.is_empty() or not days[0] is int:
			return {"error": "native season view lacks the current month prefix"}
		if days[0] != Calendar.days_in_month(rule["year"], month):
			var previous: int = Calendar.days_in_month(rule["year"], month - 1)
			if days[0] != previous:
				return {"error": "native season view calendar is neither current nor previous month"}
			return {"error": "", "checker_season": (season + 3) & 3, "checker_fade": 255}
	return {"error": "", "checker_season": season, "checker_fade": fade if fade >= 0xef else 255}
