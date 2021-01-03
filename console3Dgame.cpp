// Inspired by "Code-It-Yourself! First Person Shooter (Quick and Simple C++)"
//                                         link: https://youtu.be/xW8skO7MFYw 
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

int screenWidth = 160;
int screenHeight = 40;

int renderScreenWidth = 119; //screen width - mapwidth -1(to create emply line before map)

float speed = 5;
float sensitivity = 3;
float playerX = 1.0f;
float playerY = 1.0f;
float playerLookAngle = 0.0f;
float FOV = 1.5;
float viewDistance = 16;


template<typename T>
T rand(T min, T max)
{
    static std::mt19937 generator{ std::random_device()() };
    std::uniform_int_distribution<> onRange(min, max);
    return onRange(generator);
}


class Map
{
public:

    Map()
    {
       map += L"#######################################";
       map += L"..#..........#.....#...###............#";
       map += L"#.#.########...#.#...#.....#.####.###.#";
       map += L"#.#.#......#####.#########.#.#..#.#...#";
       map += L"#.#.#.####.#...#.#......#..#....#...#.#";
       map += L"#.....#.#....#...#.####.#.#####.#.#.#.#";
       map += L"##.####.#.#..#####.#....#.#...#.#.###.#";
       map += L"#..#....#.#..#...#.#.####.#.#...#.....#";
       map += L"#..#.##...#..#.#.#.#.#....#.#.#.#..##.#";
       map += L"#.##.#..####.#.#.#.#.#..###.###.##.#..#";
       map += L"#....#.##....#.#...#.##...#........#.##";
       map += L"#.####.#..####.#####......#..#.###.#.##";
       map += L"#.#....#.......#.....#######.#...#.#..#";
       map += L"#.#.####.#######.###....#....###.#....#";
       map += L"#.#........#.......####.#.####...######";
       map += L"#.####.###....#.#.......#....#.#.....##";
       map += L"#......#...##.#.############.#.#####..#";
       map += L"###.#.##.#.#..#.#....#.....#...#...##.#";
       map += L"#...#.#..#.#..#.#.#....#.#.###...#....#";
       map += L"#.#.#.#..#......#.#.####.#...########.#";
       map += L"#.#.#.##.#.######.#......###........#.#";
       map += L"#.#...#..#.#......######...########.#.#";
       map += L"#.#####.##.#.####......#.#........#.#.#";
       map += L"#........#.#...#..##.#.#.########.#.#.#";
       map += L"##.###.#.#.###.#.##..#.#........#.#...#";
       map += L"##.#.#.###.#...#.#..##.########.#.##.##";
       map += L"##.#.#.......#.....###.#........#.....#";
       map += L"#....#.##########....#...#.####.#####.#";
       map += L"#.##...#...#...##.########.#........#.#";
       map += L"#....#.#.#.#.#....#........#.########.#";
       map += L"###.####.#...#.######.######.#........#";
       map += L"#...#..#.#####.#....#.#......#.#.###.##";
       map += L"#.................#.#.#.#.####.#.#.#.##";
       map += L"###.##.######.###.#...#.#......#.#.#.##";
       map += L"#.#.#..#........#.#####.###.####.#....#";
       map += L"#...#..#.##.#####.......#...#....#.#..#";
       map += L"#.#.##.#.#..#...#########.#.#.####.#.##";
       map += L"#........#....#...........#........#...";
       map += L"#######################################";
        //map += L"#######################################";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.........################............#";
        //map += L"#.........#...........................#";
        //map += L"#.........#...........................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#.....................................#";
        //map += L"#######################################";

    }

    wchar_t& operator[](const int& index)
    {
        return map[index];
    }

    int width = 39;
    int height = 39;

    wstring map;

    wchar_t wallChar = '#';
    wchar_t frame = ' ';
    wchar_t enemyChar = 'E';
};

class Enemy
{
public:
    Enemy(Map& m) : map(m)
    {
        lastMovement = chrono::system_clock::now();
    }

    int X = 3;
    int Y = 3;
    int speed = 1; //in seconds

    bool inMovement = false;
    int additionalX = -1;
    int additionalY = -1;

    int lastSpottedX = -1;
    int lastSpottedY = -1;

    Map& map;

    chrono::system_clock::time_point lastMovement;


