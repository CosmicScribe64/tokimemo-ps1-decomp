extends RefCounted
## Development route checkpoints (T-39018): an exact save/resume of the port
## runtime at a frame boundary, so a dev route can restart at, say, the first
## command-select frame instead of replaying the whole opening.
##
## DEV ONLY. A checkpoint is written and read only when the dev environment
## asks for it (TOKIMEMO_DEV_CHECKPOINT_SAVE / TOKIMEMO_DEV_RESUME, set by
## tools/port/run_installed_route.sh --checkpoint-save / --resume). A resumed
## run writes /out/resumed-from.json; tools/port/compare_route_modes.py and the
## route observers' acceptance outputs refuse such runs unless explicitly
## comparing a resume (compare_route_modes.py --resumed).
##
## What is captured, at the observer's _process (after the logic frame's
## physics step, before the frame's _process pump):
##   * every script object reachable from the roots (the app node and
##     /root/ModernSettings) through script variables, Node children and bound
##     Callables, with every script variable; Arrays and Dictionaries keep
##     their sharing (two variables holding one Dictionary still do);
##   * Image contents; WeakRef targets; Resources by resource_path;
##   * native objects (nodes, players, textures, stores' engine objects)
##     only by identity: a resume reuses the native the freshly booted app
##     holds at the same graph path, and refuses if there is none;
##   * signal connections between graph objects;
##   * the static variables listed in STATICS (logic-relevant statics);
##   * the native OriginalAudioEngine as its call log (engine_recorder.gd):
##     the resume replays it into a fresh engine and checks every result;
##   * the observer's and its receipts' plain-data variables.
## Lambdas cannot be serialised; a lambda is restored from the freshly booted
## app's lambda at the same graph path, and the resume refuses otherwise.
## Resume is supported for FAST routes only (the real-time sink owns an
## AudioStreamGeneratorPlayback whose device position cannot be restored).

const SCHEMA := "tokimemo.dev-runtime-checkpoint.v1"
const Recorder = preload("res://tests/checkpoint/engine_recorder.gd")
const STATICS := [["res://presentation/command_hud_presenter.gd", "_outer_frame_guard"]]
## Observer variables that describe the observer's own bindings, not route state.
const OBSERVER_SKIP := ["observer_bound", "finishing", "fast", "receipts", "initial_entry", "initial_rng", "original_scene", "resumed_from", "mode"]

