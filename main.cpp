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

internal char *Randomised_Asset(int *asset_id, char *filepath, int max_assets)
{
    DIR *d;
    struct dirent *dir;

    int num_count = 0;
    int rand_num  = (rand() % (max_assets - 2 + 1)) + 2;
    *asset_id = rand_num;

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

struct Combination
{
    int face;
    int eyes;
    int hat;
    int id;
};


int main()
{
    int window_width  = 1920;
    int window_height = 1080;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);

    srand(time(0));

    bool first_time_load_texture = true;
    Texture2D face = {};
    Texture2D eyes = {};
    Texture2D hat  = {};

    int combination_index = 0;
    int export_count      = 1;


    int unique_combinations[MAX_BUFFER] = {};

    Combination combination = {};

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        bool unique = false;
        while(!unique)
        {
            if(first_time_load_texture)
            {
                // NOTE: The first time we load a texture, face, eyes and hat haven't
                // been set yet so no need to unload any textures.
                first_time_load_texture = false;
            }
            else
            {
                // NOTE: Make sure to clean up the old texture since we're going
                // to load a new one in.
                UnloadTexture(face);
                UnloadTexture(eyes);
                UnloadTexture(hat);
            }

            // NOTE: Set up the textures to draw.
            {
                char face_filepath[MAX_BUFFER] = "../assets/face/";
                char eyes_filepath[MAX_BUFFER] = "../assets/eyes/";
                char hat_filepath[MAX_BUFFER]  = "../assets/hat/";

                int max_faces = Directory_Count(face_filepath);
                int max_eyes  = Directory_Count(eyes_filepath);
                int max_hats  = Directory_Count(hat_filepath);

                char *face_asset = Randomised_Asset(&combination.face, face_filepath, max_faces);
                char *eyes_asset = Randomised_Asset(&combination.eyes, eyes_filepath, max_eyes);
                char *hat_asset  = Randomised_Asset(&combination.hat, hat_filepath, max_hats);

                strcat(face_filepath, face_asset);
                strcat(eyes_filepath, eyes_asset);
                strcat(hat_filepath, hat_asset);

                free(face_asset);
                free(eyes_asset);
                free(hat_asset);

                face = LoadTexture(face_filepath);
                eyes = LoadTexture(eyes_filepath);
                hat  = LoadTexture(hat_filepath);
            }

            // NOTE: Check the overall asset for uniqueness.
            combination.id = combination.hat + (combination.eyes*10) + (combination.face*100);
            for(int array_index = 0; array_index < export_count; array_index++)
            {
                if(combination.id == unique_combinations[array_index])
                {
                    unique = false;
                    break;
                }
                else
                {
                    unique = true;
                }
            }

            if(unique)
            {
                unique_combinations[combination_index] = combination.id;
                combination_index++;
            }
        }

        DrawTexture(face, 0, 0, RAYWHITE);
        DrawTexture(eyes, 0, 0, RAYWHITE);
        DrawTexture(hat,  0, 0, RAYWHITE);
        EndDrawing();

        // NOTE: Print to console for debugging.
        {
            printf("face =\t%d\neyes =\t%d\nhat =\t%d\nid =\t%d\n", combination.face, combination.eyes,
                                                                    combination.hat, combination.id);
        }


#if 1
        // NOTE: Export some random pics.
        if(export_count < MAX_BUFFER)
        {
            Image gen = GetScreenData();
            char export_path[MAX_BUFFER] = "../assets/exports/";
            char *file_type = ".png";
            char file[10];
            sprintf(file, "%d", export_count);
            strcat(export_path, file);
            strcat(export_path, file_type);
            ExportImage(gen, export_path);
            export_count++;
        }
#endif
        // NOTE: Print array of unique combinations for debugging.
        if(export_count >= MAX_BUFFER)
        {
            for(int i = 0; i < export_count; i++)
            {
                printf("%d\t--->\t%d\n", i, unique_combinations[i]);
            }
        }
        
    }
}