    void move() //TODO test n fix some AI bugs
    {
        look();

       auto now = chrono::system_clock::now();
       auto elapsed = chrono::duration_cast<chrono::seconds>(now - lastMovement);

       if (elapsed.count() >= speed)
       {
           if (inMovement) // then finish movement
           {
                X = additionalX;  
                Y = additionalY;
                additionalX = -1;
                additionalY = -1;

                inMovement = false;
                lastMovement = now;
           }
           else
           {
               if(lastSpottedX == -1 || lastSpottedY == -1) // if player wasn't spotted
               {
                   //choose random availible direction to move
                   switch (rand(1, 4))
                   {
                   case 1:   // forward
                       if (map[(Y + 1) * map.width + X] != map.wallChar) //check for wall
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X;
                           additionalY = Y + 1;

                       }
                       break;

                   case 2:   // left
                       if (map[Y * map.width + X - 1] != map.wallChar) //check for wall
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X - 1;
                           additionalY = Y;
                       }
                       break;
                   case 3:   // right
                       if (map[Y * map.width + X + 1] != map.wallChar) //check for wall
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X + 1;
                           additionalY = Y;
                       }
                       break;
                   case 4:   // backward 
                       if (map[(Y - 1) * map.width + X] != map.wallChar) //check for wall
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X;
                           additionalY = Y - 1;
                       }
                       break;
                   }
               }
               else
               {
                   if (Y == lastSpottedY)
                   {
                       auto sign = X > lastSpottedX ? -1 : 1;
                       if (map[Y * map.width + X + 1 *sign] != map.wallChar) //check for wall
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X + 1 * sign;
                           additionalY = Y;
                       }
                   }
                   else if (X == lastSpottedX)
                   {
                       auto sign = Y > lastSpottedY ? -1 : 1;
                       if (map[(Y + 1 * sign) * map.width + X] != map.wallChar) //check for wall //bluat
                       {
                           inMovement = true;
                           lastMovement = now;

                           additionalX = X ;
                           additionalY = Y + 1 * sign;
                       }
                   }
               }
           }

           if (X == lastSpottedX && Y == lastSpottedY)
           {
               lastSpottedX = -1;
               lastSpottedY = -1;
           }

       }
    }

    void look()
    {
        
        int testY, testX;

        for (testX = X, testY = Y; map[(testY + 1) * map.width + testX] != map.wallChar; testY++ ) //forward 
        {
            if (spot(testX, testY)) return;
        }

        for (testX = X, testY = Y; map[(testY + 1) * map.width + testX] != map.wallChar; testY--) //backward 
        {
            if (spot(testX, testY)) return;
        }

        for (testX = X, testY = Y; map[testY * map.width + testX ] != map.wallChar; testX--) //left
        {
            if (spot(testX, testY)) return;
        }

        for (testX = X, testY = Y; map[testY * map.width + testX] != map.wallChar; testX++) //right
        {
            if (spot(testX, testY)) return;
        }
        
        if (inMovement) //chek for additional body
        {
            for (testX = additionalX, testY = additionalY; map[(testY + 1) * map.width + testX] != map.wallChar; testY++) //forward 
            {
                if (spot(testX, testY)) return;
            }

            for (testX = additionalX, testY = additionalY; map[(testY + 1) * map.width + testX] != map.wallChar; testY--) //backward 
            {
                if (spot(testX, testY)) return;
            }

            for (testX = additionalX, testY = additionalY; map[testY * map.width + testX] != map.wallChar; testX--) //left
            {
                if (spot(testX, testY)) return;
            }

            for (testX = additionalX, testY = additionalY; map[testY * map.width + testX] != map.wallChar; testX++) //right
            {
                if (spot(testX, testY)) return;
            }
        }

    }
    
    bool spot(int X, int Y)
    {
        if (((int)playerY + 1) * screenWidth + (int)playerX == (Y + 1) * screenWidth + X)
        {
            lastSpottedX = X;
            lastSpottedY = Y;

            return true;
        } 
        return false;
    }
};

enum class HITTYPE
{
    NOHIT,
    HIT_WALL,
    HIT_ENEMY
};

