---
id: T-39303
title: Install one disc pack of the original files the game reads (M2)
status: In Progress
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[disc-layout]]"]
---

## Goal

`setup_content` installs one pack of the original files the game reads (path, LBA, size, SHA-256) from the user's disc, and the engine's CD reads use it. This is layer 4 of [[port-completion-plan]].

## Acceptance criteria

- [ ] The pack is built from the user's disc only, verified by hashes, and never committed.
- [ ] `cd_read_sectors` and related engine calls read from it by LBA.
- [ ] Existing content roles keep working.

## Comments

2026-10-30 (worker, `port/m2-disc-pack`): built the pack exporter and the reader.
- `tools/port/disc_pack_content.py` walks the ISO9660 tree of the raw track and stores every file whose sectors are all mode 2 Form 1 as whole 2048-byte sectors (`files/<path>`), with LBA, sector count, ISO size, mode/form, SHA-256 of the stored bytes and of the first `size` bytes, in `disc_pack.json` (`tokimemo.disc-pack.v1`). The 17 files with Form 2 sectors (XA audio, STR video) or no data in the image are listed under `excluded` with the reason and have no data. Real disc: 1355 files, 41,727 sectors, 85,456,896 bytes, 17 excluded; every `content_sha256` equals the file already extracted to `disc/files`.
- `ASAHINA.XA` has one sector (LBA 67338) with unequal subheader copies; as a Form 2 stream it is excluded, and a Form 1 data file with such a sector would fail the export.
- New optional dir role `disc_pack` (`packs/disc_pack`, role manifest `disc_pack.json`) in `build_installed_runtime.py` and `installed_launch_profile.gd`, like `wing_art`: never required, a failure is only a gap. The pack adds 1356 files to the inventory and 85 MB, within `MAX_FILES` and `MAX_TOTAL_BYTES`.
- `port/godot/services/disc_pack.gd`: `open(root, expected_manifest_sha256)`, `open_installed(profile)`, `lookup_path`, `lookup_lba`, `excluded_at`, `read_sectors(lba, count)`, `read_into_memory(memory, address, lba, count)` (cd_read_sectors), `read_file`, `verify_all`. A read must lie in one packed file: gaps, a run past the end of a file (even into an adjacent file), an XA/Form 2 extent, a bad hash, a wrong size, a missing file, a count outside 1..4096 and a destination outside RAM each return `{"ok": false, "error": ...}` with no bytes and no memory write. A file is read whole and hashed before any byte is returned (one verified file is kept).
- Checks on the real pack (Docker, `tests/core/check_real_disc_pack.gd`, not in the suite): O_11.NBN at 0xAC1E, 33 sectors, SHA `3e901c18...0628` and equal to `first_arrival_board/o_11.nbn`; CUT_08.NBN at 0xAA85, 49 sectors, equal to the board role's `cut_07.nbn` (that role names it by the slot after it); both read into memory at 0x801E8000 and 0x801A4000; an XA read and out-of-extent reads refuse. A copy of installed-runtime-v14 with the pack added as a role passes `InstalledLaunchProfile.read_pack`.
- Not done: no engine call uses the reader yet (T-39301 owns `cd_read_sectors`); `setup_content.py` gets the pack through `build_installed_runtime.py` with no extra step; the Form 2 streams are not packed.

2026-10-30 gates after the merge of `port/opening-to-gameplay` (`e62e31f8a`): core suite PASS, 10 suites; opening frame timing `pad 2, 104 substeps, 6709 frames to 12/0/0, 0 mismatches`, exit 0; April loop route compared with `merge-pending-2026-10-30/loop-route-fast`: IDENTICAL.

2026-10-30 review fixes (worker):
- Reads across files: `DiscPack.read_sectors` now allows a read that runs through packed extents with no gap (every touched file is verified, bytes concatenated); a gap, an excluded extent or the end of the pack still fails with the reason. This is needed by `date_scene_state.c:606,864,1239` and `date/rides.c:273,522,1004` (10 sectors at the 9-sector `D_11_11.BS` 0x4359, `D_11_10.BS` 0x4350, `D_11_08.BS` 0x433E). All three are read on the real pack, plain and into memory, equal to file + first sector of the next file.
- `tools/port/check_literal_disc_reads.py --pack <dir>` lists every literal `cd_read_sectors(_nowait)(count, dst, lba)` in `src/` against the manifest: 879 in one file, 6 across adjacent files (the six above), 0 not covered. **Computed-LBA reads (28 calls with a non-literal count or LBA: `dec_bg_cd_read`, `bg_read_sub2`, table-driven reads such as `tour_invite_script_lba`) are not yet covered**; they need the engine layer (T-39301) to check at run time.
- Launch cost: `read_pack` best of 3 (warm cache, 4 CPUs, `tests/core/time_profile_open.gd`): v14 without the pack 379 ms (334 ms on a later run), with the pack hashed whole 764 ms (+385 ms). Now the pack's manifest is hashed and its other files are size-checked only (DiscPack hashes each file on read): 415 ms with the pack. The pack has its own caps (4096 files, 256 MiB) outside `MAX_FILES`/`MAX_TOTAL_BYTES`; over a cap `disc_pack` is dropped and recorded in `optional_role_gaps`, the profile stays valid. Only the locator `packs/disc_pack` gets these rules, so no other role skips hashing; required-role checks are unchanged.
- Cache: DiscPack keeps verified files in a FIFO cache of up to 8 MiB instead of one file.
- Exporter: Form 1 sectors flagged audio/video (0x04/0x02) are streams; multi-extent (0x80) files fail the export; a record repeated with the same path and extent is one file, a path with two extents fails. The real pack is unchanged (manifest byte-identical, `pack-v2`). The manifest is hashed and parsed from the same bytes.

