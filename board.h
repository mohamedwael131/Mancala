#pragma once
struct flag {
	bool isincup, islegal;
};
class board
{
private:
	short *arr; bool mode;
	int p1, p2;
public:
	board(bool = 0);
	flag play(short, short); 
	void print();
	short calc_eval();
	bool empty(); 
	void copy(board*);
	short* state(); 
};

