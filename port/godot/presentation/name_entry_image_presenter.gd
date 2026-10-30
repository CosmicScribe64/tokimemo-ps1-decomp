extends RefCounted
## T-39004: NAME_ENT source actors, background pages and live profile values.
## Native sequence state is advanced by the entry owner, never by read-only render.
const Store = preload("res://services/name_entry_image_store.gd")
const Markers = preload("res://presentation/name_entry_markers.gd")
const ORIGIN := Vector2i(160,120)
var media := Store.new()
var translations: RefCounted
var text_renderer: RefCounted
var replaced_frames: Array[int] = []
var last_draw_records: Array[Dictionary] = []
var markers := Markers.new()
var pointer: RefCounted

func advance_markers(entry: RefCounted) -> bool:
	if not media.manifest.has("marker_sequence"): return false
	return markers.advance(entry)

func _record(kind: String, values: Dictionary) -> void:
	values["kind"] = kind
	values["draw_sequence"] = last_draw_records.size()
	last_draw_records.append(values)

func open_pack(path: String, expected: String, glyph_renderer: RefCounted) -> bool:
	text_renderer = glyph_renderer
	return media.open_pack(path, expected)

func _actor(target: Image, frame: int, position := Vector2i.ZERO) -> void:
	var row: Dictionary = media.manifest.frames[str(frame)]
	var native: Image = media.images[row.png.path]
	var image: Image = media.frame_image(frame, translations)
	if image != native and frame not in replaced_frames: replaced_frames.append(frame)
	var anchor := ORIGIN + position + Vector2i(row.bounds[0],row.bounds[1]) - Vector2i(row.origin[0],row.origin[1])
	_record("actor", {"frame":frame,"position":[position.x,position.y],
		"bounds":[anchor.x,anchor.y,image.get_width(),image.get_height()],
		"translated":image != native})
	target.blend_rect(image, Rect2i(Vector2i.ZERO,image.get_size()), anchor)

func _text(target: Image, raw: String, position: Vector2i, attribute: int) -> void:
	_record("text", {"raw_hex":raw,"position":[position.x,position.y],"attribute":attribute,"advance":16})
	# NAME_ENT uses sixteen-pixel character cells; the HUD helper advances fourteen.
	for offset: int in range(0,raw.length(),4):
		var pair := raw.substr(offset,4)
		if pair == "0000": break
		text_renderer._draw_text_hex_checked(target,pair,position+Vector2i(offset/4*16,0),attribute,"name_entry")

func _number(target: Image, value: int, position: Vector2i, attribute: int) -> void:
	if value == 255: return
	value += 1
	if value >= 10: _text(target,"82%02x" % (0x4f+value/10),position,attribute)
	_text(target,"82%02x" % (0x4f+value%10),position+Vector2i(16,0),attribute)

func _blood(target: Image, blood: int, position: Vector2i, attribute: int) -> void:
	if blood == 255: return
	var codes: Array[String] = ["8260","8261","82608261","826e"]
	_text(target,codes[blood],position+Vector2i(0 if blood == 2 else 16,0),attribute)

func _panel(target: Image, rect: Rect2i, alternate: bool) -> void:
	# prim_gradient_rect case 3 uses r*8,g*8,g*8 and 50/50 five-bit blending.
	var foreground := Vector3i(9,0,0) if alternate else Vector3i(8,8,8)
	var clipped := rect.intersection(Rect2i(0,0,320,240))
	_record("panel", {"bounds":[clipped.position.x,clipped.position.y,clipped.size.x,clipped.size.y],"alternate":alternate,"blend":"source-case3-half"})
	for y: int in range(clipped.position.y,clipped.end.y):
		for x: int in range(clipped.position.x,clipped.end.x):
			var old := target.get_pixel(x,y)
			target.set_pixel(x,y,Color8(((old.r8>>3)/2+foreground.x/2)*8,((old.g8>>3)/2+foreground.y/2)*8,((old.b8>>3)/2+foreground.z/2)*8))

func _digit(target: Image, digit: int, kind: String, position: Vector2i) -> void:
	var image: Image = media.images[media.manifest.digits["%s-%d" % [kind,digit]].path]
	_record("digit", {"digit":digit,"variant":kind,"bounds":[position.x,position.y,image.get_width(),image.get_height()]})
	target.blend_rect(image,Rect2i(Vector2i.ZERO,image.get_size()),position)

func _day_number(target: Image, number: int, position: Vector2i) -> void:
	_digit(target,number/10,"small",position-Vector2i(8,0))
	_digit(target,number%10,"small",position)

