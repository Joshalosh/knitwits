#include "include/raylib.h"
#include "include/dirent.h"
#include "stdio.h"
//#include "windows.h"

int main()
{
    int window_width  = 1920;
    int window_height = 1080;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);


    Texture2D face = LoadTexture("../assets/face1.png");
    Texture2D eyes = LoadTexture("../assets/eyes1.png");
    Texture2D hat  = LoadTexture("../assets/hat1.png");


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
#if 1
    {
        DIR *d;
        struct dirent *dir;
        d = opendir("../assets");
        if(d)
        {
            while((dir = readdir(d)) != NULL)
            {
                printf("%s\n", dir->d_name);
            }
            closedir(d);
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
