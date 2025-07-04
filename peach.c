#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "peach.h"
#include "background.h"


bool wall_colision(peach_t peach, int direction)
{
    int next_world_x = peach.world_x + direction * peach.vel_x;

    if (next_world_x < 0)
        return true;

    return false;
}

void init_peach(ALLEGRO_BITMAP **sheet_peach, peach_t *peach, int floor)
{
    *sheet_peach = al_load_bitmap("asset/Custom Edited - Mario Customs - Peach Super Mario Bros 3 GBA-Style.png");
    if (!*sheet_peach) { printf("Erro ao inicializar sprite peach"); return ;}

    peach->walk[0].frame_height = 32;
    peach->walk[0].frame_width = 32;
    peach->walk[0].frame_x = 235;
    peach->walk[0].frame_y = 58;
    peach->walk[0].name = al_create_sub_bitmap(*sheet_peach, 
                        peach->walk[0].frame_x, peach->walk[0].frame_y, 
                        peach->walk[0].frame_width, peach->walk[0].frame_height);
    if (!peach->walk[0].name) { printf("Erro ao inicializar sub peach"); return ;}
    peach->walk[1].frame_height = 32;
    peach->walk[1].frame_width = 32;
    peach->walk[1].frame_x = 263;
    peach->walk[1].frame_y = 58;
    peach->walk[1].name = al_create_sub_bitmap(*sheet_peach, 
                        peach->walk[1].frame_x, peach->walk[1].frame_y, 
                        peach->walk[1].frame_width, peach->walk[1].frame_height);
    if (!peach->walk[1].name) { printf("Erro ao inicializar sub peach"); return ;}
    peach->walk[2].frame_height = 32;
    peach->walk[2].frame_width = 32;
    peach->walk[2].frame_x = 294;
    peach->walk[2].frame_y = 58;
    peach->walk[2].name = al_create_sub_bitmap(*sheet_peach, 
                        peach->walk[2].frame_x, peach->walk[2].frame_y, 
                        peach->walk[2].frame_width, peach->walk[2].frame_height);
    if (!peach->walk[2].name) { printf("Erro ao inicializar sub peach"); return ;}
    peach->walk[3].frame_height = 32;
    peach->walk[3].frame_width = 32;
    peach->walk[3].frame_x = 321;
    peach->walk[3].frame_y = 58;
    peach->walk[3].name = al_create_sub_bitmap(*sheet_peach, 
                        peach->walk[3].frame_x, peach->walk[3].frame_y, 
                        peach->walk[3].frame_width, peach->walk[3].frame_height);
    if (!peach->walk[3].name) { printf("Erro ao inicializar sub peach"); return ;}
    peach->down.frame_height = 32;
    peach->down.frame_width = 32;
    peach->down.frame_x = 257;
    peach->down.frame_y = 95;
    peach->down.draw_height = 25;
    peach->down.draw_width = 32;
    peach->down.draw_x = 0;
    peach->down.draw_y = 0;
    peach->down.name = al_create_sub_bitmap(*sheet_peach, 
                        peach->down.frame_x, peach->down.frame_y, 
                        peach->down.frame_width, peach->down.frame_height);
    if (!peach->down.name) { printf("Erro ao inicializar sub peach"); return ;}
    

    peach->umbrella.collide = false;
    peach->umbrella.state = 0;
    peach->umbrella.angle = 0;
    peach->umbrella.active = 0;
    peach->umbrella.vel = 0;
    peach->umbrella.frame_height = 16;
    peach->umbrella.frame_width = 24;
    peach->umbrella.frame_x = 119;
    peach->umbrella.frame_y = 218;
    peach->umbrella.draw_height = 60;
    peach->umbrella.draw_width = 70;
    peach->umbrella.draw_x = 0;
    peach->umbrella.draw_y = 0;
    peach->umbrella.name = al_create_sub_bitmap(*sheet_peach, 
                        peach->umbrella.frame_x, peach->umbrella.frame_y, 
                        peach->umbrella.frame_width, peach->umbrella.frame_height);
    if (!peach->umbrella.name) { printf("Erro ao inicializar sub peach"); return ;}
		peach->umbrella.init_x = 0;   
 
    peach->x = 0;
    peach->world_x = 0;
    peach->y = floor;
    peach->size = 170;
    peach->frame = 0;
    peach->vel_x = 15;
    peach->vel_y = 0;
    peach->jump_impulse = 50;
    peach->jumps_allowed = 2;
    peach->max_jumps = 2;
    peach->right = true;       
    peach->is_down = 0;
    peach->max_lives = 5;
	peach->num_lives = 5;
    peach->collide = 0;
    peach->collision_cooldown = 0;
    peach->transition_heart_timer = 0;
    peach->repel_force = 0;
    peach->repel_timer = 0;

    return;
}

