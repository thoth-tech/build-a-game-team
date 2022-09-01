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
            
            if(level_players.size() > 1 )
            {
                draw_bitmap("PinkEmptyBar", 400, 80, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("PinkHealthBar", 400, 80, option_part_bmp(0, 0, 64/3*level_players[1]->player_health, 32, option_to_screen()));

                if(level_players[1]->player_lives==3)
                {                
                    draw_bitmap("PinkLive", 335, 80, option_to_screen());
                    draw_bitmap("PinkLive", 300, 80, option_to_screen());
                    draw_bitmap("PinkLive", 265, 80, option_to_screen());
                }
                if(level_players[1]->player_lives==2)
                {
                    draw_bitmap("PinkLive", 300, 80, option_to_screen());
                    draw_bitmap("PinkLive", 265, 80, option_to_screen());
                }
                if(level_players[1]->player_lives==1)
                {
                    draw_bitmap("PinkLive", 265, 80, option_to_screen());
                }

                draw_bitmap("BlueEmptyBar", 400, 115, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("BlueHealthBar", 400, 115, option_part_bmp(0, 0, 64/3*level_players[0]->player_health, 32, option_to_screen()));

                if(level_players[0]->player_lives==3)
                {                
                    draw_bitmap("BlueLive", 335, 115, option_to_screen());
                    draw_bitmap("BlueLive", 300, 115, option_to_screen());
                    draw_bitmap("BlueLive", 265, 115, option_to_screen());
                }
                if(level_players[0]->player_lives==2)
                {
                    draw_bitmap("BlueLive", 300, 115, option_to_screen());
                    draw_bitmap("BlueLive", 265, 115, option_to_screen());
                }
                if(level_players[0]->player_lives==1)
                {
                    draw_bitmap("BlueLive", 265, 115, option_to_screen());
                } 
            }
            else
            {
                draw_bitmap("PurpleEmptyBar", 400, 80, option_part_bmp(0, 0, 64, 32, option_to_screen()));
                draw_bitmap("PurpleHealthBar", 400, 80, option_part_bmp(0, 0, 64/3*level_players[0]->player_health, 32, option_to_screen()));
                   
                if(level_players[0]->player_lives==3)
                {                
                    draw_bitmap("PurpleLive", 335, 80, option_to_screen());
                    draw_bitmap("PurpleLive", 300, 80, option_to_screen());
                    draw_bitmap("PurpleLive", 265, 80, option_to_screen());
                }
                if(level_players[0]->player_lives==2)
                {
                    draw_bitmap("PurpleLive", 300, 80, option_to_screen());
                    draw_bitmap("PurpleLive", 265, 80, option_to_screen());
                }
                if(level_players[0]->player_lives==1)
                {
                    draw_bitmap("PurpleLive", 265, 80, option_to_screen());
                }
            }
        };
};