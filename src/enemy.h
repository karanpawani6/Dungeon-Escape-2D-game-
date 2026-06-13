#ifndef ENEMY_H
#define ENEMY_H
#include <allegro5/allegro.h>

class Enemy {
private:
    double x, y;
    double dx, dy;
    double width, height;
    ALLEGRO_COLOR color;

public:
    Enemy(double startx, double starty, double w, double h, ALLEGRO_COLOR col);
    void update(double screen_width, double screen_height);
    void draw();
    double get_x();
    double get_y();
    double get_width();
    double get_height();
    double get_dx();  
    double get_dy();  
    void reset(double startx, double starty);
};

#endif 
