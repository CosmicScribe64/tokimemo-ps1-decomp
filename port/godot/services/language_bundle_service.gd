extends RefCounted
## T-30002 WP2: semantic language choice (ja | en | custom) resolved to packs.
## 'en' opens the members of the English bundle manifest written by
## tools/port/build_english_bundle.py into the installed asset profile (never
## into the repository or res://). Members are pinned by sha256 in that
## manifest; the preference stores only the word 'en', so a regenerated bundle
## with new digests keeps English. 'custom' opens the user-picked manifests
## with their stored sha256. All members open on a WorkerThreadPool task and
## replace the active set in one step on poll(); any failure keeps the previous
## set and leaves a visible notice. Draft images open only when allowed.
## Presentation only: nothing here touches GameState, RNG or the save image.

signal applied(choice: String, coverage: Dictionary)
signal fallback(notice: String)
## The switch applied, but part of it was degraded (for example draft art that
## could not be opened, so the reviewed art is shown). `notice` holds the text.
signal warning(notice: String)

const ExternalPath = preload("res://services/external_content_path.gd")
const Images = preload("res://services/translated_image_store.gd")
const TextLayer = preload("res://services/text_layer_store.gd")
const Dialogue = preload("res://services/text_translation_store.gd")
const SCHEMA := "tokimemo.english-bundle.v1"
const CHOICES := ["ja", "en", "custom"]
const LABELS := {"ja": "Japanese (Original)", "en": "English (AI translation)", "custom": "Custom packs"}
## Default location inside an installed asset profile root. setup_content.py
## (Phase 4 WP-C) builds the bundle into <installed>/language/en by default.
const BUNDLE_RELATIVE := "language/en/english_bundle.json"
const MAX_BUNDLE := 262144
## Accepted member schemas. Image packs may be v1 (frame rows only) or v2
## (page and MDEC still rows); a v2 pack opens only with the v2 image store.
const IMAGE_PACKS := ["tokimemo.translated-image-pack.v1", "tokimemo.translated-image-pack.v2"]
const MEMBER_SCHEMAS := {
	"ui_text": ["tokimemo.text-layer-pack.v1"],
	"dialogue": ["tokimemo.text-translation-pack.v1"],
	"images": IMAGE_PACKS,
	"draft_images": IMAGE_PACKS,
}
## Coverage counts each member must carry (all non-negative integers).
const MEMBER_COVERAGE := {
	"ui_text": ["approved_records", "representable_records"],
	"dialogue": ["lines"],
	"images": ["reviewed_images", "original_japanese_images"],
	"draft_images": ["draft_images"],
}

var choice := "ja"
var images: RefCounted = null
var text_layer: RefCounted = null
var dialogue: RefCounted = null
var coverage: Dictionary = {}
var disclosure := ""
var omitted: Array = []
var notice := ""
var loading := false
var requested_choice := ""
var _generation := 0
var _tasks: Dictionary = {}
var _results: Dictionary = {}
var _mutex := Mutex.new()

static func bundle_path(installed_root: String) -> String:
	return installed_root.path_join(BUNDLE_RELATIVE)

static func _sha(value: Variant) -> bool:
	if not value is String or value.length() != 64: return false
	for character: String in value:
		if character not in "0123456789abcdef": return false
	return true

static func _relative(value: Variant) -> bool:
	if not value is String or value.is_empty() or value.length() > 512 or value.contains("\\") or value.contains(":") or value.begins_with("/"):
		return false
	for part: String in value.split("/"):
		if part in ["", ".", ".."]: return false
	return true

static func _count(value: Variant) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and float(value) == floor(float(value)) and float(value) >= 0.0

static func _failure(message: String) -> Dictionary:
	return {"ok": false, "error": message}

