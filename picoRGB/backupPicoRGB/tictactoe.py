#source code original : https://www.askpython.com/python/examples/tic-tac-toe-using-python

from rgbkeypad import RgbKeypad
import time
import random
keypad = RgbKeypad()
keys = keypad.keys
# Function to print Tic Tac Toe
def print_tic_tac_toe(values):
    print("\n")
    print("\t     |     |")
    print("\t  {}  |  {}  |  {}".format(values[0], values[1], values[2]))
    print('\t_____|_____|_____')
 
    print("\t     |     |")
    print("\t  {}  |  {}  |  {}".format(values[3], values[4], values[5]))
    print('\t_____|_____|_____')
 
    print("\t     |     |")
 
    print("\t  {}  |  {}  |  {}".format(values[6], values[7], values[8]))
    print("\t     |     |")
    print("\n")
    print(len(values))
    # LED Grid
    for i in range(4,7):
        if values[i-4]=="X":
            keys[i].set_led(255, 0, 0)
        if values[i-4]=="O":
            keys[i].set_led(0, 255, 0)
        if values[i-4]=="":
            keys[i].set_led(0, 0, 0)
        if values[i-4]==" ":
            keys[i].set_led(0, 0, 0)             
    for i in range(8,11):
        if values[i-5]=="X":
            keys[i].set_led(255, 0, 0)
        if values[i-5]=="O":
            keys[i].set_led(0, 255, 0)
        if values[i-5]=="":
            keys[i].set_led(0, 0, 0)
        if values[i-5]==" ":
            keys[i].set_led(0, 0, 0)            
    for i in range(12,15):
        if values[i-6]=="X":
            keys[i].set_led(255, 0, 0)
        if values[i-6]=="O":
            keys[i].set_led(0, 255, 0)
        if values[i-6]=="":
            keys[i].set_led(0, 0, 0)
        if values[i-6]==" ":
            keys[i].set_led(0, 0, 0)  
 
# Function to print the score-board
def print_scoreboard(score_board):
    print("\t--------------------------------")
    print("\t              SCOREBOARD       ")
    print("\t--------------------------------")
 
    players = list(score_board.keys())
    print("\t   ", players[0], "\t    ", score_board[players[0]])
    print("\t   ", players[1], "\t    ", score_board[players[1]])
 
    print("\t--------------------------------\n")
 
# Function to check if any player has won
def check_win(player_pos, cur_player):
 
    # All possible winning combinations
    soln = [[1, 2, 3], [4, 5, 6], [7, 8, 9], [1, 4, 7], [2, 5, 8], [3, 6, 9], [1, 5, 9], [3, 5, 7]]
 
    # Loop to check if any winning combination is satisfied
    for x in soln:
        if all(y in player_pos[cur_player] for y in x):
 
            # Return True if any winning combination satisfies
            return True
    # Return False if no combination is satisfied       
    return False       
 
# Function to check if the game is drawn
def check_draw(player_pos):
    if len(player_pos['X']) + len(player_pos['O']) == 9:
        return True
    return False       
 
# Function for a single game of Tic Tac Toe
def single_game(cur_player):
 
    # Represents the Tic Tac Toe
    values = [' ' for x in range(9)]
     
    # Stores the positions occupied by X and O
    player_pos = {'X':[], 'O':[]}
     
    # Game Loop for a single game of Tic Tac Toe
    while True:
        print_tic_tac_toe(values)
         
        # Try exception block for MOVE input
       # try:
       #     print("Player ", cur_player, " turn. Which box? : ", end="")
       #     move = int(input()) 
       # except ValueError:
       #     print("Wrong Input!!! Try Again")
       #     continue
        
        move=0
        while move==0:
            keypad.update()
            if keys[4].pressed:
                move=1
            if keys[5].pressed:
                move=2            
            if keys[6].pressed:
                move=3
            if keys[8].pressed:
                move=4
            if keys[9].pressed:
                move=5            
            if keys[10].pressed:
                move=6
            if keys[12].pressed:
                move=7
            if keys[13].pressed:
                move=8            
            if keys[14].pressed:
                move=9                
        # Sanity check for MOVE inout
        if move < 1 or move > 9:
            print("Wrong Input!!! Try Again")
            continue
 
        # Check if the box is not occupied already
        if values[move-1] != ' ':
            print("Place already filled. Try again!!")
            continue
 
        # Update game information
 
        # Updating grid status 
        values[move-1] = cur_player
 
        # Updating player positions
        player_pos[cur_player].append(move)
 
        # Function call for checking win
        if check_win(player_pos, cur_player):
            print_tic_tac_toe(values)
            print("Player ", cur_player, " has won the game!!")     
            print("\n")
            return cur_player
 
        # Function call for checking draw game
        if check_draw(player_pos):
            print_tic_tac_toe(values)
            print("Game Drawn")
            print("\n")
            return 'D'
 
        # Switch player moves
        if cur_player == 'X':
            cur_player = 'O'
        else:
            cur_player = 'X'
 
if __name__ == "__main__":
 
    print("Player 1")
    player1 = "Player 1"
    print("\n")
 
    print("Player 2")
    player2 = "Player 2"
    print("\n")
     
    # Stores the player who chooses X and O
    cur_player = player1
 
    # Stores the choice of players
    player_choice = {'X' : "", 'O' : ""}
 
    # Stores the options
    options = ['X', 'O']
 
    # Stores the scoreboard
    score_board = {player1: 0, player2: 0}
    print_scoreboard(score_board)
 
    # Game Loop for a series of Tic Tac Toe
    # The loop runs until the players quit 
    while True:
 
        # Player choice Menu
        print("Turn to choose for", cur_player)
        print("Enter 1 for X")
        print("Enter 2 for O")
        print("Enter 3 to Quit")
        choice = 0
        while choice==0:
            keypad.update()
            keys[1].set_led(255, 0, 0)
            keys[2].set_led(0, 0, 0)
            time.sleep(0.5)
            keys[1].set_led(0, 0, 0)
            keys[2].set_led(0, 255, 0)
            time.sleep(0.5)
            if keys[1].pressed:
                choice=1
            if keys[2].pressed:
                choice=2
        keys[1].set_led(0, 0, 0)
        keys[2].set_led(0, 0, 0)

             
        # Try exception for CHOICE input
        #try:
        #    choice = int(input())   
        #except ValueError:
        #    print("Wrong Input!!! Try Again\n")
        #    continue
 
        # Conditions for player choice  
        if choice == 1:
            player_choice['X'] = cur_player
            if cur_player == player1:
                player_choice['O'] = player2
            else:
                player_choice['O'] = player1
 
        elif choice == 2:
            player_choice['O'] = cur_player
            if cur_player == player1:
                player_choice['X'] = player2
            else:
                player_choice['X'] = player1
         
        elif choice == 3:
            print("Final Scores")
            print_scoreboard(score_board)
            break  
 
        else:
            print("Wrong Choice!!!! Try Again\n")
 
        # Stores the winner in a single game of Tic Tac Toe
        winner = single_game(options[choice-1])
         
        # Edits the scoreboard according to the winner
        if winner != 'D' :
            player_won = player_choice[winner]
            score_board[player_won] = score_board[player_won] + 1
 
        print_scoreboard(score_board)
        # Switch player who chooses X or O
        if cur_player == player1:
            cur_player = player2
        else:
            cur_player = player1