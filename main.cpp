#include "include/raylib.h"
#include <stdio.h>
#include <dirent.h>

#define BUFFER_LENGTH 100

int main()
{
    int window_width  = 1920;
    int window_height = 1080;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);


    Texture2D face = LoadTexture("../assets/face1.png");
    Texture2D eyes = LoadTexture("../assets/eyes1.png");
    Texture2D hat = LoadTexture("../assets/hat1.png");

    char *buffer[BUFFER_LENGTH];

    //NOTE: Read all files in a directory
    {
        DIR *d;
        struct dirent *dir;
        d = opendir("../assets");

        if(d)
        {
            int index = 0;
            while((dir = readdir(d)) != NULL)
            {
                buffer[index] = dir->d_name;
                index++;
            }
            closedir(d);
        }
    }

    for(int i = 0; i < 30; i++)
    {
        if(i != NULL)
        {
            printf("%s\n", buffer[i]);
        }
    }

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(face, 0, 0, RAYWHITE);
        DrawTexture(eyes, 0, 0, RAYWHITE);
        DrawTexture(hat, 0, 0, RAYWHITE);

        EndDrawing();
    }
}
