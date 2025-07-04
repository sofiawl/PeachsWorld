
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "menu.h"

void init_menu(menu_t *menu, ALLEGRO_FONT **font, int disp_y)
{
    *font = al_load_ttf_font("asset/SuperMario256.ttf", 72, 0);
    if (!*font) { printf("Erro ao inicializar font\n"); return ; } 

    menu->disp_y_play = disp_y/2;
    menu->disp_y_menu = disp_y/2 + 80;  
    menu->current_y = menu->disp_y_play;
    menu->select_color_play = false;
    menu->select_color_menu = false;


    return;
}


void write_font(ALLEGRO_FONT* font, int x, int y, int flag, const char *text,
                int r, int g, int b, bool colorfull)
{
    if (!colorfull)
    {
        al_draw_text(font, al_map_rgb(r, g, b), x, y, flag, text);
        return;
    }

    ALLEGRO_COLOR colors_title[] = 
    {
        al_map_rgb(255, 206, 41),   // amarelo 
        al_map_rgb(0, 222, 255),    // azul 
        al_map_rgb(0, 206, 0),      // verde 
        al_map_rgb(255, 0, 0),      // adicionar vermelho manualmente
    };

    int num_colors = sizeof(colors_title) / sizeof(ALLEGRO_COLOR);

    int total_width = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        char letter[2] = {text[i], '\0'};
        total_width += al_get_text_width(font, letter);
    }
    int current_x = x - (total_width / 2);


    for (int i = 0; text[i] != '\0'; i++)
    {   
        char letter[2] = {text[i], '\0'};
        ALLEGRO_COLOR color = colors_title[rand() % num_colors];
        al_draw_text(font, color, current_x, y, flag, letter);
        current_x += al_get_text_width(font, letter);
    }

    return;
}

void update_menu_click(unsigned char *key, menu_t *menu, int *state)
{
    if ((menu->current_y == menu->disp_y_menu) && key[ALLEGRO_KEY_UP])
    { 
        menu->select_color_menu = false;
        menu->current_y -= 80;
        menu->select_color_play = true;

    }
    if ((menu->current_y == menu->disp_y_play))
    {
        if (key[ALLEGRO_KEY_ENTER])
            *state = 2; // game_play
        else if (key[ALLEGRO_KEY_DOWN])
        {
            menu->select_color_play = false;
            menu->current_y += 80;
            menu->select_color_menu = true;
        }
    }   

    return;
    
}

void update_menu_mouse(menu_t *menu, ALLEGRO_EVENT *event)
{
    if (event->mouse.y >= menu->disp_y_play && event->mouse.y < menu->disp_y_play + 50)
        menu->select_color_play = true;                            
    else
        menu->select_color_play = false;
    if (event->mouse.y >= menu->disp_y_menu && event->mouse.y < menu->disp_y_menu + 50)
        menu->select_color_menu = true;
    else 
        menu->select_color_menu = false;
                    
    return;
}

void draw_menu(int disp_x, int disp_y, menu_t menu, ALLEGRO_FONT *font)
{
    al_clear_to_color(al_map_rgb(156, 231, 231));
    write_font(font, disp_x/2, disp_y/2 - 200, ALLEGRO_ALIGN_CENTER, "SUPER PEACHE'S WORLD", 0, 0, 0, true);
    write_font(font, disp_x/2, menu.disp_y_play, ALLEGRO_ALIGN_CENTER, "play", 0, 0, 0, menu.select_color_play);
    write_font(font, disp_x/2, menu.disp_y_menu, ALLEGRO_ALIGN_CENTER, "menu", 0, 0, 0, menu.select_color_menu);
    al_flip_display();
    
}

void draw_dificulty(int disp_x, int disp_y, menu_t menu, ALLEGRO_FONT *font)
{
    al_clear_to_color(al_map_rgb(156, 231, 231));
    write_font(font, disp_x/2, disp_y/2 - 200, ALLEGRO_ALIGN_CENTER, "LEVEL:", 0, 0, 0, true);
    write_font(font, disp_x/2, menu.disp_y_play, ALLEGRO_ALIGN_CENTER, "easy", 0, 0, 0, menu.select_color_play);
    write_font(font, disp_x/2, menu.disp_y_menu, ALLEGRO_ALIGN_CENTER, "hard", 0, 0, 0, menu.select_color_menu);
    al_flip_display();
    
}

void destroy_menu_bitmaps(ALLEGRO_FONT *font)
{
    if (font)
        al_destroy_font(font);
}


void draw_game_over(int disp_x, int disp_y, ALLEGRO_FONT *font, bool winner)
{ 
    al_clear_to_color(al_map_rgb(156, 231, 231));
    write_font(font, disp_x / 2, disp_y / 2 - 20, ALLEGRO_ALIGN_CENTER, 
           winner ? "WINNER" : "LOSER", 
           0, 0, 0, true);
}
