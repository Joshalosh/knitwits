#include "include/raylib.h"
#include "include/dirent.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "stdint.h"
#include "main.h"

// Using PCG for random number generation. Companion
// struct in "main.h".
uint32_t pcg32_random_r(pcg32_random_t *rng)
{
    uint64_t old_state = rng->state;
    // Advanced internal state
    rng->state = old_state *6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xor_shifted = ((old_state >> 18u) ^ old_state) >>27u;
    uint32_t rot = old_state >> 59u;
    return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31));
}

// Count the number of assets within a given directory.
internal int
Directory_Count(char *filepath)
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

// Assign a rarity "bucket", which in reality is
// just a grouping of different directories
// for the different types of rarities.
internal void 
Choose_Rarity(int *rarity_group, char *filepath, pcg32_random_t *rng)
{
    uint32_t rand_num = pcg32_random_r(rng) % 101;
    *rarity_group = 0;

    if(rand_num >= 91 && rand_num <= 100)
    {
        strcat(filepath, "ultimate/");
        *rarity_group = 400;
    }
    else if(rand_num >= 61 && rand_num <= 90)
    {
        strcat(filepath, "epic/");
        *rarity_group = 300;
    }
    else if(rand_num >= 31 && rand_num <= 60)
    {
        strcat(filepath, "rare/");
        *rarity_group = 200;
    }
    else
    {
        strcat(filepath, "common/");
        *rarity_group = 100;
    }
}

