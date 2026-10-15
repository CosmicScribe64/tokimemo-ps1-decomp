typedef struct EventStatsRecord34 {
    /* 0x00 */ GsWord unk_00;
    /* 0x04 */ GsWord unk_04;
    /* 0x08 */ GsWord unk_08;
    /* 0x0C */ u8 unk_0C[0x28];
} EventStatsRecord34; /* size 0x34 */
typedef struct EventStatsStateView {
    /* 0x000 */ u8 unk_000[0x1A4];
    /* 0x1A4 */ EventStatsRecord34 unk_1A4[12];
} EventStatsStateView; /* size 0x414 */
