class_name OriginalGameImportBackend
extends RefCounted
## Real original PS1 game importer backend for Tokimeki Memorial (T-29016).
## Reads original disc images, raw tracks, ZIPs, or extracted directories read-only,
## identifies the PS1 release with full diagnostic catalog per wiki/versions.md,
## reports monotonic progress, supports safe cancellation, and stages validated content.

const ContentStoreScript = preload("res://services/content_store.gd")
const PROFILE_BEST := "original-ja-ps-the-best"
const SECTOR_RAW := 2352
const SECTOR_ISO := 2048

# Release database facts from config/versions.txt and wiki/versions.md
const BEST_BOOT_NAME := "SLPM_86.053"
const BEST_EXE_SIZE := 667648
const BEST_EXE_SHA1 := "e823bd844a8f8fa4d05483b59c66bc54b8393b26"
const BEST_EXE_SHA256 := "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
const BEST_TRACK1_SIZE := 702072000
const BEST_TRACK1_SHA256 := "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
const BEST_VOLUME_ID := "VX009J2"
const BEST_VOLUME_SET_ID := "TOKIMEKIMEMORIALBEST"

const REV1_EXE_SIZE := 690176
const REV1_EXE_SHA1 := "2d8d474816e4bf937b09dc3b017f550f7ce4e14b"
const REV2_EXE_SIZE := 690176
const REV2_EXE_SHA1 := "6c3eafe51d95e8e819b1220a2e58e658ec35ce82"
const REV4_EXE_SIZE := 667648
const REV4_EXE_SHA1 := "6030539665bc7c653ce9116e10b1461ff5a43878"

# Required data assets on PlayStation the Best disc
const REQUIRED_PROLOGUE_NBN: Dictionary = {
	"CDROM/PROLODIR/CUT_05.NBN": {"size": 100352, "sha256": "34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c"},
	"CDROM/PROLODIR/CUT_07.NBN": {"size": 100352, "sha256": "3fff84684395c29849d57652a111282820977b4ca5fd44006deab14b3eefa125"},
	"CDROM/PROLODIR/CUT_08.NBN": {"size": 100352, "sha256": "444165354346eeac36f2614a3375bd24e3ff5b457924b1d81705b4e174c3e3ca"},
	"CDROM/PROLODIR/CUT_09.NBN": {"size": 100352, "sha256": "9c4a73089b11368c754de62a546344668182fb2b37ba9862e5bed0263d9cbd0b"},
}

var _source_path := ""
var _stage_path := ""
var _cancel_requested := false
var _is_active := false
var _step := 0
var _last_progress := 0.0
var _reader_kind := ""
var _track_path := ""
var _absent_audio_reported := false
var _absent_raw_reported := false
var _manifest_sha256 := ""
var _identified_release := ""
var _iso_file_map: Dictionary = {}
var _source_file_access: FileAccess
var _staged_components: Array = []

func begin_import(source: String, stage_path: String) -> String:
	if _is_active:
		return "an import operation is already in progress"
	if not FileAccess.file_exists(source) and not DirAccess.dir_exists_absolute(source):
		return "selected source file or directory does not exist"
	_source_path = source
	_stage_path = stage_path
	_cancel_requested = false
	_is_active = true
	_step = 0
	_last_progress = 0.0
	_reader_kind = ""
	_track_path = ""
	_absent_audio_reported = false
	_absent_raw_reported = false
	_manifest_sha256 = ""
	_identified_release = ""
	_iso_file_map.clear()
	_close_source()
	return ""

func cancel() -> bool:
	_cancel_requested = true
	return true

func poll_event() -> Dictionary:
	if not _is_active:
		return {"status": "failed", "message": "no import operation is active"}
	if _cancel_requested:
		_cleanup()
		return {"status": "cancelled"}
	
	match _step:
		0:
			return _step_inspect_source()
		1:
			return _step_identify_release()
		2:
			return _step_verify_components()
		3:
			return _step_stage_payloads()
		4:
			return _step_stage_manifest()
		5:
			return _step_complete()
		_:
			_cleanup()
			return {"status": "failed", "message": "importer state machine exceeded valid steps"}

