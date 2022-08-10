#include "splashkit.h"
#include "player.h"
#include "block.h"
#include <memory>
#include <vector>

#pragma once

void check_solid_block_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Player>> level_players)
{
    for(int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for(int j = 0; j < layers.size(); j++)
        {
            for (int i = 0; i < layers[j].size(); i++)
            {
                if(!rect_on_screen(layers[j][i]->get_block_hitbox()))
                    continue;

                if(layers[j][i]->is_block_solid())
                    collision = layers[j][i]->test_collision(level_players[k]->get_player_hitbox(), layers[j][i]->get_block_hitbox());
                else
                    continue;

                if (collision == "Top")
                {
                    level_players[k]->set_on_floor(true);
                    level_players[k]->set_landing_y_value(layers[j][i]->get_top());
                    break;
                }
                else if (collision == "Bottom")
                {
                    if(level_players[k]->is_on_floor())
                        break;

                    level_players[k]->set_player_dy(0);
                    level_players[k]->set_on_floor(false);
                    sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                    level_players[k]->change_state(new JumpFallState, "JumpFall");
                    break;
                }
                else if (collision == "Left")
                {
                    level_players[k]->set_player_dx(0);
                    sprite_set_x(level_players[k]->get_player_sprite(), sprite_x(level_players[k]->get_player_sprite()) - 3);
                    break;
                }
                else if (collision == "Right")
                {
                    level_players[k]->set_player_dx(0);
                    sprite_set_x(level_players[k]->get_player_sprite(), sprite_x(level_players[k]->get_player_sprite()) + 3);
                    break;
                }
            }

            if(collision != "None")
                break;
        }

        if (collision == "None")
            level_players[k]->set_on_floor(false);
    }
}