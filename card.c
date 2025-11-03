#include "card.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // for rand()

#define RANK_MASK 0xf0 // 1111 0000
#define SUIT_MASK 0x0f // 0000 1111

// ------------ CARD UTIL (Bitwise) ------------

int get_rank(const CARD* card) // bits 7-4 (4 bits) [get rank bit val]
{
     return (card->val & RANK_MASK) >> 4;
}

int get_suit(const CARD* card) // bits 3-0 (4 bits) [get suit bit val]
{
     return card->val & SUIT_MASK;
}

int get_score_val(const CARD* card) // convert rank to score val
{
     int val = get_rank(card);
     if (val >= RANK_10) return 10; // 10, J, Q, K
     if (val == RANK_ACE) return 1; // Ace
     return val; // 2 - 9
}

const char* get_rank_name(const CARD* card) // bits 7-4 (4 bits) [Ace, 2, 3, ..., 10, J, Q, K]
{
     int rank = get_rank(card);
     switch (rank)
     {
     case RANK_ACE:      return "Ace";
     case RANK_2:        return "2";
     case RANK_3:        return "3";
     case RANK_4:        return "4";
     case RANK_5:        return "5";
     case RANK_6:        return "6";
     case RANK_7:        return "7";
     case RANK_8:        return "8";
     case RANK_9:        return "9";
     case RANK_10:       return "10";
     case RANK_JACK:     return "Jack";
     case RANK_QUEEN:    return "Queen";
     case RANK_KING:     return "King";
     default:            return "Unknown";
     }
}

const char* get_suit_name(const CARD* card) // bits 3-0 (4 bits) [Hearts, Clubs, Diamonds, Spades]
{
     int suit = get_suit(card);
     switch (suit)
     {
     case SUIT_HEARTS:   return "Heartd";
     case SUIT_CLUBS:    return "Clubs";
     case SUIT_DIAMONDS: return "Diamonds";
     case SUIT_SPADES:   return "Spades";
     default:            return "Unknown";
     }
}

CARD* create_card(int rank, int suit_bits) // add a new card to end of list
{
     CARD* new_card = (CARD*)malloc(sizeof(CARD));
     if (!new_card)
     {
          fprintf(stderr, "ERROR: Memory allocation failed for new card.\n");
          exit(EXIT_FAILURE);
     }
     new_card->val = (uint8_t)((rank << 4)| suit_bits);
     new_card->next = NULL;
     return new_card;
}

// ------------ DECK/HAND(CardList) UTIL ------------

void init_deck(CARD_LIST* list) // init deck to NULL
{
     list->head = NULL;
     list->len = 0;
}

void card_push(CARD_LIST* list, CARD* card) // add card to front of the list
{
     if (!list || !card) return;
     card->next = list->head;
     list->head = card;
     list->len++;
     return;
}

void fill_deck(CARD_LIST* deck) // fill deck with 52 cards
{
     init_deck(deck);
     for (SuitList suit = SUIT_HEARTS; suit <= SUIT_SPADES; suit <<= 1)
     {
          for (RankList rank = RANK_ACE; rank <= RANK_KING; rank++)
          {
               CARD* new_card = create_card(rank, suit);
               card_push(deck, new_card);
          }
     }
     return;
}

void free_deck(CARD_LIST* list) // free list
{
     if (!list) return;
     CARD* current = list->head;
     while (current != NULL)
     {
          CARD* temp = current;
          current = current->next;
          free(temp);
          list->len--;
     }
     list->head = NULL;
     return;
}

CARD* card_remove_at(CARD_LIST* list, size_t index) // remove card at a given index [0 -> ListLen] and return it node
{
     if (!list)
     {
          fprintf(stderr, "ERROR: List is NULL.\n");
          return NULL;
     }
     if (index >= list->len)
     {
          fprintf(stderr, "ERROR: Index out of bounds of List.\n");
          exit(EXIT_FAILURE);
     }

     CARD* current = list->head;
     CARD* prev = NULL;
     while (index--)
     {
          prev = current;
          current = current->next;
     }
     
     if(!prev)// removing the first node in the list
     {
          list->head = current->next;
          current->next = NULL;
     }
     else
     {
          prev->next = current->next;
          current->next = NULL;
     }
     list->len--;
     return current;
}

CARD* card_pop_random(CARD_LIST* list) // remove a rand card from list
{
     if (!list || list->len == 0)
     {
          fprintf(stderr, "ERROR: List is NULL or empty.\n");
          return NULL;
     }
     size_t rand_index = rand() % list->len;
     return card_remove_at(list, rand_index); 
}

int calc_card_hand_score(const CARD_LIST* hand) // calc hand score 
{
     unsigned int score = 0;
     unsigned int ace_count = 0;
     CARD* current = hand->head;

     while (current != NULL)
     {
          int rank = get_rank(current);
          switch (rank)
          {
          case RANK_ACE:   
                         score ++;  
                         ace_count ++;
               break;
          case RANK_JACK:
                         score += 10;
               break;
          case RANK_QUEEN: 
                         score += 10;
               break;
          case RANK_KING:
                         score += 10;
               break;
          default:       
                         score += rank;
               break;
          } // END SWITCH ------------
          current = current->next;
     } // END WHILE ------------------

     // Handle Aces (1 or 11)
     while ((ace_count > 0) && (score <= 11))
     {
          score += 10;
          ace_count --;
     }

     return score;
}

void display_heands(const CARD_LIST* hend, int hide_dealer_card) // display hands
{
     CARD* current = hend->head;

     // display hands cards
     while (current != NULL) // while current != NULL
     {
          (hide_dealer_card && (current == hend->head)) ? printf("[????? of ?????] ") : printf("[%s of %s] ", get_rank_name(current), get_suit_name(current));
          current = current->next;
     }

     // display hands score
     (hide_dealer_card) ? printf("(score: \?\?\?\?\?)\n") : printf("(score: %d)\n", calc_card_hand_score(hend));

     return;
}
