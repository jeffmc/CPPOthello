#include <iostream>
#include <bitset>
#include <cstring>

#define EMPTY_CHAR ' '
#define PLAYABLE_CHAR '-'
#define PLAYER0_CHAR 'X'
#define PLAYER1_CHAR 'O'
#define othsize 8

typedef unsigned char uchar;
static const char * const alphabet = "abcdefghijklmnopqrstuvwxyz";

static const uchar PLAYER0 = 0x10, PLAYER1 = 0x01, EMPTY = 0x00;

class OthelloState {
	friend class cell;
	private:
		std::bitset<othsize*othsize*2> board; 
		// 0x00: open
		// 0x10: player1
		// 0x01: player2
		// 0x11: undefined
	public:
		OthelloState();
		class cell;
		void pbin();
		cell operator[](const int& pos);
		cell at(const int& x, const int& y);
};
class OthelloState::cell {
	friend class OthelloState;
	private:
		const int i;
		OthelloState* const othst;
		cell(const int& i, OthelloState* const othst) :
			i(i), othst(othst) { }
	public:
		void operator=(const uchar& p);
		operator uchar();
};

class OthelloMoves {
	private:
		std::bitset<othsize*othsize> plays;
	public:
		OthelloMoves(const OthelloState& othst, const uchar& p);
		bool operator[](const int& pos) const;
		bool at(const int& x, const int& y) const;
};
OthelloState::cell OthelloState::operator[](const int &pos) {
	return OthelloState::cell(pos * 2, this);
}
OthelloState::cell OthelloState::at(const int& x, const int& y) {
	return OthelloState::cell((x + y * othsize)*2, this);
}
void OthelloState::cell::operator=(const uchar& p) {
	this->othst->board[this->i] = (p == 0x10);	
	this->othst->board[this->i+1] = (p == 0x01);	
}
OthelloState::cell::operator uchar() {
	uchar c = 0;
	if (this->othst->board[this->i]) {
		c |= 0x10;
	}
	if (this->othst->board[(this->i)+1]) {
		c |= 0x01;
	}
	return c;
}
OthelloState::OthelloState() {
	this->at(3,3) = 0x10;
	this->at(4,4) = 0x10;
	this->at(4,3) = 0x01;
	this->at(3,4) = 0x01;
}
OthelloMoves::OthelloMoves(const OthelloState& othst, const uchar& p) {
	//this->plays.flip();
	// TODO: Put in move validating algo here!
}
bool OthelloMoves::operator[](const int& pos) const {
	return this->plays[pos];
}
bool OthelloMoves::at(const int& x, const int& y) const {
	return this->plays[y*othsize + x];
}

void OthelloState::pbin() {
	int i=0;
	while (i<othsize*othsize*2) {
		for (int x=0;x<othsize;x++) {
			std::cout << this->board[i++] << this->board[i++] << ' ';
		}
		std::cout << std::endl;
	}
}

void pheadfoot() {
	std::cout << "  ";
	for (int i=0;i<othsize;i++) {
		std::cout << alphabet[i] << ' ';
	}
	std::cout << std::endl;
}
void pothmoves(const OthelloMoves& mvs, OthelloState& othst) {
	pheadfoot();
	for (int y=0;y<othsize;y++) {
		std::cout << (y+1) << ' ';
		for (int x=0;x<othsize;x++) {
			uchar c = othst.at(x,y);
			switch (c) {
			case PLAYER0:
				std::cout << PLAYER0_CHAR;
				break;
			case PLAYER1:
				std::cout << PLAYER1_CHAR;
				break;
			case EMPTY:	
				std::cout << (mvs.at(x,y)?PLAYABLE_CHAR:EMPTY_CHAR);
				break;
			default:
				std::cout << "GARBLED DATA!";
				break;
			}
			std::cout << ' ';
		}
		std::cout << (y+1) << std::endl;
	}
	pheadfoot();
}

int main() {
	OthelloState os;
	OthelloMoves om(os,PLAYER0);
	pothmoves(om, os);
	return 0;
}

