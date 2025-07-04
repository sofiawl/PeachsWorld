
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

void init_turtle(ALLEGRO_BITMAP **sheet_enemies, turtle_t *enemy)
{   
    *sheet_enemies = al_load_bitmap("asset/enemies.png");
    if (!*sheet_enemies) {printf("Erro ano inicializar sprite enemies"); return ;}

    enemy->walk[0].frame_height = 36;
    enemy->walk[0].frame_width = 36;
    enemy->walk[0].frame_x = 248;
    enemy->walk[0].frame_y = 0;
    enemy->walk[0].name = al_create_sub_bitmap(*sheet_enemies, 248, 0, 36, 36);
    if (!enemy->walk[0].name) {printf("Erro ano inicializar sprite enemies"); return ;}

    enemy->walk[1].frame_height = 36;
    enemy->walk[1].frame_width = 36;
    enemy->walk[1].frame_x = 286;
    enemy->walk[1].frame_y = 0;
    enemy->walk[1].name = al_create_sub_bitmap(*sheet_enemies, 286, 0, 36, 36);
    if (!enemy->walk[1].name) {printf("Erro ano inicializar sprite enemies"); return ;}


    enemy->shoot[0].frame_height = 36;
    enemy->shoot[0].frame_width = 36;
    enemy->shoot[0].frame_x = 162;
    enemy->shoot[0].frame_y = 0;
    enemy->shoot[0].name = al_create_sub_bitmap(*sheet_enemies, 162, 0, 36, 36);
    if (!enemy->shoot[0].name) {printf("Erro ano inicializar sprite enemies"); return ;}


    enemy->shoot[1].frame_height = 36;
    enemy->shoot[1].frame_width = 36;
    enemy->shoot[1].frame_x = 162;
    enemy->shoot[1].frame_y = 36;
    enemy->shoot[1].name = al_create_sub_bitmap(*sheet_enemies, 162, 36, 36, 36);
    if (!enemy->shoot[1].name) {printf("Erro ano inicializar sprite enemies"); return ;}


    enemy->shoot[2].frame_height = 36;
    enemy->shoot[2].frame_width = 36;
    enemy->shoot[2].frame_x = 162;
    enemy->shoot[2].frame_y = 72;
    enemy->shoot[2].name = al_create_sub_bitmap(*sheet_enemies, 162, 72, 36, 36);
    if (!enemy->shoot[2].name) {printf("Erro ano inicializar sprite enemies"); return ;}

    enemy->projectile.name = *sheet_enemies;
    enemy->projectile.frame_x = 324;
    enemy->projectile.frame_y = 478;
    enemy->projectile.frame_size = 32;
    enemy->projectile.frame_size = 32;
    enemy->projectile.draw_size = 100;

    enemy->width = 150;
    enemy->height = 150;
    enemy->num_lifes = 5;
    enemy->walking = true;
    enemy->vel_x = 10;
    enemy->state_timer = 180; // comece andando por 3 segundos
    enemy->facing_right = true;
    enemy->frame = 0;
    enemy->world_x = 0;
    enemy->dead = false;
    enemy->damage_cooldown = 0;
    enemy->shoot_cooldown = 0;
    
    return;
}

