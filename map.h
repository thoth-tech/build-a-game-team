// Map Class
#include "splashkit.h"
#include <fstream>
#include <iostream>
#include <sstream>
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
        vector<vector<string> > map_array;
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

                string i;
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

        vector<vector<string> > new_level(string file)
        {
            // Initialise a 2D matrix of strings to store level design  
            vector<vector<string> > map;

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
            vector<string> map_line;
            string temp;

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

        vector<shared_ptr<Block>> get_solid_objects(vector<shared_ptr<Block>> level_blocks)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;

                    if(this->map_array[i][j] == "20")
                    {
                        shared_ptr<Block> block(new FloorBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "30")
                    {
                        shared_ptr<Block> block(new BrickBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "40")
                    {
                        shared_ptr<Block> block(new QuestionBlock(Block::MARIO_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "1")
                    {
                        shared_ptr<Block> block(new DarkSewerBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "2")
                    {
                        shared_ptr<Block> block(new LightSewerBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "3")
                    {
                        shared_ptr<Block> block(new LadderBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "4" || this->map_array[i][j] == "5")
                    {
                        shared_ptr<Block> block(new WaterBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                    if(this->map_array[i][j] == "6")
                    {
                        shared_ptr<Block> block(new ToxicBlock(Block::SEWER_BLOCKS, position));
                        level_blocks.push_back(block);
                    }
                }

            return level_blocks;
        };

};

