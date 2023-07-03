#include <sstream>
#include <list>
#include <iostream>
#include <stdlib.h>

#include "Piece.h"
#include "Board.h"
#include "Move.h"

int indexFromCoordinate(char file, int rank) {

	int f = toupper(file) - 65;
	int r = rank - 1;

	// Invalid
	if (r > BOARD_HEIGHT - 1 || r < 0 ||
		f > BOARD_WIDTH - 1 || f < 0) {
		return -1;
	}

	return (r * BOARD_WIDTH) + f;
}

int indexFromCoordinate(int file, int rank) {
	return (rank * BOARD_WIDTH) + file;
}

void clearMoveList(std::list<Move*>* moves) {

	// Free all elements and then the list
	while (!moves->empty()) {
		delete moves->front();
		moves->pop_front();
	}
}

Board::Board() {
	for (size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{
		if ((i + (i / BOARD_WIDTH)) % 2 == 0) {
			configuration[i] = Piece::None;
			continue;
		}

		if (i < BOARD_WIDTH * 3) {
			configuration[i] = Piece::Black;
		}
		else if (i > (BOARD_WIDTH * BOARD_HEIGHT) - (BOARD_HEIGHT * 3) - 1) {
			configuration[i] = Piece::White;
		}
		else {
			configuration[i] = Piece::None;
		}
	}

	whiteToPlay = true;

	hasCaptured = false;
	chainPiece = -1;
}

Board::Board(Board& board) {
	for (size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{
		configuration[i] = board.configuration[i];
	}

	whiteToPlay = board.whiteToPlay;

	hasCaptured = board.hasCaptured;
	chainPiece = board.chainPiece;
}

bool Board::getWhiteToPlay() {
	return whiteToPlay;
}

bool Board::gameOver() {
	// If there are no moves that can be made, the current player loses
	std::list<Move*>* moves = generateMoves(false);
	bool isOver = moves->size() == 0;
	clearMoveList(moves);
	delete moves;
	moves = NULL;

	return isOver;
}

bool Board::getHasCaptured() {
	return hasCaptured;
}

bool Board::getPieceColour(const Piece piece) {
	return (piece == Piece::White || piece == Piece::WhiteKing);
}

bool Board::isCoordOnBoard(int index) {
	return (index >= 0 && index < BOARD_WIDTH * BOARD_HEIGHT);
}

bool Board::isKing(const Piece piece) {
	return (piece == Piece::WhiteKing || piece == Piece::BlackKing);
}

bool Board::inKingZone(int index) {
	return ((index > BOARD_WIDTH * (BOARD_HEIGHT - 1)) ||
		index < BOARD_WIDTH);
}

int Board::getSquareColour(const int index) {
	int file = index & 7;
	int rank = index >> 3;
	return (file + rank) & 1;
}

bool Board::validateMove(const int to, const int from) {
	return (getSquareColour(to) == getSquareColour(from) &&
		configuration[to] == Piece::None);
}

void Board::removeNonCaptures(std::list<Move*>* moves) {
	std::list<Move*>::iterator i = moves->begin();
	while (i != moves->end()) {
		if ((*i)->capture == Piece::None) {
			delete (*i);
			i = moves->erase(i);
			continue;
		}
		i++;
	}
}

void Board::addMoves(std::list<Move*>* moves, const int pos, const int forward, const bool onlyCaptures) {
	for (const int direction : directions) {
		int to = pos + (direction * forward);

		if (!isCoordOnBoard(to) || !isCoordOnBoard(pos)) {
			continue;
		}

		// If a move was obstructed, check for a capture
		if (!validateMove(to, pos)) {

			// Cache before changing value of 'to'
			Piece capture = configuration[to];
			int capPos = to;

			// Hop the pieces for captures, hence direction * 2
			to = pos + (direction * 2 * forward);

			// Validate that the new 'to' lies on the board as well
			if (!isCoordOnBoard(to)) {
				continue;
			}

			if (!validateMove(to, pos) || capture == Piece::None ||
				getPieceColour(capture) == getPieceColour(configuration[pos]))
				continue;

			moves->push_back(new Move(to, pos, capture, capPos));
		}
		else {
			if (!onlyCaptures)
				moves->push_back(new Move(to, pos));
		}
	}
}

std::list<Move*>* Board::generateMoves(const bool onlyCaptures) {
	std::list<Move*>* moves = new std::list<Move*>;

	// Generate moves
	for (size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
	{
		if (configuration[i] == Piece::None ||
			getPieceColour(configuration[i]) != whiteToPlay)
			continue;

		if (onlyCaptures && i != chainPiece) {
			continue;
		}

		int forward = 1;
		if (getPieceColour(configuration[i])) {
			forward = -1;
		}

		addMoves(moves, i, forward, onlyCaptures);
		if (isKing(configuration[i]))
			addMoves(moves, i, -forward, onlyCaptures);
	}

	// Simplest was I could think of to ensure captures are made when they must be
	for (Move* move : *(moves)) {
		if (move->capture != Piece::None) {
			removeNonCaptures(moves);
			break;
		}
	}

	return moves;
}

// For the player, includes move validation
bool Board::makeMove(int to, int from) {
	std::list<Move*>* moves = generateMoves(hasCaptured);
	for (const Move* move : *(moves)) {
		if (move->to == to &&
			move->from == from) {
			if (inKingZone(move->to)) {
				configuration[to] = (getPieceColour(configuration[from]) ? Piece::WhiteKing : Piece::BlackKing);
			}
			else {
				configuration[to] = configuration[from];
			}
			configuration[from] = Piece::None;
			if (move->capture == Piece::None) {
				nextTurn();
			}
			else {
				configuration[move->capturePos] = Piece::None;

				hasCaptured = true;
				chainPiece = move->to;
			}

			clearMoveList(moves);
			delete moves;
			moves = NULL;

			return true;
		}
	}

	clearMoveList(moves);
	delete moves;
	moves = NULL;

	return false;
}

// For the bot, excludes move validation
void Board::makeMove(Move* move) {

	if (inKingZone(move->to)) {
		configuration[move->to] = (getPieceColour(configuration[move->from]) ? Piece::WhiteKing : Piece::BlackKing);
	}
	else {
		configuration[move->to] = configuration[move->from];
	}
	configuration[move->from] = Piece::None;
	if (move->capture == Piece::None) {
		nextTurn();
	}
	else {
		configuration[move->capturePos] = Piece::None;

		hasCaptured = true;
		chainPiece = move->to;
	}
}

void Board::nextTurn() {
	whiteToPlay = !whiteToPlay;

	hasCaptured = false;
	chainPiece = -1;
}

std::string Board::print() {
	std::ostringstream output;
	output << "\n    a b c d e f g h";
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
	{
		output << "\n " << y + 1 << " ";
		for (size_t x = 0; x < BOARD_WIDTH; x++)
		{
			Piece current = configuration[indexFromCoordinate((int)x, (int)y)];
			output << "|" << (getPieceColour(current) ? "\033[32m" : "\033[31m");
			output << pieceSymbols[(int)current];
			output << "\033[0m";
		}
		output << "|";
	}
	output << "\n";
	return output.str();
}