## Reads and checks the bundle manifest only (no member is opened).
static func read_bundle(path: String) -> Dictionary:
	var absolute := ProjectSettings.globalize_path(path)
	if not ExternalPath.is_external_file(absolute) or FileAccess.get_size(absolute) > MAX_BUNDLE:
		return _failure("English bundle is missing")
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(absolute))
	if not parsed is Dictionary or parsed.get("schema") != SCHEMA or parsed.get("language") != "en":
		return _failure("English bundle schema is unsupported")
	if not parsed.get("disclosure") is String or parsed.disclosure.is_empty() or parsed.disclosure.length() > 1024:
		return _failure("English bundle lacks its AI-translation disclosure")
	if not parsed.get("members") is Dictionary or parsed.members.is_empty() or not parsed.get("omitted", []) is Array:
		return _failure("English bundle member table is invalid")
	var members := {}
	for name: Variant in parsed.members:
		var row: Variant = parsed.members[name]
		if not name is String or not MEMBER_SCHEMAS.has(name) or not row is Dictionary:
			return _failure("English bundle has an unknown member")
		if row.get("schema") not in MEMBER_SCHEMAS[name] or not _relative(row.get("path")) or not _sha(row.get("sha256")) or not row.get("coverage") is Dictionary:
			return _failure("English bundle member %s is invalid" % name)
		var counts := {}
		for key: String in MEMBER_COVERAGE[name]:
			if not _count(row.coverage.get(key)): return _failure("English bundle member %s lacks coverage" % name)
			counts[key] = int(row.coverage[key])
		members[name] = {"path": absolute.get_base_dir().path_join(row.path), "sha256": row.sha256, "coverage": counts}
	if members.has("ui_text") and members.ui_text.coverage.representable_records > members.ui_text.coverage.approved_records:
		return _failure("English bundle text coverage is inconsistent")
	if members.has("draft_images") and not members.has("images"):
		return _failure("English bundle lists drafts without the reviewed image pack")
	var reasons: Array = []
	for row: Variant in parsed.get("omitted", []):
		if not row is Dictionary or not row.get("member") is String or not row.get("reason") is String or row.reason.length() > 512:
			return _failure("English bundle omission record is invalid")
		reasons.append({"member": row.member, "reason": row.reason})
	return {"ok": true, "members": members, "disclosure": parsed.disclosure, "omitted": reasons}

## Opens every member synchronously. Safe on a worker thread: the stores only
## use FileAccess, JSON and Image. Returns {"ok", "error"} plus the stores.
static func open_english(path: String, allow_drafts: bool) -> Dictionary:
	var bundle := read_bundle(path)
	if not bundle.ok: return bundle
	var members: Dictionary = bundle.members
	var result := {"ok": true, "choice": "en", "images": null, "text_layer": null, "dialogue": null,
		"disclosure": bundle.disclosure, "omitted": bundle.omitted, "drafts_active": false, "warning": ""}
	if members.has("ui_text"):
		var layer := TextLayer.new()
		if not layer.open_pack(members.ui_text.path, members.ui_text.sha256): return _failure("English UI text: " + layer.error)
		if layer.entry_count() != members.ui_text.coverage.approved_records: return _failure("English UI text coverage differs from its pack")
		result.text_layer = layer
	if members.has("dialogue"):
		var lines := Dialogue.new()
		if not lines.open_pack(members.dialogue.path, members.dialogue.sha256): return _failure("English dialogue: " + lines.error)
		if lines.entries.size() + lines.unsupported_entries.size() != members.dialogue.coverage.lines: return _failure("English dialogue coverage differs from its pack")
		result.dialogue = lines
	if members.has("images"):
		# The reviewed pack always opens and is checked against its coverage.
		var store := Images.new()
		if not store.open_pack(members.images.path, members.images.sha256, false): return _failure("English artwork: " + store.error)
		if store.asset_count() != members.images.coverage.reviewed_images: return _failure("English artwork coverage differs from its pack")
		result.images = store
		# Draft art is opened only on explicit opt-in; otherwise it is never read.
		# A draft pack that fails to open, or that lacks a reviewed asset, is
		# dropped with a warning and the reviewed art stays in use.
		if allow_drafts and members.has("draft_images"):
			var drafts := Images.new()
			var problem := ""
			if not drafts.open_pack(members.draft_images.path, members.draft_images.sha256, true): problem = drafts.error
			else: problem = _missing_reviewed(members.images, members.draft_images)
			if problem.is_empty():
				result.images = drafts; result.drafts_active = true
			else:
				result.warning = "Draft English artwork could not be loaded (%s). Showing reviewed artwork only." % problem
	var reviewed: int = members.images.coverage.reviewed_images if members.has("images") else 0
	result.coverage = {
		"text_records_approved": members.ui_text.coverage.approved_records if members.has("ui_text") else 0,
		"text_records_representable": members.ui_text.coverage.representable_records if members.has("ui_text") else 0,
		"dialogue_lines": members.dialogue.coverage.lines if members.has("dialogue") else 0,
		"images_reviewed": reviewed,
		"images_original_japanese": members.images.coverage.original_japanese_images if members.has("images") else 0,
		"draft_images": members.draft_images.coverage.draft_images if members.has("draft_images") else 0,
		"draft_images_active": result.drafts_active,
		"omitted_members": bundle.omitted.map(func(row: Dictionary) -> String: return row.member),
	}
	return result

