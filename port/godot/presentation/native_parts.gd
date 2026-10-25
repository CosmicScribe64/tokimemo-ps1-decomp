extends RefCounted
## T-29005: bounded opaque part compositor for already CLUT-resolved texels.
## STP is separate data. Nonzero STP does not imply alpha when semi-transparency is off.

var error: String = ""

func compose(parts: Array[Dictionary], bounds: Rect2i) -> Image:
	error = ""
	if bounds.size.x <= 0 or bounds.size.y <= 0 or bounds.size.x > 640 or bounds.size.y > 512:
		error = "invalid composed frame extent"
		return null
	var prepared: Array[Dictionary] = []
	for part: Dictionary in parts:
		if not part.get("image") is Image or not part.get("position") is Vector2i:
			error = "part needs CLUT-resolved Image and native integer position"
			return null
		if int(part.get("flip_flags", 0)) & ~3 or int(part.get("blend_mode", 0)) != 0:
			error = "unproved part flip or semi-transparency mode"
			return null
		var image: Image = part["image"].duplicate()
		if int(part.get("flip_flags", 0)) & 2:
			image.flip_x()
		if int(part.get("flip_flags", 0)) & 1:
			image.flip_y()
		prepared.append({"image": image, "position": part["position"] - bounds.position})
	var result: Image = Image.create(bounds.size.x, bounds.size.y, false, Image.FORMAT_RGBA8)
	result.fill(Color.TRANSPARENT)
	# actor_draw_all traverses parts from count-1 down to zero.
	for index: int in range(prepared.size() - 1, -1, -1):
		var image: Image = prepared[index]["image"]
		result.blend_rect(image, Rect2i(Vector2i.ZERO, image.get_size()), prepared[index]["position"])
	return result
