#include "connect4.h"


void ConnectFourGame::printSelectedMove() {
	if (CurrentState.getwhiteturn())
	{
		std::cout << "White Moves ";
	}
	else
	{
		std::cout << "Black Moves ";
	}

	std::cout << postorow(SelectedMove.getpos()) << " ";
	std::cout << postoclm(SelectedMove.getpos()) << " ";
	std::cout << CurrentState.directionInttoChar(SelectedMove.getdirection())<< '\n';

	std::cout << "Move has evaluation function value " << SelectedMove.gethval() << "\n";
}

void State::printStateLocal() {
	for (int i=0; i<board.size(); i++)
	{
		if(i%7==0)
		{
			std::cout << '\n';
		}
		std::cout << board[i] << ',';
	}
	std::cout << '\n';
}