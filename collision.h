#include "splashkit.h"
#include "player.h"
#include "playerinput.h"
#include "block.h"
#include <memory>
#include <vector>

#pragma once

string test_hitbox_collision(rectangle one, rectangle two)
{
    bool x_overlaps = (rectangle_left(one) < rectangle_right(two)) && (rectangle_right(one) > rectangle_left(two));
    bool y_overlaps = (rectangle_top(one) < rectangle_bottom(two)) && (rectangle_bottom(one) > rectangle_top(two));
    bool collision = x_overlaps && y_overlaps;
    
    if(collision)
        return "Collision";
    else
        return "None";
};

bool test_rectangle_collision(rectangle one, rectangle two)
{
    bool x_overlaps = (rectangle_left(one) < rectangle_right(two)) && (rectangle_right(one) > rectangle_left(two));
    bool y_overlaps = (rectangle_top(one) < rectangle_bottom(two)) && (rectangle_bottom(one) > rectangle_top(two));
    bool collision = x_overlaps && y_overlaps;
    
    if(collision)
        return true;
    else
        return false;
};

void check_solid_block_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Player>> level_players, rectangle test_area)
{
    for(int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for(int j = 0; j < layers.size(); j++)
        {
            for (int i = 0; i < layers[j].size(); i++)
            {
                //if(!rect_on_screen(layers[j][i]->get_block_hitbox()))
                    //continue;
                
                // if(!test_rectangle_collision(test_area, layers[j][i]->get_block_hitbox()))
                //     continue;

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

                    // Checks if the player is on ladder, if yes then it will go to ClimbIdle
                    if(level_players[k]->is_on_ladder())
                    {
                        sprite_start_animation(level_players[k]->get_player_sprite(), "ClimbIdle");
                        level_players[k]->set_player_dy(0);
                        level_players[k]->set_on_floor(false);
                        sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                        break;
                    }

                    level_players[k]->set_player_dy(0);
                    level_players[k]->set_on_floor(false);
                    sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                    level_players[k]->change_state(new JumpFallState, "JumpFall");
                    break;
                }
                else if (collision == "Left")
                {
                    // Checks if the player is on ladder, if yes then it will go to ClimbIdle
                    if (level_players[k]->is_on_ladder())
                    {
                        sprite_start_animation(level_players[k]->get_player_sprite(), "ClimbIdle");
                    }
                    level_players[k]->set_player_dx(0);
                    level_players[k]->set_on_floor(false);
                    sprite_set_x(level_players[k]->get_player_sprite(), sprite_x(level_players[k]->get_player_sprite()) - 3);
                    break;
                }
                else if (collision == "Right")
                {
                    // Checks if the player is on ladder, if yes then it will go to ClimbIdle
                    if (level_players[k]->is_on_ladder())
                        sprite_start_animation(level_players[k]->get_player_sprite(), "ClimbIdle");
                    level_players[k]->set_player_dx(0);
                    level_players[k]->set_on_floor(false);
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

void check_door_block_collisions(shared_ptr<DoorBlock> door, vector<shared_ptr<Player>> level_players)
{
    for(int i = 0; i < level_players.size(); i++)
    {
        string collision = "None";
        collision = door->test_collision(level_players[i]->get_player_hitbox(), door->get_block_hitbox());

        if(collision != "None" && level_players[i]->is_on_floor())
            if(level_players[i]->get_state_type() != "Dance")
            {
                level_players[i]->set_player_won(true);
                door->open_portal();
                level_players[i]->change_state(new DanceState, "Dance");
            }
    }
}

void check_ladder_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Player>> level_players)
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

                if(layers[j][i]->is_block_ladder())
                    collision = layers[j][i]->test_collision(level_players[k]->get_player_hitbox(), layers[j][i]->get_block_hitbox());
                else
                    continue;

                if(collision != "None" && (key_typed(level_players[k]->input.jump_key) || key_typed(level_players[k]->input.crouch_key)))
                {
                    if(!level_players[k]->is_on_ladder())
                    {
                        level_players[k]->set_on_ladder(true);
                        level_players[k]->change_state(new ClimbState, "Climb");
                    }
                    break;
                }
                else if(collision != "None" && level_players[k]->get_state_type() == "Climb")
                {
                    level_players[k]->set_on_ladder(true);
                    break;
                }
            }

            if(collision == "None")
            {
                level_players[k]->set_on_ladder(false);
            }
        }
    }
}

