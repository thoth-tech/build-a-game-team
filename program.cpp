#include "splashkit.h"
#include "player.h"
#include "block.h"
#include <vector>

vector<Block> make_level_blocks_horizontal(double first_x, double block_width, double first_y, int number, int space, string type, vector<Block> level_blocks, bitmap blocks)
{
    for(int i = 0; i < number; i = i + (1 + space))
    {
        point_2d block_position;
        block_position.x = first_x + (i * block_width);
        block_position.y = first_y;

        if(type == "Floor")
        {
            FloorBlock level_block(blocks, block_position);
            level_blocks.push_back(level_block);
        }
        if(type == "Brick")
        {
            BrickBlock level_block(blocks, block_position);
            level_blocks.push_back(level_block);
        }
    }

    return level_blocks;
}

int main()
{
    load_resource_bundle("mario", "mariobundle.txt");
    open_window("Platform Prototype", 1600, 900);

    sprite mario_sprite = create_sprite("MarioSpriteSheet", "MarioAnim");
    point_2d mario_position;
    mario_position.x = 20;
    mario_position.y = 520;

    bitmap blocks = bitmap_named("MarioBlocks");
    Player *player = new Player(new IdleState, mario_sprite, mario_position, false);

    vector<Block> level_blocks;

    level_blocks = make_level_blocks_horizontal(0, 16, 860, 50, 0, "Floor", level_blocks, blocks);
    level_blocks = make_level_blocks_horizontal(50, 16, 700, 20, 0, "Floor", level_blocks, blocks);
    level_blocks = make_level_blocks_horizontal(800, 16, 600, 80, 5, "Floor", level_blocks, blocks);
    level_blocks = make_level_blocks_horizontal(300, 16, 490, 70, 0, "Brick", level_blocks, blocks);
    level_blocks = make_level_blocks_horizontal(300, 16, 300, 70, 2, "Brick", level_blocks, blocks);

    while(!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_BLACK);

        player->update();
        player->get_input();

        for(int i = 0; i < level_blocks.size(); i++)
        {
            level_blocks[i].draw_block();
        }

        float landing_value = 0;
        bool collision = false;
        for(int i = 0; i < level_blocks.size(); i++)
        {
            collision = level_blocks[i].test_top_collision(player->get_player_sprite());
            if (collision)
            {
                landing_value = level_blocks[i].get_top();
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
    return 0;
}