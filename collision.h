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

    if (collision)
        return "Collision";
    else
        return "None";
};

bool test_rectangle_collision(rectangle one, rectangle two)
{
    bool x_overlaps = (rectangle_left(one) < rectangle_right(two)) && (rectangle_right(one) > rectangle_left(two));
    bool y_overlaps = (rectangle_top(one) < rectangle_bottom(two)) && (rectangle_bottom(one) > rectangle_top(two));
    bool collision = x_overlaps && y_overlaps;

    if (collision)
        return true;
    else
        return false;
};

void check_solid_block_collisions(vector<vector<shared_ptr<SolidBlock>>> solid_blocks, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for (int j = 0; j < solid_blocks.size(); j++)
        {
            for (int i = 0; i < solid_blocks[j].size(); i++)
            {
                // if(!rect_on_screen(layers[j][i]->get_block_hitbox()))
                // continue;

                // if(!test_rectangle_collision(test_area, layers[j][i]->get_block_hitbox()))
                //     continue;

                collision = solid_blocks[j][i]->test_collision(level_players[k]->get_player_hitbox());

                if (collision == "Top")
                {
                    if (level_players[k]->is_on_ladder())
                    {
                        level_players[k]->set_player_dy(0);
                        level_players[k]->set_on_floor(true);
                        sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                        level_players[k]->change_state(new IdleState, "Idle");
                        break;
                    }
                    level_players[k]->set_on_floor(true);
                    sprite_set_y(level_players[k]->get_player_sprite(), solid_blocks[j][i]->get_top());
                    break;
                }
                else if (collision == "Bottom")
                {
                    if (level_players[k]->is_on_floor())
                        break;

                    // Checks if the player is on ladder, if yes then it will go to ClimbIdle
                    if (level_players[k]->is_on_ladder())
                    {
                        if (!sound_effect_playing("HeadHit"))
                            play_sound_effect("HeadHit");
                        sprite_start_animation(level_players[k]->get_player_sprite(), "ClimbIdle");
                        level_players[k]->set_player_dy(0);
                        level_players[k]->set_on_floor(false);
                        sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                        break;
                    }

                    if (!sound_effect_playing("HeadHit"))
                        play_sound_effect("HeadHit");
                    level_players[k]->set_player_dy(0);
                    sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                    level_players[k]->set_on_floor(false);
                    sprite_set_y(level_players[k]->get_player_sprite(), sprite_y(level_players[k]->get_player_sprite()) + 1);
                    level_players[k]->change_state(new JumpFallState, "JumpFall");
                    break;
                }
                else if (collision == "Left")
                {
                    // Checks if the player is on ladder, if yes then it will go to ClimbIdle
                    if (level_players[k]->is_on_ladder())
                        sprite_start_animation(level_players[k]->get_player_sprite(), "ClimbIdle");

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
            if (collision != "None")
                break;
        }

        if (collision == "None")
            level_players[k]->set_on_floor(false);
    }
}

void check_door_block_collisions(shared_ptr<DoorBlock> door, vector<shared_ptr<Player>> level_players)
{
    for (int i = 0; i < level_players.size(); i++)
    {
        string collision = "None";
        collision = door->test_collision(level_players[i]->get_player_hitbox());

        if (collision != "None" && level_players[i]->is_on_floor())
            if (level_players[i]->get_state_type() != "Dance")
            {
                level_players[i]->set_player_won(true);
                door->open_portal();
                level_players[i]->change_state(new DanceState, "Dance");
            }
    }
}

void check_ladder_collisions(vector<vector<shared_ptr<Ladder>>> ladders, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for (int j = 0; j < ladders.size(); j++)
        {
            for (int i = 0; i < ladders[j].size(); i++)
            {
                if (!rect_on_screen(ladders[j][i]->get_block_hitbox()))
                    continue;

                collision = ladders[j][i]->test_collision(level_players[k]->get_player_hitbox());

                if (collision != "None" && (key_typed(level_players[k]->input.jump_key) || key_typed(level_players[k]->input.crouch_key)))
                {
                    if (!level_players[k]->is_on_ladder())
                    {
                        level_players[k]->set_on_ladder(true);
                        level_players[k]->change_state(new ClimbState, "Climb");
                    }
                    break;
                }
                else if (collision != "None" && level_players[k]->get_state_type() == "Climb")
                {
                    level_players[k]->set_on_ladder(true);
                    break;
                }
            }

            if (collision == "None")
            {
                level_players[k]->set_on_ladder(false);
            }
        }
    }
}

