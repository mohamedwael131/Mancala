from copy import deepcopy

def comp_move(i, state):
    comp_side, player_side, comp_cup, player_cup = deepcopy(state)
    i -= 1
    rocks = comp_side[i]
    if rocks == 0:
        print('Cant play on zero', rocks)
        play_again = True
        return [comp_side, player_side, comp_cup, player_cup], play_again
    comp_side[i] = 0
    j = i-1
    play_again = False
    while(rocks):
        while(j>=0):
            if rocks == 0:
                break
            comp_side[j] += 1
            rocks -= 1
            j -= 1
        if rocks != 0:
            comp_cup[0] += 1
            rocks -= 1
            if rocks == 0:
                play_again = True
        j = 0
        while(j<=5):
            if rocks == 0:
                break
            player_side[j] += 1
            rocks -= 1
            j += 1
        j = 5
    return [comp_side, player_side, comp_cup, player_cup], play_again

def player_move(i, state):
    comp_side, player_side, comp_cup, player_cup = deepcopy(state)
    i -= 1
    rocks = player_side[i]
    if rocks == 0:
        print('Cant play on zero', rocks)
        play_again = True
        return [comp_side, player_side, comp_cup, player_cup], play_again
    player_side[i] = 0
    j = i+1
    play_again = False
    while(rocks):
        while(j<=5):
            if rocks == 0:
                break
            player_side[j] += 1
            rocks -= 1
            j += 1
        if rocks != 0:
            player_cup[0] += 1
            rocks -= 1
            if rocks == 0:
                play_again = True
        j = 5
        while(j>=0):
            if rocks == 0:
                break
            comp_side[j] += 1
            rocks -= 1
            j -= 1
        j = 0
    return [comp_side, player_side, comp_cup, player_cup], play_again


def best_comp_move(state):

    new_states = [None] * 6
    play_again = [False] * 6
    for i in range(6):
        if state[0][i] == 0:
            continue
        new_states[i], play_again[i] = comp_move(i+1, state)
        if sum(new_states[i][0]) == 0:
            new_states[i][3][0] += sum(new_states[i][1])
            new_states[i][1] = [0, 0, 0, 0, 0, 0]
            break
        if sum(new_states[i][1]) == 0:
            new_states[i][2][0] += sum(new_states[i][0])
            new_states[i][0] = [0, 0, 0, 0, 0, 0]
            break
        if play_again[i]:
            best_move, max_cup, new_new_states = best_comp_move(new_states[i])
            new_states[i] = deepcopy(new_new_states[best_move-1])

    max_cup = -100
    max_cup_id = -1
    for i in range(6):
        if new_states[i] == None:
            continue
        cup = new_states[i][2][0]
        if cup > max_cup:
            max_cup = cup
            max_cup_id = i
    best_move = max_cup_id+1
    return best_move, max_cup, new_states

def best_player_move(state):

    new_states = [None] * 6
    play_again = [False] * 6
    for i in range(6):
        if state[1][i] == 0:
            continue
        new_states[i], play_again[i] = player_move(i+1, state)
        if sum(new_states[i][0]) == 0:
            new_states[i][3][0] += sum(new_states[i][1])
            new_states[i][1] = [0, 0, 0, 0, 0, 0]
            break
        if sum(new_states[i][1]) == 0:
            new_states[i][2][0] += sum(new_states[i][0])
            new_states[i][0] = [0, 0, 0, 0, 0, 0]
            break
        if play_again[i]:
            best_move, max_cup, new_new_states = best_player_move(new_states[i])
            # new_states[i][2][0] = max_cup
            new_states[i] = deepcopy(new_new_states[best_move-1])

    min_cup = 100
    min_cup_id = -1
    for i in range(6):
        if new_states[i] == None:
            continue
        cup = new_states[i][2][0]
        if cup < min_cup:
            min_cup = cup
            min_cup_id = i
    best_move = min_cup_id+1
    return best_move, min_cup, new_states


def get_best_move(level, state):
    best_move, max_cup, new_states = best_comp_move(state)

    return_move = best_move
    if level == 1:
        return return_move

    new_multi_states = []
    best_move = [-1] * 6
    min_cup = [-50] * 6
    for i in range(6):
        if new_states[i] == None:
            # best_move[i], min_cup[i], returned_states = best_player_move(new_states[i])
            new_multi_states.append([None]*6)
            continue
        best_move[i], min_cup[i], returned_states = best_player_move(new_states[i])
        new_multi_states.append(returned_states)

    if max(min_cup) > 0:
        return_move = min_cup.index(max(min_cup)) + 1
    if level == 2:
        return return_move

    best_move_1 = [-1] * 6
    max_cup_1 = [-50] * 6
    for i in range(6):
        if new_multi_states[0][i] == None:
            continue
        best_move_1[i], max_cup_1[i], _ = best_comp_move(new_multi_states[0][i])

    best_move_2 = [-1] * 6
    max_cup_2 = [-50] * 6
    for i in range(6):
        if new_multi_states[1][i] == None:
            continue
        best_move_2[i], max_cup_2[i], _ = best_comp_move(new_multi_states[1][i])

    best_move_3 = [-1] * 6
    max_cup_3 = [-50] * 6
    for i in range(6):
        if new_multi_states[2][i] == None:
            continue
        best_move_3[i], max_cup_3[i], _ = best_comp_move(new_multi_states[2][i])

    best_move_4 = [-1] * 6
    max_cup_4 = [-50] * 6
    for i in range(6):
        if new_multi_states[3][i] == None:
            continue
        best_move_4[i], max_cup_4[i], _ = best_comp_move(new_multi_states[3][i])

    best_move_5 = [-1] * 6
    max_cup_5 = [-50] * 6
    for i in range(6):
        if new_multi_states[4][i] == None:
            continue
        best_move_5[i], max_cup_5[i], _ = best_comp_move(new_multi_states[4][i])

    best_move_6 = [-1] * 6
    max_cup_6 = [-50] * 6
    for i in range(6):
        if new_multi_states[5][i] == None:
            continue
        best_move_6[i], max_cup_6[i], _ = best_comp_move(new_multi_states[5][i])


    total = [max(l) for l in [max_cup_1, max_cup_2, max_cup_3, max_cup_4, max_cup_5, max_cup_6]]
    if max(total) > 0:
        return_move = total.index(max(total)) + 1
    if level == 3:
        return return_move