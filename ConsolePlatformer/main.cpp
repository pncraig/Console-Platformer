/*
* Created on 1/4/2021 at 10:49 PM
* Edited on 1/5/2021 at 1:19 PM
*/

#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 30;

float gravity = 0.5;
float friction = 0.1;

float fPlayerX;
float fPlayerY;
float fPlayerVelX = 0.0f;
float fPlayerVelY = 0.0f;

struct map {
	wstring layout;
	int width;
	int height;
	int playerStartX;
	int playerStartY;
};

float clamp(float, float, float);

int main() {
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hconsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hconsole);
	DWORD dwBytesWritten = 0;

	map map1;
	map1.width = 16;
	map1.height = 16;
	map1.playerStartX = 2;
	map1.playerStartY = 15;
	map1.layout += L"%%%%%%%%%%%%%%%%";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%              %";
	map1.layout += L"%O            @%";
	map1.layout += L"%%%%%%%%%%%%%%%%";

	int nCurrentMapIndex = 0;
	map maps[] = { map1 };

	map& currentMap = maps[nCurrentMapIndex];
	for (;;) {
		this_thread::sleep_for(50ms);
		for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
			screen[i] = L' ';

		currentMap = maps[nCurrentMapIndex];

		/*Display the game ----------------------------------------------------------------------*/
		for (int x = 0; x < currentMap.width; x++) {
			for (int y = 0; y < currentMap.height; y++) {
				screen[y * nScreenWidth + x] = currentMap.layout[y * currentMap.width + x];
			}
		}

		screen[nScreenWidth * nScreenHeight] = L'\0';
		WriteConsoleOutputCharacter(hconsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
	}

	return 0;
}

float clamp(float x, float lo, float hi) {
	if (x < lo)
		return lo;
	else if (x > hi)
		return hi;
	else
		return x;
}