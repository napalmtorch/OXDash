#include "system/xbox.h"
#include "core/common.h"

TIME_FIELDS OX_GetXboxTime()
{
    TIME_FIELDS tf;
    LARGE_INTEGER lt;
    LONG timezone, daylight;
    ULONG type;
    KeQuerySystemTime(&lt);
    ExQueryNonVolatileSetting(XC_TIMEZONE_BIAS, &type, &timezone, sizeof(timezone), NULL);
    ExQueryNonVolatileSetting(XC_TZ_DLT_BIAS, &type, &daylight, sizeof(daylight), NULL);
    lt.QuadPart -= ((LONGLONG)(timezone + daylight) * 60 * 10000000);
    RtlTimeToTimeFields(&lt, &tf);
    return tf;
}