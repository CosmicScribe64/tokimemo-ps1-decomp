#!/bin/sh
# T-25604 scratch: print the case 9 tail stores (with relocations) of compiled objects. usage: c9.sh OBJ...
for o in "$@"; do
  echo "== $o"
  mips-linux-gnu-objdump -dr --disassemble=func_80143580 "$o" | awk '
    /R_MIPS_LO16/ {split($0,a,"R_MIPS_LO16"); gsub(/[ \t]/,"",a[2]); if (last!="") {print last" "a[2]; last=""}; next}
    /^ +[0-9a-f]+:\t/ {split($0,f,"\t"); ins=f[3]" "f[4]; if (ins ~ /^(sw|sb|lbu) /) last=ins; else last=""; if (ins ~ /^lui (t[0-9]|v0|v1|a[0-3]|ra),0x8000$/) print ins; if (ins ~ /^li a0,255|^jal/) print ins}' | sed -n '/li a0,255/,$p' | awk 'NR<=40' | sed -n '1,/li a0,255/p;/li a0,255/,$p' | head -60 | paste -sd';' - | sed 's/;jal/\njal/g' | awk 'length($0)>200' | head -2
done
