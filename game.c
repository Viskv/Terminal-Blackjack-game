#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // for rand()

void drew_card(GameState *state, CARD_LIST *hand);

// ---------------------------------- INIT & Cleanup ------------------------------------

void init_game(GameState* state) // setup the init game state phase [phese 1]
{
     srand((unsigned int) time(NULL));
     state->cash = 1000; // init player cash
     state->pot = 0;     // init pot to 0
     state->player_score = 0; // init player score
     state->dealer_score = 0; // init dealer score

     init_deck(&state->deck); // init deck to null
     init_deck(&state->player_hand); // init player hand to null
     init_deck(&state->deler_hand); // init deler hand to null

     fill_deck(&state->deck); // create a full play deck
     printf("Game initializetion:\n\tPlayer start cash: %d$\n", state->cash);
}

void free_game (GameState* state) // free all dinamic allocated data [phese END]
{
     printf("______________________________________________________________________\n");
     printf("---------------------------- Game Finished --------------------------\n");
     printf("Clening memmory...\n");
     free_deck(&state->deck);
     free_deck(&state->player_hand);
     free_deck(&state->deler_hand);
     puts("------------------------- Memmory Clenning done -------------------------");
}

// ---------------------------------- Game Phase Logic ------------------------------------

// Handles printing cash/pot, checking for game over, player betting, and returns GAME_OVER if player is out of cash
GameStatus betting_phase (GameState *state) // [Phase 2]
{
     int bet_amount = 0;
     
     // printf("\n-------- Phase 2: Betting phase --------\n");
     printf("Player cash: %d$\n current pot: %d$\n", state->cash, state->pot);

     do
     {
          printf("\nYour bet (must be a non-negative multiple of 10, max %d).\nEnter 0 to use current pot only\n", state->cash);
          printf("bet amount: ");
          if (scanf("%d", &bet_amount) != 1)
          {
               while (getchar() != '\n'); // clear input buffer
               printf("Invalid input. Please enter a valid bet amount.\n");
               continue;
          }
          while (getchar() != '\n'); // clear input buffer
          if (bet_amount < 0 || bet_amount % 10 != 0)
          {
               printf("bet amount must be a non-negative nult of 10.\n");
               continue;
          }
          else if (bet_amount > state->cash)
          {
               printf("you cannot bet more cash than you have. current cash: %d$\n", state->cash);
               continue;
          }
          else if (bet_amount == 0 && state->pot == 0)
          {
               printf("you cannot bet 0 if the pot is empty.\n");
               continue;
          }
          else
          {
               state->cash -= bet_amount;
               state->pot += bet_amount;
               printf("\nyour bet: %d$, current pot: %d$, ramaining cash: %d$\n", bet_amount, state->pot, state->cash);
               return GAME_CONTINUE;
          }
     } while (1);     
}

// Draws 2 cards for dealer and player
void init_deal_phase(GameState *state) // [Phase 3]
{
     printf("\n-------- Phase 3: initial deal --------\n");

     // Player draw 2 cards
     drew_card(state, &state->player_hand);
     drew_card(state, &state->player_hand);
     // Dealer draw 2 cards
     drew_card(state, &state->deler_hand);
     drew_card(state, &state->deler_hand);

     printf("dealer`s hand: ");
     display_heands(&state->deler_hand, 1); // hide dealet first card
     puts("");
     printf("player`s hand: ");
     display_heands(&state->player_hand, 0); // show player hand
     puts("");
     return;
}

// Checks if the player has 21 immediately (Adjusts cash/pot if Black Jack is hit)
GameStatus black_jack_check_phase(GameState *state) // [Phase 3]
{
     state->player_score = calc_card_hand_score(&state->player_hand);
     if (state->player_score == 21)
     {
          printf("!!!BlackJack!!!, you win 1.5x your pot!\n");
          int payout = (state->pot + (state->pot / 2));
          state->cash += payout;
          state->pot = 0;
          printf("you won %d$, current cash: %d$\n", payout, state->cash);
          return PLAYER_WINS; // go to phase 6
     }
     else {return GAME_CONTINUE;} // go to phase 4
}

// Manages the player`s turn to draw cards, and returns PLAYER_BUST if the player goes over 21
GameStatus player_turn_phase(GameState *state) // [Phase 4]
{
     char Player_choise;

     printf("\n-------- Phase 4: Player Turn --------\n");

     while (1)
     {
          printf("your current hand: ");
          display_heands(&state->player_hand, 0); // show player hand and score
          puts("");
          printf("Do you want to [H/h]Hit or [S/s]Stand? ");
          Player_choise = getchar();
          while(getchar() != '\n'); // clear inpput buffer
          // Hit
          if (Player_choise == 'H' || Player_choise == 'h')
          {
               drew_card(state, &state->player_hand);
               state->player_score = calc_card_hand_score(&state->player_hand);
               if (state->player_score > 21)
               {
                    printf("your hand: ");
                    display_heands(&state->player_hand,0);
                    puts("");
                    printf("\n!!!BUST!!! you exceeded 21. dealer wins.\n");
                    // system("pause");
                    return PLAYER_BUST; // go to Phase 6
               }
               else {continue;}
          }
          // Stand
          else if (Player_choise == 'S' || Player_choise == 's')
          {
               printf("\n your score: %d\n", state->player_score);
               // system("pause");
               return GAME_CONTINUE; // go to phase 5
          }
          // Invalid input
          else {printf("Invalid choice. Please enter H/h to hit ot S/s tp Stand.\n");}
     }
}

