"""Generate logging-only private uopt, or audit its output; Docker only."""
from pathlib import Path
import hashlib,json,sys
r=Path('build/etc24067/creation-v3');r.mkdir(exist_ok=True)
p=Path('/scratch/variants/trace.c')
expected='59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c'
if len(sys.argv)>1:
 assert sys.argv[1:]==['--audit']
 sys.path.insert(0,'tools')
 from trailing_pad import Elf
 meta=json.loads((r/'provenance.json').read_text())
 base=Path('build/etc24067/archive/indexed')
 for key,path in [('input',base/'source.c'),('baseline_object',base/'source.o')]:
  assert hashlib.sha256(path.read_bytes()).hexdigest()==meta[key]['sha256'],key
 a,b=[Elf(path.read_bytes()) for path in [base/'source.o',r/'indexed.o']]
 for name in ['.text','.rel.text','.rodata','.rel.rodata']:
  left=a.sections[a.find(name)]['data'];right=b.sections[b.find(name)]['data']
  assert left==right,name
  print(name,'identical',len(left))
 for key,path in [('private_binary',r/'trace-create'),('diagnostic_object',r/'indexed.o')]:
  meta[key]={'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}
 meta['full_sections_identical']=['.text','.rel.text','.rodata','.rel.rodata']
 (r/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 print('Private diagnostic output verified against same-source stock compile.')
 raise SystemExit(0)
assert hashlib.sha256(p.read_bytes()).hexdigest()==expected,'unexpected generated base source'
s=p.read_text()
changes=[('#include "header.h"','#include "header.h"\nstatic unsigned event_in_lr;\nstatic unsigned event_bb;'),('L468998:\n','L468998:\nevent_in_lr = 1; event_bb = 0xffffffff;\n'),('L468ad8:\n','L468ad8:\nevent_bb = MEM_U16(MEM_U32(sp + 308) + 8);\n'),('L46abac:\n','L46abac:\nevent_in_lr = 0;\n'),('L4867cc:\n','''L4867cc:
if (event_in_lr) {
    uint32_t event_chain = MEM_U32(sp + 32);
    fprintf(stderr, "CREATE_BIT proc=");
    for (unsigned event_i = 0; event_i < MEM_U32(0x1001c8d0) && event_i < 1024; event_i++)
        fputc(MEM_U8(0x1001c4d0 + event_i), stderr);
    fprintf(stderr, " bit=%u bb=%u kind=%u addend16=%08x size20=%08x locaddr24=%08x block28=%u mtype30=%u rawloc28=%08x\\n",
            a1, event_bb, MEM_U8(event_chain), MEM_U32(event_chain + 16),
            MEM_U32(event_chain + 20), MEM_U32(event_chain + 24),
            MEM_U16(event_chain + 28), MEM_U8(event_chain + 30), MEM_U32(event_chain + 28));
}
''')]
for old,new in changes:
 assert s.count(old)==1,old
 s=s.replace(old,new)
(r/'trace-create.c').write_text(s)
meta={'base_source':str(p),'base_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'private_source_sha256':hashlib.sha256(s.encode()).hexdigest(),'purpose':'newbit logging only during makelivranges; native diagnostic context variables never used by emulated allocator'}
base=Path('build/etc24067/archive/indexed')
for key,path in [('input',base/'source.c'),('baseline_object',base/'source.o')]:
 meta[key]={'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}
(r/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
print('Generated private logging source. Build and audit it before using any event evidence.')
