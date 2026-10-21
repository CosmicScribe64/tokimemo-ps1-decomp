#!/bin/sh
# Scratch (T-24929), run on the host from the worktree root: rebuild every object with
# CVT_ZCOPY_SCOPE=$1 (in docker), snapshot the objects, list the functions that differ from base.
V=$1
touch tools/cc.py
rm -f build/t24929/rprop_$V.log; tools/docker.sh sh -c "CVT_RPROP_LOG=build/t24929/rprop_$V.log CVT_ZCOPY_SCOPE=$V ninja -j4 -k 0" >build/t24929/ninja_$V.log 2>&1
rm -rf build/t24929/$V && mkdir -p build/t24929/$V
rsync -a --include '*/' --include '*.o' --exclude 't24929/' --exclude 'asm/' --exclude 'scratch/' \
  --exclude 'expected/' --exclude '*' --prune-empty-dirs build/src build/ovl build/t24929/$V/
tools/docker.sh python3 drafts/T-24929/objcmp.py build/t24929/base build/t24929/$V > build/t24929/changed_$V.txt
grep -c ": OK" build/t24929/ninja_$V.log; grep "FAILED" build/t24929/ninja_$V.log | head -5
