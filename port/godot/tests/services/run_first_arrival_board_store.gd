extends SceneTree
const Store=preload("res://services/first_arrival_board_store.gd")
var checks:=0
var failures:Array[String]=[]
func check(ok:bool,label:String)->void:
 checks+=1
 if not ok:failures.append(label)
func _initialize()->void:
 var root:=OS.get_environment("BOARD_PACK")
 var store:=Store.new()
 check(store.read_payload().is_empty() and not store.software_upload().error.is_empty(),"unopened role grants no payload")
 var error:String=store.open_pack(root,Store.MANIFEST_SHA)
 check(error.is_empty(),"exact original board role opens: "+error)
 if not error.is_empty():finish();return
 var bytes:PackedByteArray=store.read_payload()
 check(bytes.size()==18432,"native nine-sector read extent")
 var read:Dictionary=store.native_read_descriptor()
 check(read.destination==0x80162000 and read.lba==0x4954 and read.sectors==9 and not read.service_completion_supplied,"board read preserves bitstream buffer and supplies no fake completion")
 bytes[0]^=1
 check(store.read_payload()[0]!=bytes[0],"callers cannot mutate original read payload")
 var upload:Dictionary=store.software_upload()
 check(upload.rectangle==[320,0,640,160] and upload.words.size()==204800,"true decode/upload rectangle and extent")
 check(upload.following_move_requires_prior_owned_vram and not upload.hardware_pixel_parity_verified,"reader cannot fake unknown MoveSquare rows or GPU parity")
 upload.words[0]^=1
 check(store.software_upload().words[0]!=upload.words[0],"callers cannot mutate decoded source words")
 check(store.preview().get_size()==Vector2i(640,160),"source-proved original panorama size")
 var bg_descs:Dictionary=store.background_descriptions()
 check(bg_descs.size()==7 and bg_descs.has(str(0x495d)) and bg_descs.has(str(0x4966)) and bg_descs.has(str(0xacdd)) and bg_descs.has(str(0xace6)) and bg_descs.has(str(0xacf6)) and bg_descs.has(str(0xacd4)) and bg_descs.has(str(0xacec)),"names background descriptions cover 0x495D, 0x4966, 0xACDD, 0xACE6, 0xACF6, 0xACD4, and 0xACEC")
 var tr_495d:Dictionary=store.background_transfer(Store.BG_495D_ID)
 var up_495d:Dictionary=store.background_software_upload(Store.BG_495D_ID)
 check(tr_495d.error.is_empty() and tr_495d.lba==0x495d and tr_495d.sectors==10 and tr_495d.destination==0x80162000 and tr_495d.bytes.size()==20480 and up_495d.error.is_empty() and up_495d.buffer==0 and up_495d.rectangle==[704,0,320,160] and up_495d.words.size()==102400 and store.background_preview(Store.BG_495D_ID).get_size()==Vector2i(320,160),"authenticated 0x495D transfer, upload words, and preview")
 var tr_4966:Dictionary=store.background_transfer(Store.BG_4966_ID)
 var up_4966:Dictionary=store.background_software_upload(Store.BG_4966_ID)
 check(tr_4966.error.is_empty() and tr_4966.lba==0x4966 and tr_4966.sectors==10 and tr_4966.destination==0x80162000 and tr_4966.bytes.size()==20480 and up_4966.error.is_empty() and up_4966.buffer==1 and up_4966.rectangle==[320,256,320,160] and up_4966.words.size()==102400 and store.background_preview(Store.BG_4966_ID).get_size()==Vector2i(320,160),"authenticated 0x4966 transfer, upload words, and preview")
 var tr_acdd:Dictionary=store.background_transfer(Store.BG_ACDD_ID)
 var up_acdd:Dictionary=store.background_software_upload(Store.BG_ACDD_ID)
 check(tr_acdd.error.is_empty() and tr_acdd.lba==0xacdd and tr_acdd.sectors==10 and tr_acdd.destination==0x80162000 and tr_acdd.bytes.size()==20480 and up_acdd.error.is_empty() and up_acdd.buffer==1 and up_acdd.rectangle==[320,256,320,160] and up_acdd.words.size()==102400 and store.background_preview(Store.BG_ACDD_ID).get_size()==Vector2i(320,160),"authenticated 0xACDD transfer, upload words, and preview")
 var tr_ace6:Dictionary=store.background_transfer(Store.BG_ACE6_ID)
 var up_ace6:Dictionary=store.background_software_upload(Store.BG_ACE6_ID)
 check(tr_ace6.error.is_empty() and tr_ace6.lba==0xace6 and tr_ace6.sectors==10 and tr_ace6.destination==0x80162000 and tr_ace6.bytes.size()==20480 and up_ace6.error.is_empty() and up_ace6.buffer==0 and up_ace6.rectangle==[704,0,320,160] and up_ace6.words.size()==102400 and store.background_preview(Store.BG_ACE6_ID).get_size()==Vector2i(320,160),"authenticated 0xACE6 transfer, upload words, and preview")
 var tr_acf6:Dictionary=store.background_transfer(Store.BG_ACF6_ID)
 var up_acf6:Dictionary=store.background_software_upload(Store.BG_ACF6_ID)
 check(tr_acf6.error.is_empty() and tr_acf6.lba==0xacf6 and tr_acf6.sectors==10 and tr_acf6.destination==0x80162000 and tr_acf6.bytes.size()==20480 and up_acf6.error.is_empty() and up_acf6.buffer==0 and up_acf6.rectangle==[704,0,320,160] and up_acf6.words.size()==102400 and store.background_preview(Store.BG_ACF6_ID).get_size()==Vector2i(320,160),"authenticated 0xACF6 transfer, upload words, and preview")
 var tr_acd4:Dictionary=store.background_transfer(Store.BG_ACD4_ID)
 var up_acd4:Dictionary=store.background_software_upload(Store.BG_ACD4_ID)
 check(tr_acd4.error.is_empty() and tr_acd4.lba==0xacd4 and tr_acd4.sectors==10 and tr_acd4.destination==0x80162000 and tr_acd4.bytes.size()==20480 and up_acd4.error.is_empty() and up_acd4.buffer==1 and up_acd4.rectangle==[320,256,320,160] and up_acd4.words.size()==102400 and store.background_preview(Store.BG_ACD4_ID).get_size()==Vector2i(320,160),"authenticated 0xACD4 transfer, upload words, and preview")
 var tr_acec:Dictionary=store.background_transfer(Store.BG_ACEC_ID)
 var up_acec:Dictionary=store.background_software_upload(Store.BG_ACEC_ID)
 check(tr_acec.error.is_empty() and tr_acec.lba==0xacec and tr_acec.sectors==10 and tr_acec.destination==0x80162000 and tr_acec.bytes.size()==20480 and up_acec.error.is_empty() and up_acec.buffer==0 and up_acec.rectangle==[704,0,320,160] and up_acec.words.size()==102400 and store.background_preview(Store.BG_ACEC_ID).get_size()==Vector2i(320,160),"authenticated 0xACEC transfer, upload words, and preview")
 var actor_descs:Dictionary=store.actor_descriptions()
 check(actor_descs.size()==8 and store.stage_transfer(Store.CUT_12_ID).error.is_empty() and store.stage_transfer(Store.O_12_ID).error.is_empty() and store.stage_transfer(Store.CUT_05_ID).error.is_empty() and store.stage_transfer(Store.O_05_ID).error.is_empty() and store.stage_transfer(Store.CUT_06_ID).error.is_empty() and store.stage_transfer(Store.O_06_ID).error.is_empty() and store.stage_transfer(Store.CUT_07_ID).error.is_empty() and store.stage_transfer(Store.O_07_ID).error.is_empty(),"authenticated stage 2..6 NBN transfers (CUT_12, O_12, CUT_05, O_05, CUT_06, O_06, CUT_07, O_07)")
 check(not store.open_pack(root,Store.MANIFEST_SHA).is_empty(),"existing immutable resource owner cannot reopen")
 for pin:String in ["","bad","0".repeat(64)]:
  var rejected:=Store.new()
  check(not rejected.open_pack(root,pin).is_empty() and rejected.read_payload().is_empty() and rejected.preview()==null,"foreign pin rejects without claiming ownership")
 var private:=OS.get_environment("BOARD_PRIVATE")
 var manifest_path:=private.path_join("manifest.json")
 var original:=FileAccess.get_file_as_bytes(manifest_path)
 var parsed:Dictionary=JSON.parse_string(original.get_string_from_utf8())
 for mutation:String in ["destination","geometry","readiness","unknown_rows","source"]:
  var wrong:=parsed.duplicate(true)
  match mutation:
   "destination":wrong.read.destination=0x80180000
   "geometry":wrong.decode.destination=[320,0,320,240]
   "readiness":wrong.service_readiness_granted=true
   "unknown_rows":wrong.following_move.requires_prior_owned_vram=false
   "source":wrong.source.sha256="0".repeat(64)
  var file:=FileAccess.open(manifest_path,FileAccess.WRITE)
  if file==null:check(false,"BOARD_PRIVATE must be writable");finish();return
  file.store_string(JSON.stringify(wrong));file.close()
  var rejected:=Store.new()
  check(not rejected.open_pack(private,FileAccess.get_sha256(manifest_path)).is_empty() and rejected.preview()==null,"repinned forged manifest cannot bypass source profile: "+mutation)
 var restored:=FileAccess.open(manifest_path,FileAccess.WRITE);restored.store_buffer(original);restored.close()
 var words_path:=private.path_join("board.rgb555.bin")
 var source_words:=FileAccess.get_file_as_bytes(words_path)
 var corrupt:=source_words.duplicate();corrupt[1]&=0x7f
 var file:=FileAccess.open(words_path,FileAccess.WRITE);file.store_buffer(corrupt);file.close()
 check(not Store.new().open_pack(private,Store.MANIFEST_SHA).is_empty(),"missing mode2 output bit15 or changed transfer rejects")
 file=FileAccess.open(words_path,FileAccess.WRITE);file.store_buffer(source_words);file.close()
 var extra_path:=private.path_join("unlisted.bin")
 file=FileAccess.open(extra_path,FileAccess.WRITE);file.store_buffer(PackedByteArray([1]));file.close()
 check(not Store.new().open_pack(private,Store.MANIFEST_SHA).is_empty(),"unlisted role file rejects")
 check(DirAccess.remove_absolute(extra_path)==OK,"private extra-file fixture removed")
 var source_path:=private.path_join("board.bs")
 check(DirAccess.remove_absolute(source_path)==OK,"private source copy removed for symlink negative")
 var directory:=DirAccess.open(private)
 var linked:=directory!=null and directory.create_link(root.path_join("board.bs"),source_path)==OK
 check(linked,"private source symlink fixture created")
 if linked:
  check(not Store.new().open_pack(private,Store.MANIFEST_SHA).is_empty(),"linked source payload rejects")
  if DirAccess.remove_absolute(source_path)!=OK:
   check(false,"private symlink must be removed before restoration");finish();return
 file=FileAccess.open(source_path,FileAccess.WRITE);file.store_buffer(store.read_payload());file.close()
 check(Store.new().open_pack(private,Store.MANIFEST_SHA).is_empty(),"negative fixtures leave the private role fully restored")
 finish()
func finish()->void:
 print(JSON.stringify({"checks":checks,"failures":failures,"runtime_readiness_granted":false}))
 quit(0 if failures.is_empty() else 1)
