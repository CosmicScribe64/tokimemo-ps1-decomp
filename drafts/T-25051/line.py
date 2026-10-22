from harness import *
for name,form in [('eq','p->y3\n = {e};'),('rhs','p->y3 =\n {e};'),('semi','p->y3 = {e}\n;'),('parts','p->y3 =\n {e}\n;')]:
 run('line_'+name,re.sub(r'p->y3 = ([^\n]+);',lambda m:form.replace('{e}',m[1].replace(' + ',' +\n ') if name=='parts' else m[1]),body))