class Graph extends RefCounted:
	## Deterministic breadth-first traversal shared by capture and by the
	## resume's mapping of the freshly booted runtime: an object's path is its
	## shortest chain of variables from a root (ties in declaration order), so
	## an object the booted app already holds gets the same path in both.
	var objects: Array = []          # Object, in discovery order
	var paths: Array[String] = []    # discovery path per object
	var ids: Dictionary = {}         # instance id -> index
	var containers: Array = []       # Array/Dictionary, in discovery order
	var _buckets: Dictionary = {}    # content hash -> container indices
	var errors: PackedStringArray = []
	var _queue: Array[int] = []

	func add_roots(roots: Dictionary) -> void:
		for name: String in roots: _object(roots[name], name)
		var next := 0
		while next < _queue.size():
			_visit(_queue[next])
			next += 1

	func index_of(object: Object) -> int:
		return int(ids.get(object.get_instance_id(), -1)) if object != null else -1

	## Identity lookup: containers are bucketed by content hash, then matched
	## with is_same, so two distinct equal containers stay distinct.
	func container_index(value: Variant) -> int:
		for index: int in _buckets.get(_bucket(value), []):
			if is_same(containers[index], value): return index
		return -1

	## Registers a container met outside the traversal (a bound argument of a
	## signal connection, a static); returns its index.
	func register(value: Variant) -> int:
		var index := container_index(value)
		if index >= 0: return index
		var bucket := _bucket(value)
		if not _buckets.has(bucket): _buckets[bucket] = []
		_buckets[bucket].append(containers.size())
		containers.append(value)
		return containers.size() - 1

	static func _bucket(value: Variant) -> int:
		return value.hash() ^ (typeof(value) << 24)

	func _object(object: Object, path: String) -> void:
		if object == null or not is_instance_valid(object) or ids.has(object.get_instance_id()): return
		ids[object.get_instance_id()] = objects.size()
		_queue.append(objects.size())
		objects.append(object)
		paths.append(path)

	func _visit(index: int) -> void:
		var object: Object = objects[index]
		var path: String = paths[index]
		if Recorder.is_wrapper(object) or object is Image: return
		if object is Resource and object.get_script() == null:
			for prop: Dictionary in object.get_property_list():
				if prop.usage & PROPERTY_USAGE_STORAGE: _value(object.get(prop.name), path + "." + prop.name)
			return
		if object is WeakRef:
			_value((object as WeakRef).get_ref(), path + ".@ref")
			return
		if object.get_script() != null:
			for prop: Dictionary in object.get_property_list():
				if prop.usage & PROPERTY_USAGE_SCRIPT_VARIABLE:
					_value(object.get(prop.name), path + "." + prop.name)
		if object is Node:
			for child: Node in (object as Node).get_children():
				_object(child, path + "/" + str(child.name))

	func _value(value: Variant, path: String) -> void:
		match typeof(value):
			TYPE_OBJECT:
				_object(value, path)
			TYPE_ARRAY, TYPE_DICTIONARY:
				if container_index(value) >= 0: return
				register(value)
				if value is Array:
					for index in (value as Array).size(): _value(value[index], "%s[%d]" % [path, index])
				else:
					for key: Variant in value:
						_value(key, "%s{key}" % path)
						_value(value[key], "%s{%s}" % [path, var_to_str(key)])
			TYPE_CALLABLE:
				var callable: Callable = value
				if not Recorder.is_lambda(callable) and callable.get_object() != null: _object(callable.get_object(), path + "()")
				for argument: Variant in callable.get_bound_arguments(): _value(argument, path + "(bound)")

class Encoder extends RefCounted:
	var graph: Graph
	var _written: Dictionary = {}    # container index -> true once its content is written
	var errors: PackedStringArray = []

	func value(v: Variant, path: String) -> Variant:
		match typeof(v):
			TYPE_OBJECT:
				if v == null or not is_instance_valid(v): return {"$k": "null"}
				var index := graph.index_of(v)
				if index < 0: errors.append("object outside the graph at " + path)
				return {"$k": "o", "i": index}
			TYPE_ARRAY, TYPE_DICTIONARY:
				var index := graph.register(v)
				if _written.has(index): return {"$k": "r", "i": index}
				_written[index] = true
				if v is Array:
					var items: Array = []
					for item_index in (v as Array).size(): items.append(value(v[item_index], "%s[%d]" % [path, item_index]))
					var array: Array = v
					return {"$k": "a", "i": index, "v": items, "t": [array.get_typed_builtin(), array.get_typed_class_name(), _script_path(array.get_typed_script())], "ro": array.is_read_only()}
				var pairs: Array = []
				for key: Variant in v: pairs.append([value(key, path + "{key}"), value(v[key], "%s{%s}" % [path, var_to_str(key)])])
				var dict: Dictionary = v
				return {"$k": "d", "i": index, "v": pairs, "t": [dict.get_typed_key_builtin(), dict.get_typed_key_class_name(), _script_path(dict.get_typed_key_script()),
					dict.get_typed_value_builtin(), dict.get_typed_value_class_name(), _script_path(dict.get_typed_value_script())], "ro": dict.is_read_only()}
			TYPE_CALLABLE:
				var callable: Callable = v
				if Recorder.is_lambda(callable): return {"$k": "l", "p": path}
				if not callable.is_valid() and callable.get_object() == null: return {"$k": "c0"}
				var bound: Array = []
				for argument: Variant in callable.get_bound_arguments(): bound.append(value(argument, path + "(bound)"))
				return {"$k": "c", "o": value(callable.get_object(), path + "()"), "m": callable.get_method(), "b": bound}
			TYPE_SIGNAL, TYPE_RID:
				errors.append("unsupported %s at %s" % [type_string(typeof(v)), path])
				return {"$k": "null"}
		return {"$k": "v", "v": v}

	static func _script_path(script: Variant) -> String:
		return (script as Script).resource_path if script is Script else ""

