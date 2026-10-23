# units.py LOG (T-25403 diagnostic; LOG = the func_80142C9C part of a live-variant draft.log): print first R9SAVE (lr==bit) record of each 'new temporary' bit with a unit at bb 0 and a unit in 150..215
import re, sys
recs = []; cur = None
for l in open(sys.argv[1]):
    m = re.match(r'R9SAVE bit=(\d+) lr=(\d+) blocks=(\d+) priority=(\S+) state=(\d+)', l)
    if m:
        cur = dict(bit=int(m.group(1)), lr=int(m.group(2)), pr=m.group(4), st=m.group(5), units=[]); recs.append(cur); continue
    m = re.match(r'R9UNIT bb=(\d+) freq=(\d+) loads=(\d+) stores=(\d+) load=(\d+)', l)
    if m and cur is not None:
        cur['units'].append((int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(5))))
seen = set()
for r in recs:
    bbs = [u[0] for u in r['units']]
    if r['bit'] > 480 and r['bit'] == r['lr'] and r['bit'] not in seen and 0 in bbs and any(150 <= b <= 215 for b in bbs):
        seen.add(r['bit'])
        print("bit", r['bit'], "prio", r['pr'], "units", " ".join("%d%s" % (u[0], "L" if u[3] else "") + ("x%d" % u[2] if u[2] != 1 else "") for u in r['units']))
for r in recs:
    if r['bit'] in seen:
        print("  lr", r['lr'], "prio", r['pr'], "st", r['st'], " ".join(str(u[0]) + ("L" if u[3] else "") for u in r['units']))
