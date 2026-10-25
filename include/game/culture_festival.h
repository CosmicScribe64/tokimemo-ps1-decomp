#ifndef GAME_CULTURE_FESTIVAL_H
#define GAME_CULTURE_FESTIVAL_H

#include "common.h"

#ifndef BUNKA_DISPATCH8_T10630_H
#define BUNKA_DISPATCH8_T10630_H
/* The two adjacent words at D_80122EB8, read and written together (one object): BUNKASAI's and BUNKAKEN's
 * CultureFestivalDispatch (T-10630) and BUNKASAI's BunkSelectWords (T-9510) and BunkModePair (T-9740), one type since
 * T-20900. */
typedef struct CultureFestivalDispatch {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04; /* HYPOTHESIS: the word pair D_80122EB8 shared by BUNKASAI and BUNKAKEN */
} CultureFestivalDispatch; /* size 0x08 */
#endif

#endif /* GAME_CULTURE_FESTIVAL_H */