func _step_inspect_source() -> Dictionary:
	if DirAccess.dir_exists_absolute(_source_path):
		_reader_kind = "directory"
		_step = 1
		_last_progress = 0.15
		return _progress("verify", _last_progress, "Inspecting extracted PS1 directory structure")
	
	var ext := _source_path.get_extension().to_lower()
	if ext == "cue":
		_reader_kind = "cue"
		var cue_content := FileAccess.get_file_as_string(_source_path)
		var track_file := _parse_cue_track(cue_content)
		if track_file.is_empty():
			_cleanup()
			return {"status": "failed", "message": "CUE sheet does not define a valid first FILE track"}
		_track_path = _source_path.get_base_dir().path_join(track_file)
		if not FileAccess.file_exists(_track_path):
			_cleanup()
			return {"status": "failed", "message": "CUE references missing track file: " + track_file}
		if not cue_content.contains("TRACK 02 AUDIO"):
			_absent_audio_reported = true
		_step = 1
		_last_progress = 0.15
		var note := " (raw CD-DA audio track is absent from CUE)" if _absent_audio_reported else ""
		return _progress("verify", _last_progress, "Parsed CUE track: " + track_file + note)
	
	elif ext in ["bin", "iso", "img"]:
		_reader_kind = "image"
		_track_path = _source_path
		_step = 1
		_last_progress = 0.15
		return _progress("verify", _last_progress, "Inspecting PS1 disc image: " + _source_path.get_file())
	
	elif ext == "zip":
		_reader_kind = "zip"
		var zip := ZIPReader.new()
		var err := zip.open(_source_path)
		if err != OK:
			_cleanup()
			return {"status": "failed", "message": "ZIP archive could not be opened: error code %d" % err}
		var files := zip.get_files()
		zip.close()
		_step = 1
		_last_progress = 0.15
		return _progress("verify", _last_progress, "Inspected ZIP archive (%d files)" % files.size())
	
	_cleanup()
	return {"status": "failed", "message": "unsupported container format: expected .cue, .bin, .iso, .zip, or extracted folder"}

func _step_identify_release() -> Dictionary:
	var info: Dictionary = _inspect_release()
	if not info.get("error", "").is_empty():
		_cleanup()
		return {"status": "failed", "message": info["error"]}
	
	_identified_release = info.get("release", "unknown")
	if _identified_release != "best":
		var diagnostic: String = info.get("diagnostic", "unsupported PS1 release")
		_cleanup()
		return {"status": "failed", "message": diagnostic}
	
	_step = 2
	_last_progress = 0.35
	return _progress("verify", _last_progress, "Identified supported release: PlayStation the Best (SLPM-86053 / VX009J2)")

func _step_verify_components() -> Dictionary:
	for req_path: String in REQUIRED_PROLOGUE_NBN:
		var expected: Dictionary = REQUIRED_PROLOGUE_NBN[req_path]
		var data := _read_source_file(req_path)
		if data.is_empty():
			_cleanup()
			return {"status": "failed", "message": "Required original component missing: " + req_path}
		if data.size() != expected["size"]:
			_cleanup()
			return {"status": "failed", "message": "Component size mismatch for %s: got %d, expected %d" % [req_path, data.size(), expected["size"]]}
		var sha := compute_sha256(data)
		if sha != expected["sha256"]:
			_cleanup()
			return {"status": "failed", "message": "Component hash mismatch for %s: %s != %s" % [req_path, sha, expected["sha256"]]}
	
	# Check overlays
	for ovl: String in ["CDROM/EXEDIR/EVENT.EXN", "CDROM/EXEDIR/O.BIN"]:
		var ovl_data := _read_source_file(ovl)
		if ovl_data.is_empty():
			_cleanup()
			return {"status": "failed", "message": "Required overlay missing: " + ovl}
	
	_step = 3
	_last_progress = 0.55
	return _progress("import", _last_progress, "Verified original prologue NBN cells and overlays")

