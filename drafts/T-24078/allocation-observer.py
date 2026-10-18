"""Generate the approved current618 logging-only observer in Docker; audit before reading logs."""
from pathlib import Path
import hashlib, json, subprocess, difflib, sys, shutil

root = Path('build/etc24078/alloc618')
root.mkdir(exist_ok=True)
if len(sys.argv)>1:
    if sys.argv[1:] == ['--prepare']:
        base=Path('build/etc24078/replay/indexed-uv-right')
        expected={'source.c':'93c8e1be669f54a581a7d4a4b9494ea93c522955c5fce99b9f2aebd5a4ef0be7','source.o':'000d7483829f12355913baf14c0a307571fef51be47df2f6c7d93fe4c5906dda'}
        for n,h in expected.items():
            assert hashlib.sha256((base/n).read_bytes()).hexdigest()==h,'pinned stock context changed'
            shutil.copyfile(base/n,root/('stock-'+n))
        (root/'stock.json').write_text(json.dumps(expected,indent=2)+'\n')
        raise SystemExit(0)
    if sys.argv[1:] != ['--audit']:raise SystemExit('use no arguments, --prepare, or --audit')
    sys.path.insert(0,'tools')
    from trailing_pad import Elf
    a=(root/'stock-source.o').read_bytes();b=(root/'diagnostic.o').read_bytes()
    assert a==b,'whole-object mismatch: observations invalid'
    assert hashlib.sha256(a).hexdigest()=='000d7483829f12355913baf14c0a307571fef51be47df2f6c7d93fe4c5906dda','stock changed'
    e,f=Elf(a),Elf(b);sections={}
    for sec in e.sections:
        n=sec['sname']
        if n.startswith(('.text','.data','.rodata','.sdata','.bss','.sbss','.rel')):
            other=f.sections[f.find(n)]
            assert sec['data']==other['data'] and sec['size']==other['size'],n
            sections[n]={'size':sec['size'],'sha256':hashlib.sha256(sec['data']).hexdigest()}
    meta={'whole_object_equal':True,'stock_sha256':hashlib.sha256(a).hexdigest(),'diagnostic_sha256':hashlib.sha256(b).hexdigest(),'equal_sections':sections,'generator':json.loads((root/'generator.json').read_text()),'actual_input_identity':json.loads((root/'input-identity.json').read_text()),'options_changed':False}
    (root/'fidelity.json').write_text(json.dumps(meta,indent=2)+'\n')
    print('PASS exact pinned whole object and all code/data/relocations; observations may now be inspected.')
    raise SystemExit(0)
