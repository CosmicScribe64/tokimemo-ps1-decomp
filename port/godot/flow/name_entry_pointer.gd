extends RefCounted
## Source hud_cursor_update and NAME hit tests, in320×240 screen coordinates.
## Desktop origin is explicit; this does not reconstruct the prior FILESEL cursor.
var position := Vector2i(160,120)
var style := 0
var held := 0

func place(value: Vector2i) -> bool:
	var prior := position
	position = Vector2i(clampi(value.x,10,300),clampi(value.y,20,220))
	return position != prior

func advance(input: RefCounted, menu_mode: int = 0) -> bool:
	var prior := position
	var previous_style := style
	held = input.held
	var speed := 6 if (menu_mode & 3) == 0 and (held & 0xff) != 0 else 3
	if (held & 0xf) == 0:
		var movement := Vector2i.ZERO
		if (held & 0x5000) == 0x1000: movement.y = -speed
		elif (held & 0x5000) == 0x4000: movement.y = speed
		if (held & 0xa000) == 0x2000: movement.x = speed
		elif (held & 0xa000) == 0x8000: movement.x = -speed
		place(position+movement)
	if input.pressed & 0x80:
		style = 155 if style == 0 else (0 if style == 172 else style+1)
	return prior != position or previous_style != style

func hit(entry: RefCounted) -> Dictionary:
	var result := {"tab":-1,"cell":Vector2i(-1,-1),"reading":Vector2i(-1,-1),"arrow":0,"slot":-1}
	if entry.stage != 1 or entry.step_id not in [1,3,5]: return result
	for index: int in 5:
		if position.x >= 3 and position.x < 32 and position.y >= 72+index*32 and position.y < 87+index*32: result.tab = index
	if position.x >= 41 and position.x < 312 and position.y >= 73 and position.y < 168:
		result.cell = Vector2i((position.x-40)/16,(position.y-72)/16)
		if entry.tab in [2,3] and result.cell.x == 16: result.cell = Vector2i(-1,-1)
	if entry.tab in [2,3] and position.x >= 297 and position.x < 312:
		if position.y >= 73 and position.y < 88: result.arrow = -1
		elif position.y >= 137 and position.y < 152: result.arrow = 1
	if entry.tab == 3 and position.x >= 41 and position.x < 312 and position.y >= 177 and position.y < 224:
		result.reading = Vector2i((position.x-40)/16,(position.y-176)/16)
	var part: int = (entry.step_id-1)/2
	for index: int in (5 if part == 2 else 3):
		if Rect2i([57,113,233][part]+16*index,35,15,15).has_point(position): result.slot = [0,3,6][part]+index
	return result

func confirm(entry: RefCounted) -> Dictionary:
	var target := hit(entry)
	if target.slot >= 0: return {"kind":"slot","value":target.slot}
	if target.tab >= 0: return {"kind":"done"} if target.tab == 4 else {"kind":"tab","value":target.tab}
	if target.arrow != 0: return {"kind":"scroll","delta":target.arrow}
	if target.reading.x >= 0: return {"kind":"reading","column":target.reading.x,"row":target.reading.y}
	if target.cell.x >= 0: return {"kind":"cell","column":target.cell.x,"row":target.cell.y}
	return {}

func snapshot() -> Dictionary:
	return {"position":[position.x,position.y],"style":style,"held":held,
		"origin_policy":"desktop-center; prior native FILESEL cursor unavailable","speed":3,"menu_mode":0}
