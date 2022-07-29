#include "splashkit.h"
#include "player.h"

int main()
{
    load_resource_bundle("mario", "mariobundle.txt");
    open_window("Platform Prototype", 800, 600);

    sprite mario_sprite = create_sprite("MarioSpriteSheet", "MarioAnim");
    point_2d mario_position;
    mario_position.x = 20;
    mario_position.y = 520;

    Player *player = new Player(new IdleState, mario_sprite, mario_position, false);

    while(!key_typed(ESCAPE_KEY))
    {
        clear_screen(COLOR_LIGHT_BLUE);

        player->update();
        player->get_input();

        process_events();
        refresh_screen(60);
    }

    delete player;
    free_resource_bundle("mario");
    return 0;
}