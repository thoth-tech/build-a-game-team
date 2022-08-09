#include "splashkit.h"
#include "playerinput.h"
#pragma once


//Player Physics variables
#define MAX_JUMP_HEIGHT 127
#define JUMP_MOMENTUM_RATE 8
#define MAX_FALL_SPEED 9
#define FALL_RATE 0.8
#define MAX_RUN_SPEED 5
#define RUN_ACCEL 0.0981
#define JUMP_START_SPEED 9.81
#define JUMP_RISE_LOSS 0.1


class Player;
class PlayerState
{
    protected:
    Player *player;
    string player_state;
    
    public:
        virtual ~PlayerState()
        {
            //write_line("Deleting Player");
        };

        void set_state(Player *player, string player_state)
        {
            this->player = player;
            this->player_state = player_state;
        };

        string get_type()
        {
            return this->player_state;
        };

        virtual void update() = 0;
        virtual void get_input() = 0;
};

class Player
{
    private:
        PlayerState *state;
        sprite player_sprite;
        point_2d position;
        bool facing_left;
        bool on_floor;
        float landing_y_value;
        rectangle hitbox;
        
    public:
        player_input input;

        Player(PlayerState *state, sprite player_sprite, point_2d initial_position, bool facing_left, player_input input) : state(nullptr)
        {
            this->change_state(state, "Initial");
            this->player_sprite = player_sprite;
            this->position = initial_position;
            this->landing_y_value = initial_position.y;
            this->facing_left = facing_left;
            this->on_floor = true;
            this->input = input;
            sprite_set_position(player_sprite, this->position);
            make_hitbox();
        };

        ~Player()
        {
            delete state;
        };

        void change_state(PlayerState *new_state, string type)
        {
            if(this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, type);
        };

        void update()
        {
            this->state->update();
        };

        void get_input()
        {
            this->state->get_input();
        };

        void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y + 10;
            hitbox.height = sprite_height(this->player_sprite) - 10;
            hitbox.width = sprite_width(this->player_sprite);
            this->hitbox = hitbox;
        };

        void update_hitbox()
        {
            point_2d current_position = sprite_position(this->player_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y + 10;
        };

        sprite get_player_sprite()
        {
            return this->player_sprite;
        };

        rectangle get_player_hitbox()
        {
            return this->hitbox;
        };

        string get_player_state_type()
        {
            return this->state->get_type();
        }

        bool is_facing_left()
        {
            return this->facing_left;
        };

        void set_facing_left(bool facing_left)
        {
            this->facing_left = facing_left;
        };

        bool is_on_floor()
        {
            return this->on_floor;
        };

        void set_on_floor(bool new_value)
        {
            this->on_floor = new_value;
        };

        void set_landing_y_value(float landing_y_value)
        {
            this->landing_y_value = landing_y_value;
        };

        float get_landing_y_value()
        {
            return this->landing_y_value;
        };

        void set_player_dy(float value)
        {
            sprite_set_dy(this->player_sprite, value);
        };

        void set_player_dx(float value)
        {
            sprite_set_dx(this->player_sprite, value);
        };
};

class IdleState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        IdleState(){};

        ~IdleState(){};

        void update() override;
        void get_input() override;
};

class RunState : public PlayerState
{
    private:
        bool run_once = false;
        float dx;

    public:
        RunState(float dx)
        {
            this->dx = dx;
        };

        ~RunState(){};

        void update() override;
        void get_input() override;
};

class JumpRiseState : public PlayerState
{
    private:
        bool run_once = false;
        float initial_y;
        float max_jump_height;

    public:
        JumpRiseState(){};

        ~JumpRiseState(){};

        void update() override;
        void get_input() override;
};

class JumpFallState : public PlayerState
{
    private:
        bool run_once = false;

    public:
        JumpFallState(){};

        ~JumpFallState(){};

        void update() override;
        void get_input() override;
};

void sprite_fall(sprite sprite)
{
    if(sprite_dy(sprite) < MAX_FALL_SPEED)
            sprite_set_dy(sprite, sprite_dy(sprite) + FALL_RATE);
}

void animation_routine(Player* player, string left_anim, string right_anim)
{
    if(player->is_facing_left())
        sprite_start_animation(player->get_player_sprite(), left_anim);
    else
        sprite_start_animation(player->get_player_sprite(), right_anim);
}

void sprite_update_routine_continuous(sprite player_sprite)
{
    draw_sprite(player_sprite);
    if(sprite_animation_has_ended(player_sprite))
        sprite_replay_animation(player_sprite);
    update_sprite(player_sprite);
}

