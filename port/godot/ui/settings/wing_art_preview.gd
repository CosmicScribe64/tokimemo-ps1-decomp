extends TextureRect
## T-39220: live preview of a display.wing_art mode on the Display page. Shows
## the side art around the current game picture at the 16:9 shape (480 x 270
## logical, nearest scaled). It asks the display host for the preview when the
## host offers wing_art_preview(); otherwise it opens the wing art pack itself
## and uses a dark placeholder for the picture. Animation runs at 15 Hz from
## the preview's own clock and stops when motion is reduced.

const WingArt = preload("res://presentation/wing_art_provider.gd")
const LOGICAL_WIDTH := 480
const REFRESH_USEC := 66666

## Provider shared by previews when no display host is available.
static var _shared: RefCounted

var mode := "off"
var host: Object
var reduce_motion := false
var month := WingArt.DEFAULT_MONTH
## "" when the preview shows art; otherwise why it cannot.
var unavailable := ""
var _texture: ImageTexture
var _last_usec := -REFRESH_USEC
var _origin := -1
var _placeholder: Image

func _init() -> void:
	custom_minimum_size = Vector2(384, 216)
	expand_mode = TextureRect.EXPAND_IGNORE_SIZE
	stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED
	texture_filter = CanvasItem.TEXTURE_FILTER_NEAREST
	mouse_filter = Control.MOUSE_FILTER_IGNORE

func setup(display_host: Object, initial_mode: String, motion_reduced: bool) -> void:
	host = display_host
	reduce_motion = motion_reduced
	set_mode(initial_mode)

func set_mode(value: String) -> void:
	mode = value
	refresh(true)

func _process(_delta: float) -> void:
	if mode == "off" or reduce_motion or not unavailable.is_empty():
		return
	var now := Time.get_ticks_usec()
	if now - _last_usec >= REFRESH_USEC:
		refresh(false)

func refresh(force: bool) -> void:
	var now := Time.get_ticks_usec()
	if _origin < 0: _origin = now
	_last_usec = now
	var image: Image = null
	unavailable = ""
	if host != null and host.has_method("wing_art_preview"):
		image = host.wing_art_preview(mode, LOGICAL_WIDTH)
		if image == null and mode != "off":
			unavailable = "Side art needs the wing art pack made from your original disc."
	else:
		image = _own_preview(now)
	if image == null:
		image = _off_image()
	if _texture != null and _texture.get_size() == Vector2(image.get_size()) and not force:
		_texture.update(image)
	else:
		_texture = ImageTexture.create_from_image(image)
	texture = _texture

func _own_preview(now: int) -> Image:
	if mode == "off":
		return null
	if _shared == null:
		_shared = WingArt.new()
		var path := OS.get_environment("TOKIMEMO_WING_ART_PACK")
		if path.is_empty(): path = WingArt.discover_pack_path()
		_shared.open_pack(path)
	if not _shared.is_ready():
		unavailable = "Side art needs the wing art pack made from your original disc."
		return null
	var tick := WingArt.tick_for_usec(now - _origin)
	return _shared.preview(mode, {"month": month, "tick": tick, "reduce_motion": reduce_motion}, _centre(), LOGICAL_WIDTH)

func _centre() -> Image:
	if _placeholder == null:
		_placeholder = Image.create(320, 240, false, Image.FORMAT_RGBA8)
		_placeholder.fill(Color8(24, 30, 32))
		_placeholder.fill_rect(Rect2i(0, 0, 320, 1), Color8(96, 112, 104))
		_placeholder.fill_rect(Rect2i(0, 239, 320, 1), Color8(96, 112, 104))
		_placeholder.fill_rect(Rect2i(0, 0, 1, 240), Color8(96, 112, 104))
		_placeholder.fill_rect(Rect2i(319, 0, 1, 240), Color8(96, 112, 104))
	return _placeholder

func _off_image() -> Image:
	var out := Image.create(LOGICAL_WIDTH, 270, false, Image.FORMAT_RGBA8)
	out.fill(Color(0, 0, 0, 1))
	out.blit_rect(_centre(), Rect2i(0, 0, 320, 240), Vector2i((LOGICAL_WIDTH - 320) / 2, 15))
	return out