void check_enemy_solid_block_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Enemy>> level_enemies, rectangle test_area)
{
    for(int k = 0; k < level_enemies.size(); k++)
    {
        if(!rect_on_screen(level_enemies[k]->get_enemy_hitbox()))
            continue;

        string collision = "None";
        for(int j = 0; j < layers.size(); j++)
        {
            for (int i = 0; i < layers[j].size(); i++)
            {
                // if(!rect_on_screen(layers[j][i]->get_block_hitbox()))
                //     continue;

                // if(!test_rectangle_collision(test_area, layers[j][i]->get_block_hitbox()))
                //     continue;

                if(layers[j][i]->is_block_solid())
                    collision = layers[j][i]->test_collision(level_enemies[k]->get_enemy_hitbox(), layers[j][i]->get_block_hitbox());
                else
                    continue;

                if (collision == "Top")
                {
                    level_enemies[k]->get_ai()->set_on_floor(true);
                    level_enemies[k]->get_ai()->set_y_value(layers[j][i]->get_top());
                    break;
                }
                else if (collision == "Bottom")
                {
                    if(level_enemies[k]->get_ai()->is_on_floor())
                        break;
                    
                }
                else if (collision == "Left")
                {
                    level_enemies[k]->get_ai()->set_facing_left(false);
                    break;
                }
                else if (collision == "Right")
                {
                    level_enemies[k]->get_ai()->set_facing_left(true);
                    break;
                }
            }

            if(collision != "None")
                break;
        }

        if (collision == "None")
            level_enemies[k]->get_ai()->set_on_floor(false);
    }
}

void check_enemy_player_collisions(vector<shared_ptr<Enemy>> level_enemies, vector<shared_ptr<Player>> level_players)
{
    for(int i = 0; i < level_enemies.size(); i++)
    {
        if(!rect_on_screen(level_enemies[i]->get_enemy_hitbox()))
            continue;

        if(level_enemies[i]->get_dead())
            continue;

        string collision = "None";
        for(int j = 0; j < level_players.size(); j++)
        {
            collision = test_hitbox_collision(level_enemies[i]->get_enemy_hitbox(), level_players[j]->get_player_hitbox());

            if(collision != "None" && level_players[j]->get_state_type() != "JumpFall")
            {
                if(!timer_started(timer_named("DamageTimer")))
                {
                    level_players[j]->player_health -= 1;
                    write_line("Player Health: " + std::to_string(level_players[j]->player_health));
                    start_timer("DamageTimer");
                }

                int time = timer_ticks("DamageTimer")/1000;

                //Invincibility frames
                if(!(time < 2))
                    stop_timer("DamageTimer");
            }
            else if(collision != "None" && level_players[j]->get_state_type() == "JumpFall")
            {
                //Jumped on enemy
                level_players[j]->change_state(new JumpRiseState, "JumpRise");
                level_players[j]->set_player_dx(0);
                level_enemies[i]->set_dead(true);
            }
            else
                break;
        }
    }
}

void check_water_block_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Player>> level_players)
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

                if(layers[j][i]->is_block_water())
                    collision = layers[j][i]->test_collision(level_players[k]->get_player_hitbox(), layers[j][i]->get_block_hitbox());
                else
                    continue;

                if (collision == "Left")
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
    }
}

void check_toxic_block_collisions(vector<vector<shared_ptr<Block>>> layers, vector<shared_ptr<Player>> level_players)
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

                if(layers[j][i]->is_block_toxic())
                    collision = layers[j][i]->test_collision(level_players[k]->get_player_hitbox(), layers[j][i]->get_block_hitbox());
                else
                    continue;

               if(collision != "None")
               {
                    if(!timer_started(timer_named("DamageTimer")))
                    {
                        level_players[j]->player_health -= 1;
                        write_line("Player Health: " + std::to_string(level_players[j]->player_health));
                        start_timer("DamageTimer");
                    }

                    int time = timer_ticks("DamageTimer")/1000;

                    //Invincibility frames
                    if(!(time < 2))
                    {
                        stop_timer("DamageTimer");
                        break;
                    }
               }
            }

            if(collision != "None")
                break;
        }
    }
}