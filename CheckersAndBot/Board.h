#pragma once

#include <list>
#include <stdlib.h>
#include <string>

#include "Piece.h"
#include "Move.h"

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

const int directions[] = { 9, 7 };

int indexFromCoordinate(char file, int rank);
int indexFromCoordinate(int file, int rank);

void clearMoveList(std::list<Move*>* moves);

class Board {
private:
	bool whiteToPlay;

	bool hasCaptured;
	int chainPiece;

public:
	Piece configuration[BOARD_WIDTH * BOARD_HEIGHT];

	Board();
	Board(Board& board);

	bool getWhiteToPlay();
	bool gameOver();
	bool getHasCaptured();

	static bool getPieceColour(const Piece piece);
	static bool isKing(const Piece piece);
	static bool inKingZone(int index);
	static bool isCoordOnBoard(int index);
	static int getSquareColour(const int index);

	bool validateMove(const int to, const int from);
	void removeNonCaptures(std::list<Move*>* moves);
	void addMoves(std::list<Move*>* moves, const int pos, const int forward, const bool onlyCaptures);
	std::list<Move*>* generateMoves(const bool onlyCaptures = false);

	bool makeMove(int to, int from);
	void makeMove(Move* move);
	void nextTurn();

	std::string print();
};