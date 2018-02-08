#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <time.h>

#include "Move.h"
#include "State.h"

class ConnectFourGame {
public:
	ConnectFourGame() {}
	ConnectFourGame( std::string initialboard, int mxdepth) {}

	State CurrentState;								//current state				
	Move SelectedMove;
	
	time_t starttime;
	int timelimit;									//should be number of seconds

	long unsigned int searchedboardcount=0;
	std::vector<std::string> visitedboards;			//list of visited states, prevent circling, used map for built in hash, bool lowest memory value option
	std::vector<std::string> searchedboards;		//list of searched boards re-initialized in each state
	

	/****Search Functions****/
	Move absearchTimeLimit(State &SearchState, int depth, Move Alpha, Move Beta);		//ALPHA-BETA SEARCH w/ time limit
	Move absearchNoTL(State &SearchState, int depth, Move Alpha, Move Beta);			//ALPHA-BETA SEARCH
	Move minimax(State &SearchState, int depth);										//Minimax

	/**Validity Functions**/
	bool validMove(std::string &brdstring, int pos, int direction);
	//modifies vector vmoves
	void genValidMoves(std::string &brdstring, std::vector<int> & pieces, std::vector<std::vector<int>> &vmoves);
	void genChildStates(int depth, std::vector<std::vector<int>> & vmoves, State &ParentState, std::vector<State> & ChildStates);

	Move heuristic(State &TestState);								//utility function
	Move heuristicFourClosest(State &TestState);					//utility function, not used
	void h_andSortChildStates(std::vector<State>& childStatesbool, bool whiteturn);
	void printSelectedMove();

	int c4maxdepth;				//max depth of search tree

private:
	int rowclmtopos(int row, int clm) {
		return row * 7 + clm;
	}
	int postorow(int pos) {
		return int(pos / 7);
	}
	int postoclm(int pos) {
		return pos % 7;
	}
};







