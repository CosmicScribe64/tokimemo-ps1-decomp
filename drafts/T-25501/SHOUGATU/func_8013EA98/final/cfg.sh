#!/bin/sh
# cfg.sh LABEL: uopt flow graph of func_8013EA98 (node(freq-label): >successors)
awk '/flow graph for func_8013EA98/{f=1} f&&/SECONDS/{exit} f' build/ea98/$1/trace/draft.o.trace | awk '/^ +[0-9]+ +[0-9]+$/{printf "\n%s(L%s):", $1, $2; next} /suc::::/{printf " >%s", $2} END{print ""}' | tr '\n' ' ' | sed 's/  */ /g'; echo
