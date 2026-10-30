extends RefCounted
## T-39002 stage 21 handoff: the opening Session that reached mode 0x34
## stage 1 step 0 (settings_command_screen_mode_stage_select, the first frame
## where the player can choose) is handed, unchanged, to the normal gameplay
## controller (scenes/school_day_controller.gd), which rebinds the same router
## for 0x34/0x30/0x31/... The command HUD must come from the fresh
## HudSession (presentation/hud_session_context.gd) that the live route drove
## from boot; no captured context or rebuilt record is accepted.
##
## Interface the opening must provide before this can succeed:
##  - interactive_prologue.gd: command_ready (committed 0x34/1/0 after the
##    source reload, stage 0xFF steps 0-3) and take_command_handoff();
##  - a HudSession whose trace runs process_image -> game_state_init ->
##    command_screen_build -> enter_command_select(0x34, 1, 0);
##  - a controller with attach(session, state) -> String.
## The actual route cannot reach 0x34/1/0 yet. With the production native
## library the bank0 audio refuses at stage16 (BANK0_NATIVE_UNAVAILABLE) and, if
## reached without a native owner, at the stage21 bank transfer
## (FINISH_BANK_TRANSFER_UNSUPPORTED); the native-bank0 candidate performs the
## 0x11 transfer and the spring start (tests/opening/run_bank0_*.gd).
## No production code calls connect_gameplay yet, and no opening defines
## take_command_handoff; tests/opening/run_stage_21_handoff.gd pins each branch.

const HudSession = preload("res://presentation/hud_session_context.gd")
const NOT_READY := "opening has no committed command-select boundary at mode 0x34 stage 1 step 0"
const HUD_REQUIRED := "opening handoff requires the source-driven HUD session at command select"
const CONTROLLER_REQUIRED := "opening handoff requires a gameplay controller with attach(session, state)"

static func connect_gameplay(opening: RefCounted, controller: RefCounted, hud: Variant) -> Dictionary:
	if opening == null or opening.get("command_ready") != true or opening.get("state") == null:
		return {"error":NOT_READY}
	var state: Object = opening.get("state")
	if [state.get("game_mode"), state.get("mode_stage"), state.get("mode_step")] != [0x34, 1, 0] or state.get("opening").get("command_ready") != true:
		return {"error":NOT_READY}
	if not hud is HudSession or not hud.at_command_select():
		return {"error":HUD_REQUIRED}
	if controller == null or not controller.has_method("attach"):
		return {"error":CONTROLLER_REQUIRED}
	var handoff: Dictionary = opening.call("take_command_handoff")
	if not str(handoff.get("error", "")).is_empty():
		return handoff
	var failure: Variant = controller.call("attach", handoff.session, handoff.state)
	if not failure is String or not failure.is_empty():
		return {"error":str(failure) if failure is String else CONTROLLER_REQUIRED}
	return {"error":"", "session":handoff.session, "state":handoff.state, "rng":handoff.rng, "hud":hud}