void reinit_peach(peach_t *peach, int floor)
{
    peach->umbrella.collide = false;
    peach->umbrella.state = 0;
    peach->umbrella.angle = 0;
    peach->umbrella.active = 0;
    peach->umbrella.vel = 0;
    peach->umbrella.frame_height = 16;
    peach->umbrella.frame_width = 24;
    peach->umbrella.frame_x = 119;
    peach->umbrella.frame_y = 218;
    peach->umbrella.draw_height = 60;
    peach->umbrella.draw_width = 70;
    peach->umbrella.draw_x = 0;
    peach->umbrella.draw_y = 0;
		peach->umbrella.init_x = 0;
		
    peach->x = 0;
    peach->world_x = 0;
    peach->y = floor;
    peach->size = 170;
    peach->frame = 0;
    peach->vel_x = 15;
    peach->vel_y = 0;
    peach->jump_impulse = 50;
    peach->jumps_allowed = 2;
    peach->max_jumps = 2;
    peach->right = true;       
    peach->is_down = 0;
    peach->max_lives = 5;
	peach->num_lives = 5;
    peach->collide = 0;
    peach->collision_cooldown = 0;
    peach->transition_heart_timer = 0;
    peach->repel_force = 0;
    peach->repel_timer = 0;

    return;
}

void update_umbrella(unsigned char *key, peach_t *peach, int disp_x)
{
 
		if(key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_RIGHT] && key[ALLEGRO_KEY_UP] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2); 
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 4; 

		}
		else if (key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_UP] && key[ALLEGRO_KEY_LEFT] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2); 
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 4; 
		}
		else if ((key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_RIGHT] && !peach->umbrella.active) 
            || (key[ALLEGRO_KEY_W] && peach->is_down && !peach->umbrella.active))
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 1;
		}
		else if (key[ALLEGRO_KEY_DOWN] && key[ALLEGRO_KEY_RIGHT] && key[ALLEGRO_KEY_W] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2); 
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 5; 
		}
		else if (key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_UP] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2); 
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 2; 
		}
		else if (key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_DOWN] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2); 
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 3; 

		}
		else if (key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_LEFT] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 1;
		}
		else if (key[ALLEGRO_KEY_W] && key[ALLEGRO_KEY_RIGHT] && !peach->umbrella.active)
		{
      peach->umbrella.active = true;
      peach->umbrella.collide = false;
      peach->umbrella.draw_x = peach->x + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.draw_y = peach->y + (peach->size / 2) - (peach->umbrella.draw_width / 2);
      peach->umbrella.vel = 50; // velocidade de avanço
      peach->umbrella.state = 1;
		}

  
 
    if (peach->umbrella.active) 
    {
        switch (peach->umbrella.state)
        {
        case 1:
            
            if (peach->umbrella.right)
            {		
                peach->umbrella.draw_x += peach->umbrella.vel;
                peach->umbrella.angle = 0;
            }
            else 
            {
                peach->umbrella.draw_x -= peach->umbrella.vel;
                peach->umbrella.angle = ALLEGRO_PI;
            }
            break;
        
        case 2:
                peach->umbrella.draw_y -= peach->umbrella.vel;
                peach->umbrella.angle = -ALLEGRO_PI / 2; 
            break;
        case 3:
            
                peach->umbrella.draw_y += peach->umbrella.vel;
                peach->umbrella.angle = ALLEGRO_PI / 2; 
            break;

        case 4:
            if (peach->umbrella.right)
            {
                peach->umbrella.draw_x += peach->umbrella.vel;
                peach->umbrella.draw_y -= peach->umbrella.vel;
                peach->umbrella.angle = -ALLEGRO_PI / 4;
            }
            else 
            {
                peach->umbrella.draw_x -= peach->umbrella.vel;
                peach->umbrella.draw_y -= peach->umbrella.vel;
                peach->umbrella.angle = -3 * ALLEGRO_PI / 4;
            }

            break;
        case 5:
            if (peach->umbrella.right)
            {
                peach->umbrella.draw_x += peach->umbrella.vel;
                peach->umbrella.draw_y += peach->umbrella.vel;
                peach->umbrella.angle =  ALLEGRO_PI / 4;
            }
            else 
            {
                peach->umbrella.draw_x -= peach->umbrella.vel;
                peach->umbrella.draw_y += peach->umbrella.vel;
                peach->umbrella.angle = 3 * ALLEGRO_PI / 4;
            }
            break;

        default:
            break;
        
        }
    }

    // Desativa apos andar disp_x - peach.world_x
    if (abs(peach->umbrella.draw_x - peach->x) >= (disp_x - peach->umbrella.init_x) || abs(peach->umbrella.draw_y - peach->y) >= (disp_x - peach->umbrella.init_x))
    {
        peach->umbrella.active = false;
        peach->umbrella.state = 0;
    }

    float dx = peach->umbrella.draw_x - peach->x;
    float dy = peach->umbrella.draw_y - peach->y;
    if (sqrt(dx * dx + dy * dy) >= (disp_x - peach->umbrella.init_x))
    {
        peach->umbrella.active = false;
        peach->umbrella.state = 0;
    }

    if (peach->umbrella.collide) 
    {
        peach->umbrella.active = false;
        peach->umbrella.state = 0;
    }


    return;
}
 
