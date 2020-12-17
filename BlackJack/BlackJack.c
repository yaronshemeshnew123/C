#include "BlackJack.h"
Status player_turn(Game * game) {
	char hit_stand[100];	
	int player_value = get_cards_value(game->playerHand);

	while (1) {
		if (player_value == BLACK_JACK) {
			printf("Black Jack!\n\n");
			game->cash += game->bet * 1.5;
			game->bet = 0;
			return RoundOver;
		}

		printf("Hit or Stand? (hit/stand)\n");
		getc(stdin);
		scanf("%s", hit_stand);
		if (strcmp(hit_stand, "hit") == 0) {
			printf("\n");
			
			add_last(&(game->playerHand), draw_card(game));
			player_value = get_cards_value(game->playerHand);
			
			printf("Player draws a card!\n\n");
			print_player(game);

			if (player_value > BLACK_JACK) {
				printf("Bust!\n\n");
				game->bet = 0;
				return RoundOver;
			}			
		}
		else {
			printf("\n");
			return DealerTurn;
		}
	}
}

void dealer_turn(Game * game) {
	int player_value = get_cards_value(game->playerHand);
	int dealer_value = get_cards_value(game->dealerHand);

	print_dealer(game, 0);

	if (dealer_value > player_value) {
		printf("You lose!\n\n");
		game->bet = 0;
		return;
	}

	while (dealer_value <= player_value && dealer_value < DEALER_STOP) {
		printf("Dealer draws a card!\n\n");
		add_last(&(game->dealerHand), draw_card(game));
		dealer_value = get_cards_value(game->dealerHand);
		
		print_dealer(game, 0);
	}

	if (dealer_value > BLACK_JACK) {
		printf("Dealer Bust!\n");
		game->cash += game->bet * 2;
		game->bet = 0;		
	}
	else if (dealer_value > player_value) {
		printf("Dealer Wins!\n");		
		game->bet = 0;
	}
	else if (dealer_value == player_value) {
		printf("It's a Tie!\n");	
	}
	else {	
		printf("Player Wins!\n");
		game->cash += game->bet * 2;
		game->bet = 0;	
	}
	printf("\n");
}

void print_dealer(Game * game, int hide_second) {
	Card* card_list = game->dealerHand;
	printf("Dealer cards:\n");
	show_cards(card_list, hide_second);
	if (!hide_second) {
		printf("Dealer value: %d\n\n", get_cards_value(card_list));
	}
	else {
		printf("\n");
	}
}

void print_player(Game * game) {
	Card* card_list = game->playerHand;
	printf("Player cards:\n");
	show_cards(card_list, 0);
	printf("Player value: %d\n\n", get_cards_value(card_list));
}

void initial_deal(Game * game) {
	add_last(&(game->playerHand), draw_card(game));
	add_last(&(game->playerHand), draw_card(game));
	add_last(&(game->dealerHand), draw_card(game));
	add_last(&(game->dealerHand), draw_card(game));

	print_player(game);
	print_dealer(game, 1);
}

Card * draw_card(Game * game) {
	int position = rand() % game->packSize;
	Card * drawn = remove_card(&(game->cardPack), position);
	game->packSize--;
	return drawn;
}

void add_bet(Game * game) {
	int bet = 0;	
	
	printf("Player cash: %d\n\n", game->cash);
	printf("Player bet: %d\n", game->bet);
	
	while (1) {
		printf("Please enter amount to add to bet:\n");
		scanf("%d", &bet);
		if (bet % 10 != 0) {
			printf("Bet only in multiples of 10.\n");
		}
		else if (bet == 0 && game->bet == 0) {
			printf("Initial bet must be larger than 0.\n");
		}
		else if (bet > game->cash){
			printf("Not enough cash. cash is %d.\n", game->cash);
		}
		else {
			game->bet += bet;
			game->cash -= bet;
			printf("\n");
			return;
		}
	}	
}

