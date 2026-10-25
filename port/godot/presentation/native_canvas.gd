extends Node2D
## T-29005: original 320x240 coordinates. Callers supply validated image data.
## This draws exported CLUT-resolved frames, without decoding original containers.

const LOGICAL_SIZE: Vector2i = Vector2i(320, 240)
var _commands: Array[Dictionary] = []
var _textures: Dictionary = {}

func present(commands: Array[Dictionary]) -> String:
	var prepared: Array[Dictionary] = []
	for command: Dictionary in commands:
		if not command.get("image") is Image or not command.get("position") is Vector2i:
			return "draw command needs validated Image and integer logical position"
		var image: Image = command["image"]
		if image.is_empty() or image.get_width() > 640 or image.get_height() > 512:
			return "draw command image exceeds native supported extent"
		if int(command.get("blend_mode", 0)) != 0:
			return "unproved native semi-transparency operation requires a separate compositor"
		var ident: String = str(command.get("id", ""))
		if ident.is_empty():
			return "draw command requires immutable validated content ID"
		if not _textures.has(ident):
			_textures[ident] = ImageTexture.create_from_image(image)
		prepared.append({"id": ident, "position": command["position"]})
	_commands = prepared
	texture_filter = CanvasItem.TEXTURE_FILTER_NEAREST
	queue_redraw()
	return ""

func _draw() -> void:
	for command: Dictionary in _commands:
		draw_texture(_textures[command["id"]], Vector2(command["position"]))

static func background_commands(image: Image, ident: String,
	draw_origin: Vector2i = Vector2i(160, 120)) -> Array[Dictionary]:
	# dec_bg_draw(0): two adjacent pieces; padding rows are never presented.
	if image.get_size() != Vector2i(320, 88):
		return []
	return [{"id": ident + ":left", "image": image.get_region(Rect2i(0, 0, 192, 88)),
		"position": draw_origin + Vector2i(-160, -48)},
		{"id": ident + ":right", "image": image.get_region(Rect2i(192, 0, 128, 88)),
		"position": draw_origin + Vector2i(32, -48)}]

static func actor_command(image: Image, ident: String, bounds: Rect2i,
	actor_position: Vector2i, origin: Vector2i, draw_origin: Vector2i) -> Dictionary:
	# actor draw_flags=0x84 subtracts the frame origin after anchor placement.
	return {"id": ident, "image": image,
		"position": draw_origin + actor_position + bounds.position - origin}
