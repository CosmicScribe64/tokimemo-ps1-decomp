extends RefCounted
## Additive original-board resource reader. No CD/GPU/audio completion events.
const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.first-arrival-board.v1"
const SOURCE_SHA := "61b0cd543a73c64ff9ad6d1e77d48217c02dbfbe7dab4941e90451b76f20f398"
const MANIFEST_SHA := "c42c34a87d01bb65de91a2998a49ae0964eb06bf31d9284d4b5c564c0b629037"
const SOFTWARE_WORD_SHA := "fbdc0fa3d2058d316682eef1e2ca2b40131af61a63bc760e6174765f2368668e"
const BG_495D_ID := "prologue.names.0x495d"
const BG_495D_SOURCE_SHA := "aa2e82335720f7091a54ccc26e7e0a94be3a75fa1513d6bc1d5e1f3e36e5b230"
const BG_495D_TRANSFER_SHA := "064fa4988819e19c9c858500d4f49caa6a96112d99d04793f5e088c278ed4e50"
const BG_495D_WORD_SHA := "4f7947c50e1e9b1837041dba9b53b522d21f3945eb9706562bd8eb1bc391eac8"
const BG_4966_ID := "prologue.names.0x4966"
const BG_4966_SOURCE_SHA := "090ad2700ac094805298eabeb90560308e89ad7cca098c27146a24b8181b5c92"
const BG_4966_TRANSFER_SHA := "1216a2ea05373a751e76db3f05d2c98b1206dc651d3b93d638964376fb83c68b"
const BG_4966_WORD_SHA := "b07f63c896515cdbe718b788986aac2cc388ba6f99af14140a20d86cdeb19ac5"
const BG_ACDD_ID := "prologue.names.0xacdd"
const BG_ACDD_SOURCE_SHA := "ffa02dc52c645679bdbfd66739254e1088fd58672321acbefa2d671c870f2ba2"
const BG_ACDD_TRANSFER_SHA := "67d96aca10962b5379aa9193a996b2eed2f659b01514097ba5b06118ea9a91ea"
const BG_ACDD_WORD_SHA := "6a0cbd6a40f16020e58867866135ef64b8b6904216c0c2778704750735ee909e"
const BG_ACE6_ID := "prologue.names.0xace6"
const BG_ACE6_SOURCE_SHA := "d082130abe5c199b7d8c9c75add436ce0f0f33b23e6a58e3e2e06782aa41119e"
const BG_ACE6_TRANSFER_SHA := "626af576b1adcb054e360b2e2bfca4a5bd1b7f9f94662f42310aaada3d976bd4"
const BG_ACE6_WORD_SHA := "364305d3e2be6344a9ee8aec0176d5e8bca44213e9ed57cd35334cf8a74e47a1"
const BG_ACF6_ID := "prologue.names.0xacf6"
const BG_ACF6_SOURCE_SHA := "a4f19c201f7650e14be9c1eca419d3055c7704d6785f1a8263cd3f1eea559980"
const BG_ACF6_TRANSFER_SHA := "f6f921e4a85c00d30c9c63c55a1bb6ac70425de4433c7b0028e4a1e30e802e79"
const BG_ACF6_WORD_SHA := "846c23fd646da4ce2d377a9ea964ba1e7b694968a70cc088077a60b7ed372f3a"
const BG_ACD4_ID := "prologue.names.0xacd4"
const BG_ACD4_SOURCE_SHA := "dcf99efbac821fbb769847204c0e8a1a1b611ed15daa0450700a59257387fae6"
const BG_ACD4_TRANSFER_SHA := "a2f0c71eb50a70d2028aa48f6bcfaea65b5524369eb9cc5aaf1362f9a7fb340a"
const BG_ACD4_WORD_SHA := "aa35d977abba6036c12c6a0116c7189b2f1bea82982b2828f13ee3760e73c0da"
const BG_ACEC_ID := "prologue.names.0xacec"
const BG_ACEC_SOURCE_SHA := "5feccf3ed08b49ed918458c4772c870b8c1533e7fbc0391df314e59172c7858c"
const BG_ACEC_TRANSFER_SHA := "f2acadc280ae1d94841c9c08f455c5ece893dc371988ff56bcceacbfe68d80b9"
const BG_ACEC_WORD_SHA := "d6578d0ae294bf05bde4072d2ca409dc4070d8ab67c1519dec0563f510d1db11"
const CUT_12_ID := "prologue.cut_12.0xab18"
const CUT_12_SHA := "43b8e386cd4588a67c3a8ac9c71c7ac0416bfce3a340cb502ac508764606bd97"
const O_12_ID := "prologue.o_12.0xac3f"
const O_12_SHA := "b2458308e1b1de9714a59a49239728b4be60b4eaff30100566e155c2403c1954"
const CUT_05_ID := "prologue.cut_05.0xaa23"
const CUT_05_SHA := "34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c"
const O_05_ID := "prologue.o_05.0xabb8"
const O_05_SHA := "59bd16f64d1510ab290dc4af732418a91534c16eee313f1e0540d8890a606ac5"
const CUT_06_ID := "prologue.cut_06.0xaa54"
const CUT_06_SHA := "3fff84684395c29849d57652a111282820977b4ca5fd44006deab14b3eefa125"
const O_06_ID := "prologue.o_06.0xabca"
const O_06_SHA := "fb0053368ca62fa7ba2f4de8615b9a8e631284ee35ea952472cc62e74c506715"
const CUT_07_ID := "prologue.cut_07.0xaa85"
const CUT_07_SHA := "444165354346eeac36f2614a3375bd24e3ff5b457924b1d81705b4e174c3e3ca"
const O_07_ID := "prologue.o_07.0xabe3"
const O_07_SHA := "239a20180cd00bd8c117603075505147fff5d1e1f5769eb94323c7d0fd78de77"
const FILES := [
	"board.bs", "board.rgb555.bin", "board.background.png", "board.background.stp.png", "board.background.padded.png", "board.background.padded.stp.png",
	"names_495d.bs", "names_495d.transfer.bin", "names_495d.rgb555.bin", "names_495d.background.png", "names_495d.background.stp.png", "names_495d.background.padded.png", "names_495d.background.padded.stp.png",
	"names_4966.bs", "names_4966.transfer.bin", "names_4966.rgb555.bin", "names_4966.background.png", "names_4966.background.stp.png", "names_4966.background.padded.png", "names_4966.background.padded.stp.png",
	"names_acdd.bs", "names_acdd.transfer.bin", "names_acdd.rgb555.bin", "names_acdd.background.png", "names_acdd.background.stp.png", "names_acdd.background.padded.png", "names_acdd.background.padded.stp.png",
	"names_ace6.bs", "names_ace6.transfer.bin", "names_ace6.rgb555.bin", "names_ace6.background.png", "names_ace6.background.stp.png", "names_ace6.background.padded.png", "names_ace6.background.padded.stp.png",
	"names_acf6.bs", "names_acf6.transfer.bin", "names_acf6.rgb555.bin", "names_acf6.background.png", "names_acf6.background.stp.png", "names_acf6.background.padded.png", "names_acf6.background.padded.stp.png",
	"names_acd4.bs", "names_acd4.transfer.bin", "names_acd4.rgb555.bin", "names_acd4.background.png", "names_acd4.background.stp.png", "names_acd4.background.padded.png", "names_acd4.background.padded.stp.png",
	"names_acec.bs", "names_acec.transfer.bin", "names_acec.rgb555.bin", "names_acec.background.png", "names_acec.background.stp.png", "names_acec.background.padded.png", "names_acec.background.padded.stp.png",
	"cut_12.nbn", "o_12.nbn", "cut_05.nbn", "o_05.nbn", "cut_06.nbn", "o_06.nbn", "cut_07.nbn", "o_07.nbn",
]
var _root := ""
var _manifest_sha := ""
var _read_bytes := PackedByteArray()
var _words := PackedByteArray()
var _preview: Image
var _bg_transfers: Dictionary = {}
var _bg_uploads: Dictionary = {}
var _bg_previews: Dictionary = {}
var _stage_transfers: Dictionary = {}

