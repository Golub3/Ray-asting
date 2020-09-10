#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 120;			// Размер консоли (столбцы)
int nScreenHeight = 40;			// Размер консоли (строки)
int nMapWidth = 16;
int nMapHeight = 16;

float fPlayerX = 14.7f;
float fPlayerY = 5.09f;
float fPlayerA = 0.0f;			// Направление игрока
float fFOV = 3.14159f / 4.0f;	// Угол обзора
float fDepth = 16.0f;			// Дистанция обзора
float fSpeed = 5.0f;			// Скорость ходьбы

int main()
{
	// Вывод на экран
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole); // Настройка консоли
	DWORD dwBytesWritten = 0;

	wstring map;
	map += L"################";
	map += L"##.............#";
	map += L"#.......##....##";
	map += L"#...............";
	map += L"#......##.......";
	map += L"#......##.......";
	map += L"#...............";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"##.....#.......#";
	map += L"#..............#";
	map += L"#......##....###";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();   // Переменные для подсчета
	auto tp2 = chrono::system_clock::now();	  // пройденного времени

	while (1)   // Игровой цикл
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();


		// Клавишей "A" поворачиваем по часовой стрелке
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

		// Клавишей "D" поворачиваем против часовой стрелки
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

		// Клавишей "Q" идём налево
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')  // Если столкнулись со стеной, то откатываем шаг
			{
				fPlayerX += sinf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			}
		}

		// Клавишей "E" идём направо
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')  // Если столкнулись со стеной, то откатываем шаг
			{
				fPlayerX -= sinf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA + 3.14159f / 2.0f) * fSpeed * fElapsedTime;;
			}
		}

		// Клавишей "W" идём вперёд
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')  // Если столкнулись со стеной, то откатываем шаг
			{
				fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}

		// Клавишей "S" идём назад
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')  // Если столкнулись со стеной, но откатываем шаг
			{
				fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)  // Проходим по всем X
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;  // Направление луча

			float fStepSize = 0.1f;		  									
			float fDistanceToWall = 0.0f; // Расстояние до препятствия в направлении fRayAngle

			bool bHitWall = false;		// Достигнул ли луч стенку
			bool bBoundary = false;		// Достигнул ли луч границу

			float fEyeX = sinf(fRayAngle); // Координаты единичного вектора fRayAngle
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)  // Пока не столкнулись со стеной или не вышли за радиус видимости
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);  // Точка на игровом поле
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);  // в которую попал луч

				// Если мы вышли за карту, то дальше смотреть нет смысла - фиксируем соударение на расстоянии видимости
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					// Если встретили стену, то заканчиваем цикл
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						bHitWall = true;

						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)  // Проходим по всем 4м рёбрам
							for (int ty = 0; ty < 2; ty++)
							{
								float vy = (float)nTestY + ty - fPlayerY;  // Координаты вектора
								float vx = (float)nTestX + tx - fPlayerX;  // ведущего из наблюдателя в ребро
								float d = sqrt(vx * vx + vy * vy);  // Модуль этого вектора
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);  // Скалярное произведение (единичных векторов)
								p.push_back(make_pair(d, dot));  // Сохраняем результат в массив
							}

						// Мы будем выводить два ближайших ребра, поэтому сортируем их по модулю вектора ребра
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						float fBound = 0.01; // Угол, при котором начинаем различать ребро
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
			}

			//Вычисляем координаты начала и конца стенки
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;
	
			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;  // Если стенка близко, то рисуем 
			else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;  // светлую полоску
			else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;  // Для отдалённых участков 
			else if (fDistanceToWall < fDepth)				nShade = 0x2591;  // рисуем более темную
			else											nShade = ' ';

			if (bBoundary)		nShade = ' ';

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nShade;
				else
				{
					// То же самое с полом - более близкие части рисуем более заметными символами
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

		// Display Map
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'P';

		// Display Frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}
