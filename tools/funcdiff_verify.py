"""Literal-byte proof for funcdiff (T-10840); normalized relocations are diagnostics only.

Link a frozen object in a private copy of the unit's normal linker script and
compare its function address, size and bytes to the immutable target binary.
No production objects or scripts are replaced. Missing proof fails closed.
"""
from pathlib import Path
import re
import subprocess

import srcscan
import trailing_pad
from uopt_link_eval import image_functions


def command(args):
    result = subprocess.run(args, capture_output=True, text=True)
    if result.returncode:
        raise ValueError("%s failed:\n%s" % (args[0], "\n".join((result.stderr or result.stdout).splitlines()[-12:])))
    return result.stdout


def guarded(source, name):
    """A C definition and an asm fallback coexist under NON_MATCHING."""
    text = Path(source).read_text()
    return ("NON_MATCHING" in text and name in srcscan.defined_functions(Path(source))
            and re.search(r'INCLUDE_ASM\([^;]*\b%s\b' % re.escape(name), text) is not None)


def compile_guarded(c, directory):
    """Preprocess the complete TU, retaining data and active asm includes."""
    import cc
    import object_layout
    source = directory / "guarded.c"
    pre = preprocess_draft(c.src)
    source.write_text(pre)
    obj = directory / "guarded.o"
    command(["python3", "tools/cc.py", str(source), str(obj), "ido", "5.3"])
    includes = {name for _, name in cc.INCLUDE_ASM_RE.findall(pre)}
    trailing_pad.pad_functions(str(obj), str(c.src), includes)
    obj.write_bytes(object_layout.trim(obj.read_bytes(), object_layout.source_extent(c.src)))
    return str(obj)


def preprocess_draft(source):
    """Keep full-file data, helper functions and active asm while selecting guarded C."""
    return command(["gcc", "-E", "-P", "-Iinclude", "-D__sgi", "-DNON_MATCHING",
                   "-D__attribute__(x)=", "-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)",
                   "-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)", str(source)])


def target_span(c, name):
    """Address and immutable target bytes covering the original function, including nops."""
    asm = trailing_pad.find_asm(c.src, name)
    if asm is None:
        raise ValueError("no original asm span for " + name)
    rows = re.findall(r'/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/',
                      Path(asm).read_text().split(trailing_pad.RODATA_MARK)[0])
    if not rows:
        raise ValueError("no original instruction offsets for " + name)
    offset, address = (int(x, 16) for x in rows[0])
    if any(int(o, 16) != offset + 4 * i or int(a, 16) != address + 4 * i
           for i, (o, a) in enumerate(rows)):
        raise ValueError("noncontiguous original function span for " + name)
    cfg = srcscan._load_yaml("config/SLPM_86.053.yaml" if c.unit == "main"
                             else "config/overlays/%s.yaml" % c.unit)
    data = Path(cfg["options"]["target_path"]).read_bytes()[offset:offset + 4 * len(rows)]
    if len(data) != 4 * len(rows):
        raise ValueError("target function span is truncated")
    return address, data, 4 * trailing_pad.trailing_words(asm)


def exact_match(obj, c, name, directory, build=True, is_c=True):
    """(equal, explanation) after real linking; function size/address are part of the proof."""
    address, want, padding = target_span(c, name)
    size = trailing_pad.text_functions(obj).get(name, (None, None))[1]
    # ASM symbols include their trailing words; C symbols exclude the pad pass's words.
    length = size + padding if size is not None and is_c else size
    if is_c is None and size is not None and size != len(want):
        length = size + padding
    if length != len(want):
        return False, "function span differs: built %s bytes, target %d" % (length, len(want))
    stem = "build/SLPM_86.053" if c.unit == "main" else "build/ovl/" + c.unit
    if build:
        command(["ninja", stem + ".elf"])
    script = Path(stem + ".ld").read_text()
    anchor = c.obj + "("
    if anchor not in script:
        raise ValueError("object absent from unit linker script: " + c.obj)
    script = script.replace(anchor, '"' + str(Path(obj).resolve()) + '"(')
    local = directory / "unit.ld"
    local.write_text(script)
    extra = (["build/undefined_funcs_auto.txt", "build/undefined_syms_auto.txt",
              stem + "_data_syms.ld"] if c.unit == "main" else
             [stem + s for s in ("_undefined_funcs_auto.txt", "_undefined_syms_auto.txt", "_data_syms.ld")]
             + ["build/main_names.ld"])
    elf = directory / "unit.elf"
    argv = ["mips-linux-gnu-ld", "-EL", "-T", str(local)]
    for path in extra:
        argv += ["-T", path]
    command(argv + ["-o", str(elf)])
    base, data, symbols = image_functions(elf, ".main" if c.unit == "main" else "." + c.unit,
                                         directory / "unit.section")
    actual_address, actual_size = symbols.get(name, (None, None))
    if (actual_address, actual_size) != (address, size):
        return False, "linked function address or size differs from target"
    got = data[address - base:address - base + len(want)]
    return got == want, "linked function bytes %s target (%d bytes)" % (
        "equal" if got == want else "differ from", len(want))
