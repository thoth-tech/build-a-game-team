#include "splashkit.h"
#include "player.h"
#include <memory>

#pragma once

class HUD
{
    private:
        //Added the current level players so you have easy access to them
        vector<std::shared_ptr<Player>> level_players;

    public:
        HUD(vector<std::shared_ptr<Player>> level_players)
        {
            this->level_players = level_players;
        };
        ~HUD(){};

        void update()
        {
            //Whatever you put in here will draw to the front layer of the screen
            //You can put things anywhere on the screen I just used to top as a small example
            //Use option_to_screen() to glue things to the screen so they move with the camera
            fill_rectangle(COLOR_BLACK, 0, 0, screen_width(), 40, option_to_screen());

            draw_text("P1 Lives: " + std::to_string(level_players[0]->player_lives) + " P1 Health: " + std::to_string(level_players[0]->player_health), COLOR_WHITE, 0, 0, option_to_screen());
            if(level_players.size() > 1)
                draw_text("P2 Lives: " + std::to_string(level_players[1]->player_lives) + " P2 Health: " + std::to_string(level_players[1]->player_health), COLOR_WHITE, 0, 10, option_to_screen());
                
            draw_text("Hello Lily :)", COLOR_WHITE, 0, 30, option_to_screen());
        };
};