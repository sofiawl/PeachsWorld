
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

#define MENU 1
#define GAME_PLAY 2
#define GAME_OVER  3
#define EXIT 4
#define BOSS 5
#define DIFICULTY 6


typedef struct {
    ALLEGRO_BITMAP *name;
    bitmap_t type[3];
} heart_t;

void init_damage(heart_t *heart)
{
    heart->name = al_load_bitmap("asset/life.png");
    if (!heart->name) {printf("Erro ano inicializar sprite hearts"); return ;}


    heart->type[0].frame_x = 0;
    heart->type[0].frame_y = 0;
    heart->type[0].frame_width = 300;
    heart->type[0].frame_height = 300;

    heart->type[1].frame_x = 300;
    heart->type[1].frame_y = 0;
    heart->type[1].frame_width = 300;
    heart->type[1].frame_height = 300;

    heart->type[2].frame_x = 600;
    heart->type[2].frame_y = 0;
    heart->type[2].frame_width = 300;
    heart->type[2].frame_height = 300;

    int draw_width = 60;
    int draw_height = 60;

    for (int i = 0; i < 3; i++) {
        heart->type[i].draw_width = draw_width;
        heart->type[i].draw_height = draw_height;
    }

    return;
}

void draw_hearts(heart_t heart, peach_t peach) {
    int padding_x = 10;
    int padding_y = 10;
    int spacing = heart.type[0].draw_width + 5;

    for (int i = 0; i < peach.max_lives; i++) {
        int x = padding_x + i * spacing;
        int y = padding_y;

        int heart_type;

        if (i < peach.num_lives)
            heart_type = 0;
        else if (i == peach.num_lives && peach.transition_heart_timer > 0)
            heart_type = 1;
        else
            heart_type = 2;
            
        al_draw_scaled_bitmap(
            heart.name,
            heart.type[heart_type].frame_x,
            heart.type[heart_type].frame_y,
            heart.type[heart_type].frame_width,
            heart.type[heart_type].frame_height,
            x, y,
            heart.type[heart_type].draw_width,
            heart.type[heart_type].draw_height,
            0
        );
    }
}

bool peach_collide_enemy(peach_t peach, turtle_t enemy)
{
		if (enemy.dead) return false;

		if (peach.world_x > enemy.world_x + enemy.width)
        return false;
		if (peach.world_x + peach.size < enemy.world_x)
        return false;
		if (peach.y > enemy.y + enemy.height)
        return false;
		if (peach.y + peach.size < enemy.y)
        return false;


    return true;
}


bool umbrella_collide_enemy(peach_t *peach, turtle_t *enemy, int scroll_x)
{
    if (!peach->umbrella.active) return false;

    int umb_x = peach->umbrella.draw_x + scroll_x;
    int umb_y = peach->umbrella.draw_y;

    if (umb_x > enemy->world_x + enemy->width)
        return false;
    if (umb_x + peach->umbrella.draw_width < enemy->world_x)
        return false;
    if (umb_y > enemy->y + enemy->height)
        return false;
    if (umb_y + peach->umbrella.draw_height < enemy->y)
        return false;

    peach->umbrella.collide = true;
    return true;
}


bool projectile_collide_peach(peach_t peach, turtle_t enemy)
{
    if (!enemy.projectile.active || enemy.dead) 
        return false;

    int en_x = enemy.projectile.x;
    int en_y = enemy.projectile.y;

    if (peach.world_x > en_x + enemy.projectile.draw_size)
        return false;
    if (peach.world_x + peach.size < en_x)
        return false;
    if (peach.y > en_y + enemy.projectile.draw_size)
        return false;
    if (peach.y + peach.size < en_y)
        return false;

    return true;
}

bool umbrella_collide_boss(peach_t peach, boss_t boss, int scroll_x)
{
    if (!peach.umbrella.active) return false;
    
    int umb_x = peach.umbrella.draw_x + scroll_x; // corrigindo coordenada de tela para mundo
    int umb_y = peach.umbrella.draw_y;

    if (umb_x > boss.x + boss.size)
        return false;
    if (umb_x + peach.umbrella.draw_width < boss.x)
        return false;
    if (umb_y > boss.y + boss.size)
        return false;
    if (umb_y + peach.umbrella.draw_height < boss.y)
        return false;

    return true;
}