## [script path, inner-class subpath]; inner classes have no resource_path of
## their own, so their identity comes from inst_to_dict's @path/@subpath.
static func _script_identity(object: Object) -> Array:
	var script: Script = object.get_script()
	if script == null or Recorder.is_wrapper(object): return ["", ""]
	if not script.resource_path.is_empty(): return [script.resource_path, ""]
	var described: Dictionary = inst_to_dict(object)
	return [str(described.get("@path", "")), str(described.get("@subpath", ""))]

static func _roots(app: Node) -> Dictionary:
	var roots := {"app": app}
	var settings := app.get_node_or_null("/root/ModernSettings") if app.is_inside_tree() else null
	if settings != null: roots["settings"] = settings
	return roots

static func _graph(roots: Dictionary) -> Graph:
	var graph := Graph.new()
	graph.add_roots(roots)
	return graph

## Plain-data script variables of an observer-side object (no Objects,
## Callables or names in skip).
static func plain_variables(object: Object, skip: Array = []) -> Dictionary:
	var out := {}
	for prop: Dictionary in object.get_property_list():
		if not (prop.usage & PROPERTY_USAGE_SCRIPT_VARIABLE) or prop.name in skip or str(prop.name).begins_with("_dev_"): continue
		var value: Variant = object.get(prop.name)
		if _plain(value): out[prop.name] = value
	return out

static func _plain(value: Variant) -> bool:
	match typeof(value):
		TYPE_OBJECT, TYPE_CALLABLE, TYPE_SIGNAL, TYPE_RID: return false
		TYPE_ARRAY:
			for item: Variant in value:
				if not _plain(item): return false
		TYPE_DICTIONARY:
			for key: Variant in value:
				if not _plain(key) or not _plain(value[key]): return false
	return true

