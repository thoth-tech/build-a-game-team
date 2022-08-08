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

        vector<shared_ptr<Block>> get_tiles(vector<shared_ptr<Block>> level_blocks, bitmap cell_sheet, int offset)
        {
            point_2d position;

            for (int i = 0; i < this->map_height; i++)
                for (int j = 0; j < this->map_width; j++)
                {
                    position.x = j * this->tile_size;
                    position.y = i * this->tile_size;
                    int cell = (std::stoi(this->map_array[i][j]) - 1) - offset;

                    if(std::stoi(this->map_array[i][j]) > offset)
                    {

                        if(bitmap_name(cell_sheet) == "SolidBlocks")
                        {
                            if(std::stoi(this->map_array[i][j]) < bitmap_cell_count(cell_sheet) + 1)
                            {
                                shared_ptr<Block> block(new SolidBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "NonSolidBlocks")
                        {
                            if(std::stoi(this->map_array[i][j]) < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new NonSolidBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }

                        if(bitmap_name(cell_sheet) == "PipeBlocks")
                        {
                            if(std::stoi(this->map_array[i][j]) < (bitmap_cell_count(cell_sheet) + 1) + offset)
                            {
                                shared_ptr<Block> block(new PipeBlock(cell_sheet, position, cell));
                                level_blocks.push_back(block);
                            }
                        }
                    }
                }

            return level_blocks;
        };

};

