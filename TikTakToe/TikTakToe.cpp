
#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;

static int score = 0;
static int score2 = 0;


//=======================================================

enum TCell : char
{
	CROSS = 'X',
	ZERO = '0',
	EMPTY = ' '
};
struct TCoord
{
	size_t y{ 0U };
	size_t x{ 0U };
};
enum TProgress
{
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};
#pragma pack(push, 1)
struct TGame
{
	TCell** ppField{ nullptr };
	const size_t SIZE{ 3U };
	TProgress progress{ IN_PROGRESS };
	TCell human, ai;
	size_t turn{ 0U }; // четные чел. нечет комп.

};
#pragma pack(pop)

//=======================================================

int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

//=======================================================

void initGame(TGame& g)
{
	g.ppField = new TCell * [g.SIZE];
	for (size_t i = 0U; i < g.SIZE; i++)
	{
		g.ppField[i] = new TCell[g.SIZE];
	}
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			g.ppField[y][x] = EMPTY;
		}
	}
	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.turn = 1;
	}
}

void deinitGame(TGame& g)
{
	for (size_t i = 0U; i < g.SIZE; i++)
	{
		delete[] g.ppField[i];
	}

	delete[] g.ppField;
	g.ppField = nullptr;

}

void printGame(TGame& g)
{
	setlocale(LC_ALL, "Russian");
	cout << "     ";
	for (size_t x = 0; x < g.SIZE; x++)
	{
		cout << "X" << x + 1 << "  ";
	}
	cout << endl;
	for (size_t y = 0; y < g.SIZE; y++)
	{
		cout << "Y" << y + 1 << " | ";
		for (size_t x = 0; x < g.SIZE; x++)
		{
			cout << g.ppField[y][x] << " | ";
		}
		cout << endl << endl;
	}
	cout << endl << "Человек играет за: " << g.human << endl << "Компьютер играет за: " << g.ai << endl << endl;
	cout << "Сделайте ход: " << endl;
}

void congrats(TGame& g)
{
	
	setlocale(LC_ALL, "Russian");
	if (g.progress == WON_HUMAN)
	{
		
		cout << "Раунд за человеком! " << endl << endl;
		cout << "Человек: " << ++score << endl << "Компютер: " << score2;
		
	}
	else if (g.progress == WON_AI)
	{
		
		cout << "Раунд за компьютером! " << endl << endl;
		cout << "Человек: " << score << endl << "Компютер: " << ++score2;
	
	}
	else if (g.progress == DRAW)
		cout << "Ничья!" << endl;

}

//=======================================================

TProgress getWon(const TGame& g)
{
	//строки
	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
		{
			if (g.ppField[y][0] == g.human)
				return WON_HUMAN;
			if (g.ppField[y][0] == g.ai)
				return WON_AI;
		}
	}
	//столбы
	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
		{
			if (g.ppField[0][x] == g.human)
				return WON_HUMAN;
			if (g.ppField[0][x] == g.ai)
				return WON_AI;
		}
	}
	//диагональ\ 
	if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
	{
		if (g.ppField[1][1] == g.human)
			return WON_HUMAN;
		if (g.ppField[1][1] == g.ai)
			return WON_AI;
	}
	//диагональ/
	if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[2][0] == g.ppField[1][1])
	{
		if (g.ppField[1][1] == g.human)
			return WON_HUMAN;
		if (g.ppField[1][1] == g.ai)
			return WON_AI;
	}
	//ничья
	bool draw{ true };
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				draw = false;
				break;
			}
		}
		if (!draw)
			break;
	}
	if (draw)
		return DRAW;
	return IN_PROGRESS;

}

TCoord getHumanCoord(const TGame& g)
{
	TCoord c;
	do {
		cout << "Enter X: ";
		cin >> c.x;
		cout << "Enter Y: ";
		cin >> c.y;

		c.x--;
		c.y--;
	} while (c.x > 2 || c.y > 2 || g.ppField[c.y][c.x] != EMPTY);

	return c;
}

TCoord getAICoord(TGame& g)
{

	//----------------------------WIN
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.ai;
				if (getWon(g) == WON_AI)
				{
					g.ppField[y][x] = EMPTY;
					return{ y, x };
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}
	//----------------------------FAIL
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.human;
				if (getWon(g) == WON_HUMAN)
				{
					g.ppField[y][x] = EMPTY;
					return{ y, x };
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}
	//--------------------------------------

	if (g.ppField[1][1] == EMPTY)
	{
		return { 1,1 };
	}

	TCoord buf[4];
	size_t num{ 0U };

	if (g.ppField[0][0] == EMPTY)
	{
		buf[num] = { 0,0 };
		num++;
	}
	if (g.ppField[2][0] == EMPTY)
	{
		buf[num] = { 2,0 };
		num++;
	}
	if (g.ppField[0][2] == EMPTY)
	{
		buf[num] = { 0,2 };
		num++;
	}
	if (g.ppField[2][2] == EMPTY)
	{
		buf[num] = { 2,2 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}

	if (g.ppField[1][0] == EMPTY)
	{
		buf[num] = { 1,0 };
		num++;
	}
	if (g.ppField[0][1] == EMPTY)
	{
		buf[num] = { 0,1 };
		num++;
	}
	if (g.ppField[1][2] == EMPTY)
	{
		buf[num] = { 1,2 };
		num++;
	}
	if (g.ppField[2][1] == EMPTY)
	{
		buf[num] = { 2,1 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}




}


void Zastavka()
{
	setlocale(LC_ALL, "Russian");
	cout << "<!!!Игра: Крестики-Нолики!!!>" << endl << endl;

}




//=======================================================


int main()
{
	
	TGame g;
	Zastavka();
	system("pause");
	system("cls");
	
	{
		int SIZE ;
		cout << "Введите количество игр!" << endl;
		cin >> SIZE;
		int i = 0;
		while (i < SIZE)
			{
			initGame(g);
			system("cls");
			printGame(g);
			do {
				if (g.turn % 2 == 0)
				{
					const TCoord c = getHumanCoord(g);
					g.ppField[c.y][c.x] = g.human;
				}
				else
				{
					const TCoord c = getAICoord(g);
					g.ppField[c.y][c.x] = g.ai;
				}
				system("cls");
				printGame(g);
				g.turn++;
				g.progress = getWon(g);

			} while (g.progress == IN_PROGRESS);
			congrats(g);
			deinitGame(g);
			
			cout << endl;
			system("pause");
			i++;
			
		}
		system("cls");
		if (score2 > score)
		{
			cout << endl  << "Компьютер победил!" << endl;
		}
		if (score2 < score)
		{
			cout << endl << "Человек выйграл!" << endl;
		}
		if (score == score2)
		{
			cout << endl << "Ничья!" << endl;
		}

			
	}


	return 0;
}

//=======================================================
