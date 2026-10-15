from pathlib import Path
import re,shutil
BASE=Path('drafts/T-16000')
for folder in BASE.iterdir():
 if not folder.is_dir():continue
 history=folder/'attempts'/'before-layout-repair';history.mkdir(parents=True,exist_ok=True)
 for name in ['body.c','declarations.h','context.c','diagnostics.txt','result.json']:
  if (folder/name).exists() and not (history/name).exists():shutil.copyfile(folder/name,history/name)
for name in ['func_80062948','func_8007BBE8','func_8005CC40','parameter_up_down']:(BASE/name/'preamble.c').write_text('void '+name+'(); /* T-16000: before earlier unprototyped calls in this TU. */\n')
p=BASE/'func_800476C0'/'body.c';s=p.read_text().replace('                                        case 32:\n','');p.write_text(s)
p=BASE/'func_80054E6C'/'body.c';s=p.read_text().replace('GameState *var_v0','u8 *var_v0').replace('= &D_800E6280;','= (u8 *)&D_800E6280;').replace('(s32) var_v0->unk_0FC','*(s32 *)(var_v0 + 0xFC)')
for offset in range(4):s=s.replace('var_v0_2->unk_1BC[0xF].unk_14['+str(offset)+']','var_v0_2[0x518 + '+str(offset)+']')
p.write_text(s)
p=BASE/'func_80052F68'/'body.c';s=p.read_text();s=re.sub(r'^    (?:s16|s32) sp5E(?:\[[^\]]*\])?;\n','',s,flags=re.M).replace('{','{\n    s16 sp48[12];',1).replace('&sp5E','(sp48 + 11)');p.write_text(s)
p=BASE/'func_8006B0C8'/'body.c';s=p.read_text().replace('{','{\n    s32 sp7C[8];',1);p.write_text(s)
p=BASE/'func_80056BA8'/'body.c';s=p.read_text().replace('    s8 spFD;\n','').replace('    s16 spF0;\n','').replace('s32 spEC[1];','DISPENV spEC;').replace('s32 sp90[1];','DRAWENV sp90;').replace('u8 sp5C;','CdlFILE sp5C;').replace('spFD','spEC.isrgb24').replace('spF0','spEC.disp.w').replace('= sp5C;','= *(s32 *)&sp5C;');p.write_text(s)
p=BASE/'get_near_menu'/'body.c';s=p.read_text().replace('{','{\n    s16 sp88[16];',1).replace('void *','u8 *');s=re.sub(r'\b(var_v0_[2-9])->unk0',r'*\1',s).replace('*(s16 *)((unsigned char *)(sp + (var_s2 * 2)) + (136))','sp88[var_s2]');s=re.sub(r', M2C_ERROR\(/\* Read from unset register \$a1 \*/\), 1','',s);s=s.replace('(&D_800E6280 +','((u8 *)&D_800E6280 +');p.write_text(s)
p=BASE/'dialogue_render'/'body.c';s=p.read_text();sizes={'sp8C':512,'sp28C':24,'sp2A4':64,'sp84':3,'sp80':3,'sp7C':3,'sp78':3,'sp74':3,'sp70':3}
for name,size in sizes.items():s=s.replace('    u8 '+name+';','    u8 '+name+'['+str(size)+'];')
s=s.replace('{','{\n    typedef struct { u8 b[3]; } MainCopy3;',1)
for name,sym in [('sp80','D_800B2228'),('sp7C','D_800B222C'),('sp78','D_800B2230'),('sp74','D_800B2234'),('sp70','D_800B2238')]:
 s=re.sub(r'^.*M2C_LWL.*\b'+sym+r'\b.*$', '    *(MainCopy3 *)'+name+' = *(MainCopy3 *)&'+sym+';',s,flags=re.M)
for name in sizes:s=re.sub(r'&'+name+r'\b',name,s)
for first,second in [('sp80','sp81'),('sp7C','sp7D'),('sp78','sp79'),('sp74','sp75'),('sp70','sp71'),('sp84','sp85'),('sp2A4','sp2A5')]:s=re.sub(r'\b'+second+r'\b',first+'[1]',s)
for name in ['sp80','sp7C','sp78','sp74','sp70','sp84','sp2A4']:
 s=re.sub(r'(==\s*)'+name+r'\b(?!\[)',r'\1'+name+'[0]',s);s=re.sub(r'\('+name+r'\s*==', '('+name+'[0] ==',s)
s=s.replace('void dialogue_render(', 's32 dialogue_render(');p.write_text(s)
p=Path('tools/main_draft_repairs.py');s=p.read_text();s=s.replace('BASE=Path(\'drafts/T-16000\')',"BASE=Path('drafts/T-16000')\nLOG=[]")
s=s.replace(' p=subprocess.run(args,capture_output=True,text=True)'," p=subprocess.run(args,capture_output=True,text=True)\n LOG.append('$ '+' '.join(args)+'\\n'+('Preprocessed C retained in full-context.c\\n' if args[0]=='gcc' and '-E' in args else p.stdout)+p.stderr)")
s=s.replace("text=autotweak.rewrite_guard(bulk.insert_draft(original,name,b,decl),name,b);", "preamble=(out/'preamble.c').read_text() if (out/'preamble.c').exists() else ''\n text=preamble+autotweak.rewrite_guard(bulk.insert_draft(original,name,b,decl),name,b);LOG=[];")
s=s.replace("(out/'diagnostics.txt').write_text('Full translation unit compiled with IDO 5.3. Score '+str(score)+'\\n')", "(out/'diagnostics.txt').write_text(('\\n'.join(LOG)+'\\nFull translation unit compiled with IDO 5.3. Score '+str(score)).rstrip()+'\\n')")
s=s.replace("(out/'diagnostics.txt').write_text(str(e)+'\\n')", "(out/'diagnostics.txt').write_text(('\\n'.join(LOG)+'\\n'+str(e)).rstrip()+'\\n')")
p.write_text(s)
