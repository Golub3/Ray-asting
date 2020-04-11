using namespace std;
#include <iostream>
#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 40;
int nMapHeight = 16;
int nMapWidth = 16;

float fPlayerX = 1.0; 
float fPlayerY = 1.0;
float fPlayerA = 0.0; // Направление игрока
float fFOV = 3.14159 / 3; // Угол обзора
float fDepth = 30.0f; // Дистанция обзора

int main() {
    // Вывод на экран
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight + 1]; // Массив для записи в буфер
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // Буфер экрана
    SetConsoleActiveScreenBuffer(hConsole); // Настройка консоли
    DWORD dwBytesWritten = 0; // Для дебага

    wstring map; // Строковый массив
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    while (1) // Игровой цикл
    {
        auto tp1 = chrono::system_clock::now(); // Переменные для подсчета
        auto tp2 = chrono::system_clock::now(); // пройденного времени

        while (1) // Игровой цикл
        {
            tp2 = chrono::system_clock::now();
            chrono::duration <float> elapsedTime = tp2 - tp1;
            tp1 = tp2;
            float fElapsedTime = elapsedTime.count();

            if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
                fPlayerA -= (1.5f) * fElapsedTime; // Клавишей "A" поворачиваем по часовой стрелке

            if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
                fPlayerA += (1.5f) * fElapsedTime; // Клавишей "D" поворачиваем против часовой стрелки

            if (GetAsyncKeyState((unsigned short)'W') & 0x8000) // Клавишей "W" идём вперёд
            {
                fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

                if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') { // Если столкнулись со стеной, но откатываем шаг
                    fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                    fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
                }
            }

            if (GetAsyncKeyState((unsigned short)'S') & 0x8000) // Клавишей "S" идём назад
            {
                fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
                if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') { // Если столкнулись со стеной, но откатываем шаг
                    fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
                    fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
                }
            }

            for (int x = 0; x < nScreenWidth; x++) // Проходим по всем X
            {
                float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV; // Направление луча
                // Находим расстояние до стенки в направлении fRayAngle 

                float fDistanceToWall = 0.0f; // Расстояние до препятствия в направлении fRayAngle
                bool bHitWall = false; // Достигнул ли луч стенку

                float fEyeX = sinf(fRayAngle); // Координаты единичного вектора fRayAngle
                float fEyeY = cosf(fRayAngle);

                while (!bHitWall && fDistanceToWall < fDepth) // Пока не столкнулись со стеной
                {                                           // Или не вышли за радиус видимости
                    fDistanceToWall += 0.1f;

                    int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall); // Точка на игровом поле
                    int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall); // в которую попал луч

                    if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                    { // Если мы вышли за зону
                        bHitWall = true;
                        fDistanceToWall = fDepth;
                    }
                    else if (map[nTestY * nMapWidth + nTestX] == '#')
                        bHitWall = true;

                    for (int y = 0; y < nScreenHeight; y++) // При заданном X проходим по всем Y
                    {
                        // В этом цикле рисуется вертикальная полоска
                    }
                }
        }
    }
    return 0;
}


