#include "door.h"
#include <allegro5/allegro_primitives.h>
Door::Door(double x, double y) : x(x), y(y) {}
void Door::draw(bool allCoinsCollected) {
    if (allCoinsCollected) {
        al_draw_filled_rectangle(x, y, x + 80, y + 120, al_map_rgb(0, 200, 0));
    }
    else {
        al_draw_filled_rectangle(x, y, x + 80, y + 120, al_map_rgb(100, 50, 0));
    }
}
void Door::reset() {}