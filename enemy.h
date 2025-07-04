#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "peach.h"  // Supondo que você tenha um bitmap_t definido aqui
#include "background.h"

typedef struct {
    ALLEGRO_BITMAP *name;    
    int vel_x, vel_y;
    bool active;
    int x;
    int y;
    int frame_x, frame_y;
    int frame_size;
    int draw_size;
    int start_x;
} projectil_t;

typedef struct {
    bitmap_t walk[2];
    bitmap_t shoot[3];
    projectil_t projectile;
    int frame;
    int width;
    int height;
    int world_x;
    int walk_from;
    int walk_to;
    int y;
    int vel_x;
    int damage_cooldown;
    int num_lifes;
    bool walking;         // true = andando, false = atirando
    int state_timer;      // tempo restante no estado atual
    bool facing_right;    // direção que está olhando
    bool hurt;
    bool dead;
    bool can_shoot;
    int hard;
    int shoot_cooldown;
} turtle_t;


void init_turtle(ALLEGRO_BITMAP **sheet_enemies, turtle_t *enemy);
void update_turtle(turtle_t *turtle);
void draw_turtle(turtle_t turtle, int scroll_x);
void destroy_turtle_resources(ALLEGRO_BITMAP **sheet_enemies, turtle_t *enemy);

#endif // ENEMY_H
