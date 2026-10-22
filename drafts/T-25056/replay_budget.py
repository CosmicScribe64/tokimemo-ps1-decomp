#!/usr/bin/env python3
"""Replay captured ENDING input with scratch budget controls; Docker only."""
from pathlib import Path
import os,subprocess,shutil,sys
sys.path.insert(0,'tools')
import funcdiff
base=Path('build/t25056/base')
out=Path('build/t25056/budget')
for label,value in [('default',None),('raised','100')]:
    shutil.copy(base/'in.T',out/(label+'.T'))
    env=os.environ.copy()
    if value:env['T25056_BUDGET']=value
    with (out/(label+'.log')).open('w') as log:
        subprocess.run([str(out/'as1-budget'),'-elf','-G','0','-p0','-EL','-g0','-O2',str(base/'in.G'),'-o',str(out/(label+'.o')),'-t',str(out/(label+'.T'))],env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
    funcs=funcdiff.resolved_functions(out/(label+'.o'),['func_8013A2E8'])
    reference=funcdiff.resolved_functions(base/'unit.o',['func_8013A2E8'])
    print(label,'same_as_stock',funcdiff.keys(funcs['func_8013A2E8'])==funcdiff.keys(reference['func_8013A2E8']),flush=True)
    if label=='raised':
        subprocess.run(['python3','tools/funcdiff.py','--unit','ENDING','--built',str(out/(label+'.o')),'--resolve','func_8013A2E8'])
