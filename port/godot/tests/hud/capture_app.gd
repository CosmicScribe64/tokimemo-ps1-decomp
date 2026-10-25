extends "res://app/playable_boot.gd"
## Capture a settled live selection7 before its real confirmation input.
var selection_captured: bool = false

func _physics_process(delta: float) -> void:
	super._physics_process(delta)
	if not selection_captured and smoke_command == 7 and state != null and state.game_mode == 0x34 \
		and state.rule_state["menu_cursor"] == 7 and session.clock.logic_iterations >= 85:
		selection_captured = true
		set_physics_process(false)
		await _capture("selection-07")
		var receipt: FileAccess = FileAccess.open(output_root.path_join("selection-07.json"), FileAccess.WRITE)
		receipt.store_string(JSON.stringify({"state": state.projection(), "state_hash": session.state_hash(),
			"clock": session.clock.snapshot(), "rng": session.rng.checkpoint(), "trace": trace}, "\t"))
		receipt.close()
		set_physics_process(true)
