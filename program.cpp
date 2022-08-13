#include "splashkit.h"
#include "cellsheet.h"
#include "screen.h"
#include "enemy.h"
#include "testing.h"
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
    for (int i = 0; i < cell_sheet_names.size(); i++)
    {
        bitmap new_bitmap = bitmap_named(cell_sheet_names[i]);
        CellSheet new_type(new_bitmap, cell_sheet_names[i], offset);
        offset += new_type.number_of_cells;
        cell_sheets.push_back(new_type);
    }

    return cell_sheets;
}

int main(int argc, char *argv[])
{
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    open_window("Platform Prototype", SCREEN_WIDTH, SCREEN_HEIGHT);
    window_toggle_border("Platform Prototype");
    bool test_screen = false;

    vector<string> cell_sheet_names;

    // Push the cell sheets used in the same order as the level editor
    cell_sheet_names.push_back("Solid");
    cell_sheet_names.push_back("Ladder");
    cell_sheet_names.push_back("Pipe");
    cell_sheet_names.push_back("Water");
    cell_sheet_names.push_back("Toxic");
    cell_sheet_names.push_back("HoldPipes");
    cell_sheet_names.push_back("TurnPipes");
    cell_sheet_names.push_back("Empty");

    vector<CellSheet> cell_sheets = make_cell_sheets(cell_sheet_names);
    std::vector<std::string> args(argv, argv + argc);
    vector<string> files;

    try
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "-l")
            {
                for (int j = 1; j < std::stoi(args[i + 1]) + 1; j++)
                    files.push_back(args[i + 1 + j]);
            }
            if (args[i] == "-t")
            {
                test_screen = true;
            }
        }
    }
    catch (const std::exception &e)
    {
        write_line(e.what());
        write_line("Closing program");
        exit(1);
    }

    shared_ptr<Screen> screen;
    if (test_screen)
    {
        // use this variable to test different screens
        shared_ptr<Screen> test_screen(new Screen(new CompanyIntroScreen, TILE_SIZE, cell_sheets, files));
        screen = test_screen;
    }
    else
    {
        shared_ptr<Screen> normal_screen(new Screen(new MenuScreen, TILE_SIZE, cell_sheets, files));
        screen = normal_screen;
    }

    while (!key_typed(ESCAPE_KEY))
    {
        screen->update();
        process_events();
        if (!music_playing())
        {
            play_music("LevelOne");
        }
        refresh_screen(60);
    }

    free_resource_bundle("player");
    free_resource_bundle("game_resources");

    //
    free_resource_bundle("roach");
    return 0;
}