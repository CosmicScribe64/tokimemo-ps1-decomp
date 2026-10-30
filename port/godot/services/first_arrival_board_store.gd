extends RefCounted
## Additive original-board resource reader. No CD/GPU/audio completion events.
const ExternalPath = preload("res://services/external_content_path.gd")
const SCHEMA := "tokimemo.first-arrival-board.v1"
const SOURCE_SHA := "61b0cd543a73c64ff9ad6d1e77d48217c02dbfbe7dab4941e90451b76f20f398"
const MANIFEST_SHA := "f1f65db98313ae866c21b8c50d74953d42fcd261d1abfff2be62dad58d58e640"
const SOFTWARE_WORD_SHA := "1e77c3bb1fc5e715f051bd60bf6dc97a5dc2a8581a2c36cbeb360b844fcecdc5"
const BG_495D_ID := "prologue.names.0x495d"
const BG_495D_SOURCE_SHA := "aa2e82335720f7091a54ccc26e7e0a94be3a75fa1513d6bc1d5e1f3e36e5b230"
const BG_495D_TRANSFER_SHA := "064fa4988819e19c9c858500d4f49caa6a96112d99d04793f5e088c278ed4e50"
const BG_495D_WORD_SHA := "4e2bb6e1c6f443109f7586e08e7b4af8f122bd414fbb103ef2f93f3096b91a31"
const BG_4966_ID := "prologue.names.0x4966"
const BG_4966_SOURCE_SHA := "090ad2700ac094805298eabeb90560308e89ad7cca098c27146a24b8181b5c92"
const BG_4966_TRANSFER_SHA := "1216a2ea05373a751e76db3f05d2c98b1206dc651d3b93d638964376fb83c68b"
const BG_4966_WORD_SHA := "750b5a144a2f4f59d1f267211e84bd99428fcae727fda3377909c7e87200910c"
const BG_ACDD_ID := "prologue.names.0xacdd"
const BG_ACDD_SOURCE_SHA := "ffa02dc52c645679bdbfd66739254e1088fd58672321acbefa2d671c870f2ba2"
const BG_ACDD_TRANSFER_SHA := "67d96aca10962b5379aa9193a996b2eed2f659b01514097ba5b06118ea9a91ea"
const BG_ACDD_WORD_SHA := "0e8e36c414a1ea02e9f8ec0098480b2da5907c70095f960c6c1058b43563a9ba"
const BG_ACE6_ID := "prologue.names.0xace6"
const BG_ACE6_SOURCE_SHA := "d082130abe5c199b7d8c9c75add436ce0f0f33b23e6a58e3e2e06782aa41119e"
const BG_ACE6_TRANSFER_SHA := "626af576b1adcb054e360b2e2bfca4a5bd1b7f9f94662f42310aaada3d976bd4"
const BG_ACE6_WORD_SHA := "d6141f2703f760802e26bb49fb5603cac5964f93dbe2344327029dfbc93a8ff4"
const BG_ACF6_ID := "prologue.names.0xacf6"
const BG_ACF6_SOURCE_SHA := "a4f19c201f7650e14be9c1eca419d3055c7704d6785f1a8263cd3f1eea559980"
const BG_ACF6_TRANSFER_SHA := "f6f921e4a85c00d30c9c63c55a1bb6ac70425de4433c7b0028e4a1e30e802e79"
const BG_ACF6_WORD_SHA := "40927cf28817e0dbe129985b007e3b4c7d7a23108ebcf251e139177be265fe5a"
const BG_ACD4_ID := "prologue.names.0xacd4"
const BG_ACD4_SOURCE_SHA := "dcf99efbac821fbb769847204c0e8a1a1b611ed15daa0450700a59257387fae6"
const BG_ACD4_TRANSFER_SHA := "a2f0c71eb50a70d2028aa48f6bcfaea65b5524369eb9cc5aaf1362f9a7fb340a"
const BG_ACD4_WORD_SHA := "c472cae6c7cba850dc9220d86da096c44d94f1fed2aa944b38db4546c3082963"
const BG_ACEC_ID := "prologue.names.0xacec"
const BG_AD00_ID := "prologue.names.0xad00"
const BG_ACEC_SOURCE_SHA := "5feccf3ed08b49ed918458c4772c870b8c1533e7fbc0391df314e59172c7858c"
const BG_AD00_SOURCE_SHA := "184c047fce1e4da8f736980e8d25a38aec0b4fc41d8297095b8a2bea9e50a774"
const BG_ACEC_TRANSFER_SHA := "f2acadc280ae1d94841c9c08f455c5ece893dc371988ff56bcceacbfe68d80b9"
const BG_AD00_TRANSFER_SHA := "03b1b7225d0a7c512642f209e3dab711941b397b0fbf086b9dddd811c91815bb"
const BG_ACEC_WORD_SHA := "c817ff62be56f6664ff0c64c74ec3f775d2b894d34f5b78f2849a16df6d80d72"
const BG_AD00_WORD_SHA := "72bb331069c35dfb794cc832a1c5081c0a804c5d272be134e80536166839d060"
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
const CUT_09_ID := "prologue.cut_09.0xaae7"
const CUT_09_SHA := "4feef785ad85d06129b0e13ac88c7984e3284ae71d9dfac8c272d300835341c7"
const O_09_ID := "prologue.o_09.0xac05"
const O_09_SHA := "dacf206a52a5396872e759effd471f9a21a769b2a57c9a141edc221789fb3ead"
const O_11_ID := "prologue.o_11.0xac1e"
const O_11_SHA := "3e901c18b0ed186323446151f35892bdb66e477d8a419e772034fd2b61be0628"
const CUT_08_ID := "prologue.cut_08.0xaab6"
const CUT_07_SHA := "444165354346eeac36f2614a3375bd24e3ff5b457924b1d81705b4e174c3e3ca"
const CUT_08_SHA := "9c4a73089b11368c754de62a546344668182fb2b37ba9862e5bed0263d9cbd0b"
const O_07_ID := "prologue.o_07.0xabe3"
const O_08_ID := "prologue.o_08.0xabfc"
const O_07_SHA := "239a20180cd00bd8c117603075505147fff5d1e1f5769eb94323c7d0fd78de77"
const O_08_SHA := "6dcde1bbf652c9cf5ca77c9c68547afe723ce08b56bfe6c08bf3048fa85ac331"
# Stages 12-20 (wiki/spec/opening-stages-12-14.md, -15-17.md, -18-20.md).
# Ids follow the disc file names here (CUT_14 at 0xAB49, CUT_16 at 0xAB7A),
# unlike the older ids above. O_13 is read to more than one destination
# (stage11 case5 to 0x801D8000, stage17 case3 to 0x80180000): its manifest
# record and stage transfer carry no destination; each read lives only in its
# per-read composite key in actor_descriptions().
const O_13_ID := "prologue.o_13.0xac50"
const O_13_SHA := "38c0149c0fb6bd9b6b812b06a5c170a2529e7a0271b0cb3ffc812e6ce6b4236f"
const CUT_14_ID := "prologue.cut_14.0xab49"
const CUT_14_SHA := "8059ef80df31cb4662db9c8b816b258066e1730251b7454e14d3426b42c2fefe"
const O_14_ID := "prologue.o_14.0xac91"
const O_14_SHA := "f81f398f969215f0ca65f04ac6624fca8f733568f21bf719fec023c7eb30bee5"
const O_15_ID := "prologue.o_15.0xac9a"
const O_15_SHA := "9d6aea8ea1356ee666fba64e1f76d22db106fac4ad08b4e99de87ffb9326a0db"
const CUT_16_ID := "prologue.cut_16.0xab7a"
const CUT_16_SHA := "de1f091899464b4c04916815f71d6da40221da0cf3ab30c6336765e7ba279b17"
const O_16_ID := "prologue.o_16.0xacbb"
const O_16_SHA := "0bde8b1c5285bf6f1509adbfab894a983c0f5cd5c5238979643f7bbd84abe03f"
const BG_AD0B_ID := "prologue.names.0xad0b"
const BG_AD0B_SOURCE_SHA := "78c2813b109d87b5692783bbfc1bfd69ce5c89d015ed2b070cc5c8f5c7082098"
const BG_AD0B_TRANSFER_SHA := "c0327b980bbef7daeb31a20bdb3de992d36ec34d606fde2809e37dcb948e52b3"
const BG_AD0B_WORD_SHA := "fa9a1d8b696455199486e9638651f4f2428f6457b5cd73ee587abcedc24d2d9a"
# Stage11 prologue_line_17_run: case4 dec_bg_cd_read(0xAD05,1) (P_01_08, and
# the case6 retry), decoded at case6 into buffer 0 (stage10 showed ACF6 in
# buffer 1). Stages 12-14 show it in buffer 0 between their own decodes.
const BG_AD05_ID := "prologue.names.0xad05"
const BG_AD05_SOURCE_SHA := "d9b717964b37f3dcb0ef57856170269e79c10185281958bde7a8a69b94052cf2"
const BG_AD05_TRANSFER_SHA := "a7d9ab6fecca526d79333041a23a462e64b999a824a50e70fdf8c207901a5925"
const BG_AD05_WORD_SHA := "00323a996c0c512920688d581e32879b142baef22a0ee8a4cd680a081fef8ca4"
# Stages 16-17 slot-1 backgrounds: dec_bg_cd_read(0x496A,1) at 16/10 (and
# the 16/11 retry), dec_bg_cd_read(0x4973,1) at 17/2 (and 17/3). Each 10-sector
# extent carries the first sector of the next BS file. The decode buffer is
# 1 - dec_bg_show[1] at runtime; the record names the uninterrupted path's.
const BG_496A_ID := "prologue.names.0x496a"
const BG_496A_SOURCE_SHA := "c064b04b84daa381117a3b37e1c68b6e0a754b5f559dfc26aa579042e2f16762"
const BG_496A_TRANSFER_SHA := "56bbdab9a66a99bb3e2bb526a16a78107b66dd03767b2330548d3c3ff3157c9a"
const BG_496A_WORD_SHA := "ac4bd8e5f14c7f77dfce3e18eb2f564ab4b90f44ca38f4355633da187334d339"
const BG_4973_ID := "prologue.names.0x4973"
const BG_4973_SOURCE_SHA := "e5c33edfb057565c49dd9b9366781c825dae153929757632294931e8598aff08"
const BG_4973_TRANSFER_SHA := "c7dbbf759146d38b1b50f2c18373186b6523c25e71b06fb7d3eaf02ab4875f11"
const BG_4973_WORD_SHA := "69f43044160412ddd30571a000f469b25bc8bff9bbcef8416b8f5f81ff8bc711"
# [id, file stem, ISO path, source sha, bytes, lba, transfer sha, word sha, nominal buffer, rect]
# Every slot-1 background exported by the generic extent/decode path
# (P_01_08 for stage11, P_01_10/P_01_12 for stages 16-17).
const STAGES_16_17_BACKGROUNDS := [
	[BG_AD05_ID, "names_ad05", "CDROM/PROLODIR/P_01_08.BS", BG_AD05_SOURCE_SHA, 11788, 0xad05, BG_AD05_TRANSFER_SHA, BG_AD05_WORD_SHA, 0, [704,0,320,160]],
	[BG_496A_ID, "names_496a", "CDROM/BS/P_01_10.BS", BG_496A_SOURCE_SHA, 18432, 0x496a, BG_496A_TRANSFER_SHA, BG_496A_WORD_SHA, 1, [320,256,320,160]],
	[BG_4973_ID, "names_4973", "CDROM/BS/P_01_12.BS", BG_4973_SOURCE_SHA, 18432, 0x4973, BG_4973_TRANSFER_SHA, BG_4973_WORD_SHA, 0, [704,0,320,160]],
]
# dec_bg_decode_to_vram(1) rectangles of slot-1 buffers 0 and 1.
const SLOT1_BUFFER_RECTS := [[704,0,320,160],[320,256,320,160]]
# Stages 12-14 checked reads: [id, file, ISO path, sha256, bytes, lba,
# sectors, destination (-1: none, see O_13 above), actor-area copy bytes].
# Every extent ends in its cd_read_sectors 0x1012 trailer word.
const STAGES_12_14_TRANSFERS := [
	[O_13_ID, "o_13.nbn", "CDROM/PROLODIR/O_13.NBN", O_13_SHA, 133120, 0xac50, 65, -1, 0x20800],
	[CUT_14_ID, "cut_14.nbn", "CDROM/PROLODIR/CUT_14.NBN", CUT_14_SHA, 100352, 0xab49, 49, 0x801a4000, 0],
	[O_14_ID, "o_14.nbn", "CDROM/PROLODIR/O_14.NBN", O_14_SHA, 18432, 0xac91, 9, 0x801f0000, 0x4800],
	[O_15_ID, "o_15.nbn", "CDROM/PROLODIR/O_15.NBN", O_15_SHA, 67584, 0xac9a, 33, 0x801e8000, 0x10800],
	[CUT_16_ID, "cut_16.nbn", "CDROM/PROLODIR/CUT_16.NBN", CUT_16_SHA, 100352, 0xab7a, 49, 0x801a4000, 0],
	[O_16_ID, "o_16.nbn", "CDROM/PROLODIR/O_16.NBN", O_16_SHA, 51200, 0xacbb, 25, 0x801e8000, 0xc800],
]
# Per-read composite keys "sectors:destination:lba" for the stage 11-17 reads.
const STAGES_11_17_READS := [
	[O_13_ID, O_13_SHA, 65, 0x801d8000, 0xac50],  # stage11 case5 (copied by case6)
	[CUT_14_ID, CUT_14_SHA, 49, 0x801a4000, 0xab49],  # stage12 case0
	[O_14_ID, O_14_SHA, 9, 0x801f0000, 0xac91],  # stage12 case1
	[O_15_ID, O_15_SHA, 33, 0x801e8000, 0xac9a],  # stage13 case3
	[CUT_16_ID, CUT_16_SHA, 49, 0x801a4000, 0xab7a],  # stage14 case4
	[O_16_ID, O_16_SHA, 25, 0x801e8000, 0xacbb],  # stage14 case5
	[O_06_ID, O_06_SHA, 25, 0x801e8000, 0xabca],  # stage15 case33 (O_07.NBN)
	[O_13_ID, O_13_SHA, 65, 0x80180000, 0xac50],  # stage17 case3, used by stage19
]
const FILES := [
	"board.bs", "board.rgb555.bin", "board.background.png", "board.background.stp.png", "board.background.padded.png", "board.background.padded.stp.png",
	"names_495d.bs", "names_495d.transfer.bin", "names_495d.rgb555.bin", "names_495d.background.png", "names_495d.background.stp.png", "names_495d.background.padded.png", "names_495d.background.padded.stp.png",
	"names_4966.bs", "names_4966.transfer.bin", "names_4966.rgb555.bin", "names_4966.background.png", "names_4966.background.stp.png", "names_4966.background.padded.png", "names_4966.background.padded.stp.png",
	"names_acdd.bs", "names_acdd.transfer.bin", "names_acdd.rgb555.bin", "names_acdd.background.png", "names_acdd.background.stp.png", "names_acdd.background.padded.png", "names_acdd.background.padded.stp.png",
	"names_ace6.bs", "names_ace6.transfer.bin", "names_ace6.rgb555.bin", "names_ace6.background.png", "names_ace6.background.stp.png", "names_ace6.background.padded.png", "names_ace6.background.padded.stp.png",
	"names_acf6.bs", "names_acf6.transfer.bin", "names_acf6.rgb555.bin", "names_acf6.background.png", "names_acf6.background.stp.png", "names_acf6.background.padded.png", "names_acf6.background.padded.stp.png",
	"names_acd4.bs", "names_acd4.transfer.bin", "names_acd4.rgb555.bin", "names_acd4.background.png", "names_acd4.background.stp.png", "names_acd4.background.padded.png", "names_acd4.background.padded.stp.png",
	"names_acec.bs", "names_acec.transfer.bin", "names_acec.rgb555.bin", "names_acec.background.png", "names_acec.background.stp.png", "names_acec.background.padded.png", "names_acec.background.padded.stp.png",
	"names_ad00.bs", "names_ad00.transfer.bin", "names_ad00.rgb555.bin", "names_ad00.background.png", "names_ad00.background.stp.png", "names_ad00.background.padded.png", "names_ad00.background.padded.stp.png",
	"cut_12.nbn", "o_12.nbn", "cut_05.nbn", "o_05.nbn", "cut_06.nbn", "o_06.nbn", "cut_07.nbn", "o_07.nbn", "cut_08.nbn", "o_08.nbn", "cut_09.nbn", "o_09.nbn", "o_11.nbn",
	"names_ad0b.bs", "names_ad0b.transfer.bin", "names_ad0b.rgb555.bin", "names_ad0b.background.png", "names_ad0b.background.stp.png", "names_ad0b.background.padded.png", "names_ad0b.background.padded.stp.png",
	"o_13.nbn", "cut_14.nbn", "o_14.nbn", "o_15.nbn", "cut_16.nbn", "o_16.nbn",
	"names_ad05.bs", "names_ad05.transfer.bin", "names_ad05.rgb555.bin", "names_ad05.background.png", "names_ad05.background.stp.png", "names_ad05.background.padded.png", "names_ad05.background.padded.stp.png",
	"names_496a.bs", "names_496a.transfer.bin", "names_496a.rgb555.bin", "names_496a.background.png", "names_496a.background.stp.png", "names_496a.background.padded.png", "names_496a.background.padded.stp.png",
	"names_4973.bs", "names_4973.transfer.bin", "names_4973.rgb555.bin", "names_4973.background.png", "names_4973.background.stp.png", "names_4973.background.padded.png", "names_4973.background.padded.stp.png",
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

# Stage10 tpage_buf free slots in search_tpage scan order (actor_sprites.c):
# tpage_buf_clear leaves -1 only where (i&15) >= 5; _sys_default_tpage_set
# fills 11-15/21-25/43-47/53-57/27-31/59-63, prologue.c:257 locks O01 in 26
# and prologue_background_slot_set (tp 8, 3x2) locks the cut in 8-10/40-42.
const STAGE10_FREE_TPAGE_SLOTS := [5, 6, 7, 37, 38, 39, 58]

# Stages 12-14 actor resources: [tpage_table offset, page count] of the
# table each setup points tpage_table at (prologue.c:1866, :1974, :2124).
# O_13 is stage11's 0x20800-byte copy that stage12 draws before its commit
# (actor96 tpage_table 0x801801E4, prologue.c:1813).
const SHIORI_TPAGE_TABLES := {O_13_ID: [0x1e4, 8], O_14_ID: [0xf0, 1], O_15_ID: [0x284, 4], O_16_ID: [0x24c, 3]}

static func shiori_page_offsets(resource_id: String, bytes: PackedByteArray) -> Array[int]:
	# The 0x4000-byte page offsets the resource's own tpage table names, read
	# from its disc bytes; empty when a word is not a whole page inside them.
	var out: Array[int] = []
	var table: Variant = SHIORI_TPAGE_TABLES.get(resource_id)
	if not table is Array: return out
	for index in int(table[1]):
		var at: int = int(table[0]) + index*4
		if at+4 > bytes.size(): return [] as Array[int]
		var offset: int = bytes.decode_u32(at) - 0x80180000
		if offset < 0 or offset + 0x4000 > bytes.size(): return [] as Array[int]
		out.append(offset)
	return out

static func actor_frame_pages(bytes: PackedByteArray, frame_table: int, tpage_table: int, frames: int, pages: int, frame: int) -> Array[int]:
	# Page offsets of one frame-table frame in actor_draw_all part order
	# (count-1..0), resource loaded at 0x80180000. Empty on any bad pointer.
	var result: Array[int] = []
	if frame < 0 or frame >= frames or frame_table+frames*4 > bytes.size() or tpage_table+pages*4 > bytes.size(): return result
	var offset := int(bytes.decode_u32(frame_table+frame*4))-0x80180000
	if offset < 0 or offset+8 > bytes.size(): return result
	var count := bytes.decode_s16(offset)
	for part in range(count-1,-1,-1):
		var row := offset+8+part*14
		if row+14 > bytes.size(): return [] as Array[int]
		var page := bytes.decode_u16(row+4) >> 6
		if page >= pages: return [] as Array[int]
		var page_offset := int(bytes.decode_u32(tpage_table+page*4))-0x80180000
		if page_offset < 0 or page_offset+0x4000 > bytes.size(): return [] as Array[int]
		result.append(page_offset)
	return result

static func o11_frame_pages(bytes: PackedByteArray, frame: int) -> Array[int]:
	if bytes.size() != 0x10800: return [] as Array[int]
	return actor_frame_pages(bytes,0x150,0x140,9,4,frame)

# Stage9 O10 entrance (prologue.c case7: actor96 frame table 0x80184288,
# tpage table 0x80184280, frame 0; crowd actors 99/100 tpage 0x8018010C).
# The same case7 locks leave the STAGE10_FREE_TPAGE_SLOTS order. Frame0
# parts are pages [0,1,1,1], walked 3..0, so page1 (0x82c0) takes slot 5
# and page0 (0x42c0) slot 6; actors 97/98 reuse 0x42c0 and the crowd page
# takes 7. o10_entrance_tpage_slots derives the actor96 part of this.
const O10_ENTRANCE_TPAGE_SLOTS := {0x82c0: 5, 0x42c0: 6, 0x150: 7}

static func o10_entrance_tpage_slots(bytes: PackedByteArray) -> Dictionary:
	var slots: Dictionary = {}
	if bytes.size() != 0xc800: return slots
	for page: int in actor_frame_pages(bytes,0x4288,0x4280,9,2,0):
		if not slots.has(page): slots[page] = STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

# Stage10 case8 O12 line17 actor96: frame table 0x801800D8 (6 entries),
# tpage table 0x801800D4 (1 page), flags 0 so only frame0 is drawn through
# stage11 substep2. Same locks as STAGE10_FREE_TPAGE_SLOTS.
static func o12_line17_tpage_slots(bytes: PackedByteArray) -> Dictionary:
	var slots: Dictionary = {}
	if bytes.size() != 0x8800: return slots
	for page: int in actor_frame_pages(bytes,0xd8,0xd4,6,1,0):
		if not slots.has(page): slots[page] = STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

# Stage11 prologue_line_17_run. Case2 sets actors[96].flags 5 on the case8
# O12 record: sequence_table 0x801800F0 slot0 (rows at 0x8C) steps frames
# 0..4 through frame table 0x801800D8 / tpage table 0x801800D4. Case6 copies
# O_13 0x801D8000 -> 0x80180000 (0x20800) and sets flags 5 again with frame
# table 0x80180204 (7), sequence table 0x80180220 and tpage table
# 0x801801E4 (8 pages). Both actors keep actor_clear's unk_06 = 1.
const LINE17_ACTORS := {
	"o12_rei_line17": {"size":0x8800,"frame_table":0xd8,"frames":6,"sequence_table":0xf0,"tpage_table":0xd4,"pages":1},
	"o13_rei_line17_exit": {"size":0x20800,"frame_table":0x204,"frames":7,"sequence_table":0x220,"tpage_table":0x1e4,"pages":8},
}

static func actor_frame_parts(bytes: PackedByteArray, frame_table: int, tpage_table: int, frames: int, pages: int, frame: int) -> Array:
	# actor_draw_all parts of one frame in draw order (count-1..0), resource
	# at 0x80180000. Empty on any pointer, page or extent outside the bytes.
	var result: Array = []
	if frame < 0 or frame >= frames or frame_table+frames*4 > bytes.size() or tpage_table+pages*4 > bytes.size(): return result
	var pointer := int(bytes.decode_u32(frame_table+frame*4))
	var offset := pointer-0x80180000
	if (pointer & 0x80000000) == 0 or offset < 0 or offset+8 > bytes.size(): return result
	var count := bytes.decode_s16(offset)
	if count < 1 or count > 8 or offset+8+count*14 > bytes.size(): return []
	for part in range(count-1,-1,-1):
		var row := offset+8+part*14
		var texture := bytes.decode_u16(row+4)
		var page := texture >> 6
		if page >= pages: return []
		var page_pointer := int(bytes.decode_u32(tpage_table+page*4))
		var page_offset := page_pointer-0x80180000
		if (page_pointer & 0x80000000) == 0 or page_offset < 0 or page_offset+0x4000 > bytes.size(): return []
		result.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),"origin_x":bytes.decode_s16(offset+2),"origin_y":bytes.decode_s16(offset+4),
			"u":(texture&7)*16,"v":((texture>>3)&7)*16,"page_offset":page_offset,"palette_flags":bytes.decode_u16(row+6),"flip":bytes.decode_u16(row+8),
			"w":bytes.decode_s16(row+10),"h":bytes.decode_s16(row+12)})
	return result

