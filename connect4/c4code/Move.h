
class Move {
public:
	Move() {
		pos = 0;
		direction = 0; //-7 north, 1 east, 7 south, -1 west
		hval = 0;
	}
	Move(int p, int dir) {
		pos = p;
		direction = dir;
		hval = 0;
	}
	Move(int p, int dir, int h) {
		pos = p;
		direction = dir;
		hval = h;
	}
	~Move() {

	}

	int getpos() {
		return pos;
	}
	int getdirection() {
		return direction;
	}
	int gethval() {
		return hval;
	}
	void setpos(int i) {
		pos = i;
	}
	void setdirection(int i) {
		direction = i;
	}
	void sethval(int i) {
		hval = i;
	}

	void printMove();

private:
	int pos;
	int direction; //-7 north, 1 east, 7 south, -1 west
	int hval = 0;


	void copyMove(Move Copy) {
		pos = Copy.pos;
		direction = Copy.direction;
	}
	void copyMovePntr(Move *&Copy) {
		pos = Copy->pos;
		direction = Copy->direction;
	}


	int postorow(int ps) {
		return int(ps / 7);
	}
	int postoclm(int ps) {
		return pos % 7;
	}
};