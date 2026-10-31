#!/usr/bin/env python3
"""Compile a route strategy (config/port/route-strategies/*.json) into an
expanded tokimemo.input-script.v2 (config/port/input-scripts/<name>.json).

Each week gets three segments, all anchored on observed state, never on wall
time:
  wNNN-cursor   only when the command screen is the ordinary build (icon_count
                14) and the cursor is still on last week's command (or on 14
                after a proceed week): RIGHT or LEFT pulses until menu_cursor is
                this week's command. Weeks whose command equals last week's send
                nothing (until already holds at the anchor).
  wNNN-confirm  circle on the command screen until mode 0x30 starts.
  wNNN-week     circle every advance_every iterations outside mode 0x34 until
                the next 0x34/1/0 (school days, holiday, notices, scenes take
                the default cursor).
The command of week n is chosen from the plan entry for the month of that
week's command-select date: week 1 selects on 1995-04-04 (day_count 0), week n
>= 2 on the Sunday 1995-04-09 + 7 (n - 2) (m-scheduler: the queue of a week
ends with the Sunday holiday record and the command screen opens on that
Sunday). The cursor plan comes from the command icon order (m-sysmenu 3.3:
d-pad steps icons in index order, RIGHT/DOWN next, LEFT/UP previous); the
until predicate makes the press count follow the source's own skipping of
disabled items.

Repeats: consecutive weeks with the same command after the first one compile
to a ``repeat`` block in the source form (--source), and ``expand`` gives the
same segments as the direct compilation (tested).

usage: python3 -I tools/port/route_strategy.py STRATEGY [--out FILE] [--source FILE] [--continuous] [--weeks N]
"""
import argparse
import datetime
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "emu"))
import route_script  # noqa: E402

SELECT = {"game_mode": "0x34", "mode_stage": 1, "mode_step": 0}
FIRST_SUNDAY = datetime.date(1995, 4, 9)


class StrategyError(ValueError):
    pass


def select_date(week, first):
    return first if week == 1 else FIRST_SUNDAY + datetime.timedelta(days=7 * (week - 2))


def plan_command(strategy, week, date, counters):
    """(command, plan entry index) for a week: the plan entry whose 'from' month
    is the latest at or before the select date; the entry's cycle advances once
    per week spent in that entry."""
    key = "%04d-%02d" % (date.year, date.month)
    entries = [i for i, e in enumerate(strategy["plan"]) if e["from"] <= key]
    if not entries:
        raise StrategyError("week %d (%s) is before the first plan entry" % (week, date))
    index = entries[-1]
    cycle = strategy["plan"][index]["cycle"]
    position = counters.get(index, 0)
    counters[index] = position + 1
    command = cycle[position % len(cycle)]
    if command not in range(8) or command == 4:
        raise StrategyError("plan entry %d: command %r is not an ordinary weekday command without a club" % (index, command))
    return command, index


def week_segments(prefix, command, previous, timing, first_week):
    hold = timing["hold"]
    direction = "RIGHT" if (command - previous) % 14 <= 7 else "LEFT"
    cursor = {
        "id": prefix + "-cursor", "kind": "pulse",
        "note": "command %d (from cursor %d)" % (command, previous),
        "wait_until": dict(SELECT),
        "only_if": [dict(SELECT, icon_count=14, menu_cursor=previous), dict(SELECT, icon_count=14, menu_cursor=14)],
        "pulse": {"buttons": [direction], "hold": hold, "every": timing["cursor_every"], "offset": timing["cursor_offset"],
                  "max": timing["cursor_max"], "until": dict(SELECT, menu_cursor=command)}}
    confirm = {
        "id": prefix + "-confirm", "kind": "pulse", "wait_until": dict(SELECT),
        "pulse": {"buttons": ["CIRCLE"], "hold": hold, "every": timing["confirm_every"], "offset": timing["confirm_offset"],
                  "max": timing["confirm_max"], "until": {"game_mode": "0x30"}}}
    if first_week:
        confirm["expect_next"] = {"game_mode": "0x30", "mode_stage": 0, "mode_step": 0}
    week = {
        "id": prefix + "-week", "kind": "pulse", "wait_until": {"game_mode": "0x30"},
        "timeout_iterations": timing["week_timeout"],
        "pulse": {"buttons": [timing["advance_button"]], "hold": hold, "every": timing["advance_every"],
                  "offset": timing["advance_offset"], "max": timing["advance_max"], "until": dict(SELECT),
                  "skip_when": [{"game_mode": mode} for mode in timing["advance_skip_modes"]]}}
    return [cursor, confirm, week]


def save_weeks(strategy, weeks):
    save = strategy.get("save_relaunch")
    if not save:
        return {}
    out = {}
    week = save["first_after_week"]
    while week < weeks:
        out[week] = save["slot"]
        week += save["every_weeks"]
    return out


