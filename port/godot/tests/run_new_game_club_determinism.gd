extends SceneTree
## NAME_ENT club draws: determinism and source order.
##
## Installed runs r10 and r11 reached golden a4 with the same frame_count,
## system_data.seed and rand state, but different girls[1..4] clubs: the entry
## seeded the BIOS LCG from the host clock (Time.get_ticks_usec), and the
## prologue's srand(frame_count) later hid the difference. The entry now seeds
## like file_select_init_settings, srand(system_data.seed) (file_select.c:735).
##
## Checks:
## - the installed entry scene seeds from its model's system_data.seed;
## - two identical entry runs give identical girls[] records and rand traces;
## - the draws come in source order: name_finish x3 (name_entry.c:1132),
##   birthday_finalize month/day/blood for an unset player then Shiori
##   (name_entry.c:2023-2029, 1997-2003), then name_entry_new_game_setup's
##   girls[1], [2], [3], [4] draws (name_entry.c:2503-2533), skipping the girl
##   whose pair holds Shiori's club;
## - the golden set A seed and draw count reproduce the golden clubs.
const Entry = preload("res://flow/new_game_entry.gd")
const Rules = preload("res://rules/new_game.gd")
const Rng = preload("res://core/native_rng.gd")
const CoreState = preload("res://core/game_state.gd")
const Math = preload("res://core/native_math.gd")
const GIRL_SIZE := 0x38
const GIRLS_BASE := 0x1be
const CLUB_BYTE := 0x1ca
var checks: int = 0
var failures: int = 0

func check(value: bool, label: String) -> void:
	checks += 1
	if not value: printerr("FAIL " + label); failures += 1

func invented_tables() -> Dictionary:
	var t: Dictionary = {"compatibility":[],"initial_scores":[],"space":[250,251],"cheat_names":[],"girl_birthdays":[],"girl_bloods":[],"girl_fixed_clubs":[]}
	for i in 16: t.compatibility.append(i%4+1)
	for i in 20: t.initial_scores.append(i+1)
	for i in 9: t.cheat_names.append([240,i+1])
	for i in 14: t.girl_birthdays.append(1); t.girl_birthdays.append(i+1)
	for i in 12: t.girl_bloods.append(i%4)
	for i in 7: t.girl_fixed_clubs.append(i+4)
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

## One whole entry from a fresh model. `birthdays` is null for unset (rand)
## birthdays, or [month, day, blood] selections used for both profiles.
func run_entry(system_seed: int, birthdays: Variant) -> Dictionary:
	var model := CoreState.new()
	model.system_seed = system_seed
	var rng := Rng.new()
	check(rng.configure(Rng.OPENBIOS_PROFILE, Entry.file_select_seed(model)), "entry RNG seeds from system_data.seed")
	var entry := Entry.new()
	check(entry.begin(invented_tables(), invented_content(), rng).is_empty(), "entry begins")
	var actions: Array = []
	for part in 3: actions.append_array([{"kind":"tick"},{"kind":"cell","column":0,"row":0},{"kind":"start"}])
	var birthday: Array = [{"kind":"tick"}]
	if birthdays == null:
		birthday.append({"kind":"start"})
	else:
		for value: int in birthdays: birthday.append_array([{"kind":"select","value":value},{"kind":"circle"}])
	actions.append_array(birthday); actions.append_array([{"kind":"tick"},{"kind":"yes"}])
	actions.append_array(birthday); actions.append_array([{"kind":"tick"},{"kind":"yes"},{"kind":"tick"}])
	for action: Dictionary in actions:
		var error: String = entry.act(action)
		check(error.is_empty(), "entry action %s: %s" % [action, error])
	check(entry.stage == 2 and entry.step_id == 5, "entry reached name_entry_new_game_setup")
	var girls: Array = []
	for i in 13: girls.append(entry.state_bytes.slice(GIRLS_BASE + i * GIRL_SIZE, GIRLS_BASE + (i + 1) * GIRL_SIZE))
	return {"girls": girls, "trace": rng.trace.duplicate(true), "state": rng.state, "bytes": entry.state_bytes}

func club(bytes: PackedByteArray, girl: int) -> int:
	return bytes[CLUB_BYTE + girl * GIRL_SIZE] >> 4

## Independent OpenBIOS LCG (rand = (seed * 0x41C64E6D + 0x3039) >> 16 & 0x7FFF).
func lcg_draws(seed: int, count: int) -> Array[int]:
	var out: Array[int] = []
	var state: int = Math.u32(seed)
	for i in count:
		state = Math.u32(state * 0x41c64e6d + 0x3039)
		out.append((state >> 16) & 0x7fff)
	return out