## Captures the runtime. meta must carry the route identity (bundle, library,
## sources, key, tick); observers maps a name to an observer-side object whose
## plain variables are saved.
static func capture(app: Node, observers: Dictionary, meta: Dictionary) -> Dictionary:
	var roots := _roots(app)
	var graph := _graph(roots)
	var encoder := Encoder.new()
	encoder.graph = graph
	var records: Array = []
	for index in graph.objects.size():
		var object: Object = graph.objects[index]
		var path: String = graph.paths[index]
		var identity := _script_identity(object)
		var record := {"path": path, "class": object.get_class(), "script": identity[0], "subpath": identity[1]}
		if Recorder.is_wrapper(object):
			record["kind"] = "engine"
			record["log"] = (object.get("call_log") as Array).duplicate(true)
		elif object is Image:
			var image: Image = object
			record["kind"] = "image"
			record["image"] = [image.get_width(), image.get_height(), image.has_mipmaps(), image.get_format(), image.get_data()]
		elif object is WeakRef:
			record["kind"] = "weakref"
			record["ref"] = encoder.value((object as WeakRef).get_ref(), path + ".@ref")
		elif object.get_script() != null:
			record["kind"] = "script"
			var props := {}
			for prop: Dictionary in object.get_property_list():
				if prop.usage & PROPERTY_USAGE_SCRIPT_VARIABLE:
					props[prop.name] = encoder.value(object.get(prop.name), path + "." + prop.name)
			record["props"] = props
		elif object is Resource and not (object as Resource).resource_path.is_empty():
			record["kind"] = "resource"
			record["resource_path"] = (object as Resource).resource_path
		elif object is Resource:
			# A native resource built at run time (an AudioStreamWAV from
			# original bytes): reused from the booted app when it holds one at
			# this path, otherwise rebuilt from its storage properties.
			record["kind"] = "native_resource"
			var storage := {}
			for prop: Dictionary in object.get_property_list():
				if prop.usage & PROPERTY_USAGE_STORAGE:
					storage[prop.name] = encoder.value(object.get(prop.name), path + "." + prop.name)
			record["storage"] = storage
		else:
			record["kind"] = "native"
		if object is Node:
			var children: Array = []
			for child: Node in (object as Node).get_children(): children.append(graph.index_of(child))
			record["children"] = children
			record["name"] = str((object as Node).name)
			record["processing"] = [(object as Node).is_processing(), (object as Node).is_physics_processing()]
			# Node state an _init or _ready sets, which a resume that creates the
			# node without _init would otherwise lose (process order matters:
			# the offline BGM device drains before the app's logic frame).
			var node: Node = object
			var groups: Array = []
			for group: StringName in node.get_groups():
				if not str(group).begins_with("_"): groups.append(str(group))
			record["node_state"] = [node.process_priority, node.process_physics_priority, node.process_mode, groups]
		records.append(record)
	var statics := []
	for item: Array in STATICS:
		var script: Script = load(item[0])
		statics.append([item[0], item[1], encoder.value(script.get(item[1]), "static:" + item[0] + ":" + item[1])])
	var connections: Array = []
	for index in graph.objects.size():
		var source: Object = graph.objects[index]
		for sig: Dictionary in source.get_signal_list():
			for connection: Dictionary in source.get_signal_connection_list(sig.name):
				var callable: Callable = connection.callable
				if Recorder.is_lambda(callable) or graph.index_of(callable.get_object()) < 0: continue
				connections.append([index, str(sig.name), encoder.value(callable, graph.paths[index] + "@" + str(sig.name)), int(connection.flags)])
	var observer_data := {}
	for name: String in observers: observer_data[name] = plain_variables(observers[name], OBSERVER_SKIP)
	var errors := graph.errors.duplicate()
	errors.append_array(encoder.errors)
	return {"error": "; ".join(errors) if not errors.is_empty() else "", "data": {"schema": SCHEMA, "meta": meta, "roots": roots.keys(),
		"objects": records, "containers": graph.containers.size(), "connections": connections, "statics": statics, "observers": observer_data}}

static func save(path: String, app: Node, observers: Dictionary, meta: Dictionary) -> String:
	var captured := capture(app, observers, meta)
	if not captured.error.is_empty(): return captured.error
	var file := FileAccess.open(path, FileAccess.WRITE)
	if file == null: return "checkpoint file cannot be written: " + path
	var bytes := var_to_bytes(captured.data).compress(FileAccess.COMPRESSION_ZSTD)
	file.store_32(0x544b4350) # "PCKT"
	file.store_64(bytes.size())
	file.store_64(var_to_bytes(captured.data).size())
	file.store_buffer(bytes)
	file.close()
	return ""

static func load_file(path: String) -> Dictionary:
	var file := FileAccess.open(path, FileAccess.READ)
	if file == null: return {"error": "checkpoint file cannot be read: " + path}
	if file.get_32() != 0x544b4350: return {"error": "not a dev runtime checkpoint: " + path}
	var size := file.get_64()
	var raw_size := file.get_64()
	var data: Variant = bytes_to_var(file.get_buffer(size).decompress(raw_size, FileAccess.COMPRESSION_ZSTD))
	if not data is Dictionary or data.get("schema") != SCHEMA: return {"error": "checkpoint schema differs from " + SCHEMA}
	return {"error": "", "data": data}

