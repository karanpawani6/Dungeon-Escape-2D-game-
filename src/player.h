#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "platform.h"
#include "coins.h"
using namespace std;
class player {
private:
    double x, y;
    double width, height;
    double velocityY = 0;
    int jumpCount = 0; 
    const double gravity = 5;

public:
    player(double startx, double starty, double objectw, double objecth);
    void updateposition(double screenwidth, double screenheight, vector<platform>& p1, vector<coin>& coins);
    void set_X(int movement);
    void set_Y();
    double get_X();
    double get_Y();
    double get_Width();
    double get_Height();
    void reset(double startx, double starty, double objectw, double objecth);
};

#endif