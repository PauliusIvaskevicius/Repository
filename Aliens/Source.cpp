#include <iostream>
#include <fstream>
#include <amp.h>
using namespace std;

int i_map[2000][2000];
int i_mapSize[2]; // R , C   ||| Scanable area R-1 C-1
int i_minRud;

void readData(int(&map)[2000][2000], int(&mapSize)[2], int & minRud);
void findPlace(int(&map)[2000][2000], int(&mapSize)[2], int & minRud);
int findValue(int(&map)[2000][2000], int r, int c, int x, int k);

int main()
{
	readData(i_map, i_mapSize, i_minRud);
	findPlace(i_map, i_mapSize, i_minRud);
}


void readData(int(&map)[2000][2000], int(&mapSize)[2], int & minRud)
{

	ifstream data("Data.txt");
	data >> mapSize[0] >> mapSize[1] >> minRud;
	for (int i = 0; i < mapSize[0]; i++)
	{
		for (int j = 0; j < mapSize[1]; j++)
		{
			data >> map[i][j];
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
	data.close();

}

void findPlace(int(&map)[2000][2000], int(&mapSize)[2], int & minRud)
{
	int i_sumMax = 0;
	int i_sum = 0;
	int i_coords[5][5];
	for (int i = 0; i < mapSize[0]; i++)
	{
		for (int j = 0; j < mapSize[1]; j++)
		{
			// SCAN 2x2
			
			if (mapSize[0] > mapSize[1]) // Daryti viska pagal C  [1]
			{
				for (int x = 1; x <= mapSize[1]; x++) // Kvadrato dydis -----------------------
				{
					
					if (x == 1)
					{
						i_sum = i_sum + map[i][j];
					}
					else
					{
						for (int k = 0; k < x; k++)
						{
							i_sum = i_sum + map[i + k][j] + findValue(i_map, i, j, x, k);
						}
					}
					
					if (i_sum > i_sumMax && i_sum > i_minRud)
					{
						
						i_sumMax = i_sum;
						
					}
					cout << "x = " << x << " | " << i_sum << endl;
					i_sum = 0;
				}

			}
			else
			{
				for (int x = 1; x <= mapSize[0]; x++) // Kvadrato dydis -----------------------
				{

					if (x == 1)
					{
						i_sum = i_sum + map[i][j];
					}
					else
					{
						for (int k = 0; k < x; k++)
						{
							i_sum = i_sum + map[i + k][j] + findValue(i_map, i, j, x, k);
						}
					}

					if (i_sum > i_sumMax && i_sum > i_minRud)
					{

						i_sumMax = i_sum;

					}
					cout << "x = " << x << " | " << i_sum << endl;
					i_sum = 0;
				}
			}


		}
		cout << endl;
	}
	cout << i_sumMax << endl;
}


int findValue(int(&map)[2000][2000], int r, int c, int x,int k)
{
	int v = 0;

		for (int i = 1; i <= x; i++)
		{
			v += map[r + k][c + i];
		}
	
	
	return v;
}