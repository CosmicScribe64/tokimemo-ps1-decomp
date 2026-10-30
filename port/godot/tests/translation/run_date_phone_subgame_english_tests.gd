extends SceneTree
## Phase 4 WP-I: English bind/clear hooks on the date, phone, subgame and
## ending presenters. Each hook passes the triplet test: native frame, English
## frame, cleared frame byte-identical to native. Rejected bindings (overflow,
## tokens, missing glyph, wrong container, multi-row source) keep the native
## frame. The text-layer pack here is a test fixture written to user:// with
## port-authored full-width Latin; it is not a translation and never ships.
## Needs TOKIMEMO_ASSET_CACHE (default res://../../.asset-cache) holding
## port/hud-production-v1, port/fonts/native-font-v2 and port/date-preview-v1.
const Store = preload("res://services/text_layer_store.gd")
const Binding = preload("res://presentation/presenter_text_binding.gd")
const Latin = preload("res://flow/name_entry_latin_page.gd")
const Calendar = preload("res://presentation/calendar_phone_presenter.gd")
const Telephone = preload("res://presentation/telephone_overlay_presenter.gd")
const DateOverlay = preload("res://presentation/date_overlay_presenter.gd")
const Subgame = preload("res://presentation/subgame_overlays_presenter.gd")
const Encounters = preload("res://presentation/encounters_and_ending_presenter.gd")
const DateRules = preload("res://overlays/date_overlay.gd")
const Appointment = preload("res://presentation/date_appointment_presenter.gd")
const HolidayRules = preload("res://rules/holiday_appointment.gd")
const Rng = preload("res://core/native_rng.gd")
const ROW_BREAK_REASON := "multi-row or single-byte source needs the shared text flow"
const UNIT_REASON := "source is not whole code units"
const FIXTURE_CONTAINER := "CDROM/EXEDIR/WPI_FIXTURE.EXN"
const FIXTURE_CONTAINER_SHA := "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
var failures: Array[String] = []
var checks := 0

## Exact-identity image store stand-in: an opaque magenta frame of the
## original size for every HUD part it is told about. Test-only.
class StubImageStore extends RefCounted:
	var sizes: Dictionary = {}
	func replacement(_container_sha: String, _frame_index: int, _variant: Variant, png_sha: String) -> Image:
		if not sizes.has(png_sha):
			return null
		var size: Vector2i = sizes[png_sha]
		var image := Image.create(size.x, size.y, false, Image.FORMAT_RGBA8)
		image.fill(Color(1, 0, 1, 1))
		return image

func check(value: bool, label: String) -> void:
	checks += 1
	if not value: failures.append(label)

func _glyphs(text: String) -> Array:
	return ["g", Latin.encode(text).hex_encode()]

func _physical_id(raw_hex: String, offset: int) -> String:
	return Store.source_id(FIXTURE_CONTAINER, offset, raw_hex.hex_decode())

func _fixture(path: String, physical_hex: String) -> String:
	var raw := Store.terminated(physical_hex.hex_decode())
	var hashing := HashingContext.new(); hashing.start(HashingContext.HASH_SHA256); hashing.update(raw)
	var raw_sha := hashing.finish().hex_encode()
	var pack := {"schema": Store.SCHEMA, "language": "en-fixture",
		"approval": {"approver": "wp-i-test-fixture (not a review)"},
		"font": {"source_path": "SLPM_86.053", "source_sha256": "a".repeat(64), "glyph_bank_sha256": "b".repeat(64)},
		"containers": {FIXTURE_CONTAINER: {"sha256": FIXTURE_CONTAINER_SHA, "size": 4096}},
		"entries": {
			"fixture:phone": {"text": "Phone", "context_status": "fixture", "segments": [_glyphs("Phone")]},
			"fixture:datespot": {"text": "Date spots", "context_status": "fixture", "segments": [_glyphs("Date spots")]},
			"fixture:short": {"text": "Ok", "context_status": "fixture", "segments": [_glyphs("Ok")]},
			"fixture:choice": {"text": "Fish", "context_status": "fixture", "segments": [_glyphs("Fish")]},
			"fixture:together": {"text": "Go home", "context_status": "fixture", "segments": [_glyphs("Go home")]},
			"fixture:overflow": {"text": "overflow", "context_status": "fixture", "segments": [_glyphs("Telephone directory for every single student in school")]},
			"fixture:token": {"text": "Hi", "context_status": "fixture", "segments": [_glyphs("Hi"), ["t", "50", "player_name"]]},
			"fixture:glyphless": {"text": "?", "context_status": "fixture", "segments": [["g", "eaa4"]]},
			"fixture:bound": {"text": "Bound", "context_status": "fixture", "container": FIXTURE_CONTAINER, "segments": [_glyphs("Win")]},
			_physical_id(physical_hex, 0x40): {"text": "We won!", "context_status": "fixture", "container": FIXTURE_CONTAINER,
				"offset": 0x40, "raw_sha256": raw_sha, "raw_length": raw.size(), "segments": [_glyphs("We won!")]},
		}}
	var file := FileAccess.open(path, FileAccess.WRITE)
	file.store_string(JSON.stringify(pack)); file.close()
	return FileAccess.get_sha256(path)

