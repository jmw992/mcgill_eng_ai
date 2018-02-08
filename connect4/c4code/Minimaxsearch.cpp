#include "connect4.h"

/*create set of permissible actions
create children, based on permissible actions update boards, white / black piece locations
sort children by heuristic value
alpha beta search
model for Minimax search found on wikipedia
https://en.wikipedia.org/wiki/Minimax
*/

Move ConnectFourGame::minimax(State &SearchState, int depth) {

	searchedboards.push_back(SearchState.getboard());

	if (depth >= c4maxdepth || SearchState.getterminalstate()) {
		searchedboards.pop_back();
		return SearchState.RootMove; //heuristic already calculated in child in h&sort
	}


	std::vector<State> childStates;

	//Generating Children for white and black pieces
	if (SearchState.getwhiteturn())
	{
		std::vector<std::vector<int>> validMoves;
		genValidMoves(SearchState.getboard(), SearchState.getwpieces(), validMoves);
		genChildStates(depth, validMoves, SearchState, childStates);
	}
	else
	{
		std::vector<std::vector<int>> validMoves;
		genValidMoves(SearchState.getboard(), SearchState.getbpieces(), validMoves);
		genChildStates(depth, validMoves, SearchState, childStates);
	}
	//calculates the heuristic and sorts the children by h value to try and greedy search to maximize pruning
	h_andSortChildStates(childStates, SearchState.getwhiteturn());

	//Maximize value
	if (SearchState.getwhiteturn())  //child will have turn to parent
	{
		Move BestV = SearchState.RootMove;
		BestV.sethval(-999999);
		for (int i = 0; i < childStates.size(); i++) {
			Move V = minimax(childStates[i], (1 + depth));

			if (V.gethval() > BestV.gethval() ) 
				BestV = V;


		}
		searchedboards.pop_back();
		return BestV;
	}
	else {
		Move BestV = SearchState.RootMove;
		BestV.sethval(999999);
		for (int i = 0; i < childStates.size(); i++) {
			Move V = minimax(childStates[i], (1 + depth));

			if (V.gethval() < BestV.gethval() ) 
				BestV = V;
		}
		searchedboards.pop_back();
		return BestV;
	}
}