static func actor_sequence_rows(bytes: PackedByteArray, sequence_table: int, slot: int, frames: int) -> Array:
	# sequence_table[slot] -> s16 count, then (frame, duration) rows.
	var result: Array = []
	if slot < 0 or sequence_table+slot*4+4 > bytes.size(): return result
	var offset := int(bytes.decode_u32(sequence_table+slot*4))-0x80180000
	if offset < 0 or offset+2 > bytes.size(): return result
	var count := bytes.decode_s16(offset)
	if count < 1 or count > 16 or offset+2+count*4 > bytes.size(): return result
	for index in count:
		var frame := bytes.decode_s16(offset+2+index*4)
		var duration := bytes.decode_s16(offset+4+index*4)
		if frame < 0 or frame >= frames or duration <= 0: return []
		result.append({"frame":frame,"duration":duration})
	return result

static func line17_frame_parts(profile: String, bytes: PackedByteArray, frame: int) -> Array:
	var spec: Variant = LINE17_ACTORS.get(profile)
	if not spec is Dictionary or bytes.size() != spec.size: return []
	return actor_frame_parts(bytes,spec.frame_table,spec.tpage_table,spec.frames,spec.pages,frame)

static func line17_sequence(profile: String, bytes: PackedByteArray) -> Array:
	var spec: Variant = LINE17_ACTORS.get(profile)
	if not spec is Dictionary or bytes.size() != spec.size: return []
	return actor_sequence_rows(bytes,spec.sequence_table,0,spec.frames)

