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

void BossMove::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftRun");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightRun");
    }
}

class BossRise : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossRise(){};

        ~BossRise(){};

        void update() override;
};

void BossRise::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftRise");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightRise");
    }
}

class BossSpotPlayer : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossSpotPlayer(){};

        ~BossSpotPlayer(){};

        void update() override;
};

void BossSpotPlayer::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftSpotPlayer");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightSpotPlayer");
    }
}

class BossBattleCry : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossBattleCry(){};

        ~BossBattleCry(){};

        void update() override;
};

void BossBattleCry::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "LeftBattleCry");
    }
    else
    {
        sprite_set_dx(boss_sprite, 0);
        set_proper_direction(boss_sprite, "RightBattleCry");
    }
}


class BossAttack : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossAttack(){};

        ~BossAttack(){};

        void update() override;
};

void BossAttack::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftAttack");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightAttack");
    }
}

class BossDecend : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossDecend(){};

        ~BossDecend(){};

        void update() override;
};

void BossDecend::update()
{
    sprite boss_sprite = this->boss->get_sprite();

    if(this->boss->get_facing_left())
    {
        sprite_set_dx(boss_sprite, 3);
        set_proper_direction(boss_sprite, "LeftDecend");
    }
    else
    {
        sprite_set_dx(boss_sprite, -3);
        set_proper_direction(boss_sprite, "RightDecend");
    }
}

class BossDying : public BossMachineState
{
    private:
        bool run_once = false;

    public:
        BossDying(){};

        ~BossDying(){};

        void update() override;
};

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