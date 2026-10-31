extends RefCounted
## T-39303 (M2): services/disc_pack.gd against a synthetic pack written to /tmp.
## The fixture holds invented bytes only. The real pack is checked by
## tools/port/check_disc_pack.py's Godot counterpart, tests/core/check_real_disc_pack.gd,
## which is not part of the suite (it needs the user's disc).

const DiscPack = preload("res://services/disc_pack.gd")
const Memory = preload("res://core/game_memory.gd")

var failures: Array[String] = []
var base: String = ""

func run() -> Array[String]:
	failures.clear()
	base = "/tmp/tokimemo-disc-pack-test-%d" % Time.get_ticks_usec()
	_test_open_and_lookup()
	_test_reads()
	_test_read_into_memory()
	_test_refusals()
	_test_integrity()
	_test_cache()
	_test_bad_manifests()
	_remove_tree(base)
	return failures

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

## Deterministic invented content: byte i of a file is (seed * 31 + i * 7) & 0xff.
static func _content(seed_value: int, length: int) -> PackedByteArray:
	var bytes := PackedByteArray()
	bytes.resize(length)
	for i: int in length:
		bytes[i] = (seed_value * 31 + i * 7) & 0xff
	return bytes

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	return hash.finish().hex_encode()

## Files: A at LBA 100 (3 sectors, ISO size 5000), B at LBA 103 (adjacent, 2 sectors),
## C at LBA 200 (1 sector, size 2048). LBA 105..199 is a gap; XA at 300 (4 sectors) is excluded.
func _build(label: String, mutate: Callable = Callable()) -> String:
	var dir: String = base.path_join(label)
	DirAccess.make_dir_recursive_absolute(dir.path_join("files/CDROM/SUB"))
	var specs: Array = [["CDROM/SUB/A.BIN", 100, 3, 5000, 1], ["CDROM/SUB/B.BIN", 103, 2, 4096, 2], ["C.DAT", 200, 1, 2048, 3]]
	var records: Array = []
	for spec: Array in specs:
		var stored: PackedByteArray = _content(spec[4], int(spec[2]) * 2048)
		# Past the ISO size the stored tail is sector padding, not file content.
		for i: int in range(int(spec[3]), stored.size()):
			stored[i] = 0xee
		var file: FileAccess = FileAccess.open(dir.path_join("files").path_join(spec[0]), FileAccess.WRITE)
		file.store_buffer(stored)
		file.close()
		records.append({"path": spec[0], "lba": spec[1], "sectors": spec[2], "size": spec[3],
			"stored_size": stored.size(), "mode": 2, "form": 1, "sha256": _sha(stored),
			"content_sha256": _sha(stored.slice(0, int(spec[3])))})
	var manifest: Dictionary = {"schema": DiscPack.SCHEMA, "version": 1, "sector_size": 2048,
		"volume_id": "SYNTH", "source": {}, "files": records,
		"excluded": [{"path": "XA/S.XA", "lba": 300, "sectors": 4, "size": 8192,
			"reason": "form 2 stream (XA audio or STR video), not readable as data"}],
		"totals": {}}
	if mutate.is_valid():
		mutate.call(manifest)
	var out: FileAccess = FileAccess.open(dir.path_join(DiscPack.MANIFEST_NAME), FileAccess.WRITE)
	out.store_string(JSON.stringify(manifest))
	out.close()
	return dir

func _open(label: String, mutate: Callable = Callable()) -> DiscPack:
	var pack := DiscPack.new()
	var problem: String = pack.open(_build(label, mutate))
	_check(problem.is_empty(), "synthetic pack %s opens: %s" % [label, problem])
	return pack