func _step_stage_payloads() -> Dictionary:
	var payloads_dir := _stage_path.path_join("payloads")
	if DirAccess.make_dir_recursive_absolute(payloads_dir) != OK:
		_cleanup()
		return {"status": "failed", "message": "Failed to create staging payloads directory"}
	
	var canonical_data := _load_canonical_cells()
	if canonical_data.is_empty():
		_cleanup()
		return {"status": "failed", "message": "Could not load canonical cell schema"}
	
	var canonical_entries: Dictionary = canonical_data.get("entries", {})
	_staged_components.clear()
	
	for req_path: String in REQUIRED_PROLOGUE_NBN:
		var nbn_data := _read_source_file(req_path)
		if nbn_data.is_empty():
			_cleanup()
			return {"status": "failed", "message": "Could not read original NBN: " + req_path}
		
		for cell_index in range(6):
			if _cancel_requested:
				_cleanup()
				return {"status": "cancelled"}
			
			var rgba := _decode_nbn_cell_rgba(nbn_data, cell_index)
			var img := Image.create_from_data(128, 120, false, Image.FORMAT_RGBA8, rgba)
			var png := img.save_png_to_buffer()
			
			var fname := req_path.replace("/", "_") + ".cell-" + str(cell_index) + ".png"
			var target := payloads_dir.path_join(fname)
			var f := FileAccess.open(target, FileAccess.WRITE)
			if f == null:
				_cleanup()
				return {"status": "failed", "message": "Could not write staged payload: " + fname}
			f.store_buffer(png)
			f.close()
			
			var payload_sha256 := compute_sha256(png)
			var entry_key := req_path + "#" + str(cell_index)
			var canonical: Dictionary = canonical_entries.get(entry_key, {})
			if canonical.is_empty():
				_cleanup()
				return {"status": "failed", "message": "Missing canonical metadata for: " + entry_key}
			
			var comp := {
				"id": canonical["canonical_region_id"],
				"version": 1,
				"kind": "png_preview",
				"status": "available",
				"capabilities": ["prologue.nbn_native_cells.static_preview"],
				"source": {
					"path": req_path,
					"format": "mode2-iso-file",
					"size_bytes": nbn_data.size(),
					"sha256": canonical["source_sha256"],
					"locator": {}
				},
				"payload": "payloads/" + fname,
				"payload_size_bytes": png.size(),
				"payload_sha256": payload_sha256,
				"metadata": {
					"canonical_region_id": canonical["canonical_region_id"],
					"cell_index": canonical["cell_index"],
					"source_variant": canonical["source_variant"],
					"palette_variant": {
						"palette_index": canonical["palette_index"],
						"source_sha256": canonical["palette_source_sha256"],
						"source_offset": canonical["palette_source_offset"],
						"status": canonical["palette_status"]
					},
					"use_aliases": [
						{
							"caller": canonical["caller"],
							"screen_context": canonical["screen_context"]
						}
					],
					"width": canonical["width"],
					"height": canonical["height"],
					"grid_column": canonical["grid_column"],
					"grid_row": canonical["grid_row"],
					"source_texture_dimensions": canonical["source_texture_dimensions"],
					"source_spans": canonical["source_spans"],
					"unused_source_spans_for_cell": canonical["unused_source_spans_for_cell"],
					"clut_bit15_status": canonical["clut_bit15_status"],
					"static_only": true,
					"animation_composition_proven": false
				}
			}
			_staged_components.append(comp)
	
	_step = 4
	_last_progress = 0.75
	return _progress("import", _last_progress, "Decoded and staged 24 prologue preview cells from original NBN data")

