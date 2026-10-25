/* mouse.c: src/main/80059710.c; T-27059: object [0x80059710,0x80059A20); mouse-port sample buffers, state/read, sensitivity and range setup, scaled cursor movement and bounds (reuse h-cdio candidate). Original object 0x80059710-0x80059A20 (config/objects/main.txt). */
#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
/* The two mouse port buffers share one owned pointer table. T-12010. */
s8 *mouse_pad_buffers[2] = { 0, 0 };
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_position_x);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_position_y);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_x_min);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_x_max);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_y_min);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_y_max);
INCLUDE_RODATA("asm/data/main/mouse.data", mouse_sense_x);

/* Fills out[4] with dx, dy, buttons and type from the port buffer, or returns -1 with zeros when the pad is not ready. */
s32 MouseState(s32 port, s8 *out) {
    s8 **p;
    s32 result;
    p = &mouse_pad_buffers[port];
    result = 0;
    if ((*p)[0]) {
        result = -1;
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
    } else {
        out[0] = (*p)[4];
        out[1] = (*p)[5];
        out[2] = (*p)[3];
        out[3] = (*p)[1];
    }
    return result;
}

/* Stores the two pad receive buffers used for the mouse ports. */
void InitMouse(s8 *arg0, s8 *arg1) {
    mouse_pad_buffers[0] = arg0;
    mouse_pad_buffers[1] = arg1;
}

/* Sets the cursor clamp rectangle, scaled by the sensitivity. */
void RangeMouse(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 sx = mouse_sense_x;
    u32 sy = mouse_sense_y;

    mouse_x_min = arg0 * sx;
    mouse_x_max = arg1 * sx;
    mouse_y_min = arg2 * sy;
    mouse_y_max = arg3 * sy;
}

/* Sets the x and y sensitivity multipliers. */
void SenseMouse(u16 arg0, u16 arg1) {
    mouse_sense_x = arg0;
    mouse_sense_y = arg1;
}

/* Sets a port's cursor position, scaled by the sensitivity. */
void SetMouse(s32 arg0, u32 arg1, u32 arg2) {
    mouse_position_x[arg0] = mouse_sense_x * arg1;
    mouse_position_y[arg0] = mouse_sense_y * arg2;
}

typedef struct MouseSample {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 buttons;
} MouseSample; /* size 0xC */

void mouse_read(s32 arg0, MouseSample *arg1) {
    s8 *p;

    mouse_update(arg0);
    arg1->x = (u32)mouse_position_x[arg0] / mouse_sense_x;
    arg1->y = (u32)mouse_position_y[arg0] / mouse_sense_y;
    arg1->buttons = -0x100;
    p = mouse_pad_buffers[arg0];
    if ((p[0] == 0) && (p[1] == 0x12)) {
        arg1->buttons = ~p[3] & 0xC;
    }
}

void mouse_update(s32 port) {
    s8 **slot;
    s8 *p;
    slot = &mouse_pad_buffers[port];
    p = *slot;
    if ((p[0] == 0) && (p[1] == 0x12)) {
        mouse_position_x[port] += p[4];
        mouse_position_y[port] += p[5];
        if (mouse_position_x[port] > mouse_x_max) {
            mouse_position_x[port] = mouse_x_max;
        } else if (mouse_position_x[port] < mouse_x_min) {
            mouse_position_x[port] = mouse_x_min;
        }
        if (mouse_position_y[port] > mouse_y_max) {
            mouse_position_y[port] = mouse_y_max;
        } else if (mouse_position_y[port] < mouse_y_min) {
            mouse_position_y[port] = mouse_y_min;
        }
    }
}
