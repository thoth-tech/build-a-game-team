#include "splashkit.h"
#include "player.h"
#include "machinehelper.h"
#include <memory>
#include <vector>

class RatMachine;

class RatMachineState
{
    protected:
        RatMachine *rat;
        string rat_state;

    public:
        virtual ~RatMachineState()
        {};

        void set_state(RatMachine *rat, string rat_state)
        {
            this->rat = rat;
            this->rat_state = rat_state;
        };

        string get_type()
        {
            return this->rat_state;
        };

        virtual void update() = 0;
};

class RatMachine
{
    private:
        RatMachineState *state;
        sprite enemy_sprite;
        bool facing_left;
        vector<std::shared_ptr<Player>> level_players;
        

    public:
        RatMachine(RatMachineState *state, sprite enemy_sprite, vector<std::shared_ptr<Player>> level_players) : state(nullptr)
        {
            this->enemy_sprite = enemy_sprite;
            this->level_players = level_players;
            sprite_start_animation(enemy_sprite, "LeftCrawl");
            this->change_state(state, "Idle");
        };

        ~RatMachine()
        {
            delete state;
        };

        void change_state(RatMachineState *new_state, string type)
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

class RatMove : public RatMachineState
{
    private:
        bool run_once = false;

    public:
        RatMove(){};

        ~RatMove(){};

        void update() override;
};

class RatBite : public RatMachineState
{
    private:
        bool run_once = false;

    public:
        RatBite(){};

        ~RatBite(){};

        void update() override;
};

class RatCrawl : public RatMachineState
{
    private:
        bool run_once = false;

    public:
        RatCrawl(){};

        ~RatCrawl(){};

        void update() override;
};

void RatMove::update()
{
    sprite rat_sprite = this->rat->get_sprite();
    vector<std::shared_ptr<Player>> players = this->rat->get_level_players();

    if(this->rat->get_facing_left())
    {
        sprite_set_dx(rat_sprite, 7);
        set_proper_direction(rat_sprite, "LeftRun");
    }
    else
    {
        sprite_set_dx(rat_sprite, -7);
        set_proper_direction(rat_sprite, "RightRun");
    }

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d rat_pos = to_screen(sprite_position(rat_sprite));

        double x_dist = rat_pos.x - player_pos.x; 
        if(abs(x_dist) > 500)
        {
            this->rat->change_state(new RatCrawl, "Crawl");
            break;
        }
    }
}

void RatBite::update()
{
    sprite rat_sprite = this->rat->get_sprite();
    vector<std::shared_ptr<Player>> players = this->rat->get_level_players();

    sprite_set_dx(rat_sprite, 0);

    if(this->rat->get_facing_left())
        set_proper_direction(rat_sprite, "LeftBite");
    else
        set_proper_direction(rat_sprite, "RightBite");

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d rat_pos = to_screen(sprite_position(rat_sprite));

        double x_dist = rat_pos.x - player_pos.x; 
        double y_dist = rat_pos.y - player_pos.y;
        if(abs(x_dist) > 18 && abs(y_dist) >17 )
        {
            this->rat->change_state(new RatMove, "Run");
            break;
        }
    }
}

void RatCrawl::update()
{
    sprite rat_sprite = this->rat->get_sprite();
    vector<std::shared_ptr<Player>> players = this->rat->get_level_players();

    if(this->rat->get_facing_left())
    {
        sprite_set_dx(rat_sprite, 5);
        set_proper_direction(rat_sprite, "LeftCrawl");
    }
    else
    {
        sprite_set_dx(rat_sprite, -5);
        set_proper_direction(rat_sprite, "RightCrawl");
    }

    for(int i = 0; i < players.size(); i++)
    {
        point_2d player_pos = to_screen(sprite_position(players[i]->get_player_sprite()));
        point_2d rat_pos = to_screen(sprite_position(rat_sprite));

        double x_dist = rat_pos.x - player_pos.x; 
        
        if(abs(x_dist) < 20)
        {
            this->rat->change_state(new RatBite, "Bite");
            break;
        }
    }
}