func _test_open_and_lookup() -> void:
	var pack: DiscPack = _open("good")
	_check(pack.is_open() and pack.file_count() == 3 and pack.excluded_count() == 1, "pack opens with 3 files and 1 exclusion")
	_check(pack.volume_id == "SYNTH" and pack.manifest_sha256.length() == 64, "volume id and manifest hash recorded")
	var a: Dictionary = pack.lookup_path("CDROM/SUB/A.BIN")
	_check(int(a.lba) == 100 and int(a.sectors) == 3 and int(a.size) == 5000, "lookup by path")
	_check(pack.lookup_lba(102).path == "CDROM/SUB/A.BIN" and pack.lookup_lba(103).path == "CDROM/SUB/B.BIN", "lookup by LBA at a file boundary")
	_check(pack.lookup_lba(104).path == "CDROM/SUB/B.BIN" and pack.lookup_lba(200).path == "C.DAT", "lookup by LBA, last and single sector")
	_check(pack.lookup_lba(99).is_empty() and pack.lookup_lba(105).is_empty() and pack.lookup_lba(201).is_empty(), "no file outside the extents")
	_check(pack.lookup_path("missing").is_empty(), "unknown path gives {}")
	_check(pack.excluded_at(301).path == "XA/S.XA" and pack.excluded_at(304).is_empty(), "excluded extent lookup")
	var a_copy: Dictionary = pack.lookup_path("C.DAT")
	a_copy["lba"] = 1
	_check(int(pack.lookup_path("C.DAT").lba) == 200, "lookups return copies")
	var expected: String = pack.manifest_sha256
	var again := DiscPack.new()
	_check(again.open(base.path_join("good"), expected).is_empty(), "open accepts the matching manifest hash")
	var profile: Dictionary = {"valid": true, "paths": {"disc_pack": base.path_join("good")},
		"role_manifests": {"disc_pack": {"sha256": expected}}}
	var installed := DiscPack.new()
	_check(installed.open_installed(profile).is_empty() and installed.file_count() == 3, "open_installed uses the profile path and manifest hash")
	profile.role_manifests.disc_pack.sha256 = "1".repeat(64)
	_check(not DiscPack.new().open_installed(profile).is_empty(), "open_installed refuses a manifest hash that differs")
	_check(not DiscPack.new().open_installed({"valid": true, "paths": {}, "role_manifests": {}}).is_empty(), "open_installed refuses a profile without the role")
	var wrong := DiscPack.new()
	_check(wrong.open(base.path_join("good"), "0".repeat(64)).contains("manifest hash") and not wrong.is_open(), "open refuses a different manifest hash")

func _test_reads() -> void:
	var pack: DiscPack = _open("reads")
	var whole: Dictionary = pack.read_sectors(100, 3)
	var stored_a: PackedByteArray = _content(1, 3 * 2048)
	for i: int in range(5000, stored_a.size()):
		stored_a[i] = 0xee
	_check(whole.ok and whole.data == stored_a and whole.path == "CDROM/SUB/A.BIN", "whole-file read returns the stored sectors")
	var middle: Dictionary = pack.read_sectors(101, 1)
	_check(middle.ok and middle.data == stored_a.slice(2048, 4096), "single sector in the middle of a file")
	var tail: Dictionary = pack.read_sectors(102, 1)
	_check(tail.ok and tail.data.size() == 2048 and tail.data[903] == stored_a[4999] and tail.data[904] == 0xee and tail.data[2047] == 0xee, "the last sector keeps its padding past the ISO size")
	var other: Dictionary = pack.read_sectors(104, 1)
	_check(other.ok and other.data == _content(2, 2 * 2048).slice(2048, 4096), "read in a second file")
	var file: Dictionary = pack.read_file("CDROM/SUB/A.BIN")
	_check(file.ok and file.data == stored_a.slice(0, 5000), "read_file returns the ISO size bytes")
	_check(pack.read_file("C.DAT").ok and pack.read_sectors(200, 1).ok, "single sector file")
	_check(not pack.read_file("nope").ok, "read_file of an unknown path fails")
	var verified: Dictionary = pack.verify_all()
	_check(verified.ok and int(verified.files) == 3 and int(verified.bytes) == 6 * 2048, "verify_all hashes every file")

func _test_read_into_memory() -> void:
	var pack: DiscPack = _open("memory")
	var memory := Memory.new()
	var result: Dictionary = pack.read_into_memory(memory, 0x801E8000, 103, 2)
	var stored_b: PackedByteArray = _content(2, 2 * 2048)
	_check(result.ok and int(result.bytes) == 4096 and result.path == "CDROM/SUB/B.BIN", "read into memory reports path and byte count")
	_check(memory.read_bytes(0x801E8000, 4096) == stored_b and memory.u8(0x801E8000 + 4096) == 0 and memory.u8(0x801E7FFF) == 0, "bytes land at the address and nowhere else")
	_check(memory.error.is_empty(), "no memory fault")
	var alias := Memory.new()
	_check(pack.read_into_memory(alias, 0xA0010000, 200, 1).ok and alias.read_bytes(0x10000, 2048) == _content(3, 2048), "KSEG1 destination is accepted")
	var before: PackedByteArray = memory.snapshot()
	var high: Dictionary = pack.read_into_memory(memory, 0x801FF800, 103, 2)
	_check(not high.ok and high.error.contains("outside main RAM") and memory.snapshot() == before, "a destination running past RAM is refused, nothing written")
	var bad_segment: Dictionary = pack.read_into_memory(memory, 0xC0000000, 200, 1)
	_check(not bad_segment.ok and memory.snapshot() == before, "KSEG2 destination is refused")
	var gap: Dictionary = pack.read_into_memory(memory, 0x80100000, 150, 1)
	_check(not gap.ok and gap.error.contains("outside every file") and memory.snapshot() == before, "a failed read writes nothing")
	var preset := Memory.new()
	preset.write_u32(0x80000002, 1)
	var pre_error: String = preset.error
	_check(not pre_error.is_empty() and pack.read_into_memory(preset, 0x80100000, 200, 1).ok and preset.error == pre_error, "an earlier latched fault stays and does not fail a good read")

