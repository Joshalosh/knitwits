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

internal char *Choose_Rarity(char *filepath)
{
    int rand_num = (rand() % (100 - 0 + 1)) + 0;

    if(rand_num == 0 || rand_num == 100)
    {
        strcat(filepath, "ultimate/");
        return filepath;
    }
    else if(rand_num >= 91 && rand_num <= 99)
    {
        strcat(filepath, "epic/");
        return filepath;
    }
    else if(rand_num >= 71 && rand_num <= 90)
    {
        strcat(filepath, "rare/");
        return filepath;
    }
    else
    {
        strcat(filepath, "common/");
        return filepath;
    }
}

internal char *Randomised_Asset(int *asset_id, char *filepath)
{
    DIR *d;
    struct dirent *dir;

    int num_count = 0;
    int max_assets = Directory_Count(filepath);
    //bool allowed = false;

    // This is a way to decrease percentage chance but it would probably
    // be better to do two random checks. The first check can be
    // for overall rarity bucket e.g Common, Rare, Epic etc.
    // The next check could be for the specific asset in the chosen
    // rarity bucket. The rarity buckets should be based on percentages.
#if 0
    while(!allowed)
    {
        int rand_num  = (rand() % (max_assets - 2 + 1)) + 2;

        if(rand_num == 5 && *allowed_count >= 5)
        {
            allowed = true;
            *allowed_count = 0;
            *asset_id = rand_num;
        }
        else if(rand_num == 5 && *allowed_count != 5)
        {
            allowed = false;
            *allowed_count++;
        }
        else
        {
            allowed = true;
            *asset_id = rand_num;
        }
    }
#endif
    int rand_num  = (rand() % (max_assets - 2 + 1)) + 2;
    *asset_id = rand_num;

    char *random_asset = (char *)malloc(MAX_BUFFER);
    memset(random_asset, 0, MAX_BUFFER);

    d = opendir(filepath);
    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            if(num_count == *asset_id)
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
    // int allowed_count     = 0;


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

            // Set up the textures to draw.
            {
                char face_filepath[MAX_BUFFER] = "../assets/face/";
                char eyes_filepath[MAX_BUFFER] = "../assets/eyes/";
                char hat_filepath[MAX_BUFFER]  = "../assets/hat/";

                char *face_rarity = Choose_Rarity(face_filepath);
                char *eyes_rarity = Choose_Rarity(eyes_filepath);
                char *hat_rarity  = Choose_Rarity(hat_filepath);

                char *face_asset = Randomised_Asset(&combination.face, face_rarity);
                char *eyes_asset = Randomised_Asset(&combination.eyes, eyes_rarity);
                char *hat_asset  = Randomised_Asset(&combination.hat, hat_rarity);

                strcat(face_rarity, face_asset);
                strcat(eyes_rarity, eyes_asset);
                strcat(hat_rarity, hat_asset);

                free(face_asset);
                free(eyes_asset);
                free(hat_asset);

                face = LoadTexture(face_rarity);
                eyes = LoadTexture(eyes_rarity);
                hat  = LoadTexture(hat_rarity);
            }

            // Check the overall asset for uniqueness.

            // TODO: Going to have to change the way I track individual assets
            // and overall asset id's now that I have changed the way that
            // rarity works.
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

        // Print to console for debugging.
        {
            printf("face =\t%d\neyes =\t%d\nhat =\t%d\nid =\t%d\n", combination.face, combination.eyes,
                                                                    combination.hat, combination.id);
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
        for(int i = 0; i < combination_index; i++)
        {
            printf("%d\t--->\t%d\n", i, unique_combinations[i]);
        }
    }
}
