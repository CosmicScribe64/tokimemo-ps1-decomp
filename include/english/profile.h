#ifndef ENGLISH_PROFILE_H
#define ENGLISH_PROFILE_H

/* T-39506: tools/patch/text/profile.py packs these labels inside JOHO's
 * unchanged 38-sector load at 0x801A0000. */
#define ENGLISH_PROFILE_CLUBS ((u8 *)0x801B2F00)
#define ENGLISH_PROFILE_CLUB_STRIDE 18
#define ENGLISH_PROFILE_SIGNS ((u8 *)0x801B2200)
#define ENGLISH_PROFILE_SIGN_STRIDE 12

#endif
