#ifndef LIBAPI_LEGACY_H
#define LIBAPI_LEGACY_H

/* Preserve the captured implicit-call views for two legacy call sites. */
#define MAIN_API_OVERRIDE_EnterCriticalSection /* implicit declaration preserved (T-27061). */
#define MAIN_API_OVERRIDE_ExitCriticalSection /* implicit declaration preserved (T-27061). */
#include "libapi.h"
#undef MAIN_API_OVERRIDE_EnterCriticalSection
#undef MAIN_API_OVERRIDE_ExitCriticalSection

#endif /* LIBAPI_LEGACY_H */
