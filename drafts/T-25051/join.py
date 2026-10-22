from harness import *
for name,cond in [('field','p->y3'),('false','(u32)p->y3 < 0'),('arg','arg0')]:
 k=[0]
 def wrap(m):
  k[0]+=1;label='uvjoin'+str(k[0]);return 'if ('+cond+') goto '+label+';\n    '+label+':\n    '+uv
 run('join_'+name,re.sub(re.escape(uv),wrap,body))
k=[0]
def wrap(m):
 k[0]+=1;label='uvjoin'+str(k[0]);return 'if (p->y3) goto '+label+'; else goto '+label+';\n    '+label+':\n    '+uv
run('join_both',re.sub(re.escape(uv),wrap,body))