// Manages the deler`s turn to draw card, and Dealer draws until score is >= 17 or dealer wins/bustsn, and returns the appropriate GameStatus
GameStatus dealer_turn_phase(GameState *state) // [Phase 5]
{
     printf("\n-------- Phase 5: Dealer turn --------\n\n");
     printf("Dealer`s current hand: ");
     display_heands(&state->player_hand, 0); // show player hand and score
     state->dealer_score = calc_card_hand_score(&state->deler_hand);
     while (state->dealer_score < 17)
     {
          printf("\nDealer`s hand: ");
          display_heands(&state->deler_hand, 0); // show dealer hand
          puts("");
          printf("Player`s hand: ");
          display_heands(&state->player_hand, 0); // show player hand
          puts("");
          // system("pause");
          if (state->dealer_score > state->player_score)
          {
               printf("\n!!!Dealer WINS!!! with score %d against your score %d.\n", state->dealer_score, state->player_score);
               // system("pause");
               state->pot = 0;
               return DEALER_WINS; // go to phase 6
          }
          drew_card(state, &state->deler_hand);
          state->dealer_score = calc_card_hand_score(&state->deler_hand);
     }
     
     printf("\nDealer`s hand: ");
     display_heands(&state->deler_hand, 0); // show dealer hand
     puts("");
     printf("Player`s hand: ");
     display_heands(&state->player_hand, 0); // show player hand
     puts("");
     // system("pause");
     if (state->dealer_score > 21)
     {
          printf("\n!!!Dealer BUSTS!!! with score %d. you WIN!!!!\n", state->dealer_score);
          int payout = state->pot * 2;
          state->cash += payout;
          state->pot = 0;
          printf("you won %d$, current cash: %d$\n", payout, state->cash);
          // system("pause");
          return PLAYER_WINS; // go to phase 6
     }
     else if (state->dealer_score > state->player_score)
          {
               printf("\n!!!Dealer WINS!!! with score %d against your score %d.\n", state->dealer_score, state->player_score);
               state->pot = 0;
               // system("pause");
               return DEALER_WINS; // go to phase 6
          }
     else if (state->dealer_score == state->player_score)
     {
          printf("\n!!!TIE!!! the pot = %d is carried over\n", state->pot);
          // system("pause");
          return TIE; // go to phse 6
     }
     else
     {
          printf("\n!!!YOU WIN!!! with score %d.\n", state->player_score);
          int payout = state->pot * 2;
          state->cash += payout;
          state->pot = 0;
          printf("you won %d$, current cash: %d$\n", payout, state->cash);
          // system("pause");
          return PLAYER_WINS; // go to phase 6
     }
}

// Returns all hand cards to the deck, checks for game over, and asks to play again, Returns GAME_OVER if the program should exit
GameStatus reset_card_phase(GameState *state) // [Phase 6]
{
     char play_again;

     printf("\n-------- Phase 6: reset game --------\n\n");
     
     // return dealer`s hand to deck
     while (state->deler_hand.len > 0)
     {
          CARD *returned_card = card_remove_at(&state->deler_hand, 0);
          card_push(&state->deck, returned_card);
     }
     
     // return player`s hand to deck
     while (state->player_hand.len > 0)
     {
          CARD *returned_card = card_remove_at(&state->player_hand, 0);
          card_push(&state->deck, returned_card);
     }

     printf("cards returned to deck. deck size %zu\n", state->deck.len);

     // check for game over
     if (state->cash < 10 && state->pot == 0)
     {
          printf("you are out of cash and pot is empty.\n\t\t!!!GAME OVER!!!\n");
          // system("pause");
          return GAME_OVER; // exit game
     }

     // ask to play again
     do
     {
          printf("Do you want to play again? [Y/y]Yes or [N/n]No: ");
          play_again = getchar();
          while(getchar() != '\n'); // clear inpput buffer
          if (play_again == 'Y' || play_again == 'y') 
          {
               // system("pause");
               return GAME_CONTINUE;
          } // go to phase 2
          else if (play_again == 'N' || play_again == 'n'){return GAME_OVER;} // exit game
          else{printf("\nInvalide choise. please enter Y/y or N/n.\n");}
     } while (1);
}

// ---------------------------------- Help Functions ------------------------------------

// Draws a card from deck and add it to a specific hand
void drew_card(GameState *state, CARD_LIST *hand)
{
     if (state->deck.len == 0)
     {
          fprintf(stderr, "ERROR: The deck is empty. Cannot draw a card.\n");
          exit(EXIT_FAILURE);
     }
     CARD* drewen_card = card_pop_random(&state->deck);
     if(!drewen_card)
     {
          fprintf(stderr, "ERROR: Failed to draw a card from the deck.\n");
          exit(EXIT_FAILURE);
     }
     card_push(hand, drewen_card);
     return;
}