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
        bool is_solid;
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

        void make_hitbox()
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
                    if(crossWidth > (-crossHeight))
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
                    if(crossWidth - 100 > -(crossHeight))
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

class NonSolidBlock : public Block
{
    private:
        animation anim;

    public:
        NonSolidBlock(bitmap cell_sheet, point_2d position, int cell) : Block(cell_sheet, position)
        {
            this->is_solid = false;
            this->cell = cell;
            this->opts.draw_cell = this->cell;

            if(this->cell == 1 || this->cell == 2)
            {
                this->cell = 1;
                animation_script water_script = animation_script_named("CellAnim");
                animation anim2 = create_animation(water_script, "Water");
                drawing_options opts = option_defaults();
                this->opts = opts;
                this->anim = anim2;
                this->opts.anim = anim2;
            }
        };

        string test_collision(rectangle one, rectangle two) override {return "None";};

        void draw_block() override
        {
            if(this->cell == 1)
            {
                draw_bitmap("NonSolidBlocks", position.x, position.y, opts);
                update_animation(this->anim);
                if(animation_ended(this->anim))
                {
                    restart_animation(this->anim);
                }
            }
            else
            {
                draw_bitmap(image, position.x, position.y, opts);
            }
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