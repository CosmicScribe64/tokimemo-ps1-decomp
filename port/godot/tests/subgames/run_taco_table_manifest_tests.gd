extends SceneTree
## T-29024: the manifests written by tools/port/taco_tables.py open through the
## authenticated loaders, and altered copies do not. Needs TACO_COMBAT_TABLES and
## TACO_ENEMY_TABLES (absolute paths to that producer's output); absence fails.
## Bounded: table identity only, no shooter stage or route acceptance.
const Combat = preload("res://services/shooter_combat_tables.gd")
const Enemy = preload("res://services/shooter_enemy_tables.gd")
var failures: Array[String] = []
var checks: int = 0

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures.append(label)

func _tampered(path: String, out: String) -> String:
	var raw: PackedByteArray = FileAccess.get_file_as_bytes(path)
	# Change one table digit; the JSON stays well-formed.
	var text: String = raw.get_string_from_utf8().replace("\"damage_by_kind\":[1,", "\"damage_by_kind\":[2,").replace("\"radius\":[3,", "\"radius\":[4,")
	var file: FileAccess = FileAccess.open(out, FileAccess.WRITE)
	file.store_string(text)
	file.close()
	return out

func _initialize() -> void:
	var combat_path: String = OS.get_environment("TACO_COMBAT_TABLES")
	var enemy_path: String = OS.get_environment("TACO_ENEMY_TABLES")
	check(not combat_path.is_empty() and not enemy_path.is_empty(), "producer manifests are supplied")
	var combat: Combat = Combat.new()
	var enemy: Enemy = Enemy.new()
	check(combat.open_manifest(combat_path).is_empty(), "combat manifest authenticates: " + combat.error)
	check(enemy.open_manifest(enemy_path).is_empty(), "enemy manifest authenticates: " + enemy.error)
	if failures.is_empty():
		# Independent facts from wiki/spec/o-taco.md section 7: vulcan shots are
		# kind 4 with damage 6; the score table is indexed modulo 17.
		check(combat.tables["damage_by_kind"][4] == 6, "vulcan kind 4 damage is 6")
		check(combat.tables["score_by_class"].size() == 17, "score table has 17 classes")
		check(combat.tables["se_cooldown_range"].size() == 10, "cooldown covers source ids below 10")
		# rsin quarter table: 0 at 0 and 4096 (1.0) at 1024.
		check(enemy.tables["sine_quarter"][0] == 0 and enemy.tables["sine_quarter"][1024] == 4096, "sine quarter endpoints")
		check(enemy.tables["keys_d"][1] < enemy.tables["joint_count"][13], "class13 key script stays within its joints")
	# Negatives: altered bytes, project paths and relative paths are refused.
	var bad: Combat = Combat.new()
	check(not bad.open_manifest(_tampered(combat_path, "/tmp/taco-combat-tampered.json")).is_empty() and bad.tables.is_empty(), "altered combat manifest refused")
	var bad_enemy: Enemy = Enemy.new()
	check(not bad_enemy.open_manifest(_tampered(enemy_path, "/tmp/taco-enemy-tampered.json")).is_empty() and bad_enemy.tables.is_empty(), "altered enemy manifest refused")
	check(not Combat.new().open_manifest("res://services/shooter_combat_tables.gd").is_empty(), "project path refused")
	check(not Enemy.new().open_manifest("tables/enemy-tables.json").is_empty(), "relative path refused")
	check(not Enemy.new().open_manifest(combat_path).is_empty(), "combat manifest cannot stand in for the enemy manifest")
	print(JSON.stringify({"checks": checks, "failures": failures, "scope": "TACO table identity only"}))
	quit(0 if failures.is_empty() else 1)
