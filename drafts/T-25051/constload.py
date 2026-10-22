from harness import *
pat=r'p->y3 = ([^\n]+);\s*'+re.escape(uv)
for name,modify in [('g',lambda e:e.replace('g->unk_018','((const GameState *)g)->unk_018')),('mode',lambda e:e.replace('[D_8011ECA0]','[*(const s32 *)&D_8011ECA0]')),('both',lambda e:e.replace('g->unk_018','((const GameState *)g)->unk_018').replace('[D_8011ECA0]','[*(const s32 *)&D_8011ECA0]'))]:
 run('const_'+name,re.sub(pat,lambda m:uv+'\n    p->y3 = '+modify(m[1])+';',body))
