"""Check the port's mode tables against the matched C source (school-events lane).

Source facts:
  include/game_enums.h            GAME_MODE_* and DAY_EVENT_* values
  src/main/overlay_loader.c       ovl_load_start sector switch (mode -> LBA)
  src/main/scheduler.c            day_event_dispatch switch (event -> mode)
Port tables:
  port/godot/rules/overlay_load_table.gd   OVERLAY_BY_NATIVE_MODE, DEFAULT_OVERLAY,
                                           DISPATCHER_MODE_BY_OVERLAY
  port/godot/rules/day_event_modes.gd      EVENT_MODES, EVENT_TO_DISPATCHER_MODE,
                                           NATIVE_TO_DISPATCHER_MODE
  port/godot/app/overlay_dispatcher.gd     OVERLAY_CATALOG overlay names

The disc check (LBA -> EXEDIR file name) is a separate command, because it
needs the original disc image:
  python -m tools.port.test_overlay_mode_tables --disc track1.bin --files disc/files
It exits non-zero when an input is missing or any LBA does not hold the file.
"""
from __future__ import annotations

import argparse
import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ENUMS = ROOT / "include" / "game_enums.h"
LOADER = ROOT / "src" / "main" / "overlay_loader.c"
SCHEDULER = ROOT / "src" / "main" / "scheduler.c"
OVERLAY_TABLE = ROOT / "port" / "godot" / "rules" / "overlay_load_table.gd"
EVENT_TABLE = ROOT / "port" / "godot" / "rules" / "day_event_modes.gd"
DISPATCHER = ROOT / "port" / "godot" / "app" / "overlay_dispatcher.gd"

# Overlays whose catalog row this lane owns; their event and native rows must
# resolve through the loader table.
FAMILY_OVERLAYS = frozenset({"GEKO", "SHUGAKU", "SHOUGATU", "KANGEI", "TAIIKU",
                             "BUNKASAI", "BUNKAKEN", "BUNKA_SD", "MASTER", "RENSYU"})
# Native numbers that are also catalog keys of a different overlay. The native
# table keeps the catalog meaning for them (see day_event_modes.gd).
CATALOG_COLLISIONS = frozenset({0x51, 0x52, 0x53, 0x57, 0x59, 0x5A, 0x5C})


def enum_values(prefix: str) -> dict[str, int]:
    text = ENUMS.read_text(encoding="utf-8")
    values = {}
    for name, value in re.findall(r"\b(%s[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)" % prefix, text):
        values[name] = int(value, 0)
    if not values:
        raise AssertionError("no %s values in %s" % (prefix, ENUMS))
    return values


def _case_value(token: str, modes: dict[str, int]) -> int:
    token = token.strip()
    if token in modes:
        return modes[token]
    return int(token, 0)


def loader_sector_switch() -> tuple[dict[int, int], int]:
    """ovl_load_start's second switch: native mode -> LBA, plus the default LBA."""
    text = LOADER.read_text(encoding="utf-8")
    body = text[text.index("s32 ovl_load_start(void)"):]
    body = body[body.index("switch (g->ovl_target_game_mode)", body.index("sprite_quads[0].flags")):]
    body = body[:body.index("if (resident_lba == ovl_load_lba)")]
    modes = enum_values("GAME_MODE_")
    table: dict[int, int] = {}
    pending: list[int] = []
    default_lba = None
    in_default = False
    for line in body.splitlines():
        line = line.strip()
        case = re.match(r"case\s+([A-Za-z0-9_]+):", line)
        if case:
            pending.append(_case_value(case.group(1), modes))
            continue
        if line.startswith("default:"):
            in_default = True
            continue
        lba = re.match(r"ovl_load_lba\s*=\s*(0x[0-9A-Fa-f]+);", line)
        if lba:
            value = int(lba.group(1), 16)
            if in_default:
                default_lba = value
            for mode in pending:
                if mode in table:
                    raise AssertionError("mode 0x%X appears twice in the sector switch" % mode)
                table[mode] = value
            pending = []
    if default_lba is None or not table:
        raise AssertionError("sector switch not found")
    return table, default_lba


