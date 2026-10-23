#!/bin/sh
# pri.sh label: per live range: bit lr priority, unit blocks (L=loads S=stores r=reload s=save)
l=build/ea98/$1/trace/draft.log
awk '/R9PROC func_8013EA98/{f=1;next} f&&/R9PROC/{exit} f' $l | awk '
/R9SAVE/{if(h!="")print h" |"u; split($0,a," "); h=a[2]" "a[3]" "a[5]; u=""; next}
/R9UNIT/{split($0,b," "); bb=b[2]; sub("bb=","",bb); ld=b[4]; sub("loads=","",ld); st=b[5]; sub("stores=","",st); r=b[6]; sub("load=","",r); s=b[7]; sub("save=","",s);
 u=u" "bb (ld>0?"L"ld:"") (st>0?"S"st:"") (r==1?"r":"") (s==1?"s":"")}
END{print h" |"u}'
