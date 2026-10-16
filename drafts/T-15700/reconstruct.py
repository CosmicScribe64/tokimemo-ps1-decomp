#!/usr/bin/env python3
"""T-15700: recover observed stack vectors, copied tables and canonical ABI fields."""
from pathlib import Path
import re,shutil
BASE=Path('drafts/T-15700')
def slots(body,start,count,typ,scalar='s16'):
    name='stack'+format(start,'X');width=2
    tokens={x for x in re.findall(r'\bsp[0-9A-Fa-f]+\b',body) if start<=int(x[2:],16)<start+count*width and (int(x[2:],16)-start)%width==0}
    for token in tokens:body=re.sub(r'^    (?:s16|s32|u16|u32) '+token+r';\n','',body,flags=re.M)
    if typ=='SVECTOR':
        body=body.replace('{','{\n    SVECTOR '+name+';',1);fields=['vx','vy','vz','pad'];mapping={token:name+'.'+fields[(int(token[2:],16)-start)//2] for token in tokens}
    else:
        body=body.replace('{','{\n    '+scalar+' '+name+'['+str(count)+'];',1);mapping={token:name+'['+str((int(token[2:],16)-start)//2)+']' for token in tokens}
    return re.sub(r'\bsp[0-9A-Fa-f]+\b',lambda m:mapping.get(m[0],m[0]),body)
def array(body,name,typ,count):
    body=re.sub(r'^    (?:s16|s32|u8) '+name+r'(?:\[[^\]]*\])?;\n','',body,flags=re.M)
    return body.replace('{','{\n    '+typ+' '+name+'['+str(count)+'];',1)
for folder in sorted(BASE.glob('*/*')):
    if not (folder/'result.json').exists():continue
    body=(folder/'body.c').read_text();decl=(folder/'declarations.h').read_text();func=folder.name;unit=folder.parent.name
    # Preserve first complete compiler inputs before editing.
    hist=folder/'attempts'/'first-full-diagnostics'
    if not hist.exists():
        hist.mkdir(parents=True)
        for name in ['body.c','context.c','declarations.h','diagnostics.txt','result.json']:
            if (folder/name).exists():shutil.copyfile(folder/name,hist/name)
    if unit=='TACO':
        if func=='func_80137264':
            body=array(body,'sp40','u8',11);a=body.index('    sp40.unk0');b=body.index('    var_s3 =',a);body=body[:a]+'    *(RepairBytes11 *)sp40 = *(RepairBytes11 *)&D_8015F428;\n'+body[b:];decl+='typedef struct { u8 b[11]; } RepairBytes11; /* observed 8+3 byte copy */\n'
        if func=='func_8013A790':body=body.replace('*(D_8015F4EC + var_s0_3) =','(D_8015F4EC + var_s0_3)->a =')
        for name,base in [('func_8013DEE4',0x6C),('func_8013E778',0x88),('func_8013EAA4',0xA8),('func_80145650',0x74),('func_801497EC',0x68)]:
            if func==name:body=slots(body,base,4,'SVECTOR')
        if func=='func_8013EAA4':body=slots(body,0xB0,8,'array')
        if func=='func_80154960':
            decl=decl.replace('void func_80156178(void);','s32 func_80156178(void);')
            # The archived companion definition is given its consumed machine-level result;
            # production matched C is untouched. reproduce.py makes that type completion.
        if func=='func_8015ACCC':
            a=body.index('{');body='s32 func_8015ACCC(s32 a, s32 b, TcPos c, TcPos d, TcPos e, s32 f, s32 g) '+body[a:]
            for name in ['c','d','e']:body=body.replace(name+'_unk0',name+'.x').replace(name+'_unk4',name+'.z')
    else:
        if func=='func_80132BD0':
            body=array(body,'spF8','u8',23)
            for name in ['spC8','sp98','sp68']:body=array(body,name,'s16',23)
            a=body.index('    spF8.unk0');b=body.index('    var_s0 =',a)
            init='    *(RepairBytes23 *)spF8 = *(RepairBytes23 *)&D_80148F10;\n'+''.join('    *(RepairShort23 *)'+local+' = *(RepairShort23 *)&'+sym+';\n' for local,sym in [('spC8','D_80148F28'),('sp98','D_80148F58'),('sp68','D_80148F88')])
            body=body[:a]+init+body[b:];body=re.sub(r'^    \*\((?:s32|u16) \*\)\(\(unsigned char \*\)temp_t4.*\n','',body,flags=re.M)
            for name in ['spF8','spC8','sp98','sp68']:body=body.replace('&'+name,name)
            decl+='typedef struct { u8 b[23]; } RepairBytes23; /* observed 20+3 bytes */\ntypedef struct { s16 h[23]; } RepairShort23; /* observed 40+4+2 bytes */\n'
        if func=='func_80132E20':
            body=array(body,'sp78','s16',23);a=body.index('    *(M2cBlock28 *)(&sp78');b=body.index('    do {',a)
            body=body[:a]+'    *(RepairShort23 *)sp78 = *(RepairShort23 *)&D_80148FB8;\n    var_s4 = D_8011ECD0;\n    var_s5 = 0;\n'+body[b:];decl+='typedef struct { s16 h[23]; } RepairShort23; /* 46 observed copied bytes */\n'
        if func=='func_80134060':
            for name in ['sp68','sp54']:body=array(body,name,'s16',9);body=body.replace(name+'[0].unk0','*(s32 *)&'+name+'[0]')
            body=body.replace('*(s16 *)((unsigned char *)(sp + (temp_hi * 2)) + (64))','((s16 *)sp40)[temp_hi]').replace('*(s16 *)((unsigned char *)(sp + (temp_hi_2 * 2)) + (64))','((s16 *)sp40)[temp_hi_2]')
        if func=='func_80135F3C':
            body=array(body,'spF0','s32',11)
            for name in ['spD8','spC0']:body=array(body,name,'s16',11);body=body.replace(name+'[0].unk0','*(s32 *)&'+name+'[0]')
            body=body.replace('*(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (240))','spF0[D_801491EC]')
        if func=='func_80138DF8':
            body=array(body,'sp70','s32',11).replace('s32 sp44[10];','s32 sp44[11];').replace('&sp70[0] + 0x24','(u8 *)sp70 + 0x24')
            body=body.replace('*(s32 *)((unsigned char *)(sp + (D_80149940 * 4)) + (68))','sp44[D_80149940]')
        if func=='func_8013D114':body=body.replace('D_80149980','(*(s32 *)&D_80149980)')
        if func in ['func_80143DD0','func_80143F50']:body=re.sub(r'\bD_8014A13C\b(?!\[)','D_8014A13C[0]',body)
        if func=='func_80147B14':
            body=body.replace('    s32 sp30;\n','').replace('    s32 sp32;\n','');body=array(body,'sp2C','s16',8)
            body=body.replace('sp2C[0].unk0','*(s32 *)&sp2C[0]').replace('(&sp2C[0])[4].unk0','sp2C[4]').replace('sp30','sp2C[2]').replace('sp32','sp2C[3]')
    (folder/'body.c').write_text(body);(folder/'declarations.h').write_text(decl)
p=BASE/'reproduce.py';s=p.read_text();s=s.replace("original=Path(row['src']).read_text();headers=", "original=Path(row['src']).read_text()\n    if func=='func_80154960':\n        original=original.replace('void func_80156178(void)', 's32 func_80156178(void)').replace('    func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);', '    return func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);')\n    headers=")
p.write_text(s)
