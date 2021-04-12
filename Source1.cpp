#include <iostream>
#include <random> 
#include <stdlib.h>
#include <chrono>

using namespace std;

enum Cell { EMPTY = '_', CROSS = 'X', ZERO = '0' }; 

enum Progress { IN_PROGRESS, WON_HUMAN, WON_AI, DRAW }; 

//===============================================================================================================================


struct Coord
{
	size_t x; 
	size_t y; 
};

struct Game
{
	Cell** pField;
	Cell human;
	Cell ai;
	bool humanFirst;
	size_t turn;
	Progress progress;
	int size;
};

//===============================================================================================================================

inline void myClear() 
{
	system("cls");
}

int32_t getRandom(int32_t min, int32_t max) //Современный рандом C++.
{
	const auto static seed = chrono::system_clock::now().time_since_epoch().count(); 

	static mt19937_64 generator(seed); 

	uniform_int_distribution<int32_t> dis(min, max); 

	return dis(generator);
}

//===============================================================================================================================


void initGame(Game& g) 
{
	int n;

	cout << "Press size: ";

	cin >> n;

	g.size = n;

	g.pField = new Cell * [g.size]; 

	for (size_t i = 0; i < g.size; i++)  
	{
		g.pField[i] = new Cell[g.size]; 
	}

	for (size_t y = 0; y < g.size; y++)
		for (size_t x = 0; x < g.size; x++)
			g.pField[y][x] = EMPTY;


	g.progress = IN_PROGRESS;

	if (getRandom(0, 1000) > 500) 
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.humanFirst = true;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.humanFirst = false;
		g.turn = 1; //Ход.
	}
}

void deinitGame(Game& g)
{
	for (size_t i = 0; i < g.size; i++)  
		delete[] g.pField[i]; 

	delete[] g.pField; 

	g.pField = nullptr; 
}

void drawField(const Game& g) 
{
	cout << "   "; 

	for (size_t x = 0; x < g.size; x++)
	{
		cout << x + 1 << "  "; 
	}

	cout << endl;

	for (size_t y = 0; y < g.size; y++) 
	{
		cout << y + 1 << "|"; 

		for (size_t x = 0; x < g.size; x++)
		{
			cout << " " << (char)g.pField[y][x] << " "; 
		}
		cout << "|" << endl; 
	}

	cout << endl << "Human: " << (char)g.human << endl << "AI: " << (char)g.ai << endl;
}

Coord getCoordHuman(Game& g) 
{
	Coord c = { 0 };

	do 
	{
		cout << "Enter x coord: ";

		cin >> c.x;

		cout << "Enter y coord: ";

		cin >> c.y;
	}

	while (c.x > g.size || c.y > g.size || g.pField[c.y - 1][c.x - 1] != EMPTY); 
	c.x--; 
	c.y--; 

	return c;
}

Progress isWon(const Game& g);

Coord getCoordAI(Game& g) 
{
	//============================================================================================
	//Перебрать все выигрышные ситуации в игре.
	for (size_t y = 0; y < g.size; y++) 
	{
		for (size_t x = 0; x < g.size; x++) 
		{
			if (g.pField[y][x] == EMPTY)  
			{
				g.pField[y][x] = g.ai; 

				if (isWon(g) == WON_AI) 
				{
					g.pField[y][x] = EMPTY; 
					return { x, y }; 
				}

				g.pField[y][x] = EMPTY;	
			}
		}

	}

	//============================================================================================

	//Мешать выиграть человеку.
	for (size_t y = 0; y < g.size; y++) 
	{
		for (size_t x = 0; x < g.size; x++) 
		{
			if (g.pField[y][x] == EMPTY) 
			{
				g.pField[y][x] = g.human; 
				if (isWon(g) == WON_HUMAN) 
				{
					g.pField[y][x] = EMPTY; 
					return { x, y };
				}
				g.pField[y][x] = EMPTY;
			}
		}

	}

	//============================================================================================
	
	for (size_t y = 0; y < g.size; y++)
	{
		for (size_t x = 0; x < g.size; x++)
		{

				if (g.pField[1][1] == EMPTY)
				{
					
					return { 1, 1 };
				}
				
			    if (g.pField[0][0] == EMPTY)
			    {
				    return { 0, 0 };
			    }
				
			    if (g.pField[0][2] == EMPTY)
			    {
					
				    return { 2, 0 };
			    }
				
			    if (g.pField[2][0] == EMPTY)
			    {
					
			     	return { 0, 2 };
			    }
				
			    if (g.pField[2][2] == EMPTY)
			    {
					
			    	return { 2, 2 };
			    }
				

			//=========================================================================================

			    if (g.pField[0][1] == EMPTY)
			    {

					
			    	return { 1,0 };
			    }
				
			    if (g.pField[1][0] == EMPTY)
			    {
					
				   return { 0, 1 };
			    }
				
			    if (g.pField[1][2] == EMPTY)
			    {
					
			     	return { 2, 1 };
			    }
				
			    if (g.pField[2][1] == EMPTY)
			    {
					
				    return { 1, 2 };
			    }
				
		    
		}
	}
}

