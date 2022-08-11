#include "splashkit.h"
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

        ~Block()
        {
            //write_line("Deleting Block");
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

        virtual string test_collision(rectangle one, rectangle two) = 0;

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

        string test_collision(rectangle one, rectangle two) override
        {
            string collision = "None";
            double dx = (one.x + one.width/2) - (two.x + two.width/2);
            double dy = (one.y + one.height/2) - (two.y + two.height/2);
            double width = (one.width + two.width)/2;
            double height = (one.height + two.height)/2;
            double crossWidth = width * dy;
            double crossHeight = height * dx;

            if(abs(dx) <= width && abs(dy) <= height)
            {
                if(crossWidth>=crossHeight)
                {
                    if(crossWidth + 100 > (-crossHeight))
                    {
                        collision = "Bottom";
                        //draw_text("Bottom",COLOR_WHITE,0,480);
                    }
                    else
                    {
                        collision = "Left";
                        //draw_text("Left",COLOR_WHITE,0,480);
                    }
                }
                else
                {
                    //Gave a bias to top collision to avoid right edge stopping player during movement
                    if(crossWidth - 200 > -(crossHeight))
                    {
                        collision = "Right";
                        //draw_text("Right",COLOR_WHITE,0,480);
                    }
                    else
                    {
                        collision = "Top";
                        //draw_text("Top",COLOR_WHITE,0,480);
                    }
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

        string test_collision(rectangle one, rectangle two) override 
        {
            bool x_overlaps = (rectangle_left(one) < rectangle_right(two)) && (rectangle_right(one) > rectangle_left(two));
            bool y_overlaps = (rectangle_top(one) < rectangle_bottom(two)) && (rectangle_bottom(one) > rectangle_top(two));
            bool collision = x_overlaps && y_overlaps;

            if(collision)
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

    string test_collision(rectangle one, rectangle two) override {return "None";};
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
            this->position = position;

            animation_script water_script = animation_script_named("CellAnim");
            animation anim = create_animation(water_script, "Water");
            drawing_options opts = option_defaults();
            this->opts = opts;
            this->anim = anim;
            this->opts.anim = anim;
        }

    string test_collision(rectangle one, rectangle two) override
    {
        string collision = "None";
        double dx = (one.x + one.width/2) - (two.x + two.width/2);
        double dy = (one.y + one.height/2) - (two.y + two.height/2);
        double width = (one.width + two.width)/2;
        double height = (one.height + two.height)/2;
        double crossWidth = width * dy;
        double crossHeight = height * dx;

        if(abs(dx) <= width && abs(dy) <= height)
        {
            if(crossWidth>=crossHeight)
                collision = "Left";
            else
                collision = "Right";
        }

        return collision;
    };

    void draw_block() override
    {
        draw_bitmap("Water", position.x, position.y, opts);
        update_animation(this->anim);
        if(animation_ended(this->anim))
            restart_animation(this->anim);
    }
};

class ToxicBlock : public Block
{
    public:
        ToxicBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->cell = cell;
            this->opts.draw_cell = this->cell;
        }

    string test_collision(rectangle one, rectangle two) override {return "None";};
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

    string test_collision(rectangle one, rectangle two) override 
    {
        bool x_overlaps = (rectangle_left(one) < rectangle_right(two)) && (rectangle_right(one) > rectangle_left(two));
        bool y_overlaps = (rectangle_top(one) < rectangle_bottom(two)) && (rectangle_bottom(one) > rectangle_top(two));
        bool collision = x_overlaps && y_overlaps;
        
        if(collision)
            return "Collision";
        else
            return "None";
    };

    void draw_block() override
    {
        draw_bitmap("Door", position.x, position.y, opts);
        update_animation(this->anim);
        if(animation_ended(this->anim))
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