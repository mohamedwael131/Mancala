#include "tree.h"
#include <iostream>
using namespace std; 
void tree::print(node *start) {
	if (start != NULL) {
		start->state.print();
		for (int i = 0; i < 6; i++) {
			print(start->ptrs[i]);
		}
	}
}
void tree::get_children(node *start, short player) {
	board temp; node *_internal_arr[6];
	for (int i = 0; i < 6; i++) {
		temp.copy(&BOARD_NOW);
		flag out = temp.play(player, i);
		if (out.islegal) {
			_internal_arr[i] = new node;
			_internal_arr[i]->state.copy(&temp);
			for (int j = 0; j < 6; j++) {
				_internal_arr[i]->ptrs[j] = NULL;
			}
		}
		else {
			_internal_arr[i] = NULL;
		}
	}
	short count = 0;
	for (int i = 0; i < 6; i++) {
		if (_internal_arr[i] != NULL) {
			start->ptrs[count++] = _internal_arr[i];
		}
	}
}
void tree::traverse(node *root, int level) {
	if (root != NULL && level > 0) {
		get_children(root, (level % 2));
		level--;
		for (int i = 0; i < 6; i++) {
			traverse(root->ptrs[i], level);
		}
	}
}
void tree::eval_leafs(node *start) {
	if (start != NULL) {
		for (int i = 0; i < 6; i++) {
			eval_leafs(start->ptrs[i]);
			start->win_by = isleaf(start) ? eval(start) : -50;
		}
	}
}
void tree::eval_driver() {
	eval_leafs(tree_ptr);
}
bool tree::isleaf(node *start) {
	bool flag = 0;
	for (int i = 0; i < 6; i++) {
		if (start->ptrs[i] != NULL) {
			return 0;
		}
	}
	if (start != NULL) {
		return 1;
	}
	else {
		return  0;
	}
}
short tree::eval(node *nd) {
	return nd->state.calc_eval();
}
void tree::minimax(node *start, bool ismaxlevel) {
	if (start != NULL) {
		for (int i = 0; i < 6; i++) {
			minimax(start->ptrs[i], !ismaxlevel);
			calc(start, ismaxlevel);
		}
	}
}
void tree::calc(node *start, bool ismax) {
	short max, min, next_min, next_max;
	if (start != NULL && !isleaf(start)) {
		max = -50, min = 50;
		for (int i = 0; i < 6; i++) {
			if (start->ptrs[i] != NULL) {
				if (start->ptrs[i]->win_by >= max) {
					max = start->ptrs[i]->win_by;
					next_max = i;
				}
				if (start->ptrs[i]->win_by <= min) {
					min = start->ptrs[i]->win_by;
					next_min = i;
				}
			}
		}
		start->win_by = (ismax) ? max : min;
		start->next = (ismax) ? next_max : next_min;
	}
}
tree::tree(board input) {
		tree_ptr = new node;
		for (int i = 0; i < 6; i++) {
			tree_ptr->ptrs[i] = NULL;
			tree_ptr->state.copy(&input);
		}
		BOARD_NOW.copy(&input);
	}
void tree::print_all() {
		print(tree_ptr);
	}
void tree::construct(int max_depth) {
		traverse(tree_ptr, max_depth);
		eval_driver();
		minimax_driver();
	}
void tree::minimax_driver() {
		minimax(tree_ptr, 1);
	}
short tree::get_nextmove() {
	return tree_ptr->next;
}