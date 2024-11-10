#include "board.h"
#include <iostream>
using namespace std;
flag board::play(short player,short pos){
	flag out = { 0,0 }; int i;
	short last = 3;
	int num = player * 7 + pos;
	if (num == 6 || num == 13 || num >13 || num < 0) {
		out = { 0,0 };
	}
	else {
		if (arr[num % 14] == 0) {
			out = { 0,0 };
		}
		else {
			i = 1;
			while (arr[num] > 0) {
				arr[(i + num) % 14] += 1;
				arr[num]--;
				i++;
				last = (i + num - 1) % 14;
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
board::board(bool mod) :mode(mod) {
		arr = new short[14];
		for (int i = 0; i < 14; i++) {
			arr[i] = ((i + 1) % 7) ? 4 : 0;
		}
		p1 = p2 = 0;
	}
void board::print() {
		for (int i = 13; i > 6; i--) {
			cout << arr[i] << ",";
		}
		cout << endl;
		for (int i = 0; i < 7; i++) {
			cout << arr[i] << ",";
		}
		cout << endl << endl;
	}
short* board::state() {
		return arr;
	}
void board::copy(board* new_board) {
		short *temp_arr = new_board->state();
		for (int i = 0; i < 14; i++) {
			arr[i] = temp_arr[i];
		}

	}
short board::calc_eval() {
		p1 = p2 = 0;
		for (int i = 0; i < 7; i++) {
			p1 += arr[i];
			p2 += arr[i + 7];
		}
		return p2 - p1;
	}
bool board::empty() {
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
