#include "splashkit.h"

#pragma once

class Camera
{
private:
    /* save camera position */
    point_2d *pos;

public:
    Camera(point_2d _init_pos)
    {
        pos = new (point_2d);
        pos.x = _init_pos.x;
        pos.y = _init_pos.y;

        void set_camera_position(_init_pos);
    };

    ~Camera()
    {
        delete pos;
    };

    /* if the point is out side the screen, we don't have to move the camera
        return value indicate whether we have move or not
        leave the redraw to the caller thread
    */
    bool move_for_player(point_2d *_player)
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
    };
};
