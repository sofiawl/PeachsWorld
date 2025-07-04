#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef struct {
    int disp_y_play;
    int disp_y_menu;  
    int current_y;
    bool select_color_play;
    bool select_color_menu;
} menu_t;

void init_menu(menu_t *menu, ALLEGRO_FONT **font, int disp_y);
void write_font(ALLEGRO_FONT* font, int x, int y, int flag, const char *text,
                int r, int g, int b, bool colorfull);
void update_menu_click(unsigned char *key, menu_t *menu, int *state);
void update_menu_mouse(menu_t *menu, ALLEGRO_EVENT *event);
void draw_menu(int disp_x, int disp_y, menu_t menu, ALLEGRO_FONT *font);
void draw_game_over(int disp_x, int disp_y, ALLEGRO_FONT *font, bool winner);
void destroy_menu_bitmaps(ALLEGRO_FONT *font);
void draw_dificulty(int disp_x, int disp_y, menu_t menu, ALLEGRO_FONT *font);

#endif // MENU_H