int main()
{
    // creating a screen buffer 
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    HANDLE hConsoleBuff = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
    SetConsoleActiveScreenBuffer(hConsoleBuff);
    DWORD dwBytesWritten = 0;


    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    Map map;
    Enemy monster(map);

    while (true) // main game loop
    {
        //game timer
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float deltaTime = elapsedTime.count();
        
        
        //movement component

        //camera 
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
            playerLookAngle -= sensitivity * deltaTime;

        if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
            playerLookAngle += sensitivity * deltaTime;

        //walking
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            playerX += sinf(playerLookAngle) * speed * deltaTime;
            playerY += cosf(playerLookAngle) * speed * deltaTime;

            if (map[(int)playerY * map.width + (int)playerX] == map.wallChar)     //collision detection 
            {
                playerX -= sinf(playerLookAngle) * speed * deltaTime;
                playerY -= cosf(playerLookAngle) * speed * deltaTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) //TODO fix sidewalking 
        {
            playerX -= cosf(playerLookAngle) * speed * deltaTime;
            playerY -= sinf(playerLookAngle) * speed * deltaTime;

            if (map[(int)playerY * map.width + (int)playerX] == map.wallChar)   //collision detection 
            {
                playerX += cosf(playerLookAngle) * speed * deltaTime;
                playerY += sinf(playerLookAngle) * speed * deltaTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            playerX -= sinf(playerLookAngle) * speed * deltaTime;
            playerY -= cosf(playerLookAngle) * speed * deltaTime;

            if (map[(int)playerY * map.width + (int)playerX] == map.wallChar)   //collision detection 
            {
                playerX += sinf(playerLookAngle) * speed * deltaTime;
                playerY += cosf(playerLookAngle) * speed * deltaTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
           playerX += cosf(playerLookAngle) * speed * deltaTime;
            playerY += sinf(playerLookAngle) * speed * deltaTime;

            if (map[(int)playerY * map.width + (int)playerX] == map.wallChar)   //collision detection 
            {
                playerX -= cosf(playerLookAngle) * speed * deltaTime;
                playerY -= sinf(playerLookAngle) * speed * deltaTime;
            }
        }
        
        monster.move();
        // movement end.

        for (int x = 0; x < renderScreenWidth; x++) // computation to each column
        {
            float rayAngle = (playerLookAngle - FOV / 2.0f)
                + ((float)x / (float)renderScreenWidth) * FOV;

            float distanceToWall = 0.0f;

            bool anyHitted = false,
                 boundary = false;
            

            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);
            HITTYPE hit = HITTYPE::NOHIT;


            while (!anyHitted && distanceToWall < viewDistance)
            {
                distanceToWall += 0.1f; //making step

                int testX = (int)(playerX + eyeX * distanceToWall);
                int testY = (int)(playerY + eyeY * distanceToWall);

                if (testX < 0 || testX >= map.width || //test if out of bounce 
                    testY < 0 || testY >= map.height)
                {
                    anyHitted = true;
                    distanceToWall = viewDistance;
                }
                else
                {
                    if (map[testY * map.width + testX] == map.wallChar) //wall render
                    {
                        anyHitted = true;
                        hit = HITTYPE::HIT_WALL;


                        vector<pair<float, float>> p;
                        for (int tx = 0; tx < 2; tx++)
                        {
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = (float)testY + ty - playerY;
                                float vx = (float)testX + tx - playerX;

                                float d = sqrt(vx * vx + vy * vy);
                                float dot = (eyeX * vx / d) + (eyeY * vy / d);
                                
                                p.push_back(make_pair(d, dot));
                            }
                        }

                        sort(p.begin(), p.end(), [](const pair<float, float>& a, const pair<float, float>& b)
                                                    {
                                                        return a.first < b.first;
                                                    });
                         float bound = 0.01f;
                         if (acos(p.at(0).second) < bound ||
                             acos(p.at(1).second) < bound )
                             boundary = true;
                    }

                    if (testY == monster.Y && testX == monster.X ||
                        testY == monster.additionalY && testX == monster.additionalX) //enemyRender
                    {
                        anyHitted = true;
                        hit = HITTYPE::HIT_ENEMY;

                    }
                }
            }

            int ceiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
            int floor = screenHeight - ceiling;

            short texture = ' ';


            for (int y = 0; y < screenHeight; y++)
            {
                if (y <= ceiling)
                    screen[y * screenWidth + x] = ' '; //draw sky
                else if (y > ceiling && y < floor)
                {
                    if (hit == HITTYPE::HIT_WALL)
                    {
                        if (boundary)                                       texture = ' ';
                        else if (distanceToWall <= viewDistance / 4.0f)	    texture = 0x2588;	// Very close	
                        else if (distanceToWall < viewDistance / 3.0f)		texture = 0x2593;
                        else if (distanceToWall < viewDistance / 2.0f)		texture = 0x2592;
                        else if (distanceToWall < viewDistance)				texture = 0x2591;
                        else											    texture = ' ';		// Too far away
                    }
                    else if (hit == HITTYPE::HIT_ENEMY)
                    {
                        switch (rand(1, 3))
                        {
                        case 1:
                        case 2:
                            texture = ' ';
                            break;
                        case 3:
                            texture = 0x2127; //TODO workout propper symbol
                            break;
                        }
                    }
                    else
                        texture = ' ';		// Too far away

                    screen[y * screenWidth + x] = texture; // draw wall

                }
                else if (y >= floor)
                {
                    float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
                    if (b < 0.25)		texture = '#';
                    else if (b < 0.5)	texture = 'x';
                    else if (b < 0.75)	texture = '.';
                    else if (b < 0.9)	texture = '-';
                    else				texture = ' ';

                    screen[y * screenWidth + x] = texture; //draw floor
                }
                else
                   screen[y * screenWidth + x] = '?'; //render error
            }

        }

        //player info output
        swprintf_s(screen, 40, L"X=%3.2f , Y=%3.2f, A=%3.2f", playerX, playerY, playerLookAngle);

        //map
        for (int tx = -1; tx < map.width; tx++)
        {
            for (int ty = 0; ty < map.height; ty++)
            {
                if (tx < 0)
                    screen[(ty + 1) * screenWidth + tx + 120] = map.frame;
                else
                    screen[(ty+1)*screenWidth + tx + 120] = map[ty * map.width + tx];
            }
        }

        screen[((int)playerY +1) *  screenWidth + (int)playerX + 120] = 'P';
        screen[((int)monster.Y + 1) * screenWidth + (int)monster.X + 120] = map.enemyChar;
       // if(monster.inMovement)
        screen[((int)monster.additionalY + 1) * screenWidth + (int)monster.additionalX + 120] = map.enemyChar;


        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsoleBuff, screen, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}