static func _sha(value: Variant) -> bool:
	if not value is String or value.length() != 64: return false
	for c in value:
		if not "0123456789abcdef".contains(c): return false
	return true

static func _integer(value: Variant, wanted: int) -> bool:
	return (value is int or value is float) and is_finite(float(value)) and value == wanted

static func _rect_equals(value: Variant, wanted: Array) -> bool:
	if not value is Array or value.size() != 4: return false
	for i in range(4):
		if not _integer(value[i],wanted[i]): return false
	return true

static func _check_mdec_group(bytes: Dictionary, prefix: String, w: int, h: int, expected_word_sha: String, word_sha_actual: String) -> Variant:
	var words: PackedByteArray = bytes[prefix+".rgb555.bin"]
	if words.size() != w*h*2 or word_sha_actual != expected_word_sha:
		return prefix+" word transfer extent differs"
	for i in range(w*h):
		if (words.decode_u16(i*2)&0x8000) == 0: return prefix+" decoded mode2 output requires bit15 on every source word"
	var images: Dictionary = {}
	for suffix: String in [".background.png",".background.stp.png",".background.padded.png",".background.padded.stp.png"]:
		var name := prefix+suffix
		var image := Image.new()
		if image.load_png_from_buffer(bytes[name]) != OK or image.get_size() != Vector2i(w,h): return prefix+" PNG geometry/encoding differs"
		image.convert(Image.FORMAT_RGBA8)
		images[suffix] = image
	for y in range(h):
		for x in range(w):
			var word := words.decode_u16((y*w+x)*2)
			var color: Color = images[".background.png"].get_pixel(x,y)
			if color.a8 != 255 or (color.r8>>3) != (word&31) or (color.g8>>3) != ((word>>5)&31) or (color.b8>>3) != ((word>>10)&31): return prefix+" canonical preview and source word payload differ"
			if images[".background.padded.png"].get_pixel(x,y) != color: return prefix+" padded extent must equal active160 rows"
			if images[".background.stp.png"].get_pixel(x,y).r8 != 255 or images[".background.padded.stp.png"].get_pixel(x,y).r8 != 255: return prefix+" mode2 STP mask differs"
	return images[".background.png"]

