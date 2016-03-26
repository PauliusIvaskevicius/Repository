#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;


// INFO ------------------------------ http://www.lmio.mii.vu.lt/?act=getFile&id=502
int i_cards[50];
int i_usedCards[25];
int i_usedCardsCurrent = 0;

int random(int min, int max);
void mixCards();
void play();
int faceup(int id);
bool isCardGood(int id);
int main()
{
	mixCards();
	play();
		
	/*
			for (int i = 0; i < 50; i++)
		{
			char asciiChar = i_cards[i];
			cout << asciiChar << " ";
		}
	*/
	return 0;
}

void mixCards()
{ 
	for (int i = 65; i <= 89; i++)
	{
		bool placed = false;
		while (!placed)
		{
			int placeTo = random(0, 50);
			if (i_cards[placeTo] == NULL) // We can place
			{
				i_cards[placeTo] = i;
				placed = true;
			}

		}
	}
	for (int i = 65; i <= 89; i++)
	{
		bool placed = false;
		while (!placed)
		{
			int placeTo = random(0, 50);
			if (i_cards[placeTo] == NULL) // We can place
			{
				i_cards[placeTo] = i;
				placed = true;
			}

		}
	}

}

int random(int min, int max) //range : [min, max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (max - min);
}
void play()
{
	int candies = 0;
	int id;
	int cardOne, cardTwo;
	while (candies < 25)
	{
		system("cls");

		cout << endl;
		cout << "Enter card ID :" << endl; cin >> id; id--;
		cardOne = faceup(id);
		cout << "Enter card ID :" << endl; cin >> id; id--;
		cardTwo = faceup(id);
		if (cardOne == cardTwo && isCardGood(id) == true)
		{
			candies++;
			i_usedCards[i_usedCardsCurrent] = cardOne;
			i_usedCardsCurrent++;
			cout << "Cards opened : " << (char)cardOne << " " << (char)cardTwo << endl;
			cout << "Two cards were correct . +1 Candy " << endl;
		}
		else
		{
			cout << "Cards opened : " << (char)cardOne << " " << (char)cardTwo << endl;
			cout << "Cards were not the same or you already opened them :( " << endl;
		}
		Sleep(2000);
	}
}
int faceup(int id)
{
	return i_cards[id];
}
bool isCardGood(int id)
{
	for (int i = 0; i < 25; i++)
	{
		if (i_cards[id] == i_usedCards[i])
		{
			return false;
		}
	}
	return true;
}