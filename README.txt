Project Name: Terminal Black-jack Game
language: C
compiler: gcc -std=c99 -pedantic -wall -werror
----------------------------------------------
File structure:
main.c	-   Contains the main function and the central game loop that controls the sequence of phases.
game.c	-   Implements the high-level game logic and user interaction (betting, hit/stand, outcome determination).
game.h	-   Defines the GameState struct and declares all game phase function prototypes.
card.c	-   Implements the low-level data structures (linked list functions, card creation, scoring, and bitwise operations).
card.h	-   Defines the Card and CardList structs, enums (Rank, Suit, GameStatus), and declares all linked list/card helper functions.
----------------------------------------------
Game Phases:
1. Initialization:	init deck 52 cards, set cash, init hands
2. Betting: 	player input bet (validate for cash, bet x10 base)
3. Init deal:	Two cards are randomly drawn and dealt to both the player and the dealer.
4. B.J. Check:	Player wins immediately if initial score is 21 (1.5x payout).
5. Player turn:	Player chooses to 'Hit' or 'Stand'. Player busts if the score exceeds 21.
6. Dealer turn:	Dealer reveals their hidden card and hits until their score is 17 or more.
7. Outcome: 	Final scores are compared to determine the winner (2x payout) or a push/tie (pot carries over).
8. Reset Cards:	All cards are returned to the deck, and the user is prompted to play again.
----------------------------------------------
card suit representation bits 0->3:

xxxx 0001: Hearts
xxxx 0010: Clubes
xxxx 0100: Diamonds
xxxx 1000: Spades
----------------------------------------------
cards rank representation bits 4->7:

0001 xxxx: ace	(1)	val[1/11]
0010 xxxx: 2	(2)	val[2]
0011 xxxx: 3	(3)	val[3]
0100 xxxx: 4	(4)	val[4]
0101 xxxx: 5	(5)	val[5]
0110 xxxx: 6	(6)	val[6]
0111 xxxx: 7	(7)	val[7]
1000 xxxx: 8	(8)	val[8]
1001 xxxx: 9	(9)	val[9]
1010 xxxx: 10	(10)	val[10]
1011 xxxx: Jack	(11)	val[10]
1100 xxxx: Queen	(12)	val[10]
1101 xxxx: King	(13)	val[10]
-----------------------------------------------