static func _check_names_bg_manifest(entry: Variant, path: String, src_sha: String, src_bytes: int, lba: int, xfer_sha: String, buf: int, rect: Array) -> bool:
	if not entry is Dictionary: return false
	var source: Variant = entry.get("source")
	var read: Variant = entry.get("read")
	var decode: Variant = entry.get("decode")
	if not source is Dictionary or source.get("path") != path or source.get("sha256") != src_sha or not _integer(source.get("bytes"),src_bytes): return false
	if not read is Dictionary or not _integer(read.get("lba"),lba) or not _integer(read.get("sectors"),10) or not _integer(read.get("destination"),0x80162000) or read.get("transfer_sha256") != xfer_sha or not _integer(read.get("transfer_bytes"),20480): return false
	if not decode is Dictionary or not _integer(decode.get("slot"),1) or not _integer(decode.get("buffer"),buf) or not _rect_equals(decode.get("destination"),rect) or decode.get("format") != "rgb555-stp1" or not decode.has("clut") or decode.clut != null or not decode.get("hardware_pixel_parity_verified") is bool or decode.hardware_pixel_parity_verified: return false
	return true

static func _check_stage_transfer_manifest(entry: Variant, file_name: String, path: String, sha: String, total_bytes: int, lba: int, sectors: int, dest: int) -> bool:
	if not entry is Dictionary or entry.get("file") != file_name: return false
	var source: Variant = entry.get("source")
	var read: Variant = entry.get("read")
	if not source is Dictionary or source.get("path") != path or source.get("sha256") != sha or not _integer(source.get("bytes"),total_bytes): return false
	if not read is Dictionary or not _integer(read.get("lba"),lba) or not _integer(read.get("sectors"),sectors) or not _integer(read.get("destination"),dest) or read.get("transfer_sha256") != sha or not _integer(read.get("transfer_bytes"),total_bytes): return false
	return true