func _test_refusals() -> void:
	var pack: DiscPack = _open("refusals")
	var outside: Dictionary = pack.read_sectors(150, 1)
	_check(not outside.ok and outside.error.contains("outside every file") and not outside.has("data"), "LBA in a gap fails with no data")
	_check(not pack.read_sectors(99, 1).ok and not pack.read_sectors(0, 1).ok and not pack.read_sectors(201, 1).ok, "LBAs before, between and after files fail")
	var crossing: Dictionary = pack.read_sectors(102, 2)
	var stored_a: PackedByteArray = _content(1, 3 * 2048)
	for i: int in range(5000, stored_a.size()):
		stored_a[i] = 0xee
	var joined: PackedByteArray = stored_a.slice(2 * 2048)
	joined.append_array(_content(2, 2 * 2048).slice(0, 2048))
	_check(crossing.ok and crossing.data == joined and crossing.path == "CDROM/SUB/A.BIN", "a read across two adjacent files returns both, A's last sector then B's first")
	var five: Dictionary = pack.read_sectors(100, 5)
	_check(five.ok and five.data.size() == 5 * 2048, "a read over two whole adjacent files")
	var into_gap: Dictionary = pack.read_sectors(104, 2)
	_check(not into_gap.ok and into_gap.error.contains("runs past the end of CDROM/SUB/B.BIN") and into_gap.error.contains("outside every file") and not into_gap.has("data"), "a read that ends in a gap after the last adjacent file fails")
	_check(not pack.read_sectors(100, 6).ok, "a read past the last adjacent file fails")
	var tight: DiscPack = _open("excluded-adjacent", func(m: Dictionary) -> void: m.excluded[0].lba = 201)
	var into_xa: Dictionary = tight.read_sectors(200, 2)
	_check(not into_xa.ok and into_xa.error.contains("XA/S.XA") and into_xa.error.contains("not readable as data"), "a read running into an excluded extent fails and names it")
	var long: Dictionary = pack.read_sectors(100, 6)
	_check(not long.ok and long.error.contains("runs past the end"), "a read past the end of the file fails")
	var xa: Dictionary = pack.read_sectors(300, 1)
	_check(not xa.ok and xa.error.contains("XA/S.XA") and xa.error.contains("not readable as data"), "an XA/Form 2 extent fails and names the file")
	_check(not pack.read_sectors(302, 2).ok, "a read inside the XA extent fails")
	_check(not pack.read_sectors(100, 0).ok and not pack.read_sectors(100, -1).ok, "zero and negative counts fail")
	_check(not pack.read_sectors(100, DiscPack.MAX_READ_SECTORS + 1).ok, "a count over the bound fails")
	_check(not pack.read_sectors(-5, 1).ok, "a negative LBA fails")
	var closed := DiscPack.new()
	_check(not closed.read_sectors(100, 1).ok and not closed.is_open(), "an unopened pack refuses reads")

func _test_integrity() -> void:
	var dir: String = _build("integrity")
	var pack := DiscPack.new()
	_check(pack.open(dir).is_empty() and pack.read_sectors(100, 1).ok and pack.read_sectors(200, 1).ok, "pack reads before tampering")
	# A corrupt byte in a file that was not read yet is caught on its first read.
	var bpath: String = dir.path_join("files/CDROM/SUB/B.BIN")
	var file: FileAccess = FileAccess.open(bpath, FileAccess.READ_WRITE)
	file.seek(3000)
	file.store_8(0x5a)
	file.close()
	var bad: Dictionary = pack.read_sectors(103, 1)
	_check(not bad.ok and bad.error.contains("SHA-256") and not bad.has("data"), "a changed file fails its hash on read")
	_check(not pack.read_file("CDROM/SUB/B.BIN").ok, "read_file also checks the hash")
	var memory := Memory.new()
	var before: PackedByteArray = memory.snapshot()
	_check(not pack.read_into_memory(memory, 0x80100000, 103, 1).ok and memory.snapshot() == before, "a bad hash writes nothing into memory")
	var spanning: Dictionary = pack.read_sectors(102, 2)
	_check(not spanning.ok and spanning.error.contains("SHA-256") and not spanning.has("data"), "a read spanning into a changed file fails")
	_check(pack.read_sectors(100, 3).ok and pack.read_sectors(200, 1).ok, "other files still read")
	var all_files := DiscPack.new()
	_check(all_files.open(dir).is_empty() and not all_files.verify_all().ok, "verify_all reports the changed file")
	# A truncated or missing file fails (a reader that already verified a file keeps its bytes).
	var cpath: String = dir.path_join("files/C.DAT")
	var cut: FileAccess = FileAccess.open(cpath, FileAccess.WRITE)
	cut.store_buffer(_content(3, 1000))
	cut.close()
	var truncated := DiscPack.new()
	_check(truncated.open(dir).is_empty() and truncated.read_sectors(200, 1).error.contains("bytes, expected 2048"), "a truncated file fails on its size")
	DirAccess.remove_absolute(cpath)
	var missing := DiscPack.new()
	_check(missing.open(dir).is_empty() and missing.read_sectors(200, 1).error.contains("missing"), "a missing file fails")
	# A fresh reader never serves tampered bytes.
	var fresh := DiscPack.new()
	_check(fresh.open(dir).is_empty() and not fresh.read_sectors(104, 1).ok, "a new reader refuses the changed file")