def scheduler_dispatch_switch() -> dict[int, list[int]]:
    """day_event_dispatch's queue switch: event type -> native modes set."""
    text = SCHEDULER.read_text(encoding="utf-8")
    start = text.index("op = game_state.day_events[game_state.day_event_index].type;")
    body = text[start:text.index("void schedule_quarter_flags_rotate(void)")]
    modes = enum_values("GAME_MODE_")
    return _close_cases(body, modes)


def _close_cases(body: str, modes: dict[str, int]) -> dict[int, list[int]]:
    """Group game_mode_set calls by the case labels that precede them."""
    table: dict[int, list[int]] = {}
    labels: list[int] = []
    collecting_labels = False
    for raw in body.splitlines():
        line = raw.strip()
        case = re.match(r"case\s+(0x[0-9A-Fa-f]+):", line)
        if case or line.startswith("default:"):
            if not collecting_labels:
                labels = []
            collecting_labels = True
            labels.append(int(case.group(1), 16) if case else -1)
            continue
        collecting_labels = False
        mode = re.search(r"game_mode_set\((GAME_MODE_[A-Z0-9_]+)\)", line)
        if mode:
            for event in labels:
                table.setdefault(event, []).append(modes[mode.group(1)])
    return table


def gd_dict(path: Path, name: str) -> dict:
    text = path.read_text(encoding="utf-8")
    match = re.search(r"const %s[^=]*=\s*\{(.*?)\n\}" % re.escape(name), text, re.S)
    if not match:
        raise AssertionError("%s not found in %s" % (name, path))
    body = re.sub(r"#[^\n]*", "", match.group(1))
    out: dict = {}
    for key, value in re.findall(r"(0x[0-9A-Fa-f]+|\"[A-Z0-9_]+\")\s*:\s*(\[[^\]]*\]|0x[0-9A-Fa-f]+|\d+)", body):
        k = key.strip('"') if key.startswith('"') else int(key, 16)
        if value.startswith("["):
            name_match = re.match(r'\[\s*"([A-Z0-9_]+)"\s*,\s*(0x[0-9A-Fa-f]+)\s*\]', value)
            if not name_match:
                raise AssertionError("bad row %r in %s" % (value, name))
            v = (name_match.group(1), int(name_match.group(2), 16))
        else:
            v = int(value, 0)
        if k in out:
            raise AssertionError("duplicate key %r in %s" % (k, name))
        out[k] = v
    if not out:
        raise AssertionError("%s is empty" % name)
    return out


def catalog_overlays() -> dict[int, str]:
    text = DISPATCHER.read_text(encoding="utf-8")
    consts = {n: int(v, 16) for n, v in re.findall(r"const (GAME_MODE_[A-Z_]+)\s*:=\s*(0x[0-9A-Fa-f]+)", text)}
    body = text[text.index("const OVERLAY_CATALOG := {"):]
    body = body[:body.index("\n}")]
    out = {}
    for name, overlay in re.findall(r"(GAME_MODE_[A-Z_]+):\s*\{\"overlay\":\s*\"([A-Z0-9_]+)\"", body):
        out[consts[name]] = overlay
    return out


def overlay_table() -> tuple[dict[int, tuple[str, int]], tuple[str, int], dict[str, int]]:
    rows = gd_dict(OVERLAY_TABLE, "OVERLAY_BY_NATIVE_MODE")
    text = OVERLAY_TABLE.read_text(encoding="utf-8")
    default = re.search(r'const DEFAULT_OVERLAY: Array = \["([A-Z0-9_]+)", (0x[0-9A-Fa-f]+)\]', text)
    if not default:
        raise AssertionError("DEFAULT_OVERLAY not found")
    by_overlay = gd_dict(OVERLAY_TABLE, "DISPATCHER_MODE_BY_OVERLAY")
    return rows, (default.group(1), int(default.group(2), 16)), by_overlay