static func line17_tpage_slots(profile: String, bytes: PackedByteArray, frame: int) -> Dictionary:
	# search_tpage for actor96's one drawn frame: the same locks as
	# STAGE10_FREE_TPAGE_SLOTS (prologue_background_slot_set relocks the
	# cut in case6), first free slot per new page in part draw order.
	var slots: Dictionary = {}
	var parts := line17_frame_parts(profile,bytes,frame)
	if parts.is_empty(): return slots
	for part: Dictionary in parts:
		if slots.has(part.page_offset): continue
		if slots.size() >= STAGE10_FREE_TPAGE_SLOTS.size(): return {}
		slots[part.page_offset] = STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

static func o11_drawn_frames(opening: Dictionary) -> Array:
	# O11 frames actor_draw_all draws, ascending actor index (96 body, 97
	# mouth, 98 eyes). The post-switch O10-table actor98 fails the bit31
	# pointer test (actor_sprites.c:93) and allocates nothing.
	var frames: Array = [0]
	for index in [97,98]:
		var actor: Variant = opening.get("actor%d" % index)
		if actor is Dictionary and actor.get("profile") == "o11_rei_lip": frames.append(actor.get("frame_index"))
	return frames

static func o11_tpage_slots(bytes: PackedByteArray, frames: Array) -> Dictionary:
	# search_tpage per frame: reuse a slot already holding the pointer, else
	# take the first free slot. frames are drawn actors in ascending index.
	var slots: Dictionary = {}
	for frame: Variant in frames:
		if not frame is int: return {}
		var pages := o11_frame_pages(bytes,frame)
		if pages.is_empty(): return {}
		for page: int in pages:
			if slots.has(page): continue
			if slots.size() >= STAGE10_FREE_TPAGE_SLOTS.size(): return {}
			slots[page] = STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

