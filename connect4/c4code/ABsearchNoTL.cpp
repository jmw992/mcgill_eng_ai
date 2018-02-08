#include "connect4.h"

/*create set of permissible actions
create children, based on permissible actions update boards, white / black piece locations
sort children by heuristic value
alpha beta search
model for alpha beta pruning found on wikipedia
https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
*/

Move  ConnectFourGame::absearchNoTL(State &SearchState, int depth, Move Alpha, Move Beta) {
	//update searched board list
	searchedboardcount += 1;
	searchedboards.push_back(SearchState.getboard());

	if (depth >= c4maxdepth || SearchState.getterminalstate() ) {
		searchedboards.pop_back();
		return SearchState.RootMove; //heuristic already calculated in child in handsort
	}

	std::vector<State> childStates;

	//Generating Children when white turn
	if (SearchState.getwhiteturn())
	{
		//vector holds piece position and valid directions [pos][direction]
		std::vector<std::vector<int>> validMoves;
		genValidMoves(SearchState.getboard(), SearchState.getwpieces(), validMoves);
		genChildStates(depth, validMoves, SearchState, childStates);
	}
	//Generating Children when black turn
	else
	{
		//vector holds piece position and valid directions [pos][direction]
		std::vector<std::vector<int>> validMoves;
		genValidMoves(SearchState.getboard(), SearchState.getbpieces(), validMoves);
		genChildStates(depth, validMoves, SearchState, childStates);
	}

	//calculates the heuristic and sorts the children by h value to try to maximize pruning
	h_andSortChildStates(childStates, SearchState.getwhiteturn());


	//Alpha Beta maximizer
	if (SearchState.getwhiteturn())
	{
		//inheriting move info from initial root move
		Move V = SearchState.RootMove;
		V.sethval(-999999);
		for (int i = 0; i < childStates.size(); i++) {



			//V = max(absearch, beta)
			Move TmpMv = absearchNoTL(childStates[i], (1 + depth), Alpha, Beta);	//child state already fed opposite turn
			if (TmpMv.gethval() > Alpha.gethval()) V = TmpMv;
			else V = Alpha;

			//Alpha = Max (alpha, V)
			if (V.gethval() > Alpha.gethval()) Alpha = V;


			if (Beta.gethval() <= Alpha.gethval())
			{
				break;
			}
		}
		//updated searched board list
		searchedboards.pop_back();
		return V;
	}
	//Minimizer
	else
	{
		Move V = SearchState.RootMove;
		V.sethval(999999);
		for (int i = 0; i < childStates.size(); i++) {

			//V = min(absearch, beta)
			Move TmpMv = absearchNoTL(childStates[i], (1 + depth), Alpha, Beta);	//child state already fed opposite turn
			if (TmpMv.gethval() < Beta.gethval()) V = TmpMv;
			else V = Beta;

			//Beta = min (beta, V)
			if (V.gethval() < Beta.gethval()) Beta = V;

			if (Beta.gethval() <= Alpha.gethval())
			{
				break;
			}
		}
		//updated searched board list
		searchedboards.pop_back();
		return V;
	}
}

