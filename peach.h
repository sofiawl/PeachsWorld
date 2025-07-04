#ifndef PEACH_H
#define PEACH_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "background.h"

// Constantes
#define GRAVITY 4.0

typedef struct{
    int vel;
    bool active;
    int state;
    float angle;
    bool collide;
		int init_x;
    ALLEGRO_BITMAP *name;
    int frame_x;       // coordenada x do recorte da imagem
    int frame_y;       // coordenada y do recorte da imagem
    int frame_width;   // largura a ser recortada
    int frame_height;  // altura a ser recortada
    int draw_x;        // coordenada x a ser desenhada
    int draw_y;        // cooerdana y a ser desenhada
    int draw_width;    // largura a ser desenhada
    int draw_height;   // altura a ser desenhada
		bool right; 
} umbrella_t;

typedef struct{
    bitmap_t walk[4];
    bitmap_t down;
    bitmap_t shoot[2];
    umbrella_t umbrella;
    int frame;
    int size;
    int x;
    int y;
    int vel_x;
    float vel_y;
    int world_x;
    float jump_impulse; 
    int jumps_allowed;
    int max_jumps;
    bool is_down;
    bool right;
    int num_lives;
    int max_lives;
		bool collide;
    int collision_cooldown;
    int transition_heart_timer;
    int repel_force;
    int repel_timer;
} peach_t;

void init_peach(ALLEGRO_BITMAP **sheet_peach, peach_t *peach, int floor);
void reinit_peach(peach_t *peach, int floor);
int update_peach(unsigned char *key, peach_t *peach, int *scroll_x, int disp_x, int floor);
int update_peach_boss(unsigned char *key, peach_t *peach, int *scroll_x, int floor);
void draw_peach(peach_t peach, int scroll_x);
void update_umbrella(unsigned char *key, peach_t *peach, int disp_x);
void draw_umbrella(peach_t peach);
bool wall_colision(peach_t peach, int direction);
void destroy_peach_bitmaps(peach_t *peach, ALLEGRO_BITMAP *sheet_peach);
#endif // PEACH_H
