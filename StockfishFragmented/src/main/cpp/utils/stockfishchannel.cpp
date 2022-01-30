/**
 * Laurent Bernabe - 2022
 * Adapted code from Stockfish 14 uci.cpp and from ucioption.cpp source code.
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>

#include "../stockfish/bitboard.h"
#include "../stockfish/endgame.h"
#include "../stockfish/position.h"
#include "../stockfish/psqt.h"
#include "../stockfish/search.h"
#include "../stockfish/syzygy/tbprobe.h"
#include "../stockfish/thread.h"
#include "../stockfish/tt.h"
#include "../stockfish/evaluate.h"
#include "../stockfish/movegen.h"
#include "../stockfish/timeman.h"
#include "stockfishchannel.h"

namespace loloof64 {
    LockedStringQueue outputs;

    const char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    StockfishChannel::StockfishChannel(): pos(Stockfish::Position()),
    states(std::make_unique<std::deque<Stockfish::StateInfo>>(std::deque<Stockfish::StateInfo>(1))){
        using namespace Stockfish;
        
        pos.set(StartFEN, false, &states->back(), nullptr);

        UCI::init(Stockfish::Options);
        Tune::init();
        PSQT::init();
        Bitboards::init();
        Position::init();
        Bitbases::init();
        Endgames::init();
        Threads.set(size_t(Stockfish::Options["Threads"]));
        Search::clear(); // After threads are up
        Eval::NNUE::init();

        states = std::make_unique<std::deque<StateInfo>>(std::deque<StateInfo>(1));

        //------- temporary -----------------------------
        outputs.push("Set up a new instance of StockfishChannel.");
        //------- temporary (end of section) ------------
    }

    StockfishChannel::~StockfishChannel() {
        Stockfish::Threads.set(0);
        //------- temporary -----------------------------
        outputs.push("Releasing an instance of StockfishChannel.");
        //------- temporary (end of section) ------------
    }

    void StockfishChannel::newGame() {
        Stockfish::Search::clear();
    }

    void StockfishChannel::setPositionFromStart(std::vector<std::string> moves) {
        setPositionFEN(StartFEN, moves);
    }

    void StockfishChannel::setPosition(std::string positionFen, std::vector<std::string> moves) {
        setPositionFEN(positionFen, moves);
    }

    void StockfishChannel::setPositionFEN(std::string positionFen, std::vector<std::string> moves) {
        states = std::make_unique<std::deque<Stockfish::StateInfo>>(std::deque<Stockfish::StateInfo>(1)); // Drop old and create a new one
        pos.set(positionFen, Stockfish::Options["UCI_Chess960"], &states->back(), Stockfish::Threads.main());

        for (std::string currentMove: moves) {
            auto uciMove = Stockfish::UCI::to_move(pos, currentMove);
            if (uciMove == Stockfish::MOVE_NONE) break;
            states->emplace_back();
            pos.do_move(uciMove, states->back());
        }
    }

    void StockfishChannel::go(std::vector<std::string> goCommandOptions) {
        Stockfish::Search::LimitsType limits;
        limits.startTime = Stockfish::now(); // As early as possible!
        bool ponderMode = false;

        for (std::string currentOption: goCommandOptions) {
            std::stringstream ss(currentOption);
            std::string token;
            ss >> token;

            if (token == "searchmoves") // Needs to be the last command on the line
                while (ss >> token)
                    limits.searchmoves.push_back(Stockfish::UCI::to_move(pos, token));

            else if (token == "wtime")     ss >> limits.time[Stockfish::WHITE];
            else if (token == "btime")     ss >> limits.time[Stockfish::BLACK];
            else if (token == "winc")      ss >> limits.inc[Stockfish::WHITE];
            else if (token == "binc")      ss >> limits.inc[Stockfish::BLACK];
            else if (token == "movestogo") ss >> limits.movestogo;
            else if (token == "depth")     ss >> limits.depth;
            else if (token == "nodes")     ss >> limits.nodes;
            else if (token == "movetime")  ss >> limits.movetime;
            else if (token == "mate")      ss >> limits.mate;
            else if (token == "perft")     ss >> limits.perft;
            else if (token == "infinite")  limits.infinite = 1;
            else if (token == "ponder")    ponderMode = true;

            Stockfish::Threads.start_thinking(pos, states, limits, ponderMode);
        }
    }

    std::vector<std::string> StockfishChannel::getEngineOptions() {

        std::vector<std::string> result;

        for (size_t idx = 0; idx < Stockfish::Options.size(); ++idx)
            for (const auto& it : Stockfish::Options)
                if (it.second.getIdx() == idx)
                {
                    const Stockfish::UCI::Option& o = it.second;
                    std::stringstream line;
                    line << "option name " << it.first << " type " << o.getType();

                    if (o.getType() == "string" || o.getType() == "check" || o.getType() == "combo")
                        line << " default " << o.getDefaultValue();

                    if (o.getType() == "spin")
                        line << " default " << int(stof(o.getDefaultValue()))
                           << " min "     << o.getMin()
                           << " max "     << o.getMax();

                    result.push_back(line.str());

                    break;
                }

        return result;
    }

    void StockfishChannel::setEngineOption(std::string name, std::string value) {
        if (Stockfish::Options.count(name))
            Stockfish::Options[name] = value;
        else {
            std::stringstream msg;
            msg << "No such option: " << name;
            outputs.push(msg.str());
        }
    }

    StockfishChannel::StockfishChannel(StockfishChannel const &channel) {
        *states = *channel.states;
        pos.set(channel.pos.fen(), channel.pos.is_chess960(), &(states->back()), channel.pos.this_thread());
    }
}