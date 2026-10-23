#!/bin/sh
# Scratch (T-24929), inside docker: resolved diff rows of every guarded function, with CVT_ZCOPY_SCOPE=$1.
V=$1
cat build/t24929/guarded.txt | xargs -P 4 -I{} sh -c 'u=${1%%:*}; f=${1#*:}; n=$(CVT_ZCOPY_SCOPE='"$V"' python3 tools/funcdiff.py --resolve --version c --no-build --unit $u $f 2>&1 | grep -c "^  [-+]"); echo "$1 $n"' _ {} | sort > build/t24929/guards_$V.txt
