extends RefCounted
## Optional image packs are fully validated before replacing the active pack.
const Images = preload("res://services/translated_image_store.gd")
const TextLayer = preload("res://services/text_layer_store.gd")
var images: RefCounted = null
var error := ""
var language := "ja"
var manifest := ""
var digest := ""
## T-39016: source-keyed text layer; null keeps every original string.
var text_layer: RefCounted = null
var text_layer_error := ""

func select_images(path: String, expected_sha256: String, allow_drafts: bool = false) -> bool:
	if path.is_empty():
		images = null; language = "ja"; manifest = ""; digest = ""; error = ""; return true
	var candidate := Images.new()
	if not candidate.open_pack(path, expected_sha256, allow_drafts):
		error = candidate.error; return false
	images = candidate; language = candidate.language; manifest = path; digest = expected_sha256; error = ""
	return true

## A failed open leaves the previous layer active; an empty path clears it.
func select_text_layer(path: String, expected_sha256: String) -> bool:
	if path.is_empty():
		text_layer = null; text_layer_error = ""; return true
	var candidate := TextLayer.new()
	if not candidate.open_pack(path, expected_sha256):
		text_layer_error = candidate.error; return false
	text_layer = candidate; text_layer_error = ""
	return true
