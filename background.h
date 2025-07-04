#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro.h>

typedef struct {
    ALLEGRO_BITMAP *name;
    int frame_x;
    int frame_y;
    int frame_width;
    int frame_height;
    int draw_x;
    int draw_y;
    int draw_width;
    int draw_height;
} bitmap_t;

void init_scenary(ALLEGRO_BITMAP **sheet_background, bitmap_t *obs1, bitmap_t *bg, int disp_y, int disp_x);
void draw_scenery(bitmap_t obs, bitmap_t bg, int scroll_x);
void destroy_background_bitmaps(ALLEGRO_BITMAP *sheet_background, bitmap_t *obs1, bitmap_t *bg);

#endif // BACKGROUND_H
