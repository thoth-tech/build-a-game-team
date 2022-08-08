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

void draw_hitbox(rectangle hitbox)
{
    draw_rectangle(COLOR_GREEN, hitbox);
}

vector<shared_ptr<Block>> make_layer(string file, int tile_size)
{
    vector<shared_ptr<Block>> level_blocks;
    LevelOjectsMap map(file, tile_size);

    int offset = 0;

    level_blocks = map.get_tiles(level_blocks, bitmap_named("SolidBlocks"), offset);
    offset += bitmap_cell_count(bitmap_named("SolidBlocks"));
    level_blocks = map.get_tiles(level_blocks, bitmap_named("NonSolidBlocks"), offset);
    offset += bitmap_cell_count(bitmap_named("NonSolidBlocks"));
    level_blocks = map.get_tiles(level_blocks, bitmap_named("PipeBlocks"), offset);

    return level_blocks;
}

int main()
{
    load_resource_bundle("player", "playerbundle.txt");
    load_resource_bundle("game_resources", "gameresources.txt");
    open_window("Platform Prototype", SCREEN_WIDTH, SCREEN_HEIGHT);

    sprite player_sprite = create_sprite("purpleGuy", "PlayerAnim");
    point_2d player_position;
    player_position.x = 20;
    player_position.y = 200;

    player_input input = make_player1_input();
    Player *player = new Player(new IdleState, player_sprite, player_position, false, input);
    
    //Change this variable for adding another layer
    int level_layers = 2;

    vector<shared_ptr<Block>> level_blocks;
    vector<shared_ptr<Block>> level_blocks_layer2;
    level_blocks = make_layer("file.txt", 64);

    if(level_layers == 2)
    {
        level_blocks_layer2 = make_layer("file2.txt", 64);
    }

    while (!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_BLACK);
        draw_bitmap("background", 0, 0, option_to_screen());

        for (int i = 0; i < level_blocks.size(); i++)
        {
            level_blocks[i]->draw_block();
            //draw_hitbox(level_blocks[i]->get_block_hitbox());
        }

        player->update();
        player->get_input();
        player->update_hitbox();
        //draw_hitbox(player->get_player_hitbox());


        if(level_layers == 2)
        {
            for (int i = 0; i < level_blocks_layer2.size(); i++)
            {
                level_blocks_layer2[i]->draw_block();
                //draw_hitbox(level_blocks[i]->get_block_hitbox());
            }
        }

        float landing_value = 0;
        string collision = "None";
        for (int i = 0; i < level_blocks.size(); i++)
        {
            if(level_blocks[i]->is_block_solid())
                collision = level_blocks[i]->test_collision(player->get_player_hitbox(), level_blocks[i]->get_block_hitbox());
            
            
            if (collision == "Top")
            {
                landing_value = level_blocks[i]->get_top();
                player->set_on_floor(true);
                player->set_landing_y_value(landing_value);
                break;
            }
            else if (collision == "Bottom")
            {
                player->set_player_dy(0);
                player->set_on_floor(false);
                sprite_set_y(player->get_player_sprite(), sprite_y(player->get_player_sprite()) + 1);
                player->change_state(new JumpFallState, "JumpFall");
                break;
            }
            else if (collision == "Left")
            {
                player->set_player_dx(0);
                sprite_set_x(player->get_player_sprite(), sprite_x(player->get_player_sprite()) - 5);
                break;
            }
            else if (collision == "Right")
            {
                player->set_player_dx(0);
                sprite_set_x(player->get_player_sprite(), sprite_x(player->get_player_sprite()) + 5);
                break;
            }
        }

        if (collision == "None")
            player->set_on_floor(false);

        //For testing purposes only
        //center_camera_on(player->get_player_sprite(), 0, 0);

        process_events();
        refresh_screen(60);
    }

    delete player;
    free_resource_bundle("player");
    free_resource_bundle("game_resources");
    return 0;
}