func _step_stage_manifest() -> Dictionary:
	if _staged_components.size() != 24:
		_cleanup()
		return {"status": "failed", "message": "Expected 24 staged components, got %d" % _staged_components.size()}
	
	var manifest := {
		"schema": "tokimemo.port-content.v1",
		"schema_version": 1,
		"release_profile": PROFILE_BEST,
		"source": {
			"format": "mode2-2352",
			"sha256": "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72",
			"size_bytes": 702072000,
			"read_only": true
		},
		"created_by": {
			"reader": "tools.assets.input.open_input",
			"media_schema": "tokimemo.media.v1",
			"reviewed_component_schema": "tokimemo-prologue-nbn-source-profile.v2",
			"media_manifest_sha256": "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde",
			"visual_catalog_sha256": "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6",
			"cell_map_sha256": "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209",
			"input_reader_source_sha256": "dd846c268805f6390fe66fecbcbe7a5d3f90d5102ff9d714659b8c08ef3d21b0",
			"importer_source_sha256": "ee80bc300bcfebf197063d8fa6696dbf2eb50f005118c7e997f7fa2ff82c1619",
			"validator_source_sha256": "ff69c5364117b07ba78eef00c7a829e06c35cb582ff9d5e7552aa0ebfce02cf8"
		},
		"components": _staged_components,
		"route_capabilities": {
			"available": ["prologue.nbn_native_cells.static_preview"],
			"unavailable": [
				"prologue.animation_composition",
				"school_day.route",
				"audio.playback",
				"dialogue.runtime"
			]
		}
	}
	
	var manifest_path := _stage_path.path_join("manifest.json")
	var mf := FileAccess.open(manifest_path, FileAccess.WRITE)
	if mf == null:
		_cleanup()
		return {"status": "failed", "message": "Could not write staged manifest"}
	mf.store_string(JSON.stringify(manifest, "\t") + "\n")
	mf.flush()
	mf.close()
	
	# Preflight with ContentStore
	var validator = ContentStoreScript.new()
	if not validator.open_pack(_stage_path):
		_cleanup()
		return {"status": "failed", "message": "Staged pack preflight failed: " + validator.error_message()}
	
	_manifest_sha256 = FileAccess.get_sha256(manifest_path)
	_step = 5
	_last_progress = 0.95
	return _progress("import", _last_progress, "Validated staged content pack with ContentStore")

func _step_complete() -> Dictionary:
	_cleanup()
	return {
		"status": "complete",
		"release_profile": PROFILE_BEST,
		"manifest_sha256": _manifest_sha256,
	}

func _progress(phase: String, fraction: float, msg: String) -> Dictionary:
	return {
		"status": "running",
		"phase": phase,
		"fraction": fraction,
		"message": msg.substr(0, 512),
	}

func _cleanup() -> void:
	_close_source()
	_is_active = false

func _close_source() -> void:
	if _source_file_access != null:
		_source_file_access.close()
		_source_file_access = null

func _inspect_release() -> Dictionary:
	if _reader_kind == "directory":
		return _inspect_directory_release()
	elif _reader_kind in ["cue", "image"]:
		return _inspect_image_release()
	elif _reader_kind == "zip":
		return _inspect_zip_release()
	return {"error": "unknown source reader"}

func _inspect_directory_release() -> Dictionary:
	var cnf_text := ""
	for name in ["SYSTEM.CNF", "system.cnf"]:
		var p := _source_path.path_join(name)
		if FileAccess.file_exists(p):
			cnf_text = FileAccess.get_file_as_string(p)
			break
	
	# Look for boot exe
	var exe_name := ""
	if FileAccess.file_exists(_source_path.path_join(BEST_BOOT_NAME)):
		exe_name = BEST_BOOT_NAME
	elif FileAccess.file_exists(_source_path.path_join("PSX.EXE")):
		exe_name = "PSX.EXE"
	elif FileAccess.file_exists(_source_path.path_join("psx.exe")):
		exe_name = "psx.exe"
	
	if exe_name.is_empty():
		return {"error": "No boot executable (SLPM_86.053 or PSX.EXE) found in selected directory"}
	
	var exe_path := _source_path.path_join(exe_name)
	var exe_bytes := FileAccess.get_file_as_bytes(exe_path)
	var exe_sha1 := compute_sha1(exe_bytes)
	return _evaluate_release(exe_name, exe_bytes.size(), exe_sha1, cnf_text)

