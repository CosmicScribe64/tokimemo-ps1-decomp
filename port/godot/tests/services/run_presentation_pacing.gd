extends "res://tests/services/settings_test_tree.gd"
const Pacing = preload("res://services/presentation_pacing.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
func _initialize() -> void:
	var pacing := Pacing.new(); var settings: Dictionary = Preferences.defaults().accessibility
	pacing.configure(settings)
	var opening := {"prologue_text_work": {"line_id": 0, "pos": 30, "total": 30}, "prologue_text": {"line_id": 0, "k_disp_inc_result": 1}, "audio": {"epoch": 1, "clips": [], "status": "silent", "clip_index": 0}}
	var before := JSON.stringify(opening)
	check(not pacing.step(opening, 1.0 / 60.0)); check(pacing.visible_count == 30)
	settings.text_speed = "slow"; settings.auto_delay = 0.5; pacing.configure(settings); pacing.reset()
	check(pacing.step(opening, 1.0 / 60.0)); check(pacing.visible_count == 0)
	for index: int in 119: check(pacing.step(opening, 1.0 / 60.0))
	check(pacing.visible_count >= 29)
	for index: int in 32: pacing.step(opening, 1.0 / 60.0)
	check(not pacing.step(opening, 1.0 / 60.0)); check(pacing.visible_count == 30)
	check(JSON.stringify(opening) == before)
	opening.audio = {"epoch": 2, "clips": [{}, {}], "status": "finished", "clip_index": 0}
	check(not pacing.step(opening, 0.1)); check(pacing.visible_count == 1)
	opening.audio.clip_index = 1
	check(pacing.step(opening, 0.1))
	opening.audio.status = "cancelled"; check(not pacing.step(opening, 0.1)); check(pacing.identity.is_empty())
	opening.audio = {"epoch": 3, "clips": [], "status": "silent", "clip_index": 0}
	check(pacing.step(opening, 0.0)); var frozen := pacing.visible_count
	check(pacing.step(opening, 0.0)); check(pacing.visible_count == frozen)
	opening.prologue_text_work.line_id = 1; opening.prologue_text.line_id = 1
	check(pacing.step(opening, 0.1)); check(pacing.visible_count == 1)
	print("Presentation pacing: defaults, slow completion, reading delay, final voice, cancellation, paused clock, line identity and source immutability passed")
	finish()
