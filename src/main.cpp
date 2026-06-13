#include <iostream>
#include "player.h"
#include "enemy.h"
#include "platform.h"
#include "coins.h"
#include "door.h"
#include <vector>
#include <cstdlib>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
using namespace std;

void must_init(bool test, const char* description) {
    if (test) return;
    cout << "couldn't initialize " << description << endl;
    exit(1);
}

struct Level {
    vector<platform> platforms;
    vector<coin> coins;
    vector<Enemy> enemies;  
};

vector<Level> levels;
int currentLevel = 0;

bool check_collision(player& p, Enemy& e) {
    double p_left = p.get_X();
    double p_right = p_left + p.get_Width();
    double p_top = p.get_Y();
    double p_bottom = p_top + p.get_Height();

    double e_left = e.get_x();
    double e_right = e_left + e.get_width();
    double e_top = e.get_y();
    double e_bottom = e_top + e.get_height();

    return (p_left < e_right &&
        p_right > e_left &&
        p_top < e_bottom &&
        p_bottom > e_top);
}

void initialize_levels() {
    Level l1;
    l1.platforms = {
        platform(1140, 650, 1500, 700),
        platform(600, 500, 950, 550),
        platform(200, 300, 500, 350),
        platform(720, 150, 1030, 200)
    };
    l1.coins = {
        coin(1170, 625), coin(1450, 625), coin(1300, 625),
        coin(660, 475), coin(870, 475), coin(470, 275), coin(235, 275),
        coin(750,125), coin(1000,125)
    };
    l1.enemies.push_back(Enemy(750, 400, 100, 100, al_map_rgb(255, 0, 0)));

    Level l2;
    l2.platforms = {
        platform(100, 630, 400, 680),
        platform(500, 480, 800, 530),
        platform(900, 330, 1200, 380),
        platform(1300, 180, 1500, 230)
    };
    l2.coins = {
        coin(250, 605), coin(650, 455), coin(1050, 305),
        coin(1400, 155), coin(200, 280)
    };
    l2.enemies.push_back(Enemy(400, 300, 100, 100, al_map_rgb(255, 0, 0)));
    l2.enemies.push_back(Enemy(800, 500, 100, 100, al_map_rgb(255, 0, 0)));

    Level l3;
    l3.platforms = {
        platform(300, 650, 600, 700),
        platform(700, 450, 1000, 500),
        platform(1100, 250, 1400, 300)
    };
    l3.coins = {
        coin(450, 625), coin(850, 425), coin(1250, 225),
        coin(100, 700), coin(1400, 700)
    };
    l3.enemies.push_back(Enemy(100, 100, 100, 100, al_map_rgb(255, 0, 0)));
    l3.enemies.push_back(Enemy(700, 200, 100, 100, al_map_rgb(255, 0, 0)));
    l3.enemies.push_back(Enemy(1300, 300, 100, 100, al_map_rgb(255, 0, 0)));

    levels = { l1, l2, l3 };
}

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    const int screenwidth = 1500;
    const int screenheight = 800;

    ALLEGRO_DISPLAY* disp = al_create_display(screenwidth, screenheight);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    must_init(al_init_primitives_addon(), "primitives");

    ALLEGRO_BITMAP* object = al_load_bitmap("object.png");
    must_init(object, "hero");

    ALLEGRO_BITMAP* object_r = al_load_bitmap("object_r.png");
    must_init(object_r, "hero_r");

    initialize_levels();

    player b1(0, screenheight - 133, 101, 133);

    Door door(1400, 705);

    vector<Enemy> enemies = levels[currentLevel].enemies;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int frameW = 101;
    int frameH = 133;
    int counter = 0;

    ALLEGRO_BITMAP* image = object;

    bool done = false;
    bool redraw = true;
    bool game_over = false;
    ALLEGRO_EVENT event;

#define KEY_SEEN     1
#define KEY_DOWN     2
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (true) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (!game_over) {
                if (key[ALLEGRO_KEY_LEFT]) {
                    b1.set_X(-10);
                    image = object_r;
                    if (counter != 11) counter++;
                    else counter = 0;
                }

                if (key[ALLEGRO_KEY_RIGHT]) {
                    image = object;
                    b1.set_X(10);
                    if (counter != 11) counter++;
                    else counter = 0;
                }

                if (key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                b1.updateposition(screenwidth, screenheight,
                    levels[currentLevel].platforms,
                    levels[currentLevel].coins);

                for (auto& en : enemies) {
                    en.update(screenwidth, screenheight);
                    if (check_collision(b1, en)) {
                        game_over = true;
                    }
                }

                if (levels[currentLevel].coins.empty()) {
                    double p_left = b1.get_X();
                    double p_right = p_left + b1.get_Width();
                    double p_top = b1.get_Y();
                    double p_bottom = p_top + b1.get_Height();

                    double d_left = door.x;
                    double d_right = door.x + 80;
                    double d_top = door.y;
                    double d_bottom = door.y + 120;

                    if (p_left < d_right && p_right > d_left &&
                        p_top < d_bottom && p_bottom > d_top) {
                        if (currentLevel < levels.size() - 1) {
                            currentLevel++;

                            enemies = levels[currentLevel].enemies;
                        }
                        b1.reset(0, screenheight - frameH, frameW, frameH);
                    }
                }
            }
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
            if (game_over) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    game_over = false;
                    /*currentLevel = 0;*/
                    b1.reset(0, screenheight - frameH, frameW, frameH);
                    initialize_levels();
                    enemies = levels[currentLevel].enemies;
                }
            }
            else {
                if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                    b1.set_Y();
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= ~KEY_DOWN;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done) break;

        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (!game_over) {
                al_draw_bitmap_region(image, frameW * counter, 0,
                    frameW, frameH, b1.get_X(), b1.get_Y(), 0);

                for (auto& p : levels[currentLevel].platforms) {
                    al_draw_rectangle(p.x, p.y, p.endx, p.endy,
                        al_map_rgb(255, 255, 255), 5);
                }

                for (size_t i = 0; i < levels[currentLevel].coins.size(); i++) {
                    auto& c = levels[currentLevel].coins[i];
                    ALLEGRO_COLOR color = (i == levels[currentLevel].coins.size() - 1)
                        ? al_map_rgb(255, 165, 0)
                        : al_map_rgb(255, 215, 0);
                    al_draw_filled_circle(c.x, c.y, c.radius, color);
                }

                for (auto& en : enemies) {
                    en.draw();
                }

                door.draw(levels[currentLevel].coins.empty());

                al_draw_textf(font, al_map_rgb(255, 255, 255),
                    20, 20, 0, "Level: %d", currentLevel + 1);
                

                if (currentLevel == levels.size() - 1 &&
                    levels[currentLevel].coins.empty()) {
                    al_draw_text(font, al_map_rgb(0, 255, 0),
                        screenwidth / 2, screenheight / 2 - 40,
                        ALLEGRO_ALIGN_CENTER, "VICTORY!");
                }
            }
            else {
                al_draw_filled_rectangle(0, 0, screenwidth, screenheight,
                    al_map_rgba(0, 0, 0, 200));
                al_draw_text(font, al_map_rgb(255, 0, 0),
                    screenwidth / 2, screenheight / 2 - 40,
                    ALLEGRO_ALIGN_CENTER, "GAME OVER!");
                al_draw_text(font, al_map_rgb(255, 255, 255),
                    screenwidth / 2, screenheight / 2 + 20,
                    ALLEGRO_ALIGN_CENTER, "Press SPACE to restart");
            }

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_bitmap(object);
    al_destroy_bitmap(object_r);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