func _test_cache() -> void:
	var dir: String = _build("cache")
	var pack := DiscPack.new()
	_check(pack.open(dir).is_empty() and pack.read_sectors(100, 1).ok and pack.read_sectors(200, 1).ok and pack.read_sectors(103, 1).ok, "three files read and verify")
	# Alternating reads use the verified bytes, so a later change on disk is not seen by this reader.
	var file: FileAccess = FileAccess.open(dir.path_join("files/CDROM/SUB/A.BIN"), FileAccess.READ_WRITE)
	file.store_8(0x5a)
	file.close()
	_check(pack.read_sectors(100, 1).ok and pack.read_sectors(200, 1).ok and pack.read_sectors(100, 1).ok, "alternating reads of verified files are served from the cache")
	_check(pack._cache.size() == 3 and pack._cache_size == 6 * 2048, "the cache holds all three verified files")
	_check(not _open_and_read(dir, 100), "a new reader still hashes the changed file")

func _open_and_read(dir: String, lba: int) -> bool:
	var pack := DiscPack.new()
	return pack.open(dir).is_empty() and pack.read_sectors(lba, 1).ok

func _test_bad_manifests() -> void:
	var cases: Dictionary = {
		"overlap": func(m: Dictionary) -> void: m.files[1].lba = 101,
		"duplicate": func(m: Dictionary) -> void: m.files[1].path = m.files[0].path,
		"unsafe": func(m: Dictionary) -> void: m.files[0].path = "../A.BIN",
		"absolute": func(m: Dictionary) -> void: m.files[0].path = "/A.BIN",
		"form2": func(m: Dictionary) -> void: m.files[0].form = 2,
		"mode1": func(m: Dictionary) -> void: m.files[0].mode = 1,
		"sectors": func(m: Dictionary) -> void: m.files[0].sectors = 4,
		"stored": func(m: Dictionary) -> void: m.files[0].stored_size = 5000,
		"hash": func(m: Dictionary) -> void: m.files[0].sha256 = "xyz",
		"extra_key": func(m: Dictionary) -> void: m.files[0]["note"] = 1,
		"fraction": func(m: Dictionary) -> void: m.files[0].lba = 100.5,
		"negative": func(m: Dictionary) -> void: m.files[0].lba = -1,
		"schema": func(m: Dictionary) -> void: m.schema = "other",
		"version": func(m: Dictionary) -> void: m.version = 2,
		"sector_size": func(m: Dictionary) -> void: m.sector_size = 2352,
		"empty": func(m: Dictionary) -> void: m.files = [],
		"excluded": func(m: Dictionary) -> void: m.excluded = [{"path": "x"}],
	}
	for label: String in cases:
		var pack := DiscPack.new()
		var problem: String = pack.open(_build("bad-" + label, cases[label]))
		_check(not problem.is_empty() and not pack.is_open(), "manifest case %s is refused" % label)
	var garbage: String = base.path_join("garbage")
	DirAccess.make_dir_recursive_absolute(garbage)
	var out: FileAccess = FileAccess.open(garbage.path_join(DiscPack.MANIFEST_NAME), FileAccess.WRITE)
	out.store_string("not json")
	out.close()
	_check(not DiscPack.new().open(garbage).is_empty(), "a manifest that is not JSON is refused")
	_check(not DiscPack.new().open(base.path_join("absent")).is_empty(), "a missing directory is refused")
	_check(not DiscPack.new().open("res://").is_empty(), "a res:// directory is refused")

func _remove_tree(path: String) -> void:
	var directory := DirAccess.open(path)
	if directory == null:
		return
	for name: String in directory.get_directories():
		_remove_tree(path.path_join(name))
	for name: String in directory.get_files():
		DirAccess.remove_absolute(path.path_join(name))
	DirAccess.remove_absolute(path)