class LoaderTableTests(unittest.TestCase):
    def test_every_sector_case_has_the_same_lba(self):
        source, default_lba = loader_sector_switch()
        rows, default, _ = overlay_table()
        self.assertEqual(default[1], default_lba)
        for mode, lba in source.items():
            self.assertIn(mode, rows, "mode 0x%X missing from OVERLAY_BY_NATIVE_MODE" % mode)
            self.assertEqual(rows[mode][1], lba, "mode 0x%X" % mode)
        extra = sorted(set(rows) - set(source))
        # Only GAME_MODE_SHOOTER takes the default arm.
        self.assertEqual(extra, [enum_values("GAME_MODE_")["GAME_MODE_SHOOTER"]])
        self.assertEqual(rows[extra[0]], default)

    def test_one_file_name_per_lba(self):
        rows, default, _ = overlay_table()
        names: dict[int, str] = {}
        for name, lba in list(rows.values()) + [default]:
            self.assertEqual(names.setdefault(lba, name), name, "LBA 0x%X has two names" % lba)
        self.assertEqual(len(set(names.values())), len(names))

    def test_family_modes_load_their_overlay(self):
        rows, _, _ = overlay_table()
        modes = enum_values("GAME_MODE_")
        expected = {
            "GAME_MODE_PLAYER_BIRTHDAY": "GEKO", "GAME_MODE_GIRL_BIRTHDAY": "GEKO",
            "GAME_MODE_GIRL_ENCOUNTER": "GEKO", "GAME_MODE_GIRL_INVITATION": "GEKO",
            "GAME_MODE_SPECIAL_EVENT": "GEKO", "GAME_MODE_SCHOOL_TRIP": "SHUGAKU",
            "GAME_MODE_NEW_YEAR_A": "SHOUGATU", "GAME_MODE_NEW_YEAR_B": "SHOUGATU",
            "GAME_MODE_MEETING": "SHOUGATU", "GAME_MODE_CLUB_CAMP": "KANGEI",
            "GAME_MODE_CULTURE_FESTIVAL_A": "BUNKASAI", "GAME_MODE_CULTURE_FESTIVAL_B": "BUNKAKEN",
            "GAME_MODE_CULTURE_STAGE": "BUNKA_SD", "GAME_MODE_SPORTS_FESTIVAL": "TAIIKU",
        }
        for name, overlay in expected.items():
            self.assertEqual(rows[modes[name]][0], overlay, name)

    def test_dispatcher_keys_name_the_same_overlay(self):
        _, _, by_overlay = overlay_table()
        catalog = catalog_overlays()
        for overlay, key in by_overlay.items():
            self.assertIn(key, catalog)
            # The catalog labels RPG_BAT/DATE2 rows with those names too.
            self.assertEqual(catalog[key], overlay, "catalog 0x%X" % key)
            self.assertEqual([k for k, v in catalog.items() if v == overlay], [key])


