extern u8 D_800E62BE;

void func_801431D4(void) {
    func_8004E9F4(0);
    switch (D_80122EB8) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 2; irregular */
        case 0x5F:                                  /* switch 2 */
            func_8006612C((s32) "詩集展示");
            break;
        case 0x60:                                  /* switch 2 */
            func_8006612C((s32) "文集展示");
            break;
        case 0x61:                                  /* switch 2 */
            func_8006612C((s32) "弁論大会");
            break;
        }
        break;
    case 1:                                         /* switch 1 */
        func_8006612C((s32) "演劇部公演");
        break;
    case 2:                                         /* switch 1 */
        func_8006612C((s32) "実験ショー");
        break;
    case 3:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 3; irregular */
        case 0x5F:                                  /* switch 3 */
            func_8006612C((s32) "パ\x83\\コン占い");
            break;
        case 0x60:                                  /* switch 3 */
            func_8006612C((s32) "ゲーム");
            break;
        case 0x61:                                  /* switch 3 */
            func_8006612C((s32) "実験ショー");
            break;
        }
        break;
    case 4:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 4; irregular */
        case 0x5F:                                  /* switch 4 */
            func_8006612C((s32) "似顔絵");
            break;
        case 0x60:                                  /* switch 4 */
            func_8006612C((s32) "公開デッサン");
            break;
        case 0x61:                                  /* switch 4 */
            func_8006612C((s32) "油絵展示");
            break;
        }
        break;
    case 5:                                         /* switch 1 */
        func_8006612C((s32) "演奏会");
        break;
    }
    func_8004E884(1);
}
