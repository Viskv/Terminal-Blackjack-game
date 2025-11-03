#ifndef CARD_H
#define CARD_H

#include <stddef.h> // size_t
#include <stdint.h> // for uint8_t

// ------------ ENUMS & CONSTANTS ------------

// Game Status
typedef enum {
     GAME_OVER,
     GAME_CONTINUE,
     PLAYER_BUST,
     DEALER_WINS,
     PLAYER_WINS,
     TIE
}GameStatus;

// Card def
typedef enum {
     RANK_ACE = 1,
     RANK_2,
     RANK_3,
     RANK_4,
     RANK_5,
     RANK_6,
     RANK_7,
     RANK_8,
     RANK_9,
     RANK_10,
     RANK_JACK,
     RANK_QUEEN,
     RANK_KING = 13
} RankList;

// Card Suit def
typedef enum {
     SUIT_HEARTS    = 0x1,
     SUIT_CLUBS     = 0x2,
     SUIT_DIAMONDS  = 0x4,
     SUIT_SPADES    = 0x8
} SuitList;

// ------------ STRUCTURES ------------

// card structure
typedef struct CARD
{
     uint8_t val;
     struct CARD *next;
} CARD;

// deck structure
typedef struct CARD_LIST
{
     CARD* head;
     size_t len;
} CARD_LIST;

// ------------ CARD UTIL (Bitwise) ------------

int get_rank(const CARD* card); // bits 7-4 (4 bits) [get rank bit val]

int get_suit(const CARD* card); // bits 3-0 (4 bits) [get suit bit val]

int get_score_val(const CARD* card); // convert rank to score val

const char* get_rank_name(const CARD* card); // bits 7-4 (4 bits) [Ace, 2, 3, ..., 10, J, Q, K]

const char* get_suit_name(const CARD* card); // bits 3-0 (4 bits) [Hearts, Clubs, Diamonds, Spades]
 
CARD* create_card(int rank, int suit_bits); // add a new card to end of list

// ------------ DECK(List) UTIL ------------

void init_deck(CARD_LIST* list); // init deck to NULL

void fill_deck(CARD_LIST* deck); // fill deck with 52 cards

void free_deck(CARD_LIST* list); // free list

void card_push(CARD_LIST* list, CARD* card); // add card to front of the list

CARD* card_remove_at(CARD_LIST* list, size_t index); // remove card at a given index and return it node

CARD* card_pop_random(CARD_LIST* list); // remove a rand card from list

int calc_card_hand_score(const CARD_LIST* hand); // calc hand score 

void display_heands(const CARD_LIST* hend, int hide_dealer_card); // display hands

#endif // CARD_H