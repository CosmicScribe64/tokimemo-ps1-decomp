extends RefCounted
## T-29024. shooter_spawn_enemy allocation and exact SDK setup call order.
## Native model/coordinate/pose callbacks are required, never success stubs.
const Math = preload("res://core/native_math.gd")
const Player = preload("res://overlays/shooter_player_rules.gd")
const Pool = preload("res://overlays/shooter_enemy_pool_rules.gd")
const Stage = preload("res://scenes/shooter_stage3_controller.gd")
const Tables = preload("res://services/shooter_enemy_tables.gd")
const REQUIRED: Array[String] = ["map_model","init_actor_coordinate","link_actor_model",
	"init_joint_coordinate","apply_initial_pose","link_joint_model"]
# Source enemy_pool_and_keyframes.c:spawn and asm/libgs.s:GsInitCoordinate2.
# Mapping/pose/link-joint calls do not write actor records. Init-coordinate
# may write its actor coordinate or its parent actor's child link. Link-actor
# writes only the selected actor's model handle.
const SDK_ACTOR_FIELDS: Dictionary = {"map_model":[], "init_actor_coordinate":["coordinate"],
	"link_actor_model":["model"], "init_joint_coordinate":["coordinate"],
	"apply_initial_pose":[], "link_joint_model":[],
	"pose_initial":["coordinate"], "pose_key":[]}

static func validate_models(state: Dictionary) -> String:
	var error: String = Stage.validate(state)
	if not error.is_empty(): return error
	for model: Dictionary in state["joint_models"]:
		if not model.get("links") is Array or model["links"].size() != 3: return "native joint model requires three signed32 links"
		for link: Variant in model["links"]:
			if not link is int or link < -2147483648 or link > 2147483647: return "native joint-model link exceeds signed32"
	return ""

static func actor_records(state: Dictionary, allowed_actor: int = -1, allowed_fields: Array = []) -> Array:
	var records: Array = []
	for index: int in range(state["actors"].size()):
		var actor: Dictionary = state["actors"][index]
		var record: Dictionary = actor.duplicate(true)
		# Ignore only the source-backed service fields at its addressed slot.
		# All identity/provenance, other slots and rule fields remain compared.
		if index == allowed_actor:
			for field: String in allowed_fields: record.erase(field)
		records.append(record)
	return records

static func actor_identity_error(state: Dictionary, before: Array, allowed_actor: int = -1, allowed_fields: Array = []) -> String:
	return "" if actor_records(state,allowed_actor,allowed_fields) == before else "native model/pose service changed actor identity or rule-owned fields"

static func _call(services: Dictionary, name: String, state: Dictionary, args: Dictionary) -> String:
	var owner: int = args.get("actor",-1)
	var allowed: Array = SDK_ACTOR_FIELDS[name]
	var before: Array = actor_records(state,owner,allowed)
	var result: Variant = services[name].call(state,args)
	if not result is String: return "native model service must return diagnostic String: "+name
	if not result.is_empty(): return result
	var error: String = validate_models(state)
	return error if not error.is_empty() else actor_identity_error(state,before,owner,allowed)

static func _vector(value: Array) -> bool:
	if value.size() != 4: return false
	for word: Variant in value:
		if not word is int or word < -32768 or word > 32767: return false
	return true

