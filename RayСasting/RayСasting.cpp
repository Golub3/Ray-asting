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
        // Повторяющиеся действия
    }
    return 0;
}


