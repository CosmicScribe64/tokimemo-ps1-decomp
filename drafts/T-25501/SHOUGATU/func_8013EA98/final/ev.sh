#!/bin/sh
# ev.sh LABEL...: score build/ea98/LABEL.c (whole TU) and print the colouring order (2 at a time)
cd "$(git rev-parse --show-toplevel)"
D=drafts/T-25501/SHOUGATU/func_8013EA98/final
for L in "$@"; do echo $L; done | xargs -P2 -I{} tools/docker.sh sh -c "python3 build/ea98/m.py {} build/ea98/{}.c 2>&1 | tail -1 > build/ea98/{}.score; python3 build/ea98/tr.py {} >/dev/null 2>&1; python3 $D/order.py {} > build/ea98/{}.order 2>&1"
for L in "$@"; do cat build/ea98/$L.score; [ -n "$ORDER" ] && cat build/ea98/$L.order; done
