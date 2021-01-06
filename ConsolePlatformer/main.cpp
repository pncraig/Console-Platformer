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

float gravity = 0.03f;

float fPlayerX;
float fPlayerY;
int nPrevPlayerX;
int nPrevPlayerY;
float fPlayerVelX = 1.0f;
float fPlayerVelY = 0.0f;
float fPlayerJumpSpeed = 0.2f;
bool bIsJumping = false;
bool bIsTouchingGround = true;
int nJumpTimer = 0;
int nJumpLength = 3;

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
	map1.playerStartY = 14;
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
	map1.layout += L"% %%%          %";
	map1.layout += L"%              %";
	map1.layout += L"%       %     @%";
	map1.layout += L"%%%%%%%%%%%%%%%%";

	int nCurrentMapIndex = 0;
	map maps[] = { map1 };

	map currentMap = maps[nCurrentMapIndex];
	fPlayerX = (float)currentMap.playerStartX;
	fPlayerY = (float)currentMap.playerStartY;

	for (;;) {
		this_thread::sleep_for(25ms);
		// Clear the screen
		for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
			screen[i] = L' ';
		// Reset the map
		currentMap.layout = maps[nCurrentMapIndex].layout;

		/*Move the player character -------------------------------------------------------------*/

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			nPrevPlayerX = (int)fPlayerX;
			fPlayerX -= fPlayerVelX;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			nPrevPlayerX = (int)fPlayerX;
			fPlayerX += fPlayerVelX;
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			if (bIsTouchingGround) {
				while (nJumpTimer < nJumpLength) {
					fPlayerVelY -= fPlayerJumpSpeed;
					nJumpTimer++;
				}
			}
		} else 
			nJumpTimer = 0;
		
		bIsTouchingGround = false;
		fPlayerY += fPlayerVelY;
		fPlayerVelY += gravity;

		fPlayerY = clamp(fPlayerY, 0.0f, 14.0f);

		/*Collisions ----------------------------------------------------------------------------*/

		for (int x = 0; x < currentMap.width; x++) {
			for (int y = 0; y < currentMap.height; y++) {
				wchar_t currentBlock = currentMap.layout[y * currentMap.width + x];
				
				if (currentBlock == ' ')
					continue;

				if (currentBlock == '%') {
					if ((int)fPlayerX == x) {
						if((int)fPlayerY == y)
							fPlayerX = (float)nPrevPlayerX;

						if ((int)fPlayerY == y - 1) {
							fPlayerVelY *= -DBL_EPSILON;
							bIsTouchingGround = true;
						}
						if ((int)fPlayerY == y + 1) 
							fPlayerVelY += 1;
					}
				}
			}
		}

		/*Display the game ----------------------------------------------------------------------*/

		// Update the position of the player character on the map

		currentMap.layout[(int)fPlayerY * currentMap.width + (int)fPlayerX] = L'O';
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