// Choose an asset within a rarity bucket/directory
// and record it's filepath.
internal char 
*Randomised_Asset(int *asset_id, int rarity_group, char *filepath,
                  Asset *asset, pcg32_random_t *rng)
{
    DIR *d;
    struct dirent *dir;

    int num_count  = 0;
    int max_assets = Directory_Count(filepath);

    uint32_t rand_num  = (pcg32_random_r(rng) % (max_assets - 2 + 1)) + 2;
    *asset_id = rand_num + rarity_group;
    asset->counter[*asset_id] += 1;

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

// Just a little function to help print some
// statistics about the assets after creation.
// for example how many times a particular asset
// was used and it's overall percentage.
internal void
Print_Asset_Stats(Asset *asset)
{
    float total_assets = 0;
    int common_count   = 0;
    int rare_count     = 0;
    int epic_count     = 0;
    int ultimate_count = 0;
    for(int i = 0; i < MAX_BUFFER; i++)
    {
        if(asset->counter[i] != 0)
        {
            if(i >= 40)
            {
                ultimate_count += asset->counter[i];
            }
            else if(i >= 30 && i <= 39)
            {
                epic_count += asset->counter[i];
            }
            else if(i >= 20 && i <= 29)
            {
                rare_count += asset->counter[i];
            }
            else
            {
                common_count += asset->counter[i];
            }

            total_assets += asset->counter[i];
            printf("%d\t--->\t%d\n", i, asset->counter[i]);
        }
    }

    printf("PERCENTAGES:\n");
    int common_percentage   = (common_count   / total_assets)*100;
    int rare_percentage     = (rare_count     / total_assets)*100;
    int epic_percentage     = (epic_count     / total_assets)*100;
    int ultimate_percentage = (ultimate_count / total_assets)*100;

    printf("Common\t\t--->\t%d\nRare\t\t--->\t%d\nEpic\t\t--->\t%d\nUltimate\t--->\t%d\n", common_percentage, rare_percentage,
                                                                                           epic_percentage, ultimate_percentage);
}

int main()
{
    int window_width  = 960;
    int window_height = 960;

    InitWindow(window_width, window_height, "MyGame");
    SetTargetFPS(60);

    pcg32_random_t rng = {};

    bool first_time_load_texture = true;
    Texture2D background  = {};
    Texture2D face        = {};
    Texture2D mouth       = {};
    Texture2D nose        = {};
    Texture2D eyes        = {};
    Texture2D beard       = {};
    Texture2D hair        = {};
    Texture2D hat         = {};
    Texture2D accessories = {};

    int export_count = 1;
    int combo_count  = 1;
    int combo_index  = 0;
    int directory_files = 0;


    Asset background_asset  = {};
    Asset face_asset        = {};
    Asset mouth_asset       = {};
    Asset nose_asset        = {};
    Asset eyes_asset        = {};
    Asset beard_asset       = {};
    Asset hair_asset        = {};
    Asset hat_asset         = {};
    Asset accessories_asset = {};

    int unique_combinations[MAX_BUFFER] = {};
    Combination combination = {};


    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        bool unique = false;
        while(directory_files < 102)
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
                UnloadTexture(background);
                UnloadTexture(face);
                UnloadTexture(mouth);
                UnloadTexture(nose);
                UnloadTexture(eyes);
                UnloadTexture(beard);
                UnloadTexture(hair);
                UnloadTexture(hat);
                UnloadTexture(accessories);
            }

            // Prepare the textures for drawing to the screen.
            {
                char background_filepath[MAX_BUFFER]  = "../assets/background/";
                char face_filepath[MAX_BUFFER]        = "../assets/face/";
                char mouth_filepath[MAX_BUFFER]       = "../assets/mouth/";
                char nose_filepath[MAX_BUFFER]        = "../assets/nose/";
                char eyes_filepath[MAX_BUFFER]        = "../assets/eyes/";
                char beard_filepath[MAX_BUFFER]       = "../assets/beard/";
                char hair_filepath[MAX_BUFFER]        = "../assets/hair/";
                char hat_filepath[MAX_BUFFER]         = "../assets/hat/";
                char accessories_filepath[MAX_BUFFER] = "../assets/accessories/";

                // Decide the rarity of the assets to be chosen
                int background_rarity  = 0;
                int face_rarity        = 0;
                int mouth_rarity       = 0;
                int nose_rarity        = 0;
                int eyes_rarity        = 0;
                int beard_rarity       = 0;
                int hair_rarity        = 0;
                int hat_rarity         = 0;
                int accessories_rarity = 0;
                Choose_Rarity(&background_rarity, background_filepath, &rng);
                Choose_Rarity(&face_rarity, face_filepath, &rng);
                Choose_Rarity(&mouth_rarity, mouth_filepath, &rng);
                Choose_Rarity(&nose_rarity, nose_filepath, &rng);
                Choose_Rarity(&eyes_rarity, eyes_filepath, &rng);
                Choose_Rarity(&beard_rarity, beard_filepath, &rng);
                Choose_Rarity(&hair_rarity, hair_filepath, &rng);
                Choose_Rarity(&hat_rarity, hat_filepath, &rng);
                Choose_Rarity(&accessories_rarity, accessories_filepath, &rng);

                char *background_asset_path = Randomised_Asset(&combination.background, background_rarity,
                                                               background_filepath, &background_asset, &rng);
                char *face_asset_path = Randomised_Asset(&combination.face, face_rarity,
                                                         face_filepath, &face_asset, &rng);
                char *mouth_asset_path = Randomised_Asset(&combination.mouth, mouth_rarity,
                                                          mouth_filepath, &mouth_asset, &rng);
                char *nose_asset_path = Randomised_Asset(&combination.nose, nose_rarity,
                                                         nose_filepath, &nose_asset, &rng);
                char *eyes_asset_path = Randomised_Asset(&combination.eyes, eyes_rarity, 
                                                         eyes_filepath, &eyes_asset, &rng);
                char *beard_asset_path = Randomised_Asset(&combination.beard, beard_rarity, 
                                                         beard_filepath, &beard_asset, &rng);
                char *hair_asset_path = Randomised_Asset(&combination.hair, hair_rarity, 
                                                         hair_filepath, &hair_asset, &rng);
                char *hat_asset_path = Randomised_Asset(&combination.hat, hat_rarity,
                                                         hat_filepath, &hat_asset, &rng);
                char *accessories_asset_path = Randomised_Asset(&combination.accessories, accessories_rarity,
                                                                accessories_filepath, &accessories_asset, &rng);

                strcat(background_filepath, background_asset_path);
                strcat(face_filepath, face_asset_path);
                strcat(mouth_filepath, mouth_asset_path);
                strcat(nose_filepath, nose_asset_path);
                strcat(eyes_filepath, eyes_asset_path);
                strcat(beard_filepath, beard_asset_path);
                strcat(hair_filepath, hair_asset_path);
                strcat(hat_filepath, hat_asset_path);
                strcat(accessories_filepath, accessories_asset_path);

                free(background_asset_path);
                free(face_asset_path);
                free(mouth_asset_path);
                free(nose_asset_path);
                free(eyes_asset_path);
                free(beard_asset_path);
                free(hair_asset_path);
                free(hat_asset_path);
                free(accessories_asset_path);
                
                //Load the files into images first for resizing. Making
                //sure to use Nearest Neighbour on the resize so that
                //the quality doesn't degrade at all.
                Image bg_image = LoadImage(background_filepath);
                Image face_image = LoadImage(face_filepath);
                Image mouth_image = LoadImage(mouth_filepath);
                Image nose_image = LoadImage(nose_filepath);
                Image eyes_image = LoadImage(eyes_filepath);
                Image beard_image = LoadImage(beard_filepath);
                Image hair_image = LoadImage(hair_filepath);
                Image hat_image = LoadImage(hat_filepath);
                Image accessories_image = LoadImage(accessories_filepath);

                ImageResizeNN(&bg_image, 960, 960);
                ImageResizeNN(&face_image, 960, 960);
                ImageResizeNN(&mouth_image, 960, 960);
                ImageResizeNN(&nose_image, 960, 960);
                ImageResizeNN(&eyes_image, 960, 960);
                ImageResizeNN(&beard_image, 960, 960);
                ImageResizeNN(&hair_image, 960, 960);
                ImageResizeNN(&hat_image, 960, 960);
                ImageResizeNN(&accessories_image, 960, 960);

                background = LoadTextureFromImage(bg_image);
                face = LoadTextureFromImage(face_image);
                mouth = LoadTextureFromImage(mouth_image);
                nose = LoadTextureFromImage(nose_image);
                eyes = LoadTextureFromImage(eyes_image);
                beard = LoadTextureFromImage(beard_image);
                hair = LoadTextureFromImage(hair_image);
                hat = LoadTextureFromImage(hat_image);
                accessories = LoadTextureFromImage(accessories_image);

                UnloadImage(bg_image);
                UnloadImage(face_image);
                UnloadImage(mouth_image);
                UnloadImage(nose_image);
                UnloadImage(eyes_image);
                UnloadImage(beard_image);
                UnloadImage(hair_image);
                UnloadImage(hat_image);
                UnloadImage(accessories_image);
            }

            /*
            // Check the new asset combination for uniqueness in an array
            // consisting of all the other assets that have been created.
            combination.id = combination.hat + (combination.eyes*100) +
                             (combination.nose*10000) + (combination.background*1000000);
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
            */

            DrawTexture(background, 0, 0, RAYWHITE);
            DrawTexture(face, 0, 0, RAYWHITE);
            DrawTexture(mouth, 0, 0, RAYWHITE);
            DrawTexture(nose, 0, 0, RAYWHITE);
            DrawTexture(eyes, 0, 0, RAYWHITE);
            DrawTexture(beard, 0, 0, RAYWHITE);
            DrawTexture(hair, 0, 0, RAYWHITE);
            DrawTexture(hat, 0, 0, RAYWHITE);
            DrawTexture(accessories, 0, 0, RAYWHITE);
            EndDrawing();

            // Export the unique picture
            Image gen = GetScreenData();
            char export_path[MAX_BUFFER] = "../assets/exports/";
            directory_files = Directory_Count(export_path);
            char *file_type = ".png";
            char file[100];
            sprintf(file, "bg%d_face%d_mouth%d_nose%d_eyes%d_beard%d_hair%d_hat%d_accessories%d", 
                    combination.background, combination.face, combination.mouth,
                    combination.nose, combination.eyes, combination.beard,
                    combination.hair, combination.hat, combination.accessories);
            strcat(export_path, file);
            strcat(export_path, file_type);
            ExportImage(gen, export_path);

            //export_count++;
        }
    }

    // Print the asset statistics.
    {
        printf("BACKGROUND:\n");
        Print_Asset_Stats(&background_asset);
        printf("FACE:\n");
        Print_Asset_Stats(&face_asset);
        printf("MOUTH:\n");
        Print_Asset_Stats(&mouth_asset);
        printf("NOSE:\n");
        Print_Asset_Stats(&nose_asset);
        printf("EYES:\n");
        Print_Asset_Stats(&eyes_asset);
        printf("BEARD:\n");
        Print_Asset_Stats(&beard_asset);
        printf("HAIR:\n");
        Print_Asset_Stats(&hair_asset);
        printf("HAT:\n");
        Print_Asset_Stats(&hat_asset);
        printf("ACCESSORIES:\n");
        Print_Asset_Stats(&accessories_asset);
    }
}
