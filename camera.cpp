#include "splashkit.h"
#include "camera.h"

/* if the point is out side the screen, we don't have to move the camera
    return value indicate whether we have move or not
    leave the redraw to the caller thread
*/
bool Camera::move_for_player(point_2d *_player)
{
    bool should_move = point_on_screen(*_player);

    if (!should_move)
    {
        return should_move;
    }
    double dif_x = _player.x - pos.x;
    double dif_y = _player.y - pos.y;

    // update the camera pos
    *pos = *_pplayer;

    move_camera_by(dif_x, dif_y);
    return should_move;
}
