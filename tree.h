#pragma once
#include "board.h"
struct node {
	board state; node *ptrs[6]; short win_by, next;
	node() {
		win_by = -50;
		next = 0;
	}
};
class tree
{
private:
	node *tree_ptr, *temp_ptr; board BOARD_NOW;
	void print(node*);
	void get_children(node *, short);  
	void traverse(node *, int level);
	void eval_leafs(node*);
	void eval_driver();
	bool isleaf(node*);
	short eval(node*);
	void minimax(node*, bool);
	void calc(node *,bool); 
public:
	tree(board);
	void print_all(); 
	void construct(int); 
	void minimax_driver(); 
	short get_nextmove();
};

