#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro.h>

typedef struct {
    ALLEGRO_BITMAP *name;
    bitmap_t alive;
    projectil_t fire[5];
    int size;
    int x;
    int y;
    int vel_x;
    int vel_y;
    int num_lifes;
    int state_timer;
    int shoot_timer;
    bool hurt;
    bool dead;
} boss_t;

void init_boss(bitmap_t *bg, boss_t *boss);
void update_boss(boss_t *boss, int disp_x, int disp_y);
void draw_boss(boss_t boss);
void draw_backgroudn_boss(bitmap_t bg, int disp_x, int disp_y);
void destroy_boss_resources(boss_t *boss, bitmap_t *bg_boss);

#endif // BOSS_H
