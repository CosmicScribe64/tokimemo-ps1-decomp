from harness import *
pat=r'p->y3 = ([^\n]+);\s*'+re.escape(uv)
for name,form in [('lhs','*(('+uv[:-1]+'), &p->y3) = {e};'),('lhs_ptr','((('+uv[:-1]+'), p))->y3 = {e};'),('sub','p->y3 = ({e}) - (('+uv[:-1]+'), 0);'),('div','p->y3 = ({e}) / (('+uv[:-1]+'), 1);')]:
 b=re.sub(pat,lambda m:form.replace('{e}',m[1]),body)
 assert b!=body
 run('lv_'+name,b)
