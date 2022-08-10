#include "splashkit.h"
#include "level.h"
#include "cellsheet.h"
#include <memory>
#include <vector>

class Screen;

class ScreenState
{
    protected:
        Screen *screen;
        string screen_state;

    
    public:
        virtual ~ScreenState()
        {};

        void set_state(Screen *screen, string screen_state)
        {
            this->screen = screen;
            this->screen_state = screen_state;
        };

        string get_type()
        {
            return this->screen_state;
        };

        virtual void update() = 0;
};

class Screen
{
    private:
        ScreenState *state;
        int tile_size;
        int players = 1;
        vector<CellSheet> cell_sheets;
        vector<string> files;
        
    public:
        Screen(ScreenState *state, int tile_size, vector<CellSheet> cell_sheets, vector<string> files) : state(nullptr)
        {
            this->cell_sheets = cell_sheets;
            this->tile_size = tile_size;
            this->files = files;
            this->change_state(state, "Initial");
        };

        ~Screen()
        {
            delete state;
        };

        void change_state(ScreenState *new_state, string type)
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

        int get_tile_size()
        {
            return this->tile_size;
        };

        int get_players()
        {
            return this->players;
        };

        void set_players(int num)
        {
            this->players = num;
        };

        vector<CellSheet> get_cell_sheets()
        {
            return this->cell_sheets;
        };

        vector<string> get_files()
        {
            return this->files;
        };
};

class CompanyIntroScreen : public ScreenState
{
    private:
        bool run_once = false;

    public:
        CompanyIntroScreen(){};

        ~CompanyIntroScreen(){};

        void update() override;
};

class TeamIntroScreen : public ScreenState
{
    private:
        bool run_once = false;

    public:
        TeamIntroScreen(){};

        ~TeamIntroScreen(){};

        void update() override;
};

class MenuScreen : public ScreenState
{
    private:
        bool run_once = false;

    public:
        MenuScreen(){};

        ~MenuScreen(){};

        void update() override;
};

class LevelScreen : public ScreenState
{
    private:
        bool run_once = false;
        shared_ptr<Level> current_level;

    public:
        LevelScreen(){};

        ~LevelScreen(){};

        void update() override;

        //Inputs for testing functions
        void testing_input()
        {
            if(key_typed(M_KEY))
            {
                this->screen->change_state(new MenuScreen, "Menu");
            }

            if(key_typed(NUM_1_KEY))
            {
                shared_ptr<Level> level1(new Level1(this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players()));
                this->current_level = level1;
            }

            if(key_typed(NUM_2_KEY))
            {
                shared_ptr<Level> level2(new Level2(this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players()));
                this->current_level = level2;
            }
        };
};

class GameOverScreen : public ScreenState
{
    private:
        bool run_once = false;

    public:
        GameOverScreen(){};

        ~GameOverScreen(){};

        void update() override;
};


void CompanyIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_text("Company Intro Screen", COLOR_WHITE, pt.x, pt.y);

    if(key_typed(RETURN_KEY))
    {
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");
    }
}

void TeamIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_text("Test Screen", COLOR_WHITE, pt.x, pt.y);

    if(key_typed(RETURN_KEY))
    {
        this->screen->change_state(new MenuScreen, "Menu");
    }
}

void MenuScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_text("Menu Screen", COLOR_WHITE, pt.x - 20, pt.y);
    draw_text("Press 1 for 1P Game", COLOR_WHITE, pt.x - 20, pt.y + 10);
    draw_text("Press 2 for 2P Game", COLOR_WHITE, pt.x - 20, pt.y + 20);

    if(key_typed(NUM_1_KEY))
    {
        this->screen->set_players(1);
        this->screen->change_state(new LevelScreen, "Level");
    }
    if(key_typed(NUM_2_KEY))
    {
        this->screen->set_players(2);
        this->screen->change_state(new LevelScreen, "Level");
    }
}

void LevelScreen::update()
{
    if(!run_once)
    {
        if(this->screen->get_files().size() != 0)
        {
            shared_ptr<Level> custom_level(new BlankLevel(this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players(), this->screen->get_files().size(), this->screen->get_files()));
            current_level = custom_level;
        }
        else
        {
            shared_ptr<Level> normal_level(new Level1(this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players()));
            this->current_level = normal_level;
        }

        run_once = true;
    }

    this->current_level->update();

    testing_input();
}

void GameOverScreen::update()
{
    clear_screen(COLOR_BLACK);
    draw_text("Test Screen", COLOR_WHITE, 0, 0);
}