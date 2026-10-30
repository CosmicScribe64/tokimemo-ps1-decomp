extends SceneTree
## T-30002 WP9: accessibility, pacing and read-history services used by the
## dialogue textbox and VN presenters. At the v2 defaults every helper is the
## identity and native frames stay byte-identical; with options on, only
## presentation pixels and hold timing change and the native trace is equal.
## Synthetic line fixture; no game data.
const Access = preload("res://services/accessibility_service.gd")
const Pacing = preload("res://services/presentation_pacing.gd")
const History = preload("res://services/read_history_service.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
const Presenter = preload("res://presentation/dialogue_textbox_presenter.gd")
const Vn = preload("res://presentation/vn_scene_presenter.gd")
const RECORD := "talk:CDROM/X.BIN:main:g0000:l0003"
const SOURCE_SHA := "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
var failures: Array[String] = []

func check(condition: bool, label: String = "unlabelled check") -> void:
	if not condition: failures.append(label)

## Minimal native reveal: one code unit per native tick, a seeded RNG draw per
## tick and a recorded input list consumed per native tick (not per frame).
class NativeLine extends RefCounted:
	var pos := 0
	var total := 18
	var tick := 0
	var rng := RandomNumberGenerator.new()
	var state := 0
	var trace: Array[String] = []
	func _init() -> void:
		rng.seed = 0x5EED
	func advance(inputs: Array) -> void:
		var held: int = inputs[tick % inputs.size()]
		pos = mini(total, pos + 1)
		state = hash([state, held, rng.randi(), pos])
		tick += 1
		trace.append("%d:%d:%d" % [tick, pos, state])
	func view(identity: String) -> Dictionary:
		return {"identity": identity, "pos": pos, "total": total, "speed": 1,
			"native_complete": pos == total, "voice_complete": true}

## Drives frames: a held frame does not tick the native line.
func _drive(settings: Dictionary, frames: int) -> Dictionary:
	var pacing := Pacing.new(); pacing.configure(settings)
	var line := NativeLine.new()
	var identity := Access.line_identity(RECORD, SOURCE_SHA)
	var inputs := [0, 0x20, 0, 0, 0x4000]
	var holds := 0
	var visible: Array[int] = []
	for frame: int in frames:
		var view := line.view(identity)
		var frozen := JSON.stringify(view)
		var hold := pacing.step_line(view, 1.0 / 60.0)
		check(JSON.stringify(view) == frozen, "pacing never writes the native view")
		visible.append(pacing.visible_count)
		if hold: holds += 1
		elif line.pos < line.total: line.advance(inputs)
	return {"trace": line.trace, "holds": holds, "visible": visible, "pacing": pacing}

func _initialize() -> void:
	var defaults: Dictionary = Preferences.defaults().accessibility
	var access := Access.new(); access.configure(defaults)

	# Identity at defaults.
	for native: int in range(-1, 40):
		check(access.visible_glyphs(native, 32) == (32 if native < 0 else mini(32, native)), "default reveal follows native (%d)" % native)
	for alpha: int in [0, 1, 64, 128, 255]:
		check(access.flash_alpha(alpha) == alpha, "default flash alpha unchanged")
	for offset: Vector2i in [Vector2i.ZERO, Vector2i(3, -2), Vector2i(-8, 8)]:
		check(access.shake_offset(offset) == offset, "default shake unchanged")
	check(access.font_scale() == 1.0 and access.backdrop() == 1.0 and access.text_box_mode() == "native", "default painter values are native")
	check(not access.auto_advance_enabled(), "auto-advance off by default")

	# Native frames: the textbox renders byte-identically when configured with
	# the defaults; a backdrop change touches only the text box.
	var seed_image := Image.create(320, 240, false, Image.FORMAT_RGBA8)
	for y: int in 240:
		for x: int in 320:
			seed_image.set_pixel(x, y, Color8((x * 7) & 255, (y * 5) & 255, (x ^ y) & 255, 255))
	var baseline := seed_image.duplicate(); Presenter.new().render_textbox_background(baseline)
	var configured := seed_image.duplicate()
	var presenter := Presenter.new(); presenter.configure_accessibility({"backdrop": access.backdrop()}); presenter.render_textbox_background(configured)
	check(configured.get_data() == baseline.get_data(), "default accessibility keeps the native textbox frame byte-identical")
	var dimmed := seed_image.duplicate()
	var opted := Access.new(); var opted_settings := defaults.duplicate(); opted_settings.backdrop = 0.5; opted.configure(opted_settings)
	var dim_presenter := Presenter.new(); dim_presenter.configure_accessibility({"backdrop": opted.backdrop()}); dim_presenter.render_textbox_background(dimmed)
	var outside_equal := true
	var inside_changed := false
	for y: int in 240:
		for x: int in 320:
			var same: bool = dimmed.get_pixel(x, y) == baseline.get_pixel(x, y)
			if Presenter.TEXTBOX_RECT.has_point(Vector2i(x, y)): inside_changed = inside_changed or not same
			elif not same: outside_equal = false
	check(outside_equal and inside_changed, "backdrop changes only text-box pixels")

	# Pacing at defaults: no hold, visible == native pos, identical trace.
	var plain := _drive(defaults, 60)
	check(plain.holds == 0, "default pacing never holds")
	check(plain.trace.size() == 18, "native line completes")
	for index: int in plain.visible.size():
		check(plain.visible[index] == mini(index, 18), "default visible count follows native pos")
	check(not plain.pacing.take_auto_advance(), "no auto-advance at defaults")

	# Options on: holds and reveal change, the native trace does not.
	for speed: String in ["slow", "fast", "instant"]:
		var options := defaults.duplicate(); options.text_speed = speed; options.auto_delay = 0.5; options.auto_advance = true
		var run := _drive(options, 400)
		check(run.trace == plain.trace, "%s pacing leaves the native trace unchanged" % speed)
		check(run.holds > 0, "%s pacing holds presentation" % speed)
		check(run.pacing.take_auto_advance() and not run.pacing.take_auto_advance(), "%s auto-advance fires once after the reading delay" % speed)
	var instant := defaults.duplicate(); instant.text_speed = "instant"
	var fast_run := _drive(instant, 3)
	check(fast_run.visible[0] == 18, "instant shows the whole line")
	var waiting := Pacing.new(); var waiting_settings := defaults.duplicate(); waiting_settings.auto_advance = true; waiting.configure(waiting_settings)
	check(not waiting.step_line({"identity": "x", "pos": 4, "total": 4, "native_complete": true, "voice_complete": false}, 0.1), "voice still playing: no hold")
	check(not waiting.take_auto_advance(), "auto-advance waits for the voice")
	var minimum := Access.min_reading_time(4)
	# The text has been complete since the 0.1 s step above (voice playing).
	var waited := 0.1
	var fired_at := -1.0
	while waited < minimum + 1.0 and fired_at < 0.0:
		waiting.step_line({"identity": "x", "pos": 4, "total": 4, "native_complete": true, "voice_complete": true}, 0.05)
		waited += 0.05
		if waiting.take_auto_advance(): fired_at = waited
	check(fired_at >= minimum - 0.0001 and fired_at < minimum + 0.1, "zero delay auto-advance waits the minimum reading time (%.2f at %.2f)" % [fired_at, minimum])

	# auto_delay is only a delay: with auto_advance off nothing ever fires.
	var delayed := defaults.duplicate(); delayed.auto_delay = 2.0; delayed.auto_advance = false
	var manual := Access.new(); manual.configure(delayed)
	check(not manual.auto_advance_enabled(), "a reading delay alone does not enable auto-advance")
	var manual_fired := false
	for frame: int in 600: manual_fired = manual_fired or manual.auto_advance("m", true, true, 0.05)
	var manual_run := _drive(delayed, 600)
	check(not manual_fired and not manual_run.pacing.take_auto_advance(), "auto_advance=false with auto_delay>0 never fires")
	check(manual_run.holds > 0 and manual_run.trace == plain.trace, "the reading delay still holds presentation only")

	# Service-level minimum: text complete without voice, default delay.
	var quick := defaults.duplicate(); quick.auto_advance = true
	var reader := Access.new(); reader.configure(quick)
	check(not reader.auto_advance("q", true, true, 0.016, false, 20), "no auto-advance on the frame the text completes")
	var reader_time := 0.016
	var reader_fired := false
	while not reader_fired and reader_time < 10.0:
		reader_time += 0.016; reader_fired = reader.auto_advance("q", true, true, 0.016, false, 20)
	check(reader_fired and reader_time >= Access.min_reading_time(20) - 0.0001, "service auto-advance respects the minimum reading time")

	# Slow pacing with no extra delay holds until every glyph is shown and
	# never auto-advances early.
	var slow := defaults.duplicate(); slow.text_speed = "slow"; slow.auto_delay = 0.0; slow.auto_advance = true
	var slow_pacing := Pacing.new(); slow_pacing.configure(slow)
	var slow_ok := true
	var slow_holds := 0
	for frame: int in 600:
		var hold := slow_pacing.step_line({"identity": "s", "pos": 18, "total": 18, "speed": 1, "native_complete": true, "voice_complete": true}, 1.0 / 60.0)
		if slow_pacing.visible_count < 18:
			slow_ok = slow_ok and hold and not slow_pacing.take_auto_advance()
			slow_holds += 1
		elif slow_pacing.take_auto_advance(): break
	check(slow_ok and slow_holds > 0, "slow pacing holds while visible_count < total and never auto-advances before the reveal ends")

	# Defaults: a native completion before pos reaches total never holds.
	var early := Pacing.new(); early.configure(defaults)
	check(not early.step_line({"identity": "e", "pos": 3, "total": 5, "native_complete": true}, 0.016) and early.visible_count == 3, "defaults never hold when native completes early")
	var early_fast := Pacing.new(); var fast_settings := defaults.duplicate(); fast_settings.text_speed = "fast"; early_fast.configure(fast_settings)
	check(not early_fast.step_line({"identity": "e", "pos": 1, "total": 5, "native_complete": true}, 0.016) and early_fast.visible_count == 5, "fast pacing shows the whole line once native completes")
	check(not waiting.step_line({"identity": "", "pos": 1, "total": 4}, 0.1) and waiting.identity.is_empty(), "invalid views reset pacing")
	check(not waiting.step_line({"identity": "y", "pos": 1, "total": 0}, 0.1), "empty lines never hold")

	# Reduce flashes and shake are presentation-only clamps.
	var reduced := Access.new(); var reduced_settings := defaults.duplicate(); reduced_settings.reduce_flashes = true; reduced_settings.reduce_shake = true; reduced.configure(reduced_settings)
	check(reduced.flash_alpha(255) == Access.REDUCED_FLASH_ALPHA and reduced.flash_alpha(10) == 10, "reduce_flashes clamps flash alpha")
	check(reduced.shake_offset(Vector2i(4, -3)) == Vector2i.ZERO, "reduce_shake removes the presentation offset")
	var scaled := Access.new(); var scaled_settings := defaults.duplicate(); scaled_settings.font_scale = 1.5; scaled_settings.text_box = "modern"; scaled.configure(scaled_settings)
	check(scaled.font_scale() == 1.5 and scaled.text_box_mode() == "modern", "font scale and text box mode reach consumers")

	# Skip read/all keyed on text-layer identities with persistent history.
	var identity := Access.line_identity(RECORD, SOURCE_SHA)
	check(identity.begins_with("tl:" + RECORD + "@") and identity.length() <= History.MAX_IDENTITY, "text-layer identity")
	check(Access.line_identity(RECORD, "f".repeat(64)) != identity, "identity follows the source hash")
	check(History.text_layer_identity("", SOURCE_SHA).is_empty() and History.text_layer_identity(RECORD, "").is_empty(), "no identity without id and hash")
	var long_id := History.text_layer_identity("talk:" + "x".repeat(300), SOURCE_SHA)
	check(long_id.begins_with("tlh:") and long_id.length() <= History.MAX_IDENTITY, "long ids are hashed")
	var history_path := "user://wp9-history.json"
	DirAccess.remove_absolute(ProjectSettings.globalize_path(history_path))
	var history := History.new(history_path); check(history.load_history())
	var skipper := Access.new(); skipper.configure(defaults); skipper.bind_history(history)
	check(not skipper.may_skip(identity) and not history.may_skip(identity, "read"), "unread lines are not skipped in read mode")
	skipper.mark_read(identity)
	check(skipper.may_skip(identity) and history.may_skip(identity, "read"), "read lines may be skipped")
	var reopened := History.new(history_path); check(reopened.load_history() and reopened.is_read(identity), "read state persists")
	var fresh := Access.new(); fresh.configure(defaults); fresh.bind_history(reopened)
	check(fresh.may_skip(identity) and not fresh.may_skip(Access.line_identity(RECORD, "f".repeat(64))), "skip follows the persisted identity only")
	var all_settings := defaults.duplicate(); all_settings.skip = "all"; fresh.configure(all_settings)
	check(fresh.may_skip("anything") and history.may_skip("anything", "all"), "skip all")
	check(not history.is_read(""), "empty identity is never read")

	# Presenter identity adapters (T-39207 record ids): the same source-proved
	# talk record keys English, pacing and read/skip; anything else gets none.
	var talk_view := {"record_id": RECORD, "source_sha256": SOURCE_SHA, "tokens": []}
	check(Presenter.line_identity(talk_view) == identity, "textbox identity is the text-layer identity of its talk record")
	check(Vn.pacing_view(talk_view, 3, 18, false).identity == identity, "VN identity matches the textbox identity")
	for unproved: Dictionary in [{"record_id": RECORD}, {"source_sha256": SOURCE_SHA}, {"record_id": "dictionary:x", "source_sha256": SOURCE_SHA}, {}]:
		check(Presenter.line_identity(unproved).is_empty(), "no identity without a source-proved talk record")
	var adapted := Presenter.pacing_view(talk_view, 5, 18, false, true, 2)
	check(adapted == {"identity": identity, "pos": 5, "total": 18, "speed": 2, "native_complete": false, "voice_complete": true}, "pacing view carries the native reveal unchanged")
	var adapted_pacing := Pacing.new(); adapted_pacing.configure(defaults)
	check(not adapted_pacing.step_line(adapted, 1.0 / 60.0) and adapted_pacing.visible_count == 5, "defaults: presenter view never holds and shows the native count")
	var slow_only := defaults.duplicate(); slow_only.text_speed = "slow"
	var unproved_pacing := Pacing.new(); unproved_pacing.configure(slow_only)
	check(not unproved_pacing.step_line(Presenter.pacing_view({}, 5, 18, true), 1.0 / 60.0) and unproved_pacing.identity.is_empty(), "an unproved line never holds the native input")
	# The reset sentinel (-1) means "draw the native pos": the driver's glyph
	# count for an unproved line equals the native reveal at every speed.
	check(unproved_pacing.visible_count == -1, "an unproved line leaves the reset sentinel")
	var unproved_native := true
	for speed: String in ["normal", "slow", "fast", "instant"]:
		var speed_settings := defaults.duplicate(); speed_settings.text_speed = speed
		var each := Pacing.new(); each.configure(speed_settings)
		for pos: int in [0, 1, 7, 18]:
			each.step_line(Presenter.pacing_view({}, pos, 18, pos == 18), 1.0 / 60.0)
			unproved_native = unproved_native and Presenter.draw_count(each.visible_count, pos) == pos and Vn.draw_count(each.visible_count, pos) == pos
	check(unproved_native, "unproved lines draw the native pos at every text speed")
	check(Presenter.draw_count(adapted_pacing.visible_count, 5) == 5, "a proved line draws pacing.visible_count")
	var proved_fast := defaults.duplicate(); proved_fast.text_speed = "fast"
	var fast_pacing := Pacing.new(); fast_pacing.configure(proved_fast)
	fast_pacing.step_line(Presenter.pacing_view(talk_view, 3, 18, false), 1.0 / 60.0)
	check(Presenter.draw_count(fast_pacing.visible_count, 3) == 6, "a proved line at fast speed draws pacing.visible_count, not pos")
	# Route report for WP7's scope tags: only the opening consumes pacing today.
	# The presenters build the pacing view, but no frame driver calls them yet
	# (WP9 stays open), so WP7 keeps their "opening only" tags.
	var routes := {"opening_prologue": "verified (settings_runtime/prologue path, existing tests)",
		"dialogue_textbox": "NOT VERIFIED: presenter builds the pacing view; no frame driver calls it (synthetic line)",
		"vn_scene": "NOT VERIFIED: presenter builds the pacing view; no frame driver calls it (synthetic line)"}
	print("WP9 routes: " + JSON.stringify(routes))
	if failures.is_empty():
		print("Accessibility consumers: default identity, native textbox byte identity, backdrop region, pacing holds with unchanged native trace, auto-advance after text and voice, flash/shake clamps, text-layer read/skip passed")
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: " + failure)
		quit(1)
