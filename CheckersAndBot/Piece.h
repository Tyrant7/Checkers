#pragma once

enum class Piece {
	None, White, Black, WhiteKing, BlackKing
};

const char pieceSymbols[] = { ' ', 'O', 'o', 'K', 'k' };
const int pieceValues[] = { 0, 100, -100, 300, -300 };