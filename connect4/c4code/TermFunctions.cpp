/*******This file contains functions to find terminal state, return terminal value and intermediate functions*********/
#include "connect4.h"

//goal of heuristic is to find the smallest distance to line
Move  State::terminalValue() {
	//terminal value called on children who will have opposite turn as parent, but hold value of parent's move
	if (!(getwhiteturn())) 
	{
		RootMove.sethval(888888);
		return RootMove;
	}
	else {
		RootMove.sethval(-888888);
		return RootMove;
	}
}

//function takes a piece position, and direction and goes continues in a line until encountering neighbor
//or hitting an edge
int sameNeighborCheck(int pos, int direction, std::string &brd) {
	int neighborcount = 0;

	int checkpos = pos + direction;
	//handles extents of board
	while (checkpos < 49 && checkpos>0)
	{
		//handles west-most column jumping to east side
		//also handles west-most column checking east side on a nw_se check
		if (pos % 7 == 0 && direction < 0 || direction==6) {
			return neighborcount;
		}
		//handles east-most column jumping to west side
		//also handles east-most column checking west side on a nw_se check
		if ((pos + 1) % 7 == 0 && direction > 0 || direction==-6)
		{
			return neighborcount;
		}
		//continue checking as long as same piece type
		if (brd[pos] == brd[pos + direction])
		{
			neighborcount++;
			pos = checkpos;
			checkpos = pos + direction;
		}
		else {
			break;
		}
	}
	return neighborcount;
}

bool State::checkterminalState() {

	//if it is white's move, have to check that black didn't achieve terminal state
	if (!(getwhiteturn())) 
	{
		//for loop goes through each piece and checks directions for the number of same neighbors
		for (int i=0; i<whitepieces.size(); i++)
		{
			int eastwest = sameNeighborCheck(whitepieces[i], 1, getboard()) + sameNeighborCheck(whitepieces[i], -1, getboard());
			if (eastwest >= 3) return true;
			int northsouth = sameNeighborCheck(whitepieces[i], 7, getboard()) + sameNeighborCheck(whitepieces[i], -7, getboard());
			if (northsouth >= 3) return true;
			int ne_sw = sameNeighborCheck(whitepieces[i], 8, getboard()) + sameNeighborCheck(whitepieces[i], -8, getboard());
			if (ne_sw >= 3) return true;
			int nw_se = sameNeighborCheck(whitepieces[i], 6, getboard()) + sameNeighborCheck(whitepieces[i], -6, getboard());
			if (nw_se >= 3) return true;
		}
	}

	else
	{
		for (int i = 0; i < blackpieces.size(); i++)
		{
			int eastwest = sameNeighborCheck(blackpieces[i], 1, getboard()) + sameNeighborCheck(blackpieces[i], -1, getboard());
			if (eastwest >= 3) return true;
			int northsouth = sameNeighborCheck(blackpieces[i], 7, getboard()) + sameNeighborCheck(blackpieces[i], -7, getboard());
			if (northsouth >= 3) return true;
			int ne_sw = sameNeighborCheck(blackpieces[i], 8, getboard()) + sameNeighborCheck(blackpieces[i], -8, getboard());
			if (ne_sw >= 3) return true;
			int nw_se = sameNeighborCheck(blackpieces[i], 6, getboard()) + sameNeighborCheck(blackpieces[i], -6, getboard());
			if (nw_se >= 3) return true;
		}
	}
	return false;
}