# BUNKASAI label reconstruction

Preserved T-15400 full translation-unit alternatives from integration before promotion. `canonical/` changes only the GameState view and remains two diagnostic rows away. `utf8/` also restores the ordinary Unicode label and became the default-build matched function at e32e9552. Both have body, complete candidate C and resolved diff. The original census attempt remains immutable under drafts/T-14500/BUNKASAI/func_801431D4.

`reproduce.py` runs in the project Docker image from the repository root and requires the pre-promotion production source (28e09dd9). It writes ignored scratch results. No compiled binaries or generated assembly are retained here.
