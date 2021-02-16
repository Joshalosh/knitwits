#include "include/raylib.h"
#include <stdio.h>

int main()
{
    int window_width  = 900;
    int window_height = 600;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);

    Texture2D scarfy = LoadTexture("../assets/scarfy.png");

    float x = 32.0f, y = 32.0f;

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);


        if(IsGamepadAvailable(GAMEPAD_PLAYER1))
        {
            if(IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            {
                x += 30.0f;
            }

            float left_joystick_x = GetGamepadAxisMovement(GAMEPAD_PLAYER1, GAMEPAD_AXIS_LEFT_X);

            x += GetFrameTime() * 100.0f * left_joystick_x;
        }

        DrawTexturePro(scarfy,
                       Rectangle{0, 0, 128, 128},
                       Rectangle{x, y, 128, 128},
                       Vector2{0, 0}, 0, RAYWHITE);

        EndDrawing();
    }
}
