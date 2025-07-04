#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "background.h"

void init_scenary(ALLEGRO_BITMAP **sheet_background, bitmap_t *obs1, bitmap_t *bg, int disp_y, int disp_x)
{
    *sheet_background = al_load_bitmap("asset/SNES - Super Mario World - Backgrounds Animated.png");
    if (!*sheet_background) { printf("Erro ao inicializar sprite background"); return ;}
    
    obs1->name = al_load_bitmap("asset/SNES - Super Mario World - Donut Ghost House 13.png");
    if (!obs1->name) { printf("Erro ao inicializar sprite obstacles1"); return ;}
    obs1->frame_height = al_get_bitmap_height(obs1->name);
    obs1->frame_width = al_get_bitmap_width(obs1->name);
    obs1->frame_x = 0;
    obs1->frame_y = 0;
    obs1->draw_height = disp_y-60; 
    obs1->draw_width = disp_x*4;
    obs1->draw_x =  0;
    obs1->draw_y = 0;

    bg->frame_x = 44;
    bg->frame_y = 3542; // escolhe o background
    bg->frame_height = 396;
    bg->frame_width = 440;
    bg->draw_height = disp_y;
    bg->draw_width = disp_x;
    bg->draw_x = 0;
    bg->draw_y = 0;
    bg->name = al_create_sub_bitmap(*sheet_background, bg->frame_x, bg->frame_y, bg->frame_width, bg->frame_height);
    if (!bg->name) { printf("Erro ao inicializar sub background"); return ;}

    return;
}


void draw_scenery(bitmap_t obs, bitmap_t bg, int scroll_x)
{
    int obs_screen_x = obs.draw_x - scroll_x;

    al_draw_scaled_bitmap(bg.name,
        0, 0, 
        bg.frame_width, bg.frame_height, 
        0, 0, 
        bg.draw_width, bg.draw_height, 
        0);

    al_draw_scaled_bitmap(
        obs.name, 
        obs.frame_x, obs.frame_y, 
        obs.frame_width, obs.frame_height, 
        obs_screen_x, obs.draw_y, 
        obs.draw_width, obs.draw_height, 
        0
    );

    return;
}

void destroy_background_bitmaps(ALLEGRO_BITMAP *sheet_background, bitmap_t *obs1, bitmap_t *bg)
{
    if (bg->name) {
        al_destroy_bitmap(bg->name);
        bg->name = NULL;
    }

    if (obs1->name) {
        al_destroy_bitmap(obs1->name);
        obs1->name = NULL;
    }

    if (sheet_background) {
        al_destroy_bitmap(sheet_background);
        sheet_background = NULL;
    }

    return;
}

