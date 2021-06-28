#pragma once
class AccumMove
{
public:
	int idPlayer = 0;
	int idMove = 0;
	int x = 0;
	int y = 0;


	AccumMove() {};
	AccumMove(int _idPlayer, int _idMove, int _x, int _y) {
		idPlayer = _idPlayer;
		idMove = _idMove;
		x = _x;
		y = _y;
	};
};