func _same(a: Image, b: Image) -> bool:
	return a != null and b != null and a.get_size() == b.get_size() and a.get_data() == b.get_data()

## Native, English, cleared. `render` returns an Image.
func _triplet(label: String, presenter: RefCounted, render: Callable, layer: RefCounted, bindings: Array, expected_text: int) -> void:
	var native: Image = render.call()
	var bound: Dictionary = presenter.bind_english(layer, bindings)
	check(int(bound.text) == expected_text, label + ": bound rows " + str(bound))
	var english: Image = render.call()
	check(english != null and not _same(native, english), label + ": English frame differs")
	presenter.clear_english()
	check(_same(native, render.call()), label + ": cleared frame byte-identical to native")
	# Rejected binding keeps the native frame.
	var rejected: Array = []
	for row: Dictionary in bindings:
		var copy := row.duplicate(); copy["record_id"] = "fixture:overflow"; rejected.append(copy)
	var outcome: Dictionary = presenter.bind_english(layer, rejected)
	check(int(outcome.text) == 0 and not outcome.fallbacks.is_empty(), label + ": overflow rejected and logged")
	check(_same(native, render.call()), label + ": rejected binding draws native")
	presenter.bind_english(null, bindings)
	check(_same(native, render.call()), label + ": null layer draws native")
	presenter.clear_english()

## First run of a rendered frame that a short record can replace: whole
## double-byte units, at least two of them, and not one of `avoid`.
func _bindable_hex(frame: Dictionary, avoid: Array) -> String:
	for run: Dictionary in frame.get("text_runs", []):
		var hex := String(run.get("hex", ""))
		if hex.length() < 8 or hex.length() % 4 != 0 or hex in avoid:
			continue
		var whole := true
		for index: int in range(0, hex.length(), 4):
			whole = whole and hex.substr(index, 2).hex_to_int() >= 0x81
		if whole:
			return hex
	return ""

## Triplet on one compositor text-run site: binds a run that the frame draws
## through that site to the short fixture record.
func _runs_triplet(label: String, presenter: RefCounted, render: Callable, layer: RefCounted, avoid: Array) -> void:
	var hex := _bindable_hex(render.call(), avoid)
	check(not hex.is_empty(), label + ": frame has a bindable text run")
	if hex.is_empty():
		return
	_triplet(label, presenter, func() -> Image: return render.call().image,
		layer, [{"key": label, "source_hex": hex, "record_id": "fixture:short"}], 1)

## Bind with a stub image store, then re-bind without one: the frame must
## return to the text-only English frame, and clear to native.
func _image_rebind(label: String, presenter: RefCounted, render: Callable, layer: RefCounted, bindings: Array) -> void:
	var store := StubImageStore.new()
	for frame: Dictionary in presenter._manifest["frames"].values():
		for part: Dictionary in frame["parts"]:
			store.sizes[part["png"]["sha256"]] = presenter._source_images[part["png"]["path"]].get_size()
	var native: Image = render.call()
	presenter.bind_english(layer, bindings)
	var text_only: Image = render.call()
	var with_images: Dictionary = presenter.bind_english(layer, bindings, store)
	check(int(with_images.images) > 0, label + ": stub store replaced HUD frames " + str(with_images.images))
	check(not _same(text_only, render.call()), label + ": image binding changes the frame")
	var rebound: Dictionary = presenter.bind_english(layer, bindings)
	check(int(rebound.images) == 0 and _same(text_only, render.call()), label + ": re-bind without a store restores the text-only frame")
	presenter.clear_english()
	check(_same(native, render.call()), label + ": cleared after images is byte-identical to native")

