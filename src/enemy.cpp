#include "enemy.h"
#include <cmath>
#include <allegro5/allegro_primitives.h>

Enemy::Enemy(double startx, double starty, double w, double h, ALLEGRO_COLOR col) {
    x = startx;
    y = starty;
    dx = 10;
    dy = 10;
    width = w;
    height = h;
    color = col;
}

void Enemy::update(double screen_width, double screen_height) {

    x += dx;
    y += dy;

    if (x < 0) {
        dx = abs(dx);
    }
    if (x > screen_width - width) {
        dx = -abs(dx);
    }
    if (y < 0) {
        dy = abs(dy);
    }
    if (y > screen_height - height) {
        dy = -abs(dy);
    }
}

void Enemy::draw() {
    al_draw_filled_rectangle(x, y, x + width, y + height, color);
}

double Enemy::get_x() { return x; }
double Enemy::get_y() { return y; }
double Enemy::get_width() { return width; }
double Enemy::get_height() { return height; }

double Enemy::get_dx() { return dx; }
double Enemy::get_dy() { return dy; } 

void Enemy::reset(double startx, double starty) {
    x = startx;
    y = starty;
    dx = 10;
    dy = 10;
}
