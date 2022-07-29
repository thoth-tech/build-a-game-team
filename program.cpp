#include "splashkit.h"
#include "player.h"

int main()
{
    load_resource_bundle("mario", "mariobundle.txt");
    open_window("Platform Prototype", 1600, 900);

    sprite mario_sprite = create_sprite("MarioSpriteSheet", "MarioAnim");
    point_2d mario_position;
    mario_position.x = 20;
    mario_position.y = 520;

    Player *player = new Player(new IdleState, mario_sprite, mario_position, false);

    while(!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_BLACK);

        player->update();
        player->get_input();

        process_events();
        refresh_screen(60);
    }

    delete player;
    free_resource_bundle("mario");
    return 0;
}