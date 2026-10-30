extends RefCounted
## T-30001 WP2: builds the Wl x 240 presentation Image from the native 320x240
## Image. The native Image is copied byte for byte into columns
## wing..wing+319 and is never modified; the wings come from a provider.
## Wl = 320 returns a byte-identical copy and never calls the provider.
##
## A provider is any object with
##   fill_wings(target: Image, wing_width: int, context: Dictionary) -> String
## that writes only columns x < wing_width and x >= wing_width + 320 and
## returns "" on success. With no provider the wings are opaque black.

const NATIVE_SIZE := Vector2i(320, 240)
const MIN_LOGICAL_WIDTH := 320
const MAX_LOGICAL_WIDTH := 560

var error := ""

func compose(native: Image, logical_width: int, provider: Object = null, context: Dictionary = {}) -> Image:
	error = ""
	if native == null or native.is_empty() or native.get_size() != NATIVE_SIZE:
		error = "native image must be 320x240"
		return null
	if logical_width < MIN_LOGICAL_WIDTH or logical_width > MAX_LOGICAL_WIDTH or logical_width % 2 != 0:
		error = "logical width must be even and within 320..560"
		return null
	if native.is_compressed():
		error = "native image must be uncompressed"
		return null
	if logical_width == MIN_LOGICAL_WIDTH:
		return native.duplicate()
	var wing := (logical_width - NATIVE_SIZE.x) >> 1
	var image := Image.create(logical_width, NATIVE_SIZE.y, false, native.get_format())
	image.fill(Color(0, 0, 0, 1))
	if provider != null:
		if not provider.has_method("fill_wings"):
			error = "wing provider lacks fill_wings"
			return null
		var failure: Variant = provider.fill_wings(image, wing, context)
		if not failure is String or not failure.is_empty():
			error = "wing provider failed: %s" % failure
			return null
	# Copy last so no provider write can reach the native columns.
	image.blit_rect(native, Rect2i(Vector2i.ZERO, NATIVE_SIZE), Vector2i(wing, 0))
	return image

## The native columns of a composed Image, for invariant checks.
static func centre_crop(composed: Image) -> Image:
	if composed == null or composed.get_height() != NATIVE_SIZE.y or composed.get_width() < NATIVE_SIZE.x:
		return null
	var wing := (composed.get_width() - NATIVE_SIZE.x) >> 1
	return composed.get_region(Rect2i(Vector2i(wing, 0), NATIVE_SIZE))
