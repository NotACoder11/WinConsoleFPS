// Inspired by "Code-It-Yourself! First Person Shooter (Quick and Simple C++)"
//                                         link: https://youtu.be/xW8skO7MFYw 
//

#include <iostream>
#include <Windows.h>
#include <chrono>

using namespace std;

int screenWidth = 120;
int screenHeight = 40;

float playerX = 8.0f;
float playerY = 8.0f;
float playerLookAngle = 0.0f;
float FOV = 3.14159f / 4.0f;
float viewDistance = 16;

int mapWidth = 16;
int mapHeight = 16;

int main()
{
    // creating a screen buffer 
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    HANDLE hConsoleBuff = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
    SetConsoleActiveScreenBuffer(hConsoleBuff);
    DWORD dwBytesWritten = 0;

    //creating map
    wstring map;

    map += L"################";
    map += L"#......##......#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......#......#";
    map += L"#..............#";
    map += L"#.......#......#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    while (true) // main game loop
    {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float deltaTime = elapsedTime.count();
        
        
        //movement component
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            playerLookAngle -= 0.5 * deltaTime;

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            playerLookAngle += 0.5 * deltaTime;

      
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            playerX += sinf(playerLookAngle) * 0.5 * deltaTime;
            playerY += cosf(playerLookAngle) * 0.5 * deltaTime;

            //collision detection 
            if (map[playerY * mapWidth + playerX] == '#')
            {
                playerX -= sinf(playerLookAngle) * 0.5 * deltaTime;
                playerY -= cosf(playerLookAngle) * 0.5 * deltaTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            playerX -= sinf(playerLookAngle) * 0.5 * deltaTime;
            playerY -= cosf(playerLookAngle) * 0.5 * deltaTime;

            if (map[playerY * mapWidth + playerX] == '#')
            {
                playerX += sinf(playerLookAngle) * 0.5 * deltaTime;
                playerY += cosf(playerLookAngle) * 0.5 * deltaTime;
            }
        }

        for (int x = 0; x < screenWidth; x++) //doing the computation to each column
        {
            float rayAngle = (playerLookAngle - FOV / 2.0f)
                + ((float)x / (float)screenWidth) * FOV;

            float distanceToWall = 0.0f;
            bool wallHitted = false;

            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);


            while (!wallHitted && distanceToWall < viewDistance)
            {
                distanceToWall += 0.1f; //making step

                int testX = (int)(playerX + eyeX * distanceToWall);
                int testY = (int)(playerY + eyeY * distanceToWall);

                if (testX < 0 || testX >= mapWidth || //test if out of bounce 
                    testY < 0 || testY >= mapHeight)
                {
                    wallHitted = true;
                    distanceToWall = viewDistance;
                }
                else
                {
                    if (map[testY * mapWidth + testX] == '#')
                    {
                        wallHitted = true;
                    }
                }
            }

            int ceiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
            int floor = screenHeight - ceiling;

            short shade = ' ';

            


            for (int y = 0; y < screenHeight; y++)
            {
                if (y <= ceiling)
                    screen[y * screenWidth + x] = ' '; //draw sky
                else if (y > ceiling && y < floor)
                {
                    if (distanceToWall <= viewDistance / 4.0f)	        shade = 0x2588;	// Very close	
                    else if (distanceToWall < viewDistance / 3.0f)		shade = 0x2593;
                    else if (distanceToWall < viewDistance / 2.0f)		shade = 0x2592;
                    else if (distanceToWall < viewDistance)				shade = 0x2591;
                    else											    shade = ' ';		// Too far away

                    screen[y * screenWidth + x] = shade; // draw wall

                }
                else if (y >= floor)
                {
                    float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
                    if (b < 0.25)		shade = '#';
                    else if (b < 0.5)	shade = 'x';
                    else if (b < 0.75)	shade = '.';
                    else if (b < 0.9)	shade = '-';
                    else				shade = ' ';

                    screen[y * screenWidth + x] = shade; //draw floor
                }
                else
                   screen[y * screenWidth + x] = '?'; //render error
            }

        }

        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsoleBuff, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}
