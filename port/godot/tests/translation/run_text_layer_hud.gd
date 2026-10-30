extends SceneTree
## T-39016: command HUD narration through the text layer, with original
## content. Needs TEXT_LAYER_PACK/TEXT_LAYER_SHA256 (tools/port/build_text_pack.py
## output) and HUD_MANIFEST/HUD_SHA256/HUD_FONT_ROOT (regenerated HUD content).
## The rule and context below are caller state for a rendered frame, as in
## tests/hud/run_fresh_content_tests.gd; they are not a gameplay witness.
const Store = preload("res://services/text_layer_store.gd")
const Presenter = preload("res://presentation/command_hud_presenter.gd")
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition:
		failures.append(label)

func _initialize() -> void:
	var layer := Store.new()
	var started := Time.get_ticks_msec()
	check(layer.open_pack(OS.get_environment("TEXT_LAYER_PACK"), OS.get_environment("TEXT_LAYER_SHA256")), "text layer opens: " + layer.error)
	print("Text layer entries: ", layer.entry_count(), " open_ms: ", Time.get_ticks_msec() - started)
	var presenter := Presenter.new()
	check(presenter.open_manifest(OS.get_environment("HUD_MANIFEST"), OS.get_environment("HUD_SHA256"), OS.get_environment("HUD_FONT_ROOT")), "HUD opens: " + presenter.error)
	if not failures.is_empty():
		_finish()
		return
	var kinds: Array = [30]
	var markers: Array = [0]
	for day: int in range(1, 32):
		kinds.append(0)
		markers.append(0)
	var rule := {"year": 95, "month": 4, "day": 4, "weekday": 2,
		"params": [65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536, 65536],
		"calendar": [{"day_kind": kinds, "marker": markers}, {"day_kind": kinds.duplicate(), "marker": markers.duplicate()}],
		"menu_cursor": 0, "icon_enabled": [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
		"player_flags": 0, "player_flags_upper": 0, "memory_card_busy": 0}
	var context := {"game_mode": 0x34, "mode_stage": 1, "mode_step": 0, "icon_mode": 0,
		"icon_count": 14, "cursor_style": 0x9C, "hover_ticks": 32, "family_name_hex": "82a0"}
	var state: Dictionary = presenter.create_state(rule, context)
	check(not state.is_empty(), "HUD state: " + presenter.error)
	var original: Image = presenter.render(state)
	check(original != null, "original narration renders: " + presenter.error)
	check(presenter.set_translated_text(layer) == 2, "both narration strings resolve by executable location: " + presenter.text_rejection)
	var translated: Image = presenter.render(state)
	check(translated != null, "translated narration renders: " + presenter.error)
	if original != null and translated != null:
		# Every changed pixel lies in the original narration row: x from the
		# run origin over its native 14-pixel cells plus shadow, y over 12 rows
		# plus shadow.
		var strings: Dictionary = presenter._manifest["content"]["strings"]
		var cells: int = (str(strings["narration_prefix"]["raw_hex"]).length() + str(strings["ordinary_command"]["raw_hex"]).length()) / 4
		var row := Rect2i(160 + Presenter.NARRATION_X, 120 + 50, cells * 14 + 1, 13)
		var changed := 0
		var outside := 0
		for y: int in 240:
			for x: int in 320:
				if original.get_pixel(x, y) != translated.get_pixel(x, y):
					changed += 1
					if not row.has_point(Vector2i(x, y)):
						outside += 1
		check(changed > 0, "English narration changes the frame")
		check(outside == 0, "translated pixels stay inside the original narration row: %d outside" % outside)
	presenter.set_translated_text(null)
	var restored: Image = presenter.render(state)
	check(restored != null and original != null and restored.get_data() == original.get_data(), "clearing the layer restores the exact original frame")
	# Negative cases: a lookup with changed bytes or another executable hash
	# keeps the original, and the presenter reports why.
	var strings: Dictionary = presenter._manifest["content"]["strings"]
	var raw: PackedByteArray = str(strings["ordinary_command"]["raw_hex"]).hex_decode()
	var offset: int = int(strings["ordinary_command"]["address"]) - Presenter.EXECUTABLE_LOAD_ADDRESS + Presenter.EXECUTABLE_HEADER_SIZE
	var exe_sha: String = presenter._manifest["provenance"]["inputs"]["executable"]["sha256"]
	check(not layer.lookup_source("SLPM_86.053", exe_sha, offset, raw, presenter._font).is_empty(), "narration record resolves directly")
	raw[0] ^= 1
	check(layer.lookup_source("SLPM_86.053", exe_sha, offset, raw, presenter._font).is_empty(), "changed source bytes keep the original")
	raw[0] ^= 1
	check(layer.lookup_source("SLPM_86.053", "0".repeat(64), offset, raw, presenter._font).is_empty(), "another executable keeps the original")
	var empty := Store.new()
	check(presenter.set_translated_text(empty) == 0 and not presenter.text_rejection.is_empty(), "an unopened layer binds nothing and says why")
	var after: Image = presenter.render(state)
	check(after != null and original != null and after.get_data() == original.get_data(), "rejected layer leaves the original frame")
	if translated != null:
		var out := OS.get_environment("TEXT_LAYER_CAPTURE")
		if not out.is_empty():
			translated.save_png(out)
	_finish()

func _finish() -> void:
	print("Text layer HUD failures: ", failures)
	quit(0 if failures.is_empty() else 1)
