#!/bin/sh
# show.sh label [from to]: instruction lines of draft function (index from 0)
awk '/<func_8013EA98>:/{f=1;next} f&&/^$/{exit} f&&/^ *[0-9a-f]+:\t/{print}' build/ea98/$1/dis.txt | cut -f3- | awk -v a=${2:-0} -v b=${3:-999} '{if(NR-1>=a && NR-1<=b) printf "%3d %s\n", NR-1, $0}'