void init_game(Game * game) {
	game->cash = 1000;
	game->bet = 0;
	game->playerHand = NULL;
	game->dealerHand = NULL;
	game->cardPack = NULL;
	init_card_list(&(game->cardPack));
	game->packSize = NUM_RANKS * NUM_SUITS;
}

void init_card_list(Card **p_card_list) {
	Card * new_card;
	int i, j;

	for (i = 0; i < NUM_SUITS; i++) {
		for (j = 0; j < NUM_RANKS; j++) {
			new_card = (Card*)malloc(sizeof(Card));
			new_card->suit = suits[i];
			new_card->rank = ranks[j];
			add_first(p_card_list, new_card);
		}
	}
}

void free_list(Card** p_card_list) {	
	Card * current, *next;

	if (p_card_list == NULL) {
		return;
	}

	current = *p_card_list;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}

char * get_rank_string(Card * card) {
	switch (card->rank) {
	case 'A':
		return "Ace";
	case '2':
		return "2";
	case '3':
		return "3";
	case '4':
		return "4";
	case '5':
		return "5";
	case '6':
		return "6";
	case '7':
		return "7";
	case '8':
		return "8";
	case '9':
		return "9";
	case 'T':
		return "10";
	case 'J':
		return "Jack";
	case 'Q':
		return "Queen";
	case 'K':
		return "King";
	default:
		return "Unknown";
	}
}

int get_card_value(Card * card) {
	switch (card->rank) {
	case 'A':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;			
	default:
		return 10;	
	}
}

int get_num_aces(Card *card_list) {
	Card * current = card_list;
	int num = 0;

	while (current != NULL) {
		if (current->rank == 'A') {
			num++;
		}
		current = current->next;
	}

	return num;
}

int get_cards_value(Card *card_list) {
	Card * current = card_list;
	int num_aces = get_num_aces(card_list);
	int value = 0;

	while (current != NULL) {
		value += get_card_value(current);
		current = current->next;
	}

	while (num_aces > 0 && (value + 10) <= BLACK_JACK) {
		value += 10;
		num_aces--;
	}

	return value;
}

void print_card(Card* card) {
	printf("%s of %s", get_rank_string(card), card->suit);
}

void show_cards(Card* card_list, int hide_second) {
	Card * current = card_list;
	int count = 0;

	while (current != NULL) {
		count++;

		if (!hide_second || count != 2) {
			print_card(current);
		}
		else {
			printf("??????");
		}

		if (current->next != NULL) {
			printf(", ");
		}

		current = current->next;
	}

	printf("\n");
}

Card *remove_card(Card** p_card_list, int position) {
	Card * first = *p_card_list;
	Card * current = *p_card_list;
	Card * prev = NULL;
	int i;

	for (i = 0; i < position && current != NULL; i++) {
		prev = current;
		current = current->next;
	}

	if (current == NULL) {
		return NULL;
	}

	if (current == first) {
		*p_card_list = first->next;
	}
	else {
		prev->next = current->next;
	}

	current->next = NULL;
	return current;
}

void add_first(Card** p_card_list, Card* card) {
	if (card == NULL) {
		return;
	}

	card->next = *p_card_list;
	*p_card_list = card;
}

void add_last(Card** card_list, Card* card) {
	Card * current = *card_list;
	Card * prev = NULL;

	while (current != NULL) {
		prev = current;
		current = current->next;
	}

	if (prev == NULL) {
		*card_list = card;
	}
	else {
		prev->next = card;
		card->next = NULL;
	}
}

void return_cards(Game * game) {
	while (game->playerHand != NULL) {
		add_first(&(game->cardPack), remove_card(&(game->playerHand),0));		
		game->packSize++;
	}

	while (game->dealerHand != NULL) {
		add_first(&(game->cardPack), remove_card(&(game->dealerHand),0));
		game->packSize++;
	}
}
