#include "splashkit.h"
#include "types.h"

#pragma once

class Block
{
    protected:
        bitmap image;
        point_2d position;
        drawing_options opts;
        double top;
        string type;
        rectangle hitbox;
        bool is_solid = false;
        bool is_door = false;
        bool is_water = false;
        bool is_ladder = false;
        bool is_toxic = false;
        bool is_holdable = false;
        bool is_turnable = false;
        bool is_empty = false;
        bool is_picked_up = false;
        bool is_flowing = false;
        int cell;

    public:
        Block(bitmap cell_sheet, point_2d position)
        {
            this->image = cell_sheet;
            this->position = position;
            this->opts = option_defaults();
            this->top = position.y - bitmap_cell_height(image);
            make_hitbox();
        };

        ~Block(){
            // write_line("Deleting Block");
        };

        virtual void draw_block()
        {
            draw_bitmap(image, position.x, position.y, opts);
        };

        float get_top()
        {
            return this->top;
        };

        point_2d get_pos()
        {
            return this->position;
        };

        virtual void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y;
            hitbox.height = bitmap_cell_height(this->image);
            hitbox.width = bitmap_cell_width(this->image);
            this->hitbox = hitbox;
        };

        virtual string test_collision(rectangle one) = 0;

        rectangle get_block_hitbox()
        {
            return this->hitbox;
        };

        bool is_block_solid()
        {
            return this->is_solid;
        };

        bool is_block_door()
        {
            return this->is_door;
        };

        bool is_block_ladder()
        {
            return this->is_ladder;
        };

        bool is_block_water()
        {
            return this->is_water;
        };

        bool is_block_toxic()
        {
            return this->is_toxic;
        };
        bool is_holdable_pipe()
        {
            return this->is_holdable;
        };

        bool is_turnable_pipe()
        {
            return this->is_turnable;
        };

        bool is_empty_pipe()
        {
            return this->is_empty;
        };

        bool get_is_flowing()
        {
            return this->is_flowing;
        };

        void set_empty_pipe(bool new_value)
        {
            this->is_empty = new_value;
        };

        int get_cell()
        {
            return this->cell;
        };

        void set_picked_up(bool new_value)
        {
            this->is_picked_up = new_value;
        };

        void set_water(bool new_value)
        {
            this->is_water = new_value;
        };

        bool picked_up()
        {
            return this->is_picked_up;
        };

        void set_flowing(bool new_value)
        {
            this->is_flowing = new_value;
        };

        void change_cell_sheet(bitmap image)
        {
            this->image = image;
        };
};

class SolidBlock : public Block
{
    public:
        SolidBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        };

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth + 100 > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };
};

class Ladder : public Block
{
    public:
        Ladder(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->is_ladder = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        };

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class PipeBlock : public Block
{
    public:
        PipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override { return "None"; };
};

class WaterBlock : public Block
{
    private:
        animation anim;

    public:
        WaterBlock(bitmap cell_sheet, point_2d position) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->is_water = true;
            this->is_flowing = false;
            this->position = position;

            animation_script water_script = animation_script_named("CellAnim");
            animation anim = create_animation(water_script, "Water");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }

        string test_collision(rectangle one) override
        {
            string collision = "None";
            double dx = (one.x + one.width / 2) - (this->hitbox.x + this->hitbox.width / 2);
            double dy = (one.y + one.height / 2) - (this->hitbox.y + this->hitbox.height / 2);
            double width = (one.width + this->hitbox.width) / 2;
            double height = (one.height + this->hitbox.height) / 2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if (abs(dx) <= width && abs(dy) <= height)
            {
                if (crossWidth >= crossHeight)
                {
                    if (crossWidth > (-crossHeight))
                        collision = "Bottom";
                    else
                        collision = "Left";
                }
                else
                {
                    // Gave a bias to top collision to avoid right edge stopping player during movement
                    if (crossWidth - 200 > -(crossHeight))
                        collision = "Right";
                    else
                        collision = "Top";
                }
            }

            return collision;
        };

        void draw_block() override
        {
            if(is_flowing)
            {
                draw_bitmap("Water", position.x, position.y, opts);
                update_animation(this->anim);
                if (animation_ended(this->anim))
                    restart_animation(this->anim);
            }
        }
};

class ToxicBlock : public Block
{
    private:
        animation anim;

    public:
        ToxicBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->is_toxic = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;

            animation_script toxic_script = animation_script_named("CellAnim");
            animation anim = create_animation(toxic_script, "ToxicFlow");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }

        void draw_block() override
        {
            draw_bitmap("Toxic", position.x, position.y, opts);
            update_animation(this->anim);
            if (animation_ended(this->anim))
                restart_animation(this->anim);
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class DoorBlock : public Block
{
    private:
        animation anim;

    public:
        DoorBlock(bitmap cell_sheet, point_2d position) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->is_door = true;
            this->opts.draw_cell = this->cell;

            animation_script door_script = animation_script_named("CellAnim");
            animation anim = create_animation(door_script, "Door_Open");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
            make_hitbox();
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        void draw_block() override
        {
            draw_bitmap("Door", position.x, position.y, opts);
            update_animation(this->anim);
            if (animation_ended(this->anim))
                restart_animation(this->anim);
        }

        void make_hitbox() override
        {
            rectangle hitbox;
            hitbox.x = this->position.x + 10;
            hitbox.y = this->position.y + 20;
            hitbox.height = bitmap_cell_height(this->image) - 20;
            hitbox.width = bitmap_cell_width(this->image) - 30;
            this->hitbox = hitbox;
        };

        void open_portal()
        {
            animation_script door_script = animation_script_named("CellAnim");
            animation anim = create_animation(door_script, "Door_Portal");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }
};

class HoldablePipeBlock : public Block
{
    public:
        HoldablePipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_holdable = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };

        void draw_block() override
        {
            if(!is_picked_up)
                draw_bitmap(image, position.x, position.y, opts);
        };
};

class TurnablePipeBlock : public Block
{
    public:
        TurnablePipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_turnable = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};

class EmptyPipeBlock : public Block
{
    public:
        EmptyPipeBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_empty = true;
            this->is_flowing = true;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

        // Collision to test distance from how far a player is and if holding pipe to place
        string test_collision(rectangle one) override
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(this->hitbox)) && (rectangle_right(one) > rectangle_left(this->hitbox));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(this->hitbox)) && (rectangle_bottom(one) > rectangle_top(this->hitbox));
            bool collision = x_overlaps && y_overlaps;

            if (collision)
                return "Collision";
            else
                return "None";
        };
};