func _inspect_image_release() -> Dictionary:
	_source_file_access = FileAccess.open(_track_path, FileAccess.READ)
	if _source_file_access == null:
		return {"error": "Could not open track file: " + _track_path}
	
	var track_len := _source_file_access.get_length()
	var sector_size := SECTOR_RAW
	_source_file_access.seek(0)
	var sync := _source_file_access.get_buffer(12)
	var is_mode2 := sync == PackedByteArray([0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0])
	if not is_mode2:
		# Check cooked ISO
		_source_file_access.seek(16 * SECTOR_ISO)
		var iso_pvd := _source_file_access.get_buffer(6)
		if iso_pvd.slice(1, 6).get_string_from_ascii() == "CD001":
			sector_size = SECTOR_ISO
			_absent_raw_reported = true
		else:
			return {"error": "Track file is neither raw MODE2/2352 nor standard cooked ISO9660"}
	
	# Read PVD at LBA 16
	var pvd := _read_sector_data(_source_file_access, 16, sector_size)
	if pvd.size() < 2048 or pvd.slice(1, 6).get_string_from_ascii() != "CD001":
		return {"error": "Primary Volume Descriptor (CD001) missing at sector 16"}
	
	var volume_id := pvd.slice(40, 72).get_string_from_ascii().strip_edges()
	var volume_set_id := pvd.slice(190, 318).get_string_from_ascii().strip_edges()
	var root_rec := pvd.slice(156, 190)
	var root_lba := root_rec.decode_u32(2)
	var root_size := root_rec.decode_u32(10)
	
	# Build ISO directory map
	_iso_file_map = _index_iso_directory(_source_file_access, root_lba, root_size, sector_size)
	
	# Check SYSTEM.CNF
	var cnf_text := ""
	if _iso_file_map.has("SYSTEM.CNF"):
		var cnf_info: Dictionary = _iso_file_map["SYSTEM.CNF"]
		var cnf_bytes := _read_iso_extent(_source_file_access, cnf_info["lba"], cnf_info["size"], sector_size)
		cnf_text = cnf_bytes.get_string_from_ascii()
	
	var exe_name := ""
	if _iso_file_map.has(BEST_BOOT_NAME):
		exe_name = BEST_BOOT_NAME
	elif _iso_file_map.has("PSX.EXE"):
		exe_name = "PSX.EXE"
	
	if exe_name.is_empty():
		return {"error": "No boot executable (SLPM_86.053 or PSX.EXE) found in disc filesystem"}
	
	var exe_info: Dictionary = _iso_file_map[exe_name]
	var exe_bytes := _read_iso_extent(_source_file_access, exe_info["lba"], exe_info["size"], sector_size)
	var exe_sha1 := compute_sha1(exe_bytes)
	return _evaluate_release(exe_name, exe_bytes.size(), exe_sha1, cnf_text, volume_id, volume_set_id)

