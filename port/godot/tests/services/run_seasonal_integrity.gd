extends SceneTree
## Original content load followed by one adversarial reopen; no gameplay claim.
const Store = preload("res://services/seasonal_command_content_store.gd")
func _initialize() -> void:
	var original := OS.get_environment("SEASONAL_ORIGINAL")
	var rejected := OS.get_environment("SEASONAL_REJECTED")
	var store := Store.new()
	if not store.open_pack(original,Store.MANIFEST_SHA):
		printerr("original seasonal pack failed: ",store.error_message()); quit(2); return
	var bg: String = store._backgrounds.keys()[0]
	var sd: String = store._sd_sets.keys()[0]
	if store.get_background(bg)==null or store.get_sd_resources(sd).is_empty():
		printerr("original seasonal images/resources missing");quit(2);return
	# Supplying the mutated manifest's hash isolates descriptor/payload admission
	# from the separate immutable-manifest hash gate.
	if store.open_pack(rejected,FileAccess.get_sha256(rejected.path_join("manifest.json"))):
		printerr("adversarial seasonal pack accepted");quit(1);return
	if not store._images.is_empty() or store.get_background(bg)!=null or not store.get_sd_resources(sd).is_empty():
		printerr("failed seasonal reopen retained prior resources");quit(1);return
	print("Seasonal payload/descriptor rejection and clearing passed")
	quit()
