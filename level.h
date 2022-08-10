#include "splashkit.h"
#include "block.h"
#include "cellsheet.h"
#include "player.h"
#include "map.h"
#include "testing.h"
#include "collision.h"
#include <memory>
#include <vector>

#pragma once

vector<shared_ptr<Block>> make_layer_level(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Block>> level_blocks;
    LevelOjectsMap map(file, tile_size);

    for(int i = 0; i < cell_sheets.size(); i++)
    {
        level_blocks = map.get_tiles(level_blocks, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return level_blocks;
}

shared_ptr<Player> make_level_player(string file, int tile_size, int player_number)
{
    shared_ptr<Player> player;

    LevelOjectsMap map(file, tile_size);
    player = map.get_player_position(player_number);

    return player;
}

shared_ptr<DoorBlock> make_level_door(string file, int tile_size, bitmap cell_sheet)
{
    shared_ptr<DoorBlock> door;

    LevelOjectsMap map(file, tile_size);
    door = map.get_door(cell_sheet);

    return door;
}


class Level
{
    protected:
        vector<vector<shared_ptr<Block>>> layers;
        vector<CellSheet> cell_sheets;
        vector<string> files;
        vector<shared_ptr<Player>> level_players;
        shared_ptr<DoorBlock> door;
        int tile_size;
        int level_layers;
        int players;
        bool hitbox = false;
        bool test_camera = false;

    public:
        bool is_player1_out_of_lives = false;
        bool player1_complete = false;
        //set true for 1 player game
        bool is_player2_out_of_lives = true;
        bool player2_complete = true;

        Level(vector<CellSheet> cell_sheets, int tile_size, int players)
        {
            this->tile_size = tile_size;
            this->cell_sheets = cell_sheets;
            this->players = players;
            if(this->players == 2)
            {
                this->player2_complete = false;
                this->is_player2_out_of_lives = false;
            }
        };

        ~Level()
        {
            //write_line("Level Delete");
        };

        void make_level()
        {
            this->door = make_level_door(files[0], this->tile_size, cell_sheets[5].cells);

            for(int i = 0; i < level_layers; i++)
            {
                vector<shared_ptr<Block>> level_blocks;
                string file = files[i];
                level_blocks = make_layer_level(file, this->tile_size, this->cell_sheets);
                this->layers.push_back(level_blocks);
            }

            if(players == 2)
            {
                for(int i = 1; i < players + 1; i++)
                {
                    shared_ptr<Player> player = make_level_player(files[0], this->tile_size, i);
                    this->level_players.push_back(player);
                }
            }
            else
            {
                shared_ptr<Player> player = make_level_player(files[0], this->tile_size, 3);
                this->level_players.push_back(player);
            }
        }

        void update()
        {
            clear_screen(COLOR_BLACK);
            draw_bitmap("background", 0, 0, option_to_screen());

            //Draw Initial Layer
            for(int j = 0; j < layers[0].size(); j++)
            {
                if(rect_on_screen(layers[0][j]->get_block_hitbox()))
                    layers[0][j]->draw_block();
                //For testing hitboxes
                if(hitbox)
                    draw_hitbox(layers[0][j]->get_block_hitbox());
            }
            
            door->draw_block();
            if(hitbox)
                draw_hitbox(door->get_block_hitbox());

            //Player functions
            for(int i = 0; i < level_players.size(); i++)
            {
                level_players[i]->update();
                level_players[i]->get_input();
                level_players[i]->update_hitbox();
                //For testing hitboxes
                if(hitbox)
                    draw_hitbox(level_players[i]->get_player_hitbox());
                
                if(level_players[i]->has_player_won())
                {
                    if(i == 0)
                        player1_complete = true;
                    else
                        player2_complete = true;                    
                }

                point_2d player_pos = sprite_position(level_players[i]->get_player_sprite());
                //Player Will lose a life when they fall off the bottom of the screen
                if(!point_on_screen(to_screen(player_pos)) && (to_screen(player_pos).y > rectangle_bottom(screen_rectangle())))
                {
                    this->level_players[i]->set_dead(true);
                    this->level_players[i]->player_lives -= 1;

                    if(level_players[i]->player_lives == 0)
                    {
                        if(i == 0)
                            is_player1_out_of_lives = true;
                        else
                            is_player2_out_of_lives = true;
                    }
                }

            }

            //Draw foreground Layers
            for(int i = 1; i < layers.size(); i++)
            {
                for(int j = 0; j < layers[i].size(); j++)
                {
                    if(rect_on_screen(layers[i][j]->get_block_hitbox()))
                        layers[i][j]->draw_block();
                    if(hitbox)
                        draw_hitbox(layers[i][j]->get_block_hitbox());
                }
            }

            check_solid_block_collisions(layers, level_players);
            check_door_block_collisions(door, level_players);

            if(test_camera)
                test_camera_on(level_players[0]);

            check_test_input();

        }

        void check_test_input()
        {
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
        }

};

class Level1 : public Level
{
    public: 
        Level1(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("file0.txt");
            this->files.push_back("file1.txt");
            make_level();
        };
};

class Level2 : public Level
{
    public: 
        Level2(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/level2_1.txt");
            this->files.push_back("levels/level2_2.txt");
            make_level();
        };
};

class BlankLevel : public Level
{
    public: 
        BlankLevel(vector<CellSheet> cell_sheets, int tile_size, int players, int layers, vector<string> file_names) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = layers;
            for(int i = 0; i < layers; i++)
                this->files.push_back(file_names[i]);
            make_level();
        };
};