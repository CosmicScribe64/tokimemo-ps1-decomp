# T-24026 source-only evidence

func_80138240 ports matched GYOZI func_8013B0C4 using FnTbl37 and existing typed game fields. Its 69-instruction candidate differs only in four frame/table offsets: frame 208 versus target 200, table at 56 versus target 48. Declaration after table moves it to 60 without changingframe 208. Removing index gives frame 200 / table 52 and loses shared v0 selector scheduling. Narrow u8 index similarly loses scheduling. A fall-through s32 return retains the original four differences. Do not repeat these exhausted variants. No assembly or raw instruction listings are archived.

The production draft stays explicitly NON_MATCHING; it is not a new C match. The two other targets are unguarded verified C.
