#include "splashkit.h"
#include "player.h"
#include "block.h"
#include "playerinput.h"
#include "map.h"
#include <memory>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 896

int main()
{
    load_resource_bundle("mario", "mariobundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    open_window("Platform Prototype", SCREEN_WIDTH, SCREEN_HEIGHT);

    sprite mario_sprite = create_sprite("MarioSpriteSheet", "MarioAnim");
    point_2d mario_position;
    mario_position.x = 20;
    mario_position.y = 200;

    player_input input = make_player1_input();
    Player *player = new Player(new IdleState, mario_sprite, mario_position, false, input);
    vector<shared_ptr<Block>> level_blocks;

    LevelOjectsMap map("level1_morg.txt", 64);
    level_blocks = map.get_solid_objects(level_blocks);

    while(!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_BLACK);

        for(int i = 0; i < level_blocks.size(); i++)
        {
            level_blocks[i]->draw_block();
        }

        player->update();
        player->get_input();
        
        float landing_value = 0;
        bool collision = false;
        for(int i = 0; i < level_blocks.size(); i++)
        {
            collision = level_blocks[i]->test_top_collision(player->get_player_sprite());
            if (collision)
            {
                landing_value = level_blocks[i]->get_top();
                player->set_on_floor(true);
                player->set_landing_y_value(landing_value);
                break;
            }
        }

        if(!collision)
            player->set_on_floor(false);

        process_events();
        refresh_screen(60);
    }

    delete player;
    free_resource_bundle("mario");
    free_resource_bundle("game_resources");
    return 0;
}