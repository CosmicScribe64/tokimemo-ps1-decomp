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


def string_argument_role(call: str | None, argument_index: int | None) -> str | None:
    """Return a known read role; destination buffers never count as source text."""
    if argument_index is None:
        return None
    if call in DISPLAY_ARGUMENT_INDEX and argument_index == DISPLAY_ARGUMENT_INDEX[call]:
        return "display"
    if call in FORMAT_ARGUMENT_INDEX and argument_index == FORMAT_ARGUMENT_INDEX[call]:
        return "format"
    if call in DIAGNOSTIC_ARGUMENT_INDEX and argument_index == DIAGNOSTIC_ARGUMENT_INDEX[call]:
        return "diagnostic"
    if argument_index in C_STRING_READ_ARGUMENTS.get(call, ()):
        return "c_string_operand"
    return None
