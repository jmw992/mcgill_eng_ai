#include "connect4.h"

void boardinput(std::string &initboard, std::string &inputboard) {

	for (int i = 0; i < inputboard.size(); i++) {
		if (inputboard[i]!=','&& inputboard[i]!='\n')
		{
			initboard.append(1, inputboard[i]);  
		}
	}
}

int main() {
	//inputs to draw from
	std::string exa(" , , , , , ,  , , , , , , O, ,X, , , ,  , , ,O, , ,X , , , ,O,X,X , ,O, , ,O,X , , ,X,O, , ");
	std::string exb("O,O, , , , , ,X, , , , , , , , ,X, , ,O, , , ,X,O, , ,X, , , , ,O,X, , ,X, , , , , ,O, , , , , , ,");
	std::string exc(" , ,O, ,X, , , , ,O, ,X, , , , ,X, ,O, , , , ,X, ,O, , , , , , , , , , , ,O, ,X, , , , ,X, ,O, , ");
	std::string ext("O,O,O,O, , ,  ,X,X,X,X, ,  , , , , , ,  , , , , , ,  , , , , , ,  , , , , , ,  , ,X,O,O,X, ");
	std::string diag1("X, , , , , , ,O,X, , , , , ,O, ,X,O,O, , ,O, , ,X, , , ,X,X, ,O, , , , , , , , , , , , , , , , , ,");
	std::string diag2(" , , , ,X, , , , , ,X,O,O,O, , ,X, , , , , ,X,O,X,O,X, , , , , , , , , , , , , , , , , , , , , , ,");
	std::string tournament(" , , , , , ,X,X, , , , , ,O,O, , , , , ,X,X, , , , , ,O,O, , , , , ,X,X, , , , , ,O,O, , , , , , ");
	std::string tstterminal("O,O, , , , , , ,O, , , , , , , ,X, , , , , ,X,X,O,X, , ,X, , , ,O,O,X, , , , , , , , , , , , , , ,");

	//setting the initial board
	std::string initialboard;
	
	boardinput(initialboard, exa); //<---------------Select Board Here
	
	//create the initial state of the board with a board string, and whose turn it is
	//white turn =true, black turn =false
	State  TmpState(initialboard, true);

	ConnectFourGame TestGame;

	TestGame.CurrentState=TmpState;
	//specify agent search depth and/or time limit
	TestGame.c4maxdepth = 5;		//<---------------Select Time Max Depth Here
	TestGame.timelimit = 20;		//<---------------Select Time Limit Here

	int movecount = 0;  

	std::cout << "The following game uses a search depth of " << TestGame.c4maxdepth<<"\n";
	std::cout << "Initial Board " << "\n";
	TestGame.CurrentState.printStateLocal();

	//Terminal state check only checks the pieces of the prior move,
	//this checks for both white and black pieces for terminal state in case fed finished board
	TestGame.CurrentState.setterminalstate(TestGame.CurrentState.checkterminalState());
	TestGame.CurrentState.setwhiteturn(!(TmpState.getwhiteturn()));
	if (!(TestGame.CurrentState.getterminalstate()))
	{
		TestGame.CurrentState.setterminalstate(TestGame.CurrentState.checkterminalState());
	}
	//resetting proper turn
	TestGame.CurrentState.setwhiteturn(!(TestGame.CurrentState.getwhiteturn()));

	

	while (TestGame.CurrentState.getterminalstate()==false){
		
		Move BestMove;

		Move Alpha(0,0,-999999);
		Move Beta(0,0,999999);

		//conducting search
		TestGame.starttime = time(NULL);
		TestGame.SelectedMove = TestGame.absearchTimeLimit(TestGame.CurrentState, 0, Alpha, Beta);  //<---------------Select Search Method
		//TestGame.SelectedMove = TestGame.absearchNoTL(TestGame.CurrentState, 0, Alpha, Beta);
		//TestGame.SelectedMove = TestGame.minimax(TestGame.CurrentState, 0);

		//printing move and state
		TestGame.printSelectedMove();
		TestGame.CurrentState.updateStatePos(TestGame.SelectedMove.getpos(), TestGame.SelectedMove.getdirection(), !(TestGame.CurrentState.getwhiteturn()));
		TestGame.CurrentState.printStateLocal();
		//updating 
		TestGame.searchedboards.clear();
		TestGame.visitedboards.push_back(TestGame.CurrentState.getboard());
		//check if selected move resulted in terminal state
		TestGame.CurrentState.setterminalstate(TestGame.CurrentState.checkterminalState());

		movecount++;
	}
	std::cout << "This game required "<< movecount<<" moves \n";
	std::cout << "Enter a number when done \n";
	int doneyet;
	std::cin >> doneyet;
}

/*
std::string tst(" , , , , , ,X,X, , , , , ,O,O, , , , , ,X,X, , , , , ,O,O, , , , , ,X,X, , , , , ,O,O, , , , , , ");

std::string exa(" , , , , , ,  , , , , , , O, ,X, , , ,  , , ,O, , ,X , , , ,O,X,X , ,O, , ,O,X , , ,X,O, , ");
std::string exb("O,O, , , , , ,X, , , , , , , , ,X, , ,O, , , ,X,O, , ,X, , , , ,O,X, , ,X, , , , , ,O, , , , , , ,");
std::string exc(" , ,O, ,X, , , , ,O, ,X, , , , ,X, ,O, , , , ,X, ,O, , , , , , , , , , , ,O, ,X, , , , ,X, ,O, , ");

std::string ext("O,O,O,O, , ,  ,X,X,X,X, ,  , , , , , ,  , , , , , ,  , , , , , ,  , , , , , ,  , ,X,O,O,X, ");

std::string diag1("X, , , , , , ,O,X, , , , , ,O, ,X,O,O, , ,O, , ,X, , , ,X,X, ,O, , , , , , , , , , , , , , , , , ,");
std::string diag2(" , , , ,X, , , , , ,X,O,O,O, , ,X, , , , , ,X,O,X,O,X, , , , , , , , , , , , , , , , , , , , , , ,");

std::string whitewhy(" , ,O, , , , , , ,O,X,X, , , , ,X, ,O, , , , ,X, ,O, , , , , ,O, , , , , ,O, ,X, , , , ,X, , , , ,");
std::string blackwhy(" , , O, , , , , , , O, , X, , , , , X, , O, , , , , X, , O, , , , , O, X, , , , , , , , X, , , , X, O, , , , ,");
*/