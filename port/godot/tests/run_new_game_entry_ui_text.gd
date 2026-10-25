extends SceneTree
const App=preload("res://app/new_game_entry_boot.gd")
const Diagnostics=preload("res://services/new_game_load_diagnostics.gd")
var checks: int=0
var failures: int=0
func check(value: bool,label: String) -> void:
	checks+=1
	if not value: printerr("FAIL "+label);failures+=1
func _initialize() -> void:
	check(App.profile_text({"month":0,"day":0,"blood":0})=="Birthday: 1/1   Blood type: A","human date and blood values")
	check(App.profile_text({"month":3,"day":2,"blood":2})=="Birthday: 4/3   Blood type: AB","no raw zero-based date")
	check(App.profile_text({"month":11,"day":30,"blood":3})=="Birthday: 12/31   Blood type: O","native O selection rendered correctly")
	check(App.profile_text({"month":255,"day":255,"blood":255})=="Birthday: Not set/Not set   Blood type: Not set","unset choices have a readable label")
	var app:=App.new();app.panel=VBoxContainer.new();app.add_child(app.panel);app.failure="Check the data pack.";app._render()
	var labels: Array=[]
	for child in app.panel.get_children():
		if child is Label: labels.append(child.text)
	check(labels==["New Game","Check the data pack."],"load error view is short and actionable")
	app.free()
	var path: String="/tmp/new-game-load-invented.json"
	var file: FileAccess=FileAccess.open(path,FileAccess.WRITE);file.store_string("{}");file.close()
	var report: Dictionary=Diagnostics.inspect(path,false)
	check(report.strict_path_valid and not report.accepted and report.size==2 and report.json_dictionary,"read-only diagnostics preserve loader rejection")
	check(report.has("resource_root") and report.has("sha256"),"load context supports exported-runtime diagnosis")
	var rejected: Dictionary=Diagnostics.inspect("/tmp/../tmp/new-game-load-invented.json",false)
	check(not rejected.strict_path_valid and not rejected.has("sha256"),"diagnostics never read a rejected noncanonical path")
	print("PASS: %d entry UI text/diagnostic checks"%checks if not failures else "FAIL entry UI text/diagnostics")
	quit(1 if failures else 0)
