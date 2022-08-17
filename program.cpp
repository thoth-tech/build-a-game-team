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
    // Load Resources
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    load_resource_bundle("roach", "roachbundle.txt");
    load_resource_bundle("snake", "snakebundle.txt");

    open_window("Below The Surface", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    bool test_screen = false;
    bool window_border = true;

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

    // Timers
    create_timer("Dying");
    create_timer("DamageTimerP1");
    create_timer("DamageTimerP2");
    create_timer("DyingTimerP1");
    create_timer("DyingTimerP2");
    create_timer("SpawnTimerP1");
    create_timer("SpawnTimerP2");
    create_timer("DanceTime");

    vector<CellSheet> cell_sheets = make_cell_sheets(cell_sheet_names);
    std::vector<std::string> args(argv, argv + argc);
    vector<string> files;

    try
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            if (args[i] == "-l")
            {
                for (int j = 1; j < std::stoi(args[i + 1]) + 2; j++)
                    files.push_back(args[i + 1 + j]);
            }
            if (args[i] == "-t")
            {
                test_screen = true;
            }
            if (args[i] == "-b")
            {
                window_border = false;
            }
        }
    }
    catch (const std::exception &e)
    {
        write_line(e.what());
        write_line("Closing program");
        exit(1);
    }
    
    if(!window_border)
        window_toggle_border("Below The Surface");

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

    while (!key_typed(ESCAPE_KEY) && !quit_requested())
    {
        screen->update();
        process_events();
        refresh_screen(60);
    }

    free_resource_bundle("player");
    free_resource_bundle("game_resources");
    free_resource_bundle("roach");
    free_resource_bundle("snake");
    free_all_timers();
    return 0;
}