# Stage19 O_13 actor96 (prologue.c:2629-2643): frame table 0x80180204 (7),
# sequence table 0x80180220 (slot 0), tpage table 0x801801E4 (8 pages of
# 0x4000). Pages are allocated per frame from STAGE10_FREE_TPAGE_SLOTS in
# first-use draw order (search_tpage, actor_sprites.c:509-555), assuming
# actor96 is the only actor drawn (spec open question 2).
const O13_BYTES := 0x20800
# tpage_table words 0x80180230 + k*0x4000 (k = 0..7) as resource offsets: the
# 8-bit 0x4000-byte blocks load_csr_tp uploads into a tpage_buf slot.
const O13_PAGE_OFFSETS := [0x230, 0x4230, 0x8230, 0xc230, 0x10230, 0x14230, 0x18230, 0x1c230]

static func o13_frame_pages(bytes: PackedByteArray, frame: int) -> Array[int]:
	if bytes.size() != O13_BYTES: return [] as Array[int]
	return actor_frame_pages(bytes,0x204,0x1e4,7,8,frame)

static func o13_tpage_slots(bytes: PackedByteArray, frame: int) -> Dictionary:
	var slots: Dictionary = {}
	for page: int in o13_frame_pages(bytes,frame):
		if not slots.has(page): slots[page] = STAGE10_FREE_TPAGE_SLOTS[slots.size()]
	return slots

