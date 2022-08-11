#include "splashkit.h"
#pragma once

class Behaviour
{
    protected:
        sprite enemy_sprite;
        bool facing_left = true;
    public:
        Behaviour(sprite enemy_sprite)
        {
            this->enemy_sprite = enemy_sprite;
        };
        ~Behaviour(){};
        virtual void update() = 0;
        void update_animation(string left_anim, string right_anim)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, left_anim);
            else
                sprite_start_animation(enemy_sprite, right_anim);
        }

};

class RoachBehaviour : public Behaviour
{
    public:
        RoachBehaviour(sprite enemy_sprite) : Behaviour(enemy_sprite)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
            
        };
        ~RoachBehaviour()
        {
            if(!has_resource_bundle("roach"))
                load_resource_bundle("roach", "roachbundle.txt");
        };
        void update() override
        {
            if(facing_left)
            {
                if(sprite_position(enemy_sprite).x < rectangle_right(screen_rectangle()) - 100)
                    sprite_set_dx(enemy_sprite, 3);
                else
                {
                    facing_left = false;
                    sprite_set_dx(enemy_sprite, 0);
                    update_animation("LeftRun", "RightRun");
                }
            }
            else
            {
                if(sprite_position(enemy_sprite).x > rectangle_left(screen_rectangle()) + 100)
                    sprite_set_dx(enemy_sprite, -3);
                else
                {
                    facing_left = true;
                    update_animation("LeftRun", "RightRun");
                    sprite_set_dx(enemy_sprite, 0);
                }
            }
        };

};