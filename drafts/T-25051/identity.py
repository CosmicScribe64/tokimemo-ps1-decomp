from harness import *
pattern=r'p->y3 = ([^\n]+);'
for name,wrap in [('short',lambda e:'(s16)('+e+')'),('mask',lambda e:'('+e+') & 0xFFFF'),('bytepair',lambda e:'(s16)(u16)('+e+')')]:
 b=re.sub(pattern,lambda m:'p->y3 = '+wrap(m.group(1))+';',body);run('y_'+name,b)
for n in [1,2,10]:
 def wrapped(m):
  e='('+m.group(1)+')'
  for k in range(n): e='('+e+(' | ' if k%2==0 else ' + ')+'pad)'
  return 'pad = (u32)arg0 < 0;\n    p->y3 = '+e+';'
 b=re.sub(pattern,wrapped,body);run('y_late'+str(n),b)
