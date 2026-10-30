extends RefCounted
## Optional image packs are fully validated before replacing the active pack.
const Images = preload("res://services/translated_image_store.gd")
var images: RefCounted = null
var error := ""
var language := "ja"
var manifest := ""
var digest := ""

func select_images(path: String, expected_sha256: String, allow_drafts: bool = false) -> bool:
	if path.is_empty():
		images = null; language = "ja"; manifest = ""; digest = ""; error = ""; return true
	var candidate := Images.new()
	if not candidate.open_pack(path, expected_sha256, allow_drafts):
		error = candidate.error; return false
	images = candidate; language = candidate.language; manifest = path; digest = expected_sha256; error = ""
	return true
