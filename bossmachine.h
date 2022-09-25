#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>

class BossMachine;

class BossMachineState
{
    protected:
        BossMachine *boss;
        string boss_state;

    public:
        virtual ~BossMachineState()
        {};

        void set_state(BossMachine *boss, string boss_state)
        {
            this->boss = boss;
            this->boss_state = boss_state;
        };

        string get_type()
        {
            return this->boss_state;
        };

        virtual void update() = 0;
};

class BossMachine
{
    private:
        BossMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        

    public:
        BossMachine(BossMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "LeftRun");
            this->change_state(state, "Idle");
        };

        ~BossMachine()
        {
            delete state;
        };

        void change_state(BossMachineState *new_state, string type)
        {
            if (this->state != nullptr)
                delete this->state;
            this->state = new_state;
            this->state->set_state(this, type);
        };

        void update()
        {
            this->state->update();
        };

        bool get_facing_left()
        {
            return facing_left;
        };

        void set_facing_left(bool new_value)
        {
            this->facing_left = new_value;
        };

        sprite get_sprite()
        {
            return this->enemy_sprite;
        };

        vector<std::shared_ptr<Player>> get_level_players()
        {
            return this->level_players;
        };

        string get_state_type()
        {
            return this->state->get_type();
        };
};

class BossMove : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossMove(){};

        ~BossMove(){};

        void update() override;
};

class BossRise : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossRise(){};

        ~BossRise(){};

        void update() override;
};

class BossSpotPlayer : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossSpotPlayer(){};

        ~BossSpotPlayer(){};

        void update() override;
};

class BossBattleCry : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossBattleCry(){};

        ~BossBattleCry(){};

        void update() override;
};

class BossAttack : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossAttack(){};

        ~BossAttack(){};

        void update() override;
};

class BossDescend : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossDescend(){};

        ~BossDescend(){};

        void update() override;
};

class BossDying : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossDying(){};

        ~BossDying(){};

        void update() override;
};

void BossMove::update()
{
    sprite boss_sprite = this->boss->get_sprite();


    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftRun");
        //if hit by player change to dead else change to bossrise state
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightRun");
    }
}

void BossRise::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftRise");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossSpotPlayer, "LeftSpotPlayer");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightRise");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossSpotPlayer, "RightSpotPlayer");
    }
}

void BossSpotPlayer::update()
{
    sprite boss_sprite = this->boss->get_sprite();
    vector<std::shared_ptr<Player>> players = this->boss->get_level_players();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftSpotPlayer");
        if(sprite_animation_has_ended(boss_sprite)) //and close to player 
        for(int i = 0; i < players.size(); i++)
        {
            point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
            point_2d boss_pos = to_screen(sprite_position(boss_sprite));

            double x_dist = boss_pos.x - player_pos.x; 
            double y_dist = boss_pos.y - player_pos.y;
            if(abs(x_dist) < 200 && abs(y_dist) < 17)
            {
                this->boss->change_state(new BossAttack, "LeftAttack");
                break;
            }
            else
            {
                this->boss->change_state(new BossBattleCry, "LeftBattleCry");
            }
        }
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightSpotPlayer");
        if(sprite_animation_has_ended(boss_sprite)) //and close to player 
        for(int i = 0; i < players.size(); i++)
        {
            point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
            point_2d boss_pos = to_screen(sprite_position(boss_sprite));

            double x_dist = boss_pos.x - player_pos.x; 
            double y_dist = boss_pos.y - player_pos.y;
            if(abs(x_dist) < 200 && abs(y_dist) < 17)
            {
                this->boss->change_state(new BossAttack, "RightAttack");
                break;
            }
            else
            {
                this->boss->change_state(new BossBattleCry, "RightBattleCry");
            }
        }
    }
}

void BossBattleCry::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "LeftBattleCry");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossSpotPlayer, "LeftSpotPlayer");

    }
    else
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "RightBattleCry");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossSpotPlayer, "RightSpotPlayer");
    }
}

void BossAttack::update()
{
    sprite boss_sprite = this->boss->get_sprite();
    //vector<std::shared_ptr<Player>> players = this->boss->get_level_players(); 

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftAttack");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossDescend, "LeftDescend");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightAttack");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossDescend, "RightDescend");
    }
}

void BossDescend::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftDescend");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossMove, "LeftMove");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightDescend");
        if(sprite_animation_has_ended(boss_sprite))
            this->boss->change_state(new BossMove, "RightMove");
    }
}

void BossDying::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "LeftDying");
    }
    else
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "RightDying");
    }
}