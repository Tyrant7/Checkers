#include <stdlib.h>
#include "Piece.h"
#include "Move.h"

Move::Move(int t, int f) {
	to = t;
	from = f;
	capture = Piece::None;
	capturePos = -1;
}

Move::Move(int t, int f, Piece cap, int capPos) {
	to = t;
	from = f;
	capture = cap;
	capturePos = capPos;
}