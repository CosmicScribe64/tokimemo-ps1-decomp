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
