#!/bin/sh
# units.sh label : trace and print flow graph + F5F unit lists
cd "$(git rev-parse --show-toplevel)"
tools/docker.sh python3 build/ea98/tr.py $1 >/dev/null 2>&1 || { echo trace failed; exit 1; }
t=build/ea98/$1/trace/draft.o.trace; l=build/ea98/$1/trace/draft.log
awk '/flow graph for func_8013EA98/{f=1;next} f&&/SECONDS/{exit} f' $t | awk '/^ *[0-9]+ +[0-9]+$/{if(n!="")printf "%s:%s  ", n, s; n=$1; s=""; next} /^suc/{s=s">"$2} END{print n":"s}'
bit=$(awk '/OPTIMIZATION OF func_8013EA98/{f=1} f&&/isvar   S    7  3935/{sub(/.*}/,""); print $1; exit}' $t)
echo "F5F bit=$bit"
awk -v b="bit=$bit " '/R9PROC func_8013EA98/{f=1} f&&/R9PROC/&&!/func_8013EA98/{exit} f' $l | awk -v b="bit=$bit " 'index($0,"R9SAVE "b)==1{p=1;print;next} /R9SAVE|R9COST/{p=0} p'
grep -A3 "OPTIMIZATION OF func_8013EA98" $t >/dev/null
awk '/OPTIMIZATION OF func_8013EA98/{f=1} f&&/REEMISSION/{exit} f&&/live range|assigned|not colored/' $t | grep -E "^ *$bit:|live range *$bit:" 