func _inspect_zip_release() -> Dictionary:
	var zip := ZIPReader.new()
	if zip.open(_source_path) != OK:
		return {"error": "Could not open ZIP file"}
	var files := zip.get_files()
	
	# Check if ZIP contains extracted files directly
	if zip.file_exists(BEST_BOOT_NAME) or zip.file_exists("PSX.EXE") or zip.file_exists("psx.exe"):
		var exe_name := BEST_BOOT_NAME if zip.file_exists(BEST_BOOT_NAME) else "PSX.EXE"
		var exe_bytes := zip.read_file(exe_name)
		var cnf_text := ""
		if zip.file_exists("SYSTEM.CNF"):
			cnf_text = zip.read_file("SYSTEM.CNF").get_string_from_ascii()
		zip.close()
		return _evaluate_release(exe_name, exe_bytes.size(), compute_sha1(exe_bytes), cnf_text)
	
	# Check if ZIP contains CUE / BIN tracks
	var cue_file := ""
	var track_bin := ""
	for f in files:
		var low := f.to_lower()
		if low.ends_with(".cue"):
			cue_file = f
		elif low.ends_with(".bin") or low.ends_with(".iso") or low.ends_with(".img"):
			if track_bin.is_empty() or "(track 1)" in low:
				track_bin = f
	
	if not cue_file.is_empty():
		var cue_text := zip.read_file(cue_file).get_string_from_ascii()
		zip.close()
		# Inspect cue text for version heuristics
		if cue_text.contains("Rev 1") or cue_text.contains("SLPS-00064") or cue_text.contains("SLPS-00065"):
			return {
				"release": "rev1",
				"diagnostic": "Detected Rev 1 / Shokai Genteiban archive (SLPS-00064/SLPS-00065, 1995-08-31) in ZIP. This release has 521 changed functions and older PsyQ 3.x libraries. This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md.",
			}
		elif cue_text.contains("Rev 2"):
			return {
				"release": "rev2",
				"diagnostic": "Detected Rev 2 archive (SLPS-00065VER125, 1995-11-17) in ZIP. This release has 475 changed functions and older SDK libraries. This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md.",
			}
		elif cue_text.contains("Rev 4"):
			return {
				"release": "rev4",
				"diagnostic": "Detected Rev 4 archive (SLPS-00065VER143, 1996-03-31) in ZIP. This release has shifted data layout and 46 changed functions. This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md.",
			}
	zip.close()
	return {"error": "ZIP contains raw track archives. Please unpack the ZIP archive to provide the .cue/.bin or extracted folder directly"}

func _evaluate_release(exe_name: String, exe_size: int, exe_sha1: String, cnf_text: String, vol_id: String = "", vol_set_id: String = "") -> Dictionary:
	if exe_sha1 == BEST_EXE_SHA1:
		return {"release": "best", "label": "PlayStation the Best (SLPM-86053)"}
	
	if exe_sha1 == REV1_EXE_SHA1:
		var label := "Rev 1 (SLPS-00065, 1995-08-31)"
		if vol_set_id.contains("SLPS00064") or cnf_text.contains("SLPS-00064") or cnf_text.contains("SLPS-00067"):
			label = "Shokai Genteiban (SLPS-00064/SLPS-00067, 1995-08-31)"
		return {
			"release": "rev1",
			"diagnostic": "Detected %s with PSX.EXE (SHA1 %s). This release differs in 521 functions, older PsyQ 3.x libraries, and moved LBAs. This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md." % [label, exe_sha1.substr(0, 8)],
		}
	
	if exe_sha1 == REV2_EXE_SHA1:
		return {
			"release": "rev2",
			"diagnostic": "Detected Rev 2 (SLPS-00065VER125, 1995-11-17) with PSX.EXE (SHA1 %s). This release differs in 475 functions and older SDK libraries. This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md." % exe_sha1.substr(0, 8),
		}
	
	if exe_sha1 == REV4_EXE_SHA1:
		return {
			"release": "rev4",
			"diagnostic": "Detected Rev 4 (SLPS-00065VER143, 1996-03-31) with PSX.EXE (SHA1 %s). This release differs in 46 functions and shifted data symbols (rodata at 0x800AF1F0). This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md." % exe_sha1.substr(0, 8),
		}
	
	return {
		"release": "unknown",
		"diagnostic": "Unknown PS1 executable '%s' (size %d bytes, SHA1 %s). Expected PlayStation the Best (SLPM-86053 / VX009J2, SHA1 %s)." % [exe_name, exe_size, exe_sha1.substr(0, 8), BEST_EXE_SHA1.substr(0, 8)],
	}

