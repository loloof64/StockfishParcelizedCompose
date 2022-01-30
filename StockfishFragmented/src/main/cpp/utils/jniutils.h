/**
 * Laurent Bernabe - 2022
 */

#ifndef __JNI_UTILS_H
#define __JNI_UTILS_H

#include <jni.h>
#include <vector>
#include <string>

namespace loloof64 {
    std::vector<std::string> nativeStringsArrayToStlVector(jobjectArray stringsArray);
}

#endif