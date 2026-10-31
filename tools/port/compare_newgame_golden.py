"""T-39002: compare the installed New Game route's committed states with the
original golden captures (.asset-cache/emu/newgame-golden-v1, input set A).

The observer tests/opening/run_installed_command_route.gd writes
<out>/states/<mode>-<stage>-<step>-<substep>.json for every committed state
from stage 21 on. A committed state is the state draw_frame sees after that
frame's game_mode_dispatch, so:

  15-21-0-0   (20->21 exit frame committed)    <-> a1_stage21_sub0_pre
  15-21-0-1   (after stage 21 substep 0 ran)   <-> a2_stage21_sub0_post
  34-255-0-0  (after the substep 1 mode switch) <-> a4_mode_switch_post
  34-1-0-0    (first command-select commit)     <-> a6/a7 (pre of that frame)

Fields are grouped: "state" fields must match and are the only ones counted
in the match totals; "clock" fields depend on the unmeasured boot/title/
file-select history (absolute frame_count, timer_ticks, system_data.seed, the
BIOS rand word at kernel 0x850C, which prologue.c:206 reseeds from
frame_count) and are reported with their relation only; "diagnostic" rows are
derived values the port has not committed (the opening media owner's
bg_lba_cache) and are never counted.

Before command select (a1, a2, a4) the icon_enabled and menu_items.disabled
rows hold the boot values (fresh_gameplay_handoff.gd boot_icon_enabled and
boot_hud_init's menu), not output of settings_command_screen_build; build
parity needs the a7 state, which the report marks as not reached until the
route commits 34-1-0-0.

usage (Docker): python3 -I tools/port/compare_newgame_golden.py <route-out> <golden-run-dir> [--json out.json]
"""
import argparse
import json
import struct
import sys
from pathlib import Path

PAIRS = [("15-21-0-0", "a1_stage21_sub0_pre"), ("15-21-0-1", "a2_stage21_sub0_post"), ("34-255-0-0", "a4_mode_switch_post"),
         ("34-1-0-0", "a7_command_select_ready")]
RAND_WORD = 0x850C  # OpenBIOS rand() state (kernel RAM), found from srand(frame_count)


def _int(value):
    if isinstance(value, dict):
        views = value.get("views", {})
        for key in ("w", "h", "u"):
            if isinstance(views.get(key), int):
                return views[key]
        if "value" in value and "fraction" in value:
            return (value["value"] << 16) | (value["fraction"] & 0xFFFF)
        if "hex" in value:
            return int.from_bytes(bytes.fromhex(value["hex"]), "little")
    return value


def _word(fraction_value):
    return ((fraction_value["value"] & 0xFFFF) << 16) | (fraction_value["fraction"] & 0xFFFF)


def _s32(value):
    value &= 0xFFFFFFFF
    return value - 0x100000000 if value & 0x80000000 else value


BOOT_ICON_NOTE = "boot state (boot_hud_init), not settings_command_screen_build output"


