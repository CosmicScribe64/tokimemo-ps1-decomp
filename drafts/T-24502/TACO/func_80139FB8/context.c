/* FAKE: scratch implicit-int font view reduces the allocation delta;
 * production declaration/callers stay untouched. T-24502 */
#include "common.h"
#define func_80139424 ignored_font_view
#include "ovl/TACO.h"
#undef func_80139424
s32 func_80139424();
#include "body.c"
