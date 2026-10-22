"""Privately link a C candidate against read-only verified MAIN dependencies.

Run in Docker with the integration tree mounted read-only at /baseline.
All outputs remain in build/t25040; no baseline objects are modified or copied.
"""
import hashlib
import json
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as fv
from uopt_link_eval import image_functions

label, source, version = sys.argv[1:]
name = "func_8006D138"
c = fd.locate(name, "main")
directory = Path("build/t25040") / label
directory.mkdir(parents=True, exist_ok=True)
obj, _ = fv.compile_unit(c, directory, source=source, non_matching=version == "guarded")
stem = Path("/baseline/build/SLPM_86.053")
script = Path(str(stem) + ".ld").read_text().replace("build/", "/baseline/build/")
anchor = "/baseline/" + c.obj + "("
assert anchor in script
script = script.replace(anchor, '"' + str(Path(obj).resolve()) + '"(')
ld = directory / "unit.ld"
ld.write_text(script)
elf = directory / "unit.elf"
cmd = ["mips-linux-gnu-ld", "-EL", "-T", str(ld)]
for item in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt", "SLPM_86.053_data_syms.ld"):
    cmd += ["-T", "/baseline/build/" + item]
subprocess.run(cmd + ["-o", str(elf)], check=True)
address, target, padding = fv.target_span(c, name)
base, data, symbols = image_functions(elf, ".main", directory / "unit.section")
actual_address, actual_size = symbols[name]
got = data[address-base:address-base+len(target)]
assert actual_address == address
assert actual_size + padding == len(target)
assert got == target, "literal function bytes differ"
binary = directory / "unit.bin"
subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", str(elf), str(binary)], check=True)
sha1 = hashlib.sha1(binary.read_bytes()).hexdigest()
expected = Path("config/SLPM_86.053.sha1").read_text().split()[0]
assert sha1 == expected, (sha1, expected)
result = dict(version=version, function=name, address=hex(address), literal_bytes=len(target),
              function_match=True, executable_sha1=sha1, executable_match=True)
(directory / "proof.json").write_text(json.dumps(result, indent=2) + "\n")
print(json.dumps(result))
