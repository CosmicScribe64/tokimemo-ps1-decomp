"""Restore the ordinary Unicode label represented by a mixed m2c escape."""
from pathlib import Path
p=Path('src/ovl/BUNKAKEN/80144F70.c')
assert p.exists()
s=p.read_text();bad=r'パ\x83\\コン占い';assert bad in s;s=s.replace(bad,'パソコン占い');p.write_text(s)