## Identity of one image row in either pack schema (frame, page or still).
static func _image_identity(row: Variant) -> String:
	if not row is Dictionary: return ""
	var parts: Array = []
	for key: String in ["kind", "source_container_sha256", "frame", "palette", "part_index", "page", "bs_index", "decoder_identity_sha256", "source_png_sha256", "source_rgba_sha256", "output_sha256"]:
		var value: Variant = row.get(key, "frame" if key == "kind" else null)
		parts.append(str(int(value)) if (value is float or value is int) else str(value))
	return "|".join(parts)

## Reads a pinned image manifest's rows again (the store already checked it).
static func _image_rows(member: Dictionary) -> Variant:
	if FileAccess.get_sha256(member.path) != member.sha256: return null
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(member.path))
	return parsed.assets if parsed is Dictionary and parsed.get("assets") is Array else null

## "" when every reviewed asset is also in the draft pack, else the reason. A
## draft frame row that names a reviewed output in `supersedes_output_sha256`
## (same source identity) stands in for that reviewed asset (T-39004 date panel).
static func _missing_reviewed(reviewed: Dictionary, draft: Dictionary) -> String:
	var approved: Variant = _image_rows(reviewed)
	var drafts: Variant = _image_rows(draft)
	if approved == null or drafts == null: return "an image manifest changed after opening"
	var present := {}
	for row: Variant in drafts:
		present[_image_identity(row)] = true
		if row is Dictionary and row.get("kind", "frame") == "frame" and row.get("supersedes_output_sha256") is String:
			var replaced: Dictionary = row.duplicate()
			replaced["output_sha256"] = row["supersedes_output_sha256"]
			present[_image_identity(replaced)] = true
	var missing := 0
	for row: Variant in approved:
		if not present.has(_image_identity(row)): missing += 1
	return "" if missing == 0 else "the draft pack lacks %d reviewed images" % missing

## Opens the user-picked packs stored in the language preferences.
static func open_custom(language: Dictionary, allow_drafts: bool) -> Dictionary:
	var result := {"ok": true, "choice": "custom", "images": null, "text_layer": null, "dialogue": null,
		"disclosure": "", "omitted": [], "drafts_active": false, "coverage": {}, "warning": ""}
	if not str(language.get("text_manifest", "")).is_empty():
		var store := Images.new()
		if not store.open_pack(ProjectSettings.globalize_path(language.text_manifest), str(language.get("text_sha256", "")), allow_drafts):
			return _failure("Custom artwork pack: " + store.error)
		result.images = store
	if not str(language.get("ui_text_manifest", "")).is_empty():
		var layer := TextLayer.new()
		if not layer.open_pack(ProjectSettings.globalize_path(language.ui_text_manifest), str(language.get("ui_text_sha256", ""))):
			return _failure("Custom UI text pack: " + layer.error)
		result.text_layer = layer
	if not str(language.get("dialogue_manifest", "")).is_empty():
		var lines := Dialogue.new()
		if not lines.open_pack(language.dialogue_manifest, str(language.get("dialogue_sha256", ""))):
			return _failure("Custom dialogue pack: " + lines.error)
		result.dialogue = lines
	return result