void update_turtle(turtle_t *turtle) {
    turtle->state_timer--;

    if (turtle->walking) 
    {   
        if (turtle->damage_cooldown > 0)
            turtle->damage_cooldown--;

	
		if (turtle->hurt && turtle->damage_cooldown == 0)
		{  
			if (turtle->hurt)
            {
                printf("lifes: %d\n", turtle->num_lifes);
            }
            if (turtle->num_lifes > 0)
            {
                turtle->num_lifes = turtle->num_lifes - 1;	
            }
                if (turtle->num_lifes == 0)
                {    
                    turtle->dead = true;
                }

            turtle->hurt = false;
            turtle->damage_cooldown = 20; // 15 frames de invulnerabilidade
		}

        if (turtle->hard == 2)
        {
            if (turtle->facing_right)
            {
                turtle->world_x += turtle->vel_x;
            
                if (turtle->can_shoot && !turtle->projectile.active) 
                {
                    turtle->projectile.active = true;
                    // ATIRA PARA A ESQUERDA (oposto de facing_right)
                    turtle->projectile.vel_x = -20;
                    turtle->projectile.x = turtle->world_x - turtle->width;
                    turtle->projectile.y = turtle->y + 50;
                }
            }
            else 
            {
                turtle->world_x += -turtle->vel_x;
            
                if (turtle->can_shoot && !turtle->projectile.active) 
                {
                    turtle->projectile.active = true;
                    // ATIRA PARA A DIREITA (oposto de !facing_right)
                    turtle->projectile.vel_x = 20;
                    turtle->projectile.x = turtle->world_x + turtle->width;
                    turtle->projectile.y = turtle->y - 50;
                }
            }
        }
        else if (turtle->hard == 1)
        {
            if (turtle->facing_right)
            {
                turtle->world_x += turtle->vel_x;
            }
            else 
            {
                turtle->world_x += -turtle->vel_x;
                if (turtle->can_shoot && !turtle->projectile.active) 
                {
                    turtle->projectile.active = true;
                    turtle->projectile.vel_x = 20;
                    turtle->projectile.x = turtle->world_x + turtle->width;  
                    turtle->projectile.y = turtle->y - 50; 
                }
            }
        }

        // alterna o frame de andar
        turtle->frame = (turtle->frame + 1) % 2;

    
        // muda direção ao atingir limites ()
        if (turtle->world_x < turtle->walk_from) {
            turtle->facing_right = true;
        } else if (turtle->world_x > turtle->walk_to) {
            turtle->facing_right = false;
        }

        if (turtle->state_timer <= 0) {
            turtle->walking = false;
            turtle->state_timer = 90; // 1.5s 
            turtle->frame = 0;
        }

    } else if (!turtle->walking) {
        turtle->frame = (turtle->frame + 1) % 3;

        if (turtle->facing_right)
            turtle->world_x += turtle->vel_x;
        else 
            turtle->world_x += -turtle->vel_x;

        if (turtle->world_x < turtle->walk_from) {
            turtle->facing_right = true;
        } else if (turtle->world_x > turtle->walk_to) {
            turtle->facing_right = false;
        }
    
        if (turtle->state_timer <= 0) {
            turtle->walking = true;
            turtle->state_timer = 180; // ~3s andando
            turtle->frame = 0;
        }
    }
    if (turtle->projectile.active) 
        turtle->projectile.x += turtle->projectile.vel_x;

    if (abs(turtle->projectile.x - turtle->world_x) >= 500)
        turtle->projectile.active = false;
  
    return;
}

void draw_turtle(turtle_t turtle, int scroll_x) {
    int screen_x = turtle.world_x - scroll_x;
    bitmap_t sprite;

    if (turtle.walking)
        sprite = turtle.walk[turtle.frame];
    else
        sprite = turtle.shoot[turtle.frame];

    if (turtle.facing_right)
    {
        al_draw_scaled_bitmap(
            sprite.name,
            0, 0,
            sprite.frame_width, sprite.frame_height,
            screen_x, turtle.y,
            turtle.width, turtle.height,
            0);
    } else
        al_draw_scaled_bitmap(
            sprite.name,
            0, 0,
            sprite.frame_width, sprite.frame_height,
            screen_x + turtle.width, turtle.y,
            -turtle.width, turtle.height,
            0);

    if (turtle.projectile.active) 
    {
        int proj_screen_x = turtle.projectile.x - scroll_x;
        int proj_screen_y = turtle.projectile.y;

        al_draw_scaled_bitmap(turtle.projectile.name, 
                            turtle.projectile.frame_x, turtle.projectile.frame_y,
                            turtle.projectile.frame_size, turtle.projectile.frame_size,
                            proj_screen_x, proj_screen_y,
                            turtle.projectile.draw_size, turtle.projectile.draw_size,
                            0);
    }
    
}

void destroy_turtle_resources(ALLEGRO_BITMAP **sheet_enemies, turtle_t *enemy)
{
    if (*sheet_enemies) 
    {
        al_destroy_bitmap(*sheet_enemies);
        *sheet_enemies = NULL;

        for (int i = 0; i < 2; i++) 
        {
            enemy->walk[i].name = NULL;
        }

        for (int i = 0; i < 3; i++) 
        {
            enemy->shoot[i].name = NULL;
        }

        enemy->projectile.name = NULL;
    }
}
