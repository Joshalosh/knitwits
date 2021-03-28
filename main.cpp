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

internal void Choose_Rarity(int *rarity_group, char *filepath )
{
    int rand_num = (rand() % (100 - 0 + 1)) + 0;
    *rarity_group = 0;

    if(rand_num == 0 || rand_num == 100)
    {
        strcat(filepath, "ultimate/");
        *rarity_group = 40;
    }
    else if(rand_num >= 91 && rand_num <= 99)
    {
        strcat(filepath, "epic/");
        *rarity_group = 30;
    }
    else if(rand_num >= 71 && rand_num <= 90)
    {
        strcat(filepath, "rare/");
        *rarity_group = 20;
    }
    else
    {
        strcat(filepath, "common/");
        *rarity_group = 10;
    }
}

internal char *Randomised_Asset(int *asset_id, int rarity_group, char *filepath)
{
    DIR *d;
    struct dirent *dir;

    int num_count  = 0;
    int max_assets = Directory_Count(filepath);

    int rand_num  = (rand() % (max_assets - 2 + 1)) + 2;
    *asset_id = rand_num + rarity_group;

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

    int export_count = 1;
    int combo_count  = 1;
    int combo_index  = 0;


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
                // The first time we load a texture, face, eyes and hat haven't
                // been set yet so no need to unload any textures.
                first_time_load_texture = false;
            }
            else
            {
                // Make sure to clean up the old texture since we're going
                // to load a new one in.
                UnloadTexture(face);
                UnloadTexture(eyes);
                UnloadTexture(hat);
            }

            // Prepare the textures for drawing to the screen.
            {
                char face_filepath[MAX_BUFFER] = "../assets/face/";
                char eyes_filepath[MAX_BUFFER] = "../assets/eyes/";
                char hat_filepath[MAX_BUFFER]  = "../assets/hat/";

                int face_rarity = 0;
                int eyes_rarity = 0;
                int hat_rarity  = 0;
                Choose_Rarity(&face_rarity, face_filepath);
                Choose_Rarity(&eyes_rarity, eyes_filepath);
                Choose_Rarity(&hat_rarity,  hat_filepath);

                char *face_asset = Randomised_Asset(&combination.face, face_rarity, face_filepath);
                char *eyes_asset = Randomised_Asset(&combination.eyes, eyes_rarity, eyes_filepath);
                char *hat_asset  = Randomised_Asset(&combination.hat,  hat_rarity,  hat_filepath);

                strcat(face_filepath, face_asset);
                strcat(eyes_filepath, eyes_asset);
                strcat(hat_filepath,  hat_asset);

                free(face_asset);
                free(eyes_asset);
                free(hat_asset);

                face = LoadTexture(face_filepath);
                eyes = LoadTexture(eyes_filepath);
                hat  = LoadTexture(hat_filepath);
            }

            // Check the new asset combination for uniqueness in an array
            // consisting of all the other assets that have been created.
            combination.id = combination.hat + (combination.eyes*100) + (combination.face*10000);
            for(int array_index = 0; array_index < combo_count; array_index++)
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
                unique_combinations[combo_index] = combination.id;
                combo_index++;
                combo_count++;
            }
        }

        DrawTexture(face, 0, 0, RAYWHITE);
        DrawTexture(eyes, 0, 0, RAYWHITE);
        DrawTexture(hat,  0, 0, RAYWHITE);
        EndDrawing();

        // Print to console for debugging.
        {
            printf("face =\t%d\neyes =\t%d\nhat =\t%d\nid =\t%d\n", combination.face, combination.eyes,
                                                                    combination.hat,  combination.id);
        }


#if 0
        // Export some random pics.
        if(export_count < MAX_BUFFER)
        {
            Image gen = GetScreenData();
            char export_path[MAX_BUFFER] = "../../assets/exports/";
            char *file_type = ".png";
            char file[10];
            sprintf(file, "%d", export_count);
            strcat(export_path, file);
            strcat(export_path, file_type);
            ExportImage(gen, export_path);
            export_count++;
        }
#endif
        // Print array of unique combinations for debugging.
        for(int i = 0; i < combo_index; i++)
        {
            printf("%d\t--->\t%d\n", i, unique_combinations[i]);
        }
    }
}
