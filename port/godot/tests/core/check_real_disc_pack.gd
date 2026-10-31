extends SceneTree
## T-39303: checks the installed disc pack built from the user's own disc against
## known facts. Not part of the suite (it needs original content). Run in Docker:
##   DISC_PACK_ROOT=/pack BOARD_ROOT=/board godot --headless --path /workspace \
##     --script res://tests/core/check_real_disc_pack.gd
## DISC_PACK_ROOT is the pack directory (disc_pack.json, files/). BOARD_ROOT is the
## installed first_arrival_board role directory, whose o_11.nbn and cut_07.nbn are
## the bytes the opening already uses (the role names the 0xAA85 read after its slot,
## so its cut_07.nbn holds CDROM/PROLODIR/CUT_08.NBN). PROFILE_ROOT, when set, is an installed
## bundle with a disc_pack role, opened through InstalledLaunchProfile.

const DiscPack = preload("res://services/disc_pack.gd")
const Memory = preload("res://core/game_memory.gd")
const Profile = preload("res://services/installed_launch_profile.gd")

const O11_SHA := "3e901c18b0ed186323446151f35892bdb66e477d8a419e772034fd2b61be0628"

var failed: bool = false

func _initialize() -> void:
	call_deferred("_run")

func _check(condition: bool, message: String) -> void:
	print(("ok   " if condition else "FAIL ") + message)
	if not condition:
		failed = true

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	return hash.finish().hex_encode()

func _run() -> void:
	var pack := DiscPack.new()
	var profile_root: String = OS.get_environment("PROFILE_ROOT")
	if not profile_root.is_empty():
		var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(profile_root.path_join("manifest.json")))
		var profile: Dictionary = Profile.read_pack(profile_root, manifest if manifest is Dictionary else {})
		_check(profile.get("valid", false), "installed profile with a disc_pack role validates: " + str(profile.get("message", "")))
		var problem: String = pack.open_installed(profile)
		_check(problem.is_empty(), "open_installed with the inventoried manifest hash: " + problem)
	else:
		var problem: String = pack.open(OS.get_environment("DISC_PACK_ROOT"))
		_check(problem.is_empty(), "open the pack: " + problem)
	if not pack.is_open():
		quit(1)
		return
	print("pack: %d files, %d excluded, volume %s, manifest %s" % [pack.file_count(), pack.excluded_count(), pack.volume_id, pack.manifest_sha256])
	var verified: Dictionary = pack.verify_all()
	_check(verified.ok, "every packed file matches its SHA-256 (%s files, %s bytes)" % [verified.get("files"), verified.get("bytes")])
	var exe: Dictionary = pack.lookup_path("SLPM_86.053")
	_check(not exe.is_empty() and int(exe.size) == 667648, "SLPM_86.053 is packed, 667648 bytes")
	var board: String = OS.get_environment("BOARD_ROOT")

	# Stage 10 O11: CDROM/PROLODIR/O_11.NBN at LBA 0xAC1E, 33 sectors, into 0x801E8000.
	var o11: Dictionary = pack.lookup_path("CDROM/PROLODIR/O_11.NBN")
	_check(int(o11.get("lba", 0)) == 0xAC1E and int(o11.get("sectors", 0)) == 33 and int(o11.get("size", 0)) == 67584, "O_11.NBN is at LBA 0xAC1E, 33 sectors, 67584 bytes")
	_check(pack.lookup_lba(0xAC1E).get("path") == "CDROM/PROLODIR/O_11.NBN" and pack.lookup_lba(0xAC3E).get("path") == "CDROM/PROLODIR/O_11.NBN", "LBA lookup names O_11.NBN across its extent")
	var read: Dictionary = pack.read_sectors(0xAC1E, 33)
	_check(read.ok and read.data.size() == 67584 and _sha(read.data) == O11_SHA, "read_sectors(0xAC1E, 33) hashes to the pinned O11 SHA-256")
	var memory := Memory.new()
	var into: Dictionary = pack.read_into_memory(memory, 0x801E8000, 0xAC1E, 33)
	_check(into.ok and _sha(memory.read_bytes(0x801E8000, 67584)) == O11_SHA and memory.error.is_empty(), "read_into_memory puts O11 at 0x801E8000")
	if not board.is_empty():
		var role: PackedByteArray = FileAccess.get_file_as_bytes(board.path_join("o_11.nbn"))
		_check(role.size() == 67584 and read.ok and role == read.data, "O11 equals the bytes of the first_arrival_board role (o_11.nbn)")

	# Stage 9 CUT08: LBA 0xAA85, 49 sectors, into 0x801A4000.
	var cut: Dictionary = pack.lookup_path("CDROM/PROLODIR/CUT_08.NBN")
	_check(int(cut.get("lba", 0)) == 0xAA85 and int(cut.get("sectors", 0)) == 49, "CUT_08.NBN is at LBA 0xAA85, 49 sectors")
	var cut_read: Dictionary = pack.read_sectors(0xAA85, 49)
	_check(cut_read.ok and cut_read.data.size() == 49 * 2048 and cut_read.path == "CDROM/PROLODIR/CUT_08.NBN", "read_sectors(0xAA85, 49) reads CUT_08.NBN")
	var cut_memory := Memory.new()
	_check(pack.read_into_memory(cut_memory, 0x801A4000, 0xAA85, 49).ok and cut_memory.read_bytes(0x801A4000, 49 * 2048) == cut_read.data, "read_into_memory puts CUT08 at 0x801A4000")
	if not board.is_empty():
		var cut_role: PackedByteArray = FileAccess.get_file_as_bytes(board.path_join("cut_07.nbn"))
		_check(cut_role.size() > 0 and cut_read.ok and cut_read.data.slice(0, cut_role.size()) == cut_role and (cut_role.size() == 49 * 2048 or cut_read.data.slice(cut_role.size()).count(0) == cut_read.data.size() - cut_role.size()), "CUT08 equals the first_arrival_board role (cut_07.nbn, the 0xAA85 read), %d bytes" % cut_role.size())

	# Refusals on the real pack.
	var xa: Dictionary = pack.excluded_at(55974)
	_check(not xa.is_empty() and not pack.read_sectors(55974, 1).ok and pack.read_sectors(55974, 1).error.contains("not readable as data"), "an XA stream sector fails: " + str(pack.read_sectors(55974, 1).get("error")))
	_check(not pack.read_sectors(0xAC1E, 34).ok, "a read past the end of O_11.NBN fails: " + str(pack.read_sectors(0xAC1E, 34).get("error")))
	_check(not pack.read_sectors(3, 1).ok, "an LBA before the first file fails: " + str(pack.read_sectors(3, 1).get("error")))
	quit(1 if failed else 0)