static func spawn(state: Dictionary, kind: int, mode: int, argument: Array, position: Array,
		rotation: Array, hp: int, drop: int, data: Dictionary, layout: Dictionary, services: Dictionary) -> Dictionary:
	var error: String = validate_models(state)
	if error.is_empty(): error = Tables.validate(data)
	if not error.is_empty(): return {"error":error,"value":-1}
	if kind < 0 or kind >= 17 or not _vector(argument) or not _vector(position) or not _vector(rotation):
		return {"error":"native spawn kind/vector input invalid","value":-1}
	for value: int in [mode,hp,drop]:
		if value < -2147483648 or value > 2147483647:
			return {"error":"native spawn scalar exceeds signed32","value":-1}
	for key: String in ["actor_base","joint_coordinate_base"]:
		if not layout.get(key) is int or layout[key] < 0 or layout[key] > 0xffffffff:
			return {"error":"native coordinate-address layout missing: "+key,"value":-1}
	var index: int = 0
	for i: int in range(16,63):
		if (state["actors"][i]["flag_bytes"][1] & 0x80) == 0: index = i; break
	if index == 0 or Pool.count_free_coordinates(state) < data["joint_count"][kind]:
		return {"error":"","value":-1}
	# Only kind15/16 use the paired translation/rotation table path.
	if kind in [15,16]: return {"error":"paired-table model kinds15/16 are outside this stage3 tranche","value":-1}
	for name: String in REQUIRED:
		if not services.get(name) is Callable or not services[name].is_valid():
			return {"error":"required native model/pose service missing: "+name,"value":-1}
	var base: int = Math.add32(data["model_base"][kind],4)
	error = _call(services,"map_model",state,{"address":base})
	if not error.is_empty(): return {"error":error,"value":-1}
	base = Math.add32(base,8)
	var actor: Dictionary = state["actors"][index]
	# shooter_release_actor_model -> view_model_record_init(2*index+2).
	actor["flag_bytes"][0] = 1; actor["flag_bytes"][1] = 0; actor["mode"] = 0; actor["phase"] = 0
	for key: String in Player.ACTOR_WORDS: actor[key] = 0
	for key: String in ["scale_x","scale_y","scale_z"]: actor[key] = 4096
	actor["stats"][0] = 0; actor["stats"][1] = 1; actor["stats"][2] = 0
	var actor_address: int = Math.s32(layout["actor_base"]+index*0x88+0x14)
	error = _call(services,"init_actor_coordinate",state,{"actor":index,"parent":0,"address":actor_address})
	if error.is_empty(): error = _call(services,"link_actor_model",state,{"actor":index,"address":base,"object":1 if kind == 10 else 0})
	if not error.is_empty(): return {"error":error,"value":-1}
	var remaining: int = data["joint_count"][kind]
	for slot: int in range(64):
		if remaining == 0: break
		if state["coordinate_owner"][slot] != 0: continue
		var ordinal: int = data["joint_count"][kind]-remaining
		var joint_address: int = Math.s32(layout["joint_coordinate_base"]+slot*0x50)
		error = _call(services,"init_joint_coordinate",state,{"actor":index,"joint":slot,"parent":actor_address,"address":joint_address})
		if not error.is_empty(): return {"error":error,"value":-1}
		state["coordinate_owner"][slot] = index
		error = _call(services,"apply_initial_pose",state,{"joint":slot,"kind":kind,
			"pose_table":data["joint_poses"][kind],"pose_ordinal":ordinal})
		if error.is_empty(): error = _call(services,"link_joint_model",state,{"joint":slot,"address":base,"object":ordinal})
		if not error.is_empty(): return {"error":error,"value":-1}
		state["joint_models"][slot]["links"][0] = joint_address
		state["joint_models"][slot]["attribute"] = Pool.HIDDEN
		remaining -= 1
	# A plain-state SDK provider may replace the record with an equivalent
	# dictionary. Resolve the current native slot before the final stores.
	actor = state["actors"][index]
	actor["model"]["attribute"] = Pool.HIDDEN
	actor["mode"] = Math.u8(mode); actor["counter"] = 32767
	actor["stats"][2] = Math.u8(kind); actor["phase"] = 1; actor["auxiliary"] = 0; actor["weapon"] = 0
	for i: int in range(3):
		actor[["argument_x","argument_y","argument_z"][i]] = argument[i]
		actor[["x","y","z"][i]] = position[i]
		actor[["rotation_x","rotation_y","rotation_z"][i]] = rotation[i]
	actor["param"] = rotation[3]; actor["flag_bytes"][1] = 0x80
	for key: String in ["scale_x","scale_y","scale_z"]: actor[key] = data["scale"][kind]
	actor["stats"][1] = Math.u8(hp); actor["stats"][0] = data["radius"][kind]
	actor["stats"][3] = Math.u8(drop | 1)
	actor["model"]["links"][0] = actor_address
	return {"error":validate_models(state),"value":index}
