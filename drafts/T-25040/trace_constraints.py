"""Add read-only constraint-count diagnostics to the existing scratch tracer."""
from pathlib import Path
source = Path("/diagnostic/variants/trace.c").read_text()
needle = "L46ec74:\n"
assert source.count(needle) == 1
source = source.replace(needle, needle + '''{
    uint32_t lr = MEM_U32(sp + 32);
    fprintf(stderr, "T25040CONSTRAINT bit=%u lr=%u count=%u constrained=%u\\n",
        MEM_U16(MEM_U32(lr) + 2), MEM_U32(lr + 4), MEM_U32(lr + 36), v1);
}
''')
Path("build/t25040/trace-constraints.c").write_text(source)