class EventTableTests(unittest.TestCase):
    def test_event_modes_follow_day_event_dispatch(self):
        source = scheduler_dispatch_switch()
        port = gd_dict(EVENT_TABLE, "EVENT_MODES")
        events = enum_values("DAY_EVENT_")
        for event, modes in source.items():
            if event == -1:
                continue
            if event == events["DAY_EVENT_CULTURE_FESTIVAL"]:
                # Two modes chosen by club; DayEventModes.mode_for handles the branch.
                self.assertEqual(len(modes), 2)
                continue
            self.assertEqual(len(modes), 1, "event 0x%X" % event)
            self.assertEqual(port.get(event), modes[0], "event 0x%X" % event)
        # The appointment record is not a case of this switch.
        self.assertEqual(sorted(set(port) - set(source)), [events["DAY_EVENT_APPOINTMENT"]])

    def test_family_events_reach_their_overlay_catalog_key(self):
        rows, _, by_overlay = overlay_table()
        port_modes = gd_dict(EVENT_TABLE, "EVENT_MODES")
        to_dispatcher = gd_dict(EVENT_TABLE, "EVENT_TO_DISPATCHER_MODE")
        checked = 0
        for event, native in port_modes.items():
            row = rows.get(native)
            if row is None or row[0] not in FAMILY_OVERLAYS:
                continue
            self.assertEqual(to_dispatcher[event], by_overlay[row[0]], "event 0x%X" % event)
            checked += 1
        self.assertGreaterEqual(checked, 10)

    def test_family_native_rows_reach_their_overlay(self):
        rows, _, by_overlay = overlay_table()
        native = gd_dict(EVENT_TABLE, "NATIVE_TO_DISPATCHER_MODE")
        checked = 0
        for mode, key in native.items():
            row = rows.get(mode)
            if row is None or row[0] not in FAMILY_OVERLAYS or mode in CATALOG_COLLISIONS:
                continue
            self.assertEqual(key, by_overlay[row[0]], "native 0x%X" % mode)
            checked += 1
        self.assertGreaterEqual(checked, 10)

    def test_negative_wrong_row_is_detected(self):
        """A mutated EVENT_MODES row must fail the same comparison."""
        source = scheduler_dispatch_switch()
        port = dict(gd_dict(EVENT_TABLE, "EVENT_MODES"))
        port[0x33] = 0x4F
        self.assertNotEqual(port[0x33], source[0x33][0])
        rows, _, by_overlay = overlay_table()
        self.assertNotEqual(by_overlay[rows[0x56][0]], 0x4F)


# ------------------------------------------------------------------ disc check

SECTOR = 2352
USER = 24  # MODE2 Form 1: sync 12 + header 4 + subheader 8


def read_lba(image, lba: int, size: int) -> bytes:
    """User data of size bytes starting at an absolute LBA of a 2352-byte image."""
    out = bytearray()
    sector = lba
    while len(out) < size:
        image.seek(sector * SECTOR + USER)
        data = image.read(2048)
        if len(data) != 2048:
            raise ValueError("short read at LBA %d" % sector)
        out += data
        sector += 1
    return bytes(out[:size])


def check_disc(image_path: Path, files_root: Path) -> list[str]:
    """Each table LBA must hold the bytes of CDROM/EXEDIR/<name>.EXN."""
    errors = []
    rows, default, _ = overlay_table()
    with open(image_path, "rb") as image:
        for name, lba in sorted(set(rows.values()) | {default}):
            path = files_root / "CDROM" / "EXEDIR" / (name + ".EXN")
            if not path.is_file():
                errors.append("%s missing" % path)
                continue
            data = path.read_bytes()
            if len(data) > 0x60 * 2048:
                errors.append("%s.EXN larger than the 0x60-sector read" % name)
            elif read_lba(image, lba, len(data)) != data:
                errors.append("%s.EXN bytes are not at LBA 0x%X" % (name, lba))
    return errors


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description="Check overlay LBAs against the original disc.")
    parser.add_argument("--disc", type=Path, required=True, help="MODE2/2352 track 1 image")
    parser.add_argument("--files", type=Path, required=True, help="extracted disc file root")
    args = parser.parse_args(argv)
    for path in (args.disc, args.files):
        if not path.exists():
            print("not found: %s" % path, file=sys.stderr)
            return 1
    errors = check_disc(args.disc, args.files)
    for error in errors:
        print(error, file=sys.stderr)
    if errors:
        return 1
    rows, default, _ = overlay_table()
    print("disc LBAs hold all %d overlay files" % len(set(rows.values()) | {default}))
    return 0


if __name__ == "__main__":
    sys.exit(main())
