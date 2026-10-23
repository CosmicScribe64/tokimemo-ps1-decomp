#!/bin/sh
# usage (docker, repo root): sh drafts/T-25116/main/func_800476C0/snippets/snip.sh a.c [b.c ...]: compile each snippet, print its disassembly
for f in "$@"; do
  echo "=== $f"
  python3 tools/cc.py "$f" "${f%.c}.o" ido 5.3 >/dev/null 2>"${f%.c}.err" || { tail -5 "${f%.c}.err"; continue; }
  mips-linux-gnu-objdump -d --no-show-raw-insn "${f%.c}.o" | grep -E "^\s+[0-9a-f]+:" | sed -E 's/^\s+[0-9a-f]+:\s+//'
done
