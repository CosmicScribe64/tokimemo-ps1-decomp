extends SceneTree
const App = preload("res://app/first_arrival_boot.gd")
const Bgm = preload("res://services/first_arrival_bgm_bridge.gd")
func _initialize() -> void:
	var host := Node.new()
	root.add_child(host)
	var service = load("res://services/audio_service.gd").new()
	var result := Bgm.new().prepare(service,host,"/missing")
	print("BGM loader unavailable source rejected: ",result)
	host.free()
	quit(0 if not result.is_empty() else 1)
