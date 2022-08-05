// Map Class
#include "splashkit.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "block.h"
using namespace std;

/**
 * @brief Map class
 * 
 */
class LevelOjectsMap
{
    protected:
        vector<vector<int> > map_array; 
        int screen_width = 1600;
        int screen_height = 896;
        int tile_size = 64;
        int map_width;
        int map_height;
        string type;

    public:
        /**
         * @brief Default Constructor
         */
        Level_Ojects_Map(){};

        /**
         * @brief Destructor
         */
        ~Level_Ojects_Map()
        {
            map_array.clear();
        };

        /**
         * @brief First Overloaded Constructor
         * @param level        The text file used to define the level
         * @param sprite_sheet The sprite sheet used to build the map
         * 
         */
        Level_Ojects_Map(string level, int tile_size)
        {
            this->tile_size = tile_size;
            this->map_width = (screen_width / tile_size);
            this->map_height = (screen_height / tile_size);
            this->map_array = new_level(level);
        };
            
        /**
         * @brief Loads the text file contatining map design and layout
         * 
         * @param file 
         * @return vector<vector<int> > 
         */
        vector<vector<int> > new_level(string file)
        {
            write_line("In new level");
            // Initialise a 2D matrix of integers to store level design  
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

            // initialise a vector of integers to store each line of text
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
            
            return map;
        };

        vector<shared_ptr<Block>> get_solid_objects(vector<shared_ptr<Block>> level_blocks)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == 20)
                    {
                        shared_ptr<Block> block(new FloorBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 30)
                    {
                        shared_ptr<Block> block(new BrickBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 40)
                    {
                        shared_ptr<Block> block(new QuestionBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 1)
                    {
                        shared_ptr<Block> block(new DarkSewerBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 2)
                    {
                        shared_ptr<Block> block(new LightSewerBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 3)
                    {
                        shared_ptr<Block> block(new LadderBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 4 || this->map_array[i][j] == 5)
                    {
                        shared_ptr<Block> block(new WaterBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == 6)
                    {
                        shared_ptr<Block> block(new ToxicBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                }

            return level_blocks;
        };

};

