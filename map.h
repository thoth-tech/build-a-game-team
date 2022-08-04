// Map Class
#include "splashkit.h"
#include <fstream>
#include <iostream>
using namespace std;
// Define screen size and tilesize
#define SCREEN_WIDTH  1600 // 50 tiles
#define SCREEN_HEIGHT  896 // 28 tiles
#define TILESIZE        16 // pixels
#define MAP_WIDTH       40 // tiles
#define MAP_HEIGHT      32 // tiles

/**
 * @brief Map class
 * 
 */
class Map
{
    private:
        // stores the background image
        bitmap background;
        // 2D array of level design from text file
        vector<vector<int> > map_array;

    public:
        /**
         * @brief Default Constructor
         */
        Map(){}

        /**
         * @brief Destructor
         */
        ~Map(){}

        /**
         * @brief First Overloaded Constructor
         * @param level        The text file used to define the level
         * @param sprite_sheet The sprite sheet used to build the map
         * 
         */
        Map(string level, string background)
        {
            // use new_level function to load the map to new_game.map_array
            this->map_array = new_level(level);
            // load the background
            this->background = load_bitmap("bgnd", background);
        }

        /**
         * @brief Draw the level to the screen
         * 
         * @return ** void 
         */
        void draw_background()
        {          
            // draw the background first
            //for (int i = 0; i < MAP_HEIGHT; i++)
                for (int j = 0; j < MAP_WIDTH/8; j++)
                    draw_bitmap(this->background, j*bitmap_width(this->background), -TILESIZE*2);

                
            // draw map objects -- to be used to draw other indicated map objects that are not solid
            //for (int i = 0; i < MAP_HEIGHT; i++)
                //for (int j = 0; j < MAP_WIDTH; j++)
                    //if (this->map_array[i][j] != 0)
                        //draw_bitmap(this->map, j*TILESIZE, i*TILESIZE, option_with_bitmap_cell(this->map_array[i][j]));
        }


        /**
         * @brief Gets coordinates of all objects and if they are solid or not
         * @return ** void 
         */
        vector<Block*> get_solid_objects(vector<Block*> level_blocks)
        {
            point_2d position;

            for (int i = 0; i < MAP_HEIGHT; i++)
                for (int j = 0; j < MAP_WIDTH; j++)
                {
                    position.x = j*TILESIZE;
                    position.y = i*TILESIZE;

                    if(this->map_array[i][j] == 20)
                        level_blocks.push_back(new FloorBlock(Block::MARIO_BLOCKS, position));
                    if(this->map_array[i][j] == 30)
                        level_blocks.push_back(new BrickBlock(Block::MARIO_BLOCKS, position));
                    if(this->map_array[i][j] == 40)
                        level_blocks.push_back(new QuestionBlock(Block::MARIO_BLOCKS, position));
                }

            return level_blocks;
        }

            
        /**
         * @brief Loads the text file contatining map design and layout
         * 
         * @param file 
         * @return vector<vector<int> > 
         */
        vector<vector<int> > new_level(string file)
        {
            // Initialise a 2D matrix of integers to store level design  
            vector<vector<int> > map;
            // load in the level layout from file
            ifstream map_level;
            map_level.open(file);
            // Check for errors
            if(map_level.fail())
            {
                cerr << "Error Opening File" << endl;
                exit(1);
            }
            // initialise a vector of integers to store each line of text
            vector<int> map_line;
            int temp;

            for (int i = 0; i < MAP_HEIGHT; i++)
            {
                for (int j = 0; j < MAP_WIDTH; j++)
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
        }
};
