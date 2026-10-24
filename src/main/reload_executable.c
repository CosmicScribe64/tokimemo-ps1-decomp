/* reload_executable.c: src/main/80056300.c; T-27059: object [0x80056300,0x800563F0); its sole O.BIN function exit_function shuts down CD/SPU/timers/callbacks and calls LoadExec for PSX.EXE; preserve O.BIN function name. This is not STR playback; use full executable rather than prior h-cdio abbreviation reload_exe. Original object 0x80056300-0x800563F0 (config/objects/main.txt). */
#include "common.h"
#include "game.h"

void exit_function(void) {
    s32 pad; /* FAKE: unused local reserves a stack word (frame home). T-11170 */
    u8 sp2B;

    func_8009C674(0);
    func_800AD950(0);
    func_8008BE54();
    sp2B = 0x80;
    func_80087C64(0xE, (s32) &sp2B, 0);
    func_800869C8(0);
    func_80087E08(0);
    func_800879A8(0);
    CloseEvent(D_8011ECA8);
    StopRCnt(-0x0E000000);
    StopRCnt(-0x0DFFFFFF);
    StopRCnt(-0x0DFFFFFE);
    StopRCnt(-0x0DFFFFFD);
    func_8009C210(3);
    func_800ADBB0();
    StopCallback();
    ResetCallback();
    _96_init();
    EnterCriticalSection();
    LoadExec((s32) "cdrom:PSX.EXE;1", 0x801FFFF0, 0);
    _96_remove();
}