def compile_strategy(strategy, continuous=False, weeks=None, source_form=False):
    weeks = weeks or strategy["weeks"]
    first = datetime.date.fromisoformat(strategy["first_select"]["date"])
    previous = strategy["first_select"]["cursor"]
    timing = strategy["timing"]
    saves = {} if continuous else save_weeks(strategy, weeks)
    counters = {}
    plan = []
    for week in range(1, weeks + 1):
        date = select_date(week, first)
        command, entry = plan_command(strategy, week, date, counters)
        plan.append({"week": week, "select_date": date.isoformat(), "command": command, "previous": previous,
                     "plan_entry": entry})
        previous = command
    segments = []
    index = 0
    while index < len(plan):
        row = plan[index]
        # Source form: a run of weeks that repeat the same command (cursor stays) and hold no save.
        run = 1
        if source_form and row["week"] > 1 and row["command"] == row["previous"] and row["week"] not in saves:
            while (index + run < len(plan) and plan[index + run]["command"] == row["command"]
                   and plan[index + run]["previous"] == row["command"] and plan[index + run]["week"] not in saves
                   and plan[index + run - 1]["week"] not in saves):
                run += 1
        if run > 1:
            body = week_segments("w{i}", row["command"], row["command"], timing, False)
            segments.append({"id": "r%03d" % row["week"], "kind": "repeat", "first": row["week"], "count": run, "body": body,
                             "note": "weeks %d-%d: command %d" % (row["week"], row["week"] + run - 1, row["command"])})
        else:
            segments += week_segments("w%03d" % row["week"], row["command"], row["previous"], timing, row["week"] == 1)
        for offset in range(run):
            week = row["week"] + offset
            if week in saves:
                segments.append({"id": "w%03d-save" % week, "kind": "save_relaunch", "slot": saves[week],
                                 "wait_until": dict(SELECT), "note": "after week %d" % week})
        index += run
    name = strategy["name"] + ("_continuous" if continuous else "")
    script = {
        "schema": route_script.SCHEMA_V2,
        "name": name,
        "description": strategy["description"] + (" Continuous variant: no save_relaunch segments." if continuous else ""),
        "provenance": {
            "source": "config/port/route-strategies/%s.json compiled by tools/port/route_strategy.py" % strategy["name"],
            "derivation": "week n selects on 1995-04-04 (n = 1) or the Sunday 1995-04-09 + 7 (n - 2) (m-scheduler 1); "
                          "commands from the plan entry of that month; cursor moves by the icon order of m-sysmenu 3.3 "
                          "with an until predicate on menu_cursor; circle at the source confirm (m-sysmenu 3.3) and "
                          "default answers elsewhere",
            "uncertainty": "select dates assume one command screen per week (no skipped weeks); a week whose command "
                           "screen is a restricted build (exam, school trip, sick, remedial) sends no cursor move and "
                           "confirms the forced icon, so the plan's command for that week is not used. Forced events "
                           "and dates take the default cursor answer.",
            "plan": plan if not source_form else "see compiled script"},
        "start": strategy["start"],
        "segments": segments,
        "checkpoints": strategy["checkpoints"],
    }
    if strategy.get("unsampled_ok"):
        script["unsampled_ok"] = strategy["unsampled_ok"]
    # A pinned stop whose segment this compilation leaves out (--continuous drops the save_relaunch
    # segments, --weeks may end earlier) cannot happen here: it is listed as a known gap, not pinned.
    ids = {s["id"] for s in segments if s.get("kind") != "repeat"} | {
        route_script._substitute(inner["id"], i) for s in segments if s.get("kind") == "repeat"
        for i in range(s["first"], s["first"] + s["count"]) for inner in s["body"]}
    script["unsupported"] = [row for row in strategy["unsupported"] if row["at_segment"] in ids]
    gaps = list(strategy.get("known_gaps", []))
    for row in strategy["unsupported"]:
        if row["at_segment"] not in ids:
            gap = {"side": row["side"], "gap": "%s (pinned at %s, a segment this compilation leaves out)" % (
                row["stop"], row["at_segment"]), "reason": row["reason"]}
            if "ticket" in row:
                gap["ticket"] = row["ticket"]
            gaps.append(gap)
    if gaps:
        script["known_gaps"] = gaps
    errors = route_script.semantic_errors(script)
    if errors:
        raise StrategyError("; ".join(errors))
    return script


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("strategy", type=Path)
    parser.add_argument("--out", type=Path)
    parser.add_argument("--source", type=Path, help="also write the unexpanded source form (repeat blocks)")
    parser.add_argument("--continuous", action="store_true", help="leave out save_relaunch segments")
    parser.add_argument("--weeks", type=int)
    args = parser.parse_args(argv)
    try:
        strategy = json.loads(args.strategy.read_text())
        if strategy.get("schema") != "tokimemo.route-strategy.v1":
            raise StrategyError("not a tokimemo.route-strategy.v1 strategy")
        script = compile_strategy(strategy, args.continuous, args.weeks)
        if args.source:
            source = compile_strategy(strategy, args.continuous, args.weeks, source_form=True)
            if route_script.expand(source)["segments"] != script["segments"]:
                raise StrategyError("source form does not expand to the compiled script")
            args.source.write_text(json.dumps(source, indent=1) + "\n")
        text = json.dumps(script, indent=1) + "\n"
        if args.out:
            args.out.write_text(text)
        else:
            sys.stdout.write(text)
        return 0
    except (StrategyError, route_script.ScriptError, OSError, KeyError, ValueError) as error:
        print("route_strategy: %s" % error, file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
