#include "splashkit.h"
#include "level.h"
#include <memory>

shared_ptr<Level> get_next_level(int level, vector<CellSheet> cell_sheets, int tile_size, int players)
{ 
    shared_ptr<Level> next_level;
    switch(level)
    {
        case 1:
            {
                shared_ptr<Level> level1(new Level1(cell_sheets, tile_size, players));
                next_level = level1;
                break;
            }
        case 2:
            {
                shared_ptr<Level> level2(new Level2(cell_sheets, tile_size, players));
                next_level = level2;
                break;
            }
        case 3:
            {
                shared_ptr<Level> level3(new Level3(cell_sheets, tile_size, players));
                next_level = level3;
                break;
            }
        case 4:
            {
                shared_ptr<Level> level4(new Level4(cell_sheets, tile_size, players));
                next_level = level4;
                break;
            }
        // case 5:
        //     {
        //         shared_ptr<Level> level5(new Level5(cell_sheets, tile_size, players));
        //         next_level = level5;
        //         break;
        //     }
        default:
            {
                shared_ptr<Level> level1(new Level1(cell_sheets, tile_size, players));
                next_level = level1;
                break;
            }
    }

    return next_level;
}