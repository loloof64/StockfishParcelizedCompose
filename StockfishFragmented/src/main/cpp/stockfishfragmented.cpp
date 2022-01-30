#include <jni.h>
#include <string>
#include "utils/stockfishchannel.h"
#include "utils/jniutils.h"
#include <jni.h>

loloof64::StockfishChannel getEngineChannel() {
    static loloof64::StockfishChannel engineChannel;
    return engineChannel;
}

/*
 * TODO implement :
 *
        void newGame();
        void setPositionFromStart(std::vector<std::string> moves, Position& pos, StateListPtr& states);
        void setPosition(std::string positionFen, std::vector<std::string> moves, Position& pos, StateListPtr& states);
        void go(std::vector<std::string> goCommandOptions, Position& pos, StateListPtr& states);

        std::vector<std::string> getEngineOptions();
        void setEngineOption(std::string name, std::string value);
 */

extern "C" JNIEXPORT void JNICALL Java_com_loloof64_stockfishfragmented_NativeLib_newGame(
        JNIEnv* env,
        jobject /* this */) {
    getEngineChannel().newGame();
}

extern "C" JNIEXPORT void JNICALL Java_com_loloof64_stockfishfragmented_NativeLib_setPositionFromStart(
        JNIEnv* env,
        jobject /* this */,
        jobjectArray moves
        ) {

}