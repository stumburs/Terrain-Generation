#include <raylib.h>
#include "PerlinNoise.h"
#include "Functions.h"
#include <iostream>

const int WIDTH = 1920;
const int HEIGHT = 1080;

const int SCALE = 10;
const int WATERLEVEL = 0;
const double BEACHHEIGHT = 2;
const int WORLDMAXHEIGHT = 10;
const int WORLDWIDTH = WIDTH / SCALE;
const int WORLDHEIGHT = HEIGHT / SCALE;
const double AMPLITUDE = 0.04;
const double DETAIL = 8;

double worldData[WORLDWIDTH][WORLDHEIGHT] = { 0 };

int main()
{

    InitWindow(100, 100, "Terrain Gen");
    int display = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    ToggleFullscreen();
    SetTargetFPS(60);

    srand(time(NULL));
    SetRandomSeed(rand());

    const siv::PerlinNoise::seed_type seedInt = GetRandomValue(-999999, 999999);
    const siv::PerlinNoise perlin{ seedInt };

    int xoff = 0;
    int yoff = 0;
    int camSpeed = 150;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Update
        if (IsKeyDown(KEY_D)) xoff += camSpeed * GetFrameTime();
        if (IsKeyDown(KEY_A)) xoff -= camSpeed * GetFrameTime();
        if (IsKeyDown(KEY_S)) yoff += camSpeed * GetFrameTime();
        if (IsKeyDown(KEY_W)) yoff -= camSpeed * GetFrameTime();

        // Render
        BeginDrawing();
        {
            ClearBackground(BLACK);


            for (int x = 0; x < WORLDWIDTH; x++)
            {
                for (int y = 0; y < WORLDHEIGHT; y++)
                {

                    // Gen world
                    worldData[x][y] = Map(perlin.octave2D_01(((x + xoff) * AMPLITUDE), ((y + yoff) * AMPLITUDE), DETAIL), 0, 1, -WORLDMAXHEIGHT, WORLDMAXHEIGHT);

                    unsigned char red = 0, green = 0, blue = 0;
                    // Water
                    if (worldData[x][y] <= WATERLEVEL)
                    {
                        red = (unsigned char)Map(worldData[x][y], -WORLDMAXHEIGHT, WATERLEVEL, 0, 50);
                        green = (unsigned char)Map(worldData[x][y], -WORLDMAXHEIGHT, WATERLEVEL, 0, 50);
                        blue = (unsigned char)Map(worldData[x][y], -WORLDMAXHEIGHT, WATERLEVEL, 100, 255);
                    }
                    // Sand
                    else if (worldData[x][y] > WATERLEVEL && worldData[x][y] <= WATERLEVEL + BEACHHEIGHT)
                    {
                        red = (unsigned char)Map(worldData[x][y], WATERLEVEL, WATERLEVEL + BEACHHEIGHT, 255, 50);
                        green = (unsigned char)Map(worldData[x][y], WATERLEVEL, WATERLEVEL + BEACHHEIGHT, 255, 120);
                        blue = (unsigned char)Map(worldData[x][y], WATERLEVEL, WATERLEVEL + BEACHHEIGHT, 100, 50);
                    }
                    // Grass
                    else if (worldData[x][y] > WATERLEVEL + BEACHHEIGHT && worldData[x][y] <= WORLDMAXHEIGHT / 1.9)
                    {
                        red = (unsigned char)Map(worldData[x][y], WATERLEVEL + BEACHHEIGHT, WORLDMAXHEIGHT / 1.9, 50, 70);
                        green = (unsigned char)Map(worldData[x][y], WATERLEVEL + BEACHHEIGHT, WORLDMAXHEIGHT / 1.9, 120, 180);
                        blue = (unsigned char)Map(worldData[x][y], WATERLEVEL + BEACHHEIGHT, WORLDMAXHEIGHT / 1.9, 50, 70);
                    }
                    // Mountain Start
                    else if(worldData[x][y] > WORLDMAXHEIGHT / 1.9 && worldData[x][y] <= WORLDMAXHEIGHT / 1.1)
                    {
                        red = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.9, WORLDMAXHEIGHT / 1.1, 70, 140);
                        green = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.9, WORLDMAXHEIGHT / 1.1, 180, 140);
                        blue = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.9, WORLDMAXHEIGHT / 1.1, 70, 140);
                    }
                    // Mountain top
                    else
                    {
                        red = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.1, WORLDMAXHEIGHT, 140, 255);
                        green = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.1, WORLDMAXHEIGHT, 140, 255);
                        blue = (unsigned char)Map(worldData[x][y], WORLDMAXHEIGHT / 1.1, WORLDMAXHEIGHT, 140, 255);
                    }

                    //DrawPixel(x , y , { red, green, blue, 255 });
                    DrawRectangle(x + SCALE * x, y + SCALE * y, SCALE + 1, SCALE + 1, { red, green, blue, 255 });

                }
            }
            DrawText(TextFormat("SEED: %i", seedInt), 20, 20, 20, RAYWHITE);
            DrawText(TextFormat("FPS: %i", GetFPS()), 20, 40, 20, RAYWHITE);
            DrawText(TextFormat("X: %i Y: %i", xoff, -yoff), 20, 60, 20, RAYWHITE);
            DrawText(TextFormat("WASD TO MOVE"), 20, 80, 20, RAYWHITE);

        }
        EndDrawing();
    }
    CloseWindow();
}