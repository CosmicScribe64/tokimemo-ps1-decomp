"""Replay T-24515's bounded source controls serially inside project Docker.

Run from repository root after configure/build. No production files are changed.
Outputs contain diagnostics only; no candidate is asserted to match.
"""
from pathlib import Path
import subprocess
import argparse
import sys

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--saved',action='store_true',help='only check the two annotated saved bodies')
args=parser.parse_args()
if args.saved:
    sys.path.insert(0,str(Path(__file__).parent/'replay'))
    from probe import probe
    for name,rows in [('func_80138C1C',104),('func_801398EC',173)]:
        source=(Path(__file__).parent/(name+'.c')).read_text()
        assert probe(name,'preserved-snapshot',source)==rows,name
else:
    scripts=['probe','captures','loop_repair','rolled','date_repair','char_repair',
             'final_small','indexed','return_order','remainder','large_control']
    for name in scripts:
        subprocess.run(['python3',str(Path(__file__).parent/'replay'/(name+'.py'))],check=True)
