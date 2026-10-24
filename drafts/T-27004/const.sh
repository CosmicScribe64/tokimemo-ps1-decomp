#!/bin/bash
# usage: const.sh ENUM [--apply] SITE...   (header include/game_enums.h; --defines for PadBits, MouseButtons, PlayerFlag)
E=$1; shift
A=""; if [ "$1" = "--apply" ]; then A="--apply"; shift; fi
S=""; for s in "$@"; do S="$S --site $s"; done
M="--enum $E"; case $E in PadBits|MouseButtons|PlayerFlag) M="--defines";; esac
cd /Users/lila/Documents/Projects/tokimemo-wt/p2-core-apply
nice -n 19 tools/docker.sh python3 tools/rename.py const --header include/game_enums.h $M --values drafts/T-27004/values_$E.txt $S --note "T-27004 core.tsv $E" --ledger core $A 2>&1 | cut -c1-230
