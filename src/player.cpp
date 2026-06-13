#include "player.h"
player::player(double startx, double starty, double objectw, double objecth) {
    this->x = startx;
    this->y = starty;
    this->width = objectw;
    this->height = objecth;
}

void player::updateposition(double screenwidth, double screenheight, vector<platform>& p1, vector<coin>& coins) {

    if (x >= screenwidth - width) x = screenwidth - width;
    if (x <= 0) x = 0;

    double oldY = y;

    velocityY += gravity;
    y += velocityY;

    bool onPlatform = false;

    for (int i = 0; i < p1.size(); i++) {
        if (x + width >= p1[i].x && x <= p1[i].endx) {

            double oldBottom = oldY + height;
            double newBottom = y + height;

            if (oldBottom <= p1[i].y && newBottom >= p1[i].y) {

                y = p1[i].y - height;
                velocityY = 0;
                jumpCount = 0;
                onPlatform = true;
                break;
            }
        }
    }

    if (!onPlatform && y >= screenheight - height) {
        y = screenheight - height;
        velocityY = 0;
        jumpCount = 0;
    }

    for (int i = 0; i < coins.size(); i++) {
        double dx = (x + width / 2) - coins[i].x;
        double dy = (y + height / 2) - coins[i].y;
        double distance = sqrt(dx * dx + dy * dy);

        if (distance < coins[i].radius + width / 2) {
            coins.erase(coins.begin() + i);
            i--;
        }
    }
}

void player::set_X(int movement) {
    x += movement;
}

void player::set_Y() {
    if (jumpCount < 2) { 
        velocityY = -40;
        jumpCount++;
    }
}

double player::get_X() {
    return x;
}

double player::get_Y() {
    return y;
}

double player::get_Width() {
    return width;
}

double player::get_Height() {
    return height;
}

void player::reset(double startx, double starty, double objectw, double objecth) {
    x = startx;
    y = starty;
    width = objectw;
    height = objecth;
    velocityY = 0;
    jumpCount = 0;
}