void check_enemy_solid_block_collisions(vector<vector<shared_ptr<SolidBlock>>> solid_blocks, vector<shared_ptr<Enemy>> level_enemies)
{
    for (int k = 0; k < level_enemies.size(); k++)
    {
        if (!rect_on_screen(level_enemies[k]->get_enemy_hitbox()))
            continue;

        string collision = "None";
        for (int j = 0; j < solid_blocks.size(); j++)
        {
            for (int i = 0; i < solid_blocks[j].size(); i++)
            {
                // if(!rect_on_screen(layers[j][i]->get_block_hitbox()))
                //     continue;

                // if(!test_rectangle_collision(test_area, layers[j][i]->get_block_hitbox()))
                //     continue;

                collision = solid_blocks[j][i]->test_collision(level_enemies[k]->get_enemy_hitbox());


                if (collision == "Top")
                {
                    level_enemies[k]->get_ai()->set_on_floor(true);
                    level_enemies[k]->get_ai()->set_y_value(solid_blocks[j][i]->get_top());
                    break;
                }
                else if (collision == "Bottom")
                {
                    if (level_enemies[k]->get_ai()->is_on_floor())
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

            if (collision != "None")
                break;
        }

        if (collision == "None")
            level_enemies[k]->get_ai()->set_on_floor(false);
    }
}

void check_enemy_player_collisions(vector<shared_ptr<Enemy>> level_enemies, vector<shared_ptr<Player>> level_players)
{
    for (int i = 0; i < level_enemies.size(); i++)
    {
        if (!rect_on_screen(level_enemies[i]->get_enemy_hitbox()))
            continue;

        if (level_enemies[i]->get_dead())
            continue;

        string collision = "None";
        for (int j = 0; j < level_players.size(); j++)
        {
            collision = level_enemies[i]->test_collision(level_players[j]->get_player_hitbox());

            if (collision != "Top" && collision != "None")
            {
                string damage_timer = "DamageTimerP" + std::to_string(j + 1);
                if (!timer_started(timer_named(damage_timer)))
                {
                    level_players[j]->player_health -= 1;
                    start_timer(damage_timer);
                    level_players[j]->change_state(new HurtState, "Hurt");
                }

                int time = timer_ticks(damage_timer) / 1000;

                // Invincibility frames
                if (!(time < 2))
                    stop_timer(damage_timer);
            }
            else if (collision != "None" && !level_players[j]->is_on_floor())
            {
                // Jumped on enemy
                level_players[j]->change_state(new JumpRiseState, "JumpRise");
                level_players[j]->set_player_dx(0);
                level_enemies[i]->set_dead(true);
            }
        }
        if (collision != "None")
            break;
    }
}

void check_water_block_collisions(vector<vector<shared_ptr<WaterBlock>>> water, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for (int j = 0; j < water.size(); j++)
        {
            for (int i = 0; i < water[j].size(); i++)
            {
                if (!rect_on_screen(water[j][i]->get_block_hitbox()))
                    continue;

                if (water[j][i]->get_is_flowing())
                    collision = water[j][i]->test_collision(level_players[k]->get_player_hitbox());
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

            if (collision != "None")
                break;
        }
    }
}

void check_toxic_block_collisions(vector<vector<shared_ptr<ToxicBlock>>> toxic, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        string collision = "None";
        for (int j = 0; j < toxic.size(); j++)
        {
            for (int i = 0; i < toxic[j].size(); i++)
            {
                if (!rect_on_screen(toxic[j][i]->get_block_hitbox()))
                    continue;

                collision = toxic[j][i]->test_collision(level_players[k]->get_player_hitbox());

                if (collision != "None")
                {
                    string damage_timer = "DamageTimerP" + std::to_string(k + 1);
                    if (!timer_started(timer_named(damage_timer)))
                    {
                        level_players[k]->player_health -= 1;
                        start_timer(damage_timer);
                    }

                    int time = timer_ticks(damage_timer) / 1000;

                    // Invincibility frames
                    if (!(time < 2))
                    {
                        stop_timer(damage_timer);
                        break;
                    }
                }
            }
        }
    }
}

void check_holdable_pipe_block_collisions(vector<vector<shared_ptr<HoldablePipeBlock>>> pipes, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        // if the player is already hold a holding a pipe, we skip this player
        if (level_players[k]->with_pipe())
        {
            continue;
        }
        string collision = "None";
        for (int j = 0; j < pipes.size(); j++)
        {
            for (int i = 0; i < pipes[j].size(); i++)
            {
                if (!pipes[j][i]->picked_up())
                    collision = pipes[j][i]->test_collision(level_players[k]->get_player_hitbox());
                else
                    continue;

                if (collision != "None")
                {
                    //Pink and purple can interact with these pipes
                    if(pipes[j][i]->get_cell() < 6)
                    {
                        if(level_players[k]->get_player_id() == 3 || level_players[k]->get_player_id() == 2)
                        {
                            pipes[j][i]->set_picked_up(true);
                            level_players[k]->pick_pipe(pipes[j][i]);
                            break;
                        }
                    }
                    //Blue and purple can interact with these pipes
                    else if(pipes[j][i]->get_cell() >=6 && pipes[j][i]->get_cell() < 12)
                    {
                        if(level_players[k]->get_player_id() == 3 || level_players[k]->get_player_id() == 1)
                        {
                            pipes[j][i]->set_picked_up(true);
                            level_players[k]->pick_pipe(pipes[j][i]);
                            break;
                        }
                    }
                    //Everyone can interact with these pipes
                    else
                    {
                        pipes[j][i]->set_picked_up(true);
                        level_players[k]->pick_pipe(pipes[j][i]);
                        break;
                    }
                }
            }
        }
    }
}