void draw_umbrella(peach_t peach)
{
    if (peach.umbrella.active)
    {
        al_draw_scaled_rotated_bitmap(
            peach.umbrella.name,
            peach.umbrella.frame_width / 2.0, peach.umbrella.frame_height / 2.0, // c de rotacao
            peach.umbrella.draw_x + peach.umbrella.draw_width / 2.0, 
            peach.umbrella.draw_y + peach.umbrella.draw_height / 2.0, // destino
            peach.umbrella.draw_width / (float)peach.umbrella.frame_width, // escala x
            peach.umbrella.draw_height / (float)peach.umbrella.frame_height, // escala y
            peach.umbrella.angle,
            0);
    }

return;

}


int update_peach_boss(unsigned char *key, peach_t *peach, int *scroll_x, int floor)
{
    // Recuo após colisão
    if (peach->repel_timer > 0) {
        int direction = peach->right ? -1 : 1; // empurra no sentido oposto ao que estava indo
        peach->world_x += direction * peach->repel_force;
        peach->repel_timer--;
    
        peach->x = peach->world_x - *scroll_x;
    
        return 0; // pula a movimentação normal enquanto está sendo repelida
    }
   
		if (key[ALLEGRO_KEY_RIGHT]) 
    {
				if (peach->x + peach->vel_x < 1770)
        {
            peach->world_x += peach->vel_x;
        }

        peach->frame = (peach->frame + 1) % 4;
        peach->right = true;

				// Atualiza posição na tela com base no scroll
				peach->x = peach->world_x - *scroll_x;
    }
    else if (key[ALLEGRO_KEY_LEFT])
    {
        if (peach->world_x - peach->vel_x > 0)
        {
            peach->world_x -= peach->vel_x; 
        }

        peach->frame = (peach->frame + 1) % 4;
        peach->right = false;

				// Atualiza posição na tela com base no scroll
				peach->x = peach->world_x - *scroll_x;
    }
	
    if (!key[ALLEGRO_KEY_LEFT] && !key[ALLEGRO_KEY_RIGHT])
        peach->frame = 0;

    if (key[ALLEGRO_KEY_DOWN])
    {
        peach->is_down = true;
        peach->umbrella.active = false;
    } else
        peach->is_down = false;
        
    peach->vel_y += GRAVITY;
    peach->y += peach->vel_y;

		peach->umbrella.init_x = peach->x;
    
		if (peach->y >= floor) 
    {
        peach->y = floor;
        peach->vel_y = 0;
        peach->jumps_allowed = peach->max_jumps;
    }

    if (peach->collision_cooldown > 0)
    {
        peach->collision_cooldown--;
    }

    if (peach->transition_heart_timer > 0)
    {
        peach->transition_heart_timer--;
    }


		if (peach->collide && peach->collision_cooldown == 0)
    {   
        if (peach->num_lives > 0)
        {
            peach->num_lives--;
            peach->collision_cooldown = 20;
            peach->transition_heart_timer = 10;
    
            peach->repel_force = 10;
            peach->repel_timer = 5;
        }
        else if (peach->num_lives == 0)
        {
            return 1;        
        }
    }

    peach->collide = false;

    return 0;
}