Progress isWon(const Game& g)
{
	for (int y = 0; y < g.size; y++)
	{
		if (g.pField[y][0] == g.pField[y][1] && g.pField[y][0] == g.pField[y][2]) 
		{
			if (g.pField[y][0] == g.human) 
			{
				return WON_HUMAN;
			}
			else if (g.pField[y][0] == g.ai) 
			{
				return WON_AI;
			}
		}

	}

	for (int x = 0; x < g.size; x++)
	{
		if (g.pField[0][x] == g.pField[1][x] && g.pField[0][x] == g.pField[2][x]) 
		{
			if (g.pField[0][x] == g.human)
			{
				return WON_HUMAN;
			}
			else if (g.pField[0][x] == g.ai)
			{
				return WON_AI;
			}
		}

	}

	if (g.pField[0][0] == g.pField[1][1] && g.pField[0][0] == g.pField[2][2]) 
	{
		if (g.pField[0][0] == g.human) 
		{
			return WON_HUMAN;
		}
		else if (g.pField[0][0] == g.ai) 
		{
			return WON_AI;
		}
	}

	if (g.pField[2][0] == g.pField[1][1] && g.pField[2][0] == g.pField[0][2]) 
	{
		if (g.pField[1][1] == g.human) 
		{
			return WON_HUMAN;
		}
		else if (g.pField[1][1] == g.ai)
		{
			return WON_AI;
		}
	}

	bool empty = false;

	for (int y = 0; y < g.size; y++)
	{
		for (int x = 0; x < g.size; x++)
		{
			if (g.pField[y][x] == EMPTY)
			{
				empty = true;
			}
		}

	}

	if (empty) 
	{
		return IN_PROGRESS;
	}
	else 
	{
		return DRAW;
	}
}

//===============================================================================================================================

int main()
{
	Game game; //game - главная переменная, в которой содержится вся информация.
	initGame(game);

	myClear();
	drawField(game);

	while (game.progress == IN_PROGRESS) //for не подходит т.к. мы не знаем кто когда выиграет, на каком этапе. Не знаем кол-во итераций.
	{
		if (game.turn % 2 == 0) //Спрашиваем, чей ход. Ходит человек.
		{
			Coord c = getCoordHuman(game);
			game.pField[c.y][c.x] = game.human;
		}
		else // Ход AI.
		{
			Coord c = getCoordAI(game);
			game.pField[c.y][c.x] = game.ai;
		}

		game.turn++;

		myClear();
		drawField(game);
		game.progress = isWon(game); // game.progress приравниваем к isWon.
	}

	if (game.progress == WON_HUMAN)
	{
		cout << "Congrat Human, you win!";
	}
	else if (game.progress == WON_AI)
	{
		cout << "Oh, no. Mashine is better!";
	}
	else if (game.progress == DRAW)
	{
		cout << "Draw with mashine!";
	}

	deinitGame(game);

	return 0;
}