// Map Class
#include "splashkit.h"
#include "enemy.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include "block.h"
using namespace std;

#pragma once

/**
 * @brief Map class
 * 
 */
class LevelOjectsMap
{
    protected:
        vector<vector<int> > map_array;
        int tile_size = 64;
        int map_width;
        int map_height;
        string type;

    public:
        /**
         * @brief Default Constructor
         */
        LevelOjectsMap(){};

        /**
         * @brief Destructor
         */
        ~LevelOjectsMap()
        {
            //write_line("Deleting Map");
            map_array.clear();
        };

        LevelOjectsMap(string level, int tile_size)
        {
            this->tile_size = tile_size;
            check_map_dimensions(level);
            this->map_array = new_level(level);
        };
            
        void check_map_dimensions(string file)
        {
            ifstream map_level;
            std::string line;
            int y_count = 0;
            int x_count = 0;

            map_level.open(file);

            if(map_level.fail())
            {
                write_line("Error");
                cerr << "Error Opening File" << endl;
                exit(1);
            }

            while(std::getline(map_level, line))
            {
                std::istringstream iss(line);

                int i;
                x_count = 0;
                
                while(iss >> i)
                {
                    x_count += 1;
                }
                y_count += 1;
            }

            map_level.close();

            this->map_height = y_count;
            this->map_width = x_count;
        };

        int get_map_width()
        {
            return this->map_width;
        };

        int get_map_height()
        {
            return this->map_height;
        };

        vector<vector<int> > new_level(string file)
        {
            // Initialise a 2D matrix of int to store level design  
            vector<vector<int> > map;

            // load in the level layout from file
            ifstream map_level;
            map_level.open(file);

            // Check for errors
            if(map_level.fail())
            {
                write_line("Error");
                cerr << "Error Opening File" << endl;
                exit(1);
            }

            // initialise a vector of strings to store each line of text
            vector<int> map_line;
            int temp;

            for (int i = 0; i < this->map_height; i++)
            {
                for (int j = 0; j < this->map_width; j++)
                {
                    map_level >> temp;
                    map_line.push_back(temp);
                }
                // pushback line
                map.push_back(map_line);
                map_level.ignore();
                map_line.clear();
            }
            map_level.close();
            return map;
        };

        shared_ptr<Player> get_player_position(int player_number)
        {
            point_2d position;
            shared_ptr<Player> player;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 909)
                    {
                        if(player_number == 1)
                        {
                            sprite player_sprite = create_sprite("blueGuy", "PlayerAnim");
                            player_input input = make_player1_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(1);
                            return player;
                        }
                        if(player_number == 3)
                        {
                            sprite player_sprite = create_sprite("purpleGuy", "PlayerAnim");
                            player_input input = make_player1_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(3);
                            return player;
                        }
                    }
                    if(this->map_array[i][j] == 910)
                    {
                        if(player_number == 2)
                        {
                            sprite player_sprite = create_sprite("pinkGirl", "PlayerAnim");
                            player_input input = make_player2_input();
                            shared_ptr<Player> player(new Player(new IdleState, player_sprite, position, false, input));
                            player->set_player_id(2);
                            return player;
                        }
                    }
                }

            return player;
        };

        shared_ptr<DoorBlock> get_door(bitmap cell_sheet)
        {
            point_2d position;
            shared_ptr<DoorBlock> door;

            for (int i = 0; i < this->map_height; i++)
            {
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 808)
                    {
                        shared_ptr<DoorBlock> level_door(new DoorBlock(cell_sheet, position));
                        door = level_door;
                    }
                }
            }

            return door;
        };

        vector<shared_ptr<Enemy>> get_enemies(vector<shared_ptr<Enemy>> level_enemies)
        {
            point_2d position;

             for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 707)
                    {
                        sprite roach = create_sprite("Roach", "RoachAnim");
                        shared_ptr<Roach> cockroach(new Roach(roach, position));
                        cockroach->get_ai()->set_facing_left(false);
                        level_enemies.push_back(cockroach);
                    }
                    if(this->map_array[i][j] == 708)
                    {
                        sprite roach = create_sprite("Roach", "RoachAnim");
                        shared_ptr<Roach> cockroach(new Roach(roach, position));
                        cockroach->get_ai()->set_facing_left(true);
                        level_enemies.push_back(cockroach);
                    }
                }

            return level_enemies;
        };

        vector<shared_ptr<Block>> get_tiles(vector<shared_ptr<Block>> level_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    int cell = ((this->map_array[i][j]) - 1) - offset;

                    if(this->map_array[i][j] > offset)
                    {
                        if(bitmap_name(cell_sheet) == "Solid")
                        {
                            if(this->map_array[i][j] < bitmap_cell_count(cell_sheet) + 1)
                            {
                                shared_ptr<Block> block(new SolidBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "Ladder")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new Ladder(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "Pipe")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new PipeBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "Water")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new WaterBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "Toxic")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new ToxicBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "HoldPipes")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new HoldablePipeBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "TurnPipes")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new TurnablePipeBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "Empty")
                        {
                            if(this->map_array[i][j] < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new EmptyPipeBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }
                    }
                }

            return level_blocks;
        };

};