func _read_source_file(rel_path: String) -> PackedByteArray:
	if _reader_kind == "directory":
		var full := _source_path.path_join(rel_path)
		if FileAccess.file_exists(full):
			return FileAccess.get_file_as_bytes(full)
		# Fallback case-insensitive
		for f in _list_all_files_recursive(_source_path):
			if f.to_upper() == rel_path.to_upper():
				return FileAccess.get_file_as_bytes(_source_path.path_join(f))
		return PackedByteArray()
	
	elif _reader_kind in ["cue", "image"]:
		var key := rel_path.to_upper()
		if not _iso_file_map.has(key):
			return PackedByteArray()
		var info: Dictionary = _iso_file_map[key]
		var sector_size := SECTOR_RAW if not _absent_raw_reported else SECTOR_ISO
		return _read_iso_extent(_source_file_access, info["lba"], info["size"], sector_size)
	
	elif _reader_kind == "zip":
		var zip := ZIPReader.new()
		if zip.open(_source_path) != OK:
			return PackedByteArray()
		var res := PackedByteArray()
		if zip.file_exists(rel_path):
			res = zip.read_file(rel_path)
		else:
			for f in zip.get_files():
				if f.to_upper() == rel_path.to_upper():
					res = zip.read_file(f)
					break
		zip.close()
		return res
	
	return PackedByteArray()

func _list_all_files_recursive(dir_path: String, prefix: String = "") -> Array[String]:
	var result: Array[String] = []
	var dir := DirAccess.open(dir_path)
	if dir == null:
		return result
	for f in dir.get_files():
		result.append((prefix + "/" + f).trim_prefix("/"))
	for d in dir.get_directories():
		var sub := (prefix + "/" + d).trim_prefix("/")
		result.append_array(_list_all_files_recursive(dir_path.path_join(d), sub))
	return result

func _index_iso_directory(file: FileAccess, root_lba: int, root_size: int, sector_size: int) -> Dictionary:
	var file_map: Dictionary = {}
	var queue := [{"prefix": "", "lba": root_lba, "size": root_size, "depth": 0}]
	while not queue.is_empty():
		var item: Dictionary = queue.pop_front()
		if item.depth > 8:
			continue
		var dir_buf := _read_iso_extent(file, item.lba, item.size, sector_size)
		var pos := 0
		while pos < dir_buf.size():
			var r_len := dir_buf[pos]
			if r_len == 0:
				pos = (int(pos / 2048) + 1) * 2048
				continue
			var rec := dir_buf.slice(pos, pos + r_len)
			if rec.size() < 33:
				pos += r_len
				continue
			var r_lba := rec.decode_u32(2)
			var r_size := rec.decode_u32(10)
			var flags := rec[25]
			var name_len := rec[32]
			if 33 + name_len <= rec.size() and rec[33] > 1:
				var raw_name := rec.slice(33, 33 + name_len).get_string_from_ascii()
				var name := raw_name.split(";")[0]
				if not name.is_empty():
					var full_path: String = (item.prefix + "/" + name).trim_prefix("/")
					if (flags & 2) != 0:
						queue.append({"prefix": full_path, "lba": r_lba, "size": r_size, "depth": item.depth + 1})
					else:
						file_map[full_path.to_upper()] = {"lba": r_lba, "size": r_size}
			pos += r_len
	return file_map

func _read_sector_data(file: FileAccess, lba: int, sector_size: int) -> PackedByteArray:
	if sector_size == SECTOR_ISO:
		file.seek(lba * SECTOR_ISO)
		return file.get_buffer(SECTOR_ISO)
	file.seek(lba * SECTOR_RAW + 16)
	var sub := file.get_buffer(8)
	if sub.size() < 8:
		return PackedByteArray()
	if sub.slice(0, 4) != sub.slice(4, 8):
		file.seek(lba * SECTOR_RAW + 16)
		return file.get_buffer(2048)
	if (sub[2] & 0x20) != 0:
		return file.get_buffer(2324)
	return file.get_buffer(2048)

