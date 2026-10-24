#!/bin/bash
# T-25608 scratch: expand the body file's macros (SCHED_DATE, unk_54C) for ugforce.py. usage: mk.sh IN OUT
sed -e 's/SCHED_DATE(\([a-z]\))/((ScheduleEntryDate *)\&g->unk_54C[\1])/g' -e 's/unk_54C/u54C.a/g' "$1" > "$2"