static func o13_sequence(bytes: PackedByteArray) -> Array:
	# Sequence slot 0 as [frame, duration] rows: actor_sprites.c:311-346 reads
	# seq[0] as the count and seq[step*2+1] / seq[step*2+2] as frame/duration.
	if bytes.size() != O13_BYTES: return []
	var offset := int(bytes.decode_u32(0x220))-0x80180000
	if offset < 0 or offset+2 > bytes.size(): return []
	var count := bytes.decode_s16(offset)
	if count < 1 or offset+2+count*4 > bytes.size(): return []
	var rows: Array = []
	for index in count:
		var frame := bytes.decode_s16(offset+2+index*4)
		if frame < 0 or frame >= 7: return []
		rows.append([frame,bytes.decode_s16(offset+4+index*4)])
	return rows

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

static func _check_stage_resource_manifest(entry: Variant, file_name: String, path: String, sha: String, total_bytes: int, lba: int, sectors: int) -> bool:
	# A resource read to several destinations: the record pins the disc extent
	# and must not name any destination.
	if not entry is Dictionary or entry.get("file") != file_name: return false
	var source: Variant = entry.get("source")
	var read: Variant = entry.get("read")
	if not source is Dictionary or source.get("path") != path or source.get("sha256") != sha or not _integer(source.get("bytes"),total_bytes): return false
	if not read is Dictionary or read.size() != 4 or read.has("destination") or not _integer(read.get("lba"),lba) or not _integer(read.get("sectors"),sectors) or read.get("transfer_sha256") != sha or not _integer(read.get("transfer_bytes"),total_bytes): return false
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
	if not names_bg is Dictionary or names_bg.size() != 12 or not _check_names_bg_manifest(names_bg.get("0x495d"),"CDROM/BS/P_01_03.BS",BG_495D_SOURCE_SHA,16392,0x495d,BG_495D_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0x4966"),"CDROM/BS/P_01_03B.BS",BG_4966_SOURCE_SHA,8192,0x4966,BG_4966_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacdd"),"CDROM/PROLODIR/P_01_04B.BS",BG_ACDD_SOURCE_SHA,17952,0xacdd,BG_ACDD_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xace6"),"CDROM/PROLODIR/P_01_05.BS",BG_ACE6_SOURCE_SHA,10532,0xace6,BG_ACE6_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacf6"),"CDROM/PROLODIR/P_01_07.BS",BG_ACF6_SOURCE_SHA,19568,0xacf6,BG_ACF6_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacd4"),"CDROM/PROLODIR/P_01_04.BS",BG_ACD4_SOURCE_SHA,17016,0xacd4,BG_ACD4_TRANSFER_SHA,1,[320,256,320,160]) or not _check_names_bg_manifest(names_bg.get("0xacec"),"CDROM/PROLODIR/P_01_06.BS",BG_ACEC_SOURCE_SHA,18492,0xacec,BG_ACEC_TRANSFER_SHA,0,[704,0,320,160]) or not _check_names_bg_manifest(names_bg.get("0xad00"),"CDROM/PROLODIR/P_01_07B.BS",BG_AD00_SOURCE_SHA,8656,0xad00,BG_AD00_TRANSFER_SHA,1,[320,256,320,160]):
		return "names background manifest contract differs"
	if not _check_names_bg_manifest(names_bg.get("0xad0b"),"CDROM/PROLODIR/P_01_09.BS",BG_AD0B_SOURCE_SHA,15988,0xad0b,BG_AD0B_TRANSFER_SHA,1,[320,256,320,160]):
		return "stages 12-14 AD0B background manifest differs"
	for spec: Array in STAGES_16_17_BACKGROUNDS:
		if not _check_names_bg_manifest(names_bg.get("0x%x" % spec[5]),spec[2],spec[3],spec[4],spec[5],spec[6],spec[8],spec[9]):
			return "stages 16-17 background manifest differs: " + spec[1]
	var stage_xfers: Variant = m.get("stage_02_04_transfers")
	if not stage_xfers is Dictionary or stage_xfers.size() != 19 or not _check_stage_transfer_manifest(stage_xfers.get("0xab18"),"cut_12.nbn","CDROM/PROLODIR/CUT_12.NBN",CUT_12_SHA,100352,0xab18,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xac3f"),"o_12.nbn","CDROM/PROLODIR/O_12.NBN",O_12_SHA,34816,0xac3f,17,0x80180000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa23"),"cut_05.nbn","CDROM/PROLODIR/CUT_05.NBN",CUT_05_SHA,100352,0xaa23,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabb8"),"o_05.nbn","CDROM/PROLODIR/O_05.NBN",O_05_SHA,18432,0xabb8,9,0x80180000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa54"),"cut_06.nbn","CDROM/PROLODIR/CUT_07.NBN",CUT_06_SHA,100352,0xaa54,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabca"),"o_06.nbn","CDROM/PROLODIR/O_07.NBN",O_06_SHA,51200,0xabca,25,0x801f0000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaa85"),"cut_07.nbn","CDROM/PROLODIR/CUT_08.NBN",CUT_07_SHA,100352,0xaa85,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabe3"),"o_07.nbn","CDROM/PROLODIR/O_08.NBN",O_07_SHA,51200,0xabe3,25,0x801f0000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaab6"),"cut_08.nbn","CDROM/PROLODIR/CUT_09.NBN",CUT_08_SHA,100352,0xaab6,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xabfc"),"o_08.nbn","CDROM/PROLODIR/O_09.NBN",O_08_SHA,18432,0xabfc,9,0x801f0000) or not _check_stage_transfer_manifest(stage_xfers.get("0xaae7"),"cut_09.nbn","CDROM/PROLODIR/CUT_10.NBN",CUT_09_SHA,100352,0xaae7,49,0x801a4000) or not _check_stage_transfer_manifest(stage_xfers.get("0xac05"),"o_09.nbn","CDROM/PROLODIR/O_10.NBN",O_09_SHA,51200,0xac05,25,0x80180000):
		return "stage 2..6 NBN transfer manifest contract differs"
	if not _check_stage_transfer_manifest(stage_xfers.get("0xac1e"),"o_11.nbn","CDROM/PROLODIR/O_11.NBN",O_11_SHA,67584,0xac1e,33,0x801e8000):
		return "Rei next-scene backup transfer manifest differs"
	for spec: Array in STAGES_12_14_TRANSFERS:
		var entry: Variant = stage_xfers.get("0x%x" % spec[5])
		var described: bool = _check_stage_resource_manifest(entry,spec[1],spec[2],spec[3],spec[4],spec[5],spec[6]) if spec[7] < 0 else _check_stage_transfer_manifest(entry,spec[1],spec[2],spec[3],spec[4],spec[5],spec[6],spec[7])
		if not described or not _integer(entry.get("actor_area_copy_bytes"),spec[8]):
			return "stages 12-17 NBN transfer manifest differs: " + spec[1]
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
	if bytes["names_ad00.bs"].size() != 8656 or shas["names_ad00.bs"] != BG_AD00_SOURCE_SHA or bytes["names_ad00.transfer.bin"].size() != 20480 or shas["names_ad00.transfer.bin"] != BG_AD00_TRANSFER_SHA or bytes["names_ad00.transfer.bin"].slice(0,8656) != bytes["names_ad00.bs"]:
		return "0xad00 bitstream/transfer extent differs"
	if bytes["cut_12.nbn"].size() != 100352 or shas["cut_12.nbn"] != CUT_12_SHA or bytes["o_12.nbn"].size() != 34816 or shas["o_12.nbn"] != O_12_SHA or bytes["cut_05.nbn"].size() != 100352 or shas["cut_05.nbn"] != CUT_05_SHA or bytes["o_05.nbn"].size() != 18432 or shas["o_05.nbn"] != O_05_SHA or bytes["cut_06.nbn"].size() != 100352 or shas["cut_06.nbn"] != CUT_06_SHA or bytes["o_06.nbn"].size() != 51200 or shas["o_06.nbn"] != O_06_SHA or bytes["cut_07.nbn"].size() != 100352 or shas["cut_07.nbn"] != CUT_07_SHA or bytes["o_07.nbn"].size() != 51200 or shas["o_07.nbn"] != O_07_SHA or bytes["cut_08.nbn"].size() != 100352 or shas["cut_08.nbn"] != CUT_08_SHA or bytes["o_08.nbn"].size() != 18432 or shas["o_08.nbn"] != O_08_SHA or bytes["cut_09.nbn"].size() != 100352 or shas["cut_09.nbn"] != CUT_09_SHA or bytes["o_09.nbn"].size() != 51200 or shas["o_09.nbn"] != O_09_SHA:
		return "stage 2..6 NBN transfer payload extent or hash differs"
	if bytes["o_11.nbn"].size() != 67584 or shas["o_11.nbn"] != O_11_SHA:
		return "Rei next-scene backup payload differs"
	for spec: Array in STAGES_12_14_TRANSFERS:
		var data: PackedByteArray = bytes[spec[1]]
		# cd_read_sectors is a checked read: the extent ends in a 0x1012 word.
		if data.size() != spec[4] or data.size() != int(spec[6])*2048 or shas[spec[1]] != spec[3] or (data.decode_u32(data.size()-4) >> 16) != 0x1012:
			return "stages 12-17 NBN transfer payload differs: " + spec[1]
	if bytes["names_ad0b.bs"].size() != 15988 or shas["names_ad0b.bs"] != BG_AD0B_SOURCE_SHA or bytes["names_ad0b.transfer.bin"].size() != 20480 or shas["names_ad0b.transfer.bin"] != BG_AD0B_TRANSFER_SHA or bytes["names_ad0b.transfer.bin"].slice(0,15988) != bytes["names_ad0b.bs"]:
		return "0xad0b bitstream/transfer extent differs"
	for spec: Array in STAGES_16_17_BACKGROUNDS:
		var stem: String = spec[1]
		if bytes[stem+".bs"].size() != spec[4] or shas[stem+".bs"] != spec[3] or bytes[stem+".transfer.bin"].size() != 20480 or shas[stem+".transfer.bin"] != spec[6] or bytes[stem+".transfer.bin"].slice(0,spec[4]) != bytes[stem+".bs"]:
			return "0x%x bitstream/transfer extent differs" % spec[5]
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
	var img_ad00: Variant = _check_mdec_group(bytes,"names_ad00",320,160,BG_AD00_WORD_SHA,shas["names_ad00.rgb555.bin"])
	if img_ad00 is String: return img_ad00
	var img_ad0b: Variant = _check_mdec_group(bytes,"names_ad0b",320,160,BG_AD0B_WORD_SHA,shas["names_ad0b.rgb555.bin"])
	if img_ad0b is String: return img_ad0b
	var imgs_16_17: Dictionary = {}
	for spec: Array in STAGES_16_17_BACKGROUNDS:
		var img: Variant = _check_mdec_group(bytes,spec[1],320,160,spec[7],shas[spec[1]+".rgb555.bin"])
		if img is String: return img
		imgs_16_17[spec[0]] = img
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
		BG_AD00_ID: {"bytes":bytes["names_ad00.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_AD00_TRANSFER_SHA,"lba":0xad00,"sectors":10},
		BG_AD0B_ID: {"bytes":bytes["names_ad0b.transfer.bin"].duplicate(),"destination":0x80162000,"sha256":BG_AD0B_TRANSFER_SHA,"lba":0xad0b,"sectors":10},
	}
	_bg_uploads = {
		BG_495D_ID: {"resource_id":BG_495D_ID,"lba":0x495d,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_495d.rgb555.bin"].duplicate(),"word_sha256":BG_495D_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_4966_ID: {"resource_id":BG_4966_ID,"lba":0x4966,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_4966.rgb555.bin"].duplicate(),"word_sha256":BG_4966_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACDD_ID: {"resource_id":BG_ACDD_ID,"lba":0xacdd,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_acdd.rgb555.bin"].duplicate(),"word_sha256":BG_ACDD_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACE6_ID: {"resource_id":BG_ACE6_ID,"lba":0xace6,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_ace6.rgb555.bin"].duplicate(),"word_sha256":BG_ACE6_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACF6_ID: {"resource_id":BG_ACF6_ID,"lba":0xacf6,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_acf6.rgb555.bin"].duplicate(),"word_sha256":BG_ACF6_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACD4_ID: {"resource_id":BG_ACD4_ID,"lba":0xacd4,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_acd4.rgb555.bin"].duplicate(),"word_sha256":BG_ACD4_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_ACEC_ID: {"resource_id":BG_ACEC_ID,"lba":0xacec,"buffer":0,"rectangle":[704,0,320,160],"words":bytes["names_acec.rgb555.bin"].duplicate(),"word_sha256":BG_ACEC_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_AD00_ID: {"resource_id":BG_AD00_ID,"lba":0xad00,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_ad00.rgb555.bin"].duplicate(),"word_sha256":BG_AD00_WORD_SHA,"manifest_sha256":_manifest_sha},
		BG_AD0B_ID: {"resource_id":BG_AD0B_ID,"lba":0xad0b,"buffer":1,"rectangle":[320,256,320,160],"words":bytes["names_ad0b.rgb555.bin"].duplicate(),"word_sha256":BG_AD0B_WORD_SHA,"manifest_sha256":_manifest_sha},
	}
	_bg_previews = {
		BG_495D_ID: img_495d,
		BG_4966_ID: img_4966,
		BG_ACDD_ID: img_acdd,
		BG_ACE6_ID: img_ace6,
		BG_ACF6_ID: img_acf6,
		BG_ACD4_ID: img_acd4,
		BG_ACEC_ID: img_acec,
		BG_AD00_ID: img_ad00,
		BG_AD0B_ID: img_ad0b,
	}
	for spec: Array in STAGES_16_17_BACKGROUNDS:
		_bg_transfers[spec[0]] = {"bytes":bytes[spec[1]+".transfer.bin"].duplicate(),"destination":0x80162000,"sha256":spec[6],"lba":spec[5],"sectors":10}
		_bg_uploads[spec[0]] = {"resource_id":spec[0],"lba":spec[5],"buffer":spec[8],"rectangle":spec[9].duplicate(),"words":bytes[spec[1]+".rgb555.bin"].duplicate(),"word_sha256":spec[7],"manifest_sha256":_manifest_sha}
		_bg_previews[spec[0]] = imgs_16_17[spec[0]]
	_stage_transfers = {
		CUT_12_ID: {"bytes":bytes["cut_12.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_12_SHA,"lba":0xab18,"sectors":49},
		O_12_ID: {"bytes":bytes["o_12.nbn"].duplicate(),"destination":0x80180000,"sha256":O_12_SHA,"lba":0xac3f,"sectors":17},
		CUT_05_ID: {"bytes":bytes["cut_05.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_05_SHA,"lba":0xaa23,"sectors":49},
		O_05_ID: {"bytes":bytes["o_05.nbn"].duplicate(),"destination":0x80180000,"sha256":O_05_SHA,"lba":0xabb8,"sectors":9},
		CUT_06_ID: {"bytes":bytes["cut_06.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_06_SHA,"lba":0xaa54,"sectors":49},
		O_06_ID: {"bytes":bytes["o_06.nbn"].duplicate(),"destination":0x801f0000,"sha256":O_06_SHA,"lba":0xabca,"sectors":25},
		CUT_07_ID: {"bytes":bytes["cut_07.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_07_SHA,"lba":0xaa85,"sectors":49},
		CUT_09_ID: {"bytes":bytes["cut_09.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_09_SHA,"lba":0xaae7,"sectors":49},
		O_09_ID: {"bytes":bytes["o_09.nbn"].duplicate(),"destination":0x80180000,"sha256":O_09_SHA,"lba":0xac05,"sectors":25},
		O_11_ID: {"bytes":bytes["o_11.nbn"].duplicate(),"destination":0x801e8000,"sha256":O_11_SHA,"lba":0xac1e,"sectors":33},
		CUT_08_ID: {"bytes":bytes["cut_08.nbn"].duplicate(),"destination":0x801a4000,"sha256":CUT_08_SHA,"lba":0xaab6,"sectors":49},
		O_07_ID: {"bytes":bytes["o_07.nbn"].duplicate(),"destination":0x801f0000,"sha256":O_07_SHA,"lba":0xabe3,"sectors":25},
		O_08_ID: {"bytes":bytes["o_08.nbn"].duplicate(),"destination":0x801f0000,"sha256":O_08_SHA,"lba":0xabfc,"sectors":9},
	}
	for spec: Array in STAGES_12_14_TRANSFERS:
		var item := {"bytes":bytes[spec[1]].duplicate(),"sha256":spec[3],"lba":spec[5],"sectors":spec[6]}
		if spec[7] >= 0: item["destination"] = spec[7]
		_stage_transfers[spec[0]] = item
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
		str(0xad00): {"id":BG_AD00_ID,"sha256":BG_AD00_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xad00,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_AD00_WORD_SHA},
		# Stage12 case2 dec_bg_cd_read(0xAD0B,1), decoded into hidden buffer 1.
		str(0xad0b): {"id":BG_AD0B_ID,"sha256":BG_AD0B_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xad0b,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_AD0B_WORD_SHA},
		# Stage11 case4 dec_bg_cd_read(0xAD05,1), decoded into buffer 0.
		str(0xad05): {"id":BG_AD05_ID,"sha256":BG_AD05_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0xad05,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_AD05_WORD_SHA},
		# Stage16 case10 / stage17 case2 (and their retries).
		str(0x496a): {"id":BG_496A_ID,"sha256":BG_496A_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0x496a,"sectors":10,"destination":0x80162000,"buffer":1,"rectangle":[320,256,320,160],"word_sha256":BG_496A_WORD_SHA},
		str(0x4973): {"id":BG_4973_ID,"sha256":BG_4973_TRANSFER_SHA,"slot":1,"decoded":true,"lba":0x4973,"sectors":10,"destination":0x80162000,"buffer":0,"rectangle":[704,0,320,160],"word_sha256":BG_4973_WORD_SHA},
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

func _base_actor_descriptions() -> Dictionary:
	return {
		"49:2149203968:43800": {"id":CUT_12_ID,"sha256":CUT_12_SHA,"sectors":49,"destination":0x801a4000,"lba":0xab18},
		"17:2149056512:44095": {"id":O_12_ID,"sha256":O_12_SHA,"sectors":17,"destination":0x80180000,"lba":0xac3f},
		# Same original O_12.NBN bytes; stage10/substep6 reads it to 0x801f0000.
		"17:2149515264:44095": {"id":O_12_ID,"sha256":O_12_SHA,"sectors":17,"destination":0x801f0000,"lba":0xac3f},
		"49:2149203968:43555": {"id":CUT_05_ID,"sha256":CUT_05_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa23},
		"9:2149056512:43960": {"id":O_05_ID,"sha256":O_05_SHA,"sectors":9,"destination":0x80180000,"lba":0xabb8},
		"49:2149203968:43604": {"id":CUT_06_ID,"sha256":CUT_06_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa54},
		"25:2149515264:43978": {"id":O_06_ID,"sha256":O_06_SHA,"sectors":25,"destination":0x801f0000,"lba":0xabca},
		"49:2149203968:43653": {"id":CUT_07_ID,"sha256":CUT_07_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaa85},
		"49:2149203968:43751": {"id":CUT_09_ID,"sha256":CUT_09_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaae7},
		"25:2149056512:44037": {"id":O_09_ID,"sha256":O_09_SHA,"sectors":25,"destination":0x80180000,"lba":0xac05},
		"33:2149482496:44062": {"id":O_11_ID,"sha256":O_11_SHA,"sectors":33,"destination":0x801e8000,"lba":0xac1e},
		"49:2149203968:43702": {"id":CUT_08_ID,"sha256":CUT_08_SHA,"sectors":49,"destination":0x801a4000,"lba":0xaab6},
		"25:2149515264:44003": {"id":O_07_ID,"sha256":O_07_SHA,"sectors":25,"destination":0x801f0000,"lba":0xabe3},
		"9:2149515264:44028": {"id":O_08_ID,"sha256":O_08_SHA,"sectors":9,"destination":0x801f0000,"lba":0xabfc},
	}

func actor_descriptions() -> Dictionary:
	if _root.is_empty(): return {}
	var result := _base_actor_descriptions()
	# Stages 11-17: one composite per source read (O_13 has two, and its
	# stage transfer record names neither destination).
	for spec: Array in STAGES_11_17_READS:
		result["%d:%d:%d" % [spec[2],spec[3],spec[4]]] = {"id":spec[0],"sha256":spec[1],"sectors":spec[2],"destination":spec[3],"lba":spec[4]}
	return result

func stage_transfer(resource_id: String) -> Dictionary:
	if _root.is_empty() or not _stage_transfers.has(resource_id):
		return {"error":"unknown stage 2..6 NBN transfer resource"}
	var item: Dictionary = _stage_transfers[resource_id]
	# -1: a multi-read resource (O_13) has no single destination.
	return {"error":"","bytes":item.bytes.duplicate(),"destination":item.get("destination",-1),"sha256":item.sha256,"lba":item.lba,"sectors":item.sectors}

func stages_12_14_ready() -> bool:
	# The stages 12-14 resources are part of the combined board pack.
	return not _root.is_empty()