func _read_iso_extent(file: FileAccess, lba: int, size: int, sector_size: int) -> PackedByteArray:
	var total_sectors := int((size + 2047) / 2048)
	var output := PackedByteArray()
	for i in range(total_sectors):
		output.append_array(_read_sector_data(file, lba + i, sector_size))
	return output.slice(0, size)

func _parse_cue_track(text: String) -> String:
	for line in text.split("\n"):
		var trimmed := line.strip_edges()
		if trimmed.begins_with("FILE ") and trimmed.contains('"'):
			var start := trimmed.find('"') + 1
			var end := trimmed.find('"', start)
			if end > start:
				return trimmed.substr(start, end - start)
	return ""

func _load_canonical_cells() -> Dictionary:
	var candidate_paths := [
		"res://schemas/canonical_prologue_nbn_cells.v1.json",
		ProjectSettings.globalize_path("res://schemas/canonical_prologue_nbn_cells.v1.json"),
		"/workspace/port/godot/schemas/canonical_prologue_nbn_cells.v1.json",
	]
	for p: String in candidate_paths:
		if FileAccess.file_exists(p):
			var text := FileAccess.get_file_as_string(p)
			var parsed: Variant = JSON.parse_string(text)
			if parsed is Dictionary and parsed.has("entries"):
				return parsed
	return {}

func _decode_nbn_cell_rgba(data: PackedByteArray, cell_index: int) -> PackedByteArray:
	var row := int(cell_index / 3)
	var col := int(cell_index % 3)
	var indices := PackedByteArray()
	indices.resize(128 * 120)
	
	if row == 0:
		var src_offset := col * 16384
		for y in range(120):
			var src_row := src_offset + y * 128
			var dst_row := y * 128
			for x in range(128):
				indices[dst_row + x] = data[src_row + x]
	else:
		# Upper 8 rows: page col, rows 120..127
		var upper_src := col * 16384 + 120 * 128
		for y in range(8):
			var src_row := upper_src + y * 128
			var dst_row := y * 128
			for x in range(128):
				indices[dst_row + x] = data[src_row + x]
		# Lower 112 rows: page col + 3, rows 0..111
		var lower_src := (col + 3) * 16384
		for y in range(112):
			var src_row := lower_src + y * 128
			var dst_row := (8 + y) * 128
			for x in range(128):
				indices[dst_row + x] = data[src_row + x]
	
	var rgba := PackedByteArray()
	rgba.resize(128 * 120 * 4)
	for i in range(128 * 120):
		var pal_entry: int = indices[i]
		var word: int = data.decode_u16(98304 + pal_entry * 2)
		var r5 := (word >> 0) & 31
		var g5 := (word >> 5) & 31
		var b5 := (word >> 10) & 31
		var r := (r5 << 3) | (r5 >> 2)
		var g := (g5 << 3) | (g5 >> 2)
		var b := (b5 << 3) | (b5 >> 2)
		var a := 0 if word == 0 else 255
		rgba[i * 4 + 0] = r
		rgba[i * 4 + 1] = g
		rgba[i * 4 + 2] = b
		rgba[i * 4 + 3] = a
	
	return rgba

static func compute_sha1(bytes: PackedByteArray) -> String:
	var h := HashingContext.new()
	if h.start(HashingContext.HASH_SHA1) != OK:
		return ""
	h.update(bytes)
	return h.finish().hex_encode().to_lower()

static func compute_sha256(bytes: PackedByteArray) -> String:
	var h := HashingContext.new()
	if h.start(HashingContext.HASH_SHA256) != OK:
		return ""
	h.update(bytes)
	return h.finish().hex_encode().to_lower()
