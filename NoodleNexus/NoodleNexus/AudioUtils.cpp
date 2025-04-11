#include "AudioUtils.hpp"
#include <cstdio>

namespace audio {

    void CheckError(FMOD_RESULT result, const char* file, int line) {
        if (result != FMOD_OK) {
            fprintf(stderr, "FMOD error %d at %s:%d\n", result, file, line);
        }
    }

}
