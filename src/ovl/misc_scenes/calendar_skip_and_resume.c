/* calendar_skip_and_resume.c: ; src/ovl/ETC/8014C2D0.c:4-18 advances six calendar days then resumes day events in both handlers. Original object 0x8014C2D0-0x8014C370 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/misc_scenes.h"

void misc_unused_skip_week_a(void) {
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    day_event_next(1);
}

void misc_unused_skip_week_b(void) {
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    calendar_advance_day();
    day_event_next(1);
}
