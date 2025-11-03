#ifndef GAME_H
#define GAME_H

#include "card.h"

// ------------ STRUCTS ------------

typedef struct GameState
{
     CARD_LIST deck;
     CARD_LIST deler_hand;
     CARD_LIST player_hand;
     int cash;
     int pot;
     int player_score;
     int dealer_score;
} GameState;

// ----------------- INIT & Cleanup -------------------

void init_game(GameState* state); // setup the init game state phase [phese 1]

void free_game (GameState* state); // free all dinamic allocated data [phese END]

// ------------ Game Phase Logic ------------

// ######################## [Phase 2] ########################
// Handles printing cash/pot, checking for game over, player betting, and returns GAME_OVER if player is out of cash
GameStatus betting_phase (GameState *state); 

// ######################## [Phase 3] ########################

void init_deal_phase(GameState *state); // Draws 2 cards for dealer and player

GameStatus black_jack_check_phase(GameState *state); // Checks if the player has 21 immediately (Adjusts cash/pot if Black Jack is hit)

// ######################## [Phase 4] ########################
// Manages the player`s turn to draw cards, and returns PLAYER_BUST if the player goes over 21
GameStatus player_turn_phase(GameState *state);

// ######################## [Phase 5] ########################
// Manages the deler`s turn to draw card, and Dealer draws until score is >= 17 or dealer wins/bustsn, and returns the appropriate GameStatus
GameStatus dealer_turn_phase(GameState *state);

// ######################## [Phase 6] ########################
// Returns all hand cards to the deck, checks for game over, and asks to play again, Returns GAME_OVER if the program should exit
GameStatus reset_card_phase(GameState *state);

#endif // GAME_H