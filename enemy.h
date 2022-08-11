#include "splashkit.h"
#include "behaviour.h"
#include <memory>

#pragma once

class Enemy
{
    protected:
        sprite enemy_sprite;
        point_2d position;
        rectangle hitbox;
        bool is_dead = false;
        std::shared_ptr<Behaviour> ai;

    public:
        Enemy(sprite enemy_sprite, point_2d position)
        {
            this->enemy_sprite = enemy_sprite;
            this->position = position;
            sprite_set_position(enemy_sprite, this->position);
            make_hitbox();
        };

        ~Enemy(){};

        virtual void update()
        {
            ai->update();
            draw_sprite(enemy_sprite);
            if(sprite_animation_has_ended(enemy_sprite))
                sprite_replay_animation(enemy_sprite);
            update_sprite(enemy_sprite);
            update_hitbox();
        };

        void make_hitbox()
        {
            rectangle hitbox;
            hitbox.x = this->position.x;
            hitbox.y = this->position.y + 10;
            hitbox.height = sprite_height(this->enemy_sprite) - 10;
            hitbox.width = sprite_width(this->enemy_sprite);
            this->hitbox = hitbox;
        };

        void update_hitbox()
        {
            point_2d current_position = sprite_position(this->enemy_sprite);
            this->hitbox.x = current_position.x;
            this->hitbox.y = current_position.y + 10;
        };

        rectangle get_enemy_hitbox()
        {
            return this->hitbox;
        };

};

class Roach : public Enemy
{
    public:
        Roach(sprite enemy_sprite, point_2d position) : Enemy (enemy_sprite, position)
        {
            std::shared_ptr<Behaviour> ai(new RoachBehaviour(enemy_sprite));
            this->ai = ai;
            point_2d pos = this->position;
            pos.y = pos.y + 32;
            sprite_set_position(enemy_sprite, pos);
            
            make_hitbox();
        };

        ~Roach(){};
};