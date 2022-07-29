#include "splashkit.h"

class Player;
class PlayerState
{
    protected:
    Player *player;
    string player_state;
    
    public:
        virtual ~PlayerState(){};

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
        point_2d initial_position;
        bool facing_left;

    public:
        Player(PlayerState *state, sprite player_sprite, point_2d initial_position, bool facing_left) : state(nullptr)
        {
            this->change_state(state, "Initial");
            this->player_sprite = player_sprite;
            this->initial_position = initial_position;
            this->facing_left = facing_left;
            sprite_set_position(player_sprite, initial_position);
            sprite_set_scale(player_sprite, 3);
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
        }

        sprite get_player_sprite()
        {
            return this->player_sprite;
        }

        bool is_facing_left()
        {
            return this->facing_left;
        }

        void set_facing_left(bool facing_left)
        {
            this->facing_left = facing_left;
        }
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
        sprite_set_dx(player_sprite, 0);
        sprite_set_dy(player_sprite, 0);
        animation_routine(player, "LeftIdle", "RightIdle");
        run_once = true;
    }
    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void IdleState::get_input()
{
    if(key_down(LEFT_KEY))
    {
        this->player->set_facing_left(true);
        this->player->change_state(new RunState(0), "RunLeft");
    }
    if(key_down(RIGHT_KEY))
    {
        this->player->set_facing_left(false);
        this->player->change_state(new RunState(0), "RunRight");
    }
    if(key_typed(UP_KEY))
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
                sprite_set_dx(player_sprite, -2);
            else
                sprite_set_dx(player_sprite, 2);
        }

        animation_routine(player, "LeftRun", "RightRun");
        run_once = true;
    }

    if(player->is_facing_left())
    {
        if(sprite_dx(player->get_player_sprite()) > -6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())-0.08);
    }
    else
    {
        if(sprite_dx(player->get_player_sprite()) < 6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())+0.08);
    }

    sprite_update_routine_continuous(this->player->get_player_sprite());
}

void RunState::get_input()
{
    if(key_released(LEFT_KEY) || key_released(RIGHT_KEY))
    {
        this->player->change_state(new IdleState, "Idle");
    }
    if(key_typed(UP_KEY))
    {
        this->player->change_state(new JumpRiseState, "JumpRise");
    }
}

void JumpRiseState::update()
{
    if(!run_once)
    {
        initial_y = sprite_y(player->get_player_sprite());
        sprite_set_dy(player->get_player_sprite(), -8);
        animation_routine(player, "LeftJump", "RightJump");
        run_once = true;
        this->max_jump_height = 200 + abs((12 * sprite_dx(player->get_player_sprite())));
        write_line(max_jump_height);
    }

    //write_line(sprite_dy(player->get_player_sprite()));
    if(sprite_dy(player->get_player_sprite()) < 0)
        sprite_set_dy(player->get_player_sprite(), sprite_dy(player->get_player_sprite())+0.06);

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
    if(key_down(LEFT_KEY))
    {
        if(sprite_dx(player->get_player_sprite()) > -6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())-0.08);
    }
    if(key_down(RIGHT_KEY))
    {
        if(sprite_dx(player->get_player_sprite()) < 6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())+0.08);
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

    if(sprite_dy(player->get_player_sprite()) < 9.8)
        sprite_set_dy(player->get_player_sprite(), sprite_dy(player->get_player_sprite())+0.3);

    //write_line(sprite_dy(player->get_player_sprite()));

    //Bottom of screen
    if(sprite_y(player->get_player_sprite()) >= 840)
    {
        sprite_set_dy(player->get_player_sprite(), 0);
        sprite_set_y(player->get_player_sprite(), 840);
        if(player->is_facing_left() && key_down(LEFT_KEY))
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunLeft");
        else if(!player->is_facing_left() && key_down(RIGHT_KEY))
            this->player->change_state(new RunState(sprite_dx(player->get_player_sprite())), "RunRight");
        else
            this->player->change_state(new IdleState, "Idle");
    }

}

void JumpFallState::get_input()
{
    if(key_down(LEFT_KEY))
    {
        if(sprite_dx(player->get_player_sprite()) > -6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())-0.08);
    }
    if(key_down(RIGHT_KEY))
    {
        if(sprite_dx(player->get_player_sprite()) < 6)
            sprite_set_dx(player->get_player_sprite(), sprite_dx(player->get_player_sprite())+0.08);
    }
}