func open_pack(directory: String, expected_manifest_sha256: String) -> String:
	if not _root.is_empty(): return "board reader already owns an immutable prepared role"
	if not ExternalPath.is_external_path(directory) or not DirAccess.dir_exists_absolute(directory) or not _sha(expected_manifest_sha256) or expected_manifest_sha256 != MANIFEST_SHA:
		return "board role requires a canonical external directory and reviewed manifest pin"
	var manifest_path := directory.path_join("manifest.json")
	if not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > 1024*1024 or FileAccess.get_sha256(manifest_path) != expected_manifest_sha256:
		return "board role manifest is missing, linked, oversized or changed"
	var m: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not m is Dictionary or not m.get("schema") is String or m.schema != SCHEMA or not m.get("release") is String or m.release != "original-ja-ps-the-best":
		return "board role schema/release differs"
	if not m.get("service_readiness_granted") is bool or m.service_readiness_granted or not m.get("scene_execution_verified") is bool or m.scene_execution_verified:
		return "board resource metadata cannot grant service or scene readiness"
	var source: Variant = m.get("source")
	if not source is Dictionary or not source.get("path") is String or source.path != "CDROM/BS/P_01_02.BS" or not source.get("sha256") is String or source.sha256 != SOURCE_SHA or not _integer(source.get("bytes"),18432):
		return "board source file identity differs"
	var read: Variant = m.get("read")
	if not read is Dictionary or read.size() != 3 or not _integer(read.get("lba"),0x4954) or not _integer(read.get("sectors"),9) or not _integer(read.get("destination"),0x80162000):
		return "board CD read must preserve native buffer and exact nine sectors"
	var decode: Variant = m.get("decode")
	if not decode is Dictionary or not _rect_equals(decode.get("destination"),[320,0,640,160]) or not decode.get("format") is String or decode.format != "rgb555-stp1" or not decode.has("clut") or decode.clut != null or not decode.get("hardware_pixel_parity_verified") is bool or decode.hardware_pixel_parity_verified:
		return "board decode geometry/format or evidence scope differs"
	var move: Variant = m.get("following_move")
	if not move is Dictionary or not move.get("requires_prior_owned_vram") is bool or not move.requires_prior_owned_vram or not _integer(move.get("source_height"),255) or not _integer(move.get("newly_decoded_height"),160):
		return "board follow-up copies cannot invent ownership below decoded rows"
	var calls: Variant = move.get("call")
	if not calls is Array or calls.size() != 3 or not _integer(calls[0],5) or not _integer(calls[1],14) or not _integer(calls[2],15): return "board source move call differs"
	var names_bg: Variant = m.get("names_backgrounds")
	if not names_bg is Dictionary or names_bg.size() != 7 or not _check_names_bg_manifest(names_bg.get("0x495d"),"CDROM/BS/P_01_03.BS",BG_495D_SOURCE_SHA,16392,0x495d,BG_495D_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0x4966"),"CDROM/BS/P_01_03B.BS",BG_4966_SOURCE_SHA,8192,0x4966,BG_4966_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacdd"),"CDROM/PROLODIR/P_01_04B.BS",BG_ACDD_SOURCE_SHA,17952,0xacdd,BG_ACDD_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xace6"),"CDROM/PROLODIR/P_01_05.BS",BG_ACE6_SOURCE_SHA,10532,0xace6,BG_ACE6_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacf6"),"CDROM/PROLODIR/P_01_07.BS",BG_ACF6_SOURCE_SHA,19568,0xacf6,BG_ACF6_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacd4"),"CDROM/PROLODIR/P_01_04.BS",BG_ACD4_SOURCE_SHA,17016,0xacd4,BG_ACD4_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacec"),"CDROM/PROLODIR/P_01_06.BS",BG_ACEC_SOURCE_SHA,18492,0xacec,BG_ACEC_TRANSFER_SHA,0,[704,0,320,160]):
		return "names background manifest contract differs"
	var stage_xfers: Variant = m.get("stage_02_04_transfers")
	if not stage_xfers is Dictionary or stage_xfers.size() != 8 or not _check_stage_transfer_manifest(stage_xfers.get("0xab18"),"cut_12.nbn","CDROM/PROLODIR/CUT_12.NBN",CUT_12_SHA,100352,0xab18,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xac3f"),"o_12.nbn","CDROM/PROLODIR/O_12.NBN",O_12_SHA,34816,0xac3f,17,0x80180000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa23"),"cut_05.nbn","CDROM/PROLODIR/CUT_05.NBN",CUT_05_SHA,100352,0xaa23,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabb8"),"o_05.nbn","CDROM/PROLODIR/O_05.NBN",O_05_SHA,18432,0xabb8,9,0x80180000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa54"),"cut_06.nbn","CDROM/PROLODIR/CUT_07.NBN",CUT_06_SHA,100352,0xaa54,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabca"),"o_06.nbn","CDROM/PROLODIR/O_07.NBN",O_06_SHA,51200,0xabca,25,0x801f0000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa85"),"cut_07.nbn","CDROM/PROLODIR/CUT_08.NBN",CUT_07_SHA,100352,0xaa85,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabe3"),"o_07.nbn","CDROM/PROLODIR/O_08.NBN",O_07_SHA,51200,0xabe3,25,0x801f0000):
		return "stage 2..6 NBN transfer manifest contract differs"
	var files: Variant = m.get("files")
	if not files is Dictionary or files.size() != FILES.size(): return "board file inventory differs"
	var directory_files := DirAccess.get_files_at(directory)
	if directory_files.size() != FILES.size()+1 or not DirAccess.get_directories_at(directory).is_empty(): return "board role contains unlisted files/directories"
	var bytes: Dictionary = {}
	var shas: Dictionary = {}
	for name: String in FILES:
		var record: Variant = files.get(name)
		var path := directory.path_join(name)
		if not record is Dictionary or record.size() != 2 or not _sha(record.get("sha256")) or not ExternalPath.is_external_file(path) or not _integer(record.get("bytes"),FileAccess.get_size(path)) or FileAccess.get_size(path) < 1 or FileAccess.get_size(path) > 1024*1024 or FileAccess.get_sha256(path) != record.sha256:
			return "board payload missing, linked, changed or outside bounds: "+name
		bytes[name] = FileAccess.get_file_as_bytes(path)
		shas[name] = str(record.sha256)
	if bytes["board.bs"].size() != 18432 or shas["board.bs"] != SOURCE_SHA:
		return "board bitstream transfer extent differs"
	if bytes["names_495d.bs"].size() != 16392 or shas["names_495d.bs"] != BG_495D_SOURCE_SHA or bytes["names_495d.transfer.bin"].size() != 20480 or shas["names_495d.transfer.bin"] != BG_495D_TRANSFER_SHA or bytes["names_495d.transfer.bin"].slice(0,16392) != bytes["names_495d.bs"]:
		return "0x495d bitstream/transfer extent differs"
	if bytes["names_4966.bs"].size() != 8192 or shas["names_4966.bs"] != BG_4966_SOURCE_SHA or bytes["names_4966.transfer.bin"].size() != 20480 or shas["names_4966.transfer.bin"] != BG_4966_TRANSFER_SHA or bytes["names_4966.transfer.bin"].slice(0,8192) != bytes["names_4966.bs"]:
		return "0x4966 bitstream/transfer extent differs"
	if bytes["names_acdd.bs"].size() != 17952 or shas["names_acdd.bs"] != BG_ACDD_SOURCE_SHA or bytes["names_acdd.transfer.bin"].size() != 20480 or shas["names_acdd.transfer.bin"] != BG_ACDD_TRANSFER_SHA or bytes["names_acdd.transfer.bin"].slice(0,17952) != bytes["names_acdd.bs"]:
		return "0xacdd bitstream/transfer extent differs"
	if bytes["names_ace6.bs"].size() != 10532 or shas["names_ace6.bs"] != BG_ACE6_SOURCE_SHA or bytes["names_ace6.transfer.bin"].size() != 20480 or shas["names_ace6.transfer.bin"] != BG_ACE6_TRANSFER_SHA or bytes["names_ace6.transfer.bin"].slice(0,10532) != bytes["names_ace6.bs"]:
		return "0xace6 bitstream/transfer extent differs"
	if bytes["names_acf6.bs"].size() != 19568 or shas["names_acf6.bs"] != BG_ACF6_SOURCE_SHA or bytes["names_acf6.transfer.bin"].size() != 20480 or shas["names_acf6.transfer.bin"] != BG_ACF6_TRANSFER_SHA or bytes["names_acf6.transfer.bin"].slice(0,19568) != bytes["names_acf6.bs"]:
		return "0xacf6 bitstream/transfer extent differs"
	if bytes["names_acd4.bs"].size() != 17016 or shas["names_acd4.bs"] != BG_ACD4_SOURCE_SHA or bytes["names_acd4.transfer.bin"].size() != 20480 or shas["names_acd4.transfer.bin"] != BG_ACD4_TRANSFER_SHA or bytes["names_acd4.transfer.bin"].slice(0,17016) != bytes["names_acd4.bs"]:
		return "0xacd4 bitstream/transfer extent differs"
	if bytes["names_acec.bs"].size() != 18492 or shas["names_acec.bs"] != BG_ACEC_SOURCE_SHA or bytes["names_acec.transfer.bin"].size() != 20480 or shas["names_acec.transfer.bin"] != BG_ACEC_TRANSFER_SHA or bytes["names_acec.transfer.bin"].slice(0,18492) != bytes["names_acec.bs"]:
		return "0xacec bitstream/transfer extent differs"
	if bytes["cut_12.nbn"].size() != 100352 or shas["cut_12.nbn"] != CUT_12_SHA or bytes["o_12.nbn"].size() != 34816 or shas["o_12.nbn"] != O_12_SHA or bytes["cut_05.nbn"].size() != 100352 or shas["cut_05.nbn"] != CUT_05_SHA or bytes["o_05.nbn"].size() != 18432 or shas["o_05.nbn"] != O_05_SHA or bytes["cut_06.nbn"].size() != 100352 or shas["cut_06.nbn"] != CUT_06_SHA or bytes["o_06.nbn"].size() != 51200 or shas["o_06.nbn"] != O_06_SHA or bytes["cut_07.nbn"].size() != 100352 or shas["cut_07.nbn"] != CUT_07_SHA or bytes["o_07.nbn"].size() != 51200 or shas["o_07.nbn"] != O_07_SHA:
		return "stage 2..6 NBN transfer payload extent or hash differs"
	var board_img: Variant = _check_mdec_group(bytes,"board",640,160,SOFTWARE_WORD_SHA,shas["board.rgb555.bin"])
	if board_img is String: return board_img
	var img_495d: Variant = _check_mdec_group(bytes,"names_495d",320,160,BG_495D_WORD_SHA,shas["names_495d.rgb555.bin"])
	if img_495d is String: return img_495d
	var img_4966: Variant = _check_mdec_group(bytes,"names_4966",320,160,BG_4966_WORD_SHA,shas["names_4966.rgb555.bin"])
	if img_4966 is String: return img_4966
	var img_acdd: Variant = _check_mdec_group(bytes,"names_acdd",320,160,BG_ACDD_WORD_SHA,shas["names_acdd.rgb555.bin"])
	if img_acdd is String: return img_acdd
	var img_ace6: Variant = _check_mdec_group(bytes,"names_ace6",320,160,BG_ACE6_WORD_SHA,shas["names_ace6.rgb555.bin"])
	if img_ace6 is String: return img_ace6
	var img_acf6: Variant = _check_mdec_group(bytes,"names_acf6",320,160,BG_ACF6_WORD_SHA,shas["names_acf6.rgb555.bin"])
	if img_acf6 is String: return img_acf6
	var img_acd4: Variant = _check_mdec_group(bytes,"names_acd4",320,160,BG_ACD4_WORD_SHA,shas["names_acd4.rgb555.bin"])
	if img_acd4 is String: return img_acd4
	var img_acec: Variant = _check_mdec_group(bytes,"names_acec",320,160,BG_ACEC_WORD_SHA,shas["names_acec.rgb555.bin"])
	if img_acec is String: return img_acec
	_root = directory
	_manifest_sha = expected_manifest_sha256
	_read_bytes = bytes["board.bs"].duplicate()
	_words = bytes["board.rgb555.bin"].duplicate()
	_preview = board_img
	_bg_transfers = {
		BG_495D_ID: {"bytes":bytes["names_495d.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_495D_TRANSFER_SHA,"lba":0x495d,"sectors":10},
		BG_4966_ID: {"bytes":bytes["names_4966.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_4966_TRANSFER_SHA,"lba":0x4966,"sectors":10},
		BG_ACDD_ID: {"bytes":bytes["names_acdd.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_ACDD_TRANSFER_SHA,"lba":0xacdd,"sectors":10},
		BG_ACE6_ID: {"bytes":bytes["names_ace6.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_ACE6_TRANSFER_SHA,"lba":0xace6,"sectors":10},
		BG_ACF6_ID: {"bytes":bytes["names_acf6.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_ACF6_TRANSFER_SHA,"lba":0xacf6,"sectors":10},
		BG_ACD4_ID: {"bytes":bytes["names_acd4.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_ACD4_TRANSFER_SHA,"lba":0xacd4,"sectors":10},
		BG_ACEC_ID: {"bytes":bytes["names_acec.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_ACEC_TRANSFER_SHA,"lba":0xacec,"sectors":10},
	}
	_bg_uploads = {
		BG_495D_ID: {"resource_id":BG_495D_ID,"lba":0x495d,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_495d.rgb555.bin"].duplicate(),"word_sha256":BG_495D_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_4966_ID: {"resource_id":BG_4966_ID,"lba":0x4966,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_4966.rgb555.bin"].duplicate(),"word_sha256":BG_4966_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACDD_ID: {"resource_id":BG_ACDD_ID,"lba":0xacdd,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_acdd.rgb555.bin"].duplicate(),"word_sha256":BG_ACDD_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACE6_ID: {"resource_id":BG_ACE6_ID,"lba":0xace6,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_ace6.rgb555.bin"].duplicate(),"word_sha256":BG_ACE6_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACF6_ID: {"resource_id":BG_ACF6_ID,"lba":0xacf6,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_acf6.rgb555.bin"].duplicate(),"word_sha256":BG_ACF6_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACD4_ID: {"resource_id":BG_ACD4_ID,"lba":0xacd4,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_acd4.rgb555.bin"].duplicate(),"word_sha256":BG_ACD4_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACEC_ID: {"resource_id":BG_ACEC_ID,"lba":0xacec,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_acec.rgb555.bin"].duplicate(),"word_sha256":BG_ACEC_WORD_SHA,"manifest_sha256":_manifest_sha},
	}
	_bg_previews = {
		BG_495D_ID: img_495d,
		BG_4966_ID: img_4966,
		BG_ACDD_ID: img_acdd,
		BG_ACE6_ID: img_ace6,
		BG_ACF6_ID: img_acf6,
		BG_ACD4_ID: img_acd4,
		BG_ACEC_ID: img_acec,
	}
	_stage_transfers = {
		CUT_12_ID: {"bytes":bytes["cut_12.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_12_SHA,"lba":0xab18,"sectors":49},
		O_12_ID: {"bytes":bytes["o_12.nbn"].duplicate(),"destination":0x80180000,"sha256":O_12_SHA,"lba":0xac3f,"sectors":17},
		CUT_05_ID: {"bytes":bytes["cut_05.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_05_SHA,"lba":0xaa23,"sectors":49},
		O_05_ID: {"bytes":bytes["o_05.nbn"].duplicate(),"destination":0x80180000,"sha256":O_05_SHA,"lba":0xabb8,"sectors":9},
		CUT_06_ID: {"bytes":bytes["cut_06.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_06_SHA,"lba":0xaa54,"sectors":49},
		O_06_ID: {"bytes":bytes["o_06.nbn"].duplicate(),"destination":0x801f0000,"sha256":O_06_SHA,"lba":0xabca,"sectors":25},
		CUT_07_ID: {"bytes":bytes["cut_07.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_07_SHA,"lba":0xaa85,"sectors":49},
		O_07_ID: {"bytes":bytes["o_07.nbn"].duplicate(),"destination":0x801f0000,"sha256":O_07_SHA,"lba":0xabe3,"sectors":25},
	}
	return ""

func read_payload() -> PackedByteArray:
	return _read_bytes.duplicate()

func native_read_descriptor() -> Dictionary:
	if _root.is_empty(): return {"error":"board resource role is not authenticated"}
	return {"error":"","lba":0x4954,"sectors":9,"destination":0x80162000,
		"payload":_read_bytes.duplicate(),"manifest_sha256":_manifest_sha,
		"service_completion_supplied":false}

func software_upload() -> Dictionary:
	if _root.is_empty(): return {"error":"board resource role is not authenticated"}
	return {"error":"","rectangle":[320,0,640,160],"words":_words.duplicate(),"manifest_sha256":_manifest_sha,"hardware_pixel_parity_verified":false,"following_move_requires_prior_owned_vram":true}

func preview() -> Image:
	return _preview.duplicate() if _preview != null else null

func background_descriptions() -> Dictionary:
	if _root.is_empty(): return {}
	return {
		str(0x495d): {"id":BG_495D_ID,"sha256":BG_495D_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0x495d,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_495D_WORD_SHA},
		str(0x4966): {"id":BG_4966_ID,"sha256":BG_4966_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0x4966,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_4966_WORD_SHA},
		str(0xacdd): {"id":BG_ACDD_ID,"sha256":BG_ACDD_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xacdd,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_ACDD_WORD_SHA},
		str(0xace6): {"id":BG_ACE6_ID,"sha256":BG_ACE6_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xace6,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_ACE6_WORD_SHA},
		str(0xacf6): {"id":BG_ACF6_ID,"sha256":BG_ACF6_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xacf6,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_ACF6_WORD_SHA},
		str(0xacd4): {"id":BG_ACD4_ID,"sha256":BG_ACD4_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xacd4,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_ACD4_WORD_SHA},
		str(0xacec): {"id":BG_ACEC_ID,"sha256":BG_ACEC_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xacec,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_ACEC_WORD_SHA},
	}

func background_transfer(resource_id: String) -> Dictionary:
	if _root.is_empty() or not _bg_transfers.has(resource_id):
		return {"error":"unknown names background transfer resource"}
	var item: Dictionary = _bg_transfers[resource_id]
	return {"error":"","bytes":item.bytes.duplicate(),"destination":item.destination,"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors}

func background_software_upload(resource_id: String) -> Dictionary:
	if _root.is_empty() or not _bg_uploads.has(resource_id):
		return {"error":"unknown names background upload resource"}
	var item: Dictionary = _bg_uploads[resource_id]
	return {"error":"","resource_id":item.resource_id,"lba":item.lba,"buffer":item.buffer,"rectangle":item.rectangle.duplicate(),"words":item.words.duplicate(),"word_sha256":item.word_sha256,"manifest_sha256":_manifest_sha}

func background_preview(resource_id: String) -> Image:
	if _root.is_empty() or not _bg_previews.has(resource_id): return null
	return _bg_previews[resource_id].duplicate()

func actor_descriptions() -> Dictionary:
	if _root.is_empty(): return {}
	return {
		"49:2149203968:43800": {"id":CUT_12_ID,"sha256":CUT_12_SHA,"sectors":49,"destination":0x801a4000,"lba":0xab18},
		"17:2149056512:44095": {"id":O_12_ID,"sha256":O_12_SHA,"sectors":17,"destination":0x80180000,"lba":0xac3f},
		"49:2149203968:43555": {"id":CUT_05_ID,"sha256":CUT_05_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa23},
		"9:2149056512:43960": {"id":O_05_ID,"sha256":O_05_SHA,"sectors":9,"destination":0x80180000,"lba":0xabb8},
		"49:2149203968:43604": {"id":CUT_06_ID,"sha256":CUT_06_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa54},
		"25:2149515264:43978": {"id":O_06_ID,"sha256":O_06_SHA,"sectors":25,"destination":0x801f0000,"lba":0xabca},
		"49:2149203968:43653": {"id":CUT_07_ID,"sha256":CUT_07_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa85},
		"25:2149515264:44003": {"id":O_07_ID,"sha256":O_07_SHA,"sectors":25,"destination":0x801f0000,"lba":0xabe3},
	}

func stage_transfer(resource_id: String) -> Dictionary:
	if _root.is_empty() or not _stage_transfers.has(resource_id):
		return {"error":"unknown stage 2..6 NBN transfer resource"}
	var item: Dictionary = _stage_transfers[resource_id]
	return {"error":"","bytes":item.bytes.duplicate(),"destination":item.destination,"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors}
