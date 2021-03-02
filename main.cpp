#include "include/raylib.h"
#include "include/dirent.h"
#include "stdio.h"
//#include "windows.h"

#define internal static 
#define global_variable static
#define local_persist static

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
        file_count -= 2;
        return file_count;
    }
    else
    {
        return file_count;
    }
}


int main()
{
    int window_width  = 1920;
    int window_height = 1080;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);


    Texture2D face = LoadTexture("../assets/face/face1.png");
    Texture2D eyes = LoadTexture("../assets/eyes/eyes1.png");
    Texture2D hat  = LoadTexture("../assets/hat/hat1.png");
    
    char *face_filepath("../assets/face");
    char *eyes_filepath("../assets/eyes");
    char *hat_filepath("../assets/hat");

    int max_faces = Directory_Count(face_filepath);
    int max_eyes = Directory_Count(eyes_filepath);
    int max_hats = Directory_Count(hat_filepath);

    //NOTE: Read all files in a directory
#if 0
    WIN32_FIND_DATA data;
    HANDLE hFind; 
    LPCWSTR file = L"*.png";
    hFind = FindFirstFile(file, &data);

    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            printf("%ls\n", data.cFileName);
        }
        while(FindNextFile(hFind, &data));
        {
            FindClose(hFind);
        }
    }
#endif
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
