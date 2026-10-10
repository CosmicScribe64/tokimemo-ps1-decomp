# Your copy of the game

Put your copy of Tokimeki Memorial, Forever with You (Japan), PlayStation the Best release, here. Git ignores
everything in this folder except this file, so the game never reaches the repository.

Any common form works, in any subfolder:

- BIN + CUE, with one `.bin` or several (for example `Track 1` and `Track 2`) and their `.cue`.
- CHD, MAME's compressed disc format.
- ISO of the data track only (2048-byte sectors). The build needs only the files on the disc, so nothing is lost.
- A `.zip` or `.7z` holding any of those.

Then run `tools/docker.sh python3 configure.py` and `tools/docker.sh ninja`. The build finds the image,
checks which release it is, and unpacks it into `disc/` (also ignored). It only unpacks again when the image changes.

The build needs the release whose executable `SLPM_86.053` has the SHA-1 in `config/SLPM_86.053.sha1`
(PlayStation the Best). Other releases (Rev 1, Rev 2, Rev 4, Shokai Genteiban) are recognised and refused with a
message that names them; see `wiki/versions.md`.

Only this folder is searched. A `disc/` you extracted yourself also still works.
