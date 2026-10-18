"""Check the recovered table sizes and selector mapping against local immutable assembly."""
from pathlib import Path
import re

body = Path("drafts/T-24051/func_80144E20.c").read_text()
asm = Path("asm/ovl/DATE/nonmatchings/DATE/80141EE0/func_80144E20.s").read_text()
tables = [(name, int(count), int(name[2:], 16)) for count, name in re.findall(
    r"FnTbl(\d+) (sp[0-9A-F]+);", body)]
assert len(tables) == 21
for current, following in zip(tables, tables[1:]):
    assert following[2] + following[1] * 4 == current[2]
assert tables[0][2] + tables[0][1] * 4 == 0xE34
assert tables[-1][2] == 0x34
copies = re.findall(r"    (sp[0-9A-F]+) = D_([0-9A-F]+);", body)
assert [name for name, address in copies] == [name for name, count, offset in tables]
for index in range(20):
    assert int(copies[index][1], 16) + tables[index][1] * 4 == int(copies[index+1][1], 16)
cases = re.findall(r"    case (\d+):\n        base = (sp[0-9A-F]+)\.f;", body)
assert [(int(index), name) for index, name in cases] == list(enumerate(name for name, count, offset in tables))
registers = dict((reg, int(offset, 16)) for reg, offset in re.findall(
    r"addiu\s+\$(\w+), \$sp, (0x[0-9A-Fa-f]+)", asm.split(".L80144E68")[0]))
labels = re.findall(r"\.word (\.L\w+)", asm[asm.index("dlabel jtbl_8015B0B0"):])
assert len(labels) == 21
for index, label in enumerate(labels):
    section = asm.split("jlabel " + label + "\n", 1)[1].split("jlabel", 1)[0]
    move = re.search(r"or\s+\$v0, \$(\w+), \$zero", section)
    direct = re.search(r"addiu\s+\$v0, \$sp, (0x[0-9A-Fa-f]+)", section)
    offset = registers[move[1]] if move else int(direct[1], 16)
    assert offset == tables[index][2], (index, offset, tables[index])
print("21 contiguous source tables: 0xE00 bytes; stack interval +0x34..+0xE34.")
print("All 21 selector entries and copy-source extents agree with the target.")
print("Case 0 and case 20 are distinct target branch arms; no mapping repair found.")
