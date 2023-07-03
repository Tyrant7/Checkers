#include <list>
#include <cmath>
#include <iostream>

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "Bot.h"

int visitedStates;
int prunedBranches;

int evaluate(Board& board) {

	// Base case for game over
	if (board.gameOver()) {
		return (board.getWhiteToPlay() ? -WIN_SCORE : WIN_SCORE);
	}

	// Add up the material values of the pieces
	int score = 0;
	for (size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{
		score += pieceValues[(int)board.configuration[i]];
	}
	return score;
}

int negamax(Board& board, const int depth, int alpha, int beta, int colour) {

	++visitedStates;

	// Terminal node, return evaluation
	if (depth <= 0 || board.gameOver()) {
		return colour * evaluate(board);
	}

	// If there are no additional captures to be made, pass on to opponent's turn
	std::list<Move*>* moves = board.generateMoves(board.getHasCaptured());
	if (board.getHasCaptured() && moves->empty()) {

		delete moves;
		moves = NULL;

		board.nextTurn();
		return -negamax(board, depth - 1, -beta, -alpha, -colour);
	}

	// Otherwise, play moves as normal
	int score = MIN_SCORE;
	while (!moves->empty()) {
		Board* newBoard = new Board(board);
		newBoard->makeMove(moves->front());

		// Must deallocate moves dynamically instead of at the end to avoid piling up tons of move objects in memory
		delete moves->front();
		moves->pop_front();

		if (board.getWhiteToPlay() == newBoard->getWhiteToPlay()) {
			score = std::max(score, negamax(*newBoard, depth - 1, alpha, beta, colour));
		}
		else {
			score = std::max(score, -negamax(*newBoard, depth - 1, -beta, -alpha, -colour));
		}

		delete newBoard;
		newBoard = NULL;

		alpha = std::max(alpha, score);
		if (alpha >= beta) {
			++prunedBranches;
			break;
		}
	}

	// Clear all moves that weren't removed before breaking
	clearMoveList(moves);
	delete moves;
	moves = NULL;

	return score;
}


Move* findBestMove(Board& board) {
	std::list<Move*>* moves = board.generateMoves(board.getHasCaptured());
	visitedStates = 0;
	prunedBranches = 0;

	int colour = (board.getWhiteToPlay() ? 1 : -1);

	int bestScore = MIN_SCORE;
	Move* bestMove = NULL;
	while (!moves->empty()) {
		Board* newBoard = new Board(board);
		newBoard->makeMove(moves->front());

		int score;
		if (board.getWhiteToPlay() == newBoard->getWhiteToPlay()) {
			score = negamax(*newBoard, evalDepth - 1, MIN_SCORE, -MIN_SCORE, colour);
		}
		else {
			score = -negamax(*newBoard, evalDepth - 1, MIN_SCORE, -MIN_SCORE, -colour);
		}

		if (score > bestScore) {
			bestScore = score;
			bestMove = moves->front();
		}
		else {
			delete moves->front();
		}
		moves->pop_front();

		delete newBoard;
		newBoard = NULL;
	}

	delete moves;
	moves = NULL;

	std::cout << "Visited " << visitedStates << " states and pruned " << prunedBranches << " branches.\n";

	return bestMove;
}
