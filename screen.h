#include "splashkit.h"
#include "level.h"
#include "cellsheet.h"
#include "get_level.h"
#include "button.h"
#include <memory>
#include <vector>

class Screen;

class ScreenState
{
protected:
    Screen *screen;
    string screen_state;

public:
    virtual ~ScreenState(){};

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
    int level_number = 1;
    int max_levels = 6;
    shared_ptr<Level> current_level;

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
        if (this->state != nullptr)
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
    vector<shared_ptr<Button>> menu_buttons;
    int offset = -80;
    int num_buttons = 5;
    int selection = 0;

public:
    MenuScreen(){};

    ~MenuScreen(){};

    void update() override;
};

class PreLevelScreen : public ScreenState
{
private:
    bool run_once = false;

public:
    PreLevelScreen(){};

    ~PreLevelScreen(){};

    void update() override;
};

class LevelScreen : public ScreenState
{
private:
    bool run_once = false;
    bool pause = false;
    bool pause_run_once;

public:
    LevelScreen(){};

    ~LevelScreen(){};

    void update() override;

    // Inputs for testing functions
    void testing_input()
    {
        if (key_typed(M_KEY))
        {
            this->screen->level_number = 1;
            this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
            this->screen->change_state(new MenuScreen, "Menu");
        }

        if (!pause)
        {
            if (key_typed(NUM_1_KEY))
            {
                if (this->screen->level_number < this->screen->max_levels)
                {
                    this->screen->level_number += 1;
                    this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
                }
            }

            if (key_typed(NUM_2_KEY))
            {
                if (this->screen->level_number > 1)
                {
                    this->screen->level_number -= 1;
                    this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
                }
            }
        }

        if (key_typed(RETURN_KEY))
        {
            if (pause)
                pause = false;
            else
            {
                pause_run_once = false;
                pause = true;
            }
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

class WinScreen : public ScreenState
{
private:
    bool run_once = false;

public:
    WinScreen(){};

    ~WinScreen(){};

    void update() override;
};

void CompanyIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);

    bitmap title = bitmap_named("Company1");
    bitmap title2 = bitmap_named("Company2");
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE;
    string text = "Games";

    draw_bitmap(title2, pt.x - bitmap_width(title2) / 2 + 5, pt.y - bitmap_height(title2) / 2 - 5, option_to_screen());
    draw_bitmap(title, pt.x - bitmap_width(title) / 2, pt.y - bitmap_height(title) / 2, option_to_screen());

    draw_text(text, COLOR_BROWN, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2 + 5, (pt.y - text_height(text, screen_font, font_size) / 2) + 200 - 5, option_to_screen());
    draw_text(text, font_color, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2, (pt.y - text_height(text, screen_font, font_size) / 2) + 200, option_to_screen());

    if (key_typed(RETURN_KEY))
    {
        this->screen->change_state(new TeamIntroScreen, "TeamIntro");
    }
}

void TeamIntroScreen::update()
{
    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);

    bitmap logo = bitmap_named("TeamLogo");
    font screen_font = font_named("DefaultFont");
    int font_size = 30;
    color font_color = COLOR_WHITE;
    string text = "Morgaine Barter";
    string text2 = "Daniel Agbay, Lily Lan, Robert Osbourne";
    string text3 = "Jiahao Zheng, Roy Chen";
    string text4 = "And";
    string text5 = "Lachlan Morgan";
    string text6 = "Present";

    draw_bitmap(logo, pt.x - bitmap_width(logo) / 2, pt.y - bitmap_height(logo) / 2 - 150, option_to_screen());

    draw_text(text, font_color, screen_font, font_size, pt.x - text_width(text, screen_font, font_size) / 2, (pt.y - text_height(text, screen_font, font_size) / 2) + 150, option_to_screen());
    draw_text(text2, font_color, screen_font, font_size, pt.x - text_width(text2, screen_font, font_size) / 2, (pt.y - text_height(text2, screen_font, font_size) / 2) + 150 + text_height(text2, screen_font, font_size) * 1, option_to_screen());
    draw_text(text3, font_color, screen_font, font_size, pt.x - text_width(text3, screen_font, font_size) / 2, (pt.y - text_height(text3, screen_font, font_size) / 2) + 150 + text_height(text3, screen_font, font_size) * 2, option_to_screen());
    draw_text(text4, font_color, screen_font, font_size, pt.x - text_width(text4, screen_font, font_size) / 2, (pt.y - text_height(text4, screen_font, font_size) / 2) + 150 + text_height(text4, screen_font, font_size) * 3, option_to_screen());
    draw_text(text5, font_color, screen_font, font_size, pt.x - text_width(text5, screen_font, font_size) / 2, (pt.y - text_height(text5, screen_font, font_size) / 2) + 150 + text_height(text5, screen_font, font_size) * 4, option_to_screen());
    draw_text(text6, font_color, screen_font, font_size, pt.x - text_width(text6, screen_font, font_size) / 2, (pt.y - text_height(text6, screen_font, font_size) / 2) + 150 + text_height(text6, screen_font, font_size) * 6, option_to_screen());

    if (key_typed(RETURN_KEY))
    {
        this->screen->change_state(new MenuScreen, "Menu");
    }
}

string get_button_text(int id)
{
    string text = "";

    switch (id)
    {
    case 1:
        text = "1 PLAYER";
        break;
    case 2:
        text = "2 PLAYER";
        break;
    case 3:
        text = "BACKSTORY";
        break;
    case 4:
        text = "CREDITS";
        break;
    case 5:
        text = "EXIT";
        break;
    default:
        break;
    }

    return text;
}

void MenuScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);

    if (!run_once)
    {
        for (int i = 0; i < num_buttons; i++)
        {
            string text = get_button_text(i + 1);
            shared_ptr<Button> test(new Button(bitmap_named("Button"), offset, i, text));
            offset += 100;
            menu_buttons.push_back(test);
        }
        run_once = true;
    }

    if (!music_playing())
    {
        play_music("MenuMusic.mp3");
        set_music_volume(0.5f);
    }

    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_bitmap("MenuBg", 0, 0, option_to_screen());

    bitmap title = bitmap_named("Title");
    drawing_options scale = option_scale_bmp(2, 2);
    draw_bitmap(title, pt.x - bitmap_width(title) / 2, 100, scale);

    for (int i = 0; i < menu_buttons.size(); i++)
    {
        if (menu_buttons[i]->get_id() == selection)
            menu_buttons[i]->set_selected(true);
        else
            menu_buttons[i]->set_selected(false);

        menu_buttons[i]->draw();
    }

    if (key_typed(UP_KEY))
    {
        selection -= 1;

        if (selection < 0)
            selection = 0;
    }
    if (key_typed(DOWN_KEY))
    {
        selection += 1;

        if (selection > num_buttons - 1)
            selection = num_buttons - 1;
    }
    if (key_typed(RETURN_KEY))
    {
        switch (selection)
        {
        case 0:
        {
            play_sound_effect("Select");
            this->screen->set_players(1);
            stop_music();
            this->screen->change_state(new PreLevelScreen, "Pre Level");
        }
        break;
        case 1:
        {
            play_sound_effect("Select");
            this->screen->set_players(2);
            stop_music();
            this->screen->change_state(new PreLevelScreen, "Pre Level");
        }
        break;
        case 4:
        {
            exit(0);
        }
        break;
        default:
            break;
        }
    }
}

void PreLevelScreen::update()
{
    if (!run_once)
    {
        if (this->screen->get_files().size() != 0)
        {
            shared_ptr<Level> custom_level(new BlankLevel(this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players(), this->screen->get_files().size(), this->screen->get_files()));
            this->screen->current_level = custom_level;
            this->screen->max_levels = 1;
        }
        else
        {
            this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
        }

        run_once = true;
    }

    point_2d pt = screen_center();
    clear_screen(COLOR_BLACK);
    draw_text("Level " + std::to_string(this->screen->level_number), COLOR_WHITE, pt.x, pt.y);
    draw_text(this->screen->current_level->get_level_name(), COLOR_WHITE, pt.x, pt.y + 10);

    if (key_typed(RETURN_KEY))
    {
        this->screen->change_state(new LevelScreen, "Level");
    }
}

void LevelScreen::update()
{
    if (!pause)
    {
        this->screen->current_level->update();

        if (this->screen->current_level->player1_complete && this->screen->current_level->player2_complete)
        {
            if (!timer_started("DanceTime"))
                start_timer("DanceTime");
            int time = timer_ticks("DanceTime") / 1000; // Changed to int, was u_int
            if (time > 2)
            {
                stop_timer("DanceTime");
                if (this->screen->level_number < this->screen->max_levels)
                {
                    this->screen->level_number += 1;
                    this->screen->change_state(new PreLevelScreen, "Pre Level");
                }
                else
                {
                    this->screen->change_state(new WinScreen, "Win");
                }
            }
        }
        else
        {
            if (this->screen->current_level->is_player1_out_of_lives || this->screen->current_level->is_player2_out_of_lives)
            {
                write_line("Changing to game over");
                this->screen->level_number = 1;
                this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
                this->screen->change_state(new GameOverScreen, "GameOver");
            }
        }
    }
    else
    {
        if (!pause_run_once)
        {
            fill_rectangle(rgba_color(0, 0, 0, 50), screen_rectangle());
            pause_run_once = true;
        }
        draw_text("Pause", COLOR_WHITE, 800, 400, option_to_screen());
    }

    testing_input();
}

void GameOverScreen::update()
{
    set_camera_x(0);
    set_camera_y(0);
    clear_screen(COLOR_BLACK);

    point_2d pt = screen_center();
    string game_over_text = "Game Over";
    font screen_font = font_named("DefaultFont");
    int font_size = 80;
    color font_color = COLOR_WHITE_SMOKE;

    draw_text(game_over_text, font_color, screen_font, font_size, pt.x - text_width(game_over_text, screen_font, font_size) / 2, (pt.y - text_height(game_over_text, screen_font, font_size) / 2) - 300, option_to_screen());

    bitmap game_over = bitmap_named("GameOver");
    fill_rectangle(COLOR_WHITE_SMOKE, pt.x - bitmap_width(game_over) / 2 - 10, pt.y - bitmap_height(game_over) / 2 - 10, bitmap_width(game_over) + 20, bitmap_height(game_over) + 20);
    draw_bitmap(game_over, pt.x - bitmap_width(game_over) / 2, pt.y - bitmap_height(game_over) / 2, option_to_screen());

    if (key_typed(RETURN_KEY))
    {
        this->screen->change_state(new MenuScreen, "Menu");
    }
}

void WinScreen::update()
{
    clear_screen(COLOR_BLACK);
    draw_text("You Won", COLOR_WHITE, 800, 400, option_to_screen());
    draw_text("Good Job", COLOR_WHITE, 800, 410, option_to_screen());
    draw_text("Press Enter to go to Menu", COLOR_WHITE, 740, 420, option_to_screen());

    if (key_typed(RETURN_KEY))
    {
        this->screen->level_number = 1;
        this->screen->current_level = get_next_level(this->screen->level_number, this->screen->get_cell_sheets(), this->screen->get_tile_size(), this->screen->get_players());
        this->screen->change_state(new MenuScreen, "Menu");
    }
}