stock_c = Path('/scratch/recomp/build/5.3/uopt.c')
text = stock_c.read_text()
assert hashlib.sha256(stock_c.read_bytes()).hexdigest() == 'b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f'
helper = r'''
/* T-24078 native read-only diagnostic; no state enters the emulated program. */
static uint32_t t78_lr[2];
static unsigned t78_records, t78_active;
static int t78_target(uint8_t *mem) {
    static const char n[] = "func_8013D570";
    if (MEM_U32(0x1001c8d0) != sizeof(n)-1) return 0;
    for (unsigned i=0;i<sizeof(n)-1;i++)
        if (MEM_U8(0x1001c4d0+i) != (unsigned char)n[i]) return 0;
    return 1;
}
static unsigned t78_focus(uint32_t lr) { return lr && (lr==t78_lr[0] || lr==t78_lr[1]); }
static int t78_edge(uint8_t *mem, uint32_t a, uint32_t b) {
    unsigned count=0;
    for(uint32_t n=MEM_U32(a+56);n && count++<4096;n=MEM_U32(n+4))
        if(MEM_U32(n)==b) return 1;
    return count>=4096 ? -1 : 0;
}
static void t78_init(uint8_t *mem) {
    t78_active=t78_target(mem);t78_records=0;t78_lr[0]=t78_lr[1]=0;
    if(!t78_active) return;
    const char *o=getenv("T24078_PAGE_OFFSET"), *b=getenv("T24078_PAGE_BLOCK"), *m=getenv("T24078_PAGE_MTYPE");
    if(!o||!b||!m) {fprintf(stderr,"T78 missing-page-input-identity\n");return;}
    int32_t off=(int32_t)strtol(o,0,0);
    unsigned block=(unsigned)strtoul(b,0,0), mt=(unsigned)strtoul(m,0,0);
    uint32_t count=MEM_U32(0x1001cb38), table=MEM_U32(0x1001cc30);
    unsigned hits[2]={0,0};
    if(count>4096) {fprintf(stderr,"T78 bit-limit %u\n",count);return;}
    for(uint32_t i=0;i<count;i++) {
        uint32_t ic=MEM_U32(table+i*8),lr=MEM_U32(table+i*8+4);
        if(!ic||!lr) continue;
        unsigned kind=MEM_U8(ic), which=2;
        if(kind==2 && MEM_U32(ic+16)==160) which=0;
        if(kind==3 && MEM_S32(ic+16)==off && MEM_U16(ic+20)==block && MEM_U8(ic+22)==mt) which=1;
        if(which<2) {
            hits[which]++;t78_lr[which]=lr;
            fprintf(stderr,"T78 ID which=%u bit=%u lr=%u kind=%u dtype=%u value_or_offset=%d block=%u mt=%u\n",which,MEM_U16(ic+2),lr,kind,MEM_U8(ic+1),MEM_S32(ic+16),MEM_U16(ic+20),MEM_U8(ic+22));
        }
    }
    fprintf(stderr,"T78 IDCOUNTS constant=%u page=%u saved=%u..%u\n",hits[0],hits[1],MEM_U32(0x1001e5d8),MEM_U32(0x1001e5e8));
    if(hits[0]!=1||hits[1]!=1) {t78_lr[0]=t78_lr[1]=0;return;}
    fprintf(stderr,"T78 EDGE initial 160_to_page=%d page_to_160=%d\n",t78_edge(mem,t78_lr[0],t78_lr[1]),t78_edge(mem,t78_lr[1],t78_lr[0]));
}
static void t78_snapshot(uint8_t *mem,const char *phase,uint32_t chosen,int color) {
    if(!t78_active || !t78_focus(chosen) || t78_records++>=256) return;
    fprintf(stderr,"T78 EVENT %s chosen=%u color=%d\n",phase,chosen,color);
    for(unsigned j=0;j<2;j++) {
        uint32_t p=t78_lr[j],bits=MEM_U32(p+48);float priority;memcpy(&priority,&bits,4);
        uint32_t lo=MEM_U32(0x1001e5d8),hi=MEM_U32(0x1001e5e8);
        uint32_t avail0=0,avail1=0;
        for(uint32_t k=lo;k<=hi && k<64;k++)
            if(!(MEM_U32(p+40+(k/32)*4)&(0x80000000u>>(k%32)))) {
                if(k<32)avail0|=0x80000000u>>k;else avail1|=0x80000000u>>(k%32);
            }
        fprintf(stderr,"T78 LR which=%u bit=%u priority=%.9g state=%u assigned=%d colorsleft=%u forbidden=%08x/%08x available_saved=%08x/%08x\n",j,MEM_U16(MEM_U32(p)+2),(double)priority,MEM_U8(p+35),MEM_S8(p+32),MEM_U8(p+33),MEM_U32(p+40),MEM_U32(p+44),avail0,avail1);
    }
    fprintf(stderr,"T78 EDGE current 160_to_page=%d page_to_160=%d\n",t78_edge(mem,t78_lr[0],t78_lr[1]),t78_edge(mem,t78_lr[1],t78_lr[0]));
}
static void t78_candidate(uint8_t *mem,const char *phase,uint32_t lr,uint32_t color,double cost,double best) {
    if(!t78_active||!t78_focus(lr)||t78_records++>=256) return;
    fprintf(stderr,"T78 CAND %s which=%u color=%u forbidden=%u cost=%.9g best=%.9g\n",phase,lr==t78_lr[0]?0:1,color,(MEM_U32(lr+40+(color/32)*4)&(0x80000000u>>(color%32)))!=0,cost,best);
}
'''
text=text.replace('#include "header.h"\n','#include "header.h"\n'+helper,1)
def insert(label, code):
    global text
    needle=label+':\n'
    assert text.count(needle)==1,label
    text=text.replace(needle,needle+code+'\n')
# The function entry is stable and happens before its own pool setup.
needle='static void f_globalcolor(uint8_t *mem, uint32_t sp) {\n'
assert text.count(needle)==1
text=text.replace(needle,needle+'t78_init(mem);\n')
# compute_save returns a fully evaluated priority/state without modifying it.
insert('L46f254','t78_snapshot(mem,"priority",s1,-1);')
for label in ['L471d6c','L4727d4']:
    insert(label,'t78_snapshot(mem,"choice-before",s5,(int)MEM_U32(sp+220));')
for label in ['L472180','L472b50']:
    insert(label,'t78_snapshot(mem,"neighbors-after",s5,(int)MEM_U32(sp+220));')
for label in ['L4719cc','L472480']:
    insert(label,'t78_candidate(mem,"saved-entry",s5,s2,0.0,0.0);')
for label in ['L471afc','L4725b0']:
    insert(label,'t78_candidate(mem,"saved-cost",s5,s2,(double)f2.f[0],(double)f20.f[0]);')
out=root/'uopt-alloc.c';out.write_text(text)
# Zero-context patch avoids whitespace-only context lines in archived patches.
patch=''.join(difflib.unified_diff(stock_c.read_text().splitlines(True),text.splitlines(True),fromfile='a/uopt.c',tofile='b/uopt.c',n=0))
(root/'observer.patch').write_text(patch)
binary=root/'uopt-alloc'
subprocess.run(['gcc','-std=c11','-Os','-fno-strict-aliasing','-I/scratch/recomp',str(out),'/scratch/recomp/build/5.3/libc_impl.o','/scratch/recomp/build/5.3/version_info.o','-lm','-o',str(binary)],check=True)
meta={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [stock_c,out,binary,Path('/scratch/recomp/build/5.3/libc_impl.o'),Path('/scratch/recomp/build/5.3/version_info.o')]}
(root/'generator.json').write_text(json.dumps(meta,indent=2)+'\n')
print('Private diagnostic generated; fidelity required before interpreting logs.')
