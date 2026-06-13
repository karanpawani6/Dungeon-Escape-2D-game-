#ifndef DOOR_H
#define DOOR_H
class Door {
public:
    double x, y;
    Door(double x, double y);
    void draw(bool allCoinsCollected);
    void reset();
};
#endif 