#include <iostream>;
#include <stdlib.h>;
#include <string>;
#include <random>;
using namespace std;

int gameActive = true;
double credits = 1000;
double bet;
int dealerHand = 0;
int playerHand = 0;
int winLose;
int currentCard = 1;

// Create a struct (class?) for the card
struct Card
{
	const char* suit;
	const char* type;
	int value;
};

// Create the uninitialized deck of 52 cards
Card deck[52];

// Initialize the deck
void InitializeDeck() {
	const char* suits[4] = { "Clubs", "Diamonds", "Hearts", "Spades" };
	const char* cardType[13] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };
	int cardValue[13] = { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 };
	int cards = 0;
	int value = 1;
	for (int suit = 0; suit < 4; suit++)
	{
		for (int type = 0; type < 13; type++)
		{
			deck[cards].suit = suits[suit];
			deck[cards].type = cardType[type];
			if (cardType[type] == "Ace")
			{
				deck[cards].value = 1;
			}
			else if (cardType[type] == "Jack" || cardType[type] ==  "Queen" || cardType[type] == "King")
			{
				deck[cards].value = 10;
			}
			else
			{
				deck[cards].value = std::stoi(cardType[type]);
			}
			cards++;
		}
	}
	// Is using "std::shuffle" not in the spirit of the assignment or is this actually the right thing to do?
	std::random_device rd;
	std::default_random_engine rng(rd());
	std::shuffle(deck, deck + 52, rng);
}

int HandValue(int dealerHand, int playerHand, int bet, char action)
{
	cout << endl;
	if (action == 'S' || action == 'D')
	{
		cout << "Dealer: " << dealerHand << endl;
	}
	cout << "Player: " << playerHand << endl;
	cout << "Bet: " << bet << endl;
	if (action == 'X')
	{
		cout << "Passing on hand, dealer takes half bet" << endl;
		return 4;
	}
	if (dealerHand > 21)
	{
		cout << "Dealer Bust (+" << bet << ")" << endl;
		return 1;
	}
	else if (playerHand > 21)
	{
		cout << "Player Bust (-" << bet << ")" << endl;
		return 3;
	}
	else if (dealerHand == 21 && playerHand == 21)
	{
		cout << "Tie" << endl;
		return 2;
	}
	else if (dealerHand == 21 && playerHand != 21)
	{
		cout << "Dealer Wins (-" << bet << ")" << endl;
		return 3;
	}
	else if (playerHand == 21 && dealerHand != 21)
	{
		cout << "Player Wins (+" << bet << ")" << endl;
		return 1;
	}
	else if (playerHand <= 21 && (dealerHand > playerHand && dealerHand > 17) && action == 'S')
	{
		cout << "Dealer Wins (-" << bet << ")" << endl;
		return 3;
	}
	else if (dealerHand <= 21 && (dealerHand > playerHand && dealerHand > 17) && action == 'S')
	{
		cout << "Player Wins (+" << bet << ")" << endl;
		return 1;
	}
	else if (playerHand <= 21 && (dealerHand < playerHand && dealerHand > 17) && action == 'S')
	{
		cout << "Player Wins (+" << bet << ")" << endl;
		return 1;
	}
	else if (playerHand <= 21 && (dealerHand > playerHand && dealerHand > 17) && action == 'D')
	{
		cout << "Dealer Wins (-" << bet << ")" << endl;
		return 3;
	}
	else if (dealerHand <= 21 && (dealerHand > playerHand && dealerHand > 17) && action == 'D')
	{
		cout << "Player Wins (+" << bet << ")" << endl;
		return 1;
	}
	else if (playerHand <= 21 && (dealerHand < playerHand && dealerHand > 17) && action == 'D')
	{
		cout << "Player Wins (+" << bet << ")" << endl;
		return 1;
	}

	else if (playerHand == dealerHand)
	{
		cout << "Tie" << endl;
		return 2;
	}
	else
	{
		return NULL;
	}
}

bool EndRound(int dealerHand, int playerHand, int bet, char action)
{
	cout << endl;
	cout << endl;
	cout << "Hand over" << endl;
	HandValue(dealerHand, playerHand, bet, action);
	return false;
}

