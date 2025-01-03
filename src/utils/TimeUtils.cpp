#include "TimeUtils.h"


namespace TimeUtils {

    bool isFrameRipe(const unsigned long &nowInMicros, const unsigned long &previousInMicros, const unsigned long &frameRate) {
        return nowInMicros - previousInMicros >= (1000000.0 / frameRate);
    }

    bool isMillisElapsed(const unsigned long &nowInMillis, const unsigned long &previousInMillis, const unsigned long &durationMillis) {
        return nowInMillis - previousInMillis >= durationMillis;
    }

}