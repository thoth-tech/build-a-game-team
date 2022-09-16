#include "splashkit.h"
#include <memory>
#include <vector>

#pragma once

class Letter
{
    private:
        string val;
        point_2d pos;
        bool selected = false;

    public:
        Letter(string val, point_2d pos)
        {
            this->val = val;
            this->pos = pos;
        };
        ~Letter(){};

        void draw()
        {
            if(selected)
            {
                draw_text(val, COLOR_RED, pos.x, pos.y);
            }
            else
            {
                draw_text(val, COLOR_WHITE, pos.x, pos.y);
            }
        };

        string get_value()
        {
            return this->val;
        };

        void set_value(string val)
        {
            this->val = val;
        };

        void set_selected(bool new_value)
        {
            this->selected = new_value;
        };

        bool is_selected()
        {
            return this->selected;
        };
};

class Password
{
    private:
        vector<std::shared_ptr<Letter>> keyboard;
        vector<std::shared_ptr<Letter>> underscore;
        int selection = 0;
        int letter = 0;

    public:
        Password()
        {
            process_alphabet();
            process_underscore();
        };
        ~Password(){};

        void draw_element(vector<std::shared_ptr<Letter>> element)
        {
            for(int i = 0; i < element.size(); i++)
            {
                element[i]->draw();
            }
        };

        void process_alphabet()
        {
            string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ<~`";
            int row = 0;
            int col = 0;

            for(int i = 0; i < alphabet.size(); i++)
                {
                    if(i == 0){}
                    else if(i % 6 == 0)
                    {
                        row += 1;
                        col = 0;
                    }

                    char c = alphabet.at(i);
                    string str(1, c);

                    point_2d pos = screen_center();
                    pos.x += col * 30;
                    pos.y += row * 15;

                    if(str == "~")
                    {
                        str = "ENTER";
                        pos.x += 20;
                        pos.y += 20;
                    }
                    if(str == "<")
                    {
                        str = "DEL";
                        pos.x += 20;
                        pos.y += 20;
                    }
                    if(str == "`")
                    {
                        str = "EXIT";
                        pos.x += 40;
                        pos.y += 20;
                    }

                    std::shared_ptr<Letter> alpha(new Letter(str, pos));
                    keyboard.push_back(alpha);

                    col += 1;
                }
        };

        void process_underscore()
        {
            string letters = "_ _ _ _ _";

            for(int i = 0; i < letters.size(); i++)
            {
                char c = letters.at(i);
                string str(1, c);
                
                point_2d pos = screen_center();
                pos.x += (i * 10);
                pos.y += 100;

                std::shared_ptr<Letter> alpha(new Letter(str, pos));
                underscore.push_back(alpha);
            }
        };

        string input()
        {
            string password = "";

            if(key_typed(D_KEY))
            {
                selection += 1;

                if(selection > keyboard.size() - 1)
                    selection = 0;
            }

            if(key_typed(A_KEY))
            {
                selection -= 1;

                if(selection < 0)
                    selection = keyboard.size() - 1;
            }

            if(key_typed(S_KEY))
            {
                selection += 6;

                if(selection > keyboard.size() - 1)
                {
                    if(selection < 30)
                        selection = 27;
                    else
                        selection = 0 + (selection - 30);
                }
            }

            if(key_typed(W_KEY))
            {
                selection -= 6;

                if(selection < 0)
                {
                    if(selection < -2)
                        selection = selection + 30;
                    else
                        selection = 27;
                }
            }

            if(key_typed(F_KEY) || key_typed(RETURN_KEY))
            {
                string select = "";
                for(int i = 0; i < keyboard.size(); i++)
                {
                    if(keyboard[i]->is_selected())
                    {
                        select = keyboard[i]->get_value();
                    }
                }
                if(select == "DEL")
                {
                    if(letter > 0)
                        letter -= 2;
                    underscore[letter]->set_value("_");
                }
                else if(select == "ENTER")
                {
                    string temp = "";
                    for(int i = 0; i < underscore.size(); i += 2)
                        temp.append(underscore[i]->get_value());
                    
                    password = temp;
                }
                else if (select == "EXIT")
                {
                    password = "EXITEXITEXIT";
                }
                else if(letter < 10)
                {
                    underscore[letter]->set_value(select);
                    letter += 2;
                }
            }

            return password;
        };

        string update()
        {
            clear_screen(COLOR_BLACK);
            for(int i = 0; i < keyboard.size(); i++)
            {
                if(selection == i)
                    keyboard[i]->set_selected(true);
                else
                    keyboard[i]->set_selected(false);
            }

            string password = input();
            draw_element(keyboard);
            draw_element(underscore);

            return password;
        }
};