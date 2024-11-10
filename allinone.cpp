#include <iostream>
using namespace std;
struct flag {
	bool isincup, islegal;
};
class board {
private:
	short *arr; bool mode;
	int p1, p2;
public:
	board(bool mod = 0) :mode(mod) {
		arr = new short[14];
		for (int i = 0; i < 14; i++) {
			arr[i] = ((i + 1) % 7) ? 4 : 0;
		}
		p1 = p2 = 0;
	}
	flag play(short player, short pos) {
		flag out = { 0,0 }; int i;  
		short last = 3;
		int num = player*7 + pos; 
		if (num == 6 || num == 13 || num >13 || num < 0) {
			out = { 0,0 }; 
		}
		else {
			if (arr[num%14] == 0) {
				out = { 0,0 }; 
			}
			else {
				i = 1;
				while (arr[num] > 0) {
					arr[(i + num)%14] += 1; 
					arr[num]--;
					i++; 
					last = (i + num-1) % 14; 
				}
				if (last == 6 || last == 13) {
					out = { 1,1 }; 
				}
				else {
					out = { 0,1 }; 
				}
			}
		}
		return out;
	}
	void print() {
		for (int i = 13; i > 6; i--) {
			cout << arr[i] << ",";
		}
		cout << endl;
		for (int i = 0; i < 7; i++) {
			cout << arr[i] << ",";
		}
		cout << endl << endl;
	}
	short *state() {
		return arr;
	}
	void copy(board* new_board) {
		short *temp_arr = new_board->state();
		for (int i = 0; i < 14; i++) {
			arr[i] = temp_arr[i];
		}

	}
	short calc_eval() {
		p1 = p2 = 0;
		for (int i = 0; i < 7; i++) {
			p1 += arr[i];
			p2 += arr[i + 7];
		}
		return p2 - p1;
	}
	bool empty() {
		short flag_1 = 0, flag_2 = 0;
		for (int i = 0; i < 6; i++) {
			if (arr[i] != 0) {
				flag_1 += 1;
			}
			if (arr[i + 7] != 0) {
				flag_2 += 1;
			}
		}
		if (flag_1 == 0 || flag_2 == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
};
struct node {
	
	board state; node *ptrs[6]; short win_by, next;
	node() {
		win_by = -50;
		next = 0; 
	}
};

class tree {
private:
	node *tree_ptr, *temp_ptr; board BOARD_NOW;
	void print(node *start) {
		if (start != NULL) {
			start->state.print();
			for (int i = 0; i < 6; i++) {
				print(start->ptrs[i]);
			}
		}
	}
	void get_children(node *start, short player) {
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
	void traverse(node *root, int level) {
		if (root != NULL && level > 0) {
			get_children(root, (level % 2));
			level--;
			for (int i = 0; i < 6; i++) {
				traverse(root->ptrs[i], level);
			}
		}
	}
	void eval_leafs(node *start) {
		if (start != NULL) {
			for (int i = 0; i < 6; i++) {
				eval_leafs(start->ptrs[i]);
				start->win_by = isleaf(start) ? eval(start) : -50;
			}
		}
	}
	void eval_driver() {
		eval_leafs(tree_ptr);
	}
	bool isleaf(node *start) {
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
	short eval(node *nd) {
		return nd->state.calc_eval();
	}
	void minimax(node *start, bool ismaxlevel) {
		if (start != NULL) {
			for (int i = 0; i < 6; i++) {
				minimax(start->ptrs[i], !ismaxlevel);
				calc(start, ismaxlevel);
			}
		}
	}
	void calc(node *start, bool ismax) {
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
public:
	tree(board input) {
		tree_ptr = new node;
		for (int i = 0; i < 6; i++) {
			tree_ptr->ptrs[i] = NULL;
			tree_ptr->state.copy(&input);
		}
		BOARD_NOW.copy(&input);
	}
	void print_all() {
		print(tree_ptr);
	}
	void construct(int max_depth) {
		traverse(tree_ptr, max_depth);
		eval_driver();
		minimax_driver();
	}
	void minimax_driver() {
		minimax(tree_ptr, 1);
	}
	short get_nextmove() {
		return tree_ptr->next;
	}
};
class game {
	bool turn; board BOARD_NOW; short MOVE; flag player_flags; short winner; 
public:
	game() :MOVE(-1), player_flags({ 1,0 }) {
		cout << "Would you like to go first? (1|0): "; cin >> turn; cout << endl;
		tree AI_GAME_TREE = BOARD_NOW;
		if (!turn) {
			AI_GAME_TREE.construct(6);
			BOARD_NOW.play(1, AI_GAME_TREE.get_nextmove()); // AI moving
			cout << "Computer plays: " << endl; 
			BOARD_NOW.print();
		}
		while (!BOARD_NOW.empty()) {
			while (player_flags.islegal != 1 || player_flags.isincup == 1) {
				BOARD_NOW.print(); 
				cout << "Enter a number between 0-5: "; cin >> MOVE;
				cout << "You play: " << endl; 
				player_flags = BOARD_NOW.play(0, MOVE);
				BOARD_NOW.print(); 
				}
			AI_GAME_TREE = BOARD_NOW;
			AI_GAME_TREE.construct(6); BOARD_NOW.play(1, AI_GAME_TREE.get_nextmove());
			cout << "Computer plays: " << endl; 
			MOVE = -1; 
			player_flags.islegal = 0; 
			player_flags.isincup = 1; 
		}
		winner = BOARD_NOW.calc_eval(); 
		if (winner > 0) {
			cout << "Computer wins!" << endl;
		}
		else if (winner == 0) {
			cout << "A DRAW! MY AI IS AS SMART AS YOU OR YOU ARE AS DUMB AS IT" << endl;
		}
		else {
			cout << "You win!" << endl;
		}
	}	
};



int main() {

	game test;

}