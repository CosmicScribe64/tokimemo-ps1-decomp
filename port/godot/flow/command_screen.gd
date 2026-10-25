extends RefCounted
## T-29004: native menu-modeA input and confirmation for commands0/7.
## Source menu_cursor_update(0), settings_command_screen_icon_confirm.

const InputFrame = preload("res://core/input_frame.gd")
const State = preload("res://rules/school_day_state.gd")
const Session = preload("res://core/game_session.gd")

static func step(state: State, input: InputFrame, result: Session.FrameResult,
	validated_capabilities: Array[int] = []) -> void:
	if state.mode_stage != 1 or state.mode_step != 0:
		result.error = "command screen requires captured native stage1/step0"
		return
	select_cursor(state, input, result)
	if not result.error.is_empty():
		return
	confirm(state, input, result, validated_capabilities)

static func select_cursor(state: State, input: InputFrame, result: Session.FrameResult) -> void:
	# Shared source menu-modeA operation; the holiday caller invokes this at
	# its HUD tail AFTER confirmation, preserving that handler's source order.
	var data: Dictionary = state.rule_state
	if not data.has("menu_mode") or data["menu_mode"] != 1 or not data.get("menu_disabled") is Array or data["menu_disabled"].size() != 14:
		result.error = "only source menu-modeA with fourteen menu records is implemented"
		return
	for disabled: Variant in data["menu_disabled"]:
		if not disabled is int or disabled < 0 or disabled > 255:
			result.error = "native menu disabled flags must be bytes"
			return
	if input.pressed & 0x100 or input.held & 0x100:
		result.error = "native menu-mode change via Select is outside bounded slice"
		return
	var direction: int = 0
	if (input.held & 15) == 0:
		match input.pressed & 0xf000:
			0x1000, 0x8000: direction = -1
			0x2000, 0x4000: direction = 1
	var selected: int = data["menu_cursor"] + direction
	if selected >= 14:
		selected = 0
	if selected < 0:
		selected = 13
	var budget: int = 16
	while data["menu_disabled"][selected] == 1:
		if budget == 0:
			break
		budget -= 1
		selected = (selected + direction + 14) % 14
	data["menu_cursor"] = selected
	result.presentation.append({"kind": "command_selection", "command": selected})

static func confirm(state: State, input: InputFrame, result: Session.FrameResult,
	validated_capabilities: Array[int] = []) -> void:
	var data: Dictionary = state.rule_state
	var selected: int = data["menu_cursor"]
	if (input.pressed & 0x20) == 0:
		return
	if selected not in [0, 7]:
		result.error = "selected native command is not implemented: %d" % selected
		return
	# Launch-time media capabilities come from the strict external pack loader.
	# Legacy rule-only callers retain their explicit fixture capability record.
	var capabilities: Array = data["resources"].get("available_commands", []) if validated_capabilities.is_empty() else validated_capabilities
	if not capabilities.has(selected):
		result.error = "required route content capability is unavailable for selected command"
		return
	# A visually disabled icon may still be selected: menu_disabled and
	# icon_enabled are separate original arrays. Confirmation checks the latter.
	result.audio.append({"kind": "native_cd_audio_command", "command": 9})
	result.audio.append({"kind": "native_audio_command", "command": 3})
	result.audio.append({"kind": "native_restore_bgm", "bank": 0, "sequence": 0})
	if data["icon_enabled"][selected] != 1:
		return
	result.audio.append({"kind": "native_audio_command", "command": 0x500})
	state.reset_game_mode_fields(0x30)
	data["voice_wait_frames"] = 0
	data["message_title_visible"] = false
	data["last_command"] = (data["last_command"] & ~15) | selected
	result.transitions.append({"kind": "native_mode", "mode": 0x30, "command": selected})
