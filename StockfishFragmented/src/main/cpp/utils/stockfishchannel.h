/**
 * Laurent Bernabe - 2022
 */

#ifndef __STOCKFISH_CHANNEL_H
#define __STOCKFISH_CHANNEL_H

#include <vector>
#include <string.h>
#include <memory>

#include "sharedioqueues.h"
#include "../stockfish/types.h"
#include "../stockfish/position.h"
#include "../stockfish/uci.h"

namespace loloof64 {

    class StockfishChannel {
    public:
        StockfishChannel();

        StockfishChannel(StockfishChannel const &channel);

        ~StockfishChannel();

        void newGame();
        void setPositionFromStart(std::vector<std::string> moves);
        void setPosition(std::string positionFen, std::vector<std::string> moves);
        void go(std::vector<std::string> goCommandOptions);

        std::vector<std::string> getEngineOptions();
        void setEngineOption(std::string name, std::string value);

    private:
        void setPositionFEN(std::string positionFen, std::vector<std::string> moves);
        Stockfish::Position pos;
        Stockfish::StateListPtr states;
    };
}

#endif