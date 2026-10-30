extends SceneTree
const Pointer = preload("res://flow/name_entry_pointer.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Entry = preload("res://flow/new_game_entry.gd")
var failures: Array[String] = []
func check(value: bool, label: String) -> void:
	if not value: failures.append(label)
func _initialize() -> void:
	var pointer := Pointer.new()
	pointer.advance(InputFrame.new(0,0x2000,0))
	check(pointer.position == Vector2i(163,120),"normal speed3")
	pointer.advance(InputFrame.new(1,0x2020,0))
	check(pointer.position == Vector2i(169,120),"held face button doubles speed")
	pointer.advance(InputFrame.new(2,0x2001,0))
	check(pointer.position == Vector2i(169,120),"shoulder inhibits movement")
	pointer.advance(InputFrame.new(3,0xa000,0))
	check(pointer.position == Vector2i(169,120),"opposite directions cancel")
	pointer.place(Vector2i(-100,500))
	check(pointer.position == Vector2i(10,220),"source cursor clamp")
	for index: int in 19: pointer.advance(InputFrame.new(index,0x80,0))
	check(pointer.style == 0,"Square style cycle returns to default")
	var entry := Entry.new(); entry.stage=1; entry.step_id=1
	for tab: int in 4:
		pointer.place(Vector2i(16,80+tab*32))
		check(pointer.confirm(entry) == {"kind":"tab","value":tab},"all four source tabs reachable")
	entry.tab=3
	pointer.place(Vector2i(55,96))
	pointer.advance(InputFrame.new(20,0x2020,0))
	check(pointer.confirm(entry) == {"kind":"cell","column":1,"row":1},"simultaneous direction and Circle resolve moved cursor")
	pointer.place(Vector2i(300,144)); check(pointer.confirm(entry) == {"kind":"scroll","delta":1},"right edge down arrow")
	pointer.place(Vector2i(64,184)); check(pointer.confirm(entry) == {"kind":"reading","column":1,"row":0},"reading selector")
	pointer.place(Vector2i(48,96)); check(pointer.confirm(entry) == {"kind":"cell","column":0,"row":1},"candidate cell")
	pointer.place(Vector2i(64,42)); check(pointer.confirm(entry) == {"kind":"slot","value":0},"active name slot")
	print("NAME native pointer checks: ",failures)
	quit(0 if failures.is_empty() else 1)