void check_empty_pipe_block_collisions(vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes, vector<shared_ptr<Player>> level_players)
{
    for (int k = 0; k < level_players.size(); k++)
    {
        // if the player do not hold a holdable pipe, we skip this player
        if (!level_players[k]->with_pipe())
            continue;

        string collision = "None";
        for (int j = 0; j < empty_pipes.size(); j++)
        {
            for (int i = 0; i < empty_pipes[j].size(); i++)
            {
                collision = empty_pipes[j][i]->special_collision(level_players[k]->get_player_hitbox());

                if (collision != "None")
                {
                    if(empty_pipes[j][i]->get_cell() == level_players[k]->get_held_pipe()->get_cell())
                    {
                        write_line("Collision between Held Pipe Id: " + std::to_string(level_players[k]->get_held_pipe()->get_cell()) + " Empty Block Id: " + std::to_string(empty_pipes[j][i]->get_cell()));
                        // player place this pipe
                        level_players[k]->place_pipe(empty_pipes[j][i]);
                        empty_pipes[j][i]->change_cell_sheet(bitmap_named("HoldPipes"));
                        empty_pipes[j][i]->set_flowing(false); 
                        empty_pipes[j][i]->set_stopped(true);      
                    }
                }
            }
        }
    }
}

void check_water_empty_block_collisions(vector<vector<shared_ptr<EmptyPipeBlock>>> empty_pipes, vector<vector<shared_ptr<WaterBlock>>> water)
{
    string collision = "None";
    for(int i = 0; i < empty_pipes.size(); i++)
    {
        for(int j = 0; j < empty_pipes[i].size(); j++)
        {
                //Found Empty Space
            for(int k = 0; k < water.size(); k++)
            {
                for(int l = 0; l < water[k].size(); l++)
                {
                    //Found Water Block
                    if(water[k][l]->get_is_flowing())
                    {
                        if(empty_pipes[i][j]->test_collision(water[k][l]->get_block_hitbox()) != "None" && !empty_pipes[i][j]->get_is_stopped())
                        {
                            water[k][l]->set_stopped(false);
                        }
                        else if (empty_pipes[i][j]->test_collision(water[k][l]->get_block_hitbox()) != "None" && empty_pipes[i][j]->get_is_stopped())
                        {
                            water[k][l]->set_stopped(true);
                            water[k][l]->set_flowing(false);
                        }
                    }
                    else
                        continue;
                }
            }
        }
    }
}

void check_water_water_block_collisions(vector<vector<shared_ptr<WaterBlock>>> water, vector<vector<shared_ptr<WaterBlock>>> water2)
{
    string collision = "None";
    for(int i = 0; i < water.size(); i++)
    {
        for(int j = 0; j < water[i].size(); j++)
        {
            if(water[i][j]->get_is_flowing())
            {
                //Found flowing block
                for(int k = 0; k < water2.size(); k++)
                {
                    for(int l = 0; l < water2[k].size(); l++)
                    {
                        //Found Water Block
                        if(water[i][j]->get_is_flowing())
                        {
                            if(water[i][j]->test_collision(water2[k][l]->get_block_hitbox()) == "Bottom")
                                water2[k][l]->set_stopped(false);
                        }
                    }
                }
            }
            else
            {
                for(int k = 0; k < water2.size(); k++)
                {
                    for(int l = 0; l < water2[k].size(); l++)
                    {
                        if(water[i][j]->test_collision(water2[k][l]->get_block_hitbox()) == "Bottom")
                            water2[k][l]->set_stopped(true);
                    }
                }
            }
        }
    }
}