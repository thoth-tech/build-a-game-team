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
        Block(bitmap image, point_2d position)
        {
            this->image = image;
            this->position = position;
            this->opts = option_defaults();

            this->top = position.y - 30;
            this->collision = position.y - 20;
        };
        ~Block(){};

        void draw_block()
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
};

class FloorBlock : public Block
{
    public:
        FloorBlock(bitmap image, point_2d position) : Block(image, position)
        {
            this->type = "Floor Block";
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
};

class QuestionBlock : public Block
{
    public:
        QuestionBlock(bitmap image, point_2d position) : Block(image, position)
        {
            this->type = "Question Block";
            this->opts.draw_cell = 2;
        }
};