extends RefCounted
## Read-only diagnostics for strict loaders. Never changes acceptance or paths.
const Rules = preload("res://rules/new_game.gd")
static func inspect(path: String, accepted: bool) -> Dictionary:
	var info: Dictionary = {"supplied":not path.is_empty(),"accepted":accepted,"path":path,
		"absolute":path.is_absolute_path(),"canonical":path.simplify_path()==path,
		"resource_root":ProjectSettings.globalize_path("res://"),"file_exists":FileAccess.file_exists(path),
		"strict_path_valid":Rules._external_manifest_path_valid(path),"linked_components":[]}
	if path.begins_with("/"):
		var root: DirAccess=DirAccess.open("/")
		var current: String=""
		if root != null:
			for component in path.split("/",false):
				current+="/"+component
				if root.is_link(current): info.linked_components.append(current)
	# Never read rejected paths just to diagnose them.
	if info.strict_path_valid:
		var file: FileAccess=FileAccess.open(path,FileAccess.READ)
		info["open_error"]=FileAccess.get_open_error()
		if file != null:
			info["size"]=file.get_length()
			if file.get_length()<=65536:
				var raw: PackedByteArray=file.get_buffer(file.get_length())
				var hash:=HashingContext.new();hash.start(HashingContext.HASH_SHA256);hash.update(raw)
				info["sha256"]=hash.finish().hex_encode()
				var parsed: Variant=JSON.parse_string(raw.get_string_from_utf8())
				info["json_dictionary"]=parsed is Dictionary
				if parsed is Dictionary:
					info["keys"]=parsed.keys();info["schema"]=parsed.get("schema","")
					if parsed.get("tables") is Dictionary:
						var shapes: Dictionary={}
						for key in parsed.tables:
							shapes[key]=parsed.tables[key].size() if parsed.tables[key] is Array else -1
						info["table_lengths"]=shapes
	return info
