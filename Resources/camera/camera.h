#ifndef __camera_h
#define __camera_h

#include "splashkit.h"

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

    ~Camera(){};

    /* if the point is out side the screen, we don't have to move the camera
        return value indicate whether we have move or not
        leave the redraw to the caller thread
    */
    bool move_for_player(point_2d *_player);
};

#endif