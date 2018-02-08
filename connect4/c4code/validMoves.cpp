#include "connect4.h"

bool ConnectFourGame::validMove(std::string &brdstring, int pos, int direction) {

		int newpos = direction + pos;

		//handles west-most column jumping to eastside
		if (pos % 7 == 0 && direction == -1) {
			return false;
		}
		//handles east-most column jumping to west side
		if ((pos+1)%7==0 && direction==1)
		{
			return false;
		}
		//handles going out of bounds north-south and taking occupied space
		if (newpos < 49 && newpos>0) {
			if(brdstring[newpos] == ' ')
				return true;
		}
		return false;
}

void ConnectFourGame::genValidMoves(std::string &brdstring, std::vector<int> & pieces, std::vector<std::vector<int>> &vmoves) {
	int directions[4] = { 1,-1,7,-7 }; //east west, north, south
	vmoves.resize(6);
	for (int i = 0; i < 6; i++) {
		for (int j=0; j< 4; j++){
			//std::cout << i << ' ' << j << ' ' << directions[j] << '\n';
			if (validMove(brdstring, pieces[i], directions[j]))
			{
				//std::cout <<"Valid Move"<< i << ' '<< j << ' ' << directions[j] << '\n';
				vmoves[i].push_back(directions[j]);
			}
		}
	}
	return;
}