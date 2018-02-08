#include "connect4.h"

void boardinput(std::string &initboard, std::string &inputboard) {

	for (int i = 0; i < inputboard.size(); i++) {
		if (inputboard[i]!=','&& inputboard[i]!='\n')
		{
			initboard.append(1, inputboard[i]);  //append 1 copy of char
		}
	}
}

void boardoutput(std::string& outputboard) {
	for (int i = 0; i < 7; i++)
	{
		int pos = i * 7;
		for (int j = 0; j < 6; j++)
		{
			std::cout << outputboard[pos + j] << ',' ;
		}
		std::cout << outputboard[pos + 6] << '/n';
	}
	std::cout << '/n';
}

int main() {
	std::string exa(" , , , , , ,  , , , , , , O, ,X, , , ,  , , ,O, , ,X , , , ,O,X,X , ,O, , ,O,X , , ,X,O, , ");
	std::string exb("O,O, , , , , ,X, , , , , , , , ,X, , ,O, , , ,X,O, , ,X, , , , ,O,X, , ,X, , , , , ,O, , , , , , ,");
	std::string exc(" , ,O, ,X, , , , ,O, ,X, , , , ,X, ,O, , , , ,X, ,O, , , , , , , , , , , ,O, ,X, , , , ,X, ,O, , ");

	/*
	for (int depth = 1; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exa);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState=TmpState;

		TestGame.c4maxdepth = depth;
	

		Move Alpha(0,0,-999999);
		Move Beta(0,0,999999);

		TestGame.SelectedMove = TestGame.absearch(TestGame.CurrentState, 0, Alpha,Beta);
		


		std::cout << depth << " exa AB " <<  TestGame.searchedboardcount<< '\n';
	}
	*/
	
	for (int depth = 6; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exa);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState = TmpState;

		TestGame.c4maxdepth = depth;

		TestGame.SelectedMove = TestGame.minimax(TestGame.CurrentState, 0);


		std::cout << depth << " exa Mini " << TestGame.searchedbillions << " billions and" << TestGame.searchedboardcount << '\n';
	}
	/*

	for (int depth = 1; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exb);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState = TmpState;

		TestGame.c4maxdepth = depth;


		Move Alpha(0, 0, -999999);
		Move Beta(0, 0, 999999);

		TestGame.SelectedMove = TestGame.absearch(TestGame.CurrentState, 0, Alpha, Beta);

		std::cout << depth << " exb AB "  << TestGame.searchedboardcount << '\n';
	}
	*/
	for (int depth = 6; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exb);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState = TmpState;

		TestGame.c4maxdepth = depth;



		Move Alpha(0, 0, -999999);
		Move Beta(0, 0, 999999);

		TestGame.SelectedMove = TestGame.minimax(TestGame.CurrentState, 0);

		std::cout << depth << " exb Mini " << TestGame.searchedbillions << " billions and" << TestGame.searchedboardcount << '\n';
	}
	/*
	for (int depth = 1; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exc);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState = TmpState;

		TestGame.c4maxdepth = depth;


		Move Alpha(0, 0, -999999);
		Move Beta(0, 0, 999999);

		TestGame.SelectedMove = TestGame.absearch(TestGame.CurrentState, 0, Alpha, Beta);


		std::cout << depth << " exc AB " <<  TestGame.searchedboardcount << '\n';
	}
	
	*/
	for (int depth = 6; depth < 7; depth++)
	{
		std::string initialboard;
		boardinput(initialboard, exc);
		State  TmpState(initialboard, true);
		ConnectFourGame TestGame;

		TestGame.CurrentState = TmpState;

		TestGame.c4maxdepth = depth;

		TestGame.SelectedMove = TestGame.minimax(TestGame.CurrentState, 0);


		std::cout << depth << " exc Mini " << TestGame.searchedbillions << " billions and" << TestGame.searchedboardcount << '\n';
	}


	//ConnectFourGame(initialboard, 5, true);
	std::cout << "\n happy \n";
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