void IdleState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if(!run_once)
    {   
        if(player->is_on_floor())
        {
            sprite_set_dx(player_sprite, 0);
            sprite_set_dy(player_sprite, 0);
        }
        animation_routine(player, "LeftIdle", "RightIdle");
        run_once = true;
    }

    if(player->is_on_floor())
    {
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player->get_player_sprite(), 0);
        sprite_set_y(player->get_player_sprite(), player->get_landing_y_value());
    }
    else
       sprite_fall(player->get_player_sprite());

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void IdleState::get_input()
{
    if(key_down(player->input.left_key))
    {
        this->player->set_facing_left(true);
        this->player->change_state(new RunState(0), "RunLeft");
    }
    if(key_down(player->input.right_key))
    {
        this->player->set_facing_left(false);
        this->player->change_state(new RunState(0), "RunRight");
    }
    if(key_typed(player->input.jump_key) && player->is_on_floor())
    {
        this->player->change_state(new JumpRiseState, "JumpRise");
    }
}

void RunState::update()
{
    sprite player_sprite = this->player->get_player_sprite();
    if(!run_once)
    {
        if(dx == 0)
        {
            if(player->is_facing_left())
                sprite_set_dx(player_sprite, -1);
            else
                sprite_set_dx(player_sprite, 1);
        }

        animation_routine(player, "LeftRun", "RightRun");
        run_once = true;
    }

    if(player->is_facing_left())
    {
        if(sprite_dx(player->get_player_sprite()) > -MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) - RUN_ACCEL);
    }
    else
    {
        if(sprite_dx(player->get_player_sprite()) < MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) + RUN_ACCEL);
    }

    if(player->is_on_floor())
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        sprite_set_y(player->get_player_sprite(), player->get_landing_y_value());
    }
    else
       sprite_fall(player->get_player_sprite());

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void RunState::get_input()
{
    if(key_released(player->input.left_key) || key_released(player->input.right_key))
    {
        this->player->change_state(new IdleState, "Idle");
    }
    if(key_typed(player->input.jump_key)&& player->is_on_floor())
    {
        this->player->change_state(new JumpRiseState, "JumpRise");
    }
}

void JumpRiseState::update()
{
    if(!run_once)
    {
        //this->player->set_on_floor(false);
        initial_y = sprite_y(player->get_player_sprite());
        sprite_set_dy(player->get_player_sprite(), -JUMP_START_SPEED);
        animation_routine(player, "LeftJump", "RightJump");
        run_once = true;
        this->max_jump_height = MAX_JUMP_HEIGHT + abs((JUMP_MOMENTUM_RATE * sprite_dx(player->get_player_sprite())));
        //write_line(max_jump_height);
    }

    this->player->set_on_floor(false);

    //write_line(sprite_dy(player->get_player_sprite()));
    if(sprite_dy(player->get_player_sprite()) < 0)
        sprite_set_dy(player->get_player_sprite(), sprite_dy(player->get_player_sprite()) + JUMP_RISE_LOSS);

    sprite_update_routine_continuous(this->player->get_player_sprite());

    float current_y = sprite_y(player->get_player_sprite());

    if((initial_y - current_y) > max_jump_height)
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        this->player->change_state(new JumpFallState, "JumpFall");
    }
}

void JumpRiseState::get_input()
{
    if(key_down(player->input.left_key))
    {
        if(sprite_dx(player->get_player_sprite()) > -MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) - RUN_ACCEL);
    }
    if(key_down(player->input.right_key))
    {
        if(sprite_dx(player->get_player_sprite()) < MAX_RUN_SPEED)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite()) + RUN_ACCEL);
    }
}

void JumpFallState::update()
{
    if(!run_once)
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        run_once = true;
    }
    sprite_update_routine_continuous(this->player->get_player_sprite());

    sprite_fall(player->get_player_sprite());

    //write_line(sprite_dy(player->get_player_sprite()));

    if(player->is_on_floor())
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        sprite_set_y(player->get_player_sprite(), player->get_landing_y_value());
        if(player->is_facing_left() && key_down(LEFT_KEY) && player->is_on_floor())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunLeft");
        else if(!player->is_facing_left() && key_down(RIGHT_KEY) && player->is_on_floor())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunRight");
        else
            this->player->change_state(new IdleState, "Idle");
    }

}

void JumpFallState::get_input()
{
    if(player->is_on_floor())
    {
        if(key_down(player->input.left_key) && player->is_facing_left())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunLeft");
        else if(key_down(player->input.right_key) && !player->is_facing_left())
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunRight");
        else
            this->player->change_state(new IdleState, "Idle");
    }
    else
    {
        if(key_down(player->input.left_key))
        {
            if(sprite_dx(player->get_player_sprite()) > -6)
                sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())-0.072);
        }
        if(key_down(player->input.right_key))
        {
            if(sprite_dx(player->get_player_sprite()) < 6)
                sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())+0.072);
        }
    }
}