#include "connect4.h"
//goal of heuristic is to find the smallest distance to line for the 4 closest pieces
//function will iterate through each line, and calculate the min distance for each piece and place in a vector
//will sort the vector and find which are the closest 4
//28 best fit lines to check

/***function to calculate minimum manhattan distance to a line***/
int manhattandistPointLine4closest(int linestartpos, int linestep, int piecepos) {
	/*  The manhattan distance between a point of coordinates (x_0,y_0) and a line of equation  a*x+b*y+c=0 is given by :
	abs(ax_0+bx_0+d)/Max(abs(a),abs(b))
	http://artis.imag.fr/~Xavier.Decoret/resources/maths/manhattan/html/
	will use
	y=m*x+d
	so
	a=-m, b=1, c=-d
	*/
	int linex1 = linestartpos / 7;
	int linex2 = (linestartpos + linestep) / 7;
	int liney1 = linestartpos % 7;
	int liney2 = (linestartpos + linestep) % 7;


	int m = (liney1 - liney2) / (linex1 - linex2);
	int d = liney1 - m*linex1;

	int px = piecepos / 7;
	int py = piecepos % 7;

	int abmax = abs(1);

	if (abs(1)>abs(m))
	{
		abmax = abs(m);
	}
	return abs(-m*px + py - d) / abmax;
}

Move  ConnectFourGame::heuristicFourClosest(State & SearchState) {
	//makes sure terminal states receive proper terminal value & gain priority in heuristic
	SearchState.setterminalstate(SearchState.checkterminalState());
	if (SearchState.getterminalstate())
	{

		return SearchState.terminalValue();
	}


	std::vector<int> piece_distances;
	piece_distances.resize(6);
	int mindistance4closest = 888888;
	int bestline = 0;
	//if it's whites turn to move have to apply evaluation function on black's move
	if (!(SearchState.getwhiteturn()))
	{
		/***checks distance to rows**/
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = abs(i - SearchState.getwhitepiece(j) / 7);
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
				bestline = 1;
			}
		}


		/***checks distance to columns**/
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = abs(i - SearchState.getwhitepiece(j) % 7);
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
				bestline = 2;
			}
		}

		/**loop through NW-SE diagonals**/
		int nwsediagpositions[] = { 0,1,2,3,7,14,21 };
		for (int diag = 0; diag < 7; diag++) {
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = manhattandistPointLine4closest(nwsediagpositions[diag], 8, SearchState.getwhitepiece(j));
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
				bestline = 3;
			}
		}

		/***loop through NE-SW diagonals****/
		int neswdiagpositions[] = { 3,4,5,6,13,20,27 };
		for (int diag = 0; diag < 7; diag++) {
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = manhattandistPointLine4closest(neswdiagpositions[diag], 6, SearchState.getwhitepiece(j));
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
				bestline = 4;
			}
		}
		SearchState.RootMove.sethval(1000 - (mindistance4closest));

		return SearchState.RootMove;
	}
	else
	{
		/***checks distance to rows**/
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = abs(i - SearchState.getblackpiece(j) / 7);
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
			}
		}


		/***checks distance to columns**/
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = abs(i - SearchState.getblackpiece(j) % 7);
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
			}
		}

		/**loop through NW-SE diagonals**/
		int nwsediagpositions[] = { 0,1,2,3,7,14,21 };
		for (int diag = 0; diag < 7; diag++) {
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = manhattandistPointLine4closest(nwsediagpositions[diag], 8, SearchState.getblackpiece(j));
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
			}
		}

		/***loop through NE-SW diagonals****/
		int neswdiagpositions[] = { 3,4,5,6,13,20,27 };
		for (int diag = 0; diag < 7; diag++) {
			for (int j = 0; j < piece_distances.size(); j++)
			{
				piece_distances[j] = manhattandistPointLine4closest(neswdiagpositions[diag], 6, SearchState.getblackpiece(j));
			}

			int dist_to_line = 0;
			std::sort(piece_distances.begin(), piece_distances.end());
			for (int k = 0; k < piece_distances.size()-2; k++)  //care about the 4 closest pieces
			{
				dist_to_line += piece_distances[k];
			}
			if (mindistance4closest > dist_to_line)
			{
				mindistance4closest = dist_to_line;
			}
		}
		SearchState.RootMove.sethval(-1000 + (mindistance4closest));
		return SearchState.RootMove;
	}

	return SearchState.RootMove;
}