bool fire_collide_peach(peach_t peach, boss_t boss, int i)
{
		
    if (peach.world_x > boss.fire[i].x + boss.fire[i].draw_size)
        return false;
    if (peach.world_x + peach.size < boss.fire[i].x)
        return false;
    if (peach.y > boss.fire[i].y + boss.fire[i].draw_size)
        return false;
    if (peach.y + peach.size < boss.fire[i].y)
        return false;

    return true;
}


int main()
{

    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    if (!timer) { printf("Erro ao inicializar timer\n"); return 1; }
    
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) { printf("Erro ao inicializar queue\n"); return 1; }

    ALLEGRO_DISPLAY* disp; 
    ALLEGRO_DISPLAY_MODE mode;
    al_get_display_mode(0, &mode);
    int disp_x = mode.width; // 1920
    int disp_y = mode.height; // 1080p
    int floor = disp_y - 340;
    disp = al_create_display( mode.width, mode.height);
    if (!disp) { printf("Erro ao inicializar display\n"); return 1;}

    menu_t menu;
    ALLEGRO_FONT *font;
    init_menu(&menu, &font, disp_y);

    ALLEGRO_BITMAP *sheet_background;
    bitmap_t obs1;
    bitmap_t bg;
    init_scenary(&sheet_background, &obs1, &bg, disp_y, disp_x);
    
    heart_t hearts;
    init_damage(&hearts);
    
    ALLEGRO_BITMAP *sheet_peach;
    peach_t peach;
    init_peach(&sheet_peach, &peach, floor);
    

    ALLEGRO_BITMAP *sheet_enemies;
    turtle_t enemy0;
    init_turtle(&sheet_enemies, &enemy0);
    enemy0.world_x = 1000;
    enemy0.walk_from = 1000;
    enemy0.walk_to = 1400;
    enemy0.y = floor + 50;

    turtle_t enemy1 = enemy0;
    enemy1.world_x = 2400;
    enemy1.walk_from = 2400;
    enemy1.walk_to = 2800;
    
    turtle_t enemy2 = enemy0;
    enemy2.world_x = 3500;
    enemy2.walk_from = 3500;
    enemy2.walk_to = 3800;

    turtle_t enemy3 = enemy0;
    enemy3.world_x = 5000;
    enemy3.walk_from = 5000;
    enemy3.walk_to = 5400;

    turtle_t enemy4 = enemy0;
    enemy4.world_x = 6100;
    enemy4.walk_from = 6100;
    enemy4.walk_to = 6500;

    turtle_t enemy5 = enemy0;
    enemy5.world_x = 6800;
    enemy5.walk_from = 6800;
    enemy5.walk_to = 7200;

    enemy0.hard = 0;
    enemy1.hard = 0;
    enemy2.hard = 0;
    enemy3.hard = 0;
    enemy4.hard = 0;
    enemy5.hard = 0;

    boss_t boss;
    bitmap_t bg_boss;
    init_boss(&bg_boss, &boss);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    #define KEY_SEEN 1 //0000 0001
    #define KEY_DOWN 2 //0000 0010
    
    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    bool redraw = true;
    bool done = false;

    int state = MENU;
    bool winner;
    bool dificulty_selected = false;

    al_start_timer(timer);
    while (state != EXIT)
    {
        if (state == MENU)
        {
            al_wait_for_event(queue, &event);
            switch (event.type)
            {
                case ALLEGRO_EVENT_TIMER:    
                    update_menu_click(key, &menu, &state);
                    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        key[i] &= ~KEY_SEEN; // and 1111 1110
    
                    redraw = true;
                    break;
    
                case ALLEGRO_EVENT_MOUSE_AXES:
                    update_menu_mouse(&menu, &event);                      
                    redraw = true;
                    break;
                
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if (menu.select_color_play)
                    {
                        if (!dificulty_selected)
                        {
                            enemy0.hard = 1;
                            enemy1.hard = 1;
                            enemy2.hard = 1;
                            enemy3.hard = 1;
                            enemy4.hard = 1;
                            enemy5.hard = 1;
                        }

                        state = GAME_PLAY;
                    }
                    else if (menu.select_color_menu)
                        state = DIFICULTY; 
                    break;
    
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        done = true;
                    else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) 
                    {
                        if (menu.select_color_play)
                        {
                            if (!dificulty_selected)
                            {
                                enemy0.hard = 1;
                                enemy1.hard = 1;
                                enemy2.hard = 1;
                                enemy3.hard = 1;
                                enemy4.hard = 1;
                                enemy5.hard = 1;
                            }

                            state = GAME_PLAY;
                        }
                        else if (menu.select_color_menu)
                            state = DIFICULTY;
                    } 
                    else 
                        key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;

                    break;

    
                case ALLEGRO_EVENT_KEY_UP:
                    key[event.keyboard.keycode] &= ~KEY_DOWN; //
                    break;
    
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    state = EXIT;
                    break;
                
            }

            if (done)
                break;
    
            if (redraw && al_is_event_queue_empty(queue))
            {
                draw_menu(disp_x, disp_y, menu, font);
                redraw = false;
            }
    
        }

        if (state == DIFICULTY)
        {
            al_wait_for_event(queue, &event);
            switch (event.type)
            {
                case ALLEGRO_EVENT_TIMER:    
                    update_menu_click(key, &menu, &state);
                    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        key[i] &= ~KEY_SEEN; // and 1111 1110
    
                    redraw = true;
                    break;
    
                case ALLEGRO_EVENT_MOUSE_AXES:
                    update_menu_mouse(&menu, &event);                      
                    redraw = true;
                    break;
                
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if (menu.select_color_play)
                    {
                        enemy0.hard = 1;
                        enemy1.hard = 1;
                        enemy2.hard = 1;
                        enemy3.hard = 1;
                        enemy4.hard = 1;
                        enemy5.hard = 1;
                        dificulty_selected = true;
                        state = MENU;
                    }
                    else if (menu.select_color_menu)
                    {
                        enemy0.hard = 2;
                        enemy1.hard = 2;
                        enemy2.hard = 2;
                        enemy3.hard = 2;
                        enemy4.hard = 2;
                        enemy5.hard = 2;
                        dificulty_selected = true;
                        state = MENU;
                    } 
                    break;
    
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        done = true;
                    else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) 
                    {
                        if (menu.select_color_play)
                        {
                            enemy0.hard = 1;
                            enemy1.hard = 1;
                            enemy2.hard = 1;
                            enemy3.hard = 1;
                            enemy4.hard = 1;
                            enemy5.hard = 1;
                            dificulty_selected = true;
                            state = MENU;
                        }
                        else if (menu.select_color_menu)
                        {
                            enemy0.hard = 2;
                            enemy1.hard = 2;
                            enemy2.hard = 2;
                            enemy3.hard = 2;
                            enemy4.hard = 2;
                            enemy5.hard = 2;
                            dificulty_selected = true;
                            state = MENU;
                        } 
                    } 
                    else 
                        key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;  // 0000 0001 || 0000 0010 = 0000 0011
                    break;
    
                case ALLEGRO_EVENT_KEY_UP:
                    key[event.keyboard.keycode] &= ~KEY_DOWN; //
                    break;
    
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    state = MENU;
                    break;
                
            }

            if (done)
                break;
    
            if (redraw && al_is_event_queue_empty(queue))
            {
                draw_dificulty(disp_x, disp_y, menu, font);
                redraw = false;
            }
    
        }


        if (state == GAME_PLAY)
        {   
            int scroll_x = 0;

            while (state == GAME_PLAY)
            {
                al_wait_for_event(queue, &event);
                switch (event.type)
                {
                    case ALLEGRO_EVENT_TIMER:
                        if (key[ALLEGRO_KEY_ESCAPE])
                        {
                            enemy0.hard = 0;
                            enemy1.hard = 0;
                            enemy2.hard = 0;
                            enemy3.hard = 0;
                            enemy4.hard = 0;
                            enemy5.hard = 0;

                            state = MENU;
                            break; 
                        }
                        if (update_peach(key, &peach, &scroll_x, disp_x, floor))
                        {
                            winner = false;
                            state = GAME_OVER;
                        }

												if (!peach.umbrella.collide)
												{
													if (key[ALLEGRO_KEY_W])
													{
														peach.umbrella.init_x = peach.x;
														peach.umbrella.right = peach.right;
													}

													update_umbrella(key, &peach, disp_x);
												}
												else 
												{
													peach.umbrella.active = false;
													peach.umbrella.state = 0;
													peach.umbrella.collide = false;
												}

                        if (!enemy0.dead && !enemy0.hurt && umbrella_collide_enemy(&peach, &enemy0, scroll_x))
                        {
                            enemy0.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
						

                        if (!enemy1.dead && !enemy1.hurt && umbrella_collide_enemy(&peach, &enemy1, scroll_x))
                        {
                            enemy1.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
						

                        if (!enemy2.dead && !enemy2.hurt && umbrella_collide_enemy(&peach, &enemy2, scroll_x))
                        {
                            enemy2.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
                        

                        if (!enemy3.dead && !enemy3.hurt && umbrella_collide_enemy(&peach, &enemy3, scroll_x))
                        {
                            enemy3.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
						

                        if (!enemy4.dead && !enemy4.hurt && umbrella_collide_enemy(&peach, &enemy4, scroll_x))
                        {
                            enemy4.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
														
                        
                        if (!enemy5.dead && !enemy5.hurt && umbrella_collide_enemy(&peach, &enemy5, scroll_x)) 
                        {
                            enemy5.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
					
	                      update_turtle(&enemy0);
                        update_turtle(&enemy1);
                        update_turtle(&enemy2);
                        update_turtle(&enemy3);
                        update_turtle(&enemy4);
                        update_turtle(&enemy5);

						peach.collide = peach_collide_enemy(peach, enemy0)
                                        || peach_collide_enemy(peach, enemy1)
                                        || peach_collide_enemy(peach, enemy2) 
																				|| peach_collide_enemy(peach, enemy3) 
                                        || peach_collide_enemy(peach, enemy4)
                                        || peach_collide_enemy(peach, enemy5)
                                        || (projectile_collide_peach(peach, enemy0))
                                        || (projectile_collide_peach(peach, enemy1))
                                        || (projectile_collide_peach(peach, enemy2))
                                        || (projectile_collide_peach(peach, enemy3))
                                        || (projectile_collide_peach(peach, enemy4))
                                        || (projectile_collide_peach(peach, enemy5));
                        if (enemy0.dead && enemy1.dead && enemy2.dead && enemy3.dead && enemy4.dead && enemy5.dead)
                        {
                            state = BOSS;
                        }   
                        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                            key[i] &= ~KEY_SEEN; // and 1111 1110
                
                        redraw = true;
                        break;
                
                    case ALLEGRO_EVENT_KEY_DOWN:
                        key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN; // 0000 0001 || 0000 0010 = 0000 0011

                        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !peach.is_down && peach.jumps_allowed > 0)
                        {
                            peach.vel_y = -peach.jump_impulse;
                            peach.jumps_allowed--;
                        }
                        break;
                    case ALLEGRO_EVENT_KEY_UP:
                        key[event.keyboard.keycode] &= ~KEY_DOWN; //
                        break;
                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        done = true;
                        state = EXIT;
                        break;
                }

                if (done)
                    break;
                   
                if (redraw && al_is_event_queue_empty(queue))
                {

                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        draw_scenery(obs1, bg, scroll_x);
                        draw_hearts(hearts, peach);
                        draw_peach(peach, scroll_x);
                        draw_umbrella(peach);
                        if (!enemy0.dead) draw_turtle(enemy0, scroll_x);
                        if (!enemy1.dead) draw_turtle(enemy1, scroll_x);
                        if (!enemy2.dead) draw_turtle(enemy2, scroll_x);
                        if (!enemy3.dead) draw_turtle(enemy3, scroll_x);
                        if (!enemy4.dead) draw_turtle(enemy4, scroll_x);
                        if (!enemy5.dead) draw_turtle(enemy5, scroll_x);
                        al_flip_display();
                        redraw = false;
                }
            }
        }

        if (state == BOSS)
        {   
            int boss_intro_time = 3; // segundos
            float timer_speed = al_get_timer_speed(timer); 
            int frames_to_wait = (int)(boss_intro_time / timer_speed);
            int boss_intro_counter = 0;
            
            while (boss_intro_counter < frames_to_wait)
            {
                al_wait_for_event(queue, &event);
                if (event.type == ALLEGRO_EVENT_TIMER)
                {
                    boss_intro_counter++;

                    al_clear_to_color(al_map_rgb(0, 0, 0)); 
                    ALLEGRO_FONT *big_font = al_load_ttf_font("asset/SuperMario256.ttf", 300, 0);
                    if (!big_font) { printf("Erro ao inicializar font\n"); return 1; } 

                    al_draw_text(big_font, al_map_rgb(255, 255, 255), disp_x/2, disp_y/2 - 50,
                                ALLEGRO_ALIGN_CENTER, "BOSS");
                    al_flip_display();
                    al_destroy_font(big_font);
                }
                else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    done = true;
                    state = EXIT;
                    break;
                }
            }

            int scroll_x = 0;
            int boss_floor = disp_y - 290;
            reinit_peach(&peach, boss_floor);

            while (state == BOSS)
            {
                al_wait_for_event(queue, &event);
                switch (event.type)
                {
                    case ALLEGRO_EVENT_TIMER:
                        if (key[ALLEGRO_KEY_ESCAPE])
                        {
                            state = MENU;
                            break; 
                        }
                        if (update_peach_boss(key, &peach, &scroll_x, boss_floor))
                        {
                            winner = false;
                            state = GAME_OVER;
                        }

												if (!peach.umbrella.collide)
												{
													if (key[ALLEGRO_KEY_W])													
													{
														peach.umbrella.init_x = peach.x;
														peach.umbrella.right = peach.right;
													}

													update_umbrella(key, &peach, disp_x);
												}
												else 
												{
													peach.umbrella.active = false;
													peach.umbrella.state = 0;
													peach.umbrella.collide = false;
												}


                        update_boss(&boss, disp_x, disp_y);
                        peach.collide = false;
                        for (int i = 0; i < 5; i++) {
                            if (boss.fire[i].active && fire_collide_peach(peach, boss, i)) {
                                peach.collide = true;
                                break;
                            }
                        }
                        if (!boss.dead && umbrella_collide_boss(peach, boss, scroll_x))
                        {
                            boss.hurt = true;
                            peach.umbrella.active = false;
                            peach.umbrella.state = 0;
                        }
                        if (boss.dead)
                        {
                            winner = true;
                            state = GAME_OVER;
                        }    

                        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                            key[i] &= ~KEY_SEEN; // and 1111 1110
                
                        redraw = true;
                        break;
                
                    case ALLEGRO_EVENT_KEY_DOWN:
                        key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN; // 0000 0001 || 0000 0010 = 0000 0011

                        if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !peach.is_down && peach.jumps_allowed > 0)
                        {
                            peach.vel_y = -peach.jump_impulse;
                            peach.jumps_allowed--;
                        }
                        break;
                    case ALLEGRO_EVENT_KEY_UP:
                        key[event.keyboard.keycode] &= ~KEY_DOWN; //
                        break;
                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        done = true;
                        state = EXIT;
                        break;
                }

                if (done)
                    break;
                   
                if (redraw && al_is_event_queue_empty(queue))
                {
                        al_clear_to_color(al_map_rgb(107, 140, 255));
                        draw_backgroudn_boss(bg_boss, disp_x, disp_y);
                        draw_hearts(hearts, peach);
                        draw_peach(peach, scroll_x);
                        draw_umbrella(peach);
                        draw_boss(boss);
                        al_flip_display();
                        redraw = false;
                }
            }
        }


        if (state == GAME_OVER)
        {
            al_wait_for_event(queue, &event);
            switch (event.type)
            {
                case ALLEGRO_EVENT_TIMER:    
                    update_menu_click(key, &menu, &state);
                    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        key[i] &= ~KEY_SEEN; // and 1111 1110
    
                    redraw = true;
                    break;
    
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        done = true;
                    else 
                        key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN; // 0000 0001 || 0000 0010 = 0000 0011
                    break;
    
                case ALLEGRO_EVENT_KEY_UP:
                    key[event.keyboard.keycode] &= ~KEY_DOWN; //
                    break;
    
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    state = EXIT;
                    break;
                
            }

            if (done)
                break;
    
            if (redraw && al_is_event_queue_empty(queue))
            {
                draw_game_over(disp_x, disp_y, font, winner);
                al_flip_display();
                redraw = false;
            }
    
        }
    }

    
    destroy_peach_bitmaps(&peach, sheet_peach);
    destroy_menu_bitmaps(font);
    destroy_background_bitmaps(sheet_background, &obs1, &bg);
    destroy_boss_resources(&boss, &bg_boss);
    destroy_turtle_resources(&sheet_enemies, &enemy0);

    al_destroy_display(disp);   
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