int DealCard(int handValue, char activeHand, int hidden) {
	if (currentCard <= 52)
	{
		if (hidden == false)
		{
			cout << activeHand << " Deal: " << deck[currentCard].type << " " << deck[currentCard].suit << endl;
		}
		else
		{
			cout << activeHand << " Deal: Hidden" << endl;
		}
		handValue = handValue + deck[currentCard].value;
		currentCard++;
		return handValue;
	}
}

void NewGame()
{
	cout << "Credits: " << credits << endl;
	cout << "How much would you like to bet?: ";
	cin >> bet;
	while (bet > credits)
	{
		cout << "Not enough credits" << endl;
		cout << "How much would you like to bet?: ";
		cin >> bet;
	}
	playerHand = 0;
	dealerHand = 0;
	dealerHand = DealCard(dealerHand, 'D', false);
	playerHand = DealCard(playerHand, 'P', false);
	dealerHand = DealCard(dealerHand, 'D', true);
	playerHand = DealCard(playerHand, 'P', false);
	HandValue(dealerHand, playerHand, bet, 'X');
}

int main()
{
	InitializeDeck();
	char action;
	char newGame;

	cout << "-------------------------Blackjack------------------------" << endl;
	cout << endl;
	cout << "For official Blackjack rules, https://bicyclecards.com/how-to-play/blackjack" << endl;
	cout << endl;
	
	NewGame();

	while (gameActive)
	{
		if (winLose == 0)
		{
			cout << "[H]it, [S]tand, S[P]lit, [D]ouble, [X]Pass, [Q]uit" << endl;
			cin >> action;

			switch (action) {
			case 'H':
				cout << "HIT" << endl;
				playerHand = DealCard(playerHand, 'P', false);
				winLose = HandValue(dealerHand, playerHand, bet, action);
				break;

			case 'S':
				cout << "STAND" << endl;
				while (dealerHand <= 17)
				{
					dealerHand = DealCard(dealerHand, 'D', false);
					winLose = HandValue(dealerHand, playerHand, bet, action);
				}
				if (dealerHand >= 17)
				{
					winLose = HandValue(dealerHand, playerHand, bet, action);
				}
				break;

			case 'P':
				cout << "__SPLIT" << endl;
				break;

			case 'X':
				cout << "PASS" << endl;
				winLose = HandValue(dealerHand, playerHand, bet, action);
				break;

			case 'D':
				cout << "DOUBLE" << endl;
				if (playerHand == 9 || playerHand == 10 || playerHand == 11)
				{
					if (bet * 2 > credits)
					{
						cout << "Not enough credits to double" << endl;
						break;
					}
					bet = bet * 2;
					playerHand = DealCard(playerHand, 'P', false);
					while (dealerHand <= 17)
					{
						dealerHand = DealCard(dealerHand, 'D', false);
						winLose = HandValue(dealerHand, playerHand, bet, action);
					}
					if (dealerHand >= 17)
					{
						winLose = HandValue(dealerHand, playerHand, bet, action);
					}
					break;
				}
				else
				{
					cout << "Double not allowed unless hand value is 9, 10, or 11" << endl;
					break;
				}

				break;
			case 'Q':
				gameActive = false;
				break;

			default:
				cout << "Invalid entry" << endl;
			}
		}
		else
		{
			if (winLose == 4)
			{
				credits = credits - (bet / 2);
				cout << endl;
				cout << "Credits: " << credits << endl;
			}
			if (winLose == 3)
			{
				credits = credits - bet;
				cout << endl;
				cout << "Credits: " << credits << endl;
			}
			else if (winLose == 1)
			{
				credits = credits + bet;
				cout << endl;
				cout << "Credits: " << credits << endl;
			}
			else if (winLose == 2)
			{
				credits = credits;
				cout << endl;
				cout << "Credits: " << credits << endl;
			}
			winLose = 0;

			cout << "New Game? [Y]/[N]: ";
			cin >> newGame;

			switch (newGame) {
			case 'Y':
				gameActive = true;
				NewGame();
				break;

			case 'N':
				gameActive = false;
				break;
			default:
				break;
			}
		}
	}

	return 0;
}