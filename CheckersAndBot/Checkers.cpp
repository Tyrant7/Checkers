#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "Piece.h"
#include "Board.h"
#include "Move.h"
#include "Bot.h"

using namespace std;

int main() {

	cout << "Welcome to checkers!\nType the file and rank you wish to play in. E.x. a1 b2 would mean playing from a1 to b2.\n";
	cout << "If a capture is available, it MUST be made. If multiple captures are available, then you may choose which one to perform.\n";
	cout << "After capturing, it will be your turn again, however only additional captures may be made.\n";

	cout << "If you would like to play against another player, input the word \"player\".\n";
	cout << "If you would like to play against a bot, input the word \"bot\".\n";

	string inWord;
	bool againstBot = false;
	while (true) {
		cin >> inWord;
		if (inWord == "player") {
			break;
		}
		else if (inWord == "bot") {
			againstBot = true;
			break;
		}

		cout << "Unrecognized game type. Please enter \"bot\" or \"player\".\n";
	}

	Board board;
	cout << board.print();

	char inFile;
	int inRank;

	while (!board.gameOver()) {

		// Skip if there are no additional captures to be made
		list<Move*>* moves = board.generateMoves(true);
		if (board.getHasCaptured() && moves->size() == 0) {
			cout << "No additional captures can be made.\n";
			board.nextTurn();
			continue;
		}
		clearMoveList(moves);
		delete moves;
		moves = NULL;

		// Bot
		bool whiteToPlay = board.getWhiteToPlay();
		if (againstBot && !whiteToPlay) {
			Move* move = findBestMove(board);
			if (move == NULL) {
				cout << "Bot could not find a valid move.\n";
				break;
			}
			board.makeMove(move);

			delete move;
			move = NULL;
		}
		// Player
		else {
			cout << endl;
			cout << (whiteToPlay ? "Green's turn!\n" : "Red's turn!\n");

			cin >> inFile;
			cin >> inRank;
			int from = indexFromCoordinate(inFile, inRank);

			cin >> inFile;
			cin >> inRank;
			int to = indexFromCoordinate(inFile, inRank);

			if (!board.makeMove(to, from)) {
				cout << "Invalid move entered!\n";
				continue;
			}
		}

		cout << board.print();
	}

	cout << (board.getWhiteToPlay() ? "Red Wins!\n" : "Green Wins!\n");
	return 0;
}