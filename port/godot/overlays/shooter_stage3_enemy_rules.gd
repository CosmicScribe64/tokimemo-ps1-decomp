extends RefCounted
## T-29024. Native stage3 behaviors, generic death/drop and class13 key dispatch.
## Model/GTE/pose services remain mandatory source-backed capabilities.
const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")
const Pool = preload("res://overlays/shooter_enemy_pool_rules.gd")
const Particles = preload("res://overlays/shooter_enemy_particles.gd")
const Stage = preload("res://scenes/shooter_stage3_controller.gd")
const Tables = preload("res://services/shooter_enemy_tables.gd")
const Spawn = preload("res://overlays/shooter_enemy_spawn_rules.gd")
const DROPS: Dictionary = {0x10:[6],0x20:[7],0x30:[0],0x40:[1],0x50:[2],
	0x60:[3],0x70:[4],0x80:[5],0xd0:[6,6,6,6,6],0xe0:[0,1,7],
	0xf0:[6,6,7,2,0,1,5,3,4]}

static func _quarter(angle: int, table: Array) -> int:
	angle &= 4095
	if angle <= 1024: return table[angle]
	if angle <= 2048: return table[2048-angle]
	if angle <= 3072: return -table[angle-2048]
	return -table[4096-angle]

static func sine(angle: int, table: Array) -> int:
	angle = Math.s32(angle)
	return -_quarter(Math.u32(-angle),table) if angle < 0 else _quarter(angle,table)

static func cosine(angle: int, table: Array) -> int:
	angle = Math.s32(angle)
	# SDK cosine takes absolute angle then mirrors the same quarter table.
	return _quarter((Math.u32(-angle) if angle < 0 else angle)+1024,table)

static func _position(actor: Dictionary) -> Array:
	return [actor["x"],actor["y"],actor["z"],actor["counter"]]

static func _grow(actor: Dictionary) -> void:
	actor["scale_y"] = Math.s16(actor["counter"] << 9)
	actor["stats"][0] = Math.u8(Math.divmod32(actor["counter"]*22,32).quotient)
	if actor["scale_y"] >= actor["scale_x"]:
		actor["argument_x"] = Math.s16(actor["argument_x"]+1)
		actor["counter"] = 0

static func _shrink(state: Dictionary, index: int) -> void:
	var actor: Dictionary = state["actors"][index]
	actor["scale_y"] = Math.s16(Math.sll32(16-actor["counter"],9))
	actor["stats"][0] = Math.u8(Math.divmod32(Math.sll32(16-actor["counter"],5),32).quotient)
	if actor["scale_y"] <= 0: Pool.remove_enemy(state,index)

