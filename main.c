/*
     Title: C Project 2507
     project: Blackjack Game
*/

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(void)
{
     printf("\033[2J\033[H"); // Clear screen

     printf("\t##########Welcome to Blackjack Game##########\n\n");
     GameState state;
     GameStatus game_status = GAME_CONTINUE;
     init_game(&state);

     while (game_status != GAME_OVER)
     {
          // Phase 2: Betting
          game_status = betting_phase(&state); 

          // phase 3: Initial Deal
          init_deal_phase(&state);
          game_status = black_jack_check_phase(&state);
          if (game_status != GAME_CONTINUE)
          {
               game_status = reset_card_phase(&state);
               continue;
          }
          
          // phase 4: player turn
          game_status = player_turn_phase(&state);
          if (game_status != GAME_CONTINUE)
          {
               game_status = reset_card_phase(&state);
               continue;
          }
          
          // phase 5: dealer turn
          game_status = dealer_turn_phase(&state);
          
          // phase 6: reset cards
          game_status = reset_card_phase(&state);
     }
     

     free_game(&state);
     return 0;
}