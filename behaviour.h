#include "splashkit.h"
#pragma once

class Behaviour
{
    protected:
        sprite enemy_sprite;
        bool facing_left = true;
        bool on_floor = true;
        bool is_flying = false;
        bool once = false;

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

        void set_on_floor(bool new_value)
        {
            this->on_floor = new_value;
        };

        void set_facing_left(bool new_value)
        {
            this->once = false;
            this->facing_left = new_value;
        };

        bool is_on_floor()
        {
            return this->on_floor;
        };

        void set_y_value(float val)
        {
            sprite_set_y(enemy_sprite, val + (64 - sprite_height(enemy_sprite)));
        };

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
        };
        void update() override
        {
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            if(on_floor)
            {
                sprite_set_dy(enemy_sprite, 0);
            }
            else
            {
                sprite_set_dy(enemy_sprite, 10);
            }
        };

};

class BlobBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;

    public:
        BlobBehaviour(sprite enemy_sprite) : Behaviour(enemy_sprite)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
        };
        
        ~BlobBehaviour()
        {
        };

        void update() override
        {
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            if(on_floor)
            {
                sprite_set_dy(enemy_sprite, 0);
            }
            else
            {
                sprite_set_dy(enemy_sprite, 10);
            }
        };
};

class SnakeBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;

    public:
        SnakeBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
        };
        ~SnakeBehaviour()
        {
        };
        void update() override
        {
            for(int i = 0; i < level_players.size(); i++)
            {
                
            }
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            if(on_floor)
            {
                sprite_set_dy(enemy_sprite, 0);
            }
            else
            {
                sprite_set_dy(enemy_sprite, 10);
            }
        };

};

class RatBehaviour : public Behaviour
{
    private:
        vector<std::shared_ptr<Player>> level_players;

    public:
        RatBehaviour(sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : Behaviour(enemy_sprite)
        {
            this->level_players = level_players;
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
            
        };
        ~RatBehaviour()
        {
        };
        void update() override
        {
            for(int i = 0; i < level_players.size(); i++)
            {
                
            }
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            if(on_floor)
            {
                sprite_set_dy(enemy_sprite, 0);
            }
            else
            {
                sprite_set_dy(enemy_sprite, 10);
            }
        };

};

class WaterRatBehaviour : public Behaviour
{
    public:
        WaterRatBehaviour(sprite enemy_sprite) : Behaviour(enemy_sprite)
        {
            if(facing_left)
                sprite_start_animation(enemy_sprite, "LeftRun");
            else
                sprite_start_animation(enemy_sprite, "RightRun");
            
        };
        ~WaterRatBehaviour()
        {
        };
        void update() override
        {
            if(facing_left)
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, 3);
            }
            else
            {
                if(!once)
                {
                    update_animation("LeftRun", "RightRun");
                    once = true;
                }
                sprite_set_dx(enemy_sprite, -3);
            }

            if(on_floor)
            {
                sprite_set_dy(enemy_sprite, 0);
            }
            else
            {
                sprite_set_dy(enemy_sprite, 10);
            }
        };

};