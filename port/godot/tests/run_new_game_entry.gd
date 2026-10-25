extends SceneTree
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
var checks: int = 0
var failures: int = 0
func check(value: bool, label: String) -> void:
	checks += 1
	if not value: printerr("FAIL "+label); failures += 1
func invented_tables() -> Dictionary:
	var t: Dictionary = {"compatibility":[],"initial_scores":[],"space":[250,251],"cheat_names":[],"girl_birthdays":[],"girl_bloods":[],"girl_fixed_clubs":[]}
	for i in 16: t.compatibility.append(i%4+1)
	for i in 20: t.initial_scores.append(i+1)
	for i in 9: t.cheat_names.append([240,i+1])
	for i in 14: t.girl_birthdays.append(1); t.girl_birthdays.append(i+1)
	for i in 12: t.girl_bloods.append(i%4)
	for i in 7: t.girl_fixed_clubs.append(i)
	return t
func invented_content() -> Dictionary:
	var c: Dictionary = {"space":[250,251],"hiragana":[],"katakana":[],"preset_family":[],"preset_given":[],"preset_nickname":[]}
	for key in ["hiragana","katakana"]:
		for i in 17: c[key].append([1,2,3,4,5,6,7,8,9,10])
	for key in ["preset_family","preset_given","preset_nickname"]:
		for i in 15:
			var row: Array = [33,i+1,34,i+1,35,i+1]
			if key == "preset_nickname": row.append_array([36,i+1,37,i+1])
			row.append_array([0,0]); c[key].append(row)
	return c
func action(entry: Entry, kind: String) -> void:
	check(entry.act({"kind":kind}).is_empty(),"action "+kind)
func finish_names(entry: Entry, rng: Rng, entered: bool) -> void:
	for i in 3:
		action(entry,"tick")
		if entered: check(entry.act({"kind":"cell","column":0,"row":0}).is_empty(),"insert original-grid pair")
		var before: int = rng.draw_count
		action(entry,"done")
		check(rng.draw_count == before+1,"name finish always draws at its action site")
func _initialize() -> void:
	var rng := Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE,123)
	var entry := Entry.new()
	check(entry.begin(invented_tables(),invented_content(),rng).is_empty(),"independent entry begins without captured state")
	check(rng.draw_count == 0 and entry.player.month == 255 and not entry.owned_projection().command_ready and entry.known_bytes.count(255) == 3823,"begin consumes no randomization and marks 2973 unowned placeholder bytes unknown")
	finish_names(entry,rng,true)
	check(rng.draw_count == 3,"three entered-name finish draws")
	action(entry,"tick")
	check(entry.step_id == 7,"native player birthday action site")
	action(entry,"start")
	check(rng.draw_count == 6 and entry.shiori.month == 255,"player finalized before Shiori step and before commit")
	var retained: Dictionary = entry.player.duplicate()
	action(entry,"tick"); action(entry,"no")
	check(entry.stage == 1 and entry.step_id == 0 and entry.player == retained,"player reject returns to retained name-entry init")
	finish_names(entry,rng,true); action(entry,"tick")
	var before: int = rng.draw_count
	action(entry,"start")
	check(rng.draw_count == before,"retained birthday does not randomize again")
	action(entry,"tick"); action(entry,"yes"); action(entry,"tick")
	check(entry.stage == 2 and entry.step_id == 1,"native Shiori birthday action site")
	before = rng.draw_count
	action(entry,"start")
	check(rng.draw_count == before+3,"Shiori finalized at its UI action")
	action(entry,"tick"); action(entry,"yes")
	before = rng.draw_count
	action(entry,"tick")
	check(entry.step_id == 5 and entry.state_bytes.size() == Rules.SIZE,"native commit returns full independent logical projection")
	check(rng.draw_count-before in [3,4],"commit consumes only conditional club RNG")
	check(not entry.blocked.is_empty(),"prologue dependency stop explicit")
	check(entry.act({"kind":"tick"}) == entry.blocked,"unsupported prologue cannot silently advance")
	var other_rng := Rng.new(); other_rng.configure(Rng.OPENBIOS_PROFILE,5)
	var blank := Entry.new(); blank.begin(invented_tables(),invented_content(),other_rng)
	finish_names(blank,other_rng,false)
	check(blank.names.family[0] == 33 and blank.names.nickname[8] == 37,"blank names use external native presets")
	var untouched: PackedByteArray = blank.state_bytes.duplicate(); before = other_rng.draw_count
	check(not blank.act({"kind":"cell","column":false,"row":0}).is_empty() and blank.state_bytes == untouched and other_rng.draw_count == before,"invalid action cannot mutate state/RNG")
	var empty := Entry.new()
	check(not empty.begin({},invented_content(),other_rng).is_empty() and other_rng.draw_count == before,"missing rules block before RNG")
	check(Entry.load_content("relative.json").is_empty() and Entry.load_content("res://entry.json").is_empty(),"entry pack rejects relative/resource paths")
	# Source: family/given input's DONE versus START branches, followed by
	# given/nickname init's conditional NAME_ENTRY_TAB_DONE reset.
	for finish_kind in ["done","start"]:
		var tab_rng := Rng.new(); tab_rng.configure(Rng.OPENBIOS_PROFILE,7)
		var tab_entry := Entry.new(); tab_entry.begin(invented_tables(),invented_content(),tab_rng)
		action(tab_entry,"tick")
		for next_part in ["given","nickname"]:
			check(tab_entry.act({"kind":"tab","value":1}).is_empty(),"select katakana before "+finish_kind)
			var draws: int = tab_rng.draw_count
			action(tab_entry,finish_kind)
			check(tab_entry.tab == (4 if finish_kind == "done" else 1) and tab_rng.draw_count == draws+1,"DONE sets native tab4; START preserves tab; each finishes with one draw")
			action(tab_entry,"tick")
			check(tab_entry.active_part() == next_part and tab_entry.tab == (0 if finish_kind == "done" else 1),"next name init resets only native DONE tab")
	print("PASS: %d New Game entry checks"%checks if not failures else "FAIL entry checks")
	quit(1 if failures else 0)
