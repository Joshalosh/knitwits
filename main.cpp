#include "include/raylib.h"

int main()
{
    int WindowWidth  = 1280;
    int WindowHeight = 720;

    InitWindow(WindowWidth, WindowHeight, "MyGame");

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        static float rotation = 0.0f;
        rotation += 1.0f;

        static float timer = 0.0f;
        static int sides   = 0;

        timer += GetFrameTime();
        if (timer > 0.2f)
        {
            timer = 0.0f;
            sides++;
        }

        DrawRectangle(100, 100, 100, 300, RED);
        DrawRectangleV(Vector2{300, 200}, Vector2{100, 30}, BLUE);
        //DrawRectangleRec(Rectangle{200, 200, 100, 100}, GREEN);
        DrawRectanglePro(Rectangle{200, 200, 100, 100}, Vector2{50, 50}, rotation, GREEN);

        DrawRectangleLinesEx(Rectangle{400, 200, 100, 100}, 25, BLACK);

        DrawPoly(Vector2{500, 200}, sides % 12, 64, (rotation+rotation), BLUE);

        Vector2 vertices[] = {Vector2{30, 0}, Vector2{0, 100},
                              Vector2{100, 120}, Vector2{120, 50},
                              Vector2{80, 20}, Vector2{150, 80}};

        DrawTriangleFan(vertices, sizeof(vertices) / sizeof(vertices[0]), PURPLE);
        EndDrawing();
    }
}
