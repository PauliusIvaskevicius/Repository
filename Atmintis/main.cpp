#include <iostream>
#include <ctime>

using namespace std;

int i_cards[50];

int random(int min, int max);
void mixCards();
int main()
{
	mixCards();

	/*for (int i = 0; i < 50; i++)
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