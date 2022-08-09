#include "splashkit.h"
#include "player.h"
#include "level.h"
#include "cellsheet.h"
#include <memory>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 896
#define TILE_SIZE 64

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

//Testing function for changing levels
shared_ptr<Level> change_level(shared_ptr<Level> current_level, vector<CellSheet> cell_sheets, int players)
{
    //shared_ptr<Level> current_level;
    if(key_typed(NUM_1_KEY))
    {
        shared_ptr<Level> level1(new Level1(cell_sheets, TILE_SIZE, players));
        current_level = level1;
    }

    if(key_typed(NUM_2_KEY))
    {
        shared_ptr<Level> level2(new Level2(cell_sheets, TILE_SIZE, players));
        current_level = level2;
    }
    

    return current_level;
}

int main(int argc, char *argv[])
{
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    open_window("Platform Prototype", SCREEN_WIDTH, SCREEN_HEIGHT);

    bool testing = true;
    bool custom_level = false;
    int players = 2;

    vector<string> cell_sheet_names;

    //Push the cell sheets used in the same order as the level editor
    cell_sheet_names.push_back("Solid");
    cell_sheet_names.push_back("NonSolid");
    cell_sheet_names.push_back("Pipe");
    cell_sheet_names.push_back("Water");
    cell_sheet_names.push_back("Toxic");

    vector<CellSheet> cell_sheets = make_cell_sheets(cell_sheet_names);
    shared_ptr<Level> current_level;

    std::vector<std::string> args(argv, argv+argc);
    try
    {
        for (size_t i = 1; i < args.size(); ++i) 
        {
            if (args[i] == "-p") 
            {
                players = std::stoi(args[i + 1]);
            }
            if (args[i] == "-l") 
            {
                vector<string> files;

                for(int j = 1; j < std::stoi(args[i + 1]) + 1; j++)
                {
                    files.push_back(args[i + 1 + j]);
                }

                shared_ptr<Level> load(new BlankLevel(cell_sheets, TILE_SIZE, players, std::stoi(args[i + 1]), files));
                current_level = load;
                custom_level = true;
            }
        }
    }
    catch(const std::exception& e)
    {
        write_line(e.what());
        write_line("Closing program");
        exit(1);
    }

    if(!custom_level)
    {
        shared_ptr<Level> level1(new Level1(cell_sheets, TILE_SIZE, players));
        current_level = level1;
    }
    
    while (!key_typed(ESCAPE_KEY))
    {
        current_level->update();

        if(testing)
            current_level = change_level(current_level, cell_sheets, players);

        process_events();
        refresh_screen(60);
    }

    //delete player;
    free_resource_bundle("player");
    free_resource_bundle("game_resources");
    return 0;
}