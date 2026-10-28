extends RefCounted
## T-29024. Native enemy_bullets_and_pickups.c and explosion allocation.
## Mutate only a private validated transaction. Normalization remains native SDK.
const Math = preload("res://core/native_math.gd")
const Rng = preload("res://core/native_rng.gd")

static func reseed(state: Dictionary, rng: Rng, seed: int) -> void:
	rng.reseed(seed)
	state["effects"].append({"kind":"native_srand","seed":Math.u32(seed)})

static func draw(state: Dictionary, rng: Rng, caller: String) -> int:
	var before: int = rng.state
	var value: int = rng.next(caller)
	state["effects"].append({"kind":"native_rand","caller":caller,"before":before,"after":rng.state,"value":value})
	return value

static func spawn_item(state: Dictionary, position: Array, kind: int, rng: Rng) -> int:
	for index: int in range(16):
		var item: Dictionary = state["items"][index]
		if (item["flags"] & 0x80) != 0: continue
		item["age"] = 0
		item["flags"] = 0x80
		item["x"] = position[0]; item["y"] = position[1]; item["z"] = position[2]
		item["velocity_x"] = 63 - (draw(state,rng,"shooter_spawn_item.x") & 127)
		item["velocity_y"] = 63 - (draw(state,rng,"shooter_spawn_item.y") & 127)
		item["velocity_z"] = 80
		item["argument"] = Math.u8(kind)
		return index
	return -1

static func spawn_explosion(state: Dictionary, position: Array, velocity: Array) -> String:
	var index: int = Math.divmod32(state["explosions"][0]["position"]["w"],16).remainder
	if index < 0: return "native negative explosion ring index needs out-of-pool memory evidence"
	var particle: Dictionary = state["explosions"][index]
	for axis: int in range(4):
		particle["position"][["x","y","z","w"][axis]] = Math.s16(position[axis])
		particle["velocity"][["x","y","z","w"][axis]] = Math.s16(velocity[axis])
	particle["age"] = 1; particle["image_index"] = 1; particle["flags"] = 0x80
	# Index0's full position copy can replace the ring word before this increment.
	var old: int = state["explosions"][0]["position"]["w"]
	state["explosions"][0]["position"]["w"] = Math.s16(old+1)
	if old >= 17: state["explosions"][0]["position"]["w"] = 0
	return ""

static func spawn_bullet(state: Dictionary, position: Array, speed: int, flags: int, rng: Rng, normalize: Callable) -> Dictionary:
	var h: int = Math.s16(speed)
	if h == 0: h = 256
	var index: int = -1
	for i: int in range(16):
		if (state["enemy_bullets"][i]["flags"] & 0x80) == 0: index = i; break
	if index < 0: return {"error":"","value":-1}
	if (flags & 0x80) != 0 and not normalize.is_valid():
		return {"error":"native func_800AA460 normalization is required","value":-1}
	var divisor: int = Math.divmod32(4096,h).quotient+1
	if (flags & 0x80) != 0 and divisor == 0:
		return {"error":"native bullet normalization divides by zero at this speed","value":-1}
	var bullet: Dictionary = state["enemy_bullets"][index]
	bullet["age"] = 0; bullet["flags"] = 0x80
	bullet["x"] = position[0]; bullet["y"] = position[1]; bullet["z"] = position[2]
	if (flags & 0x80) != 0:
		var direction: Array = [state["actors"][0]["x"]-position[0],
			state["actors"][0]["y"]-position[1],state["actors"][0]["z"]-position[2]]
		if (flags & 0x40) != 0:
			for i: int in range(3): direction[i] = Math.add32(direction[i],127-(draw(state,rng,"shooter_spawn_enemy_bullet.jitter") & 255))
		var result: Variant = normalize.call(direction)
		if not result is Dictionary or not result.get("error") is String: return {"error":"native normalization result is malformed","value":-1}
		if not result["error"].is_empty(): return {"error":result["error"],"value":-1}
		if not result.get("value") is Array or result["value"].size() != 3: return {"error":"native normalization vector extent is malformed","value":-1}
		for i: int in range(3):
			var value: Variant = result["value"][i]
			if not value is int or value < -2147483648 or value > 2147483647: return {"error":"native normalization output exceeds signed32","value":-1}
			bullet[["velocity_x","velocity_y","velocity_z"][i]] = Math.s16(Math.divmod32(value,divisor).quotient)
	else:
		bullet["velocity_z"] = h; bullet["velocity_x"] = 0; bullet["velocity_y"] = 0
	bullet["argument"] = Math.u8(flags)
	return {"error":"","value":index}
