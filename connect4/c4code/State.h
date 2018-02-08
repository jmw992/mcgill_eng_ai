#include <string>
#include <vector>

class State {
public:


	State() {}

	State(std::string &brd, bool white) {
		board = brd;
		whiteturn = white;
		std::vector<int> pieces;
		pieces.resize(6);
		int piecenum = 0;
		for (int i = 0; i < brd.size(); i++)
		{
			if (brd[i] == 'O')
			{
				pieces[piecenum] = i;
				piecenum++;
			}
		}
		whitepieces = pieces;
		piecenum = 0;
		for (int i = 0; i < brd.size(); i++)
		{
			if (brd[i] == 'X')
			{
				pieces[piecenum] = i;
				piecenum++;
			}
		}
		blackpieces = pieces;
	}

	State(std::string &brd, std::vector<int> &blkp, std::vector<int> &whitep, bool &white, Move PrecM) {
		RootMove = PrecM;
		board = brd;
		whiteturn = white;
		blackpieces = blkp;
		whitepieces = whitep;

	}
	//getters
	std::string getboard() { return board; }
	std::vector<int> getbpieces() { return blackpieces; }
	std::vector<int> getwpieces() { return whitepieces; }
	int getblackpiece(int i) { return blackpieces[i]; }
	int getwhitepiece(int i) { return whitepieces[i]; }
	bool getwhiteturn() { return whiteturn; }
	bool getterminalstate() { return terminalstate; }

	//setters
	void setwhiteturn(bool white) { whiteturn = white; }
	void setterminalstate(bool term) { terminalstate = term; }

	//Functions
	void updateStateString(std::string mv, bool white);
	void updateStateInt(int row, int clm, int direction, bool white);
	void printStateLocal();
	void updateStatePos(int pos, int direction, bool white);

	//Functions
	bool checkterminalState();
	Move terminalValue();

	//std::string movetostring(int row, int clm, int direction);
	std::string movePostoString(int pos, int direction);

	char directionInttoChar(int direction);
	char directionChartoInt(char direction);

	bool operator < (State & other)  //(1)
	{
		return RootMove.gethval() < (other.RootMove.gethval());
	}
	Move RootMove;
private:

	/**Variables**/
	std::string board;				//list of the board states
	bool whiteturn;					//indicates who will make the next move
	bool terminalstate = false;		//indicates whether a terminal state has been achieved, sacrifices memory for computation
	std::vector<int> whitepieces;
	std::vector<int> blackpieces;



	int  rowclmtopos(int row, int clm) {
		return row * 7 + clm;
	}
	int postorow(int pos) {
		return int(pos / 7);
	}
	int postoclm(int pos) {
		return pos % 7;
	}
};
