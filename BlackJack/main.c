#include "BlackJack.h" //import the file. 
int  main() {
	Game game;
	char play_again;
	Status status = PlayerTurn;
	
	srand(time(NULL));

	init_game(&game);

	while (1) {
		add_bet(&game);
		initial_deal(&game);			
		
		status = player_turn(&game);		
		if (status == DealerTurn) {
			dealer_turn(&game);
		}
		return_cards(&game);

		printf("Player cash: %d\n", game.cash);
		printf("Player bet: %d\n\n", game.bet);

		if (game.cash < 10) {
			printf("Out of cash to bet. Game Over!\n");
			free_list(&(game.cardPack));
			return;
		}
		printf("Do you wish to bet again? (y/n)\n");
		getc(stdin);
		play_again = getc(stdin);
		if (play_again == 'n') {
			free_list(&(game.cardPack));
			return;
		}
		printf("\n");
	}
return 1;
}
