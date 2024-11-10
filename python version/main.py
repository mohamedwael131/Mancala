from copy import deepcopy
import pickle
import os.path
import time
from ai import *

def print_board(state):
    comp_side, player_side, comp_cup, player_cup = state
    print(comp_cup, ' '.join(map(str, comp_side)))
    print('   ', ' '.join(map(str, player_side)), player_cup)


def is_game_over(state):
    comp_side, player_side, comp_cup, player_cup = deepcopy(state)
    is_over = False
    if sum(comp_side) == 0:
        player_cup[0] += sum(player_side)
        player_side = [0] * 6
        is_over = True
    if sum(player_side) == 0:
        comp_cup[0] += sum(comp_side)
        comp_side = [0] * 6
        is_over = True
    return [comp_side, player_side, comp_cup, player_cup], is_over

def save(state, file_name='state.pkl'):
    with open('state.pkl', 'wb') as f:
        pickle.dump(state, f)

def load(file_name='state.pkl'):
    with open(file_name, 'rb') as f:
        return pickle.load(f)


def main():

    comp_side = [4, 4, 4, 4, 4, 4]
    comp_cup = [0]
    player_side = [4, 4, 4, 4, 4, 4]
    player_cup = [0]

    state = comp_side, player_side, comp_cup, player_cup

    turn = int(input('who goes first \n0 for computer \n1 for player\n'))

    lvl = int(input('Choose a level from 1 to 3\n3 being the hardest\n'))
    while(lvl not in [1, 2, 3]):
        lvl = int(input('Choose a level from 1 to 3\n3 being the hardest\n'))

    if os.path.isfile('state.pkl'):
        state = load()
        print('State loaded!')

    print_board(state)
    is_over = False
    while(True):
        play = True
        if turn:
            while(play):
                i = input('player\'s turn\nEnter a number from 1 to 6\n')
                if i == 's' or i == 'S':
                    save(state)
                    print('game_saved!')
                    exit(0)
                else:
                    i = int(i)
                state, play = player_move(i, state)
                print_board(state)
                final_state, is_over = is_game_over(state)
                if is_over:
                    print('game over')
                    print_board(final_state)
                    exit(0)
        play = True
        while(play):
            # best_move, new_cup, new_states = best_comp_move(state)
            best_move = get_best_move(lvl, state)
            print('computer\'s turn')
            print('best move', best_move)
            # i = int(input('computer\'s turn\n'))
            # state, play = comp_move(i, state)
            time.sleep(1)
            state, play = comp_move(best_move, state)
            print_board(state)
            final_state, is_over = is_game_over(state)
            if is_over:
                print('game over')
                print_board(final_state)
                exit(0)
        turn = True

if __name__ == '__main__':
    main()