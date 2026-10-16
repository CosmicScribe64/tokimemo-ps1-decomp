from pathlib import Path
import re
base=Path('drafts/T-16000')
p=base/'dialogue_render/body.c';s=p.read_text();marker=s.rfind('    D_800CA2C0 =');s=s[:marker].replace('return set_kanji_string(', 'set_kanji_string(')+s[marker:];p.write_text(s)
p=base/'func_80052F68/body.c';s=p.read_text()
for name,index in [('sp4A',1),('sp4C',2),('sp4E',3)]:
 s=s.replace('    s16 '+name+';\n','');s=re.sub(r'\b'+name+r'\b','sp48['+str(index)+']',s)
s=s.replace('var_a2_4 += 8;', 'var_a2_4 += 4;').replace('var_v1_4 += 8;', 'var_v1_4 += 4;');p.write_text(s)
