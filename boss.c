#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "peach.h"
#include "menu.h"
#include "background.h"
#include "enemy.h"
#include "boss.h"

void init_boss(bitmap_t *bg, boss_t *boss)
{
    bg->name = al_load_bitmap("asset/SNES - Super Mario World - Donut Ghost House Alternative Exit.png");
    if (!bg->name) {printf("Erro ano inicializar sprite background"); return ;}
    bg->frame_height = 432;
    bg->frame_width = 513;

    boss->name = al_load_bitmap("asset/BOSS.png");
    if (!boss->name) {printf("Erro ano inicializar sprite boss"); return ;}

    boss->x = 200;
    boss->y = 100;
    boss->size = 350;
    boss->vel_x = 10;
    boss->vel_y = 10;
    boss->num_lifes = 30;
    boss->dead = false;
    boss->shoot_timer = 60;


    boss->alive.name = boss->name;
    boss->alive.frame_x = 72;
    boss->alive.frame_y = 6;
    boss->alive.frame_width = 66;
    boss->alive.frame_height = 103;

    for (int i = 0; i < 5; i++) {
        boss->fire[i].name = boss->name;
        boss->fire[i].active = false;
        boss->fire[i].frame_x = (i % 2 == 0) ? 6 : 24;  // alterna sprites se quiser
        boss->fire[i].frame_y = 195;
        boss->fire[i].frame_size = 21;
        boss->fire[i].vel_x = 100;
        boss->fire[i].vel_y = 10 + rand() % 3; // entre 10 e 12
        boss->fire[i].draw_size = 100;
        boss->fire[i].start_x = 0;
    }

    return;
}

void update_boss(boss_t *boss, int disp_x, int disp_y) {

    if (boss->hurt)
    {
        if (boss->num_lifes > 0)
            boss->num_lifes--;
        if (boss->num_lifes == 0)
            boss->dead = true;

        boss->hurt = false;
    } 

    boss->x += boss->vel_x;
    boss->y += boss->vel_y;

    // Rebater nas bordas da tela
    if (boss->x < 0 || boss->x + boss->size > disp_x)
        boss->vel_x = -boss->vel_x;
    if (boss->y < 0 || boss->y + boss->size > disp_y / 2)  
        boss->vel_y = -boss->vel_y;

    boss->shoot_timer--;
    if (boss->shoot_timer <= 0) {
        int shots_fired = 0;
        for (int i = 0; i < 5; i++) {
            if (!boss->fire[i].active) {
                boss->fire[i].active = true;
                boss->fire[i].x = boss->x + boss->size / 2 + (i - 2) * 30; // espalha tiros
                boss->fire[i].y = boss->y + boss->size;
                boss->fire[i].vel_x = 0;
                boss->fire[i].vel_y = 6 + i % 2; 
                shots_fired++;
                if (shots_fired >= 3) break;  
            }
        }
        boss->shoot_timer = 60; 
    }

    // Atualiza projéteis
    for (int i = 0; i < 5; i++) {
        if (boss->fire[i].active) {
            boss->fire[i].y += boss->fire[i].vel_y;

            // Desativa se sair da tela
            if (boss->fire[i].y > disp_y) {
                boss->fire[i].active = false;
            }
        }
    }

    return;
}

void draw_boss(boss_t boss) {
    // Se estiver machucado, desenha com uma cor vermelha
    if (boss.hurt) {
        al_draw_tinted_scaled_bitmap(
            boss.name,
            al_map_rgba(255, 0, 0, 255), // Tinta vermelha
            boss.alive.frame_x, boss.alive.frame_y,
            boss.alive.frame_width, boss.alive.frame_height,
            boss.x, boss.y,
            boss.size, boss.size,
            0);
    } else {
        al_draw_scaled_bitmap(
            boss.name,
            boss.alive.frame_x, boss.alive.frame_y,
            boss.alive.frame_width, boss.alive.frame_height,
            boss.x, boss.y,
            boss.size, boss.size,
            0);
    }

    for (int i = 0; i < 5; i++) {
        if (boss.fire[i].active) {
            al_draw_scaled_bitmap(
                boss.fire[i].name,
                boss.fire[i].frame_x, boss.fire[i].frame_y,
                boss.fire[i].frame_size, boss.fire[i].frame_size,
                boss.fire[i].x, boss.fire[i].y,
                boss.fire[i].draw_size, boss.fire[i].draw_size,
                0);
        }
    }
}

void draw_backgroudn_boss(bitmap_t bg, int disp_x, int disp_y)
{
    al_clear_to_color(al_map_rgb(107, 140, 255));
    al_draw_scaled_bitmap(
            bg.name,
            0, 0,
            bg.frame_width,
            bg.frame_height,
            0, 0,
            disp_x,
            disp_y,
            0
        );
    return;
}

void destroy_boss_resources(boss_t *boss, bitmap_t *bg_boss) 
{
    if (boss->name) 
    {
        al_destroy_bitmap(boss->name);
        boss->name = NULL;
        boss->alive.name = NULL;
        for (int i = 0; i < 5; i++) 
        {
            boss->fire[i].name = NULL;
        }
    }

    if (bg_boss->name) 
    {
        al_destroy_bitmap(bg_boss->name);
        bg_boss->name = NULL;
    }
}