## Starts a switch. 'ja' applies at once; 'en' and 'custom' open on a worker
## and apply on a later poll(). options: {"bundle_path": String,
## "allow_drafts": bool, "language": settings.language for 'custom'}.
## Returns false only for an unknown choice.
func request(next_choice: String, options: Dictionary = {}) -> bool:
	if next_choice not in CHOICES: return false
	_generation += 1
	requested_choice = next_choice
	if next_choice == "ja":
		loading = false
		_apply({"ok": true, "choice": "ja", "images": null, "text_layer": null, "dialogue": null,
			"disclosure": "", "omitted": [], "coverage": {}, "warning": ""})
		return true
	loading = true
	var generation := _generation
	var allow_drafts: bool = options.get("allow_drafts", false) == true
	var bundle: String = str(options.get("bundle_path", ""))
	var language: Dictionary = options.get("language", {}).duplicate(true) if options.get("language", {}) is Dictionary else {}
	var job := func() -> void:
		var outcome := open_english(bundle, allow_drafts) if next_choice == "en" else open_custom(language, allow_drafts)
		_mutex.lock(); _results[generation] = outcome; _mutex.unlock()
	_tasks[generation] = WorkerThreadPool.add_task(job, false, "language bundle open")
	return true

## Main-thread step. Never waits on an unfinished task. Returns true when a
## result was applied or rejected in this call.
func poll() -> bool:
	var handled := false
	for generation: int in _tasks.keys():
		var task: int = _tasks[generation]
		if not WorkerThreadPool.is_task_completed(task): continue
		WorkerThreadPool.wait_for_task_completion(task)
		_tasks.erase(generation)
		_mutex.lock(); var outcome: Dictionary = _results.get(generation, _failure("language task produced no result")); _results.erase(generation); _mutex.unlock()
		if generation != _generation: continue
		loading = false; handled = true
		if outcome.ok: _apply(outcome)
		else: _reject(outcome.error)
	return handled

## True while a worker task has not been collected by poll() or wait().
func busy() -> bool:
	return not _tasks.is_empty()

## Blocks until outstanding tasks end (shutdown and tests only).
func wait() -> void:
	for generation: int in _tasks.keys():
		WorkerThreadPool.wait_for_task_completion(_tasks[generation])
	_tasks_settled()

func _tasks_settled() -> void:
	var keys: Array = _tasks.keys()
	for generation: int in keys:
		_tasks.erase(generation)
		_mutex.lock(); var outcome: Variant = _results.get(generation); _results.erase(generation); _mutex.unlock()
		if generation != _generation or not outcome is Dictionary: continue
		loading = false
		if outcome.ok: _apply(outcome)
		else: _reject(outcome.error)

func _apply(outcome: Dictionary) -> void:
	choice = outcome.choice
	images = outcome.images; text_layer = outcome.text_layer; dialogue = outcome.dialogue
	coverage = outcome.get("coverage", {}).duplicate(true)
	disclosure = outcome.disclosure; omitted = outcome.omitted.duplicate(true)
	notice = str(outcome.get("warning", ""))
	applied.emit(choice, coverage.duplicate(true))
	if not notice.is_empty(): warning.emit(notice)

func _reject(reason: String) -> void:
	notice = "%s could not be loaded (%s). Keeping %s." % [LABELS.get(requested_choice, requested_choice), reason, LABELS.get(choice, choice)]
	fallback.emit(notice)

## One line for the Language page, from the applied manifests.
func coverage_line() -> String:
	if choice != "en": return ""
	return "%d approved text records (%d drawable with the original font), %d reviewed images; %d images stay in original Japanese." % [
		coverage.get("text_records_approved", 0), coverage.get("text_records_representable", 0),
		coverage.get("images_reviewed", 0), coverage.get("images_original_japanese", 0)]
