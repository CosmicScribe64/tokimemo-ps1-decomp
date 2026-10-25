class_name CommandHud
extends RefCounted
## Bounded school-day icon-record bridge. It carries source-provided icon/frame
## identities; it does not create substitute art or infer a source atlas mapping.

static func build(command_id: int, icon_records: Array) -> Dictionary:
	if command_id != 0 and command_id != 7:
		return _unsupported("command HUD route is not in the proved command 0/7 slice")
	if icon_records.size() != 14:
		return _unsupported("weekday command screen requires 14 source icon records")
	var seen: Dictionary = {}
	var output: Array[Dictionary] = []
	for record: Variant in icon_records:
		if not record is Dictionary:
			return _unsupported("command icon record is malformed")
		for key in ["icon_id", "component_id", "frame_id", "x", "y", "enabled"]:
			if not record.has(key):
				return _unsupported("command icon record is missing " + key)
		if typeof(record["icon_id"]) != TYPE_INT or typeof(record["x"]) != TYPE_INT or typeof(record["y"]) != TYPE_INT or typeof(record["enabled"]) != TYPE_BOOL:
			return _unsupported("command icon record has a field with the wrong type")
		var icon_id: int = int(record["icon_id"])
		if icon_id < 0 or icon_id > 13 or seen.has(icon_id):
			return _unsupported("command icon IDs must be unique members of 0..13")
		if str(record["component_id"]).is_empty() or str(record["frame_id"]).is_empty():
			return _unsupported("command icon needs an external source component and frame identity")
		if int(record["x"]) < -160 or int(record["x"]) > 160 or int(record["y"]) < -120 or int(record["y"]) > 120:
			return _unsupported("command icon center is outside PS1 logical screen")
		seen[icon_id] = true
		output.append({"kind": "source_sprite", "icon_id": icon_id,
			"component_id": str(record["component_id"]), "frame_id": str(record["frame_id"]),
			"x": int(record["x"]), "y": int(record["y"]),
			"hit_box_width": 20, "hit_box_height": 16, "enabled": bool(record["enabled"]),
			"selected": icon_id == command_id, "coordinate_origin": "screen_center"})
	if seen.size() != 14:
		return _unsupported("weekday command icon set is incomplete")
	return {"status": "supported", "command_id": command_id,
		"coordinate_space": "ps1_center_relative_320x240", "sprites": output}

static func _unsupported(message: String) -> Dictionary:
	return {"status": "unsupported", "diagnostic": message}
