#pragma once

namespace TimeUtils {

    bool isFrameRipe(const unsigned long &nowInMicros, const unsigned long &previousInMicros, const unsigned long &frameRate);
    bool isMillisElapsed(const unsigned long &nowInMillis, const unsigned long &previousInMillis, const unsigned long &durationMillis);

}