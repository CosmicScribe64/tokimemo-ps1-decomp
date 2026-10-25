"""Source-backed string READ argument roles shared by text analysis (T-28001).

Project display signatures are verified in the main C implementations and
main_api.h. Standard C string/format operations distinguish read operands from
output buffers and numeric arguments. This table does not prove a call executes
or that all of a pointer table is read; consumers must prove expression flow.
"""

DISPLAY_ARGUMENT_INDEX = {
    "load_string": 0, "set_kanji_string": 3, "sndi": 0, "sndisp": 0,
    "gnsx": 0, "message_window_set_title": 0, "talk_render": 3, "talk_speak_part": 0,
}
FORMAT_ARGUMENT_INDEX = {
    "printf": 0, "fprintf": 1, "sprintf": 1, "snprintf": 2,
    "vprintf": 0, "vfprintf": 1, "vsprintf": 1, "vsnprintf": 2,
}
DIAGNOSTIC_ARGUMENT_INDEX = {"puts": 0, "perror": 0}
C_STRING_READ_ARGUMENTS = {
    "strcpy": (1,), "strncpy": (1,), "strcat": (1,), "strncat": (1,),
    "strlen": (0,), "strcmp": (0, 1), "strncmp": (0, 1),
    "strchr": (0,), "strrchr": (0,), "strstr": (0, 1),
}
DISPLAY_CALLS = set(DISPLAY_ARGUMENT_INDEX)
FORMAT_CALLS = set(FORMAT_ARGUMENT_INDEX) | set(DIAGNOSTIC_ARGUMENT_INDEX)

# These are source-proved aliases, kept separate from the general name table so
# overlay-local numeric names cannot collide. Main helpers are visible to every
# caller unit. The RPG_BAT alias is visible only from that overlay.
GLOBAL_ALIAS_ARGUMENTS = {
    "message_window_set_title_hidden": {
        0: {"role": "display", "renderer": "set_kanji_string", "conditions": ()},
    },
    "x_taku_string_set": {
        0: {"role": "display", "renderer": "set_kanji_string", "conditions": ()},
        1: {"role": "display", "renderer": "set_kanji_string", "conditions": ()},
        2: {"role": "display", "renderer": "set_kanji_string", "conditions": (
            {"argument_index": 3, "operator": "==", "value": 3},
            {"argument_index": 2, "operator": "!=", "value": 0},
        )},
    },
}
UNIT_ALIAS_ARGUMENTS = {
    "RPG_BAT": {
        "rpg_label_create": {
            2: {"role": "display", "renderer": "set_kanji_string", "conditions": ()},
        },
    },
}
DISPLAY_RENDERERS = {
    "set_kanji_string": "set_kanji_string",
    "gnsx": "set_kanji_string",
    "message_window_set_title": "set_kanji_string",
}
UNIMPLEMENTED_FIXED_RENDERER_CALLS = {"talk_render", "talk_speak_part"}


def read_argument_specs(call: str | None, unit: str | None = None) -> tuple[dict, ...]:
    """Return source-backed input argument roles, renderers, and read guards.

    ``unit`` is the caller's link unit (``main`` or an overlay ID). The main
    aliases are globally callable; overlay-local address aliases require an
    exact unit. Conditions describe source-level conditional reads and are not
    claims about runtime execution.
    """
    if call is None:
        return ()
    specs = []
    if call in DISPLAY_ARGUMENT_INDEX:
        index = DISPLAY_ARGUMENT_INDEX[call]
        renderer = DISPLAY_RENDERERS.get(call, call)
        spec = {"index": index, "role": "display", "renderer": renderer, "conditions": ()}
        if renderer != call:
            spec["renderer_status"] = "source_verified_alias"
        if call in UNIMPLEMENTED_FIXED_RENDERER_CALLS:
            spec["renderer"] = None
            spec["renderer_status"] = "unimplemented_fixed_renderer"
        specs.append(spec)
    if call in FORMAT_ARGUMENT_INDEX:
        index = FORMAT_ARGUMENT_INDEX[call]
        specs.append({"index": index, "role": "format", "renderer": call, "conditions": ()})
    if call in DIAGNOSTIC_ARGUMENT_INDEX:
        index = DIAGNOSTIC_ARGUMENT_INDEX[call]
        specs.append({"index": index, "role": "diagnostic", "renderer": call, "conditions": ()})
    for index in C_STRING_READ_ARGUMENTS.get(call, ()):
        specs.append({"index": index, "role": "c_string_operand", "renderer": call,
                      "conditions": ()})
    alias_specs = GLOBAL_ALIAS_ARGUMENTS.get(call, {})
    if unit in UNIT_ALIAS_ARGUMENTS:
        alias_specs = {**alias_specs, **UNIT_ALIAS_ARGUMENTS[unit].get(call, {})}
    for index, spec in alias_specs.items():
        specs.append({"index": index, "role": spec["role"], "renderer": spec["renderer"],
                      "renderer_status": "source_verified_alias",
                      "conditions": tuple(dict(condition) for condition in spec["conditions"])})
    return tuple(sorted(specs, key=lambda item: item["index"]))


def known_calls(unit: str | None = None) -> set[str]:
    """Return only known read consumers visible to the selected caller unit."""
    calls = set(DISPLAY_ARGUMENT_INDEX) | set(FORMAT_ARGUMENT_INDEX) | \
        set(DIAGNOSTIC_ARGUMENT_INDEX) | set(C_STRING_READ_ARGUMENTS) | set(GLOBAL_ALIAS_ARGUMENTS)
    if unit in UNIT_ALIAS_ARGUMENTS:
        calls.update(UNIT_ALIAS_ARGUMENTS[unit])
    return calls


def string_argument_role(call: str | None, argument_index: int | None,
                         unit: str | None = None) -> str | None:
    """Return a known read role; destination buffers never count as source text."""
    if argument_index is None:
        return None
    return next((spec["role"] for spec in read_argument_specs(call, unit)
                 if spec["index"] == argument_index), None)
