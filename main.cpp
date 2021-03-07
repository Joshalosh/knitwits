#include "include/raylib.h"
#include "include/dirent.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#define internal static 
#define global_variable static
#define local_persist static

#define MAX_BUFFER 100

internal int Directory_Count(char *filepath)
{
    DIR *d;
    struct dirent *dir;
    
    int file_count = 0;
    d = opendir(filepath);
    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            file_count++;
        }
        closedir(d);
        file_count -= 1;
        return file_count;
    }
    else
    {
        return file_count;
    }
}

internal char *Randomised_Asset(char *filepath, int max_assets)
{
    DIR *d;
    struct dirent *dir;

    int num_count = 0;
    int rand_num  = (rand() % (max_assets - 2 + 1)) + 2;

    char *random_asset = (char *)malloc(MAX_BUFFER);
    memset(random_asset, 0, MAX_BUFFER);

    d = opendir(filepath);
    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            if(num_count == rand_num)
            {
               strcpy(random_asset, dir->d_name);
            }
            num_count++;
        }
        closedir(d);
        return random_asset;
    }
}


int main()
{
    int window_width  = 1920;
    int window_height = 1080;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);

    srand(time(0));


    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        global_variable float delta_time = 0.0f;
        delta_time += GetFrameTime();

        if(IsKeyDown(KEY_UP))
        {
            char face_filepath[MAX_BUFFER] = "../assets/face/";
            char eyes_filepath[MAX_BUFFER] = "../assets/eyes/";
            char hat_filepath[MAX_BUFFER]  = "../assets/hat/";

            int max_faces = Directory_Count(face_filepath);
            int max_eyes  = Directory_Count(eyes_filepath);
            int max_hats  = Directory_Count(hat_filepath);

            char *face_asset = Randomised_Asset(face_filepath, max_faces);
            char *eyes_asset = Randomised_Asset(eyes_filepath, max_eyes);
            char *hat_asset  = Randomised_Asset(hat_filepath, max_hats);

            strcat(face_filepath, face_asset);
            strcat(eyes_filepath, eyes_asset);
            strcat(hat_filepath, hat_asset);

            free(face_asset);
            free(eyes_asset);
            free(hat_asset);

            Texture2D face = LoadTexture(face_filepath);
            Texture2D eyes = LoadTexture(eyes_filepath);
            Texture2D hat  = LoadTexture(hat_filepath);

            DrawTexture(face, 0, 0, RAYWHITE);
            DrawTexture(eyes, 0, 0, RAYWHITE);
            DrawTexture(hat, 0, 0, RAYWHITE);
        }

        EndDrawing();
    }
}
