extends SceneTree
## Synthetic object graph through tests/checkpoint/runtime_checkpoint.gd:
## capture, restore into a freshly built graph (as a booted app would be),
## then the restored graph must capture to the same data and keep its sharing,
## typed containers, weak references, callables, images and inner classes.
## Not route evidence. Run: godot --headless --path port/godot --script res://tests/checkpoint/run_checkpoint_tests.gd

const Checkpoint = preload("res://tests/checkpoint/runtime_checkpoint.gd")

class Leaf extends RefCounted:
	var value: int = 0
	var label := ""
	func _init(required: int) -> void:
		value = required
	func bump(amount: int) -> int:
		value += amount
		return value

class Holder extends RefCounted:
	var shared: Dictionary = {}
	var events: Array[Dictionary] = []
	var leaf: Leaf
	var weak: WeakRef
	var action: Callable
	var picture: Image
	var nested: Dictionary = {}
	var numbers := PackedInt32Array()
	var floaty := 1.0

class Root extends Node:
	var holder := Holder.new()
	var other := Holder.new()
	var boot_leaf := Leaf.new(1)
	var lambda_slot: Callable = func() -> int: return 7
	var counter := 0

var failures: PackedStringArray = []

func _check(ok: bool, message: String) -> void:
	if not ok: failures.append(message)

func _built() -> Root:
	var root := Root.new()
	var holder := root.holder
	var shared := {"k": [1, 2.5, "s"], "bytes": PackedByteArray([1, 2, 3])}
	holder.shared = shared
	root.other.shared = shared
	holder.events.append({"kind": "a", "n": 1})
	holder.leaf = Leaf.new(5)
	holder.leaf.label = "late"
	root.other.leaf = holder.leaf
	holder.weak = weakref(root.boot_leaf)
	holder.action = holder.leaf.bump.bind(2)
	holder.picture = Image.create(4, 3, false, Image.FORMAT_RGBA8)
	holder.picture.fill(Color8(10, 20, 30))
	holder.nested = {"inner": {"deep": [shared]}}
	holder.numbers = PackedInt32Array([3, -4])
	holder.floaty = 2.0
	root.counter = 41
	return root

func _initialize() -> void:
	var original := _built()
	var captured := Checkpoint.capture(original, {}, {"tick": 1})
	_check(captured.error.is_empty(), "capture: " + str(captured.error))
	var data: Dictionary = bytes_to_var(var_to_bytes(captured.data))
	var fresh := Root.new()
	var error := Checkpoint.restore(data, fresh, {})
	_check(error.is_empty(), "restore: " + error)
	if error.is_empty():
		var again := Checkpoint.capture(fresh, {}, {"tick": 1})
		_check(var_to_bytes(again.data.objects) == var_to_bytes(data.objects), "restored graph captures to the same records")
		_check(fresh.holder.shared == original.holder.shared and is_same(fresh.holder.shared, fresh.other.shared), "shared dictionary stays shared")
		_check(is_same(fresh.holder.nested.inner.deep[0], fresh.holder.shared), "nested alias stays shared")
		_check(fresh.holder.events.is_typed() and fresh.holder.events.get_typed_builtin() == TYPE_DICTIONARY, "typed array keeps its type")
		_check(fresh.holder.leaf != null and fresh.holder.leaf.value == 5 and fresh.holder.leaf.label == "late" and is_same(fresh.holder.leaf, fresh.other.leaf), "inner class without _init args restored once")
		_check(fresh.holder.weak.get_ref() == fresh.boot_leaf, "weak reference targets the restored object")
		_check(fresh.holder.action.call() == 7 and fresh.holder.leaf.value == 7, "bound callable targets the restored object")
		_check(fresh.holder.picture.get_data() == original.holder.picture.get_data(), "image bytes")
		_check(typeof(fresh.holder.floaty) == TYPE_FLOAT and fresh.holder.numbers == PackedInt32Array([3, -4]), "types are exact")
		_check(fresh.lambda_slot.call() == 7, "lambda comes from the booted graph")
		_check(fresh.counter == 41, "plain variable")
	original.free()
	fresh.free()
	if failures.is_empty():
		print("PASS: runtime checkpoint synthetic graph")
		quit(0)
	else:
		for failure in failures: printerr("FAIL: ", failure)
		quit(1)