func _initialize() -> void:
	var cache := OS.get_environment("TOKIMEMO_ASSET_CACHE")
	if cache.is_empty(): cache = ProjectSettings.globalize_path("res://../../.asset-cache").simplify_path()
	var hud := cache.path_join("port/hud-production-v1/manifest.json")
	var font_root := cache.path_join("port/fonts/native-font-v2")
	var date_pack := cache.path_join("port/date-preview-v1")
	var work := ProjectSettings.globalize_path("user://wp-i-%d" % Time.get_ticks_usec())
	DirAccess.make_dir_recursive_absolute(work)
	var victory: String = Subgame.STR_HEX["rpg_victory"]
	var pack_path := work.path_join("fixture_text_layer.json")
	var layer := Store.new()
	check(layer.open_pack(pack_path, _fixture(pack_path, victory)), "fixture pack opens: " + layer.error)

	# Binding unit checks on the subgame seam (font only, no disc data).
	var subgame := Subgame.new()
	check(subgame.init_resources(font_root).is_empty(), "subgame font opens")
	var box := Rect2i(40, 40, 240, 60)
	var draw := func() -> Image:
		var image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
		subgame._draw_text_hex_checked(image, victory, Vector2i(58, 58), 0, "rpg_victory_banner", box)
		return image
	var native: Image = draw.call()
	var result: Dictionary = subgame.bind_english(layer, [{"key": "rpg_victory", "source_hex": victory,
		"container": FIXTURE_CONTAINER, "container_sha256": FIXTURE_CONTAINER_SHA, "offset": 0x40}])
	check(int(result.text) == 1, "physical source binding: " + str(result.fallbacks))
	subgame.recorded_glyphs.clear(); subgame.bounding_box_violations.clear()
	var english: Image = draw.call()
	check(not _same(native, english), "subgame English differs")
	check(subgame.recorded_glyphs.size() == Latin.encode("We won!").size() / 2, "one native glyph draw per translated unit")
	check(subgame.bounding_box_violations.is_empty(), "translated glyphs stay in the original box")
	var last: Dictionary = subgame.recorded_glyphs[-1]
	check(last.pos.x + 12 <= 58 + victory.length() / 4 * 13, "translated run ends within the original run")
	subgame.clear_english()
	check(_same(native, draw.call()), "subgame cleared frame byte-identical")
	var negatives := {
		"wrong offset": {"key": "a", "source_hex": victory, "container": FIXTURE_CONTAINER, "container_sha256": FIXTURE_CONTAINER_SHA, "offset": 0x44},
		"wrong container hash": {"key": "b", "source_hex": victory, "container": FIXTURE_CONTAINER, "container_sha256": "d".repeat(64), "offset": 0x40},
		"source bytes differ": {"key": "c", "source_hex": Subgame.STR_HEX["rpg_defeat"], "container": FIXTURE_CONTAINER, "container_sha256": FIXTURE_CONTAINER_SHA, "offset": 0x40},
		"tokens": {"key": "d", "source_hex": victory, "record_id": "fixture:token"},
		"missing glyph": {"key": "e", "source_hex": victory, "record_id": "fixture:glyphless"},
		"overflow": {"key": "f", "source_hex": "8bbe", "record_id": "fixture:phone"},
		"unknown id": {"key": "g", "source_hex": victory, "record_id": "fixture:none"},
		"record container hash": {"key": "h", "source_hex": victory, "record_id": "fixture:bound", "container_sha256": "e".repeat(64)},
		"odd-length source": {"key": "i", "source_hex": "82a08142208d73", "record_id": "fixture:phone"},
		"odd source": {"key": "j", "source_hex": "82a", "record_id": "fixture:phone"},
		"no location": {"key": "k", "source_hex": victory},
		"malformed": {"source_hex": victory},
	}
	for name: String in negatives:
		var outcome: Dictionary = subgame.bind_english(layer, [negatives[name]])
		check(int(outcome.text) == 0 and outcome.fallbacks.size() == 1, "negative rejected: " + name + " " + str(outcome))
		check(_same(native, draw.call()), "negative draws native: " + name)
	# Reasons: a well-formed source with a 0x20 row break in a lead position is
	# refused by the row-break guard (the WP-A fallback), not the length check.
	var row_break: Dictionary = subgame.bind_english(layer, [{"key": "tel_row", "source_hex": "82a020208d73", "record_id": "fixture:phone"}])
	check(int(row_break.text) == 0 and row_break.fallbacks.size() == 1 and row_break.fallbacks[0].reason == ROW_BREAK_REASON,
		"0x20 row break keeps the original via the row-break guard: " + str(row_break.fallbacks))
	var single_byte: Dictionary = subgame.bind_english(layer, [{"key": "ascii", "source_hex": "41428260", "record_id": "fixture:phone"}])
	check(single_byte.fallbacks.size() == 1 and single_byte.fallbacks[0].reason == ROW_BREAK_REASON, "single-byte lead refused by the same guard")
	var odd: Dictionary = subgame.bind_english(layer, [negatives["odd-length source"]])
	check(odd.fallbacks.size() == 1 and odd.fallbacks[0].reason == UNIT_REASON, "odd-length source refused by the length check")
	check(_same(native, draw.call()), "row-break rejection draws native")
	var images: Dictionary = subgame.bind_english(layer, [], Store.new())
	check(int(images.images) == 0 and images.fallbacks.size() == 1, "subgame image store reported as an open gap")
	subgame.clear_english()
	var binding := Binding.new()
	check(binding.bind(layer, subgame._font, [{"key": "x", "source_hex": victory, "record_id": "fixture:choice"},
		{"key": "y", "source_hex": victory, "record_id": "fixture:phone"}], 13) == 1, "duplicate source bound once")
	var runs: Array[Dictionary] = [{"hex": victory, "position": [0, 0], "attribute": 0}, {"hex": "8bbe", "position": [0, 16], "attribute": 0}]
	var applied := binding.apply_runs(runs)
	check(applied[0].has("glyph_x") and not applied[1].has("glyph_x") and runs[0].hex == victory, "apply_runs replaces only bound runs, copying")
	check(binding.apply_runs(applied)[0].hex == applied[0].hex, "translated runs pass through unchanged")
	binding.clear()
	check(binding.apply_runs(runs) == runs, "cleared binding returns the original runs")

	# Calendar/phone book (HUD-derived).
	var calendar := Calendar.new()
	# The presenters pin an older HUD build than the cache holds; the test opens
	# the cached HUD with its own hash as the caller capability.
	var hud_sha := FileAccess.get_sha256(hud)
	check(calendar.init_resources(font_root, "").is_empty() and calendar.open_manifest(hud, hud_sha, font_root), "calendar resources: " + calendar.error)
	var dialed: Array[int] = []
	_triplet("calendar phone book", calendar, func() -> Image: return calendar.render_holiday_phone_screen(0, dialed, 0, "福原"),
		layer, [{"key": "title_phone", "source_hex": Calendar.STR_HEX["title_phone"], "record_id": "fixture:phone"}], 1)

	# TEL date-place menu.
	var telephone := Telephone.new()
	check(telephone.open_date_preview(hud, font_root, date_pack, hud_sha), "telephone resources: " + telephone.error)
	_triplet("telephone date places", telephone, func() -> Image: return telephone.render_date_place_menu_screen(0, 0),
		layer, [{"key": "title_datespot", "source_hex": Telephone.STR_HEX["title_datespot"], "record_id": "fixture:datespot"}], 1)

	# DATE three-choice prompt (compositor text runs).
	var date := DateOverlay.new()
	check(date.open_date_preview(hud, font_root, date_pack, hud_sha), "date resources: " + date.error)
	var state := DateRules.create_default_state({"year": 1995, "month": 7, "day": 23, "weekday": 6, "appointment_place_0": DateRules.SPOT_AQUARIUM})
	DateRules.date_init(state)
	DateRules.date_arrival_and_looks_check(state)
	_triplet("date three-choice", date, func() -> Image: return date.render_overlay_three_choice_prompt(state, 0).image,
		layer, [{"key": "choice0", "source_hex": String(DateRules.AQUARIUM_CHOICES_HEX[0]), "record_id": "fixture:choice"}], 1)

	# GEKO walk-home prompt (ending/encounters presenter).
	var encounters := Encounters.new()
	encounters.load_manifest(hud, font_root)
	check(encounters.open_manifest(hud, hud_sha, font_root), "encounters resources: " + encounters.error)
	_triplet("geko invitation", encounters, func() -> Image: return encounters.render_walk_home_invitation_frame(0),
		layer, [{"key": "choice_accept", "source_hex": Encounters.CHOICE_ACCEPT_HEX, "record_id": "fixture:together"}], 1)

	# Image path: a re-bind without a store drops the earlier images.
	_image_rebind("calendar images", calendar, func() -> Image: return calendar.render_holiday_phone_screen(0, dialed, 0, "福原"),
		layer, [{"key": "title_phone", "source_hex": Calendar.STR_HEX["title_phone"], "record_id": "fixture:phone"}])
	_image_rebind("geko images", encounters, func() -> Image: return encounters.render_walk_home_invitation_frame(0),
		layer, [{"key": "choice_accept", "source_hex": Encounters.CHOICE_ACCEPT_HEX, "record_id": "fixture:together"}])

	# DATE/DATE2 overlay renders: every _set_runs site gets its own triplet.
	var overlay_state := func() -> Dictionary:
		var st := DateRules.create_default_state({"year": 1995, "month": 7, "day": 23, "weekday": 6, "appointment_place_0": DateRules.SPOT_AQUARIUM})
		DateRules.date_init(st)
		DateRules.date_arrival_and_looks_check(st)
		return st
	var spot_state := func() -> Dictionary:
		var st: Dictionary = overlay_state.call()
		DateRules.setup_date_spot(st, DateRules.SPOT_AQUARIUM)
		return st
	var overlay_avoid: Array = [String(DateRules.SUB_PLACE_NAMES_HEX["indoor_hall"]), String(DateRules.SUB_PLACE_NAMES_HEX["karaoke_box"])]
	for hex: Variant in DateRules.DATE_PLACE_NAMES_HEX:
		overlay_avoid.append(String(hex))
	_runs_triplet("date arrival greeting", date, func() -> Dictionary: return date.render_date_arrival_greeting(overlay_state.call()), layer, overlay_avoid)
	_runs_triplet("date spot reaction", date, func() -> Dictionary: return date.render_date_spot_reaction(spot_state.call(), 0), layer, overlay_avoid)
	_runs_triplet("date karaoke", date, func() -> Dictionary: return date.render_amusement_or_karaoke_subscene(overlay_state.call(), 1, "karaoke_date2"), layer, overlay_avoid)
	_runs_triplet("date coaster", date, func() -> Dictionary: return date.render_amusement_or_karaoke_subscene(overlay_state.call(), 0, "amusement_coaster"), layer, overlay_avoid)

	# Holiday/date appointment presenter: one triplet per render function.
	var appointment := Appointment.new()
	check(appointment.open_date_preview(hud, font_root, date_pack, hud_sha), "appointment resources: " + appointment.error)
	var holiday := HolidayRules.HolidayState.new()
	holiday.year = 95; holiday.month = 4; holiday.day = 9
	HolidayRules.holiday_icons_set_available(holiday)
	var words := [HolidayRules.pack_appointment_word({"place": 0, "month": 4, "day": 9, "flag": 1, "girl": 0}),
		HolidayRules.pack_appointment_word({"place": 3, "month": 4, "day": 9, "flag": 0, "girl": 4}), 0, 0, 0, 0, 0, 0]
	var notice := HolidayRules.holiday_open_init_appointment_notice(holiday, words)
	var step := HolidayRules.StepResult.new()
	HolidayRules.appointment_start(holiday, words, step)
	var rng := Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE, 1)
	var menu := HolidayRules.appointment_place_menu_build(holiday, words, rng, step)
	holiday.chosen_place = 0
	var confirm := HolidayRules.build_appointment_confirm_Presentation(holiday, words)
	var launch := HolidayRules.appointment_launch(holiday, words, step, 9)
	var place_avoid: Array = []
	for place: int in 16:
		place_avoid.append(String(HolidayRules.place_name(place).get("bytes_hex", "")))
	_runs_triplet("appointment notice", appointment, func() -> Dictionary: return appointment.render_holiday_appointment_notice(holiday, notice), layer, place_avoid)
	_runs_triplet("appointment place choice", appointment, func() -> Dictionary: return appointment.render_appointment_place_choice(holiday, menu, 0), layer, place_avoid)
	_runs_triplet("appointment place confirm", appointment, func() -> Dictionary: return appointment.render_appointment_place_confirm(holiday, confirm), layer, place_avoid)
	_runs_triplet("appointment arrival scene", appointment, func() -> Dictionary: return appointment.render_date_arrival_scene(holiday, launch), layer, place_avoid)
	_runs_triplet("appointment three-choice", appointment, func() -> Dictionary: return appointment.render_date_three_choice_prompt(holiday, {}, 0), layer, place_avoid)
	_runs_triplet("appointment choice response", appointment, func() -> Dictionary: return appointment.render_date_choice_response(holiday, {}), layer, place_avoid)
	# _draw_place_title_banner: bind the place name the base scene banners.
	var banner_hex := String(HolidayRules.place_name(0)["bytes_hex"])
	_triplet("appointment title banner", appointment, func() -> Image: return appointment.render_date_choice_response(holiday, {}).image,
		layer, [{"key": "banner", "source_hex": banner_hex, "record_id": "fixture:short"}], 1)
	_image_rebind("appointment images", appointment, func() -> Image: return appointment.render_holiday_appointment_notice(holiday, notice).image,
		layer, [{"key": "speaker", "source_hex": "93a18de8", "record_id": "fixture:short"}])

	print("WP-I English hook checks: ", checks, " failures: ", failures)
	quit(0 if failures.is_empty() else 1)
