#include "splashkit.h"
#include "block.h"
#include "behaviour.h"
#include "camera.h"
#include "enemy.h"
#include "cellsheet.h"
#include "player.h"
#include "hud.h"
#include "map.h"
#include "testing.h"
#include "collision.h"
#include <memory>
#include <vector>

#pragma once

vector<shared_ptr<SolidBlock>> make_level_solid_blocks(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<SolidBlock>> solid_blocks;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        solid_blocks = map.get_solid_blocks(solid_blocks, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return solid_blocks;
}

vector<shared_ptr<Ladder>> make_level_ladders(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Ladder>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_ladders(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<WaterBlock>> make_level_water(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<WaterBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_water(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<ToxicBlock>> make_level_toxic(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<ToxicBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_toxic(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<HoldablePipeBlock>> make_holdable_pipes(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<HoldablePipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_holdable_pipes(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<EmptyPipeBlock>> make_holdable_pipe_empty_spaces(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<EmptyPipeBlock>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_empty_pipe_blocks(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<Block>> make_level_decoration(string file, int tile_size, vector<CellSheet> cell_sheets)
{
    vector<shared_ptr<Block>> block;
    LevelOjectsMap map(file, tile_size);

    for (int i = 0; i < cell_sheets.size(); i++)
    {
        block = map.get_decoration(block, cell_sheets[i].cells, cell_sheets[i].offset);
    }

    return block;
}

vector<shared_ptr<Enemy>> make_layer_enemies(vector<shared_ptr<Enemy>> level_enemy, string file, int tile_size)
{
    LevelOjectsMap map(file, tile_size);

    level_enemy = map.get_enemies(level_enemy);

    return level_enemy;
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

shared_ptr<Camera> make_level_camera(shared_ptr<Player> player, string file, int tile_size)
{
    LevelOjectsMap map(file, tile_size);
    int map_width = map.get_map_width();
    int map_height = map.get_map_height();

    shared_ptr<Camera> camera(new Camera(player, tile_size, map_height, map_width));

    return camera;
}

void draw_first_layer(vector<vector<shared_ptr<SolidBlock>>> solid_blocks, vector<vector<shared_ptr<Ladder>>> ladders, vector<vector<shared_ptr<Block>>> decoration, 
                     vector<vector<shared_ptr<WaterBlock>>> water, vector<vector<shared_ptr<ToxicBlock>>> toxic, vector<vector<shared_ptr<HoldablePipeBlock>>> hold_pipes,
                     vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes)
{
    for(int i = 0; i < solid_blocks[0].size(); i++)
        if(rect_on_screen(solid_blocks[0][i]->get_block_hitbox()))
            solid_blocks[0][i]->draw_block();

    for(int i = 0; i < ladders[0].size(); i++)
        if(rect_on_screen(ladders[0][i]->get_block_hitbox()))
            ladders[0][i]->draw_block();

    for(int i = 0; i < decoration[0].size(); i++)
        if(rect_on_screen(decoration[0][i]->get_block_hitbox()))
            decoration[0][i]->draw_block();

    for(int i = 0; i < water[0].size(); i++)
        if(rect_on_screen(water[0][i]->get_block_hitbox()))
            water[0][i]->draw_block();
    
    for(int i = 0; i < toxic[0].size(); i++)
        if(rect_on_screen(toxic[0][i]->get_block_hitbox()))
            toxic[0][i]->draw_block();

    for(int i = 0; i < hold_pipes[0].size(); i++)
        if(rect_on_screen(hold_pipes[0][i]->get_block_hitbox()))
            hold_pipes[0][i]->draw_block();
    
    for(int i = 0; i < empty_pipes[0].size(); i++)
        if(rect_on_screen(empty_pipes[0][i]->get_block_hitbox()))
            empty_pipes[0][i]->draw_block();
}

void draw_foreground_layers(vector<vector<shared_ptr<SolidBlock>>> solid_blocks, vector<vector<shared_ptr<Ladder>>> ladders, vector<vector<shared_ptr<Block>>> decoration, 
                     vector<vector<shared_ptr<WaterBlock>>> water, vector<vector<shared_ptr<ToxicBlock>>> toxic, vector<vector<shared_ptr<HoldablePipeBlock>>> hold_pipes,
                     vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes)
{
    for(int i = 1; i < solid_blocks.size(); i++)
        for(int j = 0; j < solid_blocks[i].size(); j++)
            if(rect_on_screen(solid_blocks[i][j]->get_block_hitbox()))
                solid_blocks[i][j]->draw_block();

    for(int i = 1; i < ladders.size(); i++)
        for(int j = 0; j < ladders[i].size(); j++)
            if(rect_on_screen(ladders[i][j]->get_block_hitbox()))
                ladders[i][j]->draw_block();

    for(int i = 1; i < decoration.size(); i++)
        for(int j = 0; j < decoration[i].size(); j++)
            if(rect_on_screen(decoration[i][j]->get_block_hitbox()))
                decoration[i][j]->draw_block();
    
    for(int i = 1; i < water.size(); i++)
        for(int j = 0; j < water[i].size(); j++)
            if(rect_on_screen(water[i][j]->get_block_hitbox()))
                water[i][j]->draw_block();

    for(int i = 1; i < toxic.size(); i++)
        for(int j = 0; j < toxic[i].size(); j++)
            if(rect_on_screen(toxic[i][j]->get_block_hitbox()))
                toxic[i][j]->draw_block();

    for(int i = 1; i < hold_pipes.size(); i++)
        for(int j = 0; j < hold_pipes[i].size(); j++)
            if(rect_on_screen(hold_pipes[i][j]->get_block_hitbox()))
                hold_pipes[i][j]->draw_block();

    for(int i = 1; i < empty_pipes.size(); i++)
        for(int j = 0; j < empty_pipes[i].size(); j++)
            if(rect_on_screen(empty_pipes[i][j]->get_block_hitbox()))
                empty_pipes[i][j]->draw_block();
}

class Level
{
    protected:
        vector<CellSheet> cell_sheets;
        vector<string> files;
        vector<shared_ptr<Player>> level_players;
        shared_ptr<DoorBlock> door;
        vector<shared_ptr<Enemy>> level_enemies;
        vector<vector<shared_ptr<SolidBlock>>> solid_blocks;
        vector<vector<shared_ptr<Ladder>>> ladders;
        vector<vector<shared_ptr<Block>>> decoration;
        vector<vector<shared_ptr<WaterBlock>>> water;
        vector<vector<shared_ptr<ToxicBlock>>> toxic;
        vector<vector<shared_ptr<HoldablePipeBlock>>> hold_pipes;
        vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes;
        shared_ptr<Camera> camera;
        shared_ptr<HUD> level_hud;
        int tile_size;
        int level_layers;
        int players;
        bool hitbox = false;
        string level_name = "";
        music level_music;

    public:
        bool is_player1_out_of_lives = false;
        bool player1_complete = false;

        bool is_player2_out_of_lives = false;
        bool player2_complete = true;

        Level(vector<CellSheet> cell_sheets, int tile_size, int players)
        {
            set_camera_x(0);
            set_camera_y(0);
            this->tile_size = tile_size;
            this->cell_sheets = cell_sheets;
            this->players = players;
            if (this->players == 2)
            {
                this->player2_complete = false;
                this->is_player2_out_of_lives = false;
            }
        };

        ~Level(){};

        void make_level()
        {
            this->door = make_level_door(files[0], this->tile_size, cell_sheets[5].cells);

            for (int i = 0; i < level_layers; i++)
            {
                string file = files[i];

                vector<shared_ptr<SolidBlock>> solid_block;
                solid_block = make_level_solid_blocks(file, this->tile_size, this->cell_sheets);
                this->solid_blocks.push_back(solid_block);

                vector<shared_ptr<Ladder>> ladder_block;
                ladder_block = make_level_ladders(file, this->tile_size, this->cell_sheets);
                this->ladders.push_back(ladder_block);

                vector<shared_ptr<WaterBlock>> water_block;
                water_block = make_level_water(file, this->tile_size, this->cell_sheets);
                this->water.push_back(water_block);

                vector<shared_ptr<ToxicBlock>> toxic_block;
                toxic_block = make_level_toxic(file, this->tile_size, this->cell_sheets);
                this->toxic.push_back(toxic_block);

                vector<shared_ptr<HoldablePipeBlock>> holdpipe_block;
                holdpipe_block = make_holdable_pipes(file, this->tile_size, this->cell_sheets);
                this->hold_pipes.push_back(holdpipe_block);

                vector<shared_ptr<EmptyPipeBlock>> emp_block;
                emp_block = make_holdable_pipe_empty_spaces(file, this->tile_size, this->cell_sheets);
                this->empty_pipes.push_back(emp_block);

                vector<shared_ptr<Block>> decoration_block;
                decoration_block = make_level_decoration(file, this->tile_size, this->cell_sheets);
                this->decoration.push_back(decoration_block);

                this->level_enemies = make_layer_enemies(this->level_enemies, file, this->tile_size);
            }

            if (players == 2)
            {
                for (int i = 1; i < players + 1; i++)
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

            shared_ptr<HUD> hud(new HUD(level_players));
            this->level_hud = hud;

            this->camera = make_level_camera(level_players[0], files[0], tile_size);
        }

        void update()
        {
            clear_screen(COLOR_BLACK);
            draw_bitmap("background", 0, 0, option_to_screen());
            if (!music_playing())
                play_music(this->level_music);

            draw_first_layer(solid_blocks, ladders, decoration, water, toxic, hold_pipes, empty_pipes);

            door->draw_block();
            if (hitbox)
                draw_hitbox(door->get_block_hitbox());

            // Player functions
            for (int i = 0; i < level_players.size(); i++)
            {
                if (!level_players[i]->is_player_dead())
                {
                    level_players[i]->update();
                    level_players[i]->get_input();
                    level_players[i]->update_hitbox();
                }
                // For testing hitboxes
                if (hitbox)
                    draw_hitbox(level_players[i]->get_player_hitbox());

                if (level_players[i]->has_player_won())
                {
                    if (i == 0)
                        player1_complete = true;
                    else
                        player2_complete = true;
                }
            }

            for (int i = 0; i < level_enemies.size(); i++)
            {
                if (rect_on_screen(level_enemies[i]->get_enemy_hitbox()))
                    level_enemies[i]->update();
            }

            draw_foreground_layers(solid_blocks, ladders, decoration, water, toxic, hold_pipes, empty_pipes);

            this->camera->update();

            check_solid_block_collisions(solid_blocks, level_players);

            // check for player to pick up a holdable pipe
            check_holdable_pipe_block_collisions(hold_pipes, level_players);

            // check for player to place it's pipe on th empty pipe
            check_empty_pipe_block_collisions(empty_pipes, level_players);
            check_ladder_collisions(ladders, level_players);
            check_door_block_collisions(door, level_players);
            check_enemy_solid_block_collisions(solid_blocks, level_enemies);
            check_enemy_player_collisions(level_enemies, level_players);
            check_water_block_collisions(water, level_players);
            check_toxic_block_collisions(toxic, level_players);
            check_water_water_block_collisions(water, water);
            check_water_empty_block_collisions(empty_pipes, water);

            for (int i = 0; i < level_players.size(); i++)
            {
                point_2d player_pos = sprite_position(level_players[i]->get_player_sprite());

                // Player Will lose a life when they fall off the bottom of the screen
                if (!point_on_screen(to_screen(player_pos)) && (to_screen(player_pos).y > rectangle_bottom(screen_rectangle())))
                {
                    this->level_players[i]->player_lives -= 1;
                    sprite_set_position(level_players[i]->get_player_sprite(), level_players[i]->get_player_position());
                    this->level_players[i]->change_state(new SpawningState, "Spawn");
                }

                //Player loses a life if they run out of health
                if (level_players[i]->player_health < 1)
                {
                    level_players[i]->player_health = 3;
                    level_players[i]->player_lives -= 1;
                    this->level_players[i]->change_state(new DyingState, "Dying");
                }

                //If players sets out of lives
                if (level_players[i]->player_lives == 0 && level_players[i]->get_state_type() == "Spawn")
                {
                    this->level_players[i]->set_dead(true);

                    if (i == 0)
                        is_player1_out_of_lives = true;
                    else
                        is_player2_out_of_lives = true;
                }
            }

            //level_hud->update();

            check_test_input();
        }

        void check_test_input()
        {
            if (key_typed(H_KEY))
            {
                if (hitbox)
                    hitbox = false;
                else
                    hitbox = true;
            }
        }

        rectangle get_collision_test_area()
        {
            rectangle test_area = to_screen(screen_rectangle());
            test_area.x -= (6 * tile_size);
            test_area.y -= (6 * tile_size);
            test_area.width += (12 * tile_size);
            test_area.height += (12 * tile_size);

            return test_area;
        }

        string get_level_name()
        {
            return this->level_name;
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
            this->level_music = music_named("LevelOne");
            this->level_name = "Don't Fall in the hole";
        };
};

class Level2 : public Level
{
    public:
        Level2(vector<CellSheet> cell_sheets, int tile_size, int players) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = 2;
            this->files.push_back("levels/level1_1.txt");
            this->files.push_back("levels/level1_2.txt");
            make_level();
            this->level_music = music_named("LevelOne");
            this->level_name = "Too Many Roaches";
        };
};

class BlankLevel : public Level
{
    public:
        BlankLevel(vector<CellSheet> cell_sheets, int tile_size, int players, int layers, vector<string> file_names) : Level(cell_sheets, tile_size, players)
        {
            this->level_layers = layers - 1;
            for (int i = 0; i < layers; i++)
            {  
                if(i < layers - 1)
                    this->files.push_back(file_names[i]);
                else
                    this->level_name = file_names[i];
            }
            this->level_music = music_named("LevelOne");
            make_level();
        };
};