static func behavior(state: Dictionary, index: int, data: Dictionary, rng: Rng, normalize: Callable) -> String:
	var actor: Dictionary = state["actors"][index]
	var table: Array = data["sine_quarter"]
	match actor["mode"]:
		0x20:
			var factor: int = Math.divmod32(Math.sll32(actor["z"],5),25000).quotient
			actor["x"] = Math.s16(actor["x"]-(Math.s32(factor*cosine(Math.sll32(actor["counter"],5),table)) >> 12))
			factor = Math.divmod32(Math.sll32(actor["z"],4),25000).quotient
			actor["y"] = Math.s16(actor["y"]-(Math.s32(factor*sine(Math.sll32(actor["counter"],7),table)) >> 12))
			if actor["counter"] >= 301:
				actor["z"] = Math.s16(actor["z"]+256)
				actor["x"] = Math.s16(actor["x"]+Math.divmod32(state["actors"][0]["x"]+actor["argument_x"]-actor["x"],10).quotient)
				actor["y"] = Math.s16(actor["y"]+Math.divmod32(state["actors"][0]["y"]+actor["argument_y"]-actor["y"],10).quotient)
			if actor["z"] >= 4097: Pool.remove_enemy(state,index)
		0x21:
			actor["x"] = Math.s16(actor["x"]+Math.divmod32(-actor["x"],50).quotient)
			if actor["counter"] >= 181: actor["counter"] = 0
			if Math.divmod32(actor["counter"],10).remainder == 0 and absi(actor["x"]) < 1000:
				var flags: int = -1
				if actor["y"] == -1200 and actor["counter"] < 60: flags = 0xc0
				elif actor["y"] == -700 and actor["counter"] >= 61 and actor["counter"] < 120: flags = 0x82
				elif actor["y"] == -200 and actor["counter"] >= 121: flags = 0xc0
				if flags >= 0: return Particles.spawn_bullet(state,_position(actor),384,flags,rng,normalize)["error"]
		0x22:
			match actor["argument_x"]:
				0:
					actor["x"] = Math.s16((sine(actor["param"],table)*1000) >> 12)
					actor["y"] = Math.s16(((cosine(actor["param"],table)*1000) >> 12)-700)
					actor["z"] = -20000; actor["scale_y"] = 1
					actor["argument_x"] = Math.s16(actor["argument_x"]+1)
				1: _grow(actor)
				2:
					actor["x"] = Math.s16((sine(actor["param"],table)*1000) >> 12)
					actor["y"] = Math.s16(((cosine(actor["param"],table)*1000) >> 12)-700)
					actor["param"] = Math.s16(actor["param"]+32)
					if actor["counter"] >= 601: actor["argument_x"] = Math.s16(actor["argument_x"]+1); actor["counter"] = 0
				3: _shrink(state,index)
		0x23:
			var position: Array = _position(actor)
			match actor["argument_x"]:
				0:
					actor["x"] = 0; actor["y"] = -700; actor["z"] = -20000
					actor["scale_y"] = 1; actor["argument_x"] = Math.s16(actor["argument_x"]+1)
				1: _grow(actor)
				2:
					if Math.divmod32(actor["counter"],15).remainder == 0:
						var result: Dictionary = Particles.spawn_bullet(state,position,256,0xc2,rng,normalize)
						if not result["error"].is_empty(): return result["error"]
					if actor["counter"] >= 601: actor["argument_x"] = Math.s16(actor["argument_x"]+1); actor["counter"] = 0
				3: _shrink(state,index)
		0xff: return dying(state,index,rng)
		_: return "native behavior outside the implemented stage3 set: "+str(actor["mode"])
	return ""

static func dying(state: Dictionary, index: int, rng: Rng) -> String:
	var actor: Dictionary = state["actors"][index]
	if actor["counter"] == 0:
		Particles.reseed(state,rng,Math.add32(state["script_timer"],index))
		actor["flag_bytes"][1] = 0x90
		actor["argument_x"] = Particles.draw(state,rng,"shooter_behavior_dying.x") % (10 if actor["stats"][2] == 6 else 40)-(5 if actor["stats"][2] == 6 else 20)
		actor["argument_y"] = Particles.draw(state,rng,"shooter_behavior_dying.y") % (2 if actor["stats"][2] == 6 else 10)+(2 if actor["stats"][2] == 6 else 10)
		actor["argument_z"] = Particles.draw(state,rng,"shooter_behavior_dying.z") % (2 if actor["stats"][2] == 6 else 10)+(2 if actor["stats"][2] == 6 else 10)
		if actor["stats"][2] != 6: actor["param"] = Particles.draw(state,rng,"shooter_behavior_dying.rotation") % 6
		var error: String = Particles.spawn_explosion(state,_position(actor),
			[actor["argument_x"],actor["argument_y"],actor["argument_z"],actor["auxiliary"]])
		if not error.is_empty(): return error
	if actor["stats"][2] != 6:
		var axis: String = ["rotation_x","rotation_y","rotation_z"][Math.u16(actor["param"]) % 3]
		if Math.u16(actor["param"]) < 6: actor[axis] = Math.s16(actor[axis]+(16 if actor["param"] < 3 else -16))
	else:
		if Math.divmod32(actor["counter"],3).remainder == 0:
			Particles.reseed(state,rng,Math.add32(state["script_timer"],index))
			# Source stores only xyz here; its fourth stack word is unassigned.
			return "native class6 repeated explosion position.w needs source ABI evidence"
		actor["rotation_x"] = Math.s16(actor["rotation_x"]+2)
		actor["rotation_z"] = Math.s16(actor["rotation_z"]+2)
	Pool.set_visibility(state,1 if (actor["counter"] & 1) == 0 else 0,index)
	if actor["counter"] >= 61: Pool.remove_enemy(state,index)
	return ""