func render(entry: RefCounted) -> Image:
	last_draw_records.clear()
	if media.manifest.is_empty(): return null
	replaced_frames.clear()
	var image := Image.create(320,240,false,Image.FORMAT_RGBA8)
	image.blit_rect(media.images[media.manifest.base_background.png.path],Rect2i(0,0,320,240),Vector2i.ZERO)
	_record("base_background", {"bounds":[0,0,320,240],"source":"NAME_SAK.BS"})
	# prim_bg_slot_setup uploads rows at v0/v128; its map samples contiguous v0..239.
	for index: int in range(6):
		var page: Image = media.images[media.manifest.backgrounds[index].png.path]
		var mask: Image = media.images[media.manifest.backgrounds[index].stp_png.path]
		_record("background", {"page":index,"bounds":[index%3*128,index/3*128,128,128]})
		for y: int in range(mini(128,240-index/3*128)):
			for x: int in range(mini(128,320-index%3*128)):
				var color := page.get_pixel(x,y)
				if color.a == 0: continue
				var position := Vector2i(index%3*128+x,index/3*128+y)
				if mask.get_pixel(x,y).r8 != 0:
					var old := image.get_pixelv(position)
					color = Color8(((old.r8>>3)/2+(color.r8>>3)/2)*8,((old.g8>>3)/2+(color.g8>>3)/2)*8,((old.b8>>3)/2+(color.b8>>3)/2)*8)
				image.set_pixelv(position,color)
	var alternate: bool = entry.stage == 2
	var names: bool = entry.stage == 1 and entry.step_id < 6
	var birthday: bool = (entry.stage == 1 and entry.step_id in [6,7]) or (entry.stage == 2 and entry.step_id < 2)
	var hit: Dictionary = pointer.hit(entry) if pointer != null else {}
	if names:
		_panel(image,Rect2i(6,14,304,32),false)
		_panel(image,Rect2i(33,56,285,112),false)
		if entry.tab == 3:
			for x: int in [38,134,230]: _panel(image,Rect2i(x,174,80,48),false)
		for index: int in range(5):
			_panel(image,Rect2i(3,72+32*index,27,15),false)
			if entry.tab == index: _actor(image,7,Vector2i(-143,-40+index*32))
			if hit.get("tab",-1) == index: _actor(image,9,Vector2i(-143,-40+index*32))
			_actor(image,15+index*2,Vector2i(-143,-40+index*32))
		_actor(image,0)
		var marker_x: Array[int] = [-96,-80,-64,-40,-24,-8,80,96,112,128,144]
		for index: int in marker_x.size(): _actor(image,markers.frames[index],Vector2i(marker_x[index],-74))
		_actor(image,3,Vector2i(-112,-60))
		for key: String in ["family","given","nickname"]:
			_text(image,entry.names.get(key,PackedByteArray()).hex_encode(),Vector2i({"family":56,"given":112,"nickname":232}[key],32),14)
		_text(image,media.manifest.prompts[entry.active_part()],Vector2i(8,16),0 if entry.tab == 0 else 10)
		if not hit.is_empty():
			if hit.cell.x >= 0: _actor(image,4,Vector2i(hit.cell.x*16-113,hit.cell.y*16-41))
			elif hit.reading.x >= 0 and hit.reading.x%6 != 5: _actor(image,4,Vector2i(hit.reading.x*16-113,hit.reading.y*16+63))
		for row: int in range(6):
			for column: int in range(17):
				_text(image,entry.cell(column,row).hex_encode(),Vector2i(40+column*16,72+row*16),10)
		if entry.tab in [2,3]:
			var arrow: int = hit.get("arrow",0)
			if arrow != 0 and pointer.held & 0x20: _actor(image,8,Vector2i(136,-40 if arrow == -1 else 24))
			_actor(image,26 if arrow == -1 else 25,Vector2i(136,-40))
			_actor(image,28 if arrow == 1 else 27,Vector2i(136,24))
		if entry.tab == 3:
			for row: int in range(3):
				for column: int in range(17):
					_text(image,entry.reading_cell(column,row).hex_encode(),Vector2i(40+column*16,176+row*16),10)
	elif birthday:
		_panel(image,Rect2i(6,14,304,32),alternate)
		_panel(image,Rect2i(36,59,280,158),alternate)
		_panel(image,Rect2i(3,200,28,16),alternate)
		_actor(image,1)
		_actor(image,23,Vector2i(-143,88))
		for x: int in [-72,-56,-24,-8,104,120]: _actor(image,13,Vector2i(x,-74))
		_text(image,media.manifest.prompts.birthday,Vector2i(8,16),10)
		_number(image,entry.profile().month,Vector2i(80,32),14)
		_number(image,entry.profile().day,Vector2i(128,32),14)
		_blood(image,entry.profile().blood,Vector2i(256,32),14)
		for digit: int in range(1,10): _digit(image,digit,"wide",Vector2i(48+(digit-1)*24,128))
		for index: int in range(2): _day_number(image,index+10,Vector2i(272+index*24,128))
		for index: int in range(11): _day_number(image,index+12,Vector2i(56+index*24,144))
		for index: int in range(9): _day_number(image,index+23,Vector2i(56+index*24,160))
	else:
		_panel(image,Rect2i(34,59,282,88 if alternate else 118),alternate)
		_panel(image,Rect2i(120,179,124,38),alternate)
		_actor(image,14 if alternate else 2)
		_text(image,media.manifest.prompts.confirm,Vector2i(136,184),10)
		if alternate:
			_text(image,media.manifest.prompts.shiori_family,Vector2i(104,64),10)
			_text(image,media.manifest.prompts.shiori_given,Vector2i(160,64),10)
		else:
			for key: String in ["family","given","nickname"]:
				_text(image,entry.names.get(key,PackedByteArray()).hex_encode(),{"family":Vector2i(104,65),"given":Vector2i(160,65),"nickname":Vector2i(104,97)}[key],10)
		_number(image,entry.profile().month,Vector2i(104,96 if alternate else 128),10)
		_number(image,entry.profile().day,Vector2i(152,96 if alternate else 128),10)
		_blood(image,entry.profile().blood,Vector2i(104,128 if alternate else 160),10)
	if pointer != null and media.manifest.get("cursors",{}).has(str(pointer.style)):
		var row: Dictionary = media.manifest.cursors[str(pointer.style)]
		var cursor_image: Image = media.images[row.png.path]
		var anchor: Vector2i = pointer.position+Vector2i(row.bounds[0],row.bounds[1])-Vector2i(row.origin[0],row.origin[1])
		_record("pointer",{"frame":pointer.style,"position":[pointer.position.x,pointer.position.y],"bounds":[anchor.x,anchor.y,cursor_image.get_width(),cursor_image.get_height()],"quad_flags":0})
		image.blend_rect(cursor_image,Rect2i(Vector2i.ZERO,cursor_image.get_size()),anchor)
	return image
