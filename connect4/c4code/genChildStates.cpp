#include "connect4.h"


void ConnectFourGame::genChildStates(int depth, std::vector<std::vector<int>> & vmoves, State &ParentState, std::vector<State> & childStates) {

	if (ParentState.getwhiteturn())
	{
		for (int piece = 0; piece < vmoves.size(); piece++)
		{
			for (int mv = 0; mv < vmoves[piece].size(); mv++)
			{
				//find position and direction of valid move
				int pos = ParentState.getwhitepiece(piece);
				int dir = vmoves[piece][mv];

				//create a temporary child state based on parent, has opposite move of parent
				State TmpState=ParentState;
				TmpState.updateStatePos(pos, dir, !(ParentState.getwhiteturn()));
	

				//don't want to repeat already visited game states, or boards visited along search path
				if (std::find(visitedboards.begin(), visitedboards.end(), TmpState.getboard()) != visitedboards.end() 
					|| std::find(searchedboards.begin(), searchedboards.end(), TmpState.getboard()) != searchedboards.end())
				{
					break;
				}
				
				//subsequent moves inherit this top move
				if (depth==0)
				{
					TmpState.RootMove.setdirection(dir);
					TmpState.RootMove.setpos(pos);
					TmpState.RootMove.sethval(0);
				}
			
				childStates.push_back(TmpState);
			}
		}
	}
	else
	{
		for (int piece = 0; piece < vmoves.size(); piece++)
		{
			for (int mv = 0; mv < vmoves[piece].size(); mv++)
			{
				int pos = ParentState.getblackpiece(piece);
				int dir = vmoves[piece][mv];

				//create a temporary child state based on parent, has opposite move of parent
				State TmpState = ParentState;
				TmpState.updateStatePos(pos, dir, !(ParentState.getwhiteturn()));

				//don't want to repeat already visited game states, or boards visited along search path
				if (std::find(searchedboards.begin(), searchedboards.end(), TmpState.getboard()) != searchedboards.end()
					|| std::find(visitedboards.begin(), visitedboards.end(), TmpState.getboard()) != visitedboards.end())
				{
					break;
				}

				//top root move only move info care about saving
				if (depth == 0)
				{
					TmpState.RootMove.setdirection(dir);
					TmpState.RootMove.setpos(pos);
					TmpState.RootMove.sethval(0);
				}
				//subsequent moves inherit this top move
				childStates.push_back(TmpState);
			}
		}
	}

}