int update_peach(unsigned char *key, peach_t *peach, int *scroll_x, int disp_x, int floor)
{
    // Recuo após colisão
    if (peach->repel_timer > 0) {
        int direction = peach->right ? -1 : 1; // empurra no sentido oposto ao que estava indo
        peach->world_x += direction * peach->repel_force;
        peach->repel_timer--;
    
        peach->x = peach->world_x - *scroll_x;
    
        return 0; // pula a movimentação normal enquanto está sendo repelida
    }
    
    if (key[ALLEGRO_KEY_RIGHT]) 
    {
        if ((*scroll_x != 5760) || (peach->world_x < 7500)) 
        {
            peach->world_x += peach->vel_x;
        
            // Se Peach passou do meio da tela e aind atem mundo
            if (peach->x >= disp_x/2 && (*scroll_x != 5760))
            {
                *scroll_x += peach->vel_x;
            }
        }

        peach->frame = (peach->frame + 1) % 4;
        peach->right = true;
    }
    else if (key[ALLEGRO_KEY_LEFT])
    {
        if (!wall_colision(*peach, -1)) 
        {
            peach->world_x -= peach->vel_x;

            // Se Peach passou do meio da tela indo para esquerda
            if ((peach->x <= disp_x/2) && (*scroll_x > 0))
            {
                *scroll_x -= peach->vel_x;
            }
        }

        peach->frame = (peach->frame + 1) % 4;
        peach->right = false;
    }

    // Atualiza posição na tela com base no scroll
    peach->x = peach->world_x - *scroll_x;

    if (!key[ALLEGRO_KEY_LEFT] && !key[ALLEGRO_KEY_RIGHT])
    {
        peach->frame = 0;
    }

    if (key[ALLEGRO_KEY_DOWN])
    {
        peach->is_down = true;
    } 
    else
    {
        peach->is_down = false;
    }   

    peach->vel_y += GRAVITY;
    peach->y += peach->vel_y;


		peach->umbrella.init_x = peach->x;

    if (peach->y >= floor) 
    {
        peach->y = floor;
        peach->vel_y = 0;
        peach->jumps_allowed = peach->max_jumps;
    }

    if (peach->collision_cooldown > 0)
    {
        peach->collision_cooldown--;
    }

    if (peach->transition_heart_timer > 0)
    {
        peach->transition_heart_timer--;
    }

	if (peach->collide && peach->collision_cooldown == 0)
		{   
        if (peach->num_lives > 0)
        {
            peach->num_lives--;
            peach->collision_cooldown = 20;
            peach->transition_heart_timer = 10;
    
            peach->repel_force = 10;
            peach->repel_timer = 5;
						peach->collide = false;

        }
        
        if (peach->num_lives == 0)
            return 1;        
			    
    }


    peach->collide = false;

    return 0;
}

void draw_peach(peach_t peach, int scroll_x)
{
    bitmap_t sprite = peach.walk[peach.frame];

    int peach_screen_x = peach.world_x - scroll_x;
 
    if (peach.is_down && peach.right)
    {
        al_draw_scaled_bitmap(
            peach.down.name,
            0, 0,
            peach.down.frame_width,
            peach.down.frame_height,
            peach_screen_x,
            peach.y +22,
            peach.size,
            peach.size,
            0
        );
    }
    else if (peach.is_down && !peach.right)
    {
        al_draw_scaled_bitmap(
            peach.down.name,
            0, 0,
            peach.down.frame_width,
            peach.down.frame_height,
            peach_screen_x + peach.size,
            peach.y +22,
            -peach.size,
            peach.size,
            0
        );
    }
    else if (!peach.is_down && peach.right)
    {
        al_draw_scaled_bitmap(
            sprite.name,
            0, 0,
            sprite.frame_width,
            sprite.frame_height,
            peach_screen_x,
            peach.y,
            peach.size,
            peach.size,
            0
        );
    }
    else if (!peach.is_down && !peach.right)
    {
        al_draw_scaled_bitmap(
            sprite.name,
            0, 0,
            sprite.frame_width,
            sprite.frame_height,
            peach_screen_x + peach.size, 
            peach.y,
            -peach.size,
            peach.size,
            0
        );
    }

    return;
}

void destroy_peach_bitmaps(peach_t *peach, ALLEGRO_BITMAP *sheet_peach)
{
    for (int i = 0; i < 4; i++)
    {
        if (peach->walk[i].name)
            al_destroy_bitmap(peach->walk[i].name);
    }

    if (peach->down.name)
        al_destroy_bitmap(peach->down.name);

    if (peach->umbrella.name)
        al_destroy_bitmap(peach->umbrella.name);

    if (sheet_peach)
        al_destroy_bitmap(sheet_peach);


    return;
}
