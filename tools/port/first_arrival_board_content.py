"""Additive board resource preparation; no service readiness or scene execution."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,sys,tempfile
from pathlib import Path
SCHEMA='tokimemo.first-arrival-board.v1'
SOURCE='CDROM/BS/P_01_02.BS'
SOURCE_SHA='61b0cd543a73c64ff9ad6d1e77d48217c02dbfbe7dab4941e90451b76f20f398'
NAMES_495D_SOURCE='CDROM/BS/P_01_03.BS'
NAMES_495D_SOURCE_SHA='aa2e82335720f7091a54ccc26e7e0a94be3a75fa1513d6bc1d5e1f3e36e5b230'
NAMES_495D_TRANSFER_SHA='064fa4988819e19c9c858500d4f49caa6a96112d99d04793f5e088c278ed4e50'
NAMES_4966_SOURCE='CDROM/BS/P_01_03B.BS'
NAMES_4966_SOURCE_SHA='090ad2700ac094805298eabeb90560308e89ad7cca098c27146a24b8181b5c92'
NAMES_4966_TRANSFER_SHA='1216a2ea05373a751e76db3f05d2c98b1206dc651d3b93d638964376fb83c68b'
NAMES_ACDD_SOURCE='CDROM/PROLODIR/P_01_04B.BS'
NAMES_ACDD_SOURCE_SHA='ffa02dc52c645679bdbfd66739254e1088fd58672321acbefa2d671c870f2ba2'
NAMES_ACDD_TRANSFER_SHA='67d96aca10962b5379aa9193a996b2eed2f659b01514097ba5b06118ea9a91ea'
NAMES_ACE6_SOURCE='CDROM/PROLODIR/P_01_05.BS'
NAMES_ACE6_SOURCE_SHA='d082130abe5c199b7d8c9c75add436ce0f0f33b23e6a58e3e2e06782aa41119e'
NAMES_ACE6_TRANSFER_SHA='626af576b1adcb054e360b2e2bfca4a5bd1b7f9f94662f42310aaada3d976bd4'
NAMES_ACF6_SOURCE='CDROM/PROLODIR/P_01_07.BS'
NAMES_ACF6_SOURCE_SHA='a4f19c201f7650e14be9c1eca419d3055c7704d6785f1a8263cd3f1eea559980'
NAMES_ACF6_TRANSFER_SHA='f6f921e4a85c00d30c9c63c55a1bb6ac70425de4433c7b0028e4a1e30e802e79'
NAMES_ACD4_SOURCE='CDROM/PROLODIR/P_01_04.BS'
NAMES_ACD4_SOURCE_SHA='dcf99efbac821fbb769847204c0e8a1a1b611ed15daa0450700a59257387fae6'
NAMES_ACD4_TRANSFER_SHA='a2f0c71eb50a70d2028aa48f6bcfaea65b5524369eb9cc5aaf1362f9a7fb340a'
NAMES_ACEC_SOURCE='CDROM/PROLODIR/P_01_06.BS'
NAMES_ACEC_SOURCE_SHA='5feccf3ed08b49ed918458c4772c870b8c1533e7fbc0391df314e59172c7858c'
NAMES_ACEC_TRANSFER_SHA='f2acadc280ae1d94841c9c08f455c5ece893dc371988ff56bcceacbfe68d80b9'
NAMES_AD00_SOURCE='CDROM/PROLODIR/P_01_07B.BS'
NAMES_AD00_SOURCE_SHA='184c047fce1e4da8f736980e8d25a38aec0b4fc41d8297095b8a2bea9e50a774'
NAMES_AD00_TRANSFER_SHA='03b1b7225d0a7c512642f209e3dab711941b397b0fbf086b9dddd811c91815bb'
CUT_12_SOURCE='CDROM/PROLODIR/CUT_12.NBN'
CUT_12_SHA='43b8e386cd4588a67c3a8ac9c71c7ac0416bfce3a340cb502ac508764606bd97'
O_12_SOURCE='CDROM/PROLODIR/O_12.NBN'
O_12_SHA='b2458308e1b1de9714a59a49239728b4be60b4eaff30100566e155c2403c1954'
CUT_05_SOURCE='CDROM/PROLODIR/CUT_05.NBN'
CUT_05_SHA='34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c'
O_05_SOURCE='CDROM/PROLODIR/O_05.NBN'
O_05_SHA='59bd16f64d1510ab290dc4af732418a91534c16eee313f1e0540d8890a606ac5'
CUT_06_SOURCE='CDROM/PROLODIR/CUT_07.NBN'
CUT_06_SHA='3fff84684395c29849d57652a111282820977b4ca5fd44006deab14b3eefa125'
O_06_SOURCE='CDROM/PROLODIR/O_07.NBN'
O_06_SHA='fb0053368ca62fa7ba2f4de8615b9a8e631284ee35ea952472cc62e74c506715'
CUT_07_SOURCE='CDROM/PROLODIR/CUT_08.NBN'
CUT_07_SHA='444165354346eeac36f2614a3375bd24e3ff5b457924b1d81705b4e174c3e3ca'
O_07_SOURCE='CDROM/PROLODIR/O_08.NBN'
O_07_SHA='239a20180cd00bd8c117603075505147fff5d1e1f5769eb94323c7d0fd78de77'
CUT_09_SOURCE='CDROM/PROLODIR/CUT_10.NBN'
CUT_09_SHA='4feef785ad85d06129b0e13ac88c7984e3284ae71d9dfac8c272d300835341c7'
O_09_SOURCE='CDROM/PROLODIR/O_10.NBN'
O_09_SHA='dacf206a52a5396872e759effd471f9a21a769b2a57c9a141edc221789fb3ead'
O_11_SOURCE='CDROM/PROLODIR/O_11.NBN'
O_11_SHA='3e901c18b0ed186323446151f35892bdb66e477d8a419e772034fd2b61be0628'
CUT_08_SOURCE='CDROM/PROLODIR/CUT_09.NBN'
CUT_08_SHA='9c4a73089b11368c754de62a546344668182fb2b37ba9862e5bed0263d9cbd0b'
O_08_SOURCE='CDROM/PROLODIR/O_09.NBN'
O_08_SHA='6dcde1bbf652c9cf5ca77c9c68547afe723ce08b56bfe6c08bf3048fa85ac331'
# Stage11 prologue_line_17_run (prologue.c:1756): case4 dec_bg_cd_read(0xAD05,1)
# (P_01_08) and case5 cd_read_sectors(0x41,0x801D8000,0xAC50) (O_13), copied
# 0x20800 to 0x80180000 by case6. O_13 is read from more than one place (the
# stage17 read targets 0x80180000), so its manifest record names only the disc
# extent; each read's destination lives in the BoardStore per-read composite.
NAMES_AD05_SOURCE='CDROM/PROLODIR/P_01_08.BS'
NAMES_AD05_SOURCE_SHA='d9b717964b37f3dcb0ef57856170269e79c10185281958bde7a8a69b94052cf2'
NAMES_AD05_TRANSFER_SHA='a7d9ab6fecca526d79333041a23a462e64b999a824a50e70fdf8c207901a5925'
# Stages 11-20 (wiki/spec/opening-stages-12-14.md, -15-17.md, -18-20.md). Ids
# follow the disc file names. Every extent is the exact native sector transfer
# of one prologue.c checked read and ends in its 0x1012 trailer word:
#   0xAC50 O_13.NBN    65  stage11 case5 to 0x801D8000 (0x20800 copy) and
#                          stage17 case3 to 0x80180000 (stage19 actor96)
#   0xAB49 CUT_14.NBN  49 -> 0x801A4000  stage12 case0
#   0xAC91 O_14.NBN     9 -> 0x801F0000  stage12 case1 (0x4800 copy)
#   0xAC9A O_15.NBN    33 -> 0x801E8000  stage13 case3 (0x10800 copy)
#   0xAB7A CUT_16.NBN  49 -> 0x801A4000  stage14 case4
#   0xACBB O_16.NBN    25 -> 0x801E8000  stage14 case5 (0xC800 copy)
#   0xAD0B P_01_09.BS  10 -> 0x80162000  stage12 case2, decoded at case3
# O_13 is read to two destinations, so its record names none; each read is a
# per-read composite key in the board store's actor_descriptions().
O_13_SOURCE='CDROM/PROLODIR/O_13.NBN'
O_13_SHA='38c0149c0fb6bd9b6b812b06a5c170a2529e7a0271b0cb3ffc812e6ce6b4236f'
CUT_16_SOURCE='CDROM/PROLODIR/CUT_16.NBN'
CUT_16_SHA='de1f091899464b4c04916815f71d6da40221da0cf3ab30c6336765e7ba279b17'
O_16_SOURCE='CDROM/PROLODIR/O_16.NBN'
O_16_SHA='0bde8b1c5285bf6f1509adbfab894a983c0f5cd5c5238979643f7bbd84abe03f'
AD0B_SOURCE='CDROM/PROLODIR/P_01_09.BS'
AD0B_SOURCE_SHA='78c2813b109d87b5692783bbfc1bfd69ce5c89d015ed2b070cc5c8f5c7082098'
AD0B_TRANSFER_SHA='c0327b980bbef7daeb31a20bdb3de992d36ec34d606fde2809e37dcb948e52b3'
# Stages 16-17 slot-1 backgrounds (wiki/spec/opening-stages-15-17.md). Each
# dec_bg_cd_read moves 10 sectors; the 18432-byte file is 9, so the extent
# carries the first sector of the next file (P_01_12, then X_01_13). The
# decode buffer follows dec_bg_show[1] at runtime; the record names the one
# the uninterrupted source path selects (16/11 after 15/34 showed buffer 0).
STAGES_16_17_BS=[
 ('names_496a','CDROM/BS/P_01_10.BS',0x496a,18432,'c064b04b84daa381117a3b37e1c68b6e0a754b5f559dfc26aa579042e2f16762','56bbdab9a66a99bb3e2bb526a16a78107b66dd03767b2330548d3c3ff3157c9a',1,[320,256,320,160],'prologue_lines_32_to_36_run'),
 ('names_4973','CDROM/BS/P_01_12.BS',0x4973,18432,'e5c33edfb057565c49dd9b9366781c825dae153929757632294931e8598aff08','c7dbbf759146d38b1b50f2c18373186b6523c25e71b06fb7d3eaf02ab4875f11',0,[704,0,320,160],'prologue_line_37_run'),
]
# Stage11 prologue_line_17_run (prologue.c:1756-1822): case4
# dec_bg_cd_read(0xAD05,1) (P_01_08, 11788 bytes; the 10-sector extent carries
# the following sectors), case6 dec_bg_show_set(1,dec_bg_decode_to_vram(1))
# and the dec_bg_reset retry. Stage10 showed ACF6 in buffer 1, so the
# uninterrupted path decodes P_01_08 into buffer 0; stages 12-14 show it.
LINE17_BS=[
 ('names_ad05','CDROM/PROLODIR/P_01_08.BS',0xad05,11788,'d9b717964b37f3dcb0ef57856170269e79c10185281958bde7a8a69b94052cf2','a7d9ab6fecca526d79333041a23a462e64b999a824a50e70fdf8c207901a5925',0,[704,0,320,160],'prologue_line_17_run'),
]
# Every slot-1 background exported by the generic extent/decode path.
SLOT1_EXTRA_BS=LINE17_BS+STAGES_16_17_BS
# (file name, ISO path, LBA, file bytes, sha256, sectors, destination or None, copy bytes, checked-read trailer)
STAGES_11_17_NBN=[
 ('o_13.nbn',O_13_SOURCE,0xac50,133120,O_13_SHA,65,None,0x20800,0x1012be83),
 ('cut_14.nbn','CDROM/PROLODIR/CUT_14.NBN',0xab49,100352,'8059ef80df31cb4662db9c8b816b258066e1730251b7454e14d3426b42c2fefe',49,0x801a4000,0,0x1012053a),
 ('o_14.nbn','CDROM/PROLODIR/O_14.NBN',0xac91,18432,'f81f398f969215f0ca65f04ac6624fca8f733568f21bf719fec023c7eb30bee5',9,0x801f0000,0x4800,0x1012da54),
 ('o_15.nbn','CDROM/PROLODIR/O_15.NBN',0xac9a,67584,'9d6aea8ea1356ee666fba64e1f76d22db106fac4ad08b4e99de87ffb9326a0db',33,0x801e8000,0x10800,0x1012f97a),
 ('cut_16.nbn',CUT_16_SOURCE,0xab7a,100352,CUT_16_SHA,49,0x801a4000,0,0x10129557),
 ('o_16.nbn',O_16_SOURCE,0xacbb,51200,O_16_SHA,25,0x801e8000,0xc800,0x10124d0d),
]
TRACK_SHA='28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72'
PINS={
 'src/ovl/name_entry/prologue.c':'cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e',
 'src/main/memory_card.c':'32fd2c5f413038eee79d940c3a964e47652b472935b34f8597231c60d5979811',
 'src/main/screen_layers.c':'03cafa4cc4a0b868bbcc77cba636c262732fabdea6aee2da619663a635363db3',
 'src/main/background_images.c':'7eea24dfee7fd3503beedbd1b2f633c40f19102d21f992557d626cba3d036402',
 'tools/assets/image_mdec.py':'6513b1e6434bdd1c65c804e5d637ae3db1f4fa11f69dd80b495cf05a2f8828ec',
 'tools/assets/image_mdec_model.py':'3b8e5611bf6f763171b8bf3d5a22269211048b0ec21dc342088e90ec043d2222',
 'tools/assets/media_background_context.py':'40441716507a1a407260caec428ad3aba706ee534cd05de67beded68a2d9d95e',
 'tools/assets/visual_text_equivalence.py':'1dfb936326536a29c8d65c050591f8e3df0a80a207c2395210ae01221dfb05b5',
 'tools/port/first_arrival_content.py':'8751e359d2c567edf3155b3bac3bc1b04cce9850324c7048fdea225880204ac1'}
class Invalid(ValueError):pass
def require(ok,message):
 if not ok:raise Invalid(message)
def sha(data):return hashlib.sha256(data).hexdigest()
def physical(path,must_exist=True):
 p=Path(path).absolute();require('..'not in p.parts,'parent traversal rejected')
 for parent in [p,*p.parents]:require(not parent.is_symlink(),'symlink component rejected')
 return p.resolve(strict=must_exist)
def output_path(repo,input_path,output):
 repo=physical(repo);source=physical(input_path);target=physical(output,False)
 require(not target.exists(),'output must be a fresh directory')
 require(target!=source and not(source.is_dir()and(source in target.parents or target in source.parents)),'output overlaps original input')
 if repo in target.parents:
  require(target.relative_to(repo).parts[0]in('.asset-cache','.claude'),'game assets must remain in ignored preparation storage')
 return repo,source,target
def source_context(repo,data):
 for rel,pin in PINS.items():require(sha(physical(repo/rel).read_bytes())==pin,'source/tool pin differs: '+rel)
 sys.path[:0]=[str(repo),str(repo/'tools')]
 from tools.assets import media_background_context as contexts,image_mdec
 inventory={'schema':'tokimemo.media.v1','input':{'source_sha256':TRACK_SHA,'format':'mode2-2352'},'assets':[{'id':'board','source':{'path':SOURCE,'size':len(data),'sha256':sha(data),'location':{'lba':0x4954}}}]}
 report=contexts.build_background_contexts(inventory,{rel:(repo/rel).read_text()for rel in ['src/ovl/name_entry/prologue.c','src/main/background_images.c']})
 streams=report['streams'];require(len(streams)==1 and streams[0]['context_status']=='proved_single_shape','ambiguous board source geometry')
 uses=[u for u in streams[0]['contexts']if u['function']=='prologue_board_load']
 require(len(uses)==1 and [uses[0]['lba'],uses[0]['physical_read_bytes'],uses[0]['input_buffer'],uses[0]['width'],uses[0]['height']]==[0x4954,18432,0x80162000,640,160],'board read/decoder call differs')
 context={'width':640,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_board_load cd_read_sectors(9,0x80162000,0x4954), bg_still_decode_to_vram(320,0,640,160)','src/main/memory_card.c:bg_still_decode_to_vram DecDCT mode2 and16-word column uploads']}
 probe=image_mdec.probe_mdec(data,SOURCE,context);require(probe['status']=='supported'and probe['metadata']['macroblock_count']==400,'board VLC or macroblock extent rejected')
 return context,uses[0]
def names_context(repo,data_495d,data_4966,data_acdd,data_ace6,data_acf6,data_acd4,data_acec):
 sys.path[:0]=[str(repo),str(repo/'tools')]
 from tools.assets import media_background_context as contexts,image_mdec
 inventory={'schema':'tokimemo.media.v1','input':{'source_sha256':TRACK_SHA,'format':'mode2-2352'},'assets':[
  {'id':'names_495d','source':{'path':NAMES_495D_SOURCE,'size':len(data_495d),'sha256':sha(data_495d),'location':{'lba':0x495d}}},
  {'id':'names_4966','source':{'path':NAMES_4966_SOURCE,'size':len(data_4966),'sha256':sha(data_4966),'location':{'lba':0x4966}}},
  {'id':'names_acdd','source':{'path':NAMES_ACDD_SOURCE,'size':len(data_acdd),'sha256':sha(data_acdd),'location':{'lba':0xacdd}}},
  {'id':'names_ace6','source':{'path':NAMES_ACE6_SOURCE,'size':len(data_ace6),'sha256':sha(data_ace6),'location':{'lba':0xace6}}},
  {'id':'names_acf6','source':{'path':NAMES_ACF6_SOURCE,'size':len(data_acf6),'sha256':sha(data_acf6),'location':{'lba':0xacf6}}},
  {'id':'names_acd4','source':{'path':NAMES_ACD4_SOURCE,'size':len(data_acd4),'sha256':sha(data_acd4),'location':{'lba':0xacd4}}},
  {'id':'names_acec','source':{'path':NAMES_ACEC_SOURCE,'size':len(data_acec),'sha256':sha(data_acec),'location':{'lba':0xacec}}}]}
 report=contexts.build_background_contexts(inventory,{rel:(repo/rel).read_text()for rel in ['src/ovl/name_entry/prologue.c','src/main/background_images.c']})
 streams={s['parent_asset_id']:s for s in report['streams']}
 require(len(streams)==7,'missing names background context streams')
 s_495d=streams['names_495d'];s_4966=streams['names_4966'];s_acdd=streams['names_acdd'];s_ace6=streams['names_ace6'];s_acf6=streams['names_acf6'];s_acd4=streams['names_acd4'];s_acec=streams['names_acec']
 require(s_495d['shape_candidates']==[{'width':320,'height':160}] and s_4966['shape_candidates']==[{'width':320,'height':160}] and s_4966['context_status']=='proved_single_shape' and s_acdd['shape_candidates']==[{'width':320,'height':160}] and s_ace6['shape_candidates']==[{'width':320,'height':160}] and s_acf6['shape_candidates']==[{'width':320,'height':160}],'names background source geometry differs')
 u_495d=[u for u in s_495d['contexts']if u['function']=='prologue_board_scroll'and u.get('width')==320]
 u_4966=[u for u in s_4966['contexts']if u['function']=='prologue_names_crossfade'and u.get('width')==320]
 u_acdd=[u for u in s_acdd['contexts']if u['function']=='prologue_names_crossfade'and u.get('width')==320]
 u_ace6=[u for u in s_ace6['contexts']if u['function']=='prologue_line_03_run'and u.get('width')==320]
 u_acf6=[u for u in s_acf6['contexts']if u['function']=='prologue_scene_05_transition_run'and u.get('width')==320]
 u_acd4=[u for u in s_acd4['contexts']if u['function']=='prologue_scene_05_transition_run'and u.get('line')==793]
 u_acec=[u for u in s_acec['contexts']if u['function']=='prologue_yoshio_greeting_run'and u.get('line')==1063]
 require(len(u_495d)==1 and [u_495d[0]['lba'],u_495d[0]['layer'],u_495d[0]['read_sector_count'],u_495d[0]['physical_read_bytes'],u_495d[0]['width'],u_495d[0]['height']]==[0x495d,1,10,20480,320,160],'0x495d read/decoder call differs')
 require(len(u_4966)==1 and [u_4966[0]['lba'],u_4966[0]['layer'],u_4966[0]['read_sector_count'],u_4966[0]['physical_read_bytes'],u_4966[0]['width'],u_4966[0]['height']]==[0x4966,1,10,20480,320,160],'0x4966 read/decoder call differs')
 require(len(u_acdd)==1 and [u_acdd[0]['lba'],u_acdd[0]['layer'],u_acdd[0]['read_sector_count'],u_acdd[0]['physical_read_bytes'],u_acdd[0]['width'],u_acdd[0]['height']]==[0xacdd,1,10,20480,320,160],'0xacdd read/decoder call differs')
 require(len(u_ace6)==1 and [u_ace6[0]['lba'],u_ace6[0]['layer'],u_ace6[0]['read_sector_count'],u_ace6[0]['physical_read_bytes'],u_ace6[0]['width'],u_ace6[0]['height']]==[0xace6,1,10,20480,320,160],'0xace6 read/decoder call differs')
 require(len(u_acf6)==1 and [u_acf6[0]['lba'],u_acf6[0]['layer'],u_acf6[0]['read_sector_count'],u_acf6[0]['physical_read_bytes'],u_acf6[0]['width'],u_acf6[0]['height']]==[0xacf6,1,10,20480,320,160],'0xacf6 read/decoder call differs')
 require(len(u_acd4)==1 and [u_acd4[0]['lba'],u_acd4[0]['layer'],u_acd4[0]['read_sector_count'],u_acd4[0]['physical_read_bytes']]==[0xacd4,1,10,20480],'0xacd4 read/decoder call differs')
 require(len(u_acec)==1 and [u_acec[0]['lba'],u_acec[0]['layer'],u_acec[0]['read_sector_count'],u_acec[0]['physical_read_bytes']]==[0xacec,1,10,20480],'0xacec read/decoder call differs')
 ctx_495d={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_board_scroll dec_bg_cd_read(0x495D,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(704,0,320,160)']}
 ctx_4966={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_names_crossfade dec_bg_cd_read(0x4966,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(320,256,320,160)']}
 ctx_acdd={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_names_crossfade dec_bg_cd_read(0xACDD,1), dec_bg_decode_to_vram(1), prologue_line_04_run dec_bg_show_set(1,dec_bg_cd_read(0xACDD,1))','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(320,256,320,160)']}
 ctx_ace6={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_line_03_run dec_bg_cd_read(0xACE6,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(704,0,320,160)']}
 ctx_acf6={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_scene_05_transition_run dec_bg_cd_read(0xACF6,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(704,0,320,160)']}
 ctx_acd4={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_scene_05_transition_run dec_bg_cd_read(0xACD4,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(320,256,320,160)']}
 ctx_acec={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_yoshio_greeting_run dec_bg_cd_read(0xACEC,1), dec_bg_decode_to_vram(1)','src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(704,0,320,160)']}
 for d,src,ctx in [(data_495d,NAMES_495D_SOURCE,ctx_495d),(data_4966,NAMES_4966_SOURCE,ctx_4966),(data_acdd,NAMES_ACDD_SOURCE,ctx_acdd),(data_ace6,NAMES_ACE6_SOURCE,ctx_ace6),(data_acf6,NAMES_ACF6_SOURCE,ctx_acf6),(data_acd4,NAMES_ACD4_SOURCE,ctx_acd4),(data_acec,NAMES_ACEC_SOURCE,ctx_acec)]:
  probe=image_mdec.probe_mdec(d,src,ctx);require(probe['status']=='supported'and probe['metadata']['macroblock_count']==200,'names VLC or macroblock extent rejected: '+src)
 return (ctx_495d,u_495d[0]),(ctx_4966,u_4966[0]),(ctx_acdd,u_acdd[0]),(ctx_ace6,u_ace6[0]),(ctx_acf6,u_acf6[0]),(ctx_acd4,u_acd4[0]),(ctx_acec,u_acec[0])
def stages_11_17_source_checks(repo):
 # prologue.c is hash-pinned by source_context; these are the read/copy calls
 # whose extents and destinations the stage transfers below pin.
 source=(repo/'src/ovl/name_entry/prologue.c').read_text()
 def body(name,end):return source.split('void '+name+'(void) {',1)[1].split('void '+end,1)[0]
 line17=body('prologue_line_17_run','prologue_mode_stage_line_17')
 load=body('prologue_shiori_load_run','prologue_mode_stage_shiori_load')
 line18=body('prologue_shiori_line_18_run','prologue_mode_stage_shiori_line_18')
 line19=body('prologue_shiori_line_19_run','prologue_mode_stage_shiori_line_19')
 line37=body('prologue_line_37_run','prologue_mode_stage_line_37')
 for text,call in [(line17,'cd_read_sectors(0x41U, 0x801D8000, 0xAC50);'),(line17,'memcpy((void *)0x80180000, 0x801D8000, 0x20800);'),
   (load,'cd_read_sectors(0x31U, 0x801A4000, 0xAB49);'),(load,'cd_read_sectors(9U, 0x801F0000, 0xAC91);'),(load,'dec_bg_cd_read(0xAD0B, 1);'),
   (load,'dec_bg_show_set(1, dec_bg_decode_to_vram(1));'),(load,'memcpy((void *)0x80180000, 0x801F0000, 0x4800);'),
   (line18,'cd_read_sectors(0x21U, 0x801E8000, 0xAC9A);'),(line18,'memcpy((void *)0x80180000, 0x801E8000, 0x10800);'),
   (line19,'dec_bg_cd_read(0xACEC, 1);'),(line19,'cd_read_sectors(0x31U, 0x801A4000, 0xAB7A);'),(line19,'cd_read_sectors(0x19U, 0x801E8000, 0xACBB);'),
   (line19,'memcpy((void *)0x80180000, 0x801E8000, 0xC800);'),(line37,'cd_read_sectors(0x41, 0x80180000, 0xAC50);'),
   (line37,'dec_bg_cd_read(0x4973, 1);'),(line37,'dec_bg_show_set(1, dec_bg_decode_to_vram(1));')]:
  require(call in text,'stage 11-17 source call differs: '+call)
 return {'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_shiori_load_run dec_bg_cd_read(0xAD0B,1) at substep2, dec_bg_show_set(1,dec_bg_decode_to_vram(1)) at substep3','src/main/background_images.c:dec_bg_decode_to_vram hidden slot-1 buffer 320x160']}
def stages_16_17_source_checks(repo):
 # prologue.c is hash-pinned by source_context; the read, retry and decode
 # calls of 16/10-11 and 17/2-3 must stay as the extents above assume.
 source=(repo/'src/ovl/name_entry/prologue.c').read_text()
 def body(name,end):return source.split('void '+name+'(void) {',1)[1].split('void '+end,1)[0]
 for name,path,lba,size,digest,xdigest,buf,rect,fn in SLOT1_EXTRA_BS:
  text=body(fn,'prologue_mode_stage_'+fn[len('prologue_'):-len('_run')])
  call='dec_bg_cd_read(0x%X, 1);'%lba
  require(text.count(call)==2 and text.count('dec_bg_show_set(1, dec_bg_decode_to_vram(1));')==1 and 'dec_bg_reset();' in text,'slot-1 background calls differ: '+fn)
 ctx_16_17={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_lines_32_to_36_run dec_bg_cd_read(0x496A,1) at 10 and the 11 retry; prologue_line_37_run dec_bg_cd_read(0x4973,1) at 2 and the 3 retry','src/main/background_images.c:dec_bg_decode_to_vram hidden slot-1 buffer 320x160']}
 ctx_ad05={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_line_17_run dec_bg_cd_read(0xAD05,1) at 4 and the 6 retry, dec_bg_show_set(1,dec_bg_decode_to_vram(1)) at 6','src/main/background_images.c:dec_bg_decode_to_vram hidden slot-1 buffer 0 (704,0,320,160) after stage10 showed ACF6 in buffer 1']}
 return {name:(ctx_ad05 if name=='names_ad05' else ctx_16_17) for name,*_ in SLOT1_EXTRA_BS}
def _pack_words(png_bytes,expected_size):
 from tools.assets.visual_text_equivalence import _decode_png
 width,height,rgba=_decode_png(png_bytes)
 require((width,height)==expected_size,'canonical MDEC preview geometry differs')
 words=bytearray(width*height*2)
 for index in range(width*height):
  red,green,blue,alpha=rgba[index*4:index*4+4]
  require(alpha==255,'MDEC background unexpectedly has nonopaque pixels')
  struct.pack_into('<H',words,index*2,0x8000|(red>>3)|((green>>3)<<5)|((blue>>3)<<10))
 return words
def line11_background_context(repo,data):
 from tools.assets import media_background_context as contexts,image_mdec
 inventory={'schema':'tokimemo.media.v1','input':{'source_sha256':TRACK_SHA,'format':'mode2-2352'},'assets':[
  {'id':'names_ad00','source':{'path':NAMES_AD00_SOURCE,'size':len(data),'sha256':sha(data),'location':{'lba':0xad00}}}]}
 report=contexts.build_background_contexts(inventory,{rel:(repo/rel).read_text()for rel in ['src/ovl/name_entry/prologue.c','src/main/background_images.c']})
 uses=[u for stream in report['streams'] for u in stream['contexts'] if u['function']=='prologue_line_11_run']
 require(len(uses)==1 and [uses[0][key] for key in ['lba','layer','read_sector_count','physical_read_bytes']]==[0xad00,1,10,20480],'line11 background source read differs')
 # The generic extractor does not bind a decoder nested inside show_set.
 # Both source files are hash-pinned by source_context before this function.
 source=(repo/'src/ovl/name_entry/prologue.c').read_text()
 function=source.split('void prologue_line_11_run(void) {',1)[1].split('void prologue_mode_stage_line_11',1)[0]
 require('dec_bg_show_set(1, dec_bg_decode_to_vram(1));' in function.split('case 7:',1)[1],'line11 delayed decode/show call differs')
 context={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_line_11_run reads 0xAD00 at substep3, decodes at substep7','src/main/background_images.c:dec_bg_decode_to_vram alternate buffer 320,256,320,160']}
 probe=image_mdec.probe_mdec(data,NAMES_AD00_SOURCE,context)
 require(probe['status']=='supported' and probe['metadata']['macroblock_count']==200,'line11 background VLC/geometry differs')
 return context,uses[0]

def line17_background_context(repo,data):
 # prologue_line_17_run reads 0xAD05 at case4 (and again after a failed
 # decode); case6 decodes it into the slot1 alternate buffer.
 from tools.assets import media_background_context as contexts,image_mdec
 inventory={'schema':'tokimemo.media.v1','input':{'source_sha256':TRACK_SHA,'format':'mode2-2352'},'assets':[
  {'id':'names_ad05','source':{'path':NAMES_AD05_SOURCE,'size':len(data),'sha256':sha(data),'location':{'lba':0xad05}}}]}
 report=contexts.build_background_contexts(inventory,{rel:(repo/rel).read_text()for rel in ['src/ovl/name_entry/prologue.c','src/main/background_images.c']})
 uses=[u for stream in report['streams'] for u in stream['contexts'] if u['function']=='prologue_line_17_run']
 require(len(uses)>=1 and all([u[key] for key in ['lba','layer','read_sector_count','physical_read_bytes']]==[0xad05,1,10,20480] for u in uses),'line17 background source read differs')
 source=(repo/'src/ovl/name_entry/prologue.c').read_text()
 function=source.split('void prologue_line_17_run(void) {',1)[1].split('void prologue_mode_stage_line_17',1)[0]
 require('dec_bg_cd_read(0xAD05, 1);' in function.split('case 4:',1)[1].split('case 5:',1)[0] and 'dec_bg_show_set(1, dec_bg_decode_to_vram(1));' in function.split('case 6:',1)[1],'line17 read/decode/show call differs')
 context={'width':320,'height':160,'evidence':['src/ovl/name_entry/prologue.c:prologue_line_17_run reads 0xAD05 at substep4, decodes at substep6','src/main/background_images.c:dec_bg_decode_to_vram hidden buffer 0 (704,0,320,160) after stage10 showed ACF6 in buffer 1']}
 probe=image_mdec.probe_mdec(data,NAMES_AD05_SOURCE,context)
 require(probe['status']=='supported' and probe['metadata']['macroblock_count']==200,'line17 background VLC/geometry differs')
 return context,uses[0]

def build(repo,input_path,output):
 repo,source,target=output_path(repo,input_path,output);sys.path[:0]=[str(repo),str(repo/'tools')]
 from tools.assets.input import open_input
 from tools.port.first_arrival_content import _file_entry,_native_extent
 with open_input(str(source))as original:
  require(original.describe()['source_sha256']==TRACK_SHA,'original track identity differs')
  entries=original.entries()
  entry=_file_entry(entries,SOURCE,0x4954,18432,SOURCE_SHA)
  data,transport=_native_extent(original,entry,TRACK_SHA,9,0x4954)
  require(len(data)==18432 and sha(data)==SOURCE_SHA,'board transfer differs from exact logical BS file')
  e_495d=_file_entry(entries,NAMES_495D_SOURCE,0x495d,16392,NAMES_495D_SOURCE_SHA)
  raw_495d=original.read(e_495d)
  xfer_495d,transport_495d=_native_extent(original,e_495d,TRACK_SHA,10,0x495d)
  require(len(raw_495d)==16392 and sha(raw_495d)==NAMES_495D_SOURCE_SHA and len(xfer_495d)==20480 and sha(xfer_495d)==NAMES_495D_TRANSFER_SHA and xfer_495d[:16392]==raw_495d,'0x495d logical or 10-sector transfer differs')
  e_4966=_file_entry(entries,NAMES_4966_SOURCE,0x4966,8192,NAMES_4966_SOURCE_SHA)
  raw_4966=original.read(e_4966)
  xfer_4966,transport_4966=_native_extent(original,e_4966,TRACK_SHA,10,0x4966)
  require(len(raw_4966)==8192 and sha(raw_4966)==NAMES_4966_SOURCE_SHA and len(xfer_4966)==20480 and sha(xfer_4966)==NAMES_4966_TRANSFER_SHA and xfer_4966[:8192]==raw_4966,'0x4966 logical or 10-sector transfer differs')
  e_acdd=_file_entry(entries,NAMES_ACDD_SOURCE,0xacdd,17952,NAMES_ACDD_SOURCE_SHA)
  raw_acdd=original.read(e_acdd)
  xfer_acdd,transport_acdd=_native_extent(original,e_acdd,TRACK_SHA,10,0xacdd)
  require(len(raw_acdd)==17952 and sha(raw_acdd)==NAMES_ACDD_SOURCE_SHA and len(xfer_acdd)==20480 and sha(xfer_acdd)==NAMES_ACDD_TRANSFER_SHA and xfer_acdd[:17952]==raw_acdd,'0xacdd logical or 10-sector transfer differs')
  e_ace6=_file_entry(entries,NAMES_ACE6_SOURCE,0xace6,10532,NAMES_ACE6_SOURCE_SHA)
  raw_ace6=original.read(e_ace6)
  xfer_ace6,transport_ace6=_native_extent(original,e_ace6,TRACK_SHA,10,0xace6)
  require(len(raw_ace6)==10532 and sha(raw_ace6)==NAMES_ACE6_SOURCE_SHA and len(xfer_ace6)==20480 and sha(xfer_ace6)==NAMES_ACE6_TRANSFER_SHA and xfer_ace6[:10532]==raw_ace6,'0xace6 logical or 10-sector transfer differs')
  e_acf6=_file_entry(entries,NAMES_ACF6_SOURCE,0xacf6,19568,NAMES_ACF6_SOURCE_SHA)
  raw_acf6=original.read(e_acf6)
  xfer_acf6,transport_acf6=_native_extent(original,e_acf6,TRACK_SHA,10,0xacf6)
  require(len(raw_acf6)==19568 and sha(raw_acf6)==NAMES_ACF6_SOURCE_SHA and len(xfer_acf6)==20480 and sha(xfer_acf6)==NAMES_ACF6_TRANSFER_SHA and xfer_acf6[:19568]==raw_acf6,'0xacf6 logical or 10-sector transfer differs')
  e_acd4=_file_entry(entries,NAMES_ACD4_SOURCE,0xacd4,17016,NAMES_ACD4_SOURCE_SHA)
  raw_acd4=original.read(e_acd4)
  xfer_acd4,transport_acd4=_native_extent(original,e_acd4,TRACK_SHA,10,0xacd4)
  require(len(raw_acd4)==17016 and sha(raw_acd4)==NAMES_ACD4_SOURCE_SHA and len(xfer_acd4)==20480 and sha(xfer_acd4)==NAMES_ACD4_TRANSFER_SHA and xfer_acd4[:17016]==raw_acd4,'0xacd4 logical or 10-sector transfer differs')
  e_acec=_file_entry(entries,NAMES_ACEC_SOURCE,0xacec,18492,NAMES_ACEC_SOURCE_SHA)
  raw_acec=original.read(e_acec)
  xfer_acec,transport_acec=_native_extent(original,e_acec,TRACK_SHA,10,0xacec)
  require(len(raw_acec)==18492 and sha(raw_acec)==NAMES_ACEC_SOURCE_SHA and len(xfer_acec)==20480 and sha(xfer_acec)==NAMES_ACEC_TRANSFER_SHA and xfer_acec[:18492]==raw_acec,'0xacec logical or 10-sector transfer differs')
  e_cut12=_file_entry(entries,CUT_12_SOURCE,0xab18,100352,CUT_12_SHA)
  e_ad00=_file_entry(entries,NAMES_AD00_SOURCE,0xad00,8656,NAMES_AD00_SOURCE_SHA)
  raw_ad00=original.read(e_ad00)
  xfer_ad00,transport_ad00=_native_extent(original,e_ad00,TRACK_SHA,10,0xad00)
  require(len(xfer_ad00)==20480 and sha(xfer_ad00)==NAMES_AD00_TRANSFER_SHA and xfer_ad00[:8656]==raw_ad00,'0xad00 original transfer differs')
  xfer_cut12,transport_cut12=_native_extent(original,e_cut12,TRACK_SHA,49,0xab18)
  require(len(xfer_cut12)==100352 and sha(xfer_cut12)==CUT_12_SHA,'0xab18 CUT_12.NBN transfer differs')
  e_o12=_file_entry(entries,O_12_SOURCE,0xac3f,34816,O_12_SHA)
  xfer_o12,transport_o12=_native_extent(original,e_o12,TRACK_SHA,17,0xac3f)
  require(len(xfer_o12)==34816 and sha(xfer_o12)==O_12_SHA,'0xac3f O_12.NBN transfer differs')
  e_cut05=_file_entry(entries,CUT_05_SOURCE,0xaa23,100352,CUT_05_SHA)
  xfer_cut05,transport_cut05=_native_extent(original,e_cut05,TRACK_SHA,49,0xaa23)
  require(len(xfer_cut05)==100352 and sha(xfer_cut05)==CUT_05_SHA,'0xaa23 CUT_05.NBN transfer differs')
  e_o05=_file_entry(entries,O_05_SOURCE,0xabb8,18432,O_05_SHA)
  xfer_o05,transport_o05=_native_extent(original,e_o05,TRACK_SHA,9,0xabb8)
  require(len(xfer_o05)==18432 and sha(xfer_o05)==O_05_SHA,'0xabb8 O_05.NBN transfer differs')
  e_cut06=_file_entry(entries,CUT_06_SOURCE,0xaa54,100352,CUT_06_SHA)
  xfer_cut06,transport_cut06=_native_extent(original,e_cut06,TRACK_SHA,49,0xaa54)
  require(len(xfer_cut06)==100352 and sha(xfer_cut06)==CUT_06_SHA,'0xaa54 CUT_07.NBN transfer differs')
  e_o06=_file_entry(entries,O_06_SOURCE,0xabca,51200,O_06_SHA)
  xfer_o06,transport_o06=_native_extent(original,e_o06,TRACK_SHA,25,0xabca)
  require(len(xfer_o06)==51200 and sha(xfer_o06)==O_06_SHA,'0xabca O_07.NBN transfer differs')
  e_cut07=_file_entry(entries,CUT_07_SOURCE,0xaa85,100352,CUT_07_SHA)
  xfer_cut07,transport_cut07=_native_extent(original,e_cut07,TRACK_SHA,49,0xaa85)
  require(len(xfer_cut07)==100352 and sha(xfer_cut07)==CUT_07_SHA,'0xaa85 CUT_08.NBN transfer differs')
  e_o07=_file_entry(entries,O_07_SOURCE,0xabe3,51200,O_07_SHA)
  xfer_o07,transport_o07=_native_extent(original,e_o07,TRACK_SHA,25,0xabe3)
  require(len(xfer_o07)==51200 and sha(xfer_o07)==O_07_SHA,'0xabe3 O_08.NBN transfer differs')
  e_cut08=_file_entry(entries,CUT_08_SOURCE,0xaab6,100352,CUT_08_SHA)
  xfer_cut08,transport_cut08=_native_extent(original,e_cut08,TRACK_SHA,49,0xaab6)
  e_o08=_file_entry(entries,O_08_SOURCE,0xabfc,18432,O_08_SHA)
  xfer_o08,transport_o08=_native_extent(original,e_o08,TRACK_SHA,9,0xabfc)
  require(sha(xfer_cut08)==CUT_08_SHA and sha(xfer_o08)==O_08_SHA,'line11 actor transfers differ')
  e_cut09=_file_entry(entries,CUT_09_SOURCE,0xaae7,100352,CUT_09_SHA)
  xfer_cut09,transport_cut09=_native_extent(original,e_cut09,TRACK_SHA,49,0xaae7)
  e_o09=_file_entry(entries,O_09_SOURCE,0xac05,51200,O_09_SHA)
  xfer_o09,transport_o09=_native_extent(original,e_o09,TRACK_SHA,25,0xac05)
  require(sha(xfer_cut09)==CUT_09_SHA and sha(xfer_o09)==O_09_SHA,'Rei entrance actor transfers differ')
  e_o11=_file_entry(entries,O_11_SOURCE,0xac1e,67584,O_11_SHA)
  xfer_o11,transport_o11=_native_extent(original,e_o11,TRACK_SHA,33,0xac1e)
  require(len(xfer_o11)==0x10800 and sha(xfer_o11)==O_11_SHA,'Rei next-scene backup transfer differs')
  # Stages 11-17 checked reads (O_13, CUT_14, O_14, O_15, CUT_16, O_16).
  stages_11_17={}
  for name,path,lba,size,digest,sectors,dest,copy,tail in STAGES_11_17_NBN:
   entry=_file_entry(entries,path,lba,size,digest)
   xfer,transport_x=_native_extent(original,entry,TRACK_SHA,sectors,lba)
   require(len(xfer)==size==sectors*2048 and sha(xfer)==digest,path+' transfer differs from the exact file')
   require(int.from_bytes(xfer[-4:],'little')==tail and tail>>16==0x1012,path+' lacks its checked-read 0x1012 trailer')
   stages_11_17[name]=(xfer,transport_x)
  e_ad0b=_file_entry(entries,AD0B_SOURCE,0xad0b,15988,AD0B_SOURCE_SHA)
  raw_ad0b=original.read(e_ad0b)
  xfer_ad0b,transport_ad0b=_native_extent(original,e_ad0b,TRACK_SHA,10,0xad0b)
  require(len(raw_ad0b)==15988 and sha(raw_ad0b)==AD0B_SOURCE_SHA and len(xfer_ad0b)==20480 and sha(xfer_ad0b)==AD0B_TRANSFER_SHA and xfer_ad0b[:15988]==raw_ad0b,'0xad0b logical or 10-sector transfer differs')
  stages_16_17={}
  for name,path,lba,size,digest,xdigest,_buf,_rect,_fn in SLOT1_EXTRA_BS:
   entry=_file_entry(entries,path,lba,size,digest)
   raw=original.read(entry)
   xfer,transport_x=_native_extent(original,entry,TRACK_SHA,10,lba)
   require(len(raw)==size and sha(raw)==digest and len(xfer)==20480 and sha(xfer)==xdigest and xfer[:size]==raw,hex(lba)+' logical or 10-sector transfer differs')
   stages_16_17[name]=(raw,xfer,transport_x)
 context,use=source_context(repo,data)
 ctx_ad00,use_ad00=line11_background_context(repo,raw_ad00)
 # Stage11 source-use check (main T-39002): the generic export below uses the
 # SLOT1_EXTRA_BS context; this keeps prologue_line_17_run's read/decode check.
 line17_background_context(repo,stages_16_17['names_ad05'][0])
 (ctx_495d,use_495d),(ctx_4966,use_4966),(ctx_acdd,use_acdd),(ctx_ace6,use_ace6),(ctx_acf6,use_acf6),(ctx_acd4,use_acd4),(ctx_acec,use_acec)=names_context(repo,raw_495d,raw_4966,raw_acdd,raw_ace6,raw_acf6,raw_acd4,raw_acec)
 ctx_ad0b=stages_11_17_source_checks(repo)
 from tools.assets import image_mdec
 probe_ad0b=image_mdec.probe_mdec(raw_ad0b,AD0B_SOURCE,ctx_ad0b)
 require(probe_ad0b['status']=='supported' and probe_ad0b['metadata']['macroblock_count']==200,'0xad0b VLC/geometry differs')
 ctx_16_17=stages_16_17_source_checks(repo)
 for name,path,lba,*_ in SLOT1_EXTRA_BS:
  probe=image_mdec.probe_mdec(stages_16_17[name][0],path,ctx_16_17[name])
  require(probe['status']=='supported' and probe['metadata']['macroblock_count']==200,hex(lba)+' VLC/geometry differs')
 target.parent.mkdir(parents=True,exist_ok=True)
 stage=Path(tempfile.mkdtemp(prefix='.board-',dir=target.parent))
 try:
  (stage/'board.bs').write_bytes(data)
  decoded=image_mdec.export_mdec(data,stage,'board',source_path=SOURCE,context=context)
  (stage/'board.rgb555.bin').write_bytes(_pack_words((stage/'board.background.png').read_bytes(),(640,160)))
  (stage/'names_495d.bs').write_bytes(raw_495d)
  (stage/'names_495d.transfer.bin').write_bytes(xfer_495d)
  dec_495d=image_mdec.export_mdec(raw_495d,stage,'names_495d',source_path=NAMES_495D_SOURCE,context=ctx_495d)
  (stage/'names_495d.rgb555.bin').write_bytes(_pack_words((stage/'names_495d.background.png').read_bytes(),(320,160)))
  (stage/'names_4966.bs').write_bytes(raw_4966)
  (stage/'names_4966.transfer.bin').write_bytes(xfer_4966)
  dec_4966=image_mdec.export_mdec(raw_4966,stage,'names_4966',source_path=NAMES_4966_SOURCE,context=ctx_4966)
  (stage/'names_4966.rgb555.bin').write_bytes(_pack_words((stage/'names_4966.background.png').read_bytes(),(320,160)))
  (stage/'names_acdd.bs').write_bytes(raw_acdd)
  (stage/'names_acdd.transfer.bin').write_bytes(xfer_acdd)
  dec_acdd=image_mdec.export_mdec(raw_acdd,stage,'names_acdd',source_path=NAMES_ACDD_SOURCE,context=ctx_acdd)
  (stage/'names_acdd.rgb555.bin').write_bytes(_pack_words((stage/'names_acdd.background.png').read_bytes(),(320,160)))
  (stage/'names_ace6.bs').write_bytes(raw_ace6)
  (stage/'names_ace6.transfer.bin').write_bytes(xfer_ace6)
  dec_ace6=image_mdec.export_mdec(raw_ace6,stage,'names_ace6',source_path=NAMES_ACE6_SOURCE,context=ctx_ace6)
  (stage/'names_ace6.rgb555.bin').write_bytes(_pack_words((stage/'names_ace6.background.png').read_bytes(),(320,160)))
  (stage/'names_acf6.bs').write_bytes(raw_acf6)
  (stage/'names_acf6.transfer.bin').write_bytes(xfer_acf6)
  dec_acf6=image_mdec.export_mdec(raw_acf6,stage,'names_acf6',source_path=NAMES_ACF6_SOURCE,context=ctx_acf6)
  (stage/'names_acf6.rgb555.bin').write_bytes(_pack_words((stage/'names_acf6.background.png').read_bytes(),(320,160)))
  (stage/'names_acd4.bs').write_bytes(raw_acd4)
  (stage/'names_acd4.transfer.bin').write_bytes(xfer_acd4)
  dec_acd4=image_mdec.export_mdec(raw_acd4,stage,'names_acd4',source_path=NAMES_ACD4_SOURCE,context=ctx_acd4)
  (stage/'names_acd4.rgb555.bin').write_bytes(_pack_words((stage/'names_acd4.background.png').read_bytes(),(320,160)))
  (stage/'names_acec.bs').write_bytes(raw_acec)
  (stage/'names_acec.transfer.bin').write_bytes(xfer_acec)
  dec_acec=image_mdec.export_mdec(raw_acec,stage,'names_acec',source_path=NAMES_ACEC_SOURCE,context=ctx_acec)
  (stage/'names_acec.rgb555.bin').write_bytes(_pack_words((stage/'names_acec.background.png').read_bytes(),(320,160)))
  (stage/'names_ad00.bs').write_bytes(raw_ad00)
  (stage/'names_ad00.transfer.bin').write_bytes(xfer_ad00)
  dec_ad00=image_mdec.export_mdec(raw_ad00,stage,'names_ad00',source_path=NAMES_AD00_SOURCE,context=ctx_ad00)
  (stage/'names_ad00.rgb555.bin').write_bytes(_pack_words((stage/'names_ad00.background.png').read_bytes(),(320,160)))
  (stage/'cut_12.nbn').write_bytes(xfer_cut12)
  (stage/'o_12.nbn').write_bytes(xfer_o12)
  (stage/'cut_05.nbn').write_bytes(xfer_cut05)
  (stage/'o_05.nbn').write_bytes(xfer_o05)
  (stage/'cut_06.nbn').write_bytes(xfer_cut06)
  (stage/'o_06.nbn').write_bytes(xfer_o06)
  (stage/'cut_07.nbn').write_bytes(xfer_cut07)
  (stage/'o_07.nbn').write_bytes(xfer_o07)
  (stage/'cut_08.nbn').write_bytes(xfer_cut08)
  (stage/'o_08.nbn').write_bytes(xfer_o08)
  (stage/'cut_09.nbn').write_bytes(xfer_cut09)
  (stage/'o_09.nbn').write_bytes(xfer_o09)
  (stage/'o_11.nbn').write_bytes(xfer_o11)
  for name,(xfer,_) in stages_11_17.items():(stage/name).write_bytes(xfer)
  (stage/'names_ad0b.bs').write_bytes(raw_ad0b)
  (stage/'names_ad0b.transfer.bin').write_bytes(xfer_ad0b)
  dec_ad0b=image_mdec.export_mdec(raw_ad0b,stage,'names_ad0b',source_path=AD0B_SOURCE,context=ctx_ad0b)
  (stage/'names_ad0b.rgb555.bin').write_bytes(_pack_words((stage/'names_ad0b.background.png').read_bytes(),(320,160)))
  dec_16_17={}
  for name,path,*_ in SLOT1_EXTRA_BS:
   raw,xfer,_=stages_16_17[name]
   (stage/(name+'.bs')).write_bytes(raw)
   (stage/(name+'.transfer.bin')).write_bytes(xfer)
   dec_16_17[name]=image_mdec.export_mdec(raw,stage,name,source_path=path,context=ctx_16_17[name])
   (stage/(name+'.rgb555.bin')).write_bytes(_pack_words((stage/(name+'.background.png')).read_bytes(),(320,160)))
  files={p.name:{'sha256':sha(p.read_bytes()),'bytes':p.stat().st_size}for p in stage.iterdir()}
  names_backgrounds={
   '0x495d':{'source':{'path':NAMES_495D_SOURCE,'sha256':NAMES_495D_SOURCE_SHA,'bytes':16392},'read':{'lba':0x495d,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_495D_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_495d,'source_use':use_495d,'decode':{'slot':1,'buffer':0,'destination':[704,0,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_495d,'hardware_pixel_parity_verified':False}},
   '0x4966':{'source':{'path':NAMES_4966_SOURCE,'sha256':NAMES_4966_SOURCE_SHA,'bytes':8192},'read':{'lba':0x4966,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_4966_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_4966,'source_use':use_4966,'decode':{'slot':1,'buffer':1,'destination':[320,256,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_4966,'hardware_pixel_parity_verified':False}},
   '0xacdd':{'source':{'path':NAMES_ACDD_SOURCE,'sha256':NAMES_ACDD_SOURCE_SHA,'bytes':17952},'read':{'lba':0xacdd,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_ACDD_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_acdd,'source_use':use_acdd,'decode':{'slot':1,'buffer':1,'destination':[320,256,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_acdd,'hardware_pixel_parity_verified':False}},
   '0xace6':{'source':{'path':NAMES_ACE6_SOURCE,'sha256':NAMES_ACE6_SOURCE_SHA,'bytes':10532},'read':{'lba':0xace6,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_ACE6_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_ace6,'source_use':use_ace6,'decode':{'slot':1,'buffer':0,'destination':[704,0,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_ace6,'hardware_pixel_parity_verified':False}},
   '0xacf6':{'source':{'path':NAMES_ACF6_SOURCE,'sha256':NAMES_ACF6_SOURCE_SHA,'bytes':19568},'read':{'lba':0xacf6,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_ACF6_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_acf6,'source_use':use_acf6,'decode':{'slot':1,'buffer':0,'destination':[704,0,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_acf6,'hardware_pixel_parity_verified':False}},
   '0xacd4':{'source':{'path':NAMES_ACD4_SOURCE,'sha256':NAMES_ACD4_SOURCE_SHA,'bytes':17016},'read':{'lba':0xacd4,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_ACD4_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_acd4,'source_use':use_acd4,'decode':{'slot':1,'buffer':1,'destination':[320,256,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_acd4,'hardware_pixel_parity_verified':False}},
   '0xacec':{'source':{'path':NAMES_ACEC_SOURCE,'sha256':NAMES_ACEC_SOURCE_SHA,'bytes':18492},'read':{'lba':0xacec,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_ACEC_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_acec,'source_use':use_acec,'decode':{'slot':1,'buffer':0,'destination':[704,0,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_acec,'hardware_pixel_parity_verified':False}}}
  names_backgrounds['0xad00']={'source':{'path':NAMES_AD00_SOURCE,'sha256':NAMES_AD00_SOURCE_SHA,'bytes':8656},'read':{'lba':0xad00,'sectors':10,'destination':0x80162000,'transfer_sha256':NAMES_AD00_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_ad00,'source_use':use_ad00,'decode':{'slot':1,'buffer':1,'destination':[320,256,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_ad00,'hardware_pixel_parity_verified':False}}
  stage_02_04_transfers={
   '0xaae7':{'file':'cut_09.nbn','source':{'path':CUT_09_SOURCE,'sha256':CUT_09_SHA,'bytes':100352},'read':{'lba':0xaae7,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_09_SHA,'transfer_bytes':100352},'transport':transport_cut09},
   '0xac05':{'file':'o_09.nbn','source':{'path':O_09_SOURCE,'sha256':O_09_SHA,'bytes':51200},'read':{'lba':0xac05,'sectors':25,'destination':0x80180000,'transfer_sha256':O_09_SHA,'transfer_bytes':51200},'transport':transport_o09},
   '0xac1e':{'file':'o_11.nbn','source':{'path':O_11_SOURCE,'sha256':O_11_SHA,'bytes':67584},'read':{'lba':0xac1e,'sectors':33,'destination':0x801e8000,'transfer_sha256':O_11_SHA,'transfer_bytes':67584},'transport':transport_o11},
   '0xaab6':{'file':'cut_08.nbn','source':{'path':CUT_08_SOURCE,'sha256':CUT_08_SHA,'bytes':100352},'read':{'lba':0xaab6,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_08_SHA,'transfer_bytes':100352},'transport':transport_cut08},
   '0xabfc':{'file':'o_08.nbn','source':{'path':O_08_SOURCE,'sha256':O_08_SHA,'bytes':18432},'read':{'lba':0xabfc,'sectors':9,'destination':0x801f0000,'transfer_sha256':O_08_SHA,'transfer_bytes':18432},'transport':transport_o08},
   '0xab18':{'file':'cut_12.nbn','source':{'path':CUT_12_SOURCE,'sha256':CUT_12_SHA,'bytes':100352},'read':{'lba':0xab18,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_12_SHA,'transfer_bytes':100352},'transport':transport_cut12},
   '0xac3f':{'file':'o_12.nbn','source':{'path':O_12_SOURCE,'sha256':O_12_SHA,'bytes':34816},'read':{'lba':0xac3f,'sectors':17,'destination':0x80180000,'transfer_sha256':O_12_SHA,'transfer_bytes':34816},'transport':transport_o12},
   '0xaa23':{'file':'cut_05.nbn','source':{'path':CUT_05_SOURCE,'sha256':CUT_05_SHA,'bytes':100352},'read':{'lba':0xaa23,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_05_SHA,'transfer_bytes':100352},'transport':transport_cut05},
   '0xabb8':{'file':'o_05.nbn','source':{'path':O_05_SOURCE,'sha256':O_05_SHA,'bytes':18432},'read':{'lba':0xabb8,'sectors':9,'destination':0x80180000,'transfer_sha256':O_05_SHA,'transfer_bytes':18432},'transport':transport_o05},
   '0xaa54':{'file':'cut_06.nbn','source':{'path':CUT_06_SOURCE,'sha256':CUT_06_SHA,'bytes':100352},'read':{'lba':0xaa54,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_06_SHA,'transfer_bytes':100352},'transport':transport_cut06},
   '0xabca':{'file':'o_06.nbn','source':{'path':O_06_SOURCE,'sha256':O_06_SHA,'bytes':51200},'read':{'lba':0xabca,'sectors':25,'destination':0x801f0000,'transfer_sha256':O_06_SHA,'transfer_bytes':51200},'transport':transport_o06},
   '0xaa85':{'file':'cut_07.nbn','source':{'path':CUT_07_SOURCE,'sha256':CUT_07_SHA,'bytes':100352},'read':{'lba':0xaa85,'sectors':49,'destination':0x801a4000,'transfer_sha256':CUT_07_SHA,'transfer_bytes':100352},'transport':transport_cut07},
   '0xabe3':{'file':'o_07.nbn','source':{'path':O_07_SOURCE,'sha256':O_07_SHA,'bytes':51200},'read':{'lba':0xabe3,'sectors':25,'destination':0x801f0000,'transfer_sha256':O_07_SHA,'transfer_bytes':51200},'transport':transport_o07}}
  # Stages 11-17: one record per resource; O_13 (two destinations) names none.
  for name,path,lba,size,digest,sectors,dest,copy,tail in STAGES_11_17_NBN:
   read={'lba':lba,'sectors':sectors,'transfer_sha256':digest,'transfer_bytes':size}
   if dest is not None:read['destination']=dest
   stage_02_04_transfers[hex(lba)]={'file':name,'source':{'path':path,'sha256':digest,'bytes':size},'read':read,'actor_area_copy_bytes':copy,'checked_trailer':tail,'transport':stages_11_17[name][1]}
  names_backgrounds['0xad0b']={'source':{'path':AD0B_SOURCE,'sha256':AD0B_SOURCE_SHA,'bytes':15988},'read':{'lba':0xad0b,'sectors':10,'destination':0x80162000,'transfer_sha256':AD0B_TRANSFER_SHA,'transfer_bytes':20480},'transport':transport_ad0b,
   'decode':{'slot':1,'buffer':1,'destination':[320,256,320,160],'format':'rgb555-stp1','clut':None,'software_preview':dec_ad0b,'hardware_pixel_parity_verified':False}}
  for name,path,lba,size,digest,xdigest,buf,rect,_fn in SLOT1_EXTRA_BS:
   names_backgrounds[hex(lba)]={'source':{'path':path,'sha256':digest,'bytes':size},'read':{'lba':lba,'sectors':10,'destination':0x80162000,'transfer_sha256':xdigest,'transfer_bytes':20480},'transport':stages_16_17[name][2],
    'decode':{'slot':1,'buffer':buf,'destination':rect,'format':'rgb555-stp1','clut':None,'software_preview':dec_16_17[name],'hardware_pixel_parity_verified':False}}
  manifest={'schema':SCHEMA,'release':'original-ja-ps-the-best','source':{'path':SOURCE,'sha256':SOURCE_SHA,'bytes':18432},'source_pins':PINS,'read':{'lba':0x4954,'sectors':9,'destination':0x80162000},'transport':transport,'source_use':use,'decode':{'destination':[320,0,640,160],'format':'rgb555-stp1','clut':None,'software_preview':decoded,'hardware_pixel_parity_verified':False},'following_move':{'call':[5,14,15],'source_height':255,'newly_decoded_height':160,'requires_prior_owned_vram':True},'names_backgrounds':names_backgrounds,'stage_02_04_transfers':stage_02_04_transfers,'files':files,'service_readiness_granted':False,'scene_execution_verified':False}
  (stage/'manifest.json').write_text(json.dumps(manifest,sort_keys=True,indent=2)+'\n');require(not target.exists(),'output appeared during preparation');stage.rename(target);return manifest
 except BaseException:
  shutil.rmtree(stage,ignore_errors=True);raise
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--input',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();v=build(a.source_root,a.input,a.output);print(json.dumps({'source_sha256':v['source']['sha256'],'files':len(v['files']),'service_readiness_granted':False}))