class Decoder extends RefCounted:
	var targets: Array = []          # restored object per record index
	var containers: Dictionary = {}  # container index -> restored container
	var live: Graph                  # freshly booted runtime
	var errors: PackedStringArray = []

	func value(encoded: Dictionary) -> Variant:
		match str(encoded["$k"]):
			"v": return encoded.v
			"null": return null
			"o": return targets[int(encoded.i)] if int(encoded.i) >= 0 else null
			"r": return containers[int(encoded.i)]
			"a":
				var typed: Array = encoded.t
				var array: Array = []
				if typed[0] != TYPE_NIL:
					var script: Variant = load(typed[2]) if not str(typed[2]).is_empty() else null
					array = Array([], typed[0], typed[1], script)
				containers[int(encoded.i)] = array
				for item: Dictionary in encoded.v: array.append(value(item))
				if encoded.ro: array.make_read_only()
				return array
			"d":
				var typed: Array = encoded.t
				var dict: Dictionary = {}
				if typed[0] != TYPE_NIL or typed[3] != TYPE_NIL:
					dict = Dictionary({}, typed[0], typed[1], load(typed[2]) if not str(typed[2]).is_empty() else null,
						typed[3], typed[4], load(typed[5]) if not str(typed[5]).is_empty() else null)
				containers[int(encoded.i)] = dict
				for pair: Array in encoded.v: dict[value(pair[0])] = value(pair[1])
				if encoded.ro: dict.make_read_only()
				return dict
			"c0": return Callable()
			"c":
				var target: Variant = value(encoded.o)
				if not target is Object: return Callable()
				var callable := Callable(target, StringName(encoded.m))
				if not encoded.b.is_empty():
					var bound: Array = []
					for argument: Dictionary in encoded.b: bound.append(value(argument))
					callable = callable.bindv(bound)
				return callable
			"l":
				var live_value: Variant = live_lambda(str(encoded.p))
				if not live_value is Callable: errors.append("lambda has no live counterpart at " + str(encoded.p))
				return live_value
		errors.append("unknown encoding " + str(encoded["$k"]))
		return null

	## The freshly booted runtime's value at a graph path ("root.prop.prop").
	func live_lambda(path: String) -> Variant:
		var dot := path.find(".")
		var parent := path.substr(0, path.rfind("."))
		var name := path.substr(path.rfind(".") + 1)
		if dot < 0 or name.contains("[") or name.contains("{") or name.contains("/"): return null
		var index := live.paths.find(parent)
		if index < 0: return null
		var value: Variant = live.objects[index].get(name)
		return value if value is Callable and Recorder.is_lambda(value) else null