func _initialize() -> void:
	# The installed entry no longer reads a host clock for its seed.
	var boot: String = FileAccess.get_file_as_string("res://app/new_game_entry_boot.gd")
	check(boot.contains("seed = str(Entry.file_select_seed(_entry_model))"), "installed entry seeds srand(system_data.seed) from its model")
	check(not boot.contains("get_ticks_usec"), "installed entry does not seed from the host clock")
	var model := CoreState.new()
	model.system_seed = -1
	check(Entry.file_select_seed(model) == 0xffffffff, "system_data.seed is an unsigned 32-bit word")

	# Determinism: identical runs give identical girls[] records and traces.
	for setup: Array in [[0, null], [0x3ef2fd, null], [0, [0,0,0]], [0x3ef2fd, [0,0,0]], [0x377 * 77, [5,14,2]]]:
		var first: Dictionary = run_entry(setup[0], setup[1])
		var second: Dictionary = run_entry(setup[0], setup[1])
		check(first.girls == second.girls, "seed %d birthdays %s: identical girls[] records" % setup)
		check(first.trace == second.trace and first.state == second.state, "seed %d birthdays %s: identical rand traces" % setup)
		check(first.bytes == second.bytes, "seed %d birthdays %s: identical new-game state" % setup)

	# Source order, unset birthdays: 3 name finishes, player then Shiori birthday,
	# then the girls[1..4] club draws not forced by Shiori's club.
	var seed: int = 0x377 * 4651
	var unset: Dictionary = run_entry(seed, null)
	var draws: Array[int] = lcg_draws(seed, unset.trace.size())
	var expected: Array[String] = ["name_entry_name_finish:family", "name_entry_name_finish:given", "name_entry_name_finish:nickname"]
	for who in ["player", "shiori"]:
		for field in ["month", "day", "blood"]: expected.append("name_entry_birthday_finalize:%s:%s" % [who, field])
	var month: int = draws[6] % 12
	var day: int = draws[7] % 31
	if month == 1 and day >= 28: day = 27
	if month in [3,5,8,10] and day >= 30: day = 29
	var shiori_club: int = ((day + 1) * 3 + month + 1) % 11
	check(club(unset.bytes, 0) == shiori_club, "Shiori's club follows her drawn birthday (name_entry.c:2491)")
	for girl in range(1, 5):
		if shiori_club not in [(girl - 1) * 2, (girl - 1) * 2 + 1]: expected.append("name_entry_new_game_setup:girl%d" % girl)
	var callers: Array[String] = []
	var results: Array[int] = []
	for record: Dictionary in unset.trace: callers.append(str(record.caller)); results.append(int(record.result))
	check(callers == expected, "rand draws come in source order: %s" % [callers])
	check(results == draws, "draw results follow the OpenBIOS LCG from srand(system_data.seed)")
	for girl in range(1, 5):
		var low: int = (girl - 1) * 2
		var want: int = low + 1 if shiori_club == low else (low if shiori_club == low + 1 else -1)
		if want < 0:
			var index: int = callers.find("name_entry_new_game_setup:girl%d" % girl)
			want = low + 1 if (results[index] & 1) != 0 else low
		check(club(unset.bytes, girl) == want, "girls[%d] club from its own draw (name_entry.c:2497-2533)" % girl)

	# Golden set A: srand(0x3EF2FD), 13 draws to the Shiori confirm, then the
	# club draws 14-16 = 27171/20405/27159; Jan 1 / A gives Shiori club 4, so
	# girls[3] is forced and girls[1]/[2]/[4] are DRAMA (1), COMPUTER (3), SOCCER (7).
	var golden := Rng.new()
	golden.configure(Rng.OPENBIOS_PROFILE, 0x3ef2fd)
	for i in 13: golden.next("golden_set_a:prior_draw")
	var previous := PackedByteArray(); previous.resize(Rules.SIZE)
	var names: Dictionary = {"family":PackedByteArray([1,2,250,251,250,251]),"given":PackedByteArray([1,2,250,251,250,251]),"nickname":PackedByteArray([1,2,250,251,250,251,250,251,250,251])}
	var profile: Dictionary = {"month":0,"day":0,"blood":0}
	var outcome: Rules.Outcome = Rules.setup(previous, names, profile, profile.duplicate(), invented_tables(), golden)
	check(outcome.error.is_empty() and outcome.rng_draws == 3, "golden set A commit draws three clubs")
	var golden_results: Array[int] = []
	for record: Dictionary in golden.trace.slice(13): golden_results.append(int(record.result))
	check(golden_results == [27171, 20405, 27159], "golden set A club draws 14-16: %s" % [golden_results])
	check([club(outcome.bytes, 0), club(outcome.bytes, 1), club(outcome.bytes, 2), club(outcome.bytes, 3), club(outcome.bytes, 4)] == [4, 1, 3, 5, 7], "golden set A clubs: Shiori 4, girls[1..4] 1/3/5/7")

	if failures == 0: print("PASS: %d NAME_ENT club determinism checks" % checks)
	else: printerr("FAIL: %d of %d NAME_ENT club determinism checks" % [failures, checks])
	quit(1 if failures else 0)