def compare(port: dict, golden: dict, ram: bytes, tag: str = "") -> dict:
    gs = golden["game_state"]["fields"]
    sg = golden["session_globals"]
    sd = golden["system_data"]["fields"]
    state = port["state"]
    opening = state.get("opening", {})
    rule = port["rule_state"]
    rows = []

    def row(group, name, port_value, golden_value, note=None):
        entry = {"group": group, "field": name, "port": port_value, "golden": golden_value,
                 "match": port_value == golden_value}
        if note:
            entry["note"] = note
        rows.append(entry)

    for key in ("game_mode", "mode_stage", "mode_step", "substep", "game_mode_timer", "mode_stage_timer",
                "ovl_target_game_mode"):
        row("state", key, state.get(key), _int(gs[key]))
    row("state", "mode_step_timer", state.get("mode_step_timer"), _int(gs["mode_step_timer"]))
    for key in ("year", "month", "day", "weekday", "day_count", "menu_mode"):
        row("state", key, rule.get(key), _int(gs[key]))
    row("state", "player_flags", rule.get("player_flags"), _int(gs["player_flags"]))
    row("state", "player_flags_upper", rule.get("player_flags_upper"), _int(gs["player_flags_upper"]))
    row("state", "player_birthday", rule.get("player_birthday"), _int(gs["player_birthday"]))
    params = ["param_condition", "param_liberal_arts", "param_science", "param_art", "param_athletics",
              "param_trivia", "param_looks", "param_guts", "param_stress"]
    for index, name in enumerate(params):
        port_word = rule.get("params", [None] * 9)[index]
        row("state", name, None if port_word is None else _s32(port_word), _s32(_word(gs[name])))
    for index, girl in enumerate(gs["girls"]):
        mine = rule.get("girls", [])[index] if index < len(rule.get("girls", [])) else {}
        for field, word in (("tokimeki", "tokimeki_word"), ("affection", "affection_word"), ("syoushin", "syoushin_word")):
            golden_word = ((girl[field] & 0xFFFF) << 16) | (girl[field + "_fraction"] & 0xFFFF)
            row("state", "girls[%d].%s" % (index, field), None if mine.get(word) is None else _s32(mine[word]), _s32(golden_word))
        row("state", "girls[%d].flags" % index, mine.get("flags"), _int(girl["flags"]))
        row("state", "girls[%d].birthday" % index, mine.get("birthday"), _int(girl["birthday"]))
    for name, port_key in (("player_family_name", "profile_family_bytes"), ("player_given_name", "profile_given_bytes"),
                           ("player_nickname", "profile_nickname_bytes")):
        golden_bytes = gs[name]
        if isinstance(golden_bytes, dict):
            golden_bytes = list(bytes.fromhex(golden_bytes["hex"]))
        row("state", name, rule.get(port_key), golden_bytes)
    row("state", "event_log_count", rule.get("event_log_count"), _int(gs["event_log_count"]))
    row("state", "last_command", rule.get("last_command"), _int(gs["last_command"]))
    row("state", "icon_mode", opening.get("icon_mode"), _int(gs["icon_mode"]))
    row("state", "icon_count", opening.get("icon_count"), _int(gs["icon_count"]))
    row("state", "screen_brightness", opening.get("screen_brightness"), _int(gs["screen_brightness"]))
    row("state", "current_girl", rule.get("current_girl"), _int(gs["current_girl"]))
    icon_hex = sg["icon_enabled"]["value"]["hex"]
    icon_note = None if tag.startswith("a7") else BOOT_ICON_NOTE
    row("state", "icon_enabled[0..14]", rule.get("icon_enabled"), list(bytes.fromhex(icon_hex))[:15], icon_note)
    row("state", "menu_items[0..13].disabled", rule.get("menu_disabled"),
        [item["disabled"] for item in gs["menu_items"][:14]], icon_note)
    for name in ("D_800B3DB0", "D_800B5BD4", "D_800B5BC8", "hud_mascot_choice"):
        row("state", name, opening.get(name), _int(sg[name]["value"]))
    row("state", "bg_lba_cache", rule.get("background_cache"), sg["bg_lba_cache"]["value"])
    # The opening's own owner of bg_lba_cache[2..3]: names_media.buffer_b.lba
    # (layer 1); layer 0 is never read after prologue.c:218's dec_bg_reset.
    names = opening.get("names_media", {})
    if isinstance(names, dict) and isinstance(names.get("buffer_0"), dict) and isinstance(names.get("buffer_1"), dict):
        row("diagnostic", "bg_lba_cache (opening media owner, not committed)", [0, 0, names["buffer_0"].get("lba"), names["buffer_1"].get("lba")],
            sg["bg_lba_cache"]["value"])
    row("state", "dec_bg_show[0]", rule.get("background_show"), _int(sg["dec_bg_show"]["value"]) & 0xFF)
    row("state", "scene_work", rule.get("scene_work"), sg["scene_work"]["value"])
    # Clock-origin fields: values follow the boot/title/file-select history.
    rand_word = struct.unpack_from("<I", ram, RAND_WORD)[0]
    row("clock", "frame_count", state.get("frame_count"), _int(gs["frame_count"]))
    row("clock", "timer_ticks", state.get("timer_ticks"), _int(gs["timer_ticks"]))
    row("clock", "system_data.seed", state.get("system_seed"), _int(sd["seed"]))
    row("clock", "bios_rand_state", port.get("rng", {}).get("state"), rand_word)
    return {"rows": rows, "mismatches": [r for r in rows if r["group"] == "state" and not r["match"]]}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("route_out", type=Path)
    parser.add_argument("golden_run", type=Path)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    # A development resume (tests/checkpoint) is never golden evidence.
    if (args.route_out / "resumed-from.json").exists() or (args.route_out / "checkpoint-saved.json").exists():
        print("REFUSED: %s is a dev checkpoint run (resumed-from.json / checkpoint-saved.json)" % args.route_out)
        return 2
    report = {}
    for port_key, tag in PAIRS:
        port_path = args.route_out / "states" / (port_key + ".json")
        if not port_path.is_file():
            report[tag] = {"missing": str(port_path)}
            print("%s: port state %s not reached" % (tag, port_key))
            continue
        port = json.loads(port_path.read_text())
        golden = json.loads((args.golden_run / (tag + ".decoded.json")).read_text())
        ram = (args.golden_run / (tag + ".ram.bin")).read_bytes()
        result = compare(port, golden, ram, tag)
        report[tag] = result
        state_rows = [r for r in result["rows"] if r["group"] == "state"]
        print("%s vs port %s: %d/%d state fields match" % (tag, port_key, len(state_rows) - len(result["mismatches"]), len(state_rows)))
        for r in result["mismatches"]:
            print("  MISMATCH %s: port %s golden %s" % (r["field"], json.dumps(r["port"])[:120], json.dumps(r["golden"])[:120]))
        for r in result["rows"]:
            if r["group"] in ("clock", "diagnostic"):
                print("  %s %s: port %s golden %s (%s)" % (r["group"], r["field"], r["port"], r["golden"],
                                                         "same" if r["match"] else "differs"))
            elif r.get("note"):
                print("  note %s: %s" % (r["field"], r["note"]))
    if args.json:
        args.json.write_text(json.dumps(report, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
