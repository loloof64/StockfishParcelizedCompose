/**
 * Laurent Bernabe - 2022
 */

#include <jni.h>
#include <vector>
#include <string>

namespace loloof64 {

    /*
     * Adapted from https://stackoverflow.com/a/19592062/662618
     */
    std::vector<std::string> nativeStringsArrayToStlVector(
            JNIEnv* env,
        jobject /* this */,
        jobjectArray stringsArray) {
        std::vector<std::string> result;

        int size = env->GetArrayLength(stringsArray);

        for (int i=0; i < size; ++i)
        {
            jstring currentNativeString = (jstring) env->GetObjectArrayElement(stringsArray, i);
            const char* currentCString = env->GetStringUTFChars(currentNativeString, 0);
            std::string currentSTLString(currentCString);
            result.push_back(currentSTLString);
            env->ReleaseStringUTFChars(currentNativeString, currentCString);
            env->DeleteLocalRef(currentNativeString);
        }

        return result;
    }

    jobjectArray stlStringsVectorToNativeStringsArray(
        JNIEnv* env,
        jobject /* this */,
        std::vector<std::string> inputVector
            ) {
        jobjectArray result;
        int arraySize = inputVector.size();
        result = (jobjectArray) env->NewObjectArray(arraySize,
                                                    env->FindClass("java/lang/String"),
                                                    NULL);
        for (int i = 0; i < arraySize; i++) {
            env->SetObjectArrayElement(result, i, env->NewStringUTF(inputVector[i].c_str()));
        }

        return result;
    }
}