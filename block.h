#include "splashkit.h"

class Block
{
    protected:
        bitmap image;
        point_2d position;
        drawing_options opts;
        double top;
        double collision;
        string type;
        
    public:
        /**
         * @brief Enumeration of block style bitmaps
         * 
         */
        enum BlockType
        {
            MARIO_BLOCKS
        };

        Block(bitmap image, point_2d position)
        {
            this->image = image;
            this->position = position;
            this->opts = option_defaults();

            this->top = position.y - 30;
            this->collision = position.y - 20;
        };

        /**
         * @brief Second Overloaded Constructor
         * 
         * @param BlockType 
         * @param position 
         */
        Block(BlockType type, point_2d position)
        {
            this->image = bitmap_named(block_type(type));
            this->position = position;
            this->opts = option_defaults();

            this->top = position.y - 30;
            this->collision = position.y - 20;
        };

        ~Block(){};

        virtual void draw_block()
        {
            draw_bitmap(image, position.x, position.y, opts);
        }

        bool test_top_collision(sprite player)
        {
            bool collision = false;
            collision = sprite_bitmap_collision(player, this->image, this->position.x, this->collision);
            return collision;
        }

        float get_top()
        {
            return this->top;
        }

        point_2d get_pos()
        {
            return this->position;
        }

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
                default:
                    return "MarioBlocks";
                    break;
            }
        }
};

class FloorBlock : public Block
{
    public:
        FloorBlock(bitmap image, point_2d position) : Block(image, position)
        {
            this->type = "Floor Block";
            this->opts.draw_cell = 1;
        }

        FloorBlock(BlockType type, point_2d position) : Block(type, position)
        {
            this->opts.draw_cell = 1;
        }
};

class BrickBlock : public Block
{
    public:
        BrickBlock(bitmap image, point_2d position) : Block(image, position)
        {
            this->type = "Brick Block";
            this->opts.draw_cell = 0;
        }

        BrickBlock(BlockType type, point_2d position) : Block(type, position)
        {
            this->opts.draw_cell = 0;
        }
};

class QuestionBlock : public Block
{
    public:
        QuestionBlock(bitmap image, point_2d position) : Block(image, position)
        {
            this->type = "Question Block";
            this->opts.draw_cell = 2;
        }

        QuestionBlock(BlockType type, point_2d position) : Block(type, position)
        {
            this->opts.draw_cell = 2;
        }        
};