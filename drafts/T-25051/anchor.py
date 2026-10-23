from harness import *
pat=r'p->y3 = ([^\n]+);\s*'+re.escape(uv)
for name,form in [('assignplus','p->y3 = (pad = {e}) + (('+uv[:-1]+'), 0);'),('assignsub','p->y3 = (pad = {e}) - (('+uv[:-1]+'), 0);'),('assignread','p->y3 = (pad = {e}) + (('+uv[:-1]+'), pad * 0);')]:
 b=re.sub(pat,lambda m:form.replace('{e}',m[1]),body)
 run('anchor_'+name,b)