## Restores a checkpoint into a freshly booted app (added to the tree, its
## _ready done, no logic frame run). observers maps names to the same
## observer-side objects as at capture.
static func restore(data: Dictionary, app: Node, observers: Dictionary) -> String:
	var roots := _roots(app)
	if Array(data.roots) != roots.keys(): return "checkpoint roots differ from the booted runtime"
	var live := _graph(roots)
	var live_by_path := {}
	for index in live.objects.size(): live_by_path[live.paths[index]] = live.objects[index]
	var decoder := Decoder.new()
	decoder.live = live
	var records: Array = data.objects
	# 1. A target per record: the live object at the same path when it is the
	# same script/class, otherwise a new instance (scripts without _init).
	for record: Dictionary in records:
		var existing: Variant = live_by_path.get(record.path)
		var target: Object = null
		if existing is Object and existing.get_class() == record["class"] and _script_identity(existing) == [record.script, record.subpath]:
			target = existing
		match str(record.kind):
			"engine":
				var replayed := Recorder.replay(record.log)
				if not replayed.error.is_empty(): return replayed.error
				target = replayed.engine
			"image":
				var spec: Array = record.image
				target = Image.create_from_data(spec[0], spec[1], spec[2], spec[3], spec[4])
			"weakref":
				target = null # set after all targets exist
			"script":
				if target == null:
					target = dict_to_inst({"@path": record.script, "@subpath": NodePath(record.subpath)})
					if target == null: return "cannot instantiate %s at %s" % [record.script, record.path]
			"resource":
				if target == null: target = load(record.resource_path)
			"native_resource":
				if target == null: target = ClassDB.instantiate(record["class"])
			"native":
				if target == null: return "native %s at %s has no live counterpart" % [record["class"], record.path]
		decoder.targets.append(target)
	for index in records.size():
		if records[index].kind == "weakref":
			var referenced: Variant = decoder.value(records[index].ref)
			decoder.targets[index] = weakref(referenced) if referenced is Object else WeakRef.new()
	# 2. Script variables (and storage of rebuilt native resources).
	for index in records.size():
		var record: Dictionary = records[index]
		if record.kind == "native_resource":
			# Decoded even when reused, so shared containers keep their order.
			var reused := is_same(decoder.targets[index], live_by_path.get(record.path))
			for name: String in record.storage:
				var stored: Variant = decoder.value(record.storage[name])
				if not reused: decoder.targets[index].set(name, stored)
			continue
		if record.kind != "script": continue
		var target: Object = decoder.targets[index]
		for name: String in record.props:
			target.set(name, decoder.value(record.props[name]))
	for item: Array in data.statics:
		var script: Script = load(item[0])
		script.set(item[1], decoder.value(item[2]))
	if not decoder.errors.is_empty(): return "; ".join(decoder.errors)
	# 3. Signal connections between graph objects, before any tree change:
	# removing a booted node emits its exit signals.
	for index in records.size():
		var source: Object = decoder.targets[index]
		if source == null: continue
		for sig: Dictionary in source.get_signal_list():
			for existing: Dictionary in source.get_signal_connection_list(sig.name):
				var callable: Callable = existing.callable
				if Recorder.is_lambda(callable) or not decoder.targets.has(callable.get_object()): continue
				source.disconnect(sig.name, callable)
	# Booted objects the checkpoint does not hold (the entry scene) must not
	# call into the restored graph, even while they are being freed.
	for orphan: Object in live.objects:
		if orphan == null or not is_instance_valid(orphan) or decoder.targets.has(orphan): continue
		for sig: Dictionary in orphan.get_signal_list():
			for existing: Dictionary in orphan.get_signal_connection_list(sig.name):
				var callable: Callable = existing.callable
				if callable.get_object() != null and decoder.targets.has(callable.get_object()): orphan.disconnect(sig.name, callable)
	for connection: Array in data.connections:
		var source: Object = decoder.targets[int(connection[0])]
		var callable: Callable = decoder.value(connection[2])
		if not source.is_connected(connection[1], callable): source.connect(connection[1], callable, connection[3])
	if not decoder.errors.is_empty(): return "; ".join(decoder.errors)
	# 4. Scene tree: each graph node keeps exactly its captured children, in order.
	for index in records.size():
		var record: Dictionary = records[index]
		if not record.has("children"): continue
		var node: Node = decoder.targets[index]
		var wanted: Array = []
		for child_index: int in record.children: wanted.append(decoder.targets[child_index])
		for child: Node in node.get_children():
			if not child in wanted:
				node.remove_child(child)
				child.queue_free()
		for position in wanted.size():
			var child: Node = wanted[position]
			if child.get_parent() != node:
				if child.get_parent() != null: child.get_parent().remove_child(child)
				child.name = records[record.children[position]].name
				node.add_child(child)
			node.move_child(child, position)
	for index in records.size():
		if records[index].has("processing"):
			var node: Node = decoder.targets[index]
			node.set_process(records[index].processing[0])
			node.set_physics_process(records[index].processing[1])
			var node_state: Array = records[index].node_state
			node.process_priority = node_state[0]
			node.process_physics_priority = node_state[1]
			node.process_mode = node_state[2]
			for group: StringName in node.get_groups():
				if not str(group).begins_with("_") and not str(group) in node_state[3]: node.remove_from_group(group)
			for group: String in node_state[3]:
				if not node.is_in_group(group): node.add_to_group(group, true)

	# 5. Observer-side plain variables.
	for name: String in observers:
		var saved: Dictionary = data.observers.get(name, {})
		for key: String in saved: observers[name].set(key, saved[key])
	return ""
