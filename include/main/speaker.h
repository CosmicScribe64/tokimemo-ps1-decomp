#ifndef MAIN_SPEAKER_H
#define MAIN_SPEAKER_H

#include "common.h"

extern s32 speaker_voice_source_a;
extern s32 speaker_voice_source_b;
extern s32 speaker_voice_index;
extern s32 speaker_voice_state;

#ifndef MAIN_API_OVERRIDE_gnsx
void gnsx(u8 *speaker_name);
#endif
void sndisp();
void sndi(u8 *arg0, s32 arg1, s32 arg2);
enum SpeakerVoiceState {
    SPEAKER_VOICE_PENDING = 0, /* T-27062 request not yet started */
    SPEAKER_VOICE_PLAYING = 1, /* T-27062 playback observed */
    SPEAKER_VOICE_DONE = 2 /* T-27062 finished or no voice */
};
void xa_wait(void);

#endif
