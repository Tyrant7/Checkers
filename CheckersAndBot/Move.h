#pragma once

struct Move {
	int to;
	int from;

	Piece capture;
	int capturePos;

	Move(int t, int f);
	Move(int t, int f, Piece cap, int capPos);
};