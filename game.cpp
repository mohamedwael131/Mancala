#include "game.h"
#include <iostream>
using namespace std;
game::game() :MOVE(-1), player_flags({ 1,0 }) {
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
	bool f; 
	cout << "click any key to exit" << endl; 
	cin >> f; 
}