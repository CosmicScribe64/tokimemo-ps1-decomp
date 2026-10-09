#ifndef LIBAPI_H
#define LIBAPI_H

/* PsyQ libapi kernel calls (LIBAPI.H, PsyQ SDK 3.x; the SDK headers are not
 * in this repo, so the three prototypes used by decompiled code live here). */
int EnterCriticalSection(void);
void ExitCriticalSection(void);
void FlushCache(void);

#endif /* LIBAPI_H */
