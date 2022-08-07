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
        
    public:
        /**
         * @brief Enumeration of block style bitmaps
         * 
         */
        enum BlockType
        {
            MARIO_BLOCKS,
            SEWER_BLOCKS
        };

        Block(BlockType type, point_2d position, bool is_solid)
        {
            this->image = bitmap_named(block_type(type));
            this->is_solid = is_solid;
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

        bool test_top_collision(sprite player)
        {
            bool collision = false;
            collision = sprite_bitmap_collision(player, this->image, this->position.x, this->top);
            return collision;
        };

        string test_collision(rectangle one, rectangle two)
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
                        draw_text("Bottom",COLOR_WHITE,0,480);
                    }
                    else
                    {
                        collision = "Left";
                        draw_text("Left",COLOR_WHITE,0,480);
                    }
                }
                else
                {
                    //Gave a bias to top collision to avoid right edge stopping player during movement
                    if(crossWidth - 100 > -(crossHeight))
                    {
                        collision = "Right";
                        draw_text("Right",COLOR_WHITE,0,480);
                    }
                    else
                    {
                        collision = "Top";
                        draw_text("Top",COLOR_WHITE,0,480);
                    }
                }
            }

            return collision;
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

        rectangle get_block_hitbox()
        {
            return this->hitbox;
        };

        bool is_block_solid()
        {
            return this->is_solid;
        };

        /**
         * @brief Add sprite sheets to this switch
         * 
         * Sprite sheets can be passed to the constructor by enum type
         * 
         * @param selection 
         * @return string 
         */
        string block_type(BlockType selection)
        {
            switch(selection)
            {
                case MARIO_BLOCKS:
                    return "MarioBlocks";
                    break;
                case SEWER_BLOCKS:
                    return "SewerBlocks";
                    break;
                default:
                    return "MarioBlocks";
                    break;
            }
        };
};

class FloorBlock : public Block
{
    public:
        FloorBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 1;
        }
};

class BrickBlock : public Block
{
    public:
        BrickBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 0;
        }
};

class QuestionBlock : public Block
{
    public:
        QuestionBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 2;
        }        
};

class DarkSewerBlock : public Block
{
    public:
        DarkSewerBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 0;
        }        
};

class LightSewerBlock : public Block
{
    public:
        LightSewerBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 1;
        }        
};

class LadderBlock : public Block
{
    public:
        LadderBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 2;
        }
};

class WaterBlock : public Block
{
    public:
        WaterBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 3;
        }
};

class ToxicBlock : public Block
{
    public:
        ToxicBlock(BlockType type, point_2d position, bool is_solid) : Block(type, position, is_solid)
        {
            this->opts.draw_cell = 5;
        }
};