static func animate(state: Dictionary, index: int, data: Dictionary, pose: Callable) -> String:
	var actor: Dictionary = state["actors"][index]
	if actor["phase"] == 0: return ""
	var kind: int = actor["stats"][2]
	if kind not in [11,12,13,14,15,16]: return ""
	if kind != 13: return "native animation kind is outside stage3 class13: "+str(kind)
	actor["weapon"] = Math.s16(actor["weapon"]+1)
	if actor["weapon"] < 6: return ""
	if not pose.is_valid(): return "native class13 pose/matrix provider is missing"
	actor["weapon"] = 0
	# Native calls the same script for phase1 and phase2, then increments aux.
	for phase: int in [1,2]:
		if actor["phase"] != phase: continue
		var error: String = animate_keys(state,index,data,pose)
		if not error.is_empty(): return error
		actor = state["actors"][index]
	actor["auxiliary"] = Math.s16(actor["auxiliary"]+1)
	return ""

static func animate_keys(state: Dictionary, index: int, data: Dictionary, pose: Callable) -> String:
	var actor: Dictionary = state["actors"][index]
	var keys: Array = data["keys_d"]
	var frame: int = actor["auxiliary"]
	if frame >= keys[3]: actor["auxiliary"] = 0; frame = 0
	var phase: int = Math.divmod32(frame,keys[2]).remainder
	var pose_frame: int = 0
	var found: bool = false
	for offset: int in range(4,keys.size(),2):
		if phase < keys[offset]: pose_frame = keys[offset+1]; found = true; break
	if not found: return "native animation key search left the validated script"
	var n: int = 0
	var j: int = keys[0]
	var last: int = keys[1]
	for slot: int in range(64):
		if state["coordinate_owner"][slot] == index:
			if n >= j:
				var initial: bool = Math.divmod32(actor["auxiliary"],keys[2]).remainder == 0
				var allowed: Array = Spawn.SDK_ACTOR_FIELDS["pose_initial" if initial else "pose_key"]
				var before: Array = Spawn.actor_records(state,index,allowed)
				var result: Variant = pose.call(state,{"actor":index,"joint":slot,"pose_ordinal":j,
					"pose_table":data["joint_poses"][actor["stats"][2]],"frame":pose_frame,"initial":initial})
				if not result is String: return "native pose provider must return diagnostic String"
				if not result.is_empty(): return result
				var error: String = Spawn.validate_models(state)
				if error.is_empty(): error = Spawn.actor_identity_error(state,before,index,allowed)
				if not error.is_empty(): return error
				actor = state["actors"][index]
				j += 1
			n += 1
		if last < j: break
	return ""

static func update_enemies(state: Dictionary, data: Dictionary, rng: Rng, normalize: Callable, pose: Callable) -> String:
	var error: String = Spawn.validate_models(state)
	if error.is_empty(): error = Tables.validate(data)
	if not error.is_empty(): return error
	for index: int in range(16,63):
		var actor: Dictionary = state["actors"][index]
		if (actor["flag_bytes"][1] & 0x80) == 0: continue
		if actor["counter"] == 32767:
			Pool.set_visibility(state,1,index)
			actor["counter"] = 0
		if actor["stats"][1] == 0 and actor["mode"] != 0xff:
			actor["counter"] = 0; actor["phase"] = 2; actor["mode"] = 0xff
			for kind: int in DROPS.get(actor["stats"][3] & 0xf0,[]):
				Particles.spawn_item(state,_position(actor),kind,rng)
		error = behavior(state,index,data,rng,normalize)
		if error.is_empty(): error = animate(state,index,data,pose)
		if not error.is_empty(): return error
		# Native increments even when behavior removed this actor.
		actor = state["actors"][index]
		actor["counter"] = Math.s16(actor["counter"]+1)
	return rng.step_error()
