#include "splashkit.h"
#include "player.h"
#include "block.h"
#include "playerinput.h"
#include "map.h"
#include "cellsheet.h"
#include <memory>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 896
#define TILE_SIZE 64

void draw_hitbox(rectangle hitbox)
{
    draw_rectangle(COLOR_GREEN, hitbox);
}

vector<CellSheet> make_cell_sheets(vector<string> cell_sheet_names)
{
    vector<CellSheet> cell_sheets;

    int offset = 0;
    for(int i = 0; i < cell_sheet_names.size(); i++)
    {
        bitmap new_bitmap = bitmap_named(cell_sheet_names[i]);
        CellSheet new_type(new_bitmap, cell_sheet_names[i], offset);
        offset += new_type.number_of_cells;
        cell_sheets.push_back(new_type);
    }

    return cell_sheets;
}

vector<shared_ptr<Block>> make_layer(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Block>> level_blocks;
    LevelOjectsMap map(file, tile_size);

    for(int i = 0; i < cell_sheets.size(); i++)
    {
        level_blocks = map.get_tiles(level_blocks, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return level_blocks;
}

shared_ptr<Player> make_player(string file, int tile_size, int player_number)
{
    shared_ptr<Player> player;

    LevelOjectsMap map(file, tile_size);
    player = map.get_player_position(player_number);

    return player;
}

int main()
{
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    open_window("Platform Prototype", SCREEN_WIDTH, SCREEN_HEIGHT);

    bool hitbox = false;
    bool test_camera = false;

    int players = 1;

    vector<string> cell_sheet_names;

    //Push the cell sheets used in the same order as the level editor
    cell_sheet_names.push_back("Solid");
    cell_sheet_names.push_back("NonSolid");
    cell_sheet_names.push_back("Pipe");
    cell_sheet_names.push_back("Water");
    cell_sheet_names.push_back("Toxic");

    vector<CellSheet> cell_sheets = make_cell_sheets(cell_sheet_names);

    //Change this variable for adding another layer
    int level_layers = 2;
    vector<vector<shared_ptr<Block>>> layers;

    for(int i = 0; i < level_layers; i++)
    {
        vector<shared_ptr<Block>> level_blocks;
        string file = "file" + std::to_string(i) + ".txt";
        level_blocks = make_layer(file, TILE_SIZE, cell_sheets);
        layers.push_back(level_blocks);
    }

    vector<shared_ptr<Player>> level_players;
    if(players == 2)
    {
        for(int i = 1; i < players + 1; i++)
        {
            shared_ptr<Player> player = make_player("file0.txt", TILE_SIZE, i);
            level_players.push_back(player);
        }
    }
    else
    {
        shared_ptr<Player> player = make_player("file0.txt", TILE_SIZE, 3);
        level_players.push_back(player);
    }
    

    while (!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_BLACK);
        draw_bitmap("background", 0, 0, option_to_screen());

        //Draw Initial Layer
        for(int j = 0; j < layers[0].size(); j++)
        {
            layers[0][j]->draw_block();
            //For testing hitboxes
            if(hitbox)
                draw_hitbox(layers[0][j]->get_block_hitbox());
        }

        for(int i = 0; i < level_players.size(); i++)
        {
            level_players[i]->update();
            level_players[i]->get_input();
            level_players[i]->update_hitbox();
            //For testing hitboxes
            if(hitbox)
                draw_hitbox(level_players[i]->get_player_hitbox());
        }

        //Draw foreground Layers
        for(int i = 1; i < layers.size(); i++)
        {
            for(int j = 0; j < layers[i].size(); j++)
            {
                layers[i][j]->draw_block();
                if(hitbox)
                    draw_hitbox(layers[i][j]->get_block_hitbox());
            }
        }

        for(int k = 0; k < level_players.size(); k++)
        {
            float landing_value = 0;
            string collision = "None";
            for(int j = 0; j < layers.size(); j++)
            {
                for (int i = 0; i < layers[j].size(); i++)
                {
                    if(layers[j][i]->is_block_solid())
                        collision = layers[j][i]->test_collision(level_players[k]->get_player_hitbox(), layers[j][i]->get_block_hitbox());
                    else
                        break;
                    
                    
                    if (collision == "Top")
                    {
                        landing_value = layers[j][i]->get_top();
                        level_players[k]->set_on_floor(true);
                        level_players[k]->set_landing_y_value(landing_value);
                        break;
                    }
                    else if (collision == "Bottom")
                    {
                        bool test = level_players[k]->is_on_floor();

                        if(test)
                            break;

                        level_players[k]->set_player_dy(0);
                        level_players[k]->set_on_floor(false);
                        sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                        level_players[k]->change_state(new JumpFallState, "JumpFall");
                        break;
                    }
                    else if (collision == "Left")
                    {
                        level_players[k]->set_player_dx(0);
                        sprite_set_x(level_players[k]->get_player_sprite(), sprite_x(level_players[k]->get_player_sprite()) - 5);
                        break;
                    }
                    else if (collision == "Right")
                    {
                        level_players[k]->set_player_dx(0);
                        sprite_set_x(level_players[k]->get_player_sprite(), sprite_x(level_players[k]->get_player_sprite()) + 5);
                        break;
                    }
                }
            }

            if (collision == "None")
                level_players[k]->set_on_floor(false);
        }
        
        //For testing purposes only
        if(test_camera)
            center_camera_on(level_players[0]->get_player_sprite(), 0, 0);


        //Turn on hitboxes
        if(key_typed(H_KEY))
        {
            if(hitbox)
                hitbox = false;
            else
                hitbox = true;
        }

        //Turn on test camera
        if(key_typed(C_KEY))
        {
            if(test_camera)
                test_camera = false;
            else
                test_camera = true;
        }

        process_events();
        refresh_screen(60);
    }

    //delete player;
    free_resource_bundle("player");
    free_resource_bundle("game_resources");
    return 0;
}