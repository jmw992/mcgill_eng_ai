#include "connect4.h"

char State::directionInttoChar(int direction) {
	switch (direction)
	{
	case -7: return 'N';
	case 1:	return  'E';
	case 7:	return 'S';
	case -1: return 'W';
	default:
		std::cout << "invalid move number entered";
		return 'Z';
	}
}

char State::directionChartoInt(char direction) {
	switch (direction)
	{
	case 'N':	return  7;
	case 'E':	return  1;
	case 'S':	return -7;
	case 'W':	return -1;
	default:
		std::cout << "invalid move letter entered";
		return 999999999;
	}
}
/*
std::string State::movetostring(int row, int clm, int direction) {
	std::string r = std::to_string(row);
	std::string c = std::to_string(clm);

	switch (direction)
	{
	case -7: return std::string() + r + c + 'N';
	case 1:	return std::string() + r + c + 'E';
	case 7:	return std::string() + r + c + 'S';
	case -1: return std::string() + r + c + 'W';
	default:
		std::cout << "invalid move number entered";
		return "invalid move number entered";
	}
}
*/
std::string State::movePostoString(int pos, int direction) {
	std::string p = std::to_string(pos);

	switch (direction)
	{
	case -7: return std::string() + p + 'N';
	case 1:	return std::string() + p + 'E';
	case 7:	return std::string() + p + 'S';
	case -1: return std::string() + p + 'W';
	default:
		std::cout << "invalid move number entered";
		return "invalid move number entered";
	}
}

void State::updateStatePos(int pos, int direction, bool white) {
	int newpos = pos + direction;

	if (whiteturn) {
		for (int i = 0; i < whitepieces.size(); i++)
		{
			if (whitepieces[i] == pos)
			{
				whitepieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'O';		//turn changed b4 piece moved
		}
	}
	else {
		for (int i = 0; i < blackpieces.size(); i++)
		{
			if (blackpieces[i] == pos)
			{
				blackpieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'X';		//turn changed b4 piece move
		}
	}

	whiteturn = white;
}

void State::updateStateString(std::string mv, bool white) {
	int row = int(mv[0]);
	int clm = int(mv[1]);
	int direction = directionChartoInt(mv[2]);

	int pos = rowclmtopos(row, clm);
	int newpos = pos + direction;

	if (white) {
		for (int i=0; i<whitepieces.size(); i++)
		{
			if (whitepieces[i]==pos)
			{
				whitepieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'O';		//turn is updated b4 move change
		}
	}
	else {
		for (int i = 0; i < blackpieces.size(); i++)
		{
			if (blackpieces[i] == pos)
			{
				blackpieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'X';		//turn is updated b4 move change
		}
	}
}

void State::updateStateInt(int row, int clm, int direction, bool white) {
	int pos = rowclmtopos(row, clm);
	int newpos = pos + direction;

	if (white) {
		for (int i = 0; i < whitepieces.size(); i++)
		{
			if (whitepieces[i] == pos)
			{
				whitepieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'O';
		}
	}
	else {
		for (int i = 0; i < blackpieces.size(); i++)
		{
			if (blackpieces[i] == pos)
			{
				blackpieces[i] = newpos;
			}
			board[pos] = ' ';
			board[newpos] = 'X';
		}
	}
}