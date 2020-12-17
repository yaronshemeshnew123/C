#define _CRT_SECURE_NO_WARNINGS //ignores warnings.

#ifndef _BLACK_JECK_H 
#define _BLACK_JECK_H //use this file if not used.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLACK_JACK 21
#define NUM_SUITS 4
#define NUM_RANKS 13
#define DEALER_STOP 17

char * suits[] = { "Spades", "Hearts", "Clubs", "Diamonds" };
char ranks[] = { 'A','2','3','4','5','6','7','8','9','T','J','Q','K' };

typedef enum status { PlayerTurn = 1, DealerTurn=2, RoundOver=3, GameOver=4 } Status;

typedef struct card {
	char * suit;
	char rank;
	struct card * next;
} Card;

typedef struct game {
	int cash;
	int bet;
	Card * playerHand;
	Card * dealerHand;
	Card * cardPack;
	int packSize;
} Game;

Status player_turn(Game * game);
void dealer_turn(Game * game);
void initial_deal(Game * game);
Card * draw_card(Game * game);
void return_cards(Game * game);
void print_dealer(Game * game, int hide_second);
void print_player(Game * game);
// will get game struct and ask the player to make a bet.
void add_bet(Game * game);

// will get game struct and initialize the game (cash, bet, pack))
void init_game(Game * game);

// will dynamically allocate, and fill, the list pointed by p_card_list with one of every
// combination of rank and suit.assumes p_card_list points to an empty list.
void init_card_list(Card **p_card_list);

// Free all the dynamically allocated cards in the list.To be used when the program ends.
void free_list(Card** p_card_list);

// gets a card and returns its rank as string.
char * get_rank_string(Card * card);

// gets a card and returns its value. For aces will return 1.
int get_card_value(Card * card);

// gets a list of cards and returns the number of aces in it.
int get_num_aces(Card *card_list);

// gets a list of cards, calculates its value, and returns it.
int get_cards_value(Card *card_list);

// gets a card and print it to screen - example: "Ace of Spades"
void print_card(Card* card);

// gets a list of cards, prints them in output, separated by a comma.For example :
// For a single card of suit Clubs and rank Ace print on the screen Ace of Clubs.
// For the cards 7 of Spades and 3 of Hearts, print 7 of Spades, 3 of hearts
void show_cards(Card* card_list, int is_dealer);

// Remove the card at specified position and return its address. (the card is NOT freed from
// memory, only removed from a list)
Card *remove_card(Card** p_card_list, int position);

// Add single card to start of a list.The card is NOT dynamically allocated, it should already
// exist(for example, it may have been removed from another list using the remove_card
// function above)
void add_first(Card** p_card_list, Card* card);

// 	Add single card to end of the list
void add_last(Card** card_list, Card* card);

#endif
