#include "include/raylib.h"
#include <stdio.h>

int main()
{
    int window_width  = 900;
    int window_height = 600;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);

    Texture2D scarfy = LoadTexture("../assets/scarfy.png");

    float scarfy_frame_width = scarfy.width / 6;
    float timer = 0.0f;

    int frame = 0;
    int scarfy_max_frames = (int)(scarfy.width / (int)scarfy_frame_width);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        timer += GetFrameTime();

        if(timer >= 0.2f)
        {
            timer = 0.0f;
            frame++;
        }

        frame = frame % scarfy_max_frames;
        printf("%d\n",frame);

        DrawTextureRec(scarfy,
                       Rectangle{(scarfy_frame_width * frame),
                                 0, scarfy_frame_width,
                                 (float)scarfy.height},
                       Vector2{20, 20}, RAYWHITE);

        EndDrawing();
    }
}
