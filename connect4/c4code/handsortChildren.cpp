#include "connect4.h"

void ConnectFourGame::h_andSortChildStates(std::vector<State>& childStates, bool whiteturn) {

	//if parent node is black child will be white
	//want to sort from small to largest, since moves are positive
	if (whiteturn)
	{
		for (int i = 0; i < childStates.size(); i++)
		{
			childStates[i].RootMove = heuristic(childStates[i]);
		}
		std::sort(childStates.begin(), childStates.end());
		std::reverse(childStates.begin(), childStates.end());	//want largest to smallest 
	}
	else
	{
		for (int i = 0; i < childStates.size(); i++)
		{
			childStates[i].RootMove = heuristic(childStates[i]);
			
		}
		std::sort(childStates.begin(), childStates.end());
	}
}

