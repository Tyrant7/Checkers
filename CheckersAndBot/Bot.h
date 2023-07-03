#pragma once

#include <list>

#include "Board.h"
#include "Move.h"

#define MIN_SCORE -1000000
#define WIN_SCORE 99999

const int evalDepth = 10;

int evaluate(Board& board);

int negamax(Board& board, int depth, int alpha, int beta, int colour